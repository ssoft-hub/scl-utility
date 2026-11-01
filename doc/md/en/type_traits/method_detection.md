# Member function presence detection

The macro `SCL_METHOD_DETECTION(add)` declares templates for checking a member function named `add`
of an object of a given type:

- whether the member function `add` can be called on the object with arguments of given types,
  the arguments being allowed to convert implicitly to the parameter types (non-strict match)
- whether a member function `add` is declared whose parameters are exactly the given types and whose
  qualifiers match the type of the object (strict match)
- whether a member function `add` is declared with an explicit object parameter (C++23) of a given
  type and with parameters of exactly the given types
- whether a static member function `add` can be called through the class name, and whether one is
  declared with the given parameter types

The checks run while the program compiles, and a declaration of the member function is enough for
them, a definition is not needed. The name of the member function to check is passed to the macro in
place of the name `add`, and the names of the declared templates are made from it. The macro
`SCL_METHOD_DETECTION(resize)`, for example, declares the variable templates `has_resize_method_v`,
`has_resize_method_exact_v` and the other templates with the same stem.

- Header: `#include <scl/utility/type_traits/detection/method.h>`

The macro should be expanded in a separate namespace or in a class, so that the names of
the declared templates do not collide with other declarations. The expansion of
`SCL_METHOD_DETECTION(add)` contains the declarations below. The object type `Type`, with its
cv-qualifiers and reference, is passed to every template of the expansion as the first argument,
followed by the types of the call arguments. The variable template
`has_add_method_v<counter &, int>`, for example, is `true` if the member function `add` can be
called with an argument of the type `int` on an lvalue of the type `counter`.

```cpp
#define SCL_METHOD_DETECTION(name)

template <typename Type, typename... Arguments>
using add_method_operation = decltype(std::declval<Type>().add(std::declval<Arguments>()...));
template <typename... Arguments>
inline static constexpr auto has_add_method_v = scl::is_detected_v<add_method_operation, Arguments...>;
template <typename... Arguments>
using add_method_t = scl::detected_t<add_method_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_method_exact_operation = unspecified;
template <typename... Arguments>
inline static constexpr auto has_add_method_exact_v = scl::is_detected_v<add_method_exact_operation, Arguments...>;
template <typename... Arguments>
using add_method_exact_t = scl::detected_t<add_method_exact_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_static_method_operation = decltype(std::remove_cvref_t<Type>::add(std::declval<Arguments>()...));
template <typename... Arguments>
inline static constexpr auto has_add_static_method_v = scl::is_detected_v<add_static_method_operation, Arguments...>;
template <typename... Arguments>
using add_static_method_t = scl::detected_t<add_static_method_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_static_method_exact_operation = unspecified;
template <typename... Arguments>
inline static constexpr auto has_add_static_method_exact_v =
    scl::is_detected_v<add_static_method_exact_operation, Arguments...>;
template <typename... Arguments>
using add_static_method_exact_t = scl::detected_t<add_static_method_exact_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_explicit_method_exact_operation = unspecified;
template <typename... Arguments>
inline static constexpr auto has_add_explicit_method_exact_v =
    scl::is_detected_v<add_explicit_method_exact_operation, Arguments...>;
template <typename... Arguments>
using add_explicit_method_exact_t = scl::detected_t<add_explicit_method_exact_operation, Arguments...>;
```

## Semantics

### General rules

- **Object.** The object is passed to the call as `std::declval<Type>()`. For a type `Type` of
  the form `X &` the object is an lvalue, and for the types `X` and `X &&` it is an rvalue.
  The qualifiers `const` and `volatile` of the type `Type` become the qualifiers of the object.
- **No such member.** For a class with no member `add` every variable template is `false`.

### Non-strict match

- **Condition.** The variable template `has_add_method_v<Type, Arguments...>` is `true` if the call
  `std::declval<Type>().add(std::declval<Arguments>()...)` compiles, and `false` otherwise.
  The arguments may convert implicitly to the parameter types, and the member function is chosen by
  overload resolution. For the member function `void add(int) &`, for example, the variable template
  `has_add_method_v<counter &, short>` is `true`. The alias template
  `add_method_t<Type, Arguments...>` is the result type of the call, or the marker type
  [`scl::nonesuch`](detection.md) if the call does not compile.

### Strict match

