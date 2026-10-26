# Cast of a value to the category and cv-qualifiers of a given type

The call `forward_like<Base>(t)` returns a reference to the argument `t`. The kind of the reference,
lvalue or rvalue, is taken from the type `Base`, and the qualifiers `const` and `volatile` of
the type `Base` are added to the qualifiers of the argument itself.

The alias template `forward_like_t<Base, Type>` is the type of the result of a call of the function
`forward_like` for an argument of the type `Type`.

- Header: `#include <scl/utility/type_traits/forward_like.h>`

```cpp
namespace scl
{
    template <typename Base, typename Type>
    using forward_like_t = unspecified;

    template <typename Base, typename Type>
    [[nodiscard]]
    constexpr decltype(auto) forward_like(Type && t) noexcept;
}
```

## Semantics

### Alias template `forward_like_t`

- **Kind of reference.** The alias template `forward_like_t<Base, Type>` is an lvalue reference if
  the type `Base` is an lvalue reference, and an rvalue reference otherwise, that is for a type with
  no reference and for an rvalue reference. The reference of the type `Type` itself does not affect
  the kind of reference of the result, so an argument of the type `double &` for the type `Base`
  equal to `int` is returned as `double &&`.
- **Cv-qualifiers.** The qualifiers `const` and `volatile` of the type `Base` are added to
  the cv-qualifiers of the type `Type`. The qualifiers the type `Type` already has stay, so the type
  of the result has the cv-qualifiers of both types. The type
  `forward_like_t<int const &, double volatile>`, for example, is `double const volatile &`.
- **Type `void`.** If the type `Type` is `void` with or without cv-qualifiers, the alias template
  `forward_like_t` is `void` with the cv-qualifiers of both types and with no reference.

### Function template `forward_like`

- **Cast.** The call `forward_like<Base>(t)` returns the argument `t` cast to the type
  `forward_like_t<Base, Type>`, the type `Type` being deduced from the argument. The function
  `forward_like` has no side effects. The argument is not copied or moved, and a reference to
  the same object is returned. The function is declared with the specifiers `constexpr` and
  `noexcept` and with the attribute `[[nodiscard]]`.

### Difference from `std::forward_like`

The function template `scl::forward_like` adds the qualifier `volatile` of the type `Base` to
the result type just as the qualifier `const`. The function template `std::forward_like` of C++23
does not add the qualifier `volatile` to its result type.

## Examples

The result type for different types `Base` and `Type`:

<!-- snippet: example/type_traits/forward_like/type_traits_forward_like_example.cpp type -->
```cpp
static_assert(std::is_same_v<scl::forward_like_t<int const &, double>, double const &>);
static_assert(std::is_same_v<scl::forward_like_t<int &&, double const>, double const &&>);
static_assert(std::is_same_v<scl::forward_like_t<int, double &>, double &&>);
static_assert(std::is_same_v<scl::forward_like_t<int volatile &, double>, double volatile &>);
```

A member passed on with the value category of the object it belongs to. The function `text_of`
returns the field `text` as an lvalue reference for an lvalue object and as an rvalue reference for
an rvalue object:

<!-- snippet: example/type_traits/forward_like/type_traits_forward_like_example.cpp member -->
```cpp
struct label
{
    std::string text;
};

template <typename Owner>
decltype(auto) text_of(Owner && owner)
{
    return scl::forward_like<Owner>(owner.text);
}

static_assert(std::is_same_v<decltype(text_of(std::declval<label &>())), std::string &>);
static_assert(std::is_same_v<decltype(text_of(std::declval<label const &>())), std::string const &>);
static_assert(std::is_same_v<decltype(text_of(std::declval<label>())), std::string &&>);
```

<!-- snippet: example/type_traits/forward_like/type_traits_forward_like_example.cpp move -->
```cpp
    label first{"first"};
    std::string const copied = text_of(first);
    std::string const moved = text_of(std::move(first));
    std::cout << copied << ' ' << moved << '\n'; // first first
```

## See also

- [`example/type_traits/forward_like/type_traits_forward_like_example.cpp`](../../../../example/type_traits/forward_like/type_traits_forward_like_example.cpp)

---

Previous: [Operator presence detection](operator_detection.md) |
Next: [Building the member pointer type from the object type](member_like.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
