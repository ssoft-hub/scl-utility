# `enum_string(value)`

Returns an enumeration value spelled as `Type::N`, where `N` is its number. It needs no RTTI.

- Header: `#include <scl/utility/runtime/enum.h>`

```cpp
template <::scl::concepts::enum_type E>
[[nodiscard]] ::std::string enum_string(E value);
```

## Semantics

- **Type:** `Type` is `scl::type_short_name<E>()`, read at compile time, so a namespaced
  enumeration answers its bare name.
- **Number:** `N` is the value in its underlying type, with the sign of the underlying type. A
  character or `bool` underlying type still gives a number: `'P'` answers `80`.
- **Any value:** a value no enumerator names is spelled as well as one that is named.

## Examples

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

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp named -->
```cpp
    constexpr auto name = ::scl::enum_name<Color::Red>(); // Color::Red
    auto const red = ::scl::enum_string(Color::Red);      // Color::1
    auto const blue = ::scl::enum_string(Color::Blue);    // Color::-3
```

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp unnamed -->
```cpp
    auto const unnamed = ::scl::enum_string(Color{42}); // Color::42
```

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp underlying -->
```cpp
    auto const grade = ::scl::enum_string(Grade::Pass);         // Grade::80
    auto const status = ::scl::enum_string(::net::Status::Err); // Status::42
```

## Against the compile-time name

| | `scl::enum_name<V>()` | `scl::enum_string(value)` |
|---|---|---|
| Evaluated | at compile time | at run time |
| Returns | `std::string_view` | `std::string` |
| Spells | the enumerator, `Color::Red` | the number, `Color::1` |
| Takes | an enumerator named at compile time | any value |

## See also

- [Runtime](index.md)
- [`example/runtime/enum_string/runtime_enum_string_example.cpp`](../../../../example/runtime/enum_string/runtime_enum_string_example.cpp)
