# observer_tuple

Fan-out adapter composing several `scl::hierarchy::tree` observers into one.

- Header: `#include <scl/utility/hierarchy/observer_tuple.h>`

## Overview

A `tree`'s `Observer` template parameter accepts exactly one observer type.
`observer_tuple<Tree, Observers...>` lets that one type be a composite: it
holds a `std::tuple<Observers...>` and forwards every notification it
receives to all of them, in declaration order. Each inner observer must
independently satisfy `concepts::observer` (see [tree.md](tree.md#observer-contract)).

## Features

- Forwards all four callbacks (`on_insert`, `on_erase`, `on_clear`,
  `on_change`) to every inner observer via `std::apply`
- Inner observers are reachable by index or by type through `get()`
- Adds no indirection beyond the tuple itself — no virtual dispatch, no
  heap allocation

## API reference

```cpp
template <typename Tree, typename... Observers>
class observer_tuple;
```

### Construction

Default-constructed; each inner observer is default-constructed in turn.

```cpp
template <typename T>
using two_counters = scl::hierarchy::observer_tuple<T, counter_a<T>, counter_b<T>>;

scl::hierarchy::tree<int, two_counters> t;   // t.get_observer() holds both counters
```

### Accessing inner observers

```cpp
tuple_type &       tuple() noexcept;         // the underlying std::tuple<Observers...>
tuple_type const & tuple() const noexcept;

template <std::size_t I> decltype(auto) get() noexcept;         // by index
template <std::size_t I> decltype(auto) get() const noexcept;
template <typename T>    decltype(auto) get() noexcept;         // by type
template <typename T>    decltype(auto) get() const noexcept;
```

### Observer contract (implementation)

```cpp
void on_insert(node_item item);                                  // tree::iterator
void on_erase(const_node_item item);                              // tree::const_iterator
void on_clear();
void on_change(const_payload_reference old_value, const_payload_reference new_value);
```

Each fires every inner observer's matching callback via `std::apply`, in
declaration order, unconditionally (there is no way to stop propagation partway
through the tuple).

## Usage examples

### Example 1: composing two observers

```cpp
template <typename Tree>
struct logger {
    void on_insert(typename Tree::iterator it)
    { std::cout << "insert: " << (*it).value() << '\n'; }
    void on_erase(typename Tree::const_iterator it)
    { std::cout << "erase: " << (*it).value() << '\n'; }
    void on_clear() { std::cout << "clear\n"; }
    void on_change(int old_v, int new_v)
    { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
};

template <typename Tree>
struct counter {
    std::size_t count = 0;
    void on_insert(typename Tree::iterator) { ++count; }
    void on_erase(typename Tree::const_iterator) { --count; }
    void on_clear() { count = 0; }
    void on_change(int, int) {}
};

template <typename T>
using multi_obs = scl::hierarchy::observer_tuple<T, logger<T>, counter<T>>;

scl::hierarchy::tree<int, multi_obs> t;
t.push_back(1);   // logger prints "insert: 1"; counter.count becomes 1
t.push_back(2);
std::cout << t.get_observer().get<counter<decltype(t)>>().count; // 2
```

### Example 2: accessing by index vs. by type

```cpp
auto & first_by_index = t.get_observer().get<0>();                       // logger<...>
auto & second_by_type = t.get_observer().get<counter<decltype(t)>>();    // counter<...>
```

## Notes

- Inner observers cannot see or influence each other — `observer_tuple` fans
  out, it does not chain or short-circuit.
- Order of the `Observers...` pack is the order callbacks are delivered in;
  it has no other effect since observers don't interact.
