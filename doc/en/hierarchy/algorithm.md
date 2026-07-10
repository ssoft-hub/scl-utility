# Hierarchy algorithms

Free functions that query the structural relationships between hierarchy nodes.

- Header: `#include <scl/utility/hierarchy/algorithm.h>`

## Overview

A set of function templates that express structural relationships between nodes.
They operate on any type adapted via three unqualified calls —
`adl_parent(value)`, `adl_has_parent(value)`, `adl_identity(value)` — resolved
by ordinary lookup and [ADL](https://en.cppreference.com/w/cpp/language/adl)
(argument-dependent lookup). There is no shared trait class or generic header:
`scl::hierarchy::node` defines its own three overloads right next to the class,
in `node.h`; `tree::reference`/`tree::const_reference` are adapted by a
concept-constrained template declared in `tree.h` (their nested-type-of-a-class-
template nature makes naming them directly in a function parameter list
impossible to deduce — see the comment above `tree.h`'s `node_proxy` concept).
Both work out of the box. A foreign type adapts the same way node's own
overloads do: define `adl_parent`/`adl_has_parent`/`adl_identity` for it in its
own namespace.

## API reference

### `parent_of`

```cpp
template <typename Value>
constexpr decltype(auto) parent_of(Value && value);
```

Returns the parent of `value` via an unqualified `adl_parent(value)` call (ADL),
preserving value category.

### `are_identical`

```cpp
template <typename First, typename Second>
constexpr bool are_identical(First && first, Second && second) noexcept;
```

Returns `true` when both operands denote the **same node**. Identity is an
opaque token for the node each argument denotes, obtained through an
unqualified `adl_identity(value)` call (ADL). The result is independent of
value category and never degrades into value equality — distinct objects with
equal value are never identical.

### `has_parent`

```cpp
template <typename Value>
constexpr bool has_parent(Value && value);
```

Returns `true` when `value` has a parent, via an unqualified `adl_has_parent(value)`
call (ADL). It never dereferences a null parent pointer.

### `is_parent_of`

```cpp
template <typename Parent, typename Value>
constexpr bool is_parent_of(Parent && parent, Value && value);
```

Returns `true` when `parent` is the **direct** parent of `value`. Guards on
`has_parent(value)` first, so `parent_of` is never evaluated on a parentless node.

### `is_ancestor_of`

```cpp
template <typename Parent, typename Value>
constexpr bool is_ancestor_of(Parent && parent, Value && value);
```

Returns `true` when `parent` is a **direct or transitive** ancestor of `value`.
Walks the parent chain upward; a node is never its own ancestor. O(depth).

### `are_sibling`

```cpp
template <typename First, typename Second>
constexpr bool are_sibling(First && first, Second && second);
```

Returns `true` when both nodes share the same direct parent. Returns `false` when
either node is a root (no parent).

## Usage examples

### Example 1: basic checks

```cpp
using Node = scl::hierarchy::node<int>;

Node parent;
auto it = parent.push_back(42);

assert( scl::hierarchy::has_parent(*it));
assert(!scl::hierarchy::has_parent(parent));
assert( scl::hierarchy::is_parent_of(parent, *it));
assert( scl::hierarchy::are_identical(parent, parent));
assert(!scl::hierarchy::are_identical(parent, *it));
```

### Example 2: ancestry

```cpp
Node root;
auto child = root.push_back(1);
auto grand = child->push_back(2);

assert( scl::hierarchy::is_ancestor_of(root, *grand));
assert(!scl::hierarchy::is_ancestor_of(*grand, root));
```

### Example 3: siblings

```cpp
Node parent;
auto a = parent.push_back(1);
auto b = parent.push_back(2);

assert( scl::hierarchy::are_sibling(*a, *b));
```
