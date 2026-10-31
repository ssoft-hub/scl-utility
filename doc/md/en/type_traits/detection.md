# Expression presence detection

The templates of the detection idiom check, while the program compiles, whether an expression is
valid with arguments of given types, find the type of its result, and compare that type with
an expected one.

The expression to check is written in an alias template whose parameters are the argument types.
Such an alias template is called an operation. A class template can be an operation as well, as
the subsection Well-formed operation of the section Semantics describes. The operation
`size_operation` below, for example, describes a call of the member function `size()` on a constant
reference to a container. The operation is passed to every template of this page as the parameter
`Operation`, and the types substituted into it follow as `Arguments...`.

The names of the templates are taken from the Library Fundamentals TS v2. The templates
`is_detected_exact` and `is_detected_convertible` behave differently from the TS for an ill-formed
operation, and the subsection Difference from Library Fundamentals TS v2 of the section Semantics
describes the difference.

- Header: `#include <scl/utility/type_traits/detection.h>`

```cpp
namespace scl
{
    struct nonesuch;

    template <template <typename...> class Operation, typename... Arguments>
    using is_detected = unspecified;
    template <template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_v = is_detected<Operation, Arguments...>::value;

    template <template <typename...> class Operation, typename... Arguments>
    using detected_t = unspecified;

    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or = unspecified;
    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or_t = detected_or<Default, Operation, Arguments...>::type;

    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    using is_detected_exact = std::bool_constant<(
        is_detected_v<Operation, Arguments...> && std::is_same_v<Expected, detected_t<Operation, Arguments...>>)>;
    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_exact_v = is_detected_exact<Expected, Operation, Arguments...>::value;

    template <typename To, template <typename...> class Operation, typename... Arguments>
    using is_detected_convertible = std::bool_constant<(
        is_detected_v<Operation, Arguments...> && std::is_convertible_v<detected_t<Operation, Arguments...>, To>)>;
    template <typename To, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_convertible_v =
        is_detected_convertible<To, Operation, Arguments...>::value;
}
```

## Semantics

### Well-formed operation

- **Operation.** An operation is an alias template or a class template. The operation is called
  well-formed for the types `Arguments...` if substituting these types into its declaration
  succeeds. The declaration of an alias template is the type that defines it. The operation
  `size_operation<Container>`, for example, is defined by the type
  `decltype(std::declval<Container const &>().size())` and is well-formed for `std::vector<int>`,
  but not for `int`, which has no member function `size()`. The operation
  `value_type_operation<Container>` is defined by the type `typename Container::value_type` and is
  well-formed only for a type that declares a nested type `value_type`.
- **Class template as an operation.** For a class template only the substitution into its
  declaration is checked, that is into the template parameters and the `requires` clause, and
  the body of the class is not. So an error in the body of the class does not make the operation
  ill-formed. The expression to check should be written in an alias template.
- **Error in a function body.** If the operation calls a function whose result type is deduced from
  its body, such as a function template returning `auto` or `decltype(auto)` or a generic lambda
  expression, the body is instantiated to find the result type. An error in that body does not make
  the operation ill-formed, and the build ends with a message about that error instead. If
  the result type is written in the declaration of the function, the body is not needed for
  the check and is not instantiated. For the operation to be ill-formed for unsuitable arguments,
  the condition should be written in the declaration of the function, that is in its result type or
  in a `requires` clause.

### Templates and the marker type `nonesuch`

- **`is_detected`.** The alias template `is_detected<Operation, Arguments...>` is the type
  `std::true_type` if the operation is well-formed for the types `Arguments...`, and the type
  `std::false_type` otherwise. The variable template `is_detected_v` is
  `is_detected<Operation, Arguments...>::value`.
- **`detected_t`.** The alias template `detected_t<Operation, Arguments...>` is the type
  `Operation<Arguments...>` if the operation is well-formed. Where the operation is written as
  `decltype(expression)`, this is the type of the expression itself. For
  `size_operation<std::vector<int>>`, for example, it is `std::size_t`. If the operation is
  ill-formed, the alias template `detected_t` is the marker type `scl::nonesuch`.
- **`nonesuch`.** The marker type `nonesuch` stands for the result of an ill-formed operation and
  serves no other purpose. No object of the type can be created, copied, moved or destroyed.
