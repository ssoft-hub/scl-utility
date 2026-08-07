# SCL_LIFETIMEBOUND

Lifetime-bound parameter annotation.

- Header: `#include <scl/utility/attribute/lifetimebound.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_LIFETIMEBOUND

Marks a function parameter (or implicit `*this` of a member function) as
lifetime-bound to the return value. When annotated, the compiler can warn
if the returned reference, pointer, or view outlives the argument — catching
dangling-reference bugs at compile time.

**Placement** — after the parameter declaration, or after the cv-qualifier
of a member function (same position as `[[nodiscard]]` on a return type, but
applied to a parameter instead).

> **Note:** Currently supported by Clang. GCC and MSVC do not implement this
> attribute; the fallback is empty — no warning is emitted on those compilers,
> but the annotated code compiles correctly.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(clang::lifetimebound)` (Clang) | `[[clang::lifetimebound]]` |
| `__has_attribute(lifetimebound)` (Clang older) | `__attribute__((lifetimebound))` |
| None of the above | *(empty — no lifetime analysis)* |

### Usage

```cpp
#include <scl/utility/attribute/lifetimebound.h>

// Parameter — return value must not outlive 's'
std::string_view first_word(const std::string& s SCL_LIFETIMEBOUND);

// Multiple parameters
const int& clamp(const int& v SCL_LIFETIMEBOUND,
                 const int& lo SCL_LIFETIMEBOUND,
                 const int& hi SCL_LIFETIMEBOUND);

// Member function — return value must not outlive *this
class Builder {
public:
    Builder& set_name(std::string_view name) SCL_LIFETIMEBOUND;
    std::string_view name() const SCL_LIFETIMEBOUND;
};
```

### What Clang catches

```cpp
std::string_view sv = first_word(std::string{"hello world"});
//                                            ^~~~~~~~~~~~~ temporary destroyed here
// warning: address of stack memory associated with temporary ...
```

---

## Notes

- `SCL_LIFETIMEBOUND` has no runtime cost — it is a pure compile-time hint.
- It does not replace RAII or ownership types; use it to annotate
  non-owning view-returning functions.
- The macro can be overridden before inclusion via `#define SCL_LIFETIMEBOUND`.
