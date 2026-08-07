# SCL_NORETURN

Non-returning function annotation.

- Header: `#include <scl/utility/attribute/noreturn.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_NORETURN

Marks a function that never returns to its caller — either it throws an
exception, calls `std::terminate`, enters an infinite loop, or performs a
`longjmp`. The compiler may optimize call sites that follow a `SCL_NORETURN`
function as dead code and suppresses "not all control paths return a value"
warnings.

Place before the return type on declarations and definitions.

> **Undefined behaviour:** If a `SCL_NORETURN` function does return, the
> behaviour is undefined.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(noreturn)` (C++11) | `[[noreturn]]` |
| MSVC native (not Clang-cl) | `__declspec(noreturn)` |
| `__has_attribute(noreturn)` (GCC, Clang) | `__attribute__((noreturn))` |
| None of the above | *(empty — no annotation)* |

### Usage

```cpp
#include <scl/utility/attribute/noreturn.h>

SCL_NORETURN void fatal(const char* msg) {
    std::fprintf(stderr, "fatal: %s\n", msg);
    std::terminate();
}

int divide(int a, int b) {
    if (b == 0) fatal("division by zero");
    return a / b;  // no "not all paths return" warning
}
```

---

## Notes

- Applies to the function entity, not to individual call sites.
- Can be used together with `SCL_NOINLINE` to keep fatal paths out of hot code.
- Each macro can be overridden before inclusion via `#define SCL_NORETURN`.
