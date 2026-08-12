# RTTI availability

Reports whether the translation unit is compiled with RTTI, and gates the ScL declarations
that need `typeid`.

- Header: `#include <scl/utility/preprocessor/rtti.h>`

Contents:
- [`SCL_HAS_RTTI`](#scl_has_rtti)

---

## `SCL_HAS_RTTI`

Expands to `1` when the translation unit is compiled with RTTI, `0` otherwise.

- Header: `#include <scl/utility/preprocessor/rtti.h>`
- Declaration: `#define SCL_HAS_RTTI /* 1 or 0 */`

### Semantics

- **Always defined:** interrogate it with `#if`, never `#ifdef`. A misspelled name is then
  reported by `-Wundef` instead of quietly reading as false.
- **Derived per compiler:** `_CPPRTTI` (MSVC), `__GXX_RTTI` (GCC and Clang in GNU mode) or
  `__has_feature(cxx_rtti)` (Clang). The language defines no feature-test macro for RTTI,
  which is why this macro exists.
- **A report, not a switch:** defining it by hand does not turn RTTI on or off. It states
  what the compiler was told on the command line.
- **What it gates in ScL:** [`scl::type_name`](../runtime/type_name.md) and the rest of
  `<scl/utility/runtime/type.h>`, and the `std::any` backing of
  [`any_view`](../any/any_view.md) and [`any_arg`](../any/any_arg.md). The raw backing —
  a view over a typed lvalue — needs no `typeid` and stays.
- **The RTTI-free identity key:** [`scl::type_key`](../meta/type_key.md) answers the
  question `typeid` would in both configurations, so code that only has to compare types
  need not branch at all.

### Examples

```cpp
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <scl/utility/runtime/type.h>
std::string name_of(auto const & object) { return scl::type_name(object); }
#else
std::string name_of(auto const &) { return "<unknown>"; }
#endif
```

## Notes

- [`SCL_HAS_EXCEPTIONS`](exceptions.md) is the same treatment for exceptions, and the two
  are independent: each configuration of one is available under either configuration of
  the other.
