# SCL_LIKELY / SCL_UNLIKELY / SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR

Branch-prediction hint macros.

- Header: `#include <scl/utility/attribute/likely.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_LIKELY

Portable wrapper for the C++20 `[[likely]]` statement attribute. Place it
before a statement to inform the optimizer that this branch is the common
path. Matches the no-argument style of the other `SCL_*` attribute macros.

### Placement

| Position | Syntax |
|----------|--------|
| `if` branch | `if (cond) SCL_LIKELY { ... }` |
| `switch` case label | `SCL_LIKELY case N: ...` |
| `switch` default label | `SCL_LIKELY default: ...` |

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(likely)` (C++20) | `[[likely]]` |
| None of the above | *(empty — no hint)* |

### Usage

```cpp
#include <scl/utility/attribute/likely.h>

if (cache_hit) SCL_LIKELY {
    return cache_value;
}

switch (msg.type) {
SCL_LIKELY case MsgType::Data:
    process(msg);
    break;
SCL_UNLIKELY default:
    handle_unknown(msg);
    break;
}
```

---

## SCL_UNLIKELY

Like `SCL_LIKELY` but marks the branch as the rare path.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(unlikely)` (C++20) | `[[unlikely]]` |
| None of the above | *(empty — no hint)* |

### Usage

```cpp
if (error) SCL_UNLIKELY {
    log_and_abort(error);
}
```

---

## SCL_LIKELY_EXPR(expr)

Expression-level branch hint using `__builtin_expect`. Wraps a boolean or
integer expression and returns its value unchanged. Use inside `if`, `while`,
or ternary conditions where a statement attribute cannot be placed.

On MSVC (which uses profile-guided optimisation) the expression passes through
unmodified.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_builtin(__builtin_expect)` (GCC, Clang) | `__builtin_expect(!!(expr), 1)` |
| None of the above | `(!!(expr))` |

### Usage

```cpp
if (SCL_LIKELY_EXPR(ptr != nullptr)) {
    // fast path — branch predictor biased here
}

while (SCL_LIKELY_EXPR(queue.size() > 0)) {
    drain(queue);
}
```

---

## SCL_UNLIKELY_EXPR(expr)

Like `SCL_LIKELY_EXPR` but biases the branch predictor toward the false outcome.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_builtin(__builtin_expect)` (GCC, Clang) | `__builtin_expect(!!(expr), 0)` |
| None of the above | `(!!(expr))` |

### Usage

```cpp
if (SCL_UNLIKELY_EXPR(error_code != 0)) {
    handle_error(error_code);
}
```

---

## Notes

- `SCL_LIKELY` / `SCL_UNLIKELY` are **statement attributes** — they annotate
  a branch, not an expression. They cannot appear inside a condition.
- `SCL_LIKELY_EXPR` / `SCL_UNLIKELY_EXPR` are **expression wrappers** — they
  can appear anywhere an integer expression is expected.
- Both forms always preserve the exact runtime value; only the optimizer's
  branch-layout decisions are affected.
- Do not use hints speculatively — incorrect hints can degrade performance.
- Each macro can be overridden before inclusion via `#define SCL_LIKELY`,
  `#define SCL_UNLIKELY`, `#define SCL_LIKELY_EXPR(expr)`, or
  `#define SCL_UNLIKELY_EXPR(expr)`.
