# Finding the signature of a callable type

The alias template `signature_t` and the templates of the namespace `signature` tell, while
the program compiles, the parts of the signature of a callable type, such as a function type,
a pointer to a member function or the type of a lambda expression. This way a function template
taking a callback can be constrained by the type of the callback's parameter, as in the last example
of this page. The signature includes the function type, the result type, the parameter types,
the object type, the cv-qualifiers, the ref-qualifier, the specifier `noexcept` and a trailing
ellipsis `...`.

- Header: `#include <scl/utility/type_traits/signature.h>`

```cpp
namespace scl
{
    template <typename Callable>
    using signature_t = unspecified;
}

namespace scl::signature
{
    template <typename Callable>
    using result_t = unspecified;

    template <typename Callable>
    using parameters_t = std::tuple</* parameter types */>;

    template <typename Callable>
    inline constexpr std::size_t parameter_count_v = std::tuple_size_v<parameters_t<Callable>>;

    template <typename Callable, std::size_t Index>
    using parameter_t = unspecified;

    template <typename Callable>
    using object_t = unspecified;

    template <typename Callable>
    inline constexpr bool is_const_v = unspecified;

    template <typename Callable>
    inline constexpr bool is_volatile_v = unspecified;

    template <typename Callable>
    inline constexpr bool is_lvalue_reference_v = unspecified;

    template <typename Callable>
    inline constexpr bool is_rvalue_reference_v = unspecified;

    template <typename Callable>
    inline constexpr bool is_noexcept_v = unspecified;

    template <typename Callable>
    inline constexpr bool is_variadic_v = unspecified;
}
```

The examples on this page use the class `printer` and the type `print_t` of a pointer to its member
function `print`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp types -->
```cpp
struct printer
{
    int print(long value, std::string const & label) const & noexcept;
    void clear();
    void clear(int line);
};

using print_t = decltype(&printer::print); // int (printer::*)(long, std::string const &) const & noexcept
```

The templates of this page and their values for the type `print_t`:

| Template | Property | Value for the type `print_t` |
|---|---|---|
| `scl::signature_t<F>` | the function type | `int(long, std::string const &) noexcept` |
| `scl::signature::result_t<F>` | the result type | `int` |
| `scl::signature::parameters_t<F>` | the parameter types as `std::tuple` | `std::tuple<long, std::string const &>` |
| `scl::signature::parameter_count_v<F>` | the number of parameters | `2` |
| `scl::signature::parameter_t<F, I>` | the parameter type at the index `I` | `std::string const &` for `I = 1` |
| `scl::signature::object_t<F>` | the type of the object the member function is called on | `printer const &` |
| `scl::signature::is_const_v<F>` | whether a member function is declared `const` | `true` |
| `scl::signature::is_volatile_v<F>` | whether a member function is declared `volatile` | `false` |
| `scl::signature::is_lvalue_reference_v<F>` | whether a member function carries the ref-qualifier `&` | `true` |
| `scl::signature::is_rvalue_reference_v<F>` | whether a member function carries the ref-qualifier `&&` | `false` |
| `scl::signature::is_noexcept_v<F>` | whether a function is declared `noexcept` | `true` |
| `scl::signature::is_variadic_v<F>` | whether the function takes a variable number of arguments | `false` |

## Callable types

The parameter `F` can be:

- a function type, such as `int(long)`
- a pointer or a reference to a function
- a pointer to a member function with any combination of cv-qualifiers, a ref-qualifier and
  `noexcept`
- a class or a union with one call operator that is not a template, the type of a lambda included.

Any of these types may describe a C-style variadic function, one whose parameter list ends with
the ellipsis `...`, as that of `int printf(char const *, ...)` does. A class with cv-qualifiers and
a reference to a class give the same values as the class itself. A class must be complete, and a use
of the templates of this page for a class before its definition makes the program ill-formed, no
diagnostic required.

