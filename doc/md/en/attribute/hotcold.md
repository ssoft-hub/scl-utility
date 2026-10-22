# `SCL_HOT`, `SCL_COLD`

The compiler is told that a function marked with `SCL_HOT` is called often, and that a function
marked with `SCL_COLD` is called rarely.

- Header: `#include <scl/utility/attribute/hotcold.h>`

```cpp
#define SCL_HOT /* see below */
#define SCL_COLD /* see below */
```

## Semantics

- **What the compiler is told:** the function is called often (`SCL_HOT`) or rarely (`SCL_COLD`).
- **What GCC and Clang do:** GCC and Clang mark a function under `SCL_HOT` as called often, and GCC
  optimizes it harder. Both optimize a function under `SCL_COLD` for size and take a branch that
  leads to a call of it as unlikely. Both place hot functions in one section and cold functions in
  another, apart from the rest of the code, where the target and the options allow it.
  The likelihood of a branch that leads to a call of a hot function stays as it is.
- **MSVC:** MSVC has no attribute for a function called often or rarely, and both macros expand to
  nothing there.
- **One of the two:** `SCL_HOT` and `SCL_COLD` must not both be written on one function. Clang
  rejects the pair, and GCC drops one of the two attributes with a warning.
- **Place:** either macro is written before the declaration of a function.

| Condition, first match | `SCL_HOT` | `SCL_COLD` |
|---|---|---|
| MSVC other than clang-cl | nothing | nothing |
| `__has_cpp_attribute(__gnu__::__hot__)`, `__has_cpp_attribute(__gnu__::__cold__)` (GCC, Clang) | `[[__gnu__::__hot__]]` | `[[__gnu__::__cold__]]` |
| `__has_attribute(__hot__)`, `__has_attribute(__cold__)` | `__attribute__((__hot__))` | `__attribute__((__cold__))` |
| none of the above | nothing | nothing |

The macros expand to nothing on MSVC, and to `[[__gnu__::__hot__]]` and `[[__gnu__::__cold__]]` on
GCC and Clang.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_HOT` | `[[__gnu__::__hot__]]`, `__attribute__((__hot__))`, empty | `[[__gnu__::__hot__]]`, `__attribute__((__hot__))`, empty | empty |
| `SCL_COLD` | `[[__gnu__::__cold__]]`, `__attribute__((__cold__))`, empty | `[[__gnu__::__cold__]]`, `__attribute__((__cold__))`, empty | empty |

## Examples

A function run for every block of data, and one run only for a damaged block:

<!-- snippet: example/attribute/hotcold/attribute_hotcold_example.cpp hotcold -->
```cpp
SCL_HOT unsigned checksum(unsigned char const * bytes, std::size_t size)
{
    unsigned sum = 0;
    for (std::size_t index = 0; index < size; ++index)
        sum = sum * 31 + bytes[index];
    return sum;
}

SCL_COLD void report_corruption(std::size_t block)
{
    std::cerr << "block " << block << " is damaged\n";
}
```

## See also

- [`SCL_FORCE_INLINE`, `SCL_NOINLINE`](inline.md), hints about inlining a function
- [`SCL_LIKELY`, `SCL_UNLIKELY`](likely.md), hints about one branch rather than a whole function
- [`example/attribute/hotcold/attribute_hotcold_example.cpp`](../../../../example/attribute/hotcold/attribute_hotcold_example.cpp)

---

Previous: [`SCL_FORCE_INLINE`](inline.md) | Next: [`SCL_INDETERMINATE`](indeterminate.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
