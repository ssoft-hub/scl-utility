# ScL Utility

[![GitLab Pipeline](https://gitlab.com/ssoft-scl/scl-utility/badges/dev/pipeline.svg)](https://gitlab.com/ssoft-scl/scl-utility/-/pipelines)
[![GitLab Pages](https://img.shields.io/badge/pages-gitlab-orange)](https://ssoft-scl.gitlab.io/scl-utility/)
[![GitHub Pages](https://img.shields.io/badge/pages-github-blue)](https://ssoft-hub.github.io/scl-utility/)

Common C++20 utilities not available in the standard library.

ScL Utility is a header-only module of the ScL Toolkit providing compile-time meta-programming helpers, preprocessor utilities, extended type traits, and non-cryptographic hash utilities. Licensed under [The Unlicense](LICENSE.md).

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
- **Hash** — non-cryptographic compile-time hash utilities (`#include <scl/utility/hash.h>`):
  - `fnv1a`, `djb2`, `sdbm` — 64-bit hashes over any byte range; chainable
  - `jenkins_ota` — 32-bit Jenkins one-at-a-time hash
  - `siphash` — SipHash-2-4 64-bit keyed hash (hash-flooding resistant)
  - `key<Hasher>` — strongly-typed digest; usable as `switch`/`case` label,
    STL unordered container key, and non-type template parameter (C++20 NTTP)

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

## Doxygen

- [GitLab Pages](https://ssoft-scl.gitlab.io/scl-utility/dev/)
- [GitHub Pages](https://ssoft-hub.github.io/scl-utility/dev/)

## Documentation

- [English](doc/en/Main.md)
- [Russian](doc/ru/Main.md)

## See also

- [CHANGELOG](CHANGELOG.md)
- [CONTRIBUTING](CONTRIBUTING.md)
