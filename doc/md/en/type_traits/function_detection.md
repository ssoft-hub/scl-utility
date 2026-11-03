# Function presence detection

The macro `SCL_FUNCTION_DETECTION(foo)` declares templates for checking a function named `foo`:

- whether the function `foo` can be called with arguments of given types, the arguments being
  allowed to convert implicitly to the parameter types (non-strict match)
- whether a function `foo` is declared whose parameters are exactly the given types (strict match)

The checks run while the program compiles, and a declaration of the function is enough for them,
a definition is not needed. The name of the function to check is passed to the macro in place of
the name `foo`, and the names of the declared templates are made from it. The macro
`SCL_FUNCTION_DETECTION(print)`, for example, declares the variable templates
`has_print_function_v`, `has_print_function_exact_v` and the other templates with the same stem.

- Header: `#include <scl/utility/type_traits/detection/function.h>`

The macro should be expanded after the declarations of the function to check, in a separate
namespace or in a class, so that the names of the declared templates do not collide with other
declarations. The expansion of `SCL_FUNCTION_DETECTION(foo)` contains the declarations below.

```cpp
#define SCL_FUNCTION_DETECTION(name)

template <typename... Arguments>
using foo_function_operation = decltype(foo(std::declval<Arguments>()...));
template <typename... Arguments>
inline static constexpr auto has_foo_function_v = scl::is_detected_v<foo_function_operation, Arguments...>;
template <typename... Arguments>
using foo_function_t = scl::detected_t<foo_function_operation, Arguments...>;

template <typename... Arguments>
using foo_function_exact_operation = unspecified;
template <typename... Arguments>
inline static constexpr auto has_foo_function_exact_v = scl::is_detected_v<foo_function_exact_operation, Arguments...>;
template <typename... Arguments>
using foo_function_exact_t = scl::detected_t<foo_function_exact_operation, Arguments...>;
```

## Semantics

### Non-strict match

- **Condition.** The variable template `has_foo_function_v<Arguments...>` is `true` if the call
  `foo(std::declval<Arguments>()...)` compiles, and `false` otherwise. The arguments may convert
  implicitly to the parameter types. For the function `int foo(int, int)`, for example, the variable
  template `has_foo_function_v<int, long>` is `true`, since an argument of the type `long` converts
  to a parameter of the type `int`. The alias template `foo_function_t<Arguments...>` is the result
  type of that call, or the marker type [`scl::nonesuch`](detection.md) if the call does not
  compile.
- **Lookup.** The function for the call is looked up among the declarations visible where the macro
  is expanded, and in the namespaces and the classes of the arguments (ADL). A function found
  through the arguments may be declared after the macro is expanded as well, since that lookup runs
  when the variable template is used with particular argument types.
- **Callable object.** The variable template `has_foo_function_v` is `true` also where `foo` is
  a variable, such as a pointer to a function or a function object, and a call of that variable with
  arguments of the given types compiles.

### Strict match

- **Condition.** The variable template `has_foo_function_exact_v<Arguments...>` is `true` if
  a function `foo` whose parameters are exactly the types `Arguments...` is declared before
  the macro is expanded, and the function is not deleted (`= delete`). The top-level qualifiers
  `const` and `volatile` of the types `Arguments...` do not count, as the section Notes explains.
  For the overloads `int foo(int, int)` and `double foo(double, double)`, for example, the variable
  template `has_foo_function_exact_v<int, int>` is `true`, and the variable template
  `has_foo_function_exact_v<int, long>` is `false`, although a call with such arguments compiles.
- **Lookup.** The function is looked up only among the declarations visible where the macro is
  expanded, without the lookup through the arguments (ADL). So for a function declared later
  the variable template `has_foo_function_exact_v` is `false`. It is `false` for a pointer to
  a function and for a function object named `foo` as well, since such a variable is not a function.
- **Result type.** The strict match does not compare the result type of the function. The alias
  template `foo_function_exact_t<Arguments...>` is the result type of the function found, and it can
  be compared with an expected type, for example with `std::is_same_v`. The function is found also
  where its result type is deduced from its body (`auto`), and where it is a specialization of
  a function template with parameters of exactly the types `Arguments...`.
- **Ambiguous call.** In the strict match the function is found also where the plain call is
  ambiguous. For the overloads `long f(int)` and `char f(int &&)`, for example, the call
  `f(std::declval<int>())` is ambiguous, so the variable template `has_f_function_v<int>` is
  `false`. The variable template `has_f_function_exact_v<int>` is `true` all the same, and the alias
  template `f_function_exact_t<int>` is the type `long`. The variable template
  `has_f_function_exact_v<int &&>` is `true` as well.
