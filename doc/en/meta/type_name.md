# Type name utilities

Compile-time extraction of human-readable type names without RTTI.

- Header: `#include <scl/utility/meta/type.h>`

Contents:
- [`type_name<T>`](#type_name-method)
- [`type_short_name<T>`](#type_short_name-method)

---

## type_name<T>

Retrieves the fully qualified name of a template type `T` at compile-time as a `std::string_view`.

This utility leverages compiler-specific macros to capture the decorated function signature and
extract the exact type representation.

- Header: `#include <scl/utility/meta/type.h>`
- Declaration: `template <typename T> constexpr std::string_view type_name() noexcept;`

### Semantics

- **Compiler-specific output:**
  On MSVC, the result includes `struct `, `class `, or `union ` prefixes for user-defined types
  (e.g., `"struct MyStruct"`). GCC and Clang omit these keywords.
  Use `type_short_name<T>()` if a normalized, prefix-free name is needed.
- **Qualified Names:**
  The result includes all namespace and class scope qualifiers (e.g., `std::vector<int>`).
- **Compile-time Evaluation:**
  The extraction logic is entirely `constexpr`, allowing the results to be used in static
  assertions or other compile-time contexts.

### Examples

```cpp
#include <scl/utility/meta/type.h>
#include <vector>
#include <string>

using ::scl::type_name;

struct MyStruct {};

// Fundamental types
static_assert(type_name<int>() == "int");

// Standard library types
// Note: exact string may vary slightly by STL implementation
static_assert(type_name<std::string>().find("basic_string") != std::string_view::npos);

// User-defined types
// GCC/Clang: "MyStruct"  |  MSVC: "struct MyStruct"
static_assert(type_name<MyStruct>().find("MyStruct") != std::string_view::npos);

// Templates
static_assert(type_name<std::vector<MyStruct>>().find("vector<MyStruct>") != std::string_view::npos);
```

### Typical Use Cases

Generating human-readable logs, error messages in generic code, or implementing lightweight reflection
systems where type identities need to be displayed or compared as strings without the overhead of
`typeid(T).name()`.

---

## type_short_name<T>

Retrieves only the terminal identifier of the type `T`, stripping all namespace and class qualifiers.

- Header: `#include <scl/utility/meta/type.h>`
- Declaration: `template <typename T> constexpr std::string_view type_short_name() noexcept;`

### Description

This function processes the result of `type_name<T>()` and removes everything up to the last `::`
delimiter. Additionally, `struct`/`class`/`union`/`enum` prefixes (present on MSVC) and template
arguments are stripped, so the result is always the bare identifier.

### Example

```cpp
#include <scl/utility/meta/type.h>

namespace app::core { struct Task {}; }

int main() {
    constexpr auto full    = scl::type_name<app::core::Task>();       // "app::core::Task" (GCC/Clang)
    constexpr auto short_n = scl::type_short_name<app::core::Task>(); // "Task"
}
```
