/**
 * @example node_example.cpp
 * @brief Demonstrates scl::hierarchy::node<> with an in-memory organization chart.
 *
 * node<> owns a tree of typed children and tracks each child's parent
 * automatically — insertion sets the parent pointer, erase/take clears it.
 * This example builds a small org chart, queries it with the free algorithm
 * functions from algorithm.h, and reorganizes it with take() (detach +
 * reattach) and transfer() (zero-copy subtree move).
 */

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <string>
#include <utility>

using employee = ::scl::hierarchy::node<::std::string>;

namespace
{
    void print(employee const & who, int depth = 0)
    {
        ::std::cout << ::std::string(static_cast<::std::size_t>(depth) * 2, ' ') << who.get() << '\n';
        for (employee const & report : who)
            print(report, depth + 1);
    }
} // namespace

// ============================================================================
// Pattern 1 — build the chart and query relationships via ADL free functions
// ============================================================================

static employee build_chart()
{
    employee ceo{"CEO"};
    auto vp_engineering = ceo.emplace_back("VP Engineering");
    ceo.emplace_back("VP Sales");

    vp_engineering->emplace_back("Engineer A");
    vp_engineering->emplace_back("Engineer B");

    return ceo;
}

static void show_relationships(employee const & ceo)
{
    employee const & vp_engineering = ceo.front();
    employee const & engineer_a = vp_engineering.front();
    employee const & engineer_b = vp_engineering.back();

    using ::scl::hierarchy::are_sibling;
    using ::scl::hierarchy::is_ancestor_of;
    using ::scl::hierarchy::is_parent_of;

    assert(is_parent_of(vp_engineering, engineer_a)); // direct manager
    assert(!is_parent_of(ceo, engineer_a));           // CEO is a grandparent, not a manager
    assert(is_ancestor_of(ceo, engineer_a));          // ancestor covers any depth
    assert(are_sibling(engineer_a, engineer_b));

    ::std::cout << "Engineer A's direct manager: " << vp_engineering.get() << '\n';
    ::std::cout
        << "Engineer A and B share a manager: " << (are_sibling(engineer_a, engineer_b) ? "yes" : "no") << '\n';
}

// ============================================================================
// Pattern 2 — reorganize: take() detaches, transfer() moves a subtree in O(1)
// ============================================================================

static void reorganize(employee & ceo)
{
    auto vp_engineering = ceo.begin();
    auto vp_sales = ::std::next(vp_engineering);

    auto engineer_a = vp_engineering->begin();
    auto engineer_b = ::std::next(engineer_a);

    // Engineer A now reports directly to the CEO (detach, then reattach).
    employee promoted = vp_engineering->take(engineer_a);
    ceo.push_back(::std::move(promoted));

    // Engineer B (and any reports of theirs) moves under VP Sales — O(1)
    // splice, no copy, no allocation.
    vp_sales->transfer(*vp_engineering, engineer_b);

    assert(vp_engineering->empty()); // VP Engineering has no reports left
    assert(vp_sales->size() == 1);
    assert(ceo.back().get() == "Engineer A");
}

// ============================================================================
// main
// ============================================================================

int main(int, char **)
{
    employee ceo = build_chart();

    ::std::cout << "=== Initial org chart ===\n";
    print(ceo);

    ::std::cout << "\n=== Relationships ===\n";
    show_relationships(ceo);

    reorganize(ceo);

    ::std::cout << "\n=== After reorg ===\n";
    print(ceo);

    return {};
}
