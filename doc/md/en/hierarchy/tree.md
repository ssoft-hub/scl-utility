# Hierarchy tree

An observer-aware tree that notifies on value and structural changes.

- Header: `#include <scl/utility/hierarchy/tree.h>`

## Overview

`scl::hierarchy::tree<Payload, Observer, Allocator>` owns a root-level `std::list`
of `node<Payload, Allocator>` instances and routes every structural mutation
through an `Observer<tree>` policy. "Multi-root" means exactly this: the tree
holds a *list* of root nodes (a forest), not a single mandatory root — you can
`push_back` several independent roots, each with its own subtree.

Nodes are accessed through the `reference` / `const_reference` proxies, which pair
a reference to the tree with a reference to a node so that a node cannot be
mutated without the tree being notified. Composing several observers into one
is covered separately in [observer_tuple.md](observer_tuple.md).

## Features

- Observer notification on every insert, erase, clear, and payload change
- Multi-root (forest): `push_back`/`push_front` add independent root subtrees
- `reference`/`const_reference` proxies mirror `node`'s full child-management
  API while routing every mutation through the observer
- O(1) `transfer` (splice) both at root level and through a `reference`,
  within or across trees
- `remove()` detaches a node from wherever it lives — its parent or the root
  list — without the caller needing to know which
- Full `std::list`-compatible iterator interface over the root nodes

## Observer contract

```cpp
template <typename Tree>
struct my_observer {
    void on_insert(Tree::iterator);              // after a node is inserted
    void on_erase (Tree::const_iterator);        // before a node is erased
    void on_clear ();                                     // before the tree is cleared
    void on_change(Tree::const_payload_reference old_value,
                   Tree::const_payload_reference new_value);
};
```

- `on_insert` fires top-down: a parent is reported before its children when a
  subtree is inserted.
- `on_erase` fires bottom-up: the deepest descendant is reported first.
- The `Observer` template parameter is checked against `concepts::observer` at
  instantiation — a non-conforming type fails to compile with a clear message,
  not a wall of substitution errors.

## API reference

### tree

```cpp
template <typename Payload,
          template <typename> class Observer,
          template <typename> class Allocator = std::allocator>
class tree;
```

Default-constructed; empty. No other constructors — build a tree by inserting
into it.

#### Observer access

```cpp
observer_reference       get_observer() noexcept;
observer_const_reference get_observer() const noexcept;
```

#### Root queries

```cpp
bool      empty() const noexcept;
size_type size()  const noexcept;

reference       front() noexcept;      // and back()
const_reference front() const noexcept;
```

#### Root insertion

```cpp
iterator push_back (Argument && argument);          // append a root; fires on_insert
iterator push_front(Argument && argument);          // prepend a root; fires on_insert
iterator emplace_back (Arguments &&... arguments);  // construct a root payload in place
iterator emplace_front(Arguments &&... arguments);
iterator insert (const_iterator where, Argument && argument);
iterator emplace(const_iterator where, Arguments &&... arguments);
```

#### Root removal

```cpp
void     pop_back()  noexcept;
void     pop_front() noexcept;
Iterator erase(Iterator position) noexcept;              // fires on_erase bottom-up
Iterator erase(Iterator first, Iterator last) noexcept;
void     clear() noexcept;                                // fires on_clear

void remove(Iterator position);   // detaches from its parent, or from the root
                                   // list if position has no parent — the
                                   // caller doesn't need to know which
```

#### Transfer (O(1) splice, root level)

```cpp
void transfer(tree & from);
void transfer(tree & from, const_iterator first);
void transfer(tree & from, const_iterator first, const_iterator last);
void transfer(const_iterator where, tree & from);
void transfer(const_iterator where, tree & from, const_iterator first);
void transfer(const_iterator where, tree & from, const_iterator first, const_iterator last);
```

Splices root subtrees between two trees (or within the same tree). A
cross-tree transfer fires `on_erase` on the source and `on_insert` on the
destination for every moved node.

#### Iterators

Standard `begin/end/cbegin/cend` and reverse variants over the root nodes.

### reference (mutable node proxy)

Never constructed directly — obtained by dereferencing a `tree::iterator` or
returned from a `tree`/`reference` insertion method.

#### Payload access

```cpp
const_payload_reference value() const noexcept;   // the node's payload
void set_value(Arguments &&... arguments);         // replace the payload; fires on_change
```

#### Identity and the underlying node

```cpp
tree_reference       tree() noexcept;              // the owning tree
const_tree_reference tree() const noexcept;
const_node_reference leaf() const noexcept;         // the underlying node
bool operator==(const_reference other) const noexcept;   // node-pointer identity
```

#### Parent access

```cpp
bool            has_parent() const noexcept;
reference       parent() noexcept;
const_reference parent() const noexcept;
```

#### Child queries

```cpp
bool      empty() const noexcept;
size_type size()  const noexcept;
reference front() noexcept;               // and back()
const_reference front() const noexcept;
```

