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
  On MSVC, the result includes the `struct `, `class `, `union ` and `enum ` prefixes for
  user-defined types (`"struct MyStruct"`, `"enum Color"`), at the top level and inside
  template arguments alike. GCC and Clang omit these keywords. Inline namespaces and
  defaulted template arguments diverge on top of that. `std::string` measured on GCC 13.1,
  Clang 22.1 and MSVC 19.44:

  ```text
  GCC    std::__cxx11::basic_string<char>
  Clang  std::basic_string<char>
  MSVC   class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
  ```
- **What the result is for:**
  Display - a log line, an error message, a name shown to a human. It is not a stable
  identifier: no part of it is guaranteed to agree between compilers, so nothing should
  compare it against a literal, parse it, or persist it. For type identity use
  [`type_key`](type_key.md), which is designed for comparison; for the bare identifier of a
  namespace-scope type use `type_short_name<T>()`.
- **Qualified Names:**
  The result includes all namespace and class scope qualifiers (e.g., `std::vector<int>`).
- **Compile-time Evaluation:**
  The extraction logic is entirely `constexpr`, allowing the results to be used in static
  assertions or other compile-time contexts.

### Examples

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp type_name -->
```cpp
#include <scl/utility/meta/type.h>

#include <string>
#include <vector>

struct MyStruct
{};

constexpr auto npos = ::std::string_view::npos;

// Fundamental types render identically on every compiler.
static_assert(::scl::type_name<int>() == "int");

// A standard library type spells itself the way the toolchain does. std::string:
// GCC:   "std::__cxx11::basic_string<char>"
// Clang: "std::basic_string<char>"
// MSVC:  "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >"
static_assert(::scl::type_name<::std::string>().find("basic_string") != npos);

// A user-defined type carries the MSVC prefix.
// GCC/Clang: "MyStruct"  |  MSVC: "struct MyStruct"
static_assert(::scl::type_name<MyStruct>().find("MyStruct") != npos);

// The prefix reaches template arguments as well, so a rendered template name has no
// spelling common to all three compilers.
// GCC/Clang: "std::vector<MyStruct>"
// MSVC:      "class std::vector<struct MyStruct,class std::allocator<struct MyStruct> >"
static_assert(::scl::type_name<::std::vector<MyStruct>>().find("MyStruct") != npos);
```

### Typical Use Cases

Generating human-readable logs and error messages in generic code, and labelling a type in a
diagnostic without RTTI and without the mangled spelling of `typeid(T).name()`. Where a type has
to be recognized rather than shown, [`type_key`](type_key.md) is the facility for it.

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

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp type_short_name -->
```cpp
namespace app::core
{
    struct Task
    {};
} // namespace app::core

// The short name is the bare identifier on every compiler: namespace qualifiers, the
// MSVC prefix and the template arguments are all stripped.
static_assert(::scl::type_short_name<app::core::Task>() == "Task");
static_assert(::scl::type_short_name<::std::vector<MyStruct>>() == "vector");
```

## See also

- [`example/meta/type_name/meta_type_name_example.cpp`](../../../../example/meta/type_name/meta_type_name_example.cpp) —
  runnable version of both examples above, built and run by CI on every supported
  compiler.
