# Hierarchy Tree

Observer-aware multi-root tree with value and structural change notifications.

- Header: `#include <scl/utility/hierarchy/tree.h>`

## Overview

`scl::hierarchy::tree<Value, Observer, Allocator>` wraps a collection of root
`node<Value>` instances and routes all structural mutations through an
`Observer<tree>` policy.  Users access nodes via `node_wrapper` /
`const_node_wrapper` proxies that bundle a tree reference with a node reference,
making it impossible to mutate a node without notifying the tree.

`tuple_of_observers` allows composing multiple independent observers into one.

## Observer Contract

An observer must provide:

```cpp
template <typename Tree>
struct my_observer {
    void after_insert(typename Tree::node_wrapper_iterator);
    void before_erase(typename Tree::const_node_wrapper_iterator);
    void before_clear();
    void on_change(typename Tree::const_reference_type old_val,
                   typename Tree::const_reference_type new_val);
};
```

`after_insert` fires after the node is inserted (the iterator is already
valid).  `before_erase` fires before the node is removed (the iterator still
points to a live node).  For nested erasure, `before_erase` fires bottom-up
(deepest descendant first).

## API Reference

### tree

```cpp
template <typename Value,
          template <typename> class Observer,
          template <typename> class Allocator = std::allocator>
class tree;
```

#### Methods

```cpp
observer_reference       observer() noexcept;
observer_const_reference observer() const noexcept;

bool empty() const noexcept;

node_wrapper_iterator append (/* Value args */);
node_wrapper_iterator prepend(/* Value args */);
node_wrapper_iterator emplace(const_node_wrapper_iterator before, /* Value args */);

Iterator erase(Iterator pos) noexcept;
Iterator erase(Iterator first, Iterator last) noexcept;
void     clear() noexcept;

void remove(Iterator pos);   // removes from parent or root list

node_wrapper_iterator       begin()  noexcept;
const_node_wrapper_iterator begin()  const noexcept;
const_node_wrapper_iterator cbegin() const noexcept;
node_wrapper_iterator       end()    noexcept;
const_node_wrapper_iterator end()    const noexcept;
const_node_wrapper_iterator cend()   const noexcept;
```

### node_wrapper

```cpp
void         set_value(const_reference_type val); // triggers on_change
const_reference_type value() const;

node_wrapper       parent() noexcept;
const_node_wrapper parent() const noexcept;

bool       empty() const noexcept;
node_count count() const;

node_wrapper       first() noexcept;
const_node_wrapper first() const noexcept;
node_wrapper       last()  noexcept;
const_node_wrapper last()  const noexcept;

node_wrapper_iterator append (/* Value args */);
node_wrapper_iterator prepend(/* Value args */);
node_wrapper_iterator emplace(const_node_wrapper_iterator before, /* Value args */);

Item erase(Item pos) noexcept;
Item erase(Item first, Item last) noexcept;
void clear() noexcept;

node release(const_node_wrapper_iterator pos) noexcept;
node const& access() const noexcept;

void transfer(node_wrapper from);
void transfer(const_node_wrapper_iterator before, node_wrapper from);
void transfer(node_wrapper from, const_node_wrapper_iterator from_pos);
// ... (full transfer overloads mirroring node::transfer)

node_wrapper_iterator       begin()  noexcept;
const_node_wrapper_iterator cbegin() const noexcept;
node_wrapper_iterator       end()    noexcept;
const_node_wrapper_iterator cend()   const noexcept;
```

### tuple_of_observers

```cpp
template <typename Tree, typename... Observers>
class tuple_of_observers;
```

Fans out all four observer callbacks to each inner observer.

Access inner observers via index or type:

```cpp
auto& obs0 = tree.observer().get<0>();
auto& obs1 = tree.observer().get<my_observer<Tree>>();
```

## Usage Examples

### Example 1: Simple observer

```cpp
template <typename Tree>
struct logger {
    void after_insert(typename Tree::node_wrapper_iterator it)
    { std::cout << "insert: " << (*it).value() << '\n'; }
    void before_erase(typename Tree::const_node_wrapper_iterator it)
    { std::cout << "erase: " << (*it).value() << '\n'; }
    void before_clear() { std::cout << "clear\n"; }
    void on_change(int old_v, int new_v)
    { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
};

scl::hierarchy::tree<int, logger> t;
auto root = t.append(1);       // insert: 1
(*root).append(2);             // insert: 2
(*root).set_value(10);         // change: 1 -> 10
t.clear();                     // clear
```

### Example 2: Composing observers with tuple_of_observers

```cpp
template <typename T>
using multi_obs = scl::hierarchy::tuple_of_observers<T, logger<T>, counter<T>>;

scl::hierarchy::tree<int, multi_obs> t;
t.append(1);
t.append(2);
std::cout << t.observer().get<counter<decltype(t)>>().count; // 2
```

### Example 3: Hierarchy queries via node_wrapper

```cpp
scl::hierarchy::tree<std::string, logger> t;
auto root  = t.append("root");
auto child = (*root).append("child");
auto grand = (*child).append("grand");

assert(scl::hierarchy::is_ancestor_of(*root, *grand));
assert(scl::hierarchy::are_sibling(*child, *child)); // same object
```

### Example 4: Transfer between wrappers

```cpp
scl::hierarchy::tree<int, logger> t;
auto r1 = t.append(0);
auto r2 = t.append(0);
(*r1).append(1);
(*r1).append(2);

(*r2).transfer(*r1);  // move all children of r1 into r2
assert((*r1).empty());
assert((*r2).count() == 2);
```

## Notes

- `before_erase` fires bottom-up: deepest descendants notify first.
- `after_insert` fires top-down: parent notifies before its children
  when inserting a subtree (via `transfer` or `release`/`append`).
- `release()` deep-copies the raw node; use it to extract a subtree
  and re-insert it elsewhere.
- Cross-tree `transfer()` fires `before_erase` on the source tree and
  `after_insert` on the destination tree.
