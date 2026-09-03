# SCL_FORCE_INLINE / SCL_NOINLINE

Function inlining control macros.

- Header: `#include <scl/utility/attribute/inline.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_FORCE_INLINE

Requests that the compiler always inline the annotated function, overriding the
inlining heuristics it would otherwise apply. Place before the return type.

### Detection

| Condition | Expansion |
|-----------|-----------|
| MSVC other than clang-cl | `__forceinline` |
| `__has_cpp_attribute(gnu::always_inline)` (GCC, Clang) | `[[gnu::always_inline]] inline` |
| `__has_attribute(always_inline)` (older GCC and Clang) | `__attribute__((always_inline)) inline` |
| None of the above | `inline` |

> **Note:** The GCC/Clang expansion already includes `inline`. Do not write
> `inline` separately when using `SCL_FORCE_INLINE`.

### Usage

```cpp
#include <scl/utility/attribute/inline.h>

SCL_FORCE_INLINE int clamp(int v, int lo, int hi) {
    return v < lo ? lo : v > hi ? hi : v;
}
```

---

## SCL_NOINLINE

Requests that the compiler never inline the annotated function. Useful for:

- Cold error paths that should not bloat hot code.
- Functions that must appear as named frames in profiler outputs or stack traces.
- Deliberately breaking inlining for debugging.

### Detection

| Condition | Expansion |
|-----------|-----------|
| MSVC other than clang-cl | `__declspec(noinline)` |
| `__has_cpp_attribute(gnu::noinline)` (GCC, Clang) | `[[gnu::noinline]]` |
| `__has_attribute(noinline)` (older GCC and Clang) | `__attribute__((noinline))` |
| None of the above | *(empty — function compiles without hint)* |

### Usage

```cpp
#include <scl/utility/attribute/inline.h>

SCL_NOINLINE void report_error(int code, const char* msg);

SCL_NOINLINE void cold_path() {
    // expensive diagnostics — keep out of hot code
}
```

---

## Notes

- Both macros affect a single function declaration or definition; they are
  not recursive.
- A recursive `SCL_FORCE_INLINE` function is not merely un-inlined: GCC rejects it
  outright at `-O0`, `-O1` and `-Og` (`inlining failed in call to
  'always_inline'`), and accepts it from `-O2` up. Clang accepts it at every level.
- Each macro can be overridden before inclusion via
  `#define SCL_FORCE_INLINE` or `#define SCL_NOINLINE`.
