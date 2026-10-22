# `SCL_INDETERMINATE`

The macro `SCL_INDETERMINATE` is written on a variable with automatic storage duration, or on
a function parameter, whose storage is not read before it is written. The implementation then need
not give that storage an erroneous value in C++26.

- Header: `#include <scl/utility/attribute/indeterminate.h>`

```cpp
#define SCL_INDETERMINATE /* see below */
```

## Semantics

- **Without an initializer:** a variable with automatic storage duration declared with no
  initializer stays uninitialised whatever the macro expands to, except for what a default
  constructor initialises.
- **What the compiler is told:** the storage is not read before it is written. The implementation
  gives such a variable an erroneous value in C++26, and a read of it before the first write is
  erroneous behaviour. The implementation may skip that value under the attribute
  `[[indeterminate]]`.
- **Without the attribute:** a read before the first write is undefined behaviour before C++26, and
  erroneous behaviour in C++26.
- **A false promise:** a read of such storage before it is written is undefined behaviour under
  `[[indeterminate]]`.
- **Place:** the macro is written before the definition of a variable with automatic storage
  duration, or before a parameter in the first declaration of its function. A program is ill-formed,
  no diagnostic required, where a parameter is declared with the attribute in the first declaration
  of its function in one translation unit and without it in the first declaration in another.

| Condition, first match | Expansion |
|---|---|
| `__has_cpp_attribute(indeterminate)` (C++26), `indeterminate` not a macro | `[[indeterminate]]` |
| none of the above | nothing |

The macro expands to `[[indeterminate]]` where `__has_cpp_attribute(indeterminate)` is not `0` and
`indeterminate` is not a macro. GCC 13, Clang 22 and MSVC 19.44 report `0` in C++20, so the macro
expands to nothing there.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_INDETERMINATE` | empty | empty | empty |

## Examples

A buffer filled digit by digit and read only where it has been written:

<!-- snippet: example/attribute/indeterminate/attribute_indeterminate_example.cpp indeterminate -->
```cpp
std::string reversed_decimal(unsigned value)
{
    SCL_INDETERMINATE char digits[std::numeric_limits<unsigned>::digits10 + 1];
    std::size_t count = 0;
    do
    {
        digits[count++] = static_cast<char>('0' + value % 10);
        value /= 10;
    } while (value != 0);
    return std::string(digits, count);
}
```

## See also

- [`example/attribute/indeterminate/attribute_indeterminate_example.cpp`](../../../../example/attribute/indeterminate/attribute_indeterminate_example.cpp)

---

Previous: [`SCL_HOT`](hotcold.md) | Next: [`SCL_LIFETIMEBOUND`](lifetimebound.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
