/// @file node_example.cpp
/// @brief Demonstrates scl::hierarchy::node parent-child relationships.
///
/// Shows construction, child insertion, iteration, transfer, take, and the
/// hierarchy query algorithms (are_identical, has_parent, is_parent_of,
/// is_ancestor_of, are_sibling).

#include <scl/utility/hierarchy/algorithm.h>
#include <scl/utility/hierarchy/node.h>

#include <cassert>
#include <iostream>
#include <string>

using Node = scl::hierarchy::node<::std::string>;

static void print_tree(Node const & n, int depth = 0)
{
    for (int i = 0; i < depth; ++i)
        ::std::cout << "  ";
    ::std::cout << n.value() << '\n';
    for (auto const & child : n)
        print_tree(child, depth + 1);
}

int main()
{
    // ---- Construction ----

    Node root("root");

    // ---- Insert children ----

    Node::iterator a_it = root.push_back("a");
    Node::iterator b_it = a_it->push_back("b");
    Node::iterator c_it = b_it->push_back("c");
    (void)c_it;

    //  root
    //    a
    //      b
    //        c

    print_tree(root);

    // ---- Query algorithms ----

    assert(scl::hierarchy::are_identical(root, root));
    assert(!scl::hierarchy::are_identical(root, *a_it));

    assert(!scl::hierarchy::has_parent(root));
    assert(scl::hierarchy::has_parent(*a_it));

    assert(scl::hierarchy::is_parent_of(root, *a_it));
    assert(!scl::hierarchy::is_parent_of(root, *b_it));

    assert(scl::hierarchy::is_ancestor_of(root, *c_it));
    assert(!scl::hierarchy::is_ancestor_of(*b_it, *a_it));

    // ---- Sibling check ----

    Node::iterator a2_it = root.push_back("a2");
    assert(scl::hierarchy::are_sibling(*a_it, *a2_it));
    assert(!scl::hierarchy::are_sibling(*a_it, *b_it));

    // ---- Transfer ----

    root.transfer(*a_it);

    //  root
    //    a          (was a_it, now has no children)
    //    a2
    //    b
    //      c

    ::std::cout << "\nAfter transfer:\n";
    print_tree(root);

    // ---- take() ----

    Node taken = root.take_first();
    ::std::cout << "\nTaken first: " << taken.value() << '\n';

    // ---- Range-based for ----

    ::std::cout << "\nRoot children:\n";
    for (auto const & child : root)
        ::std::cout << "  " << child.value() << '\n';

    return 0;
}
