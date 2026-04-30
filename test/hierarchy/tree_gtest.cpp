#include <gtest_utils.h>

#include <scl/utility/hierarchy/tree.h>

#include <string>
#include <vector>

template <typename Tree>
struct counting_observer
{
    int inserted = 0;
    int erased = 0;
    int cleared = 0;
    int changed = 0;

    void after_insert(typename Tree::node_wrapper_iterator) { ++inserted; }
    void before_erase(typename Tree::const_node_wrapper_iterator) { ++erased; }
    void before_clear() { ++cleared; }
    void on_change(typename Tree::const_reference_type, typename Tree::const_reference_type)
    {
        ++changed;
    }
};

using int_tree = scl::hierarchy::tree<int, counting_observer>;

/**
 * @test Freshly constructed tree is empty.
 */
TEST(HierarchyTreeTest, EmptyAfterConstruction)
{
    int_tree t;
    EXPECT_TRUE(t.empty());
}

/**
 * @test push_back() adds a root node and notifies the observer.
 */
TEST(HierarchyTreeTest, PushBackRootNotifiesObserver)
{
    int_tree t;
    auto it = t.push_back(42);
    EXPECT_FALSE(t.empty());
    EXPECT_EQ((*it).value(), 42);
    EXPECT_EQ(t.observer().inserted, 1);
}

/**
 * @test Appending a child via node_wrapper notifies the observer.
 */
TEST(HierarchyTreeTest, PushBackChildNotifiesObserver)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = (*root_it).push_back(2);
    EXPECT_EQ((*child_it).value(), 2);
    EXPECT_EQ(t.observer().inserted, 2);
}

/**
 * @test erase() at root level removes the node and notifies observer.
 */
TEST(HierarchyTreeTest, EraseRootNotifiesObserver)
{
    int_tree t;
    auto it = t.push_back(10);
    t.erase(it);
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.observer().erased, 1);
}

/**
 * @test erase() on node_wrapper removes child and notifies observer.
 */
TEST(HierarchyTreeTest, EraseChildNotifiesObserver)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = (*root_it).push_back(2);
    (*root_it).erase(child_it);
    EXPECT_TRUE((*root_it).empty());
    EXPECT_EQ(t.observer().erased, 1);
}

/**
 * @test clear() empties the tree and notifies observer.
 */
TEST(HierarchyTreeTest, ClearNotifiesObserver)
{
    int_tree t;
    t.push_back(1);
    t.push_back(2);
    t.clear();
    EXPECT_TRUE(t.empty());
    EXPECT_EQ(t.observer().cleared, 1);
}

/**
 * @test set_value() on node_wrapper updates payload and notifies observer.
 */
TEST(HierarchyTreeTest, SetValueNotifiesObserver)
{
    int_tree t;
    auto it = t.push_back(1);
    (*it).set_value(99);
    EXPECT_EQ((*it).value(), 99);
    EXPECT_EQ(t.observer().changed, 1);
}

/**
 * @test node_wrapper equality compares node identity, not value.
 */
TEST(HierarchyTreeTest, WrapperEqualityByIdentity)
{
    int_tree t;
    auto it1 = t.push_back(1);
    auto it2 = t.push_back(1); // same value, different node
    EXPECT_TRUE(*it1 == *it1);
    EXPECT_FALSE(*it1 == *it2);
}

/**
 * @test Iterator increment/decrement over root nodes.
 */
TEST(HierarchyTreeTest, IteratorTraversal)
{
    int_tree t;
    t.push_back(10);
    t.push_back(20);
    t.push_back(30);

    std::vector<int> values;
    for (auto it = t.begin(); it != t.end(); ++it)
        values.push_back((*it).value());

    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

/**
 * @test range-based for loop over tree roots.
 */
TEST(HierarchyTreeTest, RangeForOverRoots)
{
    int_tree t;
    t.push_back(1);
    t.push_back(2);

    int sum = 0;
    for (auto n : t)
        sum += n.value();
    EXPECT_EQ(sum, 3);
}

/**
 * @test push_front() adds a root node at the front.
 */
TEST(HierarchyTreeTest, PushFrontRoot)
{
    int_tree t;
    t.push_back(2);
    t.push_front(1);

    auto it = t.begin();
    EXPECT_EQ((*it).value(), 1);
    ++it;
    EXPECT_EQ((*it).value(), 2);
}

/**
 * @test release() deep-copies and removes a node, notifying observer.
 */
TEST(HierarchyTreeTest, ReleaseNode)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = root_it->push_back(42);

    auto raw = root_it->release(child_it);
    EXPECT_EQ(raw.value(), 42);
    EXPECT_TRUE((*root_it).empty());
    EXPECT_EQ(t.observer().erased, 1);
}

// tuple_of_observers requires a template alias to break the circular dependency.
template <typename T>
using two_counters = scl::hierarchy::tuple_of_observers<T, counting_observer<T>, counting_observer<T>>;
using tuple_tree = scl::hierarchy::tree<int, two_counters>;

/**
 * @test tuple_of_observers fans out insert notifications to all inner observers.
 */
TEST(HierarchyTreeTest, TupleOfObserversFansOutInsert)
{
    tuple_tree t;
    t.push_back(1);
    EXPECT_EQ(t.observer().get<0>().inserted, 1);
    EXPECT_EQ(t.observer().get<1>().inserted, 1);
}

/**
 * @test tuple_of_observers fans out erase notifications to all inner observers.
 */
TEST(HierarchyTreeTest, TupleOfObserversFansOutErase)
{
    tuple_tree t;
    auto it = t.push_back(1);
    t.erase(it);
    EXPECT_EQ(t.observer().get<0>().erased, 1);
    EXPECT_EQ(t.observer().get<1>().erased, 1);
}

/**
 * @test Erase on child via node_wrapper notifies observer for descendants too.
 */
TEST(HierarchyTreeTest, EraseNotifiesDescendants)
{
    int_tree t;
    auto root_it = t.push_back(1);
    auto child_it = (*root_it).push_back(2);
    (*child_it).push_back(3); // grandchild

    // Erasing child also fires before_erase for grandchild.
    (*root_it).erase(child_it);

    // erased: grandchild first, then child = 2 notifications.
    EXPECT_EQ(t.observer().erased, 2);
}
