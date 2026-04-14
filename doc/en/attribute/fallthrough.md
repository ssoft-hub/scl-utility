# SCL_FALLTHROUGH

Intentional switch fall-through annotation.

- Header: `#include <scl/utility/attribute/fallthrough.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_FALLTHROUGH

Marks the end of a `case` body that intentionally falls through to the next
`case` without a `break`, `return`, or `throw`. This silences
`-Wimplicit-fallthrough` (GCC, Clang) and warning C26819 (MSVC analyzer).

Place as the last statement inside the `case` body, terminated by a semicolon.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(fallthrough)` (C++17) | `[[fallthrough]]` |
| `__has_attribute(fallthrough)` (GCC ≥ 7, Clang) | `__attribute__((fallthrough))` |
| None of the above | `((void)0)` — silent no-op |

### Usage

```cpp
#include <scl/utility/attribute/fallthrough.h>

void handle(int code) {
    switch (code) {
    case 1:
        prepare();
        SCL_FALLTHROUGH;
    case 2:
        execute();
        break;
    case 3:
        log();
        SCL_FALLTHROUGH;
    case 4:
        cleanup();
        break;
    default:
        break;
    }
}
```

---

## Notes

- `SCL_FALLTHROUGH` only suppresses the implicit-fallthrough warning. It has
  no effect on runtime control flow.
- The fallback `((void)0)` is a valid statement expression on all targeted
  compilers; it compiles without side effects.
- The macro can be overridden before inclusion via `#define SCL_FALLTHROUGH`.
