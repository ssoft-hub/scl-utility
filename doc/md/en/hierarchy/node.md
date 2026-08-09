# Hierarchy node

A header-only parent-child tree node.

- Header: `#include <scl/utility/hierarchy/node.h>`

## Overview

`scl::hierarchy::node<Payload, Allocator>` is a tree node that stores an internal
value of type `Payload` and owns zero or more child nodes in a `std::list`.
Moving children between nodes is O(1) (via `std::list::splice`); deep-copying a
subtree is O(N).

Iterating a node yields its **direct children** as `node &` references, making
`node` a standard range container whose `value_type` is `node` itself. The stored
value is reached with `get()`, not by iteration.

## Features

- Recursive parent-child hierarchy with a nullable parent back-pointer
- O(1) transfer (splice) of children between nodes
- Deep copy via the copy constructor; O(1) move via the move constructor
- Full `std::list`-compatible iterator interface
- Works with any allocator template

## API reference

### Construction

```cpp
node<int> root;                 // default-constructed value
node<int> n(42);                // value forwarded to the Payload constructor
node<std::string> s("hello");
```

Copy construction deep-copies the whole subtree; move construction transfers the
child list in O(1) and rewrites the children's parent pointers. Copy/move
**assignment is deleted** — use `transfer()` to move children between nodes.

### Value access

```cpp
payload_reference       get() noexcept;         // mutable internal value
const_payload_reference get() const noexcept;   // immutable internal value
```

### Parent access

```cpp
bool            has_parent() const noexcept;
reference       parent() noexcept;              // UB if !has_parent()
const_reference parent() const noexcept;
```

Query `has_parent()` before calling `parent()`. The parent link type has the
alias `node_pointer = node *`.

### Child queries

```cpp
bool      empty() const noexcept;
size_type size()  const noexcept;

reference       front() noexcept;
const_reference front() const noexcept;
reference       back()  noexcept;
const_reference back()  const noexcept;
```

### Inserting children

```cpp
iterator push_back (Argument && argument);      // copy/move an existing node to the back
iterator push_front(Argument && argument);      // copy/move an existing node to the front
iterator emplace_back (Arguments &&... args);   // construct the child's value in place
iterator emplace_front(Arguments &&... args);
iterator insert (const_iterator where, Argument && argument);
iterator emplace(const_iterator where, Arguments &&... args);
```

All insertion routes through `emplace`; every variant returns an `iterator` to the
inserted child (a deliberate deviation from `std::list`, avoiding a linear search
to recover the position).

### Removing children

```cpp
void     pop_back()  noexcept;
void     pop_front() noexcept;
iterator erase(const_iterator position) noexcept;
iterator erase(const_iterator first, const_iterator last) noexcept;
void     clear() noexcept;

node take      (const_iterator position) noexcept;   // detach and return a free-standing node
node take_first() noexcept;
node take_last () noexcept;
```

`take()` moves the child out (no parent); its own descendants keep their parent
pointers.

### Transferring children (O(1) splice)

```cpp
void transfer(node & from);
void transfer(node & from, const_iterator first);
void transfer(node & from, const_iterator first, const_iterator last);
void transfer(const_iterator where, node & from);
void transfer(const_iterator where, node & from, const_iterator first);
void transfer(const_iterator where, node & from, const_iterator first, const_iterator last);
```

### Iterators

Standard `begin/end/cbegin/cend` and reverse variants over direct children.

## Usage examples

### Example 1: build a small hierarchy

```cpp
using Node = scl::hierarchy::node<std::string>;

Node root("root");
auto child_it = root.emplace_back("child");
child_it->emplace_back("grandchild");

for (auto & c : root)
    std::cout << c.get() << '\n'; // "child"
```

### Example 2: query relationships

```cpp
Node parent("p");
auto it = parent.push_back("c");

assert(scl::hierarchy::has_parent(*it));
assert(scl::hierarchy::is_parent_of(parent, *it));
assert(scl::hierarchy::is_ancestor_of(parent, *it));
```

### Example 3: transfer children

```cpp
Node src;
src.emplace_back(1);
src.emplace_back(2);

Node dst;
dst.transfer(src);          // move all children; src becomes empty
assert(src.empty());
assert(dst.size() == 2);
```

### Example 4: extract a subtree

```cpp
Node parent;
auto it = parent.emplace_back("child");
it->emplace_back("grandchild");

Node extracted = parent.take_first();
assert(parent.empty());
assert(extracted.size() == 1);   // grandchild came along
assert(!extracted.has_parent());
```

## Notes

- `parent()` dereferences a null pointer when the node has no parent. Always guard
  with `scl::hierarchy::has_parent()`.
- The copy constructor recursively deep-copies all descendants; the result has no
  parent.
- The move constructor transfers children in O(1) and rewrites their parent
  pointers to the destination.
- `take()` moves a child out of the tree; for O(1) bulk moves use `transfer()`.

## See also

- [`example/hierarchy/node/hierarchy_node_example.cpp`](../../../../example/hierarchy/node/hierarchy_node_example.cpp) —
  runnable version of the examples above (org chart, `take()`/`transfer()` reorg).
