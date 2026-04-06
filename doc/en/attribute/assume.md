# SCL_ASSUME / SCL_UNREACHABLE

Optimizer hints for assumption and unreachable-code marking.

- Header: `#include <scl/utility/attribute/assume.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_ASSUME(expr)

Informs the optimizer that `expr` is always true at this point. The compiler
may use this to eliminate redundant branches, omit null checks, or tighten
loop bounds.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(assume)` (C++23) | `[[assume(expr)]]` |
| MSVC native (not Clang-cl) | `__assume(expr)` |
| `__has_builtin(__builtin_assume)` (Clang) | `__builtin_assume(expr)` |
| `__has_attribute(assume)` (GCC ≥ 13) | `__attribute__((assume(expr)))` |
| `__has_builtin(__builtin_unreachable)` (GCC < 13) | `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())` |
| None of the above | `((void)(expr))` |

> **Important:** In the first four branches `expr` is **not evaluated** at
> runtime. In the GCC < 13 fallback it is evaluated. Never pass expressions
> with side effects.

### Usage

```cpp
#include <scl/utility/attribute/assume.h>

void scale(float* data, int n) {
    SCL_ASSUME(data != nullptr);
    SCL_ASSUME(n > 0);
    SCL_ASSUME(n % 4 == 0);        // hint: loop count is a multiple of 4
    for (int i = 0; i < n; ++i)
        data[i] *= 2.0f;
}
```

---

## SCL_UNREACHABLE()

Marks the current code location as unreachable. If execution reaches this
point at runtime, the behaviour is **undefined**. Use it to:

- Suppress "not all control paths return a value" warnings.
- Eliminate dead branches in generated code.
- Signal that a `switch` covers all enum values exhaustively.

### Detection

| Condition | Expansion |
|-----------|-----------|
| MSVC native (not Clang-cl) | `__assume(false)` |
| `__has_builtin(__builtin_unreachable)` (GCC, Clang) | `__builtin_unreachable()` |
| None of the above | `((void)0)` |

### Usage

```cpp
#include <scl/utility/attribute/assume.h>

int priority(Severity s) {
    switch (s) {
    case Severity::Low:      return 1;
    case Severity::Medium:   return 2;
    case Severity::High:     return 3;
    case Severity::Critical: return 4;
    }
    SCL_UNREACHABLE();  // all enum values are covered above
}
```

---

## Notes

- Both macros work at statement level — terminate with a semicolon.
- `SCL_ASSUME(false)` is equivalent to `SCL_UNREACHABLE()` on all compilers.
  Prefer `SCL_UNREACHABLE()` for clarity.
- Each macro can be overridden before inclusion via `#define SCL_ASSUME(expr)`
  or `#define SCL_UNREACHABLE()`.
