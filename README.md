# ScL Utility

[![GitLab Pipeline](https://gitlab.com/ssoft-scl/scl-utility/badges/dev/pipeline.svg)](https://gitlab.com/ssoft-scl/scl-utility/-/pipelines)
[![GitLab Pages](https://img.shields.io/badge/pages-gitlab-orange)](https://ssoft-scl.gitlab.io/scl-utility/)
[![GitHub Pages](https://img.shields.io/badge/pages-github-blue)](https://ssoft-hub.github.io/scl-utility/)

Common C++ utilities not available in the standard library.

ScL Utility is a header-only module of the ScL Toolkit providing compile-time meta-programming helpers, preprocessor utilities, extended type traits, non-cryptographic hash utilities, and parent-child tree hierarchy structures. Licensed under [The Unlicense](LICENSE.md).

## Features

- **Attribute** — portable compiler attribute macros (`#include <scl/utility/attribute.h>`):
  - `SCL_NO_UNIQUE_ADDRESS` — portable `[[no_unique_address]]` (MSVC/GCC/Clang)
  - `SCL_ASSUME(expr)` — optimizer hint: expression is always true
  - `SCL_UNREACHABLE()` — mark unreachable code path
  - `SCL_FORCE_INLINE` / `SCL_NOINLINE` — inlining control
  - `SCL_NORETURN` — mark functions that never return
  - `SCL_DEPRECATED` / `SCL_DEPRECATED_MSG(msg)` — deprecation warnings
  - `SCL_FALLTHROUGH` — suppress implicit-fallthrough warning in `switch`
  - `SCL_NODISCARD` / `SCL_NODISCARD_MSG(msg)` — warn on discarded return value
  - `SCL_MAYBE_UNUSED` — suppress unused-entity warnings
  - `SCL_LIKELY` / `SCL_UNLIKELY` — C++20 statement branch hints
  - `SCL_LIKELY_EXPR(expr)` / `SCL_UNLIKELY_EXPR(expr)` — expression branch hints via `__builtin_expect`
  - `SCL_HOT` / `SCL_COLD` — function frequency hints
  - `SCL_LIFETIMEBOUND` — dangling-reference detection (`[[clang::lifetimebound]]`)
  - `SCL_UNSEQUENCED` / `SCL_REPRODUCIBLE` — purity annotations (C++26 semantics, GCC/Clang)
  - `SCL_INDETERMINATE` — C++26 opt-in to indeterminate storage
- **Meta** — compile-time name extraction without RTTI:
  - `type_name<T>` / `type_short_name<T>` — human-readable type names
  - `enum_name<V>` / `enum_short_name<V>` — enum member names
  - `symbol_name<S>` / `symbol_short_name<S>` — function and data member names
- **Preprocessor** — macro utilities:
  - `SCL_FORWARD` — identity macro for token forwarding in macro chains
  - `SCL_COUNTER_VALUE` / `SCL_COUNTER_NEXT` — compile-time counter via ADL
- **Type traits** — extended helpers:
  - Detection idiom (`is_detected`, `detected_t`, `detected_or`)
  - `forward_like` — forward with value category of another type
  - `member_like` — member pointer trait decomposition
  - `overload_cast<Args...>` — overloaded function pointer disambiguation
- **Hierarchy** — parent-child tree structures (`#include <scl/utility/hierarchy.h>`):
  - `node<Payload, Allocator>` — tree node owning children in `std::list`
  - `tree<Payload, Observer, Allocator>` — root-level node list with observer notifications
  - `observer_tuple<Tree, Observers...>` — fan-out adapter for N observers via `std::apply`
  - Free algorithm functions: `has_parent`, `parent_of`, `is_parent_of`, `is_ancestor_of`,
    `are_sibling`, `are_identical` — adapt to any type via ADL (`adl_parent`,
    `adl_has_parent`, `adl_identity`)
- **Hash** — non-cryptographic compile-time hash utilities (`#include <scl/utility/hash.h>`):
  - `fnv1a`, `djb2`, `sdbm` — 64-bit hashes over any byte range; chainable
  - `jenkins_ota` — 32-bit Jenkins one-at-a-time hash
  - `siphash` — SipHash-2-4 64-bit keyed hash (hash-flooding resistant)
  - `key<Hasher>` — strongly-typed digest; usable as `switch`/`case` label,
    STL unordered container key, and non-type template parameter (C++20 NTTP)
- **Flags** — type-safe bitmask over a scoped enum (`#include <scl/utility/flags.h>`):
  - `flags<Enum, bit_count>` — one bit per enumerator ordinal; `constexpr`-capable
  - bitwise `~ | & ^` and compound `|= &= ^=` (flags and single-flag forms),
    `operator[]` membership test
  - `all_of`/`any_of`/`none_of` predicates (variadic flag packs and whole-mask
    subset/intersection/disjoint forms), whole-mask `any`/`none`/`all`
  - a bidirectional range over the set flags (`begin`/`end`/`rbegin`/`rend`, `size`)

## Requirements

- C++20 compiler (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.20+

## Installation

Add the module as a subdirectory and link against the interface target:

```cmake
add_subdirectory(module/utility)
target_link_libraries(your_target PRIVATE scl::utility)
```

## Quick example

```cpp
#include <scl/utility/meta/type.h>
#include <scl/utility/meta/enum.h>
#include <iostream>

enum class Color { Red, Green, Blue };

int main() {
    // Compile-time type name
    constexpr auto name = scl::type_name<std::vector<int>>();
    std::cout << name << std::endl;

    // Compile-time enum member name
    constexpr auto color = scl::enum_name<Color::Red>();
    std::cout << color << std::endl;  // "Color::Red"
}
```

## Examples

Each directory under `example/` builds into one runnable program, named `utility_` plus
the directory path (`example/hierarchy_node` becomes `utility_hierarchy_node`). The same
sources appear on the Examples page of the generated reference.

- [`example/any`](example/any/any_example.cpp) — `any_view` over a typed lvalue and over a
  `std::any`, `any_arg` in parameter position, and an `any_switch` chain.
- [`example/flags`](example/flags/flags_example.cpp) — combining and querying flags, set
  algebra between two masks, and iterating the set flags.
- [`example/hash`](example/hash/key_nttp.cpp) — `hash::key` as a non-type template
  parameter: string-tagged types, specialisation by string value, compile-time dispatch.
- [`example/hierarchy_node`](example/hierarchy_node/node_example.cpp) — an organization
  chart built from `hierarchy::node`, reorganized with `take()` and `transfer()`.
- [`example/hierarchy_tree`](example/hierarchy_tree/tree_example.cpp) — a task board on
  `hierarchy::tree` with two observers composed through `observer_tuple`.
- [`example/meta`](example/meta/type_key_example.cpp) — a hashable handle over `type_key`,
  used as an unordered-container key.
- [`example/runtime_enum`](example/runtime_enum/enum_value.cpp) — `enum_value` against its
  compile-time counterpart, including values outside the enumerator set.
- [`example/runtime_type`](example/runtime_type/type_name.cpp) — compile-time and runtime
  type names side by side under polymorphism.

The programs under `example/quick_start/` are the shortest form of each topic, and are the
source of the snippets in the Quick start above:

- [`example/quick_start/any`](example/quick_start/any/any.cpp)
- [`example/quick_start/flags`](example/quick_start/flags/flags.cpp)
- [`example/quick_start/hash`](example/quick_start/hash/hash.cpp)
- [`example/quick_start/hierarchy`](example/quick_start/hierarchy/hierarchy.cpp)
- [`example/quick_start/meta`](example/quick_start/meta/meta.cpp)
- [`example/quick_start/type_traits`](example/quick_start/type_traits/type_traits.cpp)

## Doxygen

- [GitLab Pages](https://ssoft-scl.gitlab.io/scl-utility/dev/)
- [GitHub Pages](https://ssoft-hub.github.io/scl-utility/dev/)

## Documentation

- [English](doc/md/en/Main.md)
- [Russian](doc/md/ru/Main.md)

## See also

- [CHANGELOG](CHANGELOG.md)
- [CONTRIBUTING](CONTRIBUTING.md)
