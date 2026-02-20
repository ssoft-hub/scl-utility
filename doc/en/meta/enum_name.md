# Enum name utilities

Compile-time introspection of enumeration member identifiers.

- Header: `#include <scl/utility/meta/enum.h>`

Contents:
- [`enum_name<V>`](#enum_name-v)
- [`enum_short_name<V>`](#enum_short_name-v)

---

## `enum_name<V>`

Retrieves the qualified string representation of an enumeration member `V` at compile-time.

- Header: `#include <scl/utility/meta/enum.h>`
- Declaration: `template <auto V> requires Enum<decltype(V)> constexpr std::string_view enum_name() noexcept;`

### Semantics

- **Cross-platform Normalization:** 
  MSVC often includes the `enum ` prefix in its function signatures. This utility automatically detects and removes it to ensure the result is consistent with GCC and Clang.
- **Qualified Result:** 
  Depending on the compiler and enum type (scoped vs. unscoped), the result typically includes the type name (e.g., `Color::Red`).

### Examples

```cpp
#include <scl/utility/meta/enum.h>

enum class Color { Red, Green, Blue };
enum Status { Active };

// Scoped enums
static_assert(scl::enum_name<Color::Red>() == "Color::Red");

// Unscoped enums
static_assert(scl::enum_name<Active>() == "Active");
```

---

## `enum_short_name<V>`

Retrieves only the identifier of the enum member `V`, stripping any type or namespace qualifiers.

- Header: `#include <scl/utility/meta/enum.h>`
- Declaration: `template <auto V> requires Enum<decltype(V)> constexpr std::string_view enum_short_name() noexcept;`

### Example

```cpp
#include <scl/utility/meta/enum.h>

namespace gfx { enum class Mode { Fast, Safe }; }

void log_mode() {
    constexpr auto name = scl::enum_short_name<gfx::Mode::Fast>();
    std::cout << "Current mode: " << name << std::endl; // Output: "Current mode: Fast"
}
```
