# `SCL_FORCE_INLINE`, `SCL_NOINLINE`

The compiler is asked to inline a function marked with `SCL_FORCE_INLINE` at every call, and never
to inline a function marked with `SCL_NOINLINE`.

- Header: `#include <scl/utility/attribute/inline.h>`

```cpp
#define SCL_FORCE_INLINE /* see below */
#define SCL_NOINLINE /* see below */
```

## `SCL_FORCE_INLINE`

- **What the compiler is told:** the function is to be inlined at every call, whatever
  the heuristics of the compiler. Clang and MSVC keep a direct call they cannot inline as a call,
  and GCC rejects it, as described under **Recursion**. MSVC inlines nothing at `/Od`.
- **`inline` included:** every expansion contains the keyword `inline` or `__forceinline`, so
  a function defined in a header with the macro may be defined in every translation unit in which
  the header is included. `inline` must not be written beside the macro, since GCC rejects a second
  `inline`.
- **Recursion:** GCC rejects a recursive function under the macro with
  `inlining failed in call to 'always_inline'` at `-O0`, `-O1` and `-Og`, and at every other level
  unless it turns the recursion into a loop. Clang accepts such a function at every level. MSVC
  accepts it, does not inline it and, under `/W4` with `/O1` or `/O2`, reports warning C4714.
- **Place:** the macro is written before the declaration of a function, after every other macro of
  the group there. The expansion ends in `inline` on GCC and Clang, and a standard attribute after
  it is an error.

| Condition, first match | Expansion |
|---|---|
| MSVC other than clang-cl | `__forceinline` |
| `__has_cpp_attribute(__gnu__::__always_inline__)` (GCC, Clang) | `[[__gnu__::__always_inline__]] inline` |
| `__has_attribute(__always_inline__)` | `__attribute__((__always_inline__)) inline` |
| none of the above | `inline` |

The macro expands to `__forceinline` on MSVC and to `[[__gnu__::__always_inline__]] inline` on GCC
and Clang.

## `SCL_NOINLINE`

- **What the compiler is told:** the function is not to be inlined into a caller, so it keeps
  a frame of its own in the call stack. GCC may still make a specialised copy of it at `-O3`, and
  a call of a function with no side effect whose result goes unread may still be dropped.
- **Place:** the macro is written before the declaration of a function, after every macro of
  the group that expands to a standard attribute. MSVC drops `[[nodiscard]]` and `[[maybe_unused]]`
  written after `__declspec(noinline)`, and reports the drop only by the warning C5240, which is off
  by default.

| Condition, first match | Expansion |
|---|---|
| MSVC other than clang-cl | `__declspec(noinline)` |
| `__has_cpp_attribute(__gnu__::__noinline__)` (GCC, Clang) | `[[__gnu__::__noinline__]]` |
| `__has_attribute(__noinline__)` | `__attribute__((__noinline__))` |
| none of the above | nothing |

The macro expands to `__declspec(noinline)` on MSVC and to `[[__gnu__::__noinline__]]` on GCC and
Clang.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_FORCE_INLINE` | `[[__gnu__::__always_inline__]] inline`, `__attribute__((__always_inline__)) inline`, `inline` | `[[__gnu__::__always_inline__]] inline`, `__attribute__((__always_inline__)) inline`, `inline` | `__forceinline`, `inline` |
| `SCL_NOINLINE` | `[[__gnu__::__noinline__]]`, `__attribute__((__noinline__))`, empty | `[[__gnu__::__noinline__]]`, `__attribute__((__noinline__))`, empty | `__declspec(noinline)`, empty |

## Examples

A short function the compiler is asked to inline at every call:

<!-- snippet: example/attribute/inline/attribute_inline_example.cpp force_inline -->
```cpp
SCL_FORCE_INLINE int clamp(int value, int low, int high)
{
    return value < low ? low : value > high ? high : value;
}
```

A report of an error the compiler is asked to keep as one function:

<!-- snippet: example/attribute/inline/attribute_inline_example.cpp noinline -->
```cpp
SCL_NOINLINE void report_out_of_range(int value) { std::cerr << value << " is out of range\n"; }
```

## See also

- [`SCL_HOT`, `SCL_COLD`](hotcold.md), hints about how often a function is called
- [`example/attribute/inline/attribute_inline_example.cpp`](../../../../example/attribute/inline/attribute_inline_example.cpp)

---

Previous: [`SCL_FALLTHROUGH`](fallthrough.md) | Next: [`SCL_HOT`](hotcold.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
