# Preprocessor

The preprocessor group holds macros that expand to `1` or `0` as the translation unit is compiled
with RTTI and with exceptions or not and as the standard library supports threads or not,
a macro that passes a comma-separated list through one parameter of another macro, and two macros
that read and advance a counter at compile time.

- Header: `#include <scl/utility/preprocessor.h>`

| Header | Page | Defines |
|---|---|---|
| `<scl/utility/preprocessor/counter.h>` | [`SCL_COUNTER_VALUE`](counter.md) | `SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT` |
| `<scl/utility/preprocessor/exceptions.h>` | [`SCL_HAS_EXCEPTIONS`](exceptions.md) | `SCL_HAS_EXCEPTIONS` |
| `<scl/utility/preprocessor/forward.h>` | [`SCL_FORWARD`](forward.md) | `SCL_FORWARD` |
| `<scl/utility/preprocessor/rtti.h>` | [`SCL_HAS_RTTI`](rtti.md) | `SCL_HAS_RTTI` |
| `<scl/utility/preprocessor/threads.h>` | [`SCL_HAS_THREADS`](threads.md) | `SCL_HAS_THREADS` |

## Compiler options

The values of the macros [`SCL_HAS_RTTI`](rtti.md) and [`SCL_HAS_EXCEPTIONS`](exceptions.md)
follow the command line of the compiler. The value of the macro [`SCL_HAS_THREADS`](threads.md)
follows how the standard library was built, so no option of the table changes it. The first two
macros take the values:

| Options | Compilers | `SCL_HAS_RTTI` | `SCL_HAS_EXCEPTIONS` |
|---|---|---|---|
| none of the options below | GCC, Clang | `1` | `1` |
| `-fno-rtti` | GCC, Clang | `0` | `1` |
| `-fno-exceptions` | GCC, Clang | `1` | `0` |
| `/EHsc` | MSVC, clang-cl | `1` | `1` |
| `/EHs-c-`, or no `/EH` option | MSVC, clang-cl | `1` | `0` |
| `/EHsc /GR-` | MSVC, clang-cl | `0` | `1` |
| `/GR-` | MSVC, clang-cl | `0` | `0` |

The macros [`SCL_FORWARD`](forward.md), [`SCL_COUNTER_VALUE` and `SCL_COUNTER_NEXT`](counter.md)
behave the same under every option.

## Documentation of the group

### Macros

- [`SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT`](counter.md) - a counter per tag type, read and
  advanced at compile time
- [`SCL_FORWARD`](forward.md) - a comma-separated list passed through one parameter of another
  macro
- [`SCL_HAS_EXCEPTIONS`](exceptions.md) - whether the translation unit is compiled with exceptions
- [`SCL_HAS_RTTI`](rtti.md) - whether the translation unit is compiled with RTTI
- [`SCL_HAS_THREADS`](threads.md) - whether the standard library supports threads

### Examples

- [`example/preprocessor/counter`](../../../../example/preprocessor/counter/preprocessor_counter_example.cpp)
- [`example/preprocessor/exceptions`](../../../../example/preprocessor/exceptions/preprocessor_exceptions_example.cpp)
- [`example/preprocessor/forward`](../../../../example/preprocessor/forward/preprocessor_forward_example.cpp)
- [`example/preprocessor/rtti`](../../../../example/preprocessor/rtti/preprocessor_rtti_example.cpp)
- [`example/preprocessor/threads`](../../../../example/preprocessor/threads/preprocessor_threads_example.cpp)

---

Next: [`SCL_COUNTER_VALUE`](counter.md) | [Back to the main page](../Main.md)
