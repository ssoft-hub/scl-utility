# Meta

The meta group holds function templates that return at compile time, without RTTI, the name of
a type, of a constant of an enumeration and of a function, a variable or a member. It also holds
the class `scl::type_key`, which identifies a type, and the class template `scl::is_tu_local`, which
tells whether a type is local to its translation unit.

- Header: `#include <scl/utility/meta.h>`

| Header | Page | Declares |
|---|---|---|
| `<scl/utility/meta/enum.h>` | [`enum_name<V>()`](enum_name.md) | `scl::enum_name`, `scl::enum_short_name` |
| `<scl/utility/meta/symbol.h>` | [`symbol_name<S>()`](symbol_name.md) | `scl::symbol_name`, `scl::symbol_short_name` |
| `<scl/utility/meta/tu_local.h>` | [`is_tu_local<T>`](is_tu_local.md) | `scl::is_tu_local`, `scl::is_tu_local_v` |
| `<scl/utility/meta/type.h>` | [`type_name<T>()`](type_name.md) | `scl::type_name`, `scl::type_short_name` |
| `<scl/utility/meta/type_key.h>` | [`type_key`](type_key.md) | `scl::type_key`, `scl::type_key_of` |

## At run time

The function templates of the group [runtime](../runtime/index.md) return names while
the program runs. The function template [`scl::type_name(obj)`](../runtime/type_name.md) returns
the name of the type an object of a polymorphic class was created as, and the function template
[`scl::enum_string(value)`](../runtime/enum_string.md) spells the value a variable of
an enumeration type holds.

## Documentation of the group

### Function templates

- [`enum_name<V>()`](enum_name.md) - the name of a constant of an enumeration
- [`symbol_name<S>()`](symbol_name.md) - the name of a function, a variable or a member
- [`type_name<T>()`](type_name.md) - the full and the short name of a type

### Classes and class templates

- [`is_tu_local<T>`](is_tu_local.md) - whether a type is local to its translation unit
- [`type_key`](type_key.md) - the identity of a type

### Examples

- [`example/meta/enum_name`](../../../../example/meta/enum_name/meta_enum_name_example.cpp)
- [`example/meta/is_tu_local`](../../../../example/meta/is_tu_local/meta_is_tu_local_example.cpp)
- [`example/meta/symbol_name`](../../../../example/meta/symbol_name/meta_symbol_name_example.cpp)
- [`example/meta/type_key`](../../../../example/meta/type_key/meta_type_key_example.cpp)
- [`example/meta/type_name`](../../../../example/meta/type_name/meta_type_name_example.cpp)

---

Next: [`enum_name<V>()`](enum_name.md) | [Back to the main page](../Main.md)
