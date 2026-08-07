# SCL_HOT / SCL_COLD

Function-frequency hint macros.

- Header: `#include <scl/utility/attribute/hotcold.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_HOT

Marks a function as called frequently. The compiler optimizes it more
aggressively and places it in a hot section of the binary to improve
instruction-cache locality. Call sites branching toward a `SCL_HOT`
function are implicitly treated as likely.

Place before the return type.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(gnu::hot)` (GCC, Clang) | `[[gnu::hot]]` |
| `__has_attribute(hot)` (GCC, Clang older) | `__attribute__((hot))` |
| None of the above | *(empty — no hint)* |

### Usage

```cpp
#include <scl/utility/attribute/hotcold.h>

SCL_HOT void dispatch(const Event& e);

SCL_HOT int lookup(std::uint32_t key);
```

---

## SCL_COLD

Marks a function as called rarely. The compiler optimizes it for size rather
than speed and places it in a cold section of the binary, away from hot code.
Call sites branching toward a `SCL_COLD` function are implicitly treated as
unlikely.

Place before the return type. Combines naturally with `SCL_NOINLINE` and
`SCL_NORETURN` for error handlers and fatal paths.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(gnu::cold)` (GCC, Clang) | `[[gnu::cold]]` |
| `__has_attribute(cold)` (GCC, Clang older) | `__attribute__((cold))` |
| None of the above | *(empty — no hint)* |

### Usage

```cpp
#include <scl/utility/attribute/hotcold.h>

SCL_COLD SCL_NOINLINE void report_oom(std::size_t requested);

SCL_COLD SCL_NORETURN void fatal(const char* msg);
```

---

## Relation to other attribute macros

### vs `SCL_FORCE_INLINE`

These are complementary strategies, not overlapping ones:

| | `SCL_FORCE_INLINE` | `SCL_HOT` |
|---|---|---|
| What it does | Inlines the function body at every call site | Keeps the function as a callable; optimises it more aggressively |
| Function in binary | Eliminated (or kept as a rarely-used fallback) | Present; relocated to `.text.hot` section |
| I-cache effect | Code is duplicated across call sites | Hot functions are grouped together — better locality |

`SCL_FORCE_INLINE` spreads the code; `SCL_HOT` consolidates it.
Combining `SCL_HOT` with `SCL_NOINLINE` is idiomatic: keep the function as a
single well-optimised body without bloating every call site.

### vs `SCL_LIKELY` / `SCL_UNLIKELY`

| Tool | Scope | Mechanism |
|------|-------|-----------|
| `SCL_HOT` / `SCL_COLD` | Whole function | Binary layout + optimization level for the entire function |
| `SCL_LIKELY` / `SCL_UNLIKELY` | Single branch (statement) | Branch-layout hint at one specific `if`/`switch` |
| `SCL_LIKELY_EXPR` / `SCL_UNLIKELY_EXPR` | Single branch (expression) | `__builtin_expect` at one specific condition |

`SCL_COLD` implicitly makes **all** branches leading to a call of that
function unlikely — a whole-function effect. `SCL_UNLIKELY` / `SCL_UNLIKELY_EXPR`
affect only one specific branch at the call site.

### Typical combinations

```cpp
// Hot dispatcher — optimise aggressively but do not inline,
// to avoid bloating every call site
SCL_HOT SCL_NOINLINE void dispatch(Event e);

// Cold error handler — move away from hot code entirely
SCL_COLD SCL_NOINLINE SCL_NORETURN void fatal(const char* msg);

// Inside a hot function — fine-grained per-branch hint
SCL_HOT int process(const Packet* p) {
    if (SCL_LIKELY_EXPR(p->is_valid()))
        return handle(p);
    SCL_UNLIKELY { return drop(p); }
}
```

---

## Notes

- `SCL_HOT` and `SCL_COLD` are mutually exclusive on the same function.
- MSVC has no equivalent; the fallback is empty and the code compiles normally.
- Each macro can be overridden before inclusion via `#define SCL_HOT` or
  `#define SCL_COLD`.
