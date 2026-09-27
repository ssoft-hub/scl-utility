# `type_name(obj)`

The function template `type_name(obj)` returns, through RTTI, the name of the dynamic type of an
object of a polymorphic class and the name of the static type of any other object. The function
template `type_short_name(obj)` returns the same name without the qualifiers of namespaces and
classes, without template arguments, and without a prefix such as `class` or `struct`.

- Header: `#include <scl/utility/runtime/type.h>`
- Both function templates are declared only where the macro `SCL_HAS_RTTI` is `1`.
- The code a caller writes otherwise is given in the section [Without RTTI](index.md#without-rtti)
  of the group page.

```cpp
template <typename T>
[[nodiscard]] ::std::string type_name(T const & obj);

template <typename T>
[[nodiscard]] ::std::string type_short_name(T const & obj);
```

## Semantics

- **Dynamic type:** the type is determined with the operator `typeid`. For an object reached
  through a pointer or a reference to a polymorphic base class, the function template therefore
  returns the name of the class the object was created as; through a pointer or a reference to a
  base class with no virtual function, it returns the name of that base class.
- **Spelling:** where the header `<cxxabi.h>` exists, as with GCC and with Clang on Linux, macOS
  and MinGW, the name is demangled with the function `abi::__cxa_demangle`. Elsewhere, as with
  MSVC and with Clang on the MSVC library, the result of `typeid().name()` is returned unchanged,
  with a prefix such as `class` or `struct` where that name carries one. Where demangling fails,
  the name is returned unchanged.
- **Short name:** for the type `app::Task<int>` the function template `type_short_name(obj)`
  returns `Task`. For a closure type or an unnamed class or enumeration it returns the name the
  compiler generates, such as `<lambda_1>` with MSVC.

## Examples

The classes of the examples:

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

Through a pointer to a base class, the name at compile time is that of the static type, and the
name at run time is that of the dynamic type:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp dynamic -->
```cpp
    std::unique_ptr<app::Base> const pointer = std::make_unique<app::Derived>();

    auto const static_name = scl::type_name<app::Base>(); // app::Base
    auto const dynamic_name = scl::type_name(*pointer);   // app::Derived, or struct app::Derived
```

The full and the short name at run time of an object of a specialization of a class template:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp short -->
```cpp
    std::unique_ptr<app::Base> const pointer = std::make_unique<app::Task<int>>();

    auto const full_name = scl::type_name(*pointer); // app::Task<int>, or struct app::Task<int>
    auto const short_name = scl::type_short_name(*pointer); // Task
```

## Compared with the compile-time name

| | function template `scl::type_name<T>()` | function template `scl::type_name(obj)` |
|---|---|---|
| Evaluated | at compile time | at run time |
| Returns | `std::string_view` | `std::string` |
| Names | the type written at the call site | the dynamic type of a polymorphic object |
| RTTI | not required | required |

## See also

- [Runtime](index.md)
- [`example/runtime/type_name/runtime_type_name_example.cpp`](../../../../example/runtime/type_name/runtime_type_name_example.cpp)
