# Runtime type name utilities

Runtime extraction of human-readable type names via RTTI, with polymorphism support.

- Header: `#include <scl/utility/runtime/type.h>`
- Requires: RTTI enabled (`/GR` on MSVC; not `-fno-rtti`)

Contents:
- [`type_name(obj)`](#type_nameobj)
- [`type_short_name(obj)`](#type_short_nameobj)

---

## `type_name(obj)`

Returns the demangled fully qualified name of the **dynamic** type of `obj` as a `std::string`.

- Header: `#include <scl/utility/runtime/type.h>`
- Declaration: `template <typename T> std::string type_name(T const& obj);`

### Semantics

- **Dynamic dispatch:** If `T` is polymorphic and `obj` is accessed through a base reference,
  `typeid(obj)` resolves to the actual runtime type, not the static type at the call site.
- **Demangling:** On GCC and Clang the mangled name from `typeid` is demangled via
  `abi::__cxa_demangle`. On MSVC `typeid().name()` is already human-readable and returned as-is.
- **RTTI guard:** Both functions are compiled out when RTTI is disabled.
  Check for the macro `SCL_DETAIL_HAS_RTTI` if conditional compilation is needed.

### Examples

```cpp
#include <scl/utility/runtime/type.h>
#include <memory>

struct Base  { virtual ~Base() = default; };
struct Child : Base {};

// Fundamental type
int i = 0;
scl::type_name(i);   // "int"

// Static type
Child c;
scl::type_name(c);   // "Child"

// Polymorphic: resolves to the dynamic type through a base reference
std::unique_ptr<Base> p = std::make_unique<Child>();
scl::type_name(*p);  // "Child"  (not "Base")
```

### Comparison with the compile-time counterpart

| | `scl::type_name<T>()` | `scl::type_name(obj)` |
|---|---|---|
| Evaluation | Compile-time (`constexpr`) | Runtime |
| Return type | `std::string_view` (no allocation) | `std::string` |
| Polymorphism | No — `T` is fixed at the call site | Yes — resolves the dynamic type |
| RTTI | Not required | Required |

---

## `type_short_name(obj)`

Returns the unqualified identifier of the dynamic type of `obj`, stripping all namespace
qualifiers and template arguments.

- Header: `#include <scl/utility/runtime/type.h>`
- Declaration: `template <typename T> std::string type_short_name(T const& obj);`

### Example

```cpp
#include <scl/utility/runtime/type.h>
#include <memory>

namespace app { template <typename T> struct Task : Base {}; }

std::unique_ptr<Base> p = std::make_unique<app::Task<int>>();
scl::type_name(*p);        // "app::Task<int>"
scl::type_short_name(*p);  // "Task"
```
