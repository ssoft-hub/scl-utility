# Building the member pointer type from the object type

The alias templates `member_function_like_t`, `member_object_like_t` and `member_like_t` are types
of a pointer to a member function or a data member of a class, built from the type of the object
the member is used on. The class of the pointer and the qualifiers of the member come from the type
of the object. For an object of the type `gauge const &`, for example, a pointer to a member
function has the qualifiers `const &`, and a pointer to a data member points to a member with
the qualifier `const`.

Such a type is needed, for example, where the overloads of a member function differ only in their
qualifiers. The address `&gauge::label` then has no single type, and the overload for a given object
type is chosen by casting the address to the pointer type built.

- Header: `#include <scl/utility/type_traits/member_like.h>`

```cpp
namespace scl
{
    template <typename Type, typename Signature>
    using member_function_like_t = unspecified;

    template <typename Type, typename Member>
    using member_object_like_t = unspecified;

    template <typename Type, typename Member>
    using member_like_t = unspecified;
}
```

## Semantics

### General rules

- **Class.** Each of the alias templates of this page is the type of a pointer to a member of
  the class `std::remove_cvref_t<Type>`, that is of the class of the object without cv-qualifiers
  and reference.

### Pointer to a member function

- **`member_function_like_t<Type, Signature>`.** The alias template is the type of a pointer to
  a member function. The result type, the parameter types and the specifier `noexcept` of the member
  function come from the function type `Signature`, written as `Result(Parameters...)` or
  `Result(Parameters...) noexcept`. The cv-qualifiers of the member function come from the type
  `Type`, and its ref-qualifier is `&` for the type `X &`, `&&` for the type `X &&`, and none for
  the type `X` with no reference. The type `member_function_like_t<gauge const &, std::string()>`,
  for example, is `std::string (gauge::*)() const &`.
- **Unsuitable type `Signature`.** For a function type whose parameter list ends with a C-style
  ellipsis `...`, such as `void(int, ...)`, and for a function type with cv- or ref-qualifiers, such
  as `void() const`, the alias template `member_function_like_t` is not defined. For such a type
  the substitution fails without a build error, so this can be checked with the templates of
  [expression presence detection](detection.md). If the type `Signature` is not a function type at
  all, or the type `std::remove_cvref_t<Type>` is not a class, the build ends with an error.

### Pointer to a data member

- **`member_object_like_t<Type, Member>`.** The alias template is the type of a pointer to a data
  member whose type is the type `Member` with the qualifiers `const` and `volatile` of the type
  `Type` added. The cv-qualifiers of the type `Member` itself stay, and the reference of the type
  `Type` does not affect the result. The type `member_object_like_t<gauge volatile, int const>`, for
  example, is `int const volatile gauge::*`. The build ends with an error if the type `Member` is
  not an object type, or the type `std::remove_cvref_t<Type>` is neither a class nor a union.

### Choice between a member function and a data member

- **`member_like_t<Type, Member>`.** If the type `Member` is a function type, the alias template
  `member_like_t` is the type `member_function_like_t<Type, Member>`, and otherwise it is the type
  `member_object_like_t<Type, Member>`. The build ends with an error if the type `Member` is
  a reference, a reference to a function included.

## Examples

The type of the examples:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp types -->
```cpp
struct gauge
{
    int level;
    std::string unit;

    std::string label() const & { return unit; }
    std::string label() && { return std::move(unit); }
    void set(int value) noexcept { level = value; }
};
```

A pointer to a member function with the cv-qualifiers and the reference of the object type:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp function -->
```cpp
static_assert(std::is_same_v<
    scl::member_function_like_t<gauge const &, std::string()>, std::string (gauge::*)() const &>);
static_assert(
    std::is_same_v<scl::member_function_like_t<gauge &&, std::string()>, std::string (gauge::*)() &&>);
static_assert(
    std::is_same_v<scl::member_function_like_t<gauge, void(int) noexcept>, void (gauge::*)(int) noexcept>);
```

A pointer to a data member with the cv-qualifiers of the object type:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp object -->
```cpp
static_assert(std::is_same_v<scl::member_object_like_t<gauge, int>, int gauge::*>);
static_assert(std::is_same_v<scl::member_object_like_t<gauge const &, int>, int const gauge::*>);
static_assert(
    std::is_same_v<scl::member_object_like_t<gauge volatile, int const>, int const volatile gauge::*>);
```

The kind of pointer in the alias template `member_like_t` depends on the type of the member:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp member_like -->
```cpp
static_assert(std::is_same_v<scl::member_like_t<gauge, void(int) noexcept>, decltype(&gauge::set)>);
static_assert(std::is_same_v<scl::member_like_t<gauge, int>, decltype(&gauge::level)>);
```

An overload chosen by the object type. The member functions `label` differ only in their qualifiers,
so a variable declared with the specifier `auto` cannot be initialized with the address
`&gauge::label`. The address is cast to the pointer type built from the object type, so the overload
`const &` is chosen for a constant lvalue, and the overload `&&` for a non-constant rvalue. For
a non-constant lvalue the type `std::string (gauge::*)() &` is built, and for a constant rvalue
the type `std::string (gauge::*)() const &&`. No overload has such qualifiers, so the cast does not
compile, although the member function `label` can be called on a non-constant lvalue.

The type `Object &&`, not the type `Object`, is passed to the alias template `member_like_t` as its
first argument. For an lvalue argument the parameter `Object` is deduced as an lvalue reference, and
for an rvalue argument as a type with no reference. The type `Object &&` keeps the value category of
the argument in both cases.

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp overload -->
```cpp
template <typename Object>
std::string label_of(Object && object)
{
    auto const label = static_cast<scl::member_like_t<Object &&, std::string()>>(&gauge::label);
    return (std::forward<Object>(object).*label)();
}
```

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp use -->
```cpp
    gauge tank{7, "litre"};
    gauge const & view = tank;

    std::string const copied = label_of(view);
    std::string const moved = label_of(std::move(tank));
    std::cout << copied << ' ' << moved << '\n'; // litre litre
```

## Notes

- The type of a pointer to a function is not a function type. So for a type `Member` such as
  `void (*)(int)` the alias template `member_like_t` is the type of a pointer to a data member whose
  type is that pointer to a function.

## See also

- [Finding the signature of a callable type](signature.md) - the alias template
  `scl::signature::object_t`, which fits as the first argument of the alias template `member_like_t`
- [`example/type_traits/member_like/type_traits_member_like_example.cpp`](../../../../example/type_traits/member_like/type_traits_member_like_example.cpp)

---

Previous: [Cast of a value to the category and cv-qualifiers of a given type](forward_like.md) |
Next: [Choosing an overload of a function or a member function by parameter types](overload_cast.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
