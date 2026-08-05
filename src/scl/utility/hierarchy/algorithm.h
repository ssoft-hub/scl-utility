#pragma once

/**
 * @file algorithm.h
 * @brief Hierarchy relationship query algorithms.
 * @ingroup scl_utility_hierarchy
 *
 * Every algorithm below dispatches through three ADL customization points —
 * `adl_parent`, `adl_has_parent`, `adl_identity` — instead of requiring a
 * fixed node type or an explicit registration step. A type opts in by
 * defining these three names for itself, in its own namespace, where
 * argument-dependent lookup (ADL) will find them. `scl::hierarchy::node` and
 * `tree::reference`/`tree::const_reference` already provide them (see
 * `node.h`, `tree.h`) — most callers never write an ADL hook themselves,
 * they just call `parent_of`/`has_parent`/`are_identical` below.
 *
 * - `adl_parent(self)` — returns `self`'s parent, same constness as `self`.
 *   Precondition: `adl_has_parent(self)` is `true`; calling it otherwise is
 *   whatever the implementing type documents (`node`'s hook dereferences a
 *   null pointer, i.e. undefined behavior). Constness must be preserved —
 *   a `const` `self` yields a `const`-qualified parent, a mutable `self`
 *   yields a mutable one; two overloads (mutable / `const`) express this
 *   more simply than one forwarding-reference template that would collapse
 *   both to the same constness. `noexcept` is recommended but not required:
 *   `parent_of`'s own `noexcept` is *computed* from
 *   `noexcept(adl_parent(self))`, so a throwing `adl_parent` only makes
 *   `parent_of` potentially-throwing for that type, nothing worse.
 * - `adl_has_parent(self)` — returns `true` exactly when `adl_parent(self)`
 *   is valid to call; `false` for a root / parentless value. No
 *   precondition of its own — must accept any value that reaches it. One
 *   overload taking `self const &` is enough, since `bool` carries no
 *   constness to preserve. Same non-hard `noexcept` recommendation as
 *   `adl_parent` (`has_parent`'s `noexcept` is likewise computed from
 *   `noexcept(adl_has_parent(self))`).
 * - `adl_identity(self)` — returns an opaque token identifying the node
 *   `self` denotes; any type with `operator==` works (`node`/`tree` use
 *   `void const *`) — callers must treat it as opaque, not assume it is an
 *   address. Two calls on the *same* underlying node must compare equal;
 *   two calls on *different* nodes must compare unequal, even when their
 *   payloads are equal — identity is never value equality. Unlike the two
 *   hooks above, `noexcept` here is a hard requirement: `are_identical` (see
 *   below) is declared unconditionally `noexcept`, not computed from
 *   `noexcept(adl_identity(self))`, so a throwing `adl_identity` would call
 *   `std::terminate` the moment it throws.
 *
 * @par Adapting a foreign type
 * @code
 * namespace my_lib
 * {
 *     struct widget
 *     {
 *         widget * parent = nullptr;
 *     };
 *
 *     // Found via unqualified lookup because they live in my_lib,
 *     // widget's own namespace — no registration elsewhere is needed.
 *     constexpr widget & adl_parent(widget & self) noexcept { return *self.parent; }
 *     constexpr widget const & adl_parent(widget const & self) noexcept { return *self.parent; }
 *     constexpr bool adl_has_parent(widget const & self) noexcept { return self.parent != nullptr; }
 *     constexpr void const * adl_identity(widget const & self) noexcept { return &self; }
 * }
 *
 * my_lib::widget root, child;
 * child.parent = &root;
 * assert( scl::hierarchy::has_parent(child));
 * assert(&scl::hierarchy::parent_of(child) == &root);
 * assert( scl::hierarchy::are_identical(root, root));
 * @endcode
 */

#include <utility>

