/**
 * @example tree_example.cpp
 * @brief Demonstrates scl::hierarchy::tree<> with composed observers keeping
 *        two independent indexes in sync across structural moves.
 *
 * tree<> routes every structural or payload mutation through an Observer
 * policy, and observer_tuple<> lets that policy be several independent
 * observers instead of one. This example maintains a task board — tasks can
 * have subtasks — with two observers composed via observer_tuple:
 *   - assignee_index_observer groups task titles by assignee.
 *   - status_count_observer counts tasks by status ("todo", "in-progress", ...).
 * Reassigning or re-statusing a task (set_value) updates both indexes via
 * on_change. Moving a whole subtask subtree to a different parent
 * (reference::transfer) updates both indexes too — on_erase/on_insert
 * cascade through every moved descendant, not just the node named in the
 * call, so the indexes never go stale.
 */

#include <scl/utility/hierarchy/observer_tuple.h>
#include <scl/utility/hierarchy/tree.h>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace
{
    struct task
    {
        ::std::string title;
        ::std::string assignee;
        ::std::string status;
    };

    template <typename Tree>
    struct assignee_index_observer
    {
        ::std::map<::std::string, ::std::vector<::std::string>> by_assignee;

        void on_insert(typename Tree::iterator it)
        {
            by_assignee[it->value().assignee].push_back(it->value().title);
        }

        void on_erase(typename Tree::const_iterator it)
        {
            remove_title(by_assignee[it->value().assignee], it->value().title);
        }

        void on_clear() { by_assignee.clear(); }

        void on_change(typename Tree::const_payload_reference before, /**/
            typename Tree::const_payload_reference after)
        {
            if (before.assignee == after.assignee)
                return;
            remove_title(by_assignee[before.assignee], before.title);
            by_assignee[after.assignee].push_back(after.title);
        }

    private:
        static void remove_title(::std::vector<::std::string> & titles, ::std::string const & title)
        {
            titles.erase(::std::remove(titles.begin(), titles.end(), title), titles.end());
        }
    };

    template <typename Tree>
    struct status_count_observer
    {
        ::std::map<::std::string, int> by_status;

        void on_insert(typename Tree::iterator it) { ++by_status[it->value().status]; }
        void on_erase(typename Tree::const_iterator it) { --by_status[it->value().status]; }
        void on_clear() { by_status.clear(); }

        void on_change(typename Tree::const_payload_reference before, /**/
            typename Tree::const_payload_reference after)
        {
            if (before.status == after.status)
                return;
            --by_status[before.status];
            ++by_status[after.status];
        }
    };

    // observer_tuple fans every notification out to both inner observers, in
    // declaration order — tree sees one Observer type, this example gets two
    // independently maintained indexes for free.
    template <typename Tree>
    using board_observer =
        ::scl::hierarchy::observer_tuple<Tree, assignee_index_observer<Tree>, status_count_observer<Tree>>;

    using board = ::scl::hierarchy::tree<task, board_observer>;

    void print_tree(board::const_reference who, int depth = 0)
    {
        task const & value = who.value();
        ::std::cout
            << ::std::string(static_cast<::std::size_t>(depth) * 2, ' ') << value.title << " ["
            << value.assignee << ", " << value.status << "]\n";
        for (board::const_reference child : who)
            print_tree(child, depth + 1);
    }

    void print_board(board const & tasks)
    {
        for (board::const_reference root : tasks)
            print_tree(root);
    }

    void print_indexes(board const & tasks)
    {
        auto const & by_assignee = tasks.get_observer().get<assignee_index_observer<board>>().by_assignee;
        ::std::cout << "  by assignee:\n";
        for (auto const & [assignee, titles] : by_assignee)
        {
            if (titles.empty())
                continue;
            ::std::cout << "    " << assignee << ":";
            for (::std::string const & title : titles)
                ::std::cout << " \"" << title << "\"";
            ::std::cout << '\n';
        }

        auto const & by_status = tasks.get_observer().get<1>().by_status;
        ::std::cout << "  by status:\n";
        for (auto const & [status, count] : by_status)
            if (count > 0)
                ::std::cout << "    " << status << ": " << count << '\n';
    }

    ::std::size_t total_tasks(board const & tasks)
    {
        ::std::size_t total = 0;
        for (auto const & [status, count] : tasks.get_observer().get<1>().by_status)
            total += count;
        return total;
    }
} // namespace

int main(int, char **)
{
    board tasks;

    auto launch = tasks.push_back(task{"Launch v2", "Alice", "todo"});
    launch->push_back(task{"Design mockups", "Alice", "todo"});
    auto docs = launch->push_back(task{"Write API docs", "Alice", "todo"});
    docs->push_back(task{"Proofread docs", "Carol", "todo"});

    auto bug = tasks.push_back(task{"Fix login bug", "Bob", "in-progress"});

    ::std::cout << "=== Initial board ===\n";
    print_board(tasks);
    print_indexes(tasks);
    auto const total_before = total_tasks(tasks);

    // Reassign "Design mockups" -- on_change updates the assignee index.
    launch->begin()->set_value(task{"Design mockups", "Bob", "todo"});

    // "Fix login bug" is done -- on_change updates the status index.
    bug->set_value(task{"Fix login bug", "Bob", "done"});

    ::std::cout << "\n=== After reassigning and closing a task ===\n";
    print_board(tasks);
    print_indexes(tasks);

    // Move "Write API docs" -- together with its subtask "Proofread docs" --
    // from under "Launch v2" to under "Fix login bug". transfer() is O(1)
    // (splice, no copy); on_erase/on_insert still fire for both moved nodes,
    // so both indexes stay correct without walking the subtree by hand.
    bug->transfer(*launch, docs);

    ::std::cout << "\n=== After moving a subtask subtree across parents ===\n";
    print_board(tasks);
    print_indexes(tasks);

    assert(total_tasks(tasks) == total_before); // the move touched no task's status

    return {};
}
