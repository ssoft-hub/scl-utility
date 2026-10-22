# `SCL_NODISCARD`, `SCL_NODISCARD_MSG`

The compiler warns where a result marked with `SCL_NODISCARD` or `SCL_NODISCARD_MSG` is discarded.

- Header: `#include <scl/utility/attribute/nodiscard.h>`

```cpp
#define SCL_NODISCARD /* see below */
#define SCL_NODISCARD_MSG(msg) /* see below */
```

## Semantics

- **What the compiler is told:** discarding the result of the function is a mistake. The compiler
  warns where a call of the function is written as a statement of its own.
- **On a type:** the compiler warns where the result of any function returning the type by value is
  discarded.
- **The message:** the string literal `msg` of `SCL_NODISCARD_MSG` is put into the warning where
  `__has_cpp_attribute(nodiscard)` is `201907L` or more. No message is put into the warning
  elsewhere, and `SCL_NODISCARD_MSG` expands to `SCL_NODISCARD` where `SCL_NODISCARD` is defined
  before the header.
- **Place:** either macro is written before the declaration of a function, and after `struct`,
  `class`, `enum` or `enum class` in the declaration of a type.

| Condition, first match | `SCL_NODISCARD` |
|---|---|
| `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
| `__has_attribute(__warn_unused_result__)` | `__attribute__((__warn_unused_result__))` |
| none of the above | nothing |

| Condition, first match | `SCL_NODISCARD_MSG(msg)` |
|---|---|
| `__has_cpp_attribute(nodiscard)` at `201907L` or more (C++20) | `[[nodiscard(msg)]]` |
| `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
| `__has_attribute(__warn_unused_result__)` | `__attribute__((__warn_unused_result__))` |
| none of the above | `SCL_NODISCARD` |

The macros expand to `[[nodiscard]]` and `[[nodiscard(msg)]]` on GCC, Clang and MSVC in C++20.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_NODISCARD` | `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, empty | `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, empty | `[[nodiscard]]`, empty |
| `SCL_NODISCARD_MSG(msg)` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, `SCL_NODISCARD` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, `SCL_NODISCARD` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `SCL_NODISCARD` |

## Examples

A function whose result is `false` where the value was not stored, and a type whose value is meant
to be read:

<!-- snippet: example/attribute/nodiscard/attribute_nodiscard_example.cpp nodiscard -->
```cpp
SCL_NODISCARD_MSG("the result tells whether the value was stored")
bool store(std::vector<int> & storage, int value)
{
    if (storage.size() == storage.capacity())
        return false;
    storage.push_back(value);
    return true;
}

struct SCL_NODISCARD status
{
    int code;
};

status open_channel(int id) { return status{id < 0 ? 1 : 0}; }
```

## See also

- [`example/attribute/nodiscard/attribute_nodiscard_example.cpp`](../../../../example/attribute/nodiscard/attribute_nodiscard_example.cpp)

---

Previous: [`SCL_MAYBE_UNUSED`](maybe_unused.md) | Next: [`SCL_NORETURN`](noreturn.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
