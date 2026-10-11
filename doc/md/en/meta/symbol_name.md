# `symbol_name<S>()`

The function template `symbol_name<S>()` returns at compile time the name of the function,
the variable or the member that `S` points to, with its namespaces and classes. The function
template `symbol_short_name<S>()` returns that name alone.

- Header: `#include <scl/utility/meta/symbol.h>`
- Both function templates need no RTTI and can be evaluated in a constant expression.

```cpp
namespace scl
{
    template <auto S>
    [[nodiscard]] constexpr ::std::string_view symbol_name() noexcept;

    template <auto S>
    [[nodiscard]] constexpr ::std::string_view symbol_short_name() noexcept;
}
```

## Semantics

- **Argument:** `S` is a pointer to a function, a variable, a member function or a data member.
  The name of a function in a template argument converts to a pointer to it by itself, so
  `symbol_name<start>()` and `symbol_name<&start>()` are one specialization of the template and
  return `start`.
- **Full name:** the name comes with the namespaces and classes of the entity, such as
  `app::Clock::reset` for `&app::Clock::reset`, and without the address operator, the parentheses
  and the signature the compiler writes around it. GCC writes `__conv_op` in place of the name of
  a conversion operator, such as `app::Clock::__conv_op`, and MSVC writes the name of a stub the
  compiler generates in place of the name of a virtual member function, such as
  ``app::Clock::`vcall'{0}'``.
- **Short name:** the name of the entity alone, such as `reset` for `&app::Clock::reset`.
- **What the results are for:** the results of both function templates are for display. Their
  spelling differs between compilers and may change in a later version of the module, so nothing
  should compare a result against a literal, parse it, or persist it. The examples below compare
  results with literals to show the spelling of this version.

## Examples

The function and the class of the examples:

<!-- snippet: example/meta/symbol_name/meta_symbol_name_example.cpp types -->
```cpp
void start() {}

namespace app
{
    struct Clock
    {
        int ticks = 0;

        void reset() {}
    };
} // namespace app
```

The full name:

<!-- snippet: example/meta/symbol_name/meta_symbol_name_example.cpp symbol_name -->
```cpp
static_assert(scl::symbol_name<start>() == "start");
static_assert(scl::symbol_name<&app::Clock::reset>() == "app::Clock::reset");
static_assert(scl::symbol_name<&app::Clock::ticks>() == "app::Clock::ticks");
```

The short name:

<!-- snippet: example/meta/symbol_name/meta_symbol_name_example.cpp symbol_short_name -->
```cpp
static_assert(scl::symbol_short_name<&app::Clock::reset>() == "reset");
static_assert(scl::symbol_short_name<&app::Clock::ticks>() == "ticks");
```

## See also

- [`example/meta/symbol_name/meta_symbol_name_example.cpp`](../../../../example/meta/symbol_name/meta_symbol_name_example.cpp)

---

Previous: [`enum_name<V>()`](enum_name.md) | Next: [`type_name<T>()`](type_name.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
