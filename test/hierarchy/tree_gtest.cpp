#include <gtest_utils.h>

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/observer_tuple.h>
#include <scl/utility/hierarchy/tree.h>

#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

template <typename Tree>
struct counting_observer
{
public:
    using iterator = Tree::iterator;
    using const_iterator = Tree::const_iterator;
    using const_reference = Tree::const_payload_reference;

public:
    int inserted = 0;
    int erased = 0;
    int cleared = 0;
    int changed = 0;

public:
    constexpr void on_insert(iterator) { ++inserted; }
    constexpr void on_erase(const_iterator) { ++erased; }
    constexpr void on_clear() { ++cleared; }
    constexpr void on_change(const_reference, const_reference) { ++changed; }
};

using int_tree = scl::hierarchy::tree<int, counting_observer>;

// =============================================================================
// Compile-time contracts
// =============================================================================

/**
 * @test tree and its proxies satisfy the std::ranges concepts they advertise.
 */
TEST(HierarchyTreeTest, RangesCompliance)
{
    STATIC_EXPECT_TRUE(::std::ranges::range<int_tree>);
    STATIC_EXPECT_TRUE(::std::ranges::range<int_tree const>);
    STATIC_EXPECT_TRUE(::std::ranges::bidirectional_range<int_tree>);
    STATIC_EXPECT_TRUE(::std::ranges::sized_range<int_tree>);
    STATIC_EXPECT_TRUE(::std::ranges::range<int_tree::reference>);
    STATIC_EXPECT_TRUE(::std::ranges::range<int_tree::const_reference>);
    STATIC_EXPECT_TRUE(::std::ranges::bidirectional_range<int_tree::reference>);
    STATIC_EXPECT_TRUE(::std::ranges::bidirectional_range<int_tree::const_reference>);
}

// =============================================================================
// constexpr usability — requires constexpr std::list (C++23)
// =============================================================================
#ifdef __cpp_lib_constexpr_list
namespace
{
    // End-to-end constant evaluation: build, observe, clear — proves the tree and
    // its observer notifications are usable at compile time.
    constexpr bool test_tree_constexpr_usage()
    {
        int_tree t;
        auto root = t.push_back(1);
        (*root).push_back(2);
        bool const inserted = t.get_observer().inserted == 2;
        t.clear();
        return inserted && t.empty() && t.get_observer().cleared == 1;
    }
} // namespace

TEST(HierarchyTreeTest, ConstexprUsage) { STATIC_EXPECT_TRUE(test_tree_constexpr_usage()); }
#endif // __cpp_lib_constexpr_list

// =============================================================================
// Structure and notifications
// =============================================================================

/**
 * @test A freshly constructed tree is empty.
 */
TEST(HierarchyTreeTest, EmptyAfterConstruction)
{
    int_tree t;
    EXPECT_TRUE(t.empty());
}

/**
 * @test push_back() adds a root node and notifies the observer once.
 */
TEST(HierarchyTreeTest, PushBackRootNotifiesObserver)
{
    int_tree t;
    auto it = t.push_back(42);
    EXPECT_FALSE(t.empty());
    EXPECT_EQ((*it).value(), 42);
    EXPECT_EQ(t.get_observer().inserted, 1);
}

/**
 * @test Appending a child through a reference notifies the observer.
 */
TEST(HierarchyTreeTest, PushBackChildNotifiesObserver)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = (*root_it).push_back(2);
    EXPECT_EQ((*child_it).value(), 2);
    EXPECT_EQ(t.get_observer().inserted, 2);
}

/**
 * @test erase() at root level removes the node and notifies the observer.
 */
TEST(HierarchyTreeTest, EraseRootNotifiesObserver)
{
    int_tree t;
    auto it = t.push_back(10);
    t.erase(it);
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.get_observer().erased, 1);
}

/**
 * @test erase() through a reference removes a child and notifies the observer.
 */
TEST(HierarchyTreeTest, EraseChildNotifiesObserver)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = (*root_it).push_back(2);
    (*root_it).erase(child_it);
    EXPECT_TRUE((*root_it).empty());
    EXPECT_EQ(t.get_observer().erased, 1);
}

/**
 * @test Erasing a subtree notifies on_erase for every descendant, bottom-up.
 */
TEST(HierarchyTreeTest, EraseNotifiesDescendants)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = (*root_it).push_back(2);
    (*child_it).push_back(3); // grandchild

    (*root_it).erase(child_it);

    // erased: grandchild first, then child = 2 notifications.
    EXPECT_EQ(t.get_observer().erased, 2);
}