namespace scl::hierarchy
{
    /**
     * @brief Returns the parent of @p value, found via ADL.
     * @ingroup scl_utility_hierarchy
     *
     * Dispatches to an unqualified call `adl_parent(value)`, resolved by ordinary
     * lookup and argument-dependent lookup (ADL): `scl::hierarchy::node` and
     * `tree::reference`/`tree::const_reference` each define their own overloads of
     * this name next to their class (see `node.h`, `tree.h`); a foreign type adapts
     * the same way, in its own namespace.
     *
     * @param  value  The node whose parent to retrieve.
     * @return        The parent node (reference or proxy), value category preserved.
     *
     * @par Example
     * @code
     * scl::hierarchy::node<int> root{42};
     * auto & child = *root.push_back(7);
     * assert(&scl::hierarchy::parent_of(child) == &root);
     * @endcode
     */
    template <typename Value>
    [[nodiscard]]
    constexpr decltype(auto) parent_of(Value && value) /**/
        noexcept(noexcept(adl_parent(::std::forward<Value>(value))))
        requires requires { adl_parent(::std::forward<Value>(value)); }
    {
        return adl_parent(::std::forward<Value>(value));
    }

    /**
     * @brief Returns `true` when @p first and @p second denote the same node.
     * @ingroup scl_utility_hierarchy
     *
     * Identity is an opaque token for the node each argument denotes, obtained
     * through an unqualified call `adl_identity(value)` (ADL — see `parent_of`).
     * Distinct objects with equal value are never identical, and the result does
     * not depend on value category — unlike an `operator==` fallback, it never
     * degrades into value equality.
     *
     * @param  first   Left-hand node or node-reference proxy.
     * @param  second  Right-hand node or node-reference proxy.
     * @return `true` when both denote the same node.
     *
     * @par Example
     * @code
     * scl::hierarchy::node<int> a{1}, b{1};
     * assert( scl::hierarchy::are_identical(a, a)); // same object
     * assert(!scl::hierarchy::are_identical(a, b)); // different objects, equal value
     *
     * int_tree t;
     * auto it1 = t.push_back(1);
     * auto it2 = t.push_back(1);
     * assert( scl::hierarchy::are_identical(*it1, *it1)); // proxies to the same node
     * assert(!scl::hierarchy::are_identical(*it1, *it2));
     * @endcode
     */
    template <typename First, typename Second>
    [[nodiscard]]
    constexpr bool are_identical(First && first, Second && second) noexcept
        requires requires {
                     adl_identity(::std::forward<First>(first));
                     adl_identity(::std::forward<Second>(second));
                 }
    {
        return adl_identity(::std::forward<First>(first)) == adl_identity(::std::forward<Second>(second));
    }

    /**
     * @brief Returns `true` when @p value has a parent, found via ADL.
     * @ingroup scl_utility_hierarchy
     *
     * Dispatches to an unqualified call `adl_has_parent(value)` (ADL — see
     * `parent_of`). Answers without dereferencing a possibly-null parent pointer.
     *
     * @param  value  The node to query.
     * @return        Result of `adl_has_parent`.
     *
     * @par Example
     * @code
     * scl::hierarchy::node<int> root{0};
     * auto & child = *root.push_back(1);
     * assert(!scl::hierarchy::has_parent(root));
     * assert( scl::hierarchy::has_parent(child));
     * @endcode
     */
    template <typename Value>
    [[nodiscard]]
    constexpr bool has_parent(Value && value) /**/
        noexcept(noexcept(adl_has_parent(::std::forward<Value>(value))))
        requires requires { adl_has_parent(::std::forward<Value>(value)); }
    {
        return adl_has_parent(::std::forward<Value>(value));
    }

    /**
     * @brief Returns `true` when @p parent is the direct parent of @p value.
     * @ingroup scl_utility_hierarchy
     *
     * Guards on `has_parent(value)` first, so `parent_of` is never evaluated on a parentless
     * node.  Does **not** walk the ancestor chain — use `is_ancestor_of` for transitive checks.
     *
     * @param  parent  The node to test as the direct parent.
     * @param  value   The child node.
     * @return `true` when @p value has a parent identical to @p parent.
     *
     * @par Example
     * @code
     * scl::hierarchy::node<int> root{0};
     * auto & child = *root.push_back(1);
     * auto & grandchild = *child.push_back(2);
     * assert( scl::hierarchy::is_parent_of(root, child));
     * assert(!scl::hierarchy::is_parent_of(root, grandchild)); // not direct
     * @endcode
     */
    template <typename Parent, typename Value>
    [[nodiscard]]
    constexpr bool is_parent_of(Parent && parent, Value && value) /**/
        noexcept(noexcept(::scl::hierarchy::has_parent(value)) &&
            noexcept(::scl::hierarchy::are_identical(parent,
                ::scl::hierarchy::parent_of(::std::forward<Value>(value)))))
        requires requires(Value && v) {
                     ::scl::hierarchy::has_parent(v);
                     ::scl::hierarchy::parent_of(v);
                 }
    {
        return ::scl::hierarchy::has_parent(value) &&
            ::scl::hierarchy::are_identical(::std::forward<Parent>(parent),
                ::scl::hierarchy::parent_of(::std::forward<Value>(value)));
    }