- **Condition.** The variable template `has_add_method_exact_v<Type, Arguments...>` is `true` if
  the class `std::remove_cvref_t<Type>` or a public base class of it declares a member function
  `add` whose parameters are exactly the types `Arguments...` and whose qualifiers match the type
  `Type`, and the member function is neither deleted (`= delete`) nor inaccessible. A member
  function of a private or protected base class is not found, even where a `using` declaration in
  the class `std::remove_cvref_t<Type>` makes it public. The top-level qualifiers `const` and
  `volatile` of the types `Arguments...` do not count, as the section Notes explains. Only a member
  function with an implicit object parameter, that is a plain non-static member function, is found
  in this form. A member function with an explicit object parameter is checked by a variable
  template of its own, described below.
- **Matching qualifiers.** In the strict match the cv-qualifiers of the member function have to be
  those of the type `Type`, and its ref-qualifier has to be `&` for the type `X &`, `&&` for
  the type `X &&`, and none for the type `X`. For the type `X const &`, for example, the variable
  template is `true` only for a member function `add(...) const &`. For a member function
  `add(...) const` with no ref-qualifier it is `false`, although such a member function can be
  called on that object.
- **Result type.** The strict match does not compare the result type of the member function.
  The alias template `add_method_exact_t<Type, Arguments...>` is the result type of the member
  function found, and it can be compared with an expected type, for example with `std::is_same_v`.
- **Ambiguous call.** In the strict match the member function is found also where the plain call is
  ambiguous. For the overloads `long add(int) &` and `char add(int &&) &`, for example, the variable
  template `has_add_method_v<X &, int>` is `false`, and the variable templates
  `has_add_method_exact_v<X &, int>` and `has_add_method_exact_v<X &, int &&>` are `true`.
- **Member function template among the overloads.** If the overloads of `add` include a member
  function template, the member function with the needed parameters is chosen by the result type of
  the plain call, that is the overload taken is the one whose result type is that of the call
  `std::declval<Type>().add(std::declval<Arguments>()...)`. If that call is ambiguous, the variable
  template `has_add_method_exact_v` is `false`.
- **Specifier `noexcept`.** The strict match does not compare the specifier `noexcept` of the member
  function. It can be checked with the operator `noexcept` applied to a call, as in the example
  below. Where the call is ambiguous, the specifier shows in the type of the pointer to the needed
  overload that [`scl::member_overload_cast`](overload_cast.md) returns.

### Static member functions and the explicit object parameter

- **Static member function.** The variable template `has_add_static_method_v<Type, Arguments...>` is
  `true` if the call through the class name
  `std::remove_cvref_t<Type>::add(std::declval<Arguments>()...)` compiles. The variable template
  `has_add_static_method_exact_v<Type, Arguments...>` is `true` if a static member function `add`
  whose parameters are exactly the types `Arguments...` is declared. As in the strict match of
  a member function, such a static member function is found also where the call through the class
  name is ambiguous. For the static forms the object type `Type` gives only the class, and its
  qualifiers do not count.
- **Explicit object parameter.** The variable template
  `has_add_explicit_method_exact_v<Type, Arguments...>` is `true` if a member function `add` is
  declared with an explicit object parameter (C++23) of exactly the type `Type` and with the other
  parameters of exactly the types `Arguments...`, and the member function is neither deleted nor
  inaccessible. For the member function `int add(this X const & self, int)`, for example,
  the variable template `has_add_explicit_method_exact_v<X const &, int>` is `true`, and
  the variable template `has_add_explicit_method_exact_v<X &, int>` is `false`, although such
  a member function can be called on an object of the type `X &` as well. A member function of
  the base class `Base` with the parameter `this Base const & self` is not found for the type
  `Derived const &`, since the type of the object parameter differs from the type `Type`. A member
  function template with the parameter `this Self && self` is found, since the type `Self` is
  deduced from the type `Type`.
- **Static member function beside one with an explicit object parameter.** A pointer to the static
  member function `static int add(X const &, int)` has the type of a pointer to the member function
  `int add(this X const &, int)`. The two differ only in that the static one can be called through
  the class name, as in `X::add(object, 1)`. So the variable template
  `has_add_explicit_method_exact_v<Type, Arguments...>` is `false` where the call
  `std::remove_cvref_t<Type>::add(std::declval<Type>(), std::declval<Arguments>()...)` compiles,
  that is a member function with an explicit object parameter is not found beside such a static one.
  Where that call is ambiguous, for example for the static member functions `add(X, int)` and
  `add(X const &, int)`, the variable template `has_add_explicit_method_exact_v<X const &, int>` is
  `true` even though no member function with an explicit object parameter is declared. On MSVC
  the variable template is `false` in that case, as the item MSVC below explains.
