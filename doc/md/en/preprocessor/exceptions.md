# `SCL_HAS_EXCEPTIONS`

The macro `SCL_HAS_EXCEPTIONS` expands to `1` where the translation unit is compiled with
exceptions and to `0` where it is compiled without them.

- Header: `#include <scl/utility/preprocessor/exceptions.h>`

```cpp
#define SCL_HAS_EXCEPTIONS /* 1 or 0 */
```

## Semantics

- **Always defined:** a caller tests the macro with `#if` rather than `#ifdef`. Under `#if`
  a misspelled name reads as `0` and draws a `-Wundef` warning; under `#ifdef` it reads as false
  with no warning.
- **Source:** the value of the macro follows `__has_feature(cxx_exceptions)` where the compiler
  offers it (Clang, clang-cl, GCC 14 and later), and otherwise `_CPPUNWIND` (MSVC) or
  `__EXCEPTIONS` (GCC 13). The language defines no feature-test macro for exceptions.
- **Not a switch:** the value of the macro follows the command line of the compiler, and
  exceptions are not turned on or off by defining the macro by hand. The options that set it to
  `0` are listed on the [group page](index.md#compiler-options).

## Examples

A function that reads text in both builds:

<!-- snippet: example/preprocessor/exceptions/preprocessor_exceptions_example.cpp text_of -->
```cpp
std::string text_of(scl::any_view const & view)
{
#if SCL_HAS_EXCEPTIONS
    try
    {
        return scl::any_cast<std::string>(view);
    }
    catch (scl::bad_any_cast const &)
    {
        return "<not a string>";
    }
#else
    auto const * text = scl::any_cast<std::string>(&view);
    return text != nullptr ? *text : "<not a string>";
#endif
}
```

## See also

- [`SCL_HAS_RTTI`](rtti.md), the same macro for RTTI; the two macros are independent of each
  other
- [`example/preprocessor/exceptions/preprocessor_exceptions_example.cpp`](../../../../example/preprocessor/exceptions/preprocessor_exceptions_example.cpp)

---

Previous: [`SCL_FORWARD`](forward.md) | Next: [`SCL_HAS_RTTI`](rtti.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