/**
 * @test clear() empties the tree and notifies the observer.
 */
TEST(HierarchyTreeTest, ClearNotifiesObserver)
{
    int_tree t;
    t.push_back(1);
    t.push_back(2);
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.get_observer().cleared, 1);
}

/**
 * @test set_value() through a reference updates the payload and fires on_change.
 */
TEST(HierarchyTreeTest, SetValueNotifiesObserver)
{
    int_tree t;
    auto it = t.push_back(1);
    (*it).set_value(99);
    EXPECT_EQ((*it).value(), 99);
    EXPECT_EQ(t.get_observer().changed, 1);
}

/**
 * @test take() detaches and returns a node, notifying the observer.
 */
TEST(HierarchyTreeTest, TakeNode)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = root_it->push_back(42);

    auto raw = root_it->take(child_it);
    EXPECT_EQ(raw.get(), 42);
    EXPECT_TRUE((*root_it).empty());
    EXPECT_EQ(t.get_observer().erased, 1);
}

/**
 * @test A tree payload can be built from a std::initializer_list.
 */
TEST(HierarchyTreeTest, EmplaceInitializerListPayload)
{
    scl::hierarchy::tree<std::vector<int>, counting_observer> t;
    auto it = t.emplace_back(std::initializer_list<int>{1, 2, 3});
    ASSERT_EQ((*it).value().size(), 3u);
    EXPECT_EQ((*it).value()[0], 1);
    EXPECT_EQ((*it).value()[2], 3);
    EXPECT_EQ(t.get_observer().inserted, 1);
}

// =============================================================================
// Hierarchy algorithm functions on reference (found via ADL, no unwrapping)
// =============================================================================

/**
 * @test The free hierarchy algorithm functions accept a reference directly.
 */
TEST(HierarchyTreeTest, AlgorithmFunctionsAcceptReferenceDirectly)
{
    int_tree t;
    auto root = t.push_back(1);
    auto child = (*root).push_back(2);
    auto grand = (*child).push_back(3);

    EXPECT_TRUE(scl::hierarchy::are_identical(scl::hierarchy::parent_of(*child), *root));
    EXPECT_TRUE(scl::hierarchy::has_parent(*child));
    EXPECT_FALSE(scl::hierarchy::has_parent(*root));
    EXPECT_TRUE(scl::hierarchy::is_parent_of(*root, *child));
    EXPECT_TRUE(scl::hierarchy::is_ancestor_of(*root, *grand));
    EXPECT_TRUE(scl::hierarchy::are_sibling(*child, *child));
    EXPECT_TRUE(scl::hierarchy::are_identical(*root, *root));
    EXPECT_FALSE(scl::hierarchy::are_identical(*root, *child));
}

// =============================================================================
// Reference identity and iteration
// =============================================================================

/**
 * @test reference equality compares node identity, not value.
 */
TEST(HierarchyTreeTest, ReferenceEqualityByIdentity)
{
    int_tree t;
    auto it1 = t.push_back(1);
    auto it2 = t.push_back(1); // same value, different node
    EXPECT_TRUE(*it1 == *it1);
    EXPECT_FALSE(*it1 == *it2);
}

/**
 * @test Iteration visits the root nodes in insertion order; push_front prepends.
 */
TEST(HierarchyTreeTest, RootIterationOrder)
{
    int_tree t;
    t.push_back(2);
    t.push_back(3);
    t.push_front(1);

    std::vector<int> values;
    for (auto it = t.begin(); it != t.end(); ++it)
        values.push_back((*it).value());

    std::vector<int> const expected{1, 2, 3};
    EXPECT_EQ(values, expected);
}

/**
 * @test Reverse iteration visits the root nodes from the last one back.
 */
TEST(HierarchyTreeTest, RootReverseIterationOrder)
{
    int_tree t;
    t.push_back(1);
    t.push_back(2);
    t.push_back(3);

    std::vector<int> values;
    for (auto it = t.rbegin(); it != t.rend(); ++it)
        values.push_back((*it).value());

    std::vector<int> const expected{3, 2, 1};
    EXPECT_EQ(values, expected);
}

/**
 * @test Reverse iteration over a reference visits its direct children from the last one back.
 */
