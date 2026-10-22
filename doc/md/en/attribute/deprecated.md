# `SCL_DEPRECATED`, `SCL_DEPRECATED_MSG`

The compiler warns at every use of an entity marked with `SCL_DEPRECATED` or `SCL_DEPRECATED_MSG`.

- Header: `#include <scl/utility/attribute/deprecated.h>`

```cpp
#define SCL_DEPRECATED /* see below */
#define SCL_DEPRECATED_MSG(msg) /* see below */
```

## Semantics

- **What the compiler is told:** the entity is kept only for old code, and another entity should be
  used in new code. The compiler warns at every use of the entity and issues no warning at
  the declaration itself.
- **The message:** the string literal `msg` of `SCL_DEPRECATED_MSG` is put into the warning.
  `SCL_DEPRECATED_MSG` expands to `SCL_DEPRECATED`, and no message is put into the warning, where no
  row before the last matches or where `SCL_DEPRECATED` is defined before the header.
- **Place:** either macro is written before the declaration of a function or a variable, and after
  `struct`, `class`, `enum` or `enum class` in the declaration of a type.

| Condition, first match | `SCL_DEPRECATED` | `SCL_DEPRECATED_MSG(msg)` |
|---|---|---|
| `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated]]` | `[[deprecated(msg)]]` |
| MSVC other than clang-cl | `__declspec(deprecated)` | `__declspec(deprecated(msg))` |
| `__has_attribute(__deprecated__)` | `__attribute__((__deprecated__))` | `__attribute__((__deprecated__(msg)))` |
| none of the above | nothing | `SCL_DEPRECATED` |

The macros expand to `[[deprecated]]` and `[[deprecated(msg)]]` on GCC, Clang and MSVC in C++20.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_DEPRECATED` | `[[deprecated]]`, `__attribute__((__deprecated__))`, empty | `[[deprecated]]`, `__attribute__((__deprecated__))`, empty | `[[deprecated]]`, `__declspec(deprecated)`, empty |
| `SCL_DEPRECATED_MSG(msg)` | `[[deprecated(msg)]]`, `__attribute__((__deprecated__(msg)))`, `SCL_DEPRECATED` | `[[deprecated(msg)]]`, `__attribute__((__deprecated__(msg)))`, `SCL_DEPRECATED` | `[[deprecated(msg)]]`, `__declspec(deprecated(msg))`, `SCL_DEPRECATED` |

## Examples

A function and a type kept only for old code:

<!-- snippet: example/attribute/deprecated/attribute_deprecated_example.cpp deprecated -->
```cpp
struct endpoint
{
    char const * host;
    int port;
};

bool connect(endpoint const & target) { return target.port > 0; }

SCL_DEPRECATED_MSG("use connect(endpoint) instead")
bool connect(char const * host, int port);

struct SCL_DEPRECATED legacy_handle
{
    int descriptor;
};
```

## See also

- [`example/attribute/deprecated/attribute_deprecated_example.cpp`](../../../../example/attribute/deprecated/attribute_deprecated_example.cpp)

---

Previous: [`SCL_ASSUME`](assume.md) | Next: [`SCL_FALLTHROUGH`](fallthrough.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
