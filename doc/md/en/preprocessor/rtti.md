# `SCL_HAS_RTTI`

The macro `SCL_HAS_RTTI` expands to `1` where the translation unit is compiled with RTTI and to
`0` where it is compiled without it.

- Header: `#include <scl/utility/preprocessor/rtti.h>`

```cpp
#define SCL_HAS_RTTI /* 1 or 0 */
```

## Semantics

- **Always defined:** a caller tests the macro with `#if` rather than `#ifdef`. Under `#if`
  a misspelled name reads as `0` and draws a `-Wundef` warning; under `#ifdef` it reads as false
  with no warning.
- **Source:** the value of the macro follows `__has_feature(cxx_rtti)` where the compiler offers
  it (Clang, clang-cl, GCC 14 and later), and otherwise `_CPPRTTI` (MSVC) or `__GXX_RTTI`
  (GCC 13). The language defines no feature-test macro for RTTI.
- **Not a switch:** the value of the macro follows the command line of the compiler, and RTTI is
  not turned on or off by defining the macro by hand. The options that set it to `0` are
  listed on the [group page](index.md#compiler-options).

## Examples

A function that returns the name of the type of an object in both builds:

<!-- snippet: example/preprocessor/rtti/preprocessor_rtti_example.cpp name_of -->
```cpp
template <typename Type>
std::string name_of([[maybe_unused]] Type const & object)
{
#if SCL_HAS_RTTI
    return scl::type_short_name(object); // the dynamic type of a polymorphic object
#else
    return std::string{scl::type_short_name<Type>()}; // the static type of the argument
#endif
}
```

## See also

- [`SCL_HAS_EXCEPTIONS`](exceptions.md), the same macro for exceptions; the two macros are
  independent of each other
- [`example/preprocessor/rtti/preprocessor_rtti_example.cpp`](../../../../example/preprocessor/rtti/preprocessor_rtti_example.cpp)

---

Previous: [`SCL_HAS_EXCEPTIONS`](exceptions.md) | Next: [`SCL_HAS_THREADS`](threads.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
