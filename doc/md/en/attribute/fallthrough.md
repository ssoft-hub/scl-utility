# `SCL_FALLTHROUGH`

Execution passes on purpose from a case of a `switch` marked with `SCL_FALLTHROUGH` into the next
case.

- Header: `#include <scl/utility/attribute/fallthrough.h>`

```cpp
#define SCL_FALLTHROUGH /* see below */
```

## Semantics

- **What the compiler is told:** the case ends without `break` on purpose. The compiler issues no
  warning `-Wimplicit-fallthrough` for it. MSVC issues no warning C5262, which is off by default,
  and its code analysis with `/analyze /analyze:plugin EspXEngine.dll` issues no warning C26819.
  The flow of control is the same with the macro and without it.
- **Place:** the macro is written as the last statement of a case, closed by a semicolon, right
  before the next label.

| Condition, first match | Expansion |
|---|---|
| `__has_cpp_attribute(fallthrough)` (C++17) | `[[fallthrough]]` |
| `__has_attribute(__fallthrough__)` | `__attribute__((__fallthrough__))` |
| none of the above | `((void)0)` |

The macro expands to `[[fallthrough]]` on GCC, Clang and MSVC in C++20.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_FALLTHROUGH` | `[[fallthrough]]`, `__attribute__((__fallthrough__))`, `((void)0)` | `[[fallthrough]]`, `__attribute__((__fallthrough__))`, `((void)0)` | `[[fallthrough]]`, `((void)0)` |

## Examples

Every step from the stage given to the last one:

<!-- snippet: example/attribute/fallthrough/attribute_fallthrough_example.cpp fallthrough -->
```cpp
int steps_left(int stage)
{
    int steps = 0;
    switch (stage)
    {
    case 0:
        ++steps;
        SCL_FALLTHROUGH;
    case 1:
        ++steps;
        SCL_FALLTHROUGH;
    case 2:
        ++steps;
        break;
    default:
        break;
    }
    return steps;
}
```

## See also

- [`example/attribute/fallthrough/attribute_fallthrough_example.cpp`](../../../../example/attribute/fallthrough/attribute_fallthrough_example.cpp)

---

Previous: [`SCL_DEPRECATED`](deprecated.md) | Next: [`SCL_FORCE_INLINE`](inline.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