- **Explicit object member function in the static strict form.** For the same reason the variable
  template `has_add_static_method_exact_v` is `true` also for a member function with an explicit
  object parameter whose type is the first of the types `Arguments...`. For the member function
  `int add(this X const &, int)`, for example, the variable template
  `has_add_static_method_exact_v<X, X const &, int>` is `true`, although no static member function
  `add` is declared. On MSVC the variable template is `false` here, as the item MSVC below
  explains.

### Limitations

- **MSVC.** On MSVC the access is not checked during substitution in the call of a non-static
  member function. So a private and a protected member function are found in the non-strict match,
  and so is a member function of a private or a protected base class. In the strict match and in
  the static forms a private or a protected member function is found on MSVC only where the class
  declares other overloads of that name. The address of a deleted member function is not rejected
  on MSVC either, so there the strict match also requires the plain call to compile, and the static
  strict form requires the call through the class name to compile. So on MSVC the variable
  templates `has_add_method_exact_v`, `has_add_static_method_exact_v` and
  `has_add_explicit_method_exact_v` are `false` where that call is ambiguous. A member function
  with an explicit object parameter cannot be called through the class name, so on MSVC
  the variable template `has_add_static_method_exact_v` is `false` for it. For the static member
  functions `add(X, int)` and `add(X const &, int)` the plain call `object.add(1)` does not
  compile, so on MSVC the variable template `has_add_explicit_method_exact_v<X const &, int>` is
  `false` for them.

## Examples

A class and the templates declared for its member functions:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp declare -->
```cpp
class counter
{
private:
    int m_value{};

public:
    int value() const noexcept { return m_value; }
    void add(int step) & { m_value += step; }
    static counter zero() { return {}; }
};

namespace counter_traits
{
    SCL_METHOD_DETECTION(value)
    SCL_METHOD_DETECTION(add)
    SCL_METHOD_DETECTION(zero)
} // namespace counter_traits
```

The non-strict match for objects with different qualifiers and value categories:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp call -->
```cpp
static_assert(counter_traits::has_value_method_v<counter>);
static_assert(counter_traits::has_value_method_v<counter const &>);
static_assert(std::is_same_v<counter_traits::value_method_t<counter>, int>);
static_assert(counter_traits::has_add_method_v<counter &, int>);
static_assert(counter_traits::has_add_method_v<counter &, short>);
static_assert(!counter_traits::has_add_method_v<counter, int>);
```

The strict match of the object qualifiers and the parameter types. The member function `value()` is
declared with the qualifier `const`, so the variable template is `true` for the type `counter const`
and `false` for the type `counter`. The member function `add` takes a parameter of the type `int`,
so for an argument type `short` the variable template is `false`:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp exact -->
```cpp
static_assert(counter_traits::has_value_method_exact_v<counter const>);
static_assert(!counter_traits::has_value_method_exact_v<counter>);
static_assert(counter_traits::has_add_method_exact_v<counter &, int>);
static_assert(!counter_traits::has_add_method_exact_v<counter &, short>);
```

The result type and the specifier `noexcept`, checked apart:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp properties -->
```cpp
static_assert(std::is_same_v<counter_traits::value_method_exact_t<counter const>, int>);
static_assert(noexcept(std::declval<counter const &>().value()));
```

A static member function:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp static -->
```cpp
static_assert(counter_traits::has_zero_static_method_v<counter>);
static_assert(counter_traits::has_zero_static_method_exact_v<counter>);
static_assert(!counter_traits::has_value_static_method_v<counter>);
```

## Notes

- A member function with the qualifier `&` and no `const` can be called only on an lvalue without
  `const`, that is for the type `X &`. One with `const &` can be called on any object without
  the qualifier `volatile`. One with `&&` can be called only on an rvalue, that is for the types `X`
  and `X &&`. A member function with the qualifier `const` can be called on an object without
  `const` as well, so the non-strict match finds it for both the type `X` and the type `X const`.
- The top-level qualifiers `const` and `volatile` of a parameter type are not part of a function
  type. So the strict match does not take them into account, and for the member function
  `add(int) &` the variable template `has_add_method_exact_v<X &, int const>` is `true` as well.
- The top-level qualifiers `const` and `volatile` of an explicit object parameter taken by value are
  not part of a function type either and do not count.

## See also

- [Function presence detection](function_detection.md) - the same checks for a function
- [Operator presence detection](operator_detection.md) - the same checks for an operator declared as
  a member function of a class
- [`example/type_traits/method_detection/type_traits_method_detection_example.cpp`](../../../../example/type_traits/method_detection/type_traits_method_detection_example.cpp)

---

Previous: [Function presence detection](function_detection.md) |
Next: [Operator presence detection](operator_detection.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
