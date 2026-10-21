# `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE`

A function marked with `SCL_UNSEQUENCED` or `SCL_REPRODUCIBLE` changes nothing observable by
a caller, so the optimizer may merge, move or drop its calls.

- Header: `#include <scl/utility/attribute/unsequenced.h>`

```cpp
#define SCL_UNSEQUENCED(...) /* see below */
#define SCL_REPRODUCIBLE(...) /* see below */
```

## Semantics

- **The argument:** the argument of either macro is the return type and the declarator of
  the function, without the body. The attribute is placed where each spelling requires it.
- **What the compiler is told by `SCL_UNSEQUENCED`:** the result depends on the arguments alone.
  The function changes nothing observable by a caller and reads no memory that may change between
  two calls, memory reached through a pointer or a reference argument included. GCC and Clang may
  merge two calls with equal arguments, move a call out of a loop and drop a call whose result goes
  unread.
- **What the compiler is told by `SCL_REPRODUCIBLE`:** the function changes nothing observable by
  a caller, and may read global memory and memory a pointer or a reference argument refers to.
  The optimizer may merge two calls with no write to memory between them.
- **A false promise:** the behaviour of the program is undefined where the expansion is taken from
  a row of the table that carries an attribute and the function changes anything observable by
  a caller, or reads under `SCL_UNSEQUENCED` memory that may change between two calls.
- **An argument with a side effect:** the increment of `f(++i)` happens, yet the call of `f` may be
  merged with an earlier one or dropped.
- **The C23 spelling:** C23 has the related attributes `[[unsequenced]]` and `[[reproducible]]`,
  written after the declarator, with promises of their own. No revision of C++ has taken them.
  The first row of each table is there for a compiler that offers them in C++, and under that row
  the compiler is told the C23 promise instead of the one above.

| Condition, first match | `SCL_UNSEQUENCED(...)` |
|---|---|
| `__has_cpp_attribute(unsequenced)`, not GCC, `unsequenced` not a macro | `__VA_ARGS__ [[unsequenced]]` |
| MSVC other than clang-cl | `__VA_ARGS__` |
| `__has_cpp_attribute(__gnu__::__const__)` (GCC, Clang) | `[[__gnu__::__const__]] __VA_ARGS__` |
| `__has_attribute(__const__)` | `__attribute__((__const__)) __VA_ARGS__` |
| none of the above | `__VA_ARGS__` |

| Condition, first match | `SCL_REPRODUCIBLE(...)` |
|---|---|
| `__has_cpp_attribute(reproducible)`, not GCC, `reproducible` not a macro | `__VA_ARGS__ [[reproducible]]` |
| MSVC other than clang-cl | `__VA_ARGS__` |
| `__has_cpp_attribute(__gnu__::__pure__)` (GCC, Clang) | `[[__gnu__::__pure__]] __VA_ARGS__` |
| `__has_attribute(__pure__)` | `__attribute__((__pure__)) __VA_ARGS__` |
| none of the above | `__VA_ARGS__` |

The macros expand to `__VA_ARGS__` on MSVC, and to `[[__gnu__::__const__]] __VA_ARGS__` and
`[[__gnu__::__pure__]] __VA_ARGS__` on GCC and Clang. Clang 22 reports neither C23 attribute in C++.
GCC 15 and later report both and ignore them in C++ with a warning, so the first row is not taken on
GCC.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_UNSEQUENCED(...)` | `[[__gnu__::__const__]] __VA_ARGS__`, `__attribute__((__const__)) __VA_ARGS__`, `__VA_ARGS__` | `[[__gnu__::__const__]] __VA_ARGS__`, `__attribute__((__const__)) __VA_ARGS__`, `__VA_ARGS__` | `__VA_ARGS__` |
| `SCL_REPRODUCIBLE(...)` | `[[__gnu__::__pure__]] __VA_ARGS__`, `__attribute__((__pure__)) __VA_ARGS__`, `__VA_ARGS__` | `[[__gnu__::__pure__]] __VA_ARGS__`, `__attribute__((__pure__)) __VA_ARGS__`, `__VA_ARGS__` | `__VA_ARGS__` |

## Examples

A function whose result depends on its argument alone, and a function that reads the array its
argument points to:

<!-- snippet: example/attribute/unsequenced/attribute_unsequenced_example.cpp unsequenced -->
```cpp
SCL_UNSEQUENCED(int square(int value)) { return value * value; }

SCL_REPRODUCIBLE(std::size_t count_zeros(int const * values, std::size_t size))
{
    std::size_t zeros = 0;
    for (std::size_t index = 0; index < size; ++index)
        if (values[index] == 0)
            ++zeros;
    return zeros;
}
```

## See also

- [`example/attribute/unsequenced/attribute_unsequenced_example.cpp`](../../../../example/attribute/unsequenced/attribute_unsequenced_example.cpp)

---

Previous: [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
