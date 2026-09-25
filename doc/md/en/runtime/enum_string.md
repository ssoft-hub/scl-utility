# Runtime enum value representation

Returns a string of the form `"TypeName::N"` where `N` is the underlying numeric value of
an enum, including out-of-range values that have no named enumerator.

- Header: `#include <scl/utility/runtime/enum.h>`
- Does **not** require RTTI.

---

## `enum_string(value)`

- Header: `#include <scl/utility/runtime/enum.h>`
- Declaration: `template <typename E> std::string enum_string(E value) requires std::is_enum_v<E>;`

### Semantics

- **Type name** is extracted at compile-time via `scl::type_short_name<E>()` — no RTTI needed.
- **Numeric value** is cast to `std::underlying_type_t<E>` before formatting, so the sign
  and magnitude always match the declaration (e.g., `unsigned char` renders as `0`–`255`,
  `int` renders with a leading `-` for negative values).
- **Out-of-range values** that have no named enumerator are represented by their numeric value.

### Examples

```cpp
#include <scl/utility/runtime/enum.h>

enum class Color : int  { Red = 1, Green = 2, Blue = -3 };
enum class Flags : unsigned { None = 0, A = 1, B = 2 };

namespace app { enum class Status : int { Ok = 0, Err = 42 }; }

auto const red_name     = scl::enum_string(Color::Red);       // "Color::1"
auto const blue_name    = scl::enum_string(Color::Blue);      // "Color::-3"
auto const other_name   = scl::enum_string(Color{42});        // "Color::42"  - unnamed value
auto const flag_name    = scl::enum_string(Flags::B);         // "Flags::2"
auto const status_name  = scl::enum_string(app::Status::Err); // "Status::42" - namespace stripped
```

### Comparison with the compile-time counterpart

| | `scl::enum_name<V>()` | `scl::enum_string(v)` |
|---|---|---|
| Evaluation | Compile-time (`constexpr`) | Runtime |
| Return type | `std::string_view` (no allocation) | `std::string` |
| Result | Member identifier (`"Color::Red"`) | Numeric value (`"Color::1"`) |
| Out-of-range values | Not applicable — `V` must be a named enumerator | Supported |
| RTTI | Not required | Not required |

## See also

- [`example/runtime/enum_string/runtime_enum_string_example.cpp`](../../../../example/runtime/enum_string/runtime_enum_string_example.cpp) —
  runnable version: a named enumerator against its compile-time counterpart, a value
  outside the enumerator set, an unsigned underlying type and a namespaced enum.
