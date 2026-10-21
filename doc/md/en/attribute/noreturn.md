# `SCL_NORETURN`

A function marked with `SCL_NORETURN` never returns to its caller.

- Header: `#include <scl/utility/attribute/noreturn.h>`

```cpp
#define SCL_NORETURN /* see below */
```

## Semantics

- **What the compiler is told:** every call of the function ends the program, throws an exception,
  jumps away or never ends. The compiler takes the code after a call as never reached and stops
  warning that a function returning a value may end without one.
- **A false promise:** the behaviour of the program is undefined where the function returns and
  the expansion is taken from a row of the table other than the last.
- **Beside other macros:** `SCL_NORETURN` must be written before `SCL_NOINLINE` and
  `SCL_FORCE_INLINE`. `[[noreturn]]` after `__declspec(noinline)` or `__forceinline` is error C3829
  on MSVC, and `[[noreturn]]` after the `inline` of `SCL_FORCE_INLINE` is an error on GCC and Clang
  too.
- **Place:** the macro is written before the declaration of a function.

| Condition, first match | Expansion |
|---|---|
| `__has_cpp_attribute(noreturn)` (C++11) | `[[noreturn]]` |
| MSVC other than clang-cl | `__declspec(noreturn)` |
| `__has_attribute(__noreturn__)` | `__attribute__((__noreturn__))` |
| none of the above | nothing |

The macro expands to `[[noreturn]]` on GCC, Clang and MSVC in C++20.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_NORETURN` | `[[noreturn]]`, `__attribute__((__noreturn__))`, empty | `[[noreturn]]`, `__attribute__((__noreturn__))`, empty | `[[noreturn]]`, `__declspec(noreturn)`, empty |

## Examples

A function that ends the program, and a function that calls it:

<!-- snippet: example/attribute/noreturn/attribute_noreturn_example.cpp noreturn -->
```cpp
SCL_NORETURN void fail(char const * message)
{
    std::cerr << "fatal: " << message << '\n';
    std::exit(EXIT_FAILURE);
}

int divide(int dividend, int divisor)
{
    if (divisor == 0)
        fail("division by zero");
    return dividend / divisor;
}
```

## See also

- [`example/attribute/noreturn/attribute_noreturn_example.cpp`](../../../../example/attribute/noreturn/attribute_noreturn_example.cpp)

---

Previous: [`SCL_NODISCARD`](nodiscard.md) | Next: [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
