#include <gtest_utils.h>

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <string>

using Node = scl::hierarchy::node<int>;
using StringNode = scl::hierarchy::node<std::string>;

/**
 * @test Default-constructed node has no parent and is empty.
 */
TEST(HierarchyNodeTest, DefaultConstructedIsEmpty)
{
    Node n;
    EXPECT_TRUE(n.empty());
    EXPECT_EQ(n.size(), 0u);
}

/**
 * @test Node constructed with a value stores that value.
 */
TEST(HierarchyNodeTest, ValueConstructor)
{
    Node n(42);
    EXPECT_EQ(n.value(), 42);
}

/**
 * @test push_back() adds a child and returns a valid iterator to it.
 */
TEST(HierarchyNodeTest, PushBackAddsChild)
{
    Node parent;
    auto it = parent.push_back(10);
    EXPECT_FALSE(parent.empty());
    EXPECT_EQ(parent.size(), 1u);
    EXPECT_EQ(it->value(), 10);
}

/**
 * @test push_front() adds a child at the front.
 */
TEST(HierarchyNodeTest, PushFrontAddsChildAtFront)
{
    Node parent;
    parent.push_back(2);
    parent.push_front(1);
    EXPECT_EQ(parent.front().value(), 1);
    EXPECT_EQ(parent.back().value(), 2);
}

/**
 * @test emplace() inserts a child at the specified position.
 */
TEST(HierarchyNodeTest, EmplaceAtPosition)
{
    Node parent;
    parent.push_back(1);
    parent.push_back(3);
    auto mid = parent.begin();
    ++mid;
    parent.emplace(mid, 2);

    auto it = parent.begin();
    EXPECT_EQ(it->value(), 1);
    ++it;
    EXPECT_EQ(it->value(), 2);
    ++it;
    EXPECT_EQ(it->value(), 3);
}

/**
 * @test Appended child reports the correct parent.
 */
TEST(HierarchyNodeTest, ChildHasCorrectParent)
{
    Node parent(99);
    auto it = parent.push_back(1);
    EXPECT_TRUE(scl::hierarchy::has_parent(*it));
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *it));
}

/**
 * @test erase() removes child and returns valid iterator.
 */
TEST(HierarchyNodeTest, EraseRemovesChild)
{
    Node parent;
    parent.push_back(1);
    auto it2 = parent.push_back(2);
    parent.push_back(3);

    parent.erase(it2);
    EXPECT_EQ(parent.size(), 2u);
    EXPECT_EQ(parent.front().value(), 1);
    EXPECT_EQ(parent.back().value(), 3);
}

/**
 * @test clear() removes all children.
 */
TEST(HierarchyNodeTest, ClearRemovesAll)
{
    Node parent;
    parent.push_back(1);
    parent.push_back(2);
    parent.push_back(3);
    parent.clear();
    EXPECT_TRUE(parent.empty());
}

/**
 * @test take_first() deep-copies and removes the first child.
 */
TEST(HierarchyNodeTest, TakeFirstExtractsChild)
{
    Node parent;
    parent.push_back(10);
    parent.push_back(20);

    Node taken = parent.take_first();
    EXPECT_EQ(taken.value(), 10);
    EXPECT_EQ(parent.size(), 1u);
    EXPECT_EQ(parent.front().value(), 20);
}

/**
 * @test take_last() deep-copies and removes the last child.
 */
TEST(HierarchyNodeTest, TakeLastExtractsChild)
{
    Node parent;
    parent.push_back(10);
    parent.push_back(20);

    Node taken = parent.take_last();
    EXPECT_EQ(taken.value(), 20);
    EXPECT_EQ(parent.size(), 1u);
    EXPECT_EQ(parent.front().value(), 10);
}

/**
 * @test Copy constructor deep-copies descendants.
 */
TEST(HierarchyNodeTest, CopyConstructorDeepCopies)
{
    Node src(1);
    src.push_back(2);
    src.front().push_back(3);

    Node copy = src;
    EXPECT_EQ(copy.value(), 1);
    EXPECT_EQ(copy.size(), 1u);
    EXPECT_EQ(copy.front().value(), 2);
    EXPECT_EQ(copy.front().size(), 1u);
    EXPECT_EQ(copy.front().front().value(), 3);
}

/**
 * @test Move constructor transfers children, updating their parent pointers.
 */
TEST(HierarchyNodeTest, MoveConstructorTransfersChildren)
{
    Node src(1);
    src.push_back(2);
    src.push_back(3);

    Node dst = std::move(src);
    EXPECT_EQ(dst.size(), 2u);
    for (auto & child : dst)
        EXPECT_TRUE(scl::hierarchy::is_parent_of(dst, child));
}

/**
 * @test transfer() moves all children from one node to another.
 */
TEST(HierarchyNodeTest, TransferAllChildren)
{
    Node src;
    src.push_back(1);
    src.push_back(2);

    Node dst;
    dst.transfer(src);

    EXPECT_TRUE(src.empty());
    EXPECT_EQ(dst.size(), 2u);
    for (auto & child : dst)
        EXPECT_TRUE(scl::hierarchy::is_parent_of(dst, child));
}

/**
 * @test transfer() with single iterator moves one child.
 */
TEST(HierarchyNodeTest, TransferSingleChild)
{
    Node src;
    src.push_back(1);
    src.push_back(2);

    Node dst;
    dst.transfer(src, src.cbegin());

    EXPECT_EQ(src.size(), 1u);
    EXPECT_EQ(dst.size(), 1u);
    EXPECT_EQ(dst.front().value(), 1);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(dst, dst.front()));
}

/**
 * @test Range-based for loop iterates over direct children only.
 */
TEST(HierarchyNodeTest, RangeForIteratesDirectChildren)
{
    Node parent;
    parent.push_back(10);
    parent.push_back(20);
    parent.push_back(30);

    std::vector<int> values;
    for (auto & child : parent)
        values.push_back(child.value());

    ASSERT_EQ(values.size(), 3u);
    EXPECT_EQ(values[0], 10);
    EXPECT_EQ(values[1], 20);
    EXPECT_EQ(values[2], 30);
}

/**
 * @test front() and back() access first and last child.
 */
TEST(HierarchyNodeTest, FrontBack)
{
    Node parent;
    parent.push_back(1);
    parent.push_back(2);
    EXPECT_EQ(parent.front().value(), 1);
    EXPECT_EQ(parent.back().value(), 2);
}
