# Exception availability

Reports whether the translation unit is compiled with exceptions, and gates the ScL
declarations that answer a failed request by throwing.

- Header: `#include <scl/utility/preprocessor/exceptions.h>`

Contents:
- [`SCL_HAS_EXCEPTIONS`](#scl_has_exceptions)

---

## `SCL_HAS_EXCEPTIONS`

Expands to `1` when the translation unit is compiled with exceptions, `0` otherwise.

- Header: `#include <scl/utility/preprocessor/exceptions.h>`
- Declaration: `#define SCL_HAS_EXCEPTIONS /* 1 or 0 */`

### Semantics

- **Always defined:** interrogate it with `#if`, never `#ifdef`. A misspelled name is then
  reported by `-Wundef` instead of quietly reading as false.
- **Derived per compiler:** `_CPPUNWIND` (MSVC), `__EXCEPTIONS` (GCC and Clang) or
  `__has_feature(cxx_exceptions)` (Clang). The language defines no feature-test macro for
  exceptions, which is why this macro exists.
- **A report, not a switch:** defining it by hand does not turn exceptions on or off. It
  states what the compiler was told on the command line.
- **What it gates in ScL:** the four `scl::any_cast` overloads that return by value or by
  reference are declared only where it is `1` — see
  [`any_view`](../any/any_view.md) and [`any_arg`](../any/any_arg.md). The pointer-returning
  overloads stay, and answer a failed request with `nullptr`.
- **`scl::flags`:** an enumerator ordinal `>= bit_count` is a precondition violation rather
  than a query that answered no, so there is nothing to return in its place. Where the
  macro is `0` the offending call ends the program through `std::abort()` instead of
  throwing `std::out_of_range`. It is ill-formed in constant evaluation either way.

### Examples

```cpp
#include <scl/utility/any.h>
#include <scl/utility/preprocessor/exceptions.h>

std::string const & text(scl::any_view const & view)
{
#if SCL_HAS_EXCEPTIONS
    return scl::any_cast<std::string const &>(view);
#else
    return *scl::any_cast<std::string>(&view);
#endif
}
```

Code that has to compile in both configurations reaches for the pointer form alone, which
is declared in both:

```cpp
#include <scl/utility/any.h>

bool is_empty_string(scl::any_view const & view)
{
    auto const * text = scl::any_cast<std::string>(&view);
    return (text != nullptr) && text->empty();
}
```

## Notes

- `scl::bad_any_cast` is declared in both configurations. Only the overloads that throw it
  go away, so a `catch` written against the type still compiles wherever it is reachable.
- [`SCL_HAS_RTTI`](rtti.md) is the same treatment for RTTI, and the two are independent:
  each configuration of one is available under either configuration of the other.
