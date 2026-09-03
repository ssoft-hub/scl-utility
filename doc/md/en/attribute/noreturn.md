# SCL_NORETURN

Non-returning function annotation.

- Header: `#include <scl/utility/attribute/noreturn.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_NORETURN

Marks a function that never returns to its caller — either it throws an
exception, calls `std::terminate`, enters an infinite loop, or performs a
`longjmp`. The compiler may treat the code following a call to such a function as
dead, and it suppresses "not all control paths return a value" warnings.

Place before the return type on declarations and definitions.

> **Undefined behaviour:** If a `SCL_NORETURN` function does return, the
> behaviour is undefined.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(noreturn)` (C++11) | `[[noreturn]]` |
| MSVC other than clang-cl | `__declspec(noreturn)` |
| `__has_attribute(noreturn)` (older GCC and Clang) | `__attribute__((noreturn))` |
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

- Applies to the function declaration, not to individual call sites.
- Can be used together with `SCL_NOINLINE` to keep fatal paths out of hot code.
  Write `SCL_NORETURN` first: on MSVC the other macros expand to `__declspec`, and
  `[[noreturn]]` after a `__declspec` is error C3829.
- The macro can be overridden before inclusion via `#define SCL_NORETURN`.
