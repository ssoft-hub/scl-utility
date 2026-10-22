# `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`

A branch marked with `SCL_LIKELY` is the one a program usually takes, and a branch marked with
`SCL_UNLIKELY` the one it rarely takes. An expression under `SCL_LIKELY_EXPR` is usually `true`, and
an expression under `SCL_UNLIKELY_EXPR` is usually `false`.

- Header: `#include <scl/utility/attribute/likely.h>`

```cpp
#define SCL_LIKELY /* see below */
#define SCL_UNLIKELY /* see below */
#define SCL_LIKELY_EXPR(expr) /* see below */
#define SCL_UNLIKELY_EXPR(expr) /* see below */
```

## `SCL_LIKELY`, `SCL_UNLIKELY`

- **What the compiler is told:** the statement after the macro is the path the program usually takes
  (`SCL_LIKELY`) or rarely takes (`SCL_UNLIKELY`). The optimizer may lay out the code for that path.
- **Place:** either macro is written before the statement of a branch of `if` or `else`, or before
  a `case` or `default` label, and never inside a condition.

| Condition, first match | `SCL_LIKELY` | `SCL_UNLIKELY` |
|---|---|---|
| `__has_cpp_attribute(likely)`, `__has_cpp_attribute(unlikely)` (C++20) | `[[likely]]` | `[[unlikely]]` |
| none of the above | nothing | nothing |

The macros expand to `[[likely]]` and `[[unlikely]]` on GCC, Clang and MSVC in C++20.

## `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`

- **What the compiler is told:** `expr` is usually `true` (`SCL_LIKELY_EXPR`) or usually `false`
  (`SCL_UNLIKELY_EXPR`).
- **The argument:** `expr` is an expression the operator `!` can be applied to.
- **The value:** the value of the expansion is the truth value of `expr`, of the type `bool` on
  every compiler.
- **MSVC:** no hint is given on MSVC, and only the truth test is left in the expansion.
- **Place:** either macro is written in any expression, the condition of `if`, `while` or `?:`
  included.

| Condition, first match | `SCL_LIKELY_EXPR(expr)` | `SCL_UNLIKELY_EXPR(expr)` |
|---|---|---|
| MSVC other than clang-cl | `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
| `__has_builtin(__builtin_expect)` (GCC, Clang) | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))` |
| none of the above | `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |

The macros expand to `(static_cast<bool>(!!(expr)))` on MSVC, and to the expansions with
`__builtin_expect` on GCC and Clang.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_LIKELY` | `[[likely]]`, empty | `[[likely]]`, empty | `[[likely]]`, empty |
| `SCL_UNLIKELY` | `[[unlikely]]`, empty | `[[unlikely]]`, empty | `[[unlikely]]`, empty |
| `SCL_LIKELY_EXPR(expr)` | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
| `SCL_UNLIKELY_EXPR(expr)` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |

## Examples

The branch of a digit, the usual one in parsing, and the other branch:

<!-- snippet: example/attribute/likely/attribute_likely_example.cpp likely -->
```cpp
int parse_digit(char symbol)
{
    if (symbol >= '0' && symbol <= '9')
        SCL_LIKELY { return symbol - '0'; }
    else
        SCL_UNLIKELY { return -1; }
}
```

The condition of an error, rare in the loop, and a result that is usually true:

<!-- snippet: example/attribute/likely/attribute_likely_example.cpp likely_expr -->
```cpp
int count_errors(int const * codes, std::size_t size)
{
    int errors = 0;
    for (std::size_t index = 0; index < size; ++index)
        if (SCL_UNLIKELY_EXPR(codes[index] != 0))
            ++errors;
    return errors;
}

bool all_valid(int const * codes, std::size_t size)
{
    return SCL_LIKELY_EXPR(count_errors(codes, size) == 0);
}
```

## See also

- [`SCL_HOT`, `SCL_COLD`](hotcold.md), hints about a whole function rather than one branch
- [`example/attribute/likely/attribute_likely_example.cpp`](../../../../example/attribute/likely/attribute_likely_example.cpp)

---

Previous: [`SCL_LIFETIMEBOUND`](lifetimebound.md) | Next: [`SCL_MAYBE_UNUSED`](maybe_unused.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
