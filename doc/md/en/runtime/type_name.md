# `type_name(obj)`

The function template `type_name(obj)` returns, through RTTI, the name of the dynamic type of an
object of a polymorphic class and the name of the static type of any other object. The function
template `type_short_name(obj)` returns the same name without the qualifiers of namespaces and
classes, without template arguments, and without a prefix such as `class` or `struct`.

- Header: `#include <scl/utility/runtime/type.h>`
- Both function templates are declared only where the macro
  [`SCL_HAS_RTTI`](../preprocessor/rtti.md) is `1`. The code a caller writes where it is `0` is
  given in the section [Without RTTI](index.md#without-rtti) of the group page.
- Where the macro [`SCL_HAS_THREADS`](../preprocessor/threads.md) is `1`, both function templates
  may be called from several threads at once.

```cpp
namespace scl
{
    template <typename T>
    [[nodiscard]] ::std::string type_name(T const & obj);

    template <typename T>
    [[nodiscard]] ::std::string type_short_name(T const & obj);
}
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
  the name is returned unchanged. Where the name is demangled, each type is demangled once, and its
  name is kept until the program ends, so a destructor that runs after `main` returns can call the
  function templates as well.
- **What the results are for:** the results of both function templates are for display, such as a
  log line or an error message, not for identity: two distinct types, such as classes of one name
  in the unnamed namespaces of two translation units, can have one name. The spelling differs
  between compilers and standard libraries and may change in a later version of the module, so
  nothing should compare a result against a literal, parse it, or persist it.
  `std::type_index(typeid(obj))` identifies the type the function templates name.
- **Short name:** for the type `app::Task<int>` the function template `type_short_name(obj)`
  returns `Task`. For a closure type or an unnamed class or enumeration it returns the name the
  compiler generates, such as `<lambda_1>` with MSVC. For any other type the result is its name
  with everything through the last `::` outside brackets, a leading `class` or `struct` and
  everything from the first `<` removed, which need not be an identifier: `Derived*` with GCC for
  `app::Derived *`, but `Task` for `app::Task<int> *`.

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

The full and the short name at run time of the type of an object of a specialization of a class
template:

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

- [`scl::type_name<T>()`](../meta/type_name.md), the name at compile time
- [`example/runtime/type_name/runtime_type_name_example.cpp`](../../../../example/runtime/type_name/runtime_type_name_example.cpp)

---

Previous: [Runtime](index.md) | Next: [`enum_string(value)`](enum_string.md) |
[Russian documentation](../../ru/runtime/type_name.md)
