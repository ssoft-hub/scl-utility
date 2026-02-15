# SCL.Utility

Common C++20 utilities not available in the standard library.

SCL.Utility is a header-only module of the ScL toolkit providing compile-time meta-programming helpers, preprocessor utilities, and extended type traits.

## Requirements

- C++20 compatible compiler (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.20+

## Installation

Add as a subdirectory in CMake and link against the interface target:

```cmake
add_subdirectory(module/utility)
target_link_libraries(your_target PRIVATE SCL::utility)
```

Then include the umbrella header or individual component headers:

```cpp
#include <scl/utility.h>           // everything
#include <scl/utility/meta.h>      // meta only
```

## Components

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