A call operator can take the object through an explicit object parameter (C++23). That parameter is
not among the parameters of the type `signature_t<F>`, and its type is `signature::object_t<F>`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp explicit -->
```cpp
#if defined(__cpp_explicit_this_parameter)
struct stepper
{
    int operator()(this stepper const & self, long step);
};

static_assert(std::is_same_v<scl::signature_t<stepper>, int(long)>);
static_assert(std::is_same_v<scl::signature::object_t<stepper>, stepper const &>);
#endif
```

A static call operator, also introduced in C++23, has no object, and all of its parameters are among
the parameters of the type `signature_t<F>`.

A generic lambda, a class with an overloaded call operator and a type whose object is not callable
have no single signature. Neither do a pointer to a data member, a function type with cv- or
ref-qualifiers such as `int() const &`, and a function with a calling convention other than
the default, such as `__vectorcall`, or `__stdcall` on 32-bit x86. For such types a use of
the templates of this page does not compile. Substitution into the alias template `signature_t` then
gives no type, so whether a signature exists can be checked with a requires-expression:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp absent -->
```cpp
template <typename Callable>
concept has_signature = requires { typename scl::signature_t<Callable>; };

[[maybe_unused]]
auto const generic = [](auto value) { return value; };
static_assert(!has_signature<decltype(generic)>);
static_assert(!has_signature<int>);
```

For an overloaded function or an overloaded member function, the overload should first be selected
with the function object `scl::overload_cast`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp overload -->
```cpp
using clear_t = decltype(scl::overload_cast<int>(&printer::clear));

static_assert(std::is_same_v<scl::signature_t<clear_t>, void(int)>);
```

## `signature_t`

The alias template `signature_t` is the function type with the pointer, the class, the cv-qualifiers
and the ref-qualifier removed. The specifier `noexcept` and a trailing ellipsis `...` are kept in
that type.

## `signature::result_t`

The alias template `signature::result_t` is the result type.

## `signature::parameters_t`, `signature::parameter_count_v` and `signature::parameter_t`

The alias template `signature::parameters_t` is the specialization of the template `std::tuple`
whose arguments are the parameter types of the function in the order of their declaration.
The variable template `signature::parameter_count_v` is the number of parameters of the function.
A trailing ellipsis `...` is not among the types of the alias template `signature::parameters_t` and
does not count in the variable template `signature::parameter_count_v`.

The alias template `signature::parameter_t` is the parameter type at the index `Index`. Where
the value of the parameter `Index` is not below the number of parameters of the function, a use of
the alias template `signature::parameter_t` does not compile, which a requires-expression can check
as well.

The function type, the result type and the parameter types of `printer::print`. Here and below
`print_t` is the type of a pointer to that member function, declared at the top of the page:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp signature -->
```cpp
static_assert(std::is_same_v<scl::signature_t<print_t>, int(long, std::string const &) noexcept>);
static_assert(std::is_same_v<scl::signature::result_t<print_t>, int>);
static_assert(
    std::is_same_v<scl::signature::parameters_t<print_t>, std::tuple<long, std::string const &>>);
static_assert(std::is_same_v<scl::signature::parameter_t<print_t, 1>, std::string const &>);
```

## `signature::object_t`

The alias template `signature::object_t` is the type of the object the member function or the call
operator is called on. That type is the class of the member function with the cv-qualifiers and
the ref-qualifier the member function is declared with. Where a member function of the class
`widget` is declared `const &`, it is `widget const &`, and where it has no qualifiers, it is
`widget`. The class without qualifiers can be determined with
`std::remove_cvref_t<signature::object_t<F>>`.

For a class with a call operator, the alias template `signature::object_t` is that class with
the cv-qualifiers and the ref-qualifier of the call operator, such as `L const` for the type `L` of
a lambda expression, even where the call operator is declared in a base class. For a call operator
with an explicit object parameter, the alias template `signature::object_t` is the type of that
parameter, also where the call operator is declared in a base class.

The object type has the form of the parameter `Type` of the alias template `scl::member_like_t`, so
for a pointer to a member function whose parameter list does not end with an ellipsis the type of
that pointer can be obtained as `scl::member_like_t<signature::object_t<F>, signature_t<F>>`. For
the type `print_t` this is checked below:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp object -->
```cpp
static_assert(std::is_same_v<scl::signature::object_t<print_t>, printer const &>);
static_assert(std::is_same_v<
    scl::member_like_t<scl::signature::object_t<print_t>, scl::signature_t<print_t>>, print_t>);
```