- **`detected_or`.** The alias template `detected_or<Default, Operation, Arguments...>` is a class
  with the members `type` and `value_t`. The member `type` is the type `Operation<Arguments...>` if
  the operation is well-formed, and the type `Default` otherwise. The member `value_t` is the type
  `is_detected<Operation, Arguments...>`. The alias template `detected_or_t` is the type
  `detected_or<Default, Operation, Arguments...>::type`, that is the alias template `detected_t`
  with the marker type `nonesuch` replaced by the type `Default`.
- **`is_detected_exact`.** The alias template `is_detected_exact<Expected, Operation, Arguments...>`
  is the type `std::true_type` if the operation is well-formed and its type is exactly the type
  `Expected`. Otherwise it is the type `std::false_type`.
- **`is_detected_convertible`.** The alias template
  `is_detected_convertible<To, Operation, Arguments...>` is the type `std::true_type` if
  the operation is well-formed and its type converts implicitly to the type `To`. Otherwise it is
  the type `std::false_type`.

### Difference from Library Fundamentals TS v2

For an ill-formed operation the variable templates `is_detected_exact_v` and
`is_detected_convertible_v` are always `false`, whatever type is passed as `Expected` or `To`.
In the TS these templates check the type `detected_t<Operation, Arguments...>`, which for
an ill-formed operation is the marker type `nonesuch`. So in the TS they are `true` where the type
`Expected` is the type `nonesuch` or where the type `nonesuch` converts to the type `To`.

Values of the variable templates for an ill-formed operation:

| Variable template | `scl` | TS |
|---|---|---|
| `is_detected_exact_v<nonesuch, Operation, Arguments...>` | `false` | `true` |
| `is_detected_convertible_v<nonesuch const &, Operation, Arguments...>` | `false` | `true` |
| `is_detected_convertible_v<To, Operation, Arguments...>` for a class `To` with a constructor `template <typename T> To(T &&)` | `false` | `true` |

## Examples

The operations of the examples below:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp operations -->
```cpp
template <typename Container>
using size_operation = decltype(std::declval<Container const &>().size());

template <typename Container>
using value_type_operation = typename Container::value_type;
```

Whether an operation is well-formed:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp is_detected -->
```cpp
static_assert(scl::is_detected_v<size_operation, std::vector<int>>);
static_assert(!scl::is_detected_v<size_operation, int>);
static_assert(std::is_same_v<scl::is_detected<size_operation, int>, std::false_type>);
```

The result type of a well-formed operation, and the marker type `scl::nonesuch` for an ill-formed
one:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp detected_t -->
```cpp
static_assert(std::is_same_v<scl::detected_t<value_type_operation, std::vector<long>>, long>);
static_assert(std::is_same_v<scl::detected_t<value_type_operation, int>, scl::nonesuch>);
```

The type `Default` in place of the result of an ill-formed operation:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp detected_or -->
```cpp
static_assert(std::is_same_v<scl::detected_or_t<int, value_type_operation, std::string>, char>);
static_assert(std::is_same_v<scl::detected_or_t<int, value_type_operation, double>, int>);
static_assert(scl::detected_or<int, value_type_operation, std::string>::value_t::value);
static_assert(!scl::detected_or<int, value_type_operation, double>::value_t::value);
```

The result type compared with an expected type, and checked for a conversion to another type:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp exact -->
```cpp
static_assert(scl::is_detected_exact_v<std::size_t, size_operation, std::vector<int>>);
static_assert(!scl::is_detected_exact_v<int, size_operation, std::vector<int>>);
static_assert(scl::is_detected_convertible_v<double, size_operation, std::vector<int>>);
static_assert(!scl::is_detected_convertible_v<std::string, size_operation, std::vector<int>>);
```

A branch of the program chosen by the result of the check. The function `count_of` returns the size
of a container if the argument has a member function `size()`, and `1` for any other argument:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp dispatch -->
```cpp
template <typename Container>
std::size_t count_of(Container const & container)
{
    if constexpr (scl::is_detected_v<size_operation, Container>)
        return container.size();
    else
        return 1;
}
```

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp run -->
```cpp
    std::cout << count_of(std::array<int, 3>{1, 2, 3}) << '\n'; // 3
    std::cout << count_of(42) << '\n';                          // 1
```

## See also

- [Function presence detection](function_detection.md),
  [member function presence detection](method_detection.md) and
  [operator presence detection](operator_detection.md) - operations for calling a function and
  a member function, declared by macros, and ready operations for every operator
- [`example/type_traits/detection/type_traits_detection_example.cpp`](../../../../example/type_traits/detection/type_traits_detection_example.cpp)

---

Previous: [Type traits](index.md) | Next: [Function presence detection](function_detection.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
