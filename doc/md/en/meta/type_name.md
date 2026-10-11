# `type_name<T>()`

The function template `type_name<T>()` returns at compile time the name of the type `T` as
the compiler spells it. The function template `type_short_name<T>()` returns the same name without
the qualifiers of namespaces and classes, without template arguments, and without `const`,
`volatile`, a reference and a prefix such as `struct`.

- Header: `#include <scl/utility/meta/type.h>`
- Both function templates need no RTTI and can be evaluated in a constant expression.

```cpp
namespace scl
{
    template <typename T>
    [[nodiscard]] constexpr ::std::string_view type_name() noexcept;

    template <typename T>
    [[nodiscard]] constexpr ::std::string_view type_short_name() noexcept
        requires(see below class_enum_or_fundamental);
}
```

## Semantics

- **Spelling:** the name is taken from the signature the compiler gives a function template,
  `__FUNCSIG__` with MSVC and `__PRETTY_FUNCTION__` with GCC and Clang, with the namespaces,
  the classes, `const`, `volatile` and the reference the type has. MSVC writes the key `class`,
  `struct`, `union` or `enum` before the name of a class, a union or an enumeration, at the top
  level and in template arguments alike, and GCC and Clang write none. GCC spells a type of
  the standard library with every namespace its library declares it in, an inline one included, such
  as `std::__cxx11` with libstdc++, and Clang leaves an inline namespace out. MSVC writes
  the default arguments of the template of such a type, which GCC and Clang leave out. This is how
  `std::string` is spelled on GCC 13.1, on Clang 22.1 with the MSVC library and on MSVC 19.44:

  ```text
  GCC     std::__cxx11::basic_string<char>
  Clang   std::basic_string<char>
  MSVC    class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
  ```
- **What the results are for:** the results of both function templates are for display, such as
  a log line or an error message, not for identity. They differ between compilers and standard
  libraries, and their spelling may change in a later version of the module, so nothing should
  compare a result against a literal, parse it, or persist it. Types are compared by the keys
  [`scl::type_key_of<T>()`](type_key.md) returns.
- **Short name:** for the type `app::Task<int> const &` the function template `type_short_name<T>()`
  returns `Task`. For a closure type or an unnamed class or enumeration it returns the name
  the compiler generates, such as `<lambda()>` with GCC, and for a fundamental type the name
  the compiler spells it with, without its namespace: `int` for `int`, `nullptr_t` with GCC and
  Clang and `nullptr` with MSVC for `std::nullptr_t`, and `long long unsigned int` with GCC and
  `unsigned __int64` with MSVC for `unsigned long long`.
- **Constraint:** in the declaration of `type_short_name<T>()`, the constraint
  `class_enum_or_fundamental` is satisfied by a class, a union, an enumeration and a fundamental
  type, with or without `const`, `volatile` and a reference. A pointer, an array, a function and
  a pointer to a member do not satisfy it, and a call with such a type does not compile.
- **Two types of one name:** GCC and Clang spell a pointer to `operator-` before a closing `>` as
  `operator->`, so two types can get one name. Take a class `P` with both operators and class
  templates `Holder` and `Wrap`, each with a member template `Inner`. Clang names both
  `Wrap<Holder<&P::operator- >>::Inner<&P::operator-> >` and
  `Wrap<Holder<&P::operator-> >::Inner<&P::operator- >>` as
  `Wrap<Holder<&P::operator->>::Inner<&P::operator->>`, so the function template cannot tell the two
  apart and returns `Wrap` for both, while the first is a specialization of `Inner`.

## Examples

The types of the examples:

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp types -->
```cpp
namespace app
{
    struct Item
    {};

    template <typename T>
    struct Task
    {};
} // namespace app
```

The full name, checked only in the part every compiler spells alike:

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp type_name -->
```cpp
// Every compiler spells int alike.
static_assert(scl::type_name<int>() == "int");

// MSVC writes the class key before the name of a class: app::Item, or struct app::Item.
static_assert(scl::type_name<app::Item>().ends_with("app::Item"));

// GCC spells a type of the standard library with an inline namespace of its library, such as
// std::__cxx11::basic_string<char>, and Clang leaves that namespace out, as in
// std::basic_string<char>.
static_assert(scl::type_name<std::string>().find("string") != std::string_view::npos);
```

The short name:

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp type_short_name -->
```cpp
static_assert(scl::type_short_name<app::Task<int>>() == "Task");
static_assert(scl::type_short_name<app::Item const &>() == "Item");
static_assert(scl::type_short_name<std::vector<app::Item>>() == "vector");
```

## See also

- [`scl::type_name(obj)`](../runtime/type_name.md), the name of the type of an object at run time
- [`scl::type_key_of<T>()`](type_key.md), the identity of a type
- [`example/meta/type_name/meta_type_name_example.cpp`](../../../../example/meta/type_name/meta_type_name_example.cpp)

---

Previous: [`symbol_name<S>()`](symbol_name.md) | Next: [`is_tu_local<T>`](is_tu_local.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