- **Function template among the overloads.** If the overloads of `foo` include a function template,
  the function with the needed parameters is chosen by the result type of the plain call, that is
  the overload taken is the one whose result type is that of the call
  `foo(std::declval<Arguments>()...)`. If that call is ambiguous, it has no result type, and
  the variable template `has_foo_function_exact_v` is `false`. For the overloads
  `template <typename T> T f(T)`, `long f(int)` and `char f(int &&)`, for example, the variable
  template `has_f_function_exact_v<int>` is `false`, although the function `f(int)` is declared.
- **Specifier `noexcept`.** The strict match does not compare the specifier `noexcept` of
  the function. It can be checked with the operator `noexcept` applied to a call, for example
  `noexcept(foo(1, 2))`. Where the call is ambiguous, the specifier shows in the type of the pointer
  to the needed overload that [`scl::overload_cast`](overload_cast.md) returns.

### Limitations

- **MSVC.** On MSVC the address of a deleted function is not rejected during substitution. To keep
  such a function out of the strict match, on MSVC the strict match also requires the plain call
  `foo(std::declval<Arguments>()...)` to compile. So on MSVC the variable template
  `has_foo_function_exact_v` is `false` where the plain call is ambiguous.
- **Inaccessible static member function.** Where the macro is expanded in a class, a static member
  function `foo` of a base class that the class cannot access, such as a private one, is not
  supported. For such a function the variable templates of both matches can be `true`.
- **Place of expansion.** The build ends with an error if no declaration of `foo` is visible where
  the macro is expanded.

## Examples

Overloads of a function and the templates declared for them:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp declare -->
```cpp
int foo(int value, int factor);
double foo(double value, double factor);

namespace foo_traits
{
    SCL_FUNCTION_DETECTION(foo)
} // namespace foo_traits
```

The non-strict match, where the arguments convert to the parameter types:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp call -->
```cpp
static_assert(foo_traits::has_foo_function_v<int, int>);
static_assert(foo_traits::has_foo_function_v<int, long>);
static_assert(!foo_traits::has_foo_function_v<int>);
static_assert(std::is_same_v<foo_traits::foo_function_t<double, double>, double>);
```

The strict match of the parameter types:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp exact -->
```cpp
static_assert(foo_traits::has_foo_function_exact_v<int, int>);
static_assert(!foo_traits::has_foo_function_exact_v<int, long>);
```

The result type and the specifier `noexcept`, checked apart:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp properties -->
```cpp
static_assert(std::is_same_v<foo_traits::foo_function_exact_t<double, double>, double>);
static_assert(!noexcept(foo(std::declval<int>(), std::declval<int>())));
```

A call of a friend function defined inside a class compiles, but the variable template of the strict
match is `false` for it:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp adl -->
```cpp
namespace shapes
{
    struct circle
    {
        double radius;

        friend double foo(circle shape, double factor) { return shape.radius * factor; }
    };
} // namespace shapes

static_assert(foo_traits::has_foo_function_v<shapes::circle, double>);
static_assert(!foo_traits::has_foo_function_exact_v<shapes::circle, double>);
```

## Notes

- The top-level qualifiers `const` and `volatile` of a parameter type are not part of a function
  type, so `void f(int)` and `void f(int const)` are declarations of the same function. For this
  reason the strict match does not take such qualifiers into account, and for the function `f(int)`
  the variable template `has_f_function_exact_v<int const>` is `true` as well. Qualifiers below
  the top level do count, and for the function `f(int &)` the variable template
  `has_f_function_exact_v<int const &>` is `false`.
- A friend function defined inside a class is visible only to the lookup through the arguments
  (ADL). So for it the variable template `has_foo_function_v` is `true`, and the variable template
  `has_foo_function_exact_v` is `false`, as in the last example.

## See also

- [Member function presence detection](method_detection.md) - the same checks for a member function
- [Expression presence detection](detection.md) - the templates `scl::is_detected` and
  `scl::detected_t`, on which the declared templates are built
- [`example/type_traits/function_detection/type_traits_function_detection_example.cpp`](../../../../example/type_traits/function_detection/type_traits_function_detection_example.cpp)

---

Previous: [Expression presence detection](detection.md) |
Next: [Member function presence detection](method_detection.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
