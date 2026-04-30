# Hierarchy Node

A C++20 header-only parent-child tree node.

- Header: `#include <scl/utility/hierarchy/node.h>`

## Overview

`scl::hierarchy::node<Value, Allocator>` is a tree node that stores a payload
of type `Value` and owns zero or more child nodes in an `std::list`.  Moving
children between nodes is O(1) (via `std::list::splice`); deep-copying a
subtree is O(N).

Iteration over a node yields its **direct children** as `node<Value>&`
references, making `node` a standard range container where `value_type` is
`node<Value>` itself.

## Features

- Recursive parent-child hierarchy with stable iterators
- O(1) transfer (splice) between nodes
- Deep copy via copy constructor
- Full `std::list`-compatible iterator interface
- Works with any allocator template

## API Reference

### Construction

```cpp
node<int> root;              // default-constructed payload
node<int> n(42);             // forwards to Value constructor
node<std::string> s("hello");
```

### Payload access

```cpp
reference_type       value() noexcept;
const_reference_type value() const noexcept;
```

### Parent access

```cpp
node_reference_type       parent() noexcept;        // UB if no parent
node_const_reference_type parent() const noexcept;
```

Use `scl::hierarchy::has_parent(n)` before calling `parent()`.

### Child queries

```cpp
bool       empty()  const noexcept;
node_count count()  const noexcept;
node_count size()   const noexcept; // alias

node_reference_type       first() noexcept;
node_const_reference_type first() const noexcept;
node_reference_type       last()  noexcept;
node_const_reference_type last()  const noexcept;
```

### Child insertion

```cpp
iterator append (/* Value args */);   // constructs at end
iterator prepend(/* Value args */);   // constructs at front
iterator emplace(const_iterator pos, /* Value args */);
```

### Child removal

```cpp
iterator       erase(iterator pos) noexcept;
iterator       erase(iterator first, iterator last) noexcept;
const_iterator erase(const_iterator pos) noexcept;
const_iterator erase(const_iterator first, const_iterator last) noexcept;
void           clear() noexcept;

node take      (const_iterator pos) noexcept;  // deep-copy out
node take_first() noexcept;
node take_last () noexcept;
```

### Child transfer (O(1) splice)

```cpp
void transfer(node& from);
void transfer(node& from, const_iterator from_pos);
void transfer(node& from, const_iterator from_first, const_iterator from_last);
void transfer(const_iterator before, node& from);
void transfer(const_iterator before, node& from, const_iterator from_pos);
void transfer(const_iterator before, node& from, const_iterator from_first, const_iterator from_last);
```

### Iterators

Standard `begin/end/cbegin/cend` over direct children.

## Usage Examples

### Example 1: Building a simple hierarchy

```cpp
using Node = scl::hierarchy::node<std::string>;

Node root("root");
auto child_it = root.append("child");
child_it->append("grandchild");

for (auto& c : root)
    std::cout << c.value() << '\n'; // "child"
```

### Example 2: Querying relationships

```cpp
Node parent("p");
auto it = parent.append("c");

assert(scl::hierarchy::has_parent(*it));
assert(scl::hierarchy::is_parent_of(parent, *it));
assert(scl::hierarchy::is_ancestor_of(parent, *it));
```

### Example 3: Transferring children

```cpp
Node src;
src.append(1);
src.append(2);

Node dst;
dst.transfer(src);          // moves all children; src becomes empty
assert(src.empty());
assert(dst.count() == 2);
```

### Example 4: Extracting a subtree

```cpp
Node parent;
auto it = parent.append("child");
it->append("grandchild");

Node extracted = parent.take_first();
// extracted is a deep copy of the former first child tree
assert(parent.empty());
assert(extracted.count() == 1); // grandchild is included
```

## Notes

- `parent()` returns a dangling reference when the node has no parent.
  Always guard with `scl::hierarchy::has_parent()`.
- `take()` performs a deep copy (calls copy constructor), not a move.
  Use `transfer()` for O(1) bulk relocation.
- Copy constructor recursively deep-copies all descendants.
- Move constructor transfers children and updates their parent pointers.
