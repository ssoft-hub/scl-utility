# `enum_string(value)`

The function template `enum_string` returns a value of an enumeration type spelled as `Type::N`,
where the part `Type` is the name of the enumeration type and the part `N` is the number the value
holds. It needs no RTTI, so it is declared in a build with RTTI disabled as well. An overload takes
a function object as its second argument and puts the text that object returns for the number in
place of `N` unchanged.

- Header: `#include <scl/utility/runtime/enum.h>`

```cpp
template <::scl::concepts::enum_type E>
[[nodiscard]] ::std::string enum_string(E value);

namespace scl::concepts
{
    template <typename Format, typename Enum>
    concept enum_string_format = /* see Function object below */;
}

template <::scl::concepts::enum_type E, typename Format>
[[nodiscard]] ::std::string enum_string(E value, Format && format)
    requires ::scl::concepts::enum_string_format<Format, E>;
```

## Semantics

- **Type:** the part `Type` is the short name the function template `scl::type_short_name<E>()`
  returns at compile time, so an enumeration type declared in a namespace or a class is spelled
  without the name of the namespace or the class. For an enumeration type with no name, `Type` is
  the name the compiler generates, which differs between compilers and may change in a later
  version of the module.
- **Number:** the part `N` is the value in its underlying type, in decimal and with the sign of
  that type. For a character or `bool` underlying type, `N` is a number as well: the value `'P'` is
  spelled as `80`.
- **Any value:** a value no enumeration constant has is spelled the same way as the value of a
  constant.

## Function object

The overload `enum_string(value, format)` calls the function object `format` as it is passed, so a
lambda declared with the specifier `mutable` is accepted too. The overload passes the number as a
constant lvalue of an integer type of the size and signedness of the underlying type, or of the
type `unsigned char` for the underlying type `bool`.

The function object must return a value of any type implicitly convertible to `std::string_view`:
an object of the type `std::string` or `std::string_view`, a reference to one, or a pointer to a
null-terminated string.

Where the function object has a single signature, the type of its first parameter is determined
with the alias template `scl::signature::parameter_t`. That type, with the reference and the
cv-qualifiers removed, must be an integral type of the signedness of the underlying type and no
narrower than it. `bool` and the character types count as integral types. The width is the number
of value bits, so `bool` is one bit wide.

| Underlying type | Parameter type | The call |
|---|---|---|
| `char16_t` | `char16_t`, `char32_t`, `std::uint16_t`, `std::uint32_t` | compiles |
| `char16_t` | `std::uint8_t`, narrower | does not compile |
| `char16_t` | `std::int32_t`, of the other signedness | does not compile |
| `bool` | `bool`, `unsigned char` | compiles |
| `std::uint8_t` | `bool`, one bit wide | does not compile |
| `char16_t` | `std::uint32_t &`, a reference to non-const | does not compile, since the number comes as a constant lvalue |

A generic lambda, an object of a class whose call operator is overloaded, and the objects the
function templates `std::ref` and `std::bind_front` return have no single signature. For such a
function object, and for a function whose parameter list is `(...)` alone, since it has no first
parameter, only the call and the type of its result are checked.

All of these conditions are collected in the concept `scl::concepts::enum_string_format<Format, E>`,
so a function object can be checked before it is passed to the function template
`scl::enum_string`.

## Examples

The enumeration types of the examples:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp types -->
```cpp
enum class Color : int
{
    Red = 1,
    Blue = -3,
};

enum class Grade : char
{
    Pass = 'P',
};

namespace net
{
    enum class Status : unsigned
    {
        Err = 42,
    };
} // namespace net
```

A value a constant has, beside the name of that constant obtained at compile time:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp named -->
```cpp
    constexpr auto name = scl::enum_name<Color::Red>(); // Color::Red
    auto const red = scl::enum_string(Color::Red);      // Color::1
    auto const blue = scl::enum_string(Color::Blue);    // Color::-3
```

A value no constant has:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp unnamed -->
```cpp
    auto const unnamed = scl::enum_string(Color{42}); // Color::42
```

A character underlying type, and an enumeration type declared in a namespace:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp underlying -->
```cpp
    auto const grade = scl::enum_string(Grade::Pass);       // Grade::80
    auto const status = scl::enum_string(net::Status::Err); // Status::42
```

A function object that returns the number spelled in binary, as wide as the type of the number:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp formatted -->
```cpp
    auto const binary = [](auto number) {
        return std::bitset<sizeof(number) * CHAR_BIT>(number).to_string();
    };
    auto const grade = scl::enum_string(Grade::Pass, binary); // Grade::01010000
```

A check of two function objects against the concept, before either is passed:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp concept -->
```cpp
constexpr auto wide = [](unsigned long long number) { return std::to_string(number); };
constexpr auto narrow = [](unsigned char number) { return std::to_string(number); };

static_assert(scl::concepts::enum_string_format<decltype(wide), net::Status>);
static_assert(!scl::concepts::enum_string_format<decltype(narrow), net::Status>);
```

## Compared with the compile-time name

| | function template `scl::enum_name<V>()` | function template `scl::enum_string(value)` |
|---|---|---|
| Evaluated | at compile time | at run time |
| Returns | `std::string_view` | `std::string` |
| Result holds | the constant, `Color::Red` | the number, `Color::1` |
| Takes | a constant named at compile time | any value |

## See also

- [Runtime](index.md)
- [`scl::signature`](../type_traits/signature.md)
- [`example/runtime/enum_string/runtime_enum_string_example.cpp`](../../../../example/runtime/enum_string/runtime_enum_string_example.cpp)
