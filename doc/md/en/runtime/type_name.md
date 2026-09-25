# `type_name(obj)`

Returns the name of the type an object of a polymorphic class was created as, and of the
static type for any other object, read through RTTI.
`type_short_name(obj)` returns the same name with no qualifiers.

- Header: `#include <scl/utility/runtime/type.h>`
- Declared only where `SCL_HAS_RTTI` is `1`; see [Without RTTI](index.md#without-rtti)

```cpp
template <typename T>
[[nodiscard]] ::std::string type_name(T const & obj);

template <typename T>
[[nodiscard]] ::std::string type_short_name(T const & obj);
```

## Semantics

- **Dynamic type:** `typeid(obj)` resolves the type, so an object reached through a pointer
  or a reference to a polymorphic base answers the class it was created as. Through a pointer
  or a reference to a base with no virtual function, the answer is that base.
- **Spelling:** where `<cxxabi.h>` exists, as with GCC and with Clang on Linux, macOS and
  MinGW, the name goes through `abi::__cxa_demangle`. Elsewhere, as with MSVC and with Clang
  on the MSVC library, `typeid().name()` is returned as it is, with a prefix such as `class`
  or `struct` where the type has one.
- **Short name:** `type_short_name(obj)` drops the namespace and class qualifiers and the
  template arguments: `app::Task<int>` answers `Task`.

## Examples

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp types -->
```cpp
namespace app
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Derived : Base
    {};

    template <typename T>
    struct Task : Base
    {};
} // namespace app
```

Through a pointer or a reference to a base the compile-time name is the static type's and the
runtime name the dynamic type's:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp dynamic -->
```cpp
    ::std::unique_ptr<::app::Base> const pointer = ::std::make_unique<::app::Derived>();

    auto const static_name = ::scl::type_name<::app::Base>(); // app::Base
    auto const dynamic_name = ::scl::type_name(*pointer); // app::Derived, or struct app::Derived
```

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp short -->
```cpp
    ::std::unique_ptr<::app::Base> const pointer = ::std::make_unique<::app::Task<int>>();

    auto const full_name = ::scl::type_name(*pointer); // app::Task<int>, or struct app::Task<int>
    auto const short_name = ::scl::type_short_name(*pointer); // Task
```

## Against the compile-time name

| | `scl::type_name<T>()` | `scl::type_name(obj)` |
|---|---|---|
| Evaluated | at compile time | at run time |
| Returns | `std::string_view` | `std::string` |
| Names | the type written at the call site | the dynamic type of a polymorphic object |
| RTTI | not required | required |

## See also

- [Runtime](index.md)
- [`example/runtime/type_name/runtime_type_name_example.cpp`](../../../../example/runtime/type_name/runtime_type_name_example.cpp)