TEST(HierarchyTreeTest, ReferenceReverseIterationOrder)
{
    int_tree t;
    auto root = t.push_back(0);
    root->push_back(1);
    root->push_back(2);

    std::vector<int> values;
    for (auto it = (*root).rbegin(); it != (*root).rend(); ++it)
        values.push_back((*it).value());

    std::vector<int> const expected{2, 1};
    EXPECT_EQ(values, expected);
}

/**
 * @test A reverse iterator of a mutable reference reaches a child that can be written to
 *       through `set_value`, firing `on_change` once.
 */
TEST(HierarchyTreeTest, ReferenceReverseIteratorWrites)
{
    int_tree t;
    auto root = t.push_back(0);
    root->push_back(1);
    root->push_back(2);

    (*(*root).rbegin()).set_value(99);

    EXPECT_EQ((*root).back().value(), 99);
    EXPECT_EQ(t.get_observer().changed, 1);
}

/**
 * @test The iterator aliases follow the proxy that hands them out: `reference` mutable,
 *       its `const` overloads, `crbegin()` and `const_reference` immutable, and
 *       `iterator_traits` agreeing with `operator*` for both iterators.
 */
TEST(HierarchyTreeTest, ReverseIteratorMutabilityFollowsTheProxy)
{
    STATIC_EXPECT_TRUE((std::is_same_v<int_tree::reference::reverse_iterator, int_tree::reverse_iterator>));
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(*std::declval<int_tree::reference &>().rbegin()),
        int_tree::reference>));
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(*std::declval<int_tree::reference const &>().rbegin()),
        int_tree::const_reference>));
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(*std::declval<int_tree::reference &>().crbegin()),
        int_tree::const_reference>));
    STATIC_EXPECT_TRUE((std::is_same_v<decltype(*std::declval<int_tree::const_reference &>().rbegin()),
        int_tree::const_reference>));

    STATIC_EXPECT_TRUE((std::is_same_v<std::iterator_traits<int_tree::iterator>::reference,
        decltype(*std::declval<int_tree::iterator &>())>));
    STATIC_EXPECT_TRUE((std::is_same_v<std::iterator_traits<int_tree::const_iterator>::reference,
        decltype(*std::declval<int_tree::const_iterator &>())>));

    int_tree t;
    auto root = t.push_back(0);
    root->push_back(1);
    EXPECT_EQ((*(*root).crbegin()).value(), 1);
}

/**
 * @test `operator->` reaches a proxy's members through a reverse iterator as it does
 *       through a forward one, and `iterator_traits` names what it hands back.
 */
TEST(HierarchyTreeTest, ArrowReachesTheProxyThroughAReverseIterator)
{
    STATIC_EXPECT_TRUE((std::is_same_v<std::iterator_traits<int_tree::iterator>::pointer,
        decltype(std::declval<int_tree::iterator const &>().operator->())>));
    STATIC_EXPECT_TRUE((std::is_same_v<std::iterator_traits<int_tree::const_iterator>::pointer,
        decltype(std::declval<int_tree::const_iterator const &>().operator->())>));

    int_tree t;
    auto root = t.push_back(0);
    root->push_back(1);
    root->push_back(2);

    auto reverse = (*root).rbegin();
    reverse->set_value(99);

    auto const const_reverse = (*root).crbegin();

    EXPECT_EQ((*root).back().value(), 99);
    EXPECT_EQ(const_reverse->value(), 99);
    EXPECT_EQ(t.get_observer().changed, 1);
}

// =============================================================================
// observer_tuple fan-out
// =============================================================================

// observer_tuple requires a template alias to break the circular dependency.
template <typename T>
using two_counters = scl::hierarchy::observer_tuple<T, counting_observer<T>, counting_observer<T>>;
using tuple_tree = scl::hierarchy::tree<int, two_counters>;

/**
 * @test observer_tuple fans out insert notifications to all inner observers.
 */
TEST(HierarchyTreeTest, ObserverTupleFansOutInsert)
{
    tuple_tree t;
    t.push_back(1);
    EXPECT_EQ(t.get_observer().get<0>().inserted, 1);
    EXPECT_EQ(t.get_observer().get<1>().inserted, 1);
}

/**
 * @test observer_tuple fans out erase notifications to all inner observers.
 */
TEST(HierarchyTreeTest, ObserverTupleFansOutErase)
{
    tuple_tree t;
    auto it = t.push_back(1);
    t.erase(it);
    EXPECT_EQ(t.get_observer().get<0>().erased, 1);
    EXPECT_EQ(t.get_observer().get<1>().erased, 1);
}
