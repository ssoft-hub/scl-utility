# SCL_DEPRECATED / SCL_DEPRECATED_MSG

Deprecation annotation macros.

- Header: `#include <scl/utility/attribute/deprecated.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_DEPRECATED

Marks any named entity (function, class, variable, typedef, enum) as
deprecated. The compiler emits a warning at every use site, nudging users to
migrate to a replacement.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated]]` |
| MSVC native (not Clang-cl) | `__declspec(deprecated)` |
| `__has_attribute(deprecated)` (GCC, Clang) | `__attribute__((deprecated))` |
| None of the above | *(empty — no warning emitted)* |

### Usage

```cpp
#include <scl/utility/attribute/deprecated.h>

SCL_DEPRECATED void old_connect(const char* host, int port);

SCL_DEPRECATED struct LegacyHandle { int fd; };
```

---

## SCL_DEPRECATED_MSG(msg)

Like `SCL_DEPRECATED` but includes a string literal `msg` in the diagnostic
message, explaining why the entity is deprecated and what to use instead. On
compilers that do not support message-bearing deprecation the macro falls back
to `SCL_DEPRECATED` (warning without the message text).

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated(msg)]]` |
| MSVC native (not Clang-cl) | `__declspec(deprecated(msg))` |
| `__has_attribute(deprecated)` (GCC, Clang) | `__attribute__((deprecated(msg)))` |
| None of the above | *(empty)* |

### Usage

```cpp
#include <scl/utility/attribute/deprecated.h>

SCL_DEPRECATED_MSG("use connect(Endpoint) instead")
void old_connect(const char* host, int port);

SCL_DEPRECATED_MSG("replaced by ResourceHandle in v2")
struct LegacyHandle { int fd; };
```

---

## Notes

- Both macros suppress no existing warnings; they only add new ones at call
  sites.
- To silence the deprecation warning inside your own implementation (e.g.,
  in a wrapper that calls the deprecated function) use the compiler's
  diagnostic suppression pragma at that site.
- Each macro can be overridden before inclusion via `#define SCL_DEPRECATED`
  or `#define SCL_DEPRECATED_MSG(msg)`.
