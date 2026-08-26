#include <gtest_utils.h>

#include <scl/utility/hierarchy/algorithm.h>

#include <list>
#include <type_traits>

namespace
{
    /**
     * Minimal node-like type: enough parent/child structure to exercise the
     * hierarchy algorithms without pulling in scl::hierarchy::node. A std::list
     * backs the children so their addresses (and parent back-pointers) stay stable.
     */
    struct mock_node
    {
    public:
        mock_node * m_parent = nullptr;
        ::std::list<mock_node> m_children;

    public:
        mock_node() = default;

        [[nodiscard]]
        bool has_parent() const noexcept
        {
            return m_parent != nullptr;
        }

        mock_node & parent() noexcept { return *m_parent; }
        mock_node const & parent() const noexcept { return *m_parent; }

        mock_node & add_child()
        {
            mock_node & child = m_children.emplace_back();
            child.m_parent = this;
            return child;
        }
    };

    // Explicit opt-in: the hierarchy algorithms find adl_parent/adl_has_parent/adl_identity
    // via ADL, so mock_node must define its own — the same pattern a foreign node type
    // would use, in its own namespace (see algorithm.h's parent_of doc comment).
    mock_node & adl_parent(mock_node & self) noexcept { return self.parent(); }
    mock_node const & adl_parent(mock_node const & self) noexcept { return self.parent(); }
    bool adl_has_parent(mock_node const & self) noexcept { return self.has_parent(); }
    void const * adl_identity(mock_node const & self) noexcept { return &self; }
} // namespace

/**
 * @test parent_of returns a reference to the actual parent object.
 */
TEST(HierarchyAlgorithmTest, ParentOfReturnsParent)
{
    mock_node parent;
    mock_node & child = parent.add_child();
    EXPECT_EQ(&scl::hierarchy::parent_of(child), &parent);
}

/**
 * @test parent_of on a const child yields a const reference to the parent.
 */
TEST(HierarchyAlgorithmTest, ParentOfPreservesConst)
{
    mock_node parent;
    mock_node & child = parent.add_child();
    mock_node const & const_child = child;
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<decltype(scl::hierarchy::parent_of(const_child)), mock_node const &>));
    EXPECT_EQ(&scl::hierarchy::parent_of(const_child), &parent);
}

/**
 * @test are_identical returns true for the same object.
 */
TEST(HierarchyAlgorithmTest, AreIdenticalSameObject)
{
    mock_node a;
    EXPECT_TRUE(scl::hierarchy::are_identical(a, a));
}

/**
 * @test are_identical returns false for two distinct objects.
 */
TEST(HierarchyAlgorithmTest, AreIdenticalDifferentObjects)
{
    mock_node a;
    mock_node b;
    EXPECT_FALSE(scl::hierarchy::are_identical(a, b));
}

/**
 * @test are_identical means identity, not equality — two distinct rvalue temporaries
 *       are never identical (regression: must not degrade into operator== value equality).
 */
TEST(HierarchyAlgorithmTest, AreIdenticalIsNotEquality)
{
    EXPECT_FALSE(scl::hierarchy::are_identical(mock_node{}, mock_node{}));
}

/**
 * @test The identity token is a value: built, copied, compared, and nothing else.
 */
TEST(HierarchyAlgorithmTest, IdentityIsAValue)
{
    STATIC_EXPECT_TRUE(::std::is_default_constructible_v<scl::hierarchy::identity>);
    STATIC_EXPECT_TRUE(::std::is_copy_constructible_v<scl::hierarchy::identity>);
    STATIC_EXPECT_TRUE(::std::is_copy_assignable_v<scl::hierarchy::identity>);

    // Opaque: the address it was built from never comes back out.
    STATIC_EXPECT_FALSE((::std::is_convertible_v<scl::hierarchy::identity, void const *>));

    mock_node first;
    mock_node second;

    EXPECT_EQ(scl::hierarchy::identity{first}, scl::hierarchy::identity{first});
    EXPECT_NE(scl::hierarchy::identity{first}, scl::hierarchy::identity{second});
    EXPECT_EQ(scl::hierarchy::identity{}, scl::hierarchy::identity{});
    EXPECT_NE(scl::hierarchy::identity{first}, scl::hierarchy::identity{});
}

