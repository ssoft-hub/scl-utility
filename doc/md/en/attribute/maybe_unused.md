# `SCL_MAYBE_UNUSED`

An entity marked with `SCL_MAYBE_UNUSED` may go unused in some build, and the compiler issues no
warning about it.

- Header: `#include <scl/utility/attribute/maybe_unused.h>`

```cpp
#define SCL_MAYBE_UNUSED /* see below */
```

## Semantics

- **What the compiler is told:** the entity may go unused, such as a parameter read only by
  an assertion in a build without `NDEBUG`. The compiler issues no warning about the unused entity,
  and the builds with the macro and without it differ in that warning alone.
- **Place:** the macro is written before the declaration of a variable, a parameter or a function,
  and after `struct`, `class`, `enum` or `enum class` in the declaration of a type.

| Condition, first match | Expansion |
|---|---|
| `__has_cpp_attribute(maybe_unused)` (C++17) | `[[maybe_unused]]` |
| `__has_attribute(__unused__)` | `__attribute__((__unused__))` |
| none of the above | nothing |

The macro expands to `[[maybe_unused]]` on GCC, Clang and MSVC in C++20.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_MAYBE_UNUSED` | `[[maybe_unused]]`, `__attribute__((__unused__))`, empty | `[[maybe_unused]]`, `__attribute__((__unused__))`, empty | `[[maybe_unused]]`, empty |

## Examples

A parameter read only by an assertion:

<!-- snippet: example/attribute/maybe_unused/attribute_maybe_unused_example.cpp maybe_unused -->
```cpp
void resize(std::vector<int> & storage, std::size_t size, SCL_MAYBE_UNUSED std::size_t capacity)
{
    assert(size <= capacity);
    storage.resize(size);
}
```

## See also

- [`example/attribute/maybe_unused/attribute_maybe_unused_example.cpp`](../../../../example/attribute/maybe_unused/attribute_maybe_unused_example.cpp)

---

Previous: [`SCL_LIKELY`](likely.md) | Next: [`SCL_NODISCARD`](nodiscard.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
