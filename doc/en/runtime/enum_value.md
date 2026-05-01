# Runtime enum value representation

Returns a string of the form `"TypeName::N"` where `N` is the underlying numeric value of
an enum, including out-of-range values that have no named enumerator.

- Header: `#include <scl/utility/runtime/enum.h>`
- Does **not** require RTTI.

---

## `enum_value(value)`

- Header: `#include <scl/utility/runtime/enum.h>`
- Declaration: `template <typename E> std::string enum_value(E value) requires std::is_enum_v<E>;`

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

scl::enum_value(Color::Red);       // "Color::1"
scl::enum_value(Color::Blue);      // "Color::-3"
scl::enum_value(Color{42});        // "Color::42"  — unnamed value
scl::enum_value(Flags::B);         // "Flags::2"
scl::enum_value(app::Status::Err); // "Status::42" — namespace stripped
```

### Comparison with the compile-time counterpart

| | `scl::enum_name<V>()` | `scl::enum_value(v)` |
|---|---|---|
| Evaluation | Compile-time (`constexpr`) | Runtime |
| Return type | `std::string_view` (no allocation) | `std::string` |
| Result | Member identifier (`"Color::Red"`) | Numeric value (`"Color::1"`) |
| Out-of-range values | Not applicable — `V` must be a named enumerator | Supported |
| RTTI | Not required | Not required |