For a free function and a static call operator a use of the alias template `signature::object_t`
does not compile.

## `signature::is_const_v` and `signature::is_volatile_v`

The variable template `signature::is_const_v` is `true` where the member function or the call
operator is declared `const`. The variable template `signature::is_volatile_v` is `true` where
the member function or the call operator is declared `volatile`. For a free function and a member
function with no cv-qualifiers both are `false`. For a call operator with an explicit object
parameter the values are determined by the type of that parameter.

## `signature::is_lvalue_reference_v` and `signature::is_rvalue_reference_v`

The variable template `signature::is_lvalue_reference_v` is `true` where the member function or
the call operator is declared with the ref-qualifier `&`. The variable template
`signature::is_rvalue_reference_v` is `true` where the member function or the call operator is
declared with the ref-qualifier `&&`. For a free function and a member function with no
ref-qualifier both are `false`. For a call operator with an explicit object parameter the values
follow the type of that parameter.

## `signature::is_noexcept_v` and `signature::is_variadic_v`

The variable template `signature::is_noexcept_v` is `true` where the function, the member function
or the call operator is declared `noexcept`. The value follows the declaration rather than a call.

The variable template `signature::is_variadic_v` is `true` where the function, the member function
or the call operator takes a variable number of arguments, that is, its parameter list ends with
the ellipsis `...`, as that of the function type `int(char const *, ...)` does.

## Examples

The qualifiers and `noexcept` of `printer::print`, a pointer to which has the type `print_t`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp qualifiers -->
```cpp
static_assert(scl::signature::is_const_v<print_t>);
static_assert(!scl::signature::is_volatile_v<print_t>);
static_assert(scl::signature::is_lvalue_reference_v<print_t>);
static_assert(!scl::signature::is_rvalue_reference_v<print_t>);
static_assert(scl::signature::is_noexcept_v<print_t>);
```

A callback whose parameter is an unsigned type at least as wide as `std::uint16_t` can be passed to
the function template `on_port`. A generic lambda can be passed too, and since it has no signature,
only the call with an argument of the type `std::uint16_t` is checked. A callback whose parameter
has the type `std::uint8_t` is refused:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp constrain -->
```cpp
template <typename Callback>
void on_port(Callback && callback)
    requires(std::is_invocable_v<Callback, std::uint16_t>) &&
    (!has_signature<Callback> ||
        (std::is_unsigned_v<std::remove_cvref_t<scl::signature::parameter_t<Callback, 0>>> &&
            sizeof(scl::signature::parameter_t<Callback, 0>) >= sizeof(std::uint16_t)))
{
    std::forward<Callback>(callback)(std::uint16_t{8080});
}

template <typename Callback>
concept accepted_by_on_port = requires(Callback callback) { on_port(callback); };

[[maybe_unused]]
auto const narrow = [](std::uint8_t port) { return port; };
static_assert(!accepted_by_on_port<decltype(narrow)>);
```

## Notes

- A C-style variadic function, such as `int(char const *, ...)`, is not a function template with
  a parameter pack. A specialization of such a template, such as `f<int, long>`, has a finite
  parameter list, and for it the variable template `signature::is_variadic_v` is `false`.
- For a member function the values of the variable templates `signature::is_lvalue_reference_v` and
  `signature::is_rvalue_reference_v` equal those of the variable templates
  `std::is_lvalue_reference_v` and `std::is_rvalue_reference_v` for the type
  `signature::object_t<F>`.
- For the function `void f(std::string) noexcept` the variable template `signature::is_noexcept_v`
  is `true`, although copying an argument into the parameter can throw.

## See also

- [`scl::member_like_t`](member_like.md)
- [`scl::overload_cast`](overload_cast.md)
- [`example/type_traits/signature/type_traits_signature_example.cpp`](../../../../example/type_traits/signature/type_traits_signature_example.cpp)

---

Previous: [Choosing an overload of a function or a member function by parameter types](overload_cast.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
