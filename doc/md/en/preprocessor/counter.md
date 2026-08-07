# Compile-time counter

Stateful compile-time counter mechanism based on function overloading and ADL.

- Header: `#include <scl/utility/preprocessor/counter.h>`

Contents:
- [`SCL_COUNTER_VALUE(Tag)`](#scl_counter_value-tag)
- [`SCL_COUNTER_NEXT(Tag)`](#scl_counter_next-tag)

---

## `SCL_COUNTER_VALUE(Tag)`

Reads the current value of a compile-time counter identified by `Tag`.

- Header: `#include <scl/utility/preprocessor/counter.h>`
- Declaration: `#define SCL_COUNTER_VALUE(Tag) /* ... */`

### Semantics

- **Tag-based:** Each unique tag type defines an independent counter. Different tags do not interfere with each other.
- **Template tags:** Template specializations of the same tag template are treated as separate counters (e.g., `Counter<int>` and `Counter<float>` are independent).
- **32-bit range:** The counter supports values from 0 to 2^32 - 1, using 32 bits of overload resolution.
- **Compile-time:** The result is a `constexpr` value of type `std::uint_fast32_t`.
- **Initial value:** A counter starts at 0 before any `SCL_COUNTER_NEXT` invocations for its tag.

### Examples

```cpp
#include <scl/utility/preprocessor/counter.h>

struct MyTag {};

constexpr auto v0 = SCL_COUNTER_VALUE(MyTag);
static_assert(v0 == 0);

SCL_COUNTER_NEXT(MyTag)
constexpr auto v1 = SCL_COUNTER_VALUE(MyTag);
static_assert(v1 == 1);
```

---

## `SCL_COUNTER_NEXT(Tag)`

Increments a compile-time counter identified by `Tag`.

- Header: `#include <scl/utility/preprocessor/counter.h>`
- Declaration: `#define SCL_COUNTER_NEXT(Tag) /* ... */`

### Semantics

- **Side effect:** Defines a new function overload in `scl::preprocessor::detail` that causes subsequent `SCL_COUNTER_VALUE(Tag)` calls to return a higher value.
- **Monotonic:** Each invocation increases the counter by exactly 1.
- **Translation-unit scoped:** Counter state is local to the translation unit where the macros are used.

### Examples

```cpp
#include <scl/utility/preprocessor/counter.h>

struct TagA {};
struct TagB {};

// Two independent counters
constexpr auto a0 = SCL_COUNTER_VALUE(TagA);  // 0
constexpr auto b0 = SCL_COUNTER_VALUE(TagB);  // 0

SCL_COUNTER_NEXT(TagA)
constexpr auto a1 = SCL_COUNTER_VALUE(TagA);  // 1
constexpr auto b1 = SCL_COUNTER_VALUE(TagB);  // still 0

SCL_COUNTER_NEXT(TagA)
SCL_COUNTER_NEXT(TagB)
constexpr auto a2 = SCL_COUNTER_VALUE(TagA);  // 2
constexpr auto b2 = SCL_COUNTER_VALUE(TagB);  // 1
```

### Template tag example

```cpp
#include <scl/utility/preprocessor/counter.h>

template <typename T>
struct TypeCounter {};

constexpr auto int_v0 = SCL_COUNTER_VALUE(TypeCounter<int>);
static_assert(int_v0 == 0);

SCL_COUNTER_NEXT(TypeCounter<int>)
constexpr auto int_v1 = SCL_COUNTER_VALUE(TypeCounter<int>);
static_assert(int_v1 == 1);

// A different specialization is a separate counter
constexpr auto float_v0 = SCL_COUNTER_VALUE(TypeCounter<float>);
static_assert(float_v0 == 0);
```

## Notes

- The `SCL_FORWARD` macro is used internally to handle template tags that contain commas.
- This technique relies on stateful metaprogramming via ADL, supported by MSVC, GCC, and Clang.
