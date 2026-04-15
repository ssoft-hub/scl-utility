# ScL.Utility

Common C++20 utilities not available in the standard library.

ScL.Utility is a header-only module of the ScL Toolkit providing compile-time meta-programming helpers, preprocessor utilities, and extended type traits.

## Requirements

- C++20 compatible compiler (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.20+

## Installation

Add as a subdirectory in CMake and link against the interface target:

```cmake
add_subdirectory(module/utility)
target_link_libraries(your_target PRIVATE scl::utility)
```

Then include the umbrella header or individual component headers:

```cpp
#include <scl/utility.h>           // everything
#include <scl/utility/meta.h>      // meta only
```

## Components

### Attribute — portable compiler attribute macros

| Component | Header | Description |
|-----------|--------|-------------|
| [SCL_NO_UNIQUE_ADDRESS](attribute/no_unique_address.md) | `<scl/utility/attribute/no_unique_address.h>` | Portable `[[no_unique_address]]` for MSVC/GCC/Clang |
| [SCL_ASSUME / SCL_UNREACHABLE](attribute/assume.md) | `<scl/utility/attribute/assume.h>` | Optimizer hints: assumption and unreachable marker |
| [SCL_FORCE_INLINE / SCL_NOINLINE](attribute/inline.md) | `<scl/utility/attribute/inline.h>` | Inlining control |
| [SCL_NORETURN](attribute/noreturn.md) | `<scl/utility/attribute/noreturn.h>` | Mark a function that never returns |
| [SCL_DEPRECATED / SCL_DEPRECATED_MSG](attribute/deprecated.md) | `<scl/utility/attribute/deprecated.h>` | Deprecation annotations |
| [SCL_FALLTHROUGH](attribute/fallthrough.md) | `<scl/utility/attribute/fallthrough.h>` | Intentional switch fall-through marker |
| [SCL_NODISCARD / SCL_NODISCARD_MSG](attribute/nodiscard.md) | `<scl/utility/attribute/nodiscard.h>` | Warn on discarded return value |
| [SCL_MAYBE_UNUSED](attribute/maybe_unused.md) | `<scl/utility/attribute/maybe_unused.h>` | Suppress unused-entity warnings |
| [SCL_LIKELY / SCL_UNLIKELY / SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR](attribute/likely.md) | `<scl/utility/attribute/likely.h>` | Branch-prediction hints |
| [SCL_UNSEQUENCED / SCL_REPRODUCIBLE](attribute/unsequenced.md) | `<scl/utility/attribute/unsequenced.h>` | Pure-function annotations (C++26 / gnu::const / gnu::pure) |
| [SCL_INDETERMINATE](attribute/indeterminate.md) | `<scl/utility/attribute/indeterminate.h>` | Opt-in to indeterminate (uninitialised) storage (C++26) |
| [SCL_HOT / SCL_COLD](attribute/hotcold.md) | `<scl/utility/attribute/hotcold.h>` | Function-frequency hints for hot/cold code paths |
| [SCL_LIFETIMEBOUND](attribute/lifetimebound.md) | `<scl/utility/attribute/lifetimebound.h>` | Dangling-reference detection via lifetime-bound parameter annotation |

### Meta — compile-time reflection

| Component | Header | Description |
|-----------|--------|-------------|
| [type_name](meta/type_name.md) | `<scl/utility/meta/type.h>` | Compile-time type name extraction |
| [enum_name](meta/enum_name.md) | `<scl/utility/meta/enum.h>` | Compile-time enum member name extraction |
| [symbol_name](meta/symbol_name.md) | `<scl/utility/meta/symbol.h>` | Compile-time function/member name extraction |

### Preprocessor — macro utilities

| Component | Header | Description |
|-----------|--------|-------------|
| [SCL_FORWARD](preprocessor/forward.md) | `<scl/utility/preprocessor/forward.h>` | Identity macro for token forwarding |
| [SCL_COUNTER](preprocessor/counter.md) | `<scl/utility/preprocessor/counter.h>` | Compile-time counter via ADL |

### Type traits — extended traits and helpers

| Component | Header | Description |
|-----------|--------|-------------|
| [detection](type_traits/detection.md) | `<scl/utility/type_traits/detection.h>` | Detection idiom (is_detected, etc.) |
| [forward_like](type_traits/forward_like.md) | `<scl/utility/type_traits/forward_like.h>` | Forward with value category of another type |
| [member_like](type_traits/member_like.md) | `<scl/utility/type_traits/member_like.h>` | Member pointer trait decomposition |
| [overload_cast](type_traits/overload_cast.md) | `<scl/utility/type_traits/overload_cast.h>` | Overloaded function disambiguation |

## Quick start

```cpp
#include <scl/utility/meta/type.h>
#include <scl/utility/meta/enum.h>
#include <iostream>

enum class Color { Red, Green, Blue };

int main() {
    std::cout << scl::type_name<std::vector<int>>() << std::endl;
    std::cout << scl::enum_name<Color::Red>() << std::endl;
}
```

## See also

- [Russian documentation](../ru/Main.md)
