#include <gtest_utils.h>

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

using Node = scl::hierarchy::node<int>;

/**
 * @test are_identical returns true for the same object.
 */
TEST(HierarchyAlgorithmTest, AreIdenticalSameObject)
{
    Node a(1);
    EXPECT_TRUE(scl::hierarchy::are_identical(a, a));
}

/**
 * @test are_identical returns false for two distinct objects.
 */
TEST(HierarchyAlgorithmTest, AreIdenticalDifferentObjects)
{
    Node a(1);
    Node b(1);
    EXPECT_FALSE(scl::hierarchy::are_identical(a, b));
}

/**
 * @test has_parent returns false for a root node.
 */
TEST(HierarchyAlgorithmTest, HasParentFalseForRoot)
{
    Node root;
    EXPECT_FALSE(scl::hierarchy::has_parent(root));
}

/**
 * @test has_parent returns true for a child node.
 */
TEST(HierarchyAlgorithmTest, HasParentTrueForChild)
{
    Node parent;
    auto it = parent.push_back(1);
    EXPECT_TRUE(scl::hierarchy::has_parent(*it));
}

/**
 * @test is_parent_of returns true when node is the direct parent.
 */
TEST(HierarchyAlgorithmTest, IsParentOfDirect)
{
    Node parent;
    auto it = parent.push_back(42);
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, *it));
}

/**
 * @test is_parent_of returns false for a non-parent.
 */
TEST(HierarchyAlgorithmTest, IsParentOfFalseForNonParent)
{
    Node a;
    Node b;
    auto it = a.push_back(1);
    EXPECT_FALSE(scl::hierarchy::is_parent_of(b, *it));
}

/**
 * @test is_ancestor_of returns true for direct parent.
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfDirectParent)
{
    Node root;
    auto it = root.push_back(1);
    EXPECT_TRUE(scl::hierarchy::is_ancestor_of(root, *it));
}

/**
 * @test is_ancestor_of returns true for grandparent.
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfGrandparent)
{
    Node root;
    auto child_it = root.push_back(1);
    auto grand_it = child_it->push_back(2);
    EXPECT_TRUE(scl::hierarchy::is_ancestor_of(root, *grand_it));
}

/**
 * @test is_ancestor_of returns false when there is no ancestry.
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfFalse)
{
    Node a;
    Node b;
    auto it = b.push_back(1);
    EXPECT_FALSE(scl::hierarchy::is_ancestor_of(a, *it));
}

/**
 * @test is_ancestor_of returns false for root node (no parent).
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfFalseForRoot)
{
    Node a;
    Node root;
    EXPECT_FALSE(scl::hierarchy::is_ancestor_of(a, root));
}

/**
 * @test are_sibling returns true when both share the same parent.
 */
TEST(HierarchyAlgorithmTest, AreSiblingTrue)
{
    Node parent;
    auto it1 = parent.push_back(1);
    auto it2 = parent.push_back(2);
    EXPECT_TRUE(scl::hierarchy::are_sibling(*it1, *it2));
}

/**
 * @test are_sibling returns false for nodes with different parents.
 */
TEST(HierarchyAlgorithmTest, AreSiblingFalse)
{
    Node parent1;
    Node parent2;
    auto it1 = parent1.push_back(1);
    auto it2 = parent2.push_back(2);
    EXPECT_FALSE(scl::hierarchy::are_sibling(*it1, *it2));
}

/**
 * @test are_sibling returns false for root nodes (no parent).
 */
TEST(HierarchyAlgorithmTest, AreSiblingFalseForRoots)
{
    Node a;
    Node b;
    EXPECT_FALSE(scl::hierarchy::are_sibling(a, b));
}
