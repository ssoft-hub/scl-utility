// Code-size counterpart to node_gbench.cpp: one externally visible wrapper per
// operation, so -ffunction-sections gives each its own section and arm-none-eabi-size
// reports what annotating the group costs in .text.
//
// Never linked or run. The wrappers exist to be compiled, not called.

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>
#include <scl/utility/hierarchy/tree.h>

#include <cstddef>

namespace scl::benchmarks
{

    using node = ::scl::hierarchy::node<int>;

    ::std::size_t walk_children(node const & parent)
    {
        ::std::size_t count = 0;
        for ([[maybe_unused]]
             auto const & child : parent)
            ++count;
        return count;
    }

    ::std::size_t walk_recursive(node const & parent)
    {
        ::std::size_t count = 1;
        for (auto const & child : parent)
            count += walk_recursive(child);
        return count;
    }

    ::std::size_t child_count(node const & parent) { return parent.size(); }

    bool is_empty(node const & parent) { return parent.empty(); }

    bool ancestor_of(node const & parent, node const & value)
    {
        return ::scl::hierarchy::is_ancestor_of(parent, value);
    }

    bool parent_of(node const & parent, node const & value)
    {
        return ::scl::hierarchy::is_parent_of(parent, value);
    }

    bool sibling_of(node const & first, node const & second)
    {
        return ::scl::hierarchy::are_sibling(first, second);
    }

    void add_child(node & parent, int payload) { parent.emplace_back(payload); }

    // The tree wrapper adds the observer notification to every mutation, so it carries
    // its own wrappers rather than being read off the node ones.
    template <typename Tree>
    struct silent_observer
    {
    public:
        using iterator = Tree::iterator;
        using const_iterator = Tree::const_iterator;
        using const_reference = Tree::const_payload_reference;

    public:
        constexpr void on_insert(iterator) {}
        constexpr void on_erase(const_iterator) {}
        constexpr void on_clear() {}
        constexpr void on_change(const_reference, const_reference) {}
    };

    using int_tree = ::scl::hierarchy::tree<int, silent_observer>;

    ::std::size_t tree_walk_children(int_tree const & tree)
    {
        ::std::size_t count = 0;
        for (auto it = tree.begin(); it != tree.end(); ++it)
            ++count;
        return count;
    }

    ::std::size_t tree_child_count(int_tree const & tree) { return tree.size(); }

    void tree_add_child(int_tree & tree, int payload) { tree.push_back(payload); }

    void tree_remove_last(int_tree & tree) { tree.pop_back(); }

} // namespace scl::benchmarks
