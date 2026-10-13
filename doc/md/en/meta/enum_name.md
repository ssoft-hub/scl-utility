# `enum_name<V>()`

The function template `enum_name<V>()` returns at compile time the name of the constant of
an enumeration that has the value `V`, with its namespaces and classes, and for a scoped enumeration
with its type. The function template `enum_short_name<V>()` returns the name of the constant alone.

- Header: `#include <scl/utility/meta/enum.h>`
- Both function templates need no RTTI and can be evaluated in a constant expression.

```cpp
namespace scl
{
    template <::scl::concepts::enum_type auto V>
    [[nodiscard]] constexpr ::std::string_view enum_name() noexcept;

    template <::scl::concepts::enum_type auto V>
    [[nodiscard]] constexpr ::std::string_view enum_short_name() noexcept;
}
```

## Semantics

- **Constraint:** the concept `scl::concepts::enum_type` is satisfied by a scoped and an unscoped
  enumeration type, so `V` is a value of either.
- **Full name:** a constant of a scoped enumeration is named with its type, a constant of
  an unscoped enumeration without it, and both with the namespaces and classes around
  the enumeration: `app::Color::Red` and `app::Active`.
- **Value without a constant:** for a value no constant of its type has, such as `Color{42}`,
  both function templates return an empty `std::string_view`.
- **Spelling:** the namespaces are spelled the way the compiler spells them. For a constant of
  an enumeration declared in an unnamed namespace, Clang writes
  `(anonymous namespace)::Hidden::Shown`, GCC `<unnamed>::Hidden::Shown` and MSVC
  `` `anonymous-namespace'::Hidden::Shown ``.
- **What the results are for:** the results of both function templates are for display. The spelling
  of `enum_name<V>()` can differ between compilers, as for a type in an unnamed namespace, and
  the spelling of both may change in a later version of the module, so nothing should compare
  a result against a literal, parse it, or persist it. The examples below compare results with
  literals to show the spelling of this version.

## Examples

The enumerations of the examples:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp types -->
```cpp
enum class Color
{
    Red,
    Green,
};

enum Status
{
    Active,
};
```

The full name:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp enum_name -->
```cpp
static_assert(scl::enum_name<Color::Red>() == "Color::Red");
static_assert(scl::enum_name<Active>() == "Active");
```

The short name:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp enum_short_name -->
```cpp
static_assert(scl::enum_short_name<Color::Green>() == "Green");
static_assert(scl::enum_short_name<Active>() == "Active");
```

A value no constant has:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp no_constant -->
```cpp
// No constant of Color has the value 42.
static_assert(scl::enum_name<Color{42}>().empty());
static_assert(scl::enum_short_name<Color{42}>().empty());
```

## See also

- [`scl::enum_string(value)`](../runtime/enum_string.md), the value a variable of an enumeration
  type holds, spelled at run time
- [`example/meta/enum_name/meta_enum_name_example.cpp`](../../../../example/meta/enum_name/meta_enum_name_example.cpp)

---

Previous: [Meta](index.md) | Next: [`symbol_name<S>()`](symbol_name.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