#### Insertion

```cpp
iterator push_back (Argument && argument);          // append a child; fires on_insert
iterator push_front(Argument && argument);          // prepend a child; fires on_insert
iterator emplace_back (Arguments &&... arguments);  // construct a child payload in place
iterator emplace_front(Arguments &&... arguments);
iterator insert (const_iterator where, Argument && argument);
iterator emplace(const_iterator where, Arguments &&... arguments);
```

#### Removal and extraction

```cpp
void     pop_back()  noexcept;
void     pop_front() noexcept;
Iterator erase(Iterator position) noexcept;         // fires on_erase bottom-up
Iterator erase(Iterator first, Iterator last) noexcept;
void     clear() noexcept;                           // fires on_erase for every child

node take      (const_iterator position) noexcept;   // detach; fires on_erase
node take_first() noexcept;
node take_last () noexcept;
```

#### Transfer (O(1) splice)

```cpp
void transfer(reference from);
void transfer(reference from, const_iterator first);
void transfer(reference from, const_iterator first, const_iterator last);
void transfer(const_iterator where, reference from);
void transfer(const_iterator where, reference from, const_iterator first);
void transfer(const_iterator where, reference from, const_iterator first, const_iterator last);
```

A cross-tree transfer fires `on_erase` on the source tree and `on_insert` on
the destination tree for every moved node.

#### Iterators

Standard `begin/end/cbegin/cend` and reverse variants over direct children.

### const_reference

The read-only counterpart of `reference`, obtained by dereferencing a
`tree::const_iterator` or converting from a `reference`. Mirrors `reference`'s
read-only subset:

```cpp
const_payload_reference value() const noexcept;
const_tree_reference    tree()  const noexcept;
const_node_reference    leaf()  const noexcept;

bool            has_parent() const noexcept;
const_reference parent()     const noexcept;

bool      empty() const noexcept;
size_type size()  const noexcept;
const_reference front() const noexcept;   // and back()

bool operator==(const_reference other) const noexcept;   // node-pointer identity

// begin/end/cbegin/cend and reverse variants over direct children
```

There is no mutation, insertion, removal, or transfer on `const_reference`.

## Usage examples

### Example 1: a simple observer

```cpp
template <typename Tree>
struct logger {
    void on_insert(Tree::iterator it)
    { std::cout << "insert: " << (*it).value() << '\n'; }
    void on_erase(Tree::const_iterator it)
    { std::cout << "erase: " << (*it).value() << '\n'; }
    void on_clear() { std::cout << "clear\n"; }
    void on_change(int old_v, int new_v)
    { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
};

scl::hierarchy::tree<int, logger> t;
auto root = t.push_back(1);    // insert: 1
(*root).push_back(2);          // insert: 2
(*root).set_value(10);         // change: 1 -> 10
t.clear();                     // clear
```

### Example 2: query the hierarchy through a reference

The free algorithm functions in `algorithm.h` find `reference`/`const_reference`
via ADL (see [algorithm.md](algorithm.md)) — call them directly, no unwrapping
needed:

```cpp
scl::hierarchy::tree<std::string, logger> t;
auto root  = t.push_back("root");
auto child = (*root).push_back("child");
auto grand = (*child).push_back("grand");

assert(scl::hierarchy::is_ancestor_of(*root, *grand));
assert(scl::hierarchy::are_sibling(*child, *child));
```

### Example 3: transfer between references

```cpp
scl::hierarchy::tree<int, logger> t;
auto r1 = t.push_back(0);
auto r2 = t.push_back(0);
(*r1).push_back(1);
(*r1).push_back(2);

(*r2).transfer(*r1);  // move all children of r1 into r2
assert((*r1).empty());
assert((*r2).size() == 2);
```

### Example 4: remove() detaches by identity, from parent or root list

```cpp
scl::hierarchy::tree<int, logger> t;
auto root  = t.push_back(1);          // insert: 1
auto child = (*root).push_back(2);    // insert: 2

t.remove(child);    // child has a parent -> erased through (*root), not through t
assert((*root).empty());

t.remove(root);      // root has no parent -> erased from the root list directly
assert(t.empty());
```

## Notes

- `on_erase` fires bottom-up: the deepest descendant is notified first.
- `on_insert` fires top-down: a parent is notified before its children when a
  subtree is inserted.
- A cross-tree `transfer()` fires `on_erase` on the source tree and `on_insert`
  on the destination tree.
- `reference`/`const_reference` are proxies, not owning handles — they stay
  valid exactly as long as the underlying node does.

## See also

- [`example/hierarchy_tree/tree_example.cpp`](../../../../example/hierarchy_tree/tree_example.cpp) —
  runnable version: a task board with two observers composed via
  `observer_tuple`, keeping independent indexes in sync across `set_value()`
  and cross-parent `transfer()`.
