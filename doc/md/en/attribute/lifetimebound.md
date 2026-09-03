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
of a member function.

> **Note:** Supported by Clang only. GCC and MSVC do not implement this attribute,
> and there the macro expands to nothing: those compilers emit no warning, and the
> annotated code compiles correctly.

### Detection

| Condition | Expansion |
|-----------|-----------|
| MSVC other than clang-cl | *(empty, no equivalent)* |
| `__has_cpp_attribute(clang::lifetimebound)` (Clang) | `[[clang::lifetimebound]]` |
| `__has_attribute(lifetimebound)` (older Clang) | `__attribute__((lifetimebound))` |
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
// warning: temporary whose address is used as value of local variable 'sv'
//          will be destroyed at the end of the full-expression [-Wdangling]
```

---

## Notes

- `SCL_LIFETIMEBOUND` has no runtime cost — it is a pure compile-time hint.
- It does not replace RAII or ownership types; use it to annotate
  non-owning view-returning functions.
- The macro can be overridden before inclusion via `#define SCL_LIFETIMEBOUND`.
