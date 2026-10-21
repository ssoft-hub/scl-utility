# `SCL_ASSUME`, `SCL_UNREACHABLE`

The optimizer takes the condition of `SCL_ASSUME` as true at the point the macro is written, and
takes the point of `SCL_UNREACHABLE` as never reached.

- Header: `#include <scl/utility/attribute/assume.h>`

```cpp
#define SCL_ASSUME(expr) /* see below */
#define SCL_UNREACHABLE() /* see below */
```

## `SCL_ASSUME`

- **What the compiler is told:** `expr` is `true` each time execution reaches the statement.
  The optimizer may drop a branch or a check that the condition makes redundant.
- **A false promise:** the behaviour of the program is undefined where `expr` is `false` at that
  point and the expansion is taken from one of the first five rows of the table.
- **The argument:** `expr` is not evaluated under the first four rows of the table and is evaluated
  under the last two, so an expression with no side effects should be passed.
- **`SCL_ASSUME(false)`:** the promise is that of `SCL_UNREACHABLE`, yet Clang still reports
  the warning `-Wreturn-type` where a function ends after `[[assume(false)]]`. `SCL_UNREACHABLE()`
  should be written for that promise.
- **Place:** the macro is written as a statement of its own, closed by a semicolon.

| Condition, first match | Expansion |
|---|---|
| `__has_cpp_attribute(assume)`, where `__cplusplus` or `_MSVC_LANG` is above `202002L` | `[[assume(expr)]]` |
| MSVC other than clang-cl | `__assume(expr)` |
| `__has_builtin(__builtin_assume)` (Clang) | `__builtin_assume(expr)` |
| `__has_attribute(__assume__)` | `__attribute__((__assume__(expr)))` |
| `__has_builtin(__builtin_unreachable)` | `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())` |
| none of the above | `((void)(expr))` |

The macro expands to `__assume(expr)` on MSVC, to `__builtin_assume(expr)` on Clang and to
`__attribute__((__assume__(expr)))` on GCC in C++20. It expands to `[[assume(expr)]]` only where
`__cplusplus` or `_MSVC_LANG` is above `202002L` and `__has_cpp_attribute(assume)` is not `0`, as on
GCC 13 and Clang 22 under `-std=c++23`. MSVC 19.44 reports `0` for `assume` under `/std:c++latest`,
so the macro keeps `__assume(expr)` there.

## `SCL_UNREACHABLE`

- **What the compiler is told:** execution never reaches the statement. The compiler stops warning
  that a function returning a value may end without one.
- **A false promise:** the behaviour of the program is undefined where execution reaches
  the statement and the expansion is taken from one of the first two rows of the table.
- **Place:** the macro is written as a statement of its own, closed by a semicolon.

| Condition, first match | Expansion |
|---|---|
| MSVC other than clang-cl | `__assume(false)` |
| `__has_builtin(__builtin_unreachable)` (GCC, Clang) | `__builtin_unreachable()` |
| none of the above | `((void)0)` |

The macro expands to `__assume(false)` on MSVC and to `__builtin_unreachable()` on GCC and Clang.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_ASSUME(expr)` | `__attribute__((__assume__(expr)))`, `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())`, `((void)(expr))` | `__builtin_assume(expr)`, `__attribute__((__assume__(expr)))`, `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())`, `((void)(expr))` | `__assume(expr)`, `((void)(expr))` |
| `SCL_UNREACHABLE()` | `__builtin_unreachable()`, `((void)0)` | `__builtin_unreachable()`, `((void)0)` | `__assume(false)`, `((void)0)` |

`[[assume(expr)]]` is admitted as well in C++23 on GCC and on Clang 19 and later.

## Examples

A count promised to be a multiple of four:

<!-- snippet: example/attribute/assume/attribute_assume_example.cpp assume -->
```cpp
int sum_of_quads(int const * values, int count)
{
    SCL_ASSUME(count % 4 == 0);
    int sum = 0;
    for (int index = 0; index < count; ++index)
        sum += values[index];
    return sum;
}
```

The point after a `switch` whose every case returns:

<!-- snippet: example/attribute/assume/attribute_assume_example.cpp unreachable -->
```cpp
enum class severity
{
    low,
    high
};

int priority(severity level)
{
    switch (level)
    {
    case severity::low:
        return 1;
    case severity::high:
        return 2;
    }
    SCL_UNREACHABLE();
}
```

## See also

- [`example/attribute/assume/attribute_assume_example.cpp`](../../../../example/attribute/assume/attribute_assume_example.cpp)

---

Next: [`SCL_DEPRECATED`](deprecated.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
