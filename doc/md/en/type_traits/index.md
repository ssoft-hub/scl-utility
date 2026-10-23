# Type traits

The templates of the type traits group are meant for working with types and expressions while
the program compiles.

The detection templates tell whether an expression is valid with arguments of given types, whether
a function or a member function of a given name can be called, whether an operator applies to
operands, and what type the result has. Any expression is checked through an operation written in
an alias template. For a call of a function and of a member function the checking templates are
declared by macros, and for every operator they are already declared in the header `operator.h`.

The other templates of the group cast a value to the category and qualifiers of another type, build
the type of a pointer to a member from the object type, choose one overload of a function or
a member function, and find the parts of the signature of a callable type.

- Header: `#include <scl/utility/type_traits.h>`

| Header | Page | Declares |
|---|---|---|
| `<scl/utility/type_traits/detection.h>` | [Expression presence detection](detection.md) | `scl::is_detected`, `scl::detected_t`, `scl::detected_or`, `scl::is_detected_exact`, `scl::is_detected_convertible`, `scl::nonesuch` |
| `<scl/utility/type_traits/detection/function.h>` | [Function presence detection](function_detection.md) | `SCL_FUNCTION_DETECTION` |
| `<scl/utility/type_traits/detection/method.h>` | [Member function presence detection](method_detection.md) | `SCL_METHOD_DETECTION` |
| `<scl/utility/type_traits/detection/operator.h>` | [Operator presence detection](operator_detection.md) | `scl::has_plus_v`, `scl::plus_t`, `scl::has_plus_member_v` and the same for every operator |
| `<scl/utility/type_traits/forward_like.h>` | [Cast of a value to the category and cv-qualifiers of a given type](forward_like.md) | `scl::forward_like`, `scl::forward_like_t` |
| `<scl/utility/type_traits/member_like.h>` | [Building the member pointer type from the object type](member_like.md) | `scl::member_like_t`, `scl::member_function_like_t`, `scl::member_object_like_t` |
| `<scl/utility/type_traits/overload_cast.h>` | [Choosing an overload of a function or a member function by parameter types](overload_cast.md) | `scl::overload_cast`, `scl::member_overload_cast` |
| `<scl/utility/type_traits/signature.h>` | [Finding the signature of a callable type](signature.md) | `scl::signature_t`, `scl::signature::result_t` and the rest of the namespace `scl::signature` |

## Documentation of the group

### Detection

- [Expression presence detection](detection.md) - whether an expression written in an alias template
  is valid, and the type of its result
- [Function presence detection](function_detection.md) - whether a function of a given name can be
  called with arguments of given types, and whether it is declared with such parameters
- [Member function presence detection](method_detection.md) - the same checks for a member function
  of a given name and an object of a given type
- [Operator presence detection](operator_detection.md) - whether an operator applies to operands of
  given types, and whether it is declared as a member function of a class

### Building types

- [Cast of a value to the category and cv-qualifiers of a given type](forward_like.md) - a reference
  to the argument with the kind of reference and the cv-qualifiers of another type
- [Building the member pointer type from the object type](member_like.md) - the type of a pointer to
  a member function or a data member, with the qualifiers taken from the object type
- [Choosing an overload of a function or a member function by parameter types](overload_cast.md) -
  a pointer to an overload chosen by its exact parameter types, and for a member function also by
  the object type
- [Finding the signature of a callable type](signature.md) - the result type, the parameter types,
  the qualifiers and other properties of functions, member functions and lambda expressions

### Examples

- [`example/type_traits/detection`](../../../../example/type_traits/detection/type_traits_detection_example.cpp)
- [`example/type_traits/forward_like`](../../../../example/type_traits/forward_like/type_traits_forward_like_example.cpp)
- [`example/type_traits/function_detection`](../../../../example/type_traits/function_detection/type_traits_function_detection_example.cpp)
- [`example/type_traits/member_like`](../../../../example/type_traits/member_like/type_traits_member_like_example.cpp)
- [`example/type_traits/method_detection`](../../../../example/type_traits/method_detection/type_traits_method_detection_example.cpp)
- [`example/type_traits/operator_detection`](../../../../example/type_traits/operator_detection/type_traits_operator_detection_example.cpp)
- [`example/type_traits/overload_cast`](../../../../example/type_traits/overload_cast/type_traits_overload_cast_example.cpp)
- [`example/type_traits/signature`](../../../../example/type_traits/signature/type_traits_signature_example.cpp)

---

Next: [Expression presence detection](detection.md) | [Back to the main page](../Main.md)
