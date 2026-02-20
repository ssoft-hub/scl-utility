# ScL.Utility

Common C++20 utilities not available in the standard library.

ScL.Utility is a header-only module of the ScL Toolkit providing compile-time meta-programming helpers, preprocessor utilities, and extended type traits. Licensed under [The Unlicense](LICENSE.md).

## Features

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

## Documentation

- [English](doc/en/Main.md)
- [Russian](doc/ru/Main.md)

## See also

- [CHANGELOG](CHANGELOG.md)
- [CONTRIBUTING](CONTRIBUTING.md)
