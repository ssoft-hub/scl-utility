#include <gtest_utils.h>

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <algorithm>
#include <vector>

using Node = scl::hierarchy::node<int>;

struct Point
{
    int x = 0;
    int y = 0;
    bool operator==(Point const &) const = default;
};
using PointNode = scl::hierarchy::node<Point>;

// =============================================================================
// Compile-time contracts
// =============================================================================

/**
 * @test Node and its proxies satisfy the std::ranges concepts they advertise.
 */
TEST(HierarchyNodeTest, RangesCompliance)
{
    STATIC_EXPECT_TRUE(::std::ranges::range<Node>);
    STATIC_EXPECT_TRUE(::std::ranges::range<Node const>);
    STATIC_EXPECT_TRUE(::std::ranges::bidirectional_range<Node>);
    STATIC_EXPECT_TRUE(::std::ranges::sized_range<Node>);
    STATIC_EXPECT_TRUE(::std::ranges::common_range<Node>);
}

// =============================================================================
// constexpr usability — requires constexpr std::list (C++23)
// =============================================================================
#ifdef __cpp_lib_constexpr_list
namespace
{
    // End-to-end constant evaluation: construct, insert, extract. Proves node's
    // operations are usable at compile time where the standard library provides
    // constexpr std::list.
    constexpr bool test_node_constexpr_usage()
    {
        Node n{1};
        n.push_back(2);
        n.push_back(3);
        Node taken = n.take_first();
        return n.size() == 1u && n.front().get() == 3 && taken.get() == 2 && !taken.has_parent();
    }
} // namespace

TEST(HierarchyNodeTest, ConstexprUsage) { STATIC_EXPECT_TRUE(test_node_constexpr_usage()); }
#endif // __cpp_lib_constexpr_list

// =============================================================================
// Construction and value access
// =============================================================================

/**
 * @test Default-constructed node is empty, valueless-default, and rootless.
 */
TEST(HierarchyNodeTest, DefaultConstructed)
{
    Node n{};
    EXPECT_TRUE(n.empty());
    EXPECT_EQ(n.size(), 0u);
    EXPECT_FALSE(n.has_parent());
}

/**
 * @test A node constructed with a value stores it.
 */
TEST(HierarchyNodeTest, ValueConstructor)
{
    Node n{42};
    EXPECT_EQ(n.get(), 42);
}

// =============================================================================
// Insertion
// =============================================================================

/**
 * @test emplace_back/emplace_front place children at the ends and set their parent.
 */
TEST(HierarchyNodeTest, EmplaceBackFront)
{
    Node parent{0};
    auto back = parent.emplace_back(2);
    auto front = parent.emplace_front(1);

    EXPECT_EQ(parent.size(), 2u);
    EXPECT_EQ(parent.front().get(), 1);
    EXPECT_EQ(parent.back().get(), 2);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *front));
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *back));
}

/**
 * @test emplace() inserts a child at an explicit position.
 */
TEST(HierarchyNodeTest, EmplaceAtPosition)
{
    Node parent;
    parent.emplace_back(1);
    parent.emplace_back(3);
    parent.emplace(std::next(parent.cbegin()), 2);

    auto it = parent.begin();
    EXPECT_EQ((it++)->get(), 1);
    EXPECT_EQ((it++)->get(), 2);
    EXPECT_EQ(it->get(), 3);
}

/**
 * @test emplace_back constructs an aggregate payload in place from several args.
 */
TEST(HierarchyNodeTest, EmplaceBackAggregate)
{
    PointNode parent;
    auto it = parent.emplace_back(3, 4);
    Point const expected{3, 4};
    EXPECT_EQ(it->get(), expected);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *it));
}

/**
 * @test emplace_back builds a child payload from a std::initializer_list.
 */
TEST(HierarchyNodeTest, EmplaceBackInitializerListPayload)
{
    scl::hierarchy::node<std::vector<int>> parent;
    auto it = parent.emplace_back(std::initializer_list<int>{4, 5});
    ASSERT_EQ(it->get().size(), 2u);
    EXPECT_EQ(it->get()[0], 4);
    EXPECT_EQ(it->get()[1], 5);
}

/**
 * @test push_back(node) deep-copies a whole subtree as a new child.
 */
