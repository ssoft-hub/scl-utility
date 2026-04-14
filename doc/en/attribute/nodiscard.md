# SCL_NODISCARD / SCL_NODISCARD_MSG

Warn-on-discard annotation macros.

- Header: `#include <scl/utility/attribute/nodiscard.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_NODISCARD

Instructs the compiler to warn when the return value of a decorated function
(or an object of a decorated type) is discarded by the caller. Applies to
function declarations and class/struct/enum declarations.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(nodiscard)` ≥ 201907L (C++20) | `[[nodiscard]]` |
| `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
| `__has_attribute(warn_unused_result)` (GCC, Clang) | `__attribute__((warn_unused_result))` |
| None of the above | *(empty — no warning emitted)* |

### Usage

```cpp
#include <scl/utility/attribute/nodiscard.h>

SCL_NODISCARD bool try_lock();

SCL_NODISCARD struct ErrorCode { int value; };
```

---

## SCL_NODISCARD_MSG(msg)

Like `SCL_NODISCARD` but attaches a string literal `msg` to the diagnostic,
explaining what discarding the value implies. Available with C++20 `[[nodiscard]]`.
On compilers where the message form is unavailable the macro falls back to
`SCL_NODISCARD` (the warning is still emitted, but without the message).

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(nodiscard)` ≥ 201907L (C++20) | `[[nodiscard(msg)]]` |
| `__has_cpp_attribute(nodiscard)` (C++17) or earlier fallback | `SCL_NODISCARD` (message dropped) |

### Usage

```cpp
#include <scl/utility/attribute/nodiscard.h>

SCL_NODISCARD_MSG("ignoring the error code leaks the file descriptor")
ErrorCode write(int fd, const void* buf, std::size_t n);

SCL_NODISCARD_MSG("call release() to avoid a resource leak")
ResourceHandle acquire(std::size_t bytes);
```

---

## Notes

- Applying `SCL_NODISCARD` to a type marks every function returning that type
  as nodiscard implicitly.
- A cast to `void` suppresses the warning at a specific call site:
  `(void)try_lock();`
- Each macro can be overridden before inclusion via `#define SCL_NODISCARD`
  or `#define SCL_NODISCARD_MSG(msg)`.