    /**
     * @brief Returns `true` when @p parent is a direct or transitive ancestor of @p value.
     * @ingroup scl_utility_hierarchy
     *
     * Walks the parent chain upward from @p value until @p parent is found or a root
     * (no parent) is reached.  Time complexity is O(depth).
     *
     * @param  parent  The node to test as an ancestor.
     * @param  value   The node to start the upward search from.
     * @return `true` when @p parent is found anywhere on the ancestor chain of @p value.
     *
     * @par Example
     * @code
     * scl::hierarchy::node<int> root{0};
     * auto & child = *root.push_back(1);
     * auto & grandchild = *child.push_back(2);
     * assert( scl::hierarchy::is_ancestor_of(root, grandchild)); // transitive
     * assert( scl::hierarchy::is_ancestor_of(root, child));      // direct
     * assert(!scl::hierarchy::is_ancestor_of(child, root));      // wrong direction
     * assert(!scl::hierarchy::is_ancestor_of(root, root));       // node is not its own ancestor
     * @endcode
     */
    template <typename Parent, typename Value>
    [[nodiscard]]
    constexpr bool is_ancestor_of(Parent && parent, Value && value) /**/
        noexcept(noexcept(::scl::hierarchy::has_parent(value)) &&
            noexcept(::scl::hierarchy::is_parent_of(::std::forward<Parent>(parent), ::std::forward<Value>(value))))
        requires requires(Value && v) {
                     ::scl::hierarchy::has_parent(v);
                     ::scl::hierarchy::parent_of(v);
                 }
    {
        if (!::scl::hierarchy::has_parent(value))
            return false;
        if (::scl::hierarchy::is_parent_of(parent, value))
            return true;
        return ::scl::hierarchy::is_ancestor_of(::std::forward<Parent>(parent),
            ::scl::hierarchy::parent_of(::std::forward<Value>(value)));
    }

    /**
     * @brief Returns `true` when @p first and @p second share the same direct parent.
     * @ingroup scl_utility_hierarchy
     *
     * Returns `false` when either node has no parent.
     *
     * @param  first   First node.
     * @param  second  Second node.
     * @return `true` when both nodes have a parent and those parents are identical.
     *
     * @par Example
     * @code
     * scl::hierarchy::node<int> root{0};
     * auto & a = *root.push_back(1);
     * auto & b = *root.push_back(2);
     * auto & c = *a.push_back(3);
     * assert( scl::hierarchy::are_sibling(a, b));       // same parent
     * assert( scl::hierarchy::are_sibling(a, a));       // same node — same parent — true
     * assert(!scl::hierarchy::are_sibling(a, c));       // c is child of a, not sibling
     * assert(!scl::hierarchy::are_sibling(root, root)); // root has no parent
     * @endcode
     */
    template <typename First, typename Second>
    [[nodiscard]]
    constexpr bool are_sibling(First && first, Second && second) /**/
        noexcept(noexcept(::scl::hierarchy::has_parent(first)) &&
            noexcept(::scl::hierarchy::are_identical(::scl::hierarchy::parent_of(::std::forward<First>(first)),
                ::scl::hierarchy::parent_of(::std::forward<Second>(second)))))
        requires requires(First && f, Second && s) {
                     ::scl::hierarchy::has_parent(f);
                     ::scl::hierarchy::parent_of(f);
                     ::scl::hierarchy::has_parent(s);
                     ::scl::hierarchy::parent_of(s);
                 }
    {
        if (!::scl::hierarchy::has_parent(first) || !::scl::hierarchy::has_parent(second))
            return false;
        return ::scl::hierarchy::are_identical(::scl::hierarchy::parent_of(::std::forward<First>(first)),
            ::scl::hierarchy::parent_of(::std::forward<Second>(second)));
    }

} // namespace scl::hierarchy
