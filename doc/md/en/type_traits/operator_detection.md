# Operator presence detection

The templates of the header `operator.h` are meant for checking a C++ operator for operands of given
types:

- whether the operator applies to the operands, that is whether an expression with it compiles,
  whatever the operator is, built-in, a member function of a class or a free function
- whether the operator can be called as a member function of a class, the arguments being allowed to
  convert implicitly to the parameter types (non-strict match)
- whether the operator is declared as a member function with parameters that are exactly the given
  types and with qualifiers that match the type of the object (strict match).

The checks run while the program compiles. For each check a variable template `has_..._v` and
an alias template `..._t` for the result type are declared.

- Header: `#include <scl/utility/type_traits/detection/operator.h>`

The declarations for the binary operator `+`, named `plus`, are shown below. The same templates are
declared for every operator of the table, with the name of that operator in place of `plus` in their
names. The ways some operators differ from this scheme are described in the section Semantics.

```cpp
namespace scl
{
    template <typename Left, typename Right>
    using plus_operation = decltype(std::declval<Left>() + std::declval<Right>());
    template <typename... Arguments>
    inline constexpr auto has_plus_v = is_detected_v<plus_operation, Arguments...>;
    template <typename... Arguments>
    using plus_t = detected_t<plus_operation, Arguments...>;

    template <typename Type, typename... Arguments>
    using plus_member_operation = decltype(std::declval<Type>().operator+(std::declval<Arguments>()...));
    template <typename... Arguments>
    inline constexpr auto has_plus_member_v = is_detected_v<plus_member_operation, Arguments...>;
    template <typename... Arguments>
    using plus_member_t = detected_t<plus_member_operation, Arguments...>;

    template <typename Type, typename... Arguments>
    using plus_member_exact_operation = unspecified;
    template <typename... Arguments>
    inline constexpr auto has_plus_member_exact_v = is_detected_v<plus_member_exact_operation, Arguments...>;
    template <typename... Arguments>
    using plus_member_exact_t = detected_t<plus_member_exact_operation, Arguments...>;
}
```

## Operators

| Kind | Operator and name |
|---|---|
| Prefix | `&` `address_of`, `*` `indirection`, `+` `unary_plus`, `-` `negate`, `++` `prefix_increment`, `--` `prefix_decrement`, `~` `bit_not`, `!` `logical_not` |
| Postfix | `++` `postfix_increment`, `--` `postfix_decrement` |
| Comparison | `==` `equal_to`, `!=` `not_equal_to`, `<` `less`, `<=` `less_equal`, `>` `greater`, `>=` `greater_equal` |
| Arithmetic and bitwise | `*` `multiplies`, `/` `divides`, `%` `modulus`, `+` `plus`, `-` `minus`, `<<` `left_shift`, `>>` `right_shift`, `&` `bit_and`, `\|` `bit_or`, `^` `bit_xor` |
| Logical | `&&` `logical_and`, `\|\|` `logical_or` |
| Assignment | `=` `assign`, `*=` `multiplies_assign`, `/=` `divides_assign`, `%=` `modulus_assign`, `+=` `plus_assign`, `-=` `minus_assign`, `<<=` `left_shift_assign`, `>>=` `right_shift_assign`, `&=` `bit_and_assign`, `\|=` `bit_or_assign`, `^=` `bit_xor_assign` |
| Other | `->*` `pointer_to_member_access`, `->` `member_access`, `,` `comma`, `[]` `subscript`, `()` `call` |

Where the standard library has a function object for the same operator, such as `std::multiplies` or
`std::bit_and`, the name of the operator is the name of that class template.

## Semantics

### Expression form

- **Condition.** The variable template `has_plus_v<Left, Right>` is `true` if the expression
  `std::declval<Left>() + std::declval<Right>()` compiles, and `false` otherwise. The operator may
  be built-in, a member function of a class or a free function, and it is chosen by overload
  resolution. The alias template `plus_t<Left, Right>` is the result type of the expression, or
  the marker type [`scl::nonesuch`](detection.md) if the expression does not compile. For a prefix
  operator the expression templates take one operand type, as in `has_negate_v<int>`, for
  the operator `[]` the object type comes first, followed by the type of one index, as in
  `has_subscript_v<std::vector<int> &, std::size_t>`, and for the operator `()` the object type is
  followed by the argument types.
- **Value category of an operand.** Each operand is passed to the expression as `std::declval` of
  its type. For an operand type of the form `X &` the operand is an lvalue, and for the types `X`
  and `X &&` it is an rvalue. So for a built-in operator that needs an lvalue the operand type
  should be passed with the reference `&`, as in the example `has_prefix_increment_v<int &>` below.

### Member forms

- **Non-strict match.** The variable template `has_plus_member_v<Type, Arguments...>` is `true` if
  the call of the member function `std::declval<Type>().operator+(std::declval<Arguments>()...)`
  compiles. The arguments may convert implicitly to the parameter types. A built-in operator and
  a free function `operator+` do not count in this check, since it is the call of a member function
  that is checked. The alias template `plus_member_t` is the result type of that call. Only
  the forms of the member function check a call of `operator[]` with several indices (C++23), as in
  `has_subscript_member_v<X &, int, int>`, and the expression templates take one index.
- **Strict match.** The variable template `has_plus_member_exact_v<Type, Arguments...>` is `true` if
  the class `std::remove_cvref_t<Type>` or a public base class of it declares a member function
  `operator+` whose parameters are exactly the types `Arguments...` and whose qualifiers match
  the type `Type`, and the member function is neither deleted nor inaccessible. The qualifiers match
  as on the page of [member function presence detection](method_detection.md). The type `X const`,
  for example, needs a member function with the qualifier `const` and no ref-qualifier, the type
  `X &` needs one with the qualifier `&`, and so on.