TEST(HierarchyNodeTest, PushBackCopiesSubtree)
{
    Node child{10};
    child.emplace_back(11);

    Node parent{0};
    auto it = parent.push_back(child);
    EXPECT_EQ(it->get(), 10);
    EXPECT_EQ(it->size(), 1u);
    EXPECT_EQ(it->front().get(), 11);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *it));
    EXPECT_TRUE(scl::hierarchy::is_parent_of(*it, it->front()));
    EXPECT_FALSE(child.empty()); // source untouched by copy
}

/**
 * @test push_back(std::move(node)) transfers a subtree without copying.
 */
TEST(HierarchyNodeTest, PushBackMovesSubtree)
{
    Node child{20};
    child.emplace_back(21);
    child.emplace_back(22);

    Node parent{0};
    auto it = parent.push_back(std::move(child));
    EXPECT_EQ(it->get(), 20);
    EXPECT_EQ(it->size(), 2u);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(*it, it->front()));
    EXPECT_TRUE(scl::hierarchy::is_parent_of(*it, it->back()));
}

/**
 * @test insert(where, node) inserts a copy at the specified position.
 */
TEST(HierarchyNodeTest, InsertNodeAtPosition)
{
    Node parent{0};
    parent.emplace_back(1);
    parent.emplace_back(3);
    parent.insert(std::next(parent.cbegin()), Node{2});

    auto it = parent.begin();
    EXPECT_EQ((it++)->get(), 1);
    EXPECT_EQ((it++)->get(), 2);
    EXPECT_EQ(it->get(), 3);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *std::next(parent.begin())));
}

// =============================================================================
// Removal
// =============================================================================

/**
 * @test erase(position) removes one child; erase(first, last) removes a range.
 */
TEST(HierarchyNodeTest, Erase)
{
    Node parent;
    auto it2 = parent.emplace_back(1);
    parent.emplace_back(2);
    parent.emplace_back(3);
    parent.emplace_back(4);

    parent.erase(it2); // remove 1
    EXPECT_EQ(parent.size(), 3u);
    EXPECT_EQ(parent.front().get(), 2);

    auto first = std::next(parent.cbegin()); // 3
    parent.erase(first, parent.cend());      // remove 3, 4
    EXPECT_EQ(parent.size(), 1u);
    EXPECT_EQ(parent.front().get(), 2);
}

/**
 * @test pop_front/pop_back remove the end children.
 */
TEST(HierarchyNodeTest, PopFrontBack)
{
    Node parent;
    parent.emplace_back(1);
    parent.emplace_back(2);
    parent.emplace_back(3);

    parent.pop_front();
    parent.pop_back();
    EXPECT_EQ(parent.size(), 1u);
    EXPECT_EQ(parent.front().get(), 2);
}

/**
 * @test clear() removes all children.
 */
TEST(HierarchyNodeTest, ClearRemovesAll)
{
    Node parent;
    parent.emplace_back(1);
    parent.emplace_back(2);
    parent.clear();
    EXPECT_TRUE(parent.empty());
}

// =============================================================================
// Extraction
// =============================================================================

/**
 * @test take(position) detaches the child, clears its parent, and keeps siblings.
 */
TEST(HierarchyNodeTest, TakeDetaches)
{
    Node parent;
    parent.emplace_back(1);
    parent.emplace_back(2);
    parent.emplace_back(3);

    Node taken = parent.take(std::next(parent.cbegin()));
    EXPECT_EQ(taken.get(), 2);
    EXPECT_FALSE(taken.has_parent());
    EXPECT_EQ(parent.size(), 2u);
    EXPECT_EQ(parent.front().get(), 1);
    EXPECT_EQ(parent.back().get(), 3);
}

/**
 * @test take() preserves the extracted subtree and its inner parent pointers.
 */
TEST(HierarchyNodeTest, TakePreservesSubtree)
{
    Node parent;
    auto child_it = parent.emplace_back(10);
    child_it->emplace_back(11);
    child_it->emplace_back(12);

    Node taken = parent.take(parent.cbegin());
    EXPECT_FALSE(taken.has_parent());
    EXPECT_EQ(taken.size(), 2u);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(taken, taken.front()));
    EXPECT_TRUE(scl::hierarchy::is_parent_of(taken, taken.back()));
    EXPECT_TRUE(parent.empty());
}

// =============================================================================
// Copy and move semantics
// =============================================================================

/**
 * @test Copy construction deep-copies descendants and reparents them into the copy.
 */
