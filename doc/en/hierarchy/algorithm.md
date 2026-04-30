# Hierarchy Algorithms

Compile-time and runtime relationship queries for hierarchy nodes.

- Header: `#include <scl/utility/hierarchy/algorithm.h>`

## Overview

A set of free-function templates that express structural relationships between
nodes.  All functions work with any type that exposes a `parent()` member
returning a reference to the parent.

The identity check uses `std::addressof` for lvalue references and falls back
to `operator==` for value types.  A null parent pointer evaluates as "no
parent" via the `addressof(x) != nullptr` guard.

## API Reference

### `are_identical`

```cpp
template <typename FirstValue, typename SecondValue>
bool are_identical(FirstValue&& first, SecondValue&& second);
```

Returns `true` when `first` and `second` refer to the **same object** (same
address for references, `operator==` for values).

### `has_parent`

```cpp
template <typename Value>
bool has_parent(Value&& value);
```

Returns `true` when `value` has a parent.  Implemented by comparing
`parent_of(value)` with itself — a null parent pointer produces a null address
that fails the `!= nullptr` check.

### `is_parent_of`

```cpp
template <typename Parent, typename Value>
bool is_parent_of(Parent&& parent, Value&& value);
```

Returns `true` when `parent` is the **direct** parent of `value`.

### `is_ancestor_of`

```cpp
template <typename Parent, typename Value>
bool is_ancestor_of(Parent&& parent, Value&& value);
```

Returns `true` when `parent` is a **direct or transitive** ancestor of
`value`.  Recursively follows `parent_of` up the chain.

### `are_sibling`

```cpp
template <typename ParentedFirst, typename ParentedSecond>
bool are_sibling(ParentedFirst&& first, ParentedSecond&& second);
```

Returns `true` when `first` and `second` share the same parent.  Returns
`false` for root nodes (they have no parent, so the null addresses differ).

## Usage Examples

### Example 1: Basic checks

```cpp
using Node = scl::hierarchy::node<int>;

Node parent;
auto it = parent.append(42);

assert( scl::hierarchy::has_parent(*it));
assert(!scl::hierarchy::has_parent(parent));
assert( scl::hierarchy::is_parent_of(parent, *it));
assert( scl::hierarchy::are_identical(parent, parent));
assert(!scl::hierarchy::are_identical(parent, *it));
```

### Example 2: Ancestor detection

```cpp
Node root;
auto child = root.append(1);
auto grand = child->append(2);

assert( scl::hierarchy::is_ancestor_of(root, *grand));
assert(!scl::hierarchy::is_ancestor_of(*grand, root));
```

### Example 3: Sibling detection

```cpp
Node parent;
auto it1 = parent.append(1);
auto it2 = parent.append(2);

assert( scl::hierarchy::are_sibling(*it1, *it2));
```

## Customisation

The identity check is customisable via the `identity_of` policy struct.
Specialize it for your own value or reference types if the default
`std::addressof` comparison is not appropriate.

```cpp
namespace scl::hierarchy {
    template <>
    struct identity_of<MyType, MyType> {
        static bool check(MyType a, MyType b) { return a.id() == b.id(); }
    };
}
```