/**
 * @test has_parent returns false for a root node.
 */
TEST(HierarchyAlgorithmTest, HasParentFalseForRoot)
{
    mock_node root;
    EXPECT_FALSE(scl::hierarchy::has_parent(root));
}

/**
 * @test has_parent returns true for a child node.
 */
TEST(HierarchyAlgorithmTest, HasParentTrueForChild)
{
    mock_node parent;
    mock_node & child = parent.add_child();
    EXPECT_TRUE(scl::hierarchy::has_parent(child));
}

/**
 * @test is_parent_of returns true when node is the direct parent.
 */
TEST(HierarchyAlgorithmTest, IsParentOfDirect)
{
    mock_node parent;
    mock_node & child = parent.add_child();
    EXPECT_TRUE(scl::hierarchy::is_parent_of(parent, child));
}

/**
 * @test is_parent_of returns false for a non-parent.
 */
TEST(HierarchyAlgorithmTest, IsParentOfFalseForNonParent)
{
    mock_node a;
    mock_node b;
    mock_node & child = a.add_child();
    EXPECT_FALSE(scl::hierarchy::is_parent_of(b, child));
}

/**
 * @test is_parent_of returns false for a parentless node (no dereference of a null parent).
 */
TEST(HierarchyAlgorithmTest, IsParentOfFalseForRoot)
{
    mock_node parent;
    mock_node root;
    EXPECT_FALSE(scl::hierarchy::is_parent_of(parent, root));
}

/**
 * @test is_ancestor_of returns true for direct parent.
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfDirectParent)
{
    mock_node root;
    mock_node & child = root.add_child();
    EXPECT_TRUE(scl::hierarchy::is_ancestor_of(root, child));
}

/**
 * @test is_ancestor_of returns true for grandparent.
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfGrandparent)
{
    mock_node root;
    mock_node & child = root.add_child();
    mock_node & grand = child.add_child();
    EXPECT_TRUE(scl::hierarchy::is_ancestor_of(root, grand));
}

/**
 * @test is_ancestor_of returns false when there is no ancestry.
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfFalse)
{
    mock_node a;
    mock_node b;
    mock_node & child = b.add_child();
    EXPECT_FALSE(scl::hierarchy::is_ancestor_of(a, child));
}

/**
 * @test is_ancestor_of returns false for root node (no parent).
 */
TEST(HierarchyAlgorithmTest, IsAncestorOfFalseForRoot)
{
    mock_node a;
    mock_node root;
    EXPECT_FALSE(scl::hierarchy::is_ancestor_of(a, root));
}

/**
 * @test are_sibling returns true when both share the same parent.
 */
TEST(HierarchyAlgorithmTest, AreSiblingTrue)
{
    mock_node parent;
    mock_node & a = parent.add_child();
    mock_node & b = parent.add_child();
    EXPECT_TRUE(scl::hierarchy::are_sibling(a, b));
}

/**
 * @test are_sibling returns false for nodes with different parents.
 */
TEST(HierarchyAlgorithmTest, AreSiblingFalse)
{
    mock_node parent1;
    mock_node parent2;
    mock_node & a = parent1.add_child();
    mock_node & b = parent2.add_child();
    EXPECT_FALSE(scl::hierarchy::are_sibling(a, b));
}

/**
 * @test are_sibling returns false for root nodes (no parent).
 */
TEST(HierarchyAlgorithmTest, AreSiblingFalseForRoots)
{
    mock_node a;
    mock_node b;
    EXPECT_FALSE(scl::hierarchy::are_sibling(a, b));
}