- **Result type and an ambiguous call.** The strict match does not compare the result type of
  the member function. The alias template `plus_member_exact_t` is the result type of the member
  function found. The member function is found also where the call of the member function
  `operator+` is ambiguous, as for the overloads `long operator+(int) const` and
  `char operator+(int &&) const`. If the overloads of `operator+` include a member function
  template, only the overload whose result type is that of the call
  `std::declval<Type>().operator+(std::declval<Arguments>()...)` is found.
- **Specifier `noexcept`.** The strict match does not compare the specifier `noexcept` of
  the operator. It can be checked with the operator `noexcept` applied to the call
  `std::declval<Type>().operator+(std::declval<Arguments>()...)`. Where that call is ambiguous and
  the overloads differ not only in their qualifiers, the specifier shows in the type of the pointer
  to the needed overload that [`scl::overload_cast`](overload_cast.md) returns for the address of
  the operator.

### Operators with rules of their own

- **Operators `->` and `->*`.** For the operators `->` and `->*` no expression is checked, and
  the variable templates `has_member_access_v` and `has_pointer_to_member_access_v` are `true` if
  a call of the member function `operator->` or `operator->*` compiles. So for a built-in pointer
  and for a free function `operator->*` these variable templates are `false`.
- **Postfix `++` and `--`.** The templates `has_postfix_increment_member_v`,
  `has_postfix_increment_member_exact_v` and the other templates of the postfix operators `++` and
  `--` that check a member function take only the object type. The call `operator++(0)` or
  `operator--(0)` with a dummy argument of the type `int` is checked for that object.

### Limitations

- **MSVC.** On MSVC the access is checked as on the page of
  [member function presence detection](method_detection.md). So a private and a protected operator
  member function are found in the expression form and in the non-strict member form, and in
  the strict form only where the class declares other overloads of that operator. The strict match
  on MSVC also requires the call of the operator member function to compile, so there the variable
  template `has_plus_member_exact_v` is `false` where the call of the member function `operator+`
  is ambiguous.

## Examples

The types of the examples:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp types -->
```cpp
struct money
{
    long cents;

    money operator+(money other) const { return {cents + other.cents}; }
    bool operator==(money const & other) const = default;
};

money operator*(money amount, int times);
```

An expression and the type of its result. The addition of objects of the type `money` is declared as
a member function, and the multiplication by an `int` as a free function, and the variable template
of the expression is `true` for both operators:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp expression -->
```cpp
static_assert(scl::has_plus_v<money, money>);
static_assert(std::is_same_v<scl::plus_t<money, money>, money>);
static_assert(scl::has_multiplies_v<money, int>);
static_assert(!scl::has_multiplies_v<money, money>);
static_assert(scl::has_less_v<int, double>);
static_assert(!scl::has_less_v<money, money>);
```

The member function and the strict match. The member function `operator+` is declared with
the qualifier `const`, with no ref-qualifier and with the parameter `money`. So the variable
template of the strict match is `true` for an object of the type `money const` and an argument of
the type `money`. For an object of the type `money` it is `false`, since the member function has
the qualifier `const` and the object has not. For an argument of the type `money const &` it is
`false` as well, since the parameter type `money` is not exactly that type. The free function
`operator*` is not a member function, so the variable template `has_multiplies_member_v` is `false`:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp member -->
```cpp
static_assert(scl::has_plus_member_v<money, money>);
static_assert(!scl::has_multiplies_member_v<money, int>);
static_assert(scl::has_plus_member_exact_v<money const, money>);
static_assert(!scl::has_plus_member_exact_v<money, money>);
static_assert(!scl::has_plus_member_exact_v<money const, money const &>);
```

The subscript, the call and the prefix increment:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp special -->
```cpp
static_assert(scl::has_subscript_v<std::vector<int> &, std::size_t>);
static_assert(std::is_same_v<scl::subscript_t<std::vector<int> &, std::size_t>, int &>);
static_assert(scl::has_call_v<std::plus<>, int, int>);
static_assert(scl::has_prefix_increment_v<int &>);
static_assert(!scl::has_prefix_increment_v<int>);
```

## Notes

- The built-in prefix increment applies only to an lvalue. So the variable template
  `has_prefix_increment_v<int>` is `false`, and the variable template
  `has_prefix_increment_v<int &>` is `true`.
- A postfix operator differs from the prefix one by a dummy parameter of the type `int`, which tells
  the declarations `operator++()` and `operator++(int)` apart.
- The top-level qualifiers `const` and `volatile` of a parameter type are not part of a function
  type. So the strict match does not take them into account, and for the member function
  `operator+(money) const` the variable template `has_plus_member_exact_v<money const, money const>`
  is `true` as well.

## See also

- [Member function presence detection](method_detection.md) - the same checks for a member function
  of any name
- [Expression presence detection](detection.md) - the templates `scl::is_detected` and
  `scl::detected_t`, on which the templates of the header `operator.h` are built
- [`example/type_traits/operator_detection/type_traits_operator_detection_example.cpp`](../../../../example/type_traits/operator_detection/type_traits_operator_detection_example.cpp)

---

Previous: [Member function presence detection](method_detection.md) |
Next: [Cast of a value to the category and cv-qualifiers of a given type](forward_like.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