TEST(HierarchyNodeTest, CopyConstructorDeepCopies)
{
    Node src{1};
    src.emplace_back(2);
    src.front().emplace_back(3);

    Node copy = src;
    EXPECT_EQ(copy.get(), 1);
    EXPECT_EQ(copy.front().get(), 2);
    EXPECT_EQ(copy.front().front().get(), 3);
    EXPECT_EQ(&copy.front().parent(), &copy); // parent points into the copy
    EXPECT_NE(&copy.front().parent(), &src);  // not into the source
}

/**
 * @test Move construction transfers children and updates their parent pointers.
 */
TEST(HierarchyNodeTest, MoveConstructorTransfersChildren)
{
    Node src{1};
    src.emplace_back(2);
    src.emplace_back(3);

    Node dst = std::move(src);
    EXPECT_EQ(dst.size(), 2u);
    EXPECT_TRUE(std::ranges::all_of(dst, [&dst](auto & child) {
        return scl::hierarchy::is_parent_of(dst, child);
    }));
}

// =============================================================================
// Transfer (O(1) splice)
// =============================================================================

/**
 * @test transfer(from) moves all children and reparents them.
 */
TEST(HierarchyNodeTest, TransferAllChildren)
{
    Node src;
    src.emplace_back(1);
    src.emplace_back(2);

    Node dst;
    dst.transfer(src);

    EXPECT_TRUE(src.empty());
    EXPECT_EQ(dst.size(), 2u);
    EXPECT_TRUE(std::ranges::all_of(dst, [&dst](auto & child) {
        return scl::hierarchy::is_parent_of(dst, child);
    }));
}

/**
 * @test transfer(from, first) moves a single child.
 */
TEST(HierarchyNodeTest, TransferSingleChild)
{
    Node src;
    src.emplace_back(1);
    src.emplace_back(2);

    Node dst;
    dst.transfer(src, src.cbegin());

    EXPECT_EQ(src.size(), 1u);
    EXPECT_EQ(dst.size(), 1u);
    EXPECT_EQ(dst.front().get(), 1);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(dst, dst.front()));
}

/**
 * @test transfer(from, first, last) moves a half-open range.
 */
TEST(HierarchyNodeTest, TransferRange)
{
    Node src;
    src.emplace_back(1);
    src.emplace_back(2);
    src.emplace_back(3);
    src.emplace_back(4);

    Node dst;
    dst.transfer(src, std::next(src.cbegin()), std::prev(src.cend())); // 2, 3

    EXPECT_EQ(src.size(), 2u);
    EXPECT_EQ(dst.size(), 2u);
    EXPECT_EQ(dst.front().get(), 2);
    EXPECT_EQ(dst.back().get(), 3);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(dst, dst.front()));
}

/**
 * @test transfer(where, from) inserts the moved children at an explicit position.
 */
TEST(HierarchyNodeTest, TransferAtInsertionPoint)
{
    Node src;
    src.emplace_back(2);
    src.emplace_back(3);

    Node dst;
    dst.emplace_back(1);
    dst.emplace_back(4);
    dst.transfer(std::next(dst.cbegin()), src); // insert 2, 3 before 4

    EXPECT_TRUE(src.empty());
    ASSERT_EQ(dst.size(), 4u);
    auto it = dst.begin();
    EXPECT_EQ((it++)->get(), 1);
    EXPECT_EQ((it++)->get(), 2);
    EXPECT_EQ((it++)->get(), 3);
    EXPECT_EQ(it->get(), 4);
}

// =============================================================================
// Iteration
// =============================================================================

/**
 * @test Range-based iteration visits direct children only, in order.
 */
TEST(HierarchyNodeTest, RangeForIteratesDirectChildren)
{
    Node parent;
    parent.emplace_back(10);
    parent.emplace_back(20)->emplace_back(99); // grandchild — must be skipped
    parent.emplace_back(30);

    std::vector<int> values;
    std::ranges::transform(parent, std::back_inserter(values), [](auto & c) { return c.get(); });

    std::vector<int> const expected{10, 20, 30};
    EXPECT_EQ(values, expected);
}

/**
 * @test rbegin()/rend() traverse children in reverse order.
 */
TEST(HierarchyNodeTest, ReverseIteratorTraversal)
{
    Node parent;
    parent.emplace_back(1);
    parent.emplace_back(2);
    parent.emplace_back(3);

    std::vector<int> values;
    for (auto it = parent.rbegin(); it != parent.rend(); ++it)
        values.push_back(it->get());

    std::vector<int> const expected{3, 2, 1};
    EXPECT_EQ(values, expected);
}
