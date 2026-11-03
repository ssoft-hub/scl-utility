# Choosing an overload of a function or a member function by parameter types

The address of an overloaded function `&f` or of an overloaded member function `&Class::f` has no
single type, so it cannot be stored in a variable declared with the specifier `auto` or passed to
a function template. The function object `overload_cast<Arguments...>` chooses from the overload set
the overload whose parameters are exactly the types `Arguments...`, and returns a pointer to it.

Where the overloads of a member function differ only in their qualifiers, such as `int & count() &`
and `int count() const &`, the parameter types cannot choose between them. For that case
the function object `member_overload_cast<Type, Arguments...>` chooses the member function whose
parameters are exactly the types `Arguments...` and whose qualifiers match the object type `Type`.

- Header: `#include <scl/utility/type_traits/overload_cast.h>`

```cpp
namespace scl
{
    template <typename... Arguments>
    inline constexpr auto overload_cast = unspecified;

    template <typename Type, typename... Arguments>
    inline constexpr auto member_overload_cast = unspecified;
}
```

## Semantics

### Function object `overload_cast`

- **Call.** The calls `overload_cast<Arguments...>(&f)` and `overload_cast<Arguments...>(&Class::f)`
  return a pointer to the overload of the function or the member function whose parameters are
  exactly the types `Arguments...`. The result type and the specifier `noexcept` of the overload may
  be any, and they stay in the type of the pointer returned. For the overloads `int twice(int)` and
  `double twice(double) noexcept`, for example, the call `overload_cast<double>(&twice)` returns
  a pointer of the type `double (*)(double) noexcept`. The call operator of the function object is
  declared with the specifiers `constexpr` and `noexcept` and with the attribute `[[nodiscard]]`.
- **Exact parameter types.** A parameter type has to be exactly the type from `Arguments...`, and no
  implicit conversion counts in the choice. So for the overloads `f(int &)` and `f(int const &)`
  the calls `overload_cast<int &>(&f)` and `overload_cast<int const &>(&f)` choose different
  overloads. The top-level qualifiers `const` and `volatile` of the types `Arguments...` do not
  count, as the section Notes explains.
- **Qualifiers of a member function.** The qualifiers of the member function itself (`const`,
  `volatile`, `&` and `&&`) do not count in the choice, that is a member function with the right
  parameters is chosen whatever its qualifiers. The pointer returned has the type of the chosen
  overload, these qualifiers included.
- **No overload, or several.** The call does not compile if no overload has the parameters
  `Arguments...`. Nor does it compile if several member functions with the parameters `Arguments...`
  differ only in their qualifiers, such as `int f(int)` and `int f(int) const`, since the parameter
  types cannot choose between them. The function object `member_overload_cast` chooses one of such
  overloads.
- **Member function with an explicit object parameter.** The address of a member function with
  an explicit object parameter (C++23), such as `int f(this Class const & self, int)`, is a plain
  pointer to a function of the type `int (*)(Class const &, int)`. So such a member function is
  chosen by the call `overload_cast<Class const &, int>(&Class::f)`, where the type of the object
  parameter comes first.

### Function object `member_overload_cast`

- **Call.** The call `member_overload_cast<Type, Arguments...>(&Class::f)` returns a pointer to
  the member function whose parameters are exactly the types `Arguments...` and whose qualifiers
  match the object type `Type`. The cv-qualifiers of the member function have to be those of
  the type `Type`, and its ref-qualifier has to be `&` for an lvalue reference, `&&` for an rvalue
  reference and none for a type with no reference, as in the alias template
  [`member_function_like_t`](member_like.md). For the overloads `int & count() &` and
  `int count() const &`, for example, the call
  `member_overload_cast<counter const &>(&counter::count)` chooses the second one. A member function
  with no ref-qualifier is chosen only for a type with no reference, so the member function
  `int level() const` is chosen for the type `gauge const`, and with the type `gauge const &`
  the call does not compile, although such a member function can be called on an object of that
  type. The result type and the specifier `noexcept` of the member function may be any. The pointer
  returned is a pointer to a member of the class that declares the member function. The function
  object `member_overload_cast` never chooses a member function with an explicit object parameter,
  since the address of such a member function is not a pointer to a member. The call operator is
  declared with the specifiers `constexpr` and `noexcept` and with the attribute `[[nodiscard]]`.

### Limitations

- **Function templates.** A call of either function object does not compile if the overload set
  holds a function template, even where the overload with the parameters `Arguments...` is not
  a template.
- **Functions with an ellipsis.** An overload whose parameter list ends with a C-style ellipsis
  `...` is never chosen. A set that holds, beside the overload with the parameters `Arguments...`,
  an overload with the same parameters and an ellipsis is not supported, since the outcome of
  the call for such a set depends on the compiler.

## Examples

The types of the examples:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp types -->
```cpp
class canvas
{
private:
    int m_width{};

public:
    void resize(int width) { m_width = width; }
    void resize(int width, int height) noexcept { m_width = width * height; }
    int width() const & { return m_width; }
};

int twice(int value);
double twice(double value) noexcept;

class counter
{
private:
    int m_count{};

public:
    int & count() & { return m_count; }
    int count() const & { return m_count; }
};

class gauge
{
private:
    int m_level{};

public:
    int & level() { return m_level; }
    int level() const { return m_level; }
};
```

Overloads of a member function that differ in their parameters. The member function `width` has no
overloads, but the call `overload_cast<>` fits it as well and returns a pointer with the qualifiers
`const &`:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp member -->
```cpp
[[maybe_unused]]
auto const resize_one = scl::overload_cast<int>(&canvas::resize);
auto const resize_two = scl::overload_cast<int, int>(&canvas::resize);

static_assert(std::is_same_v<decltype(resize_one), void (canvas::* const)(int)>);
static_assert(std::is_same_v<decltype(resize_two), void (canvas::* const)(int, int) noexcept>);
static_assert(std::is_same_v<decltype(scl::overload_cast<>(&canvas::width)), int (canvas::*)() const &>);
```

Overloads of a function. The specifier `noexcept` of an overload stays in the type of the pointer:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp function -->
```cpp
auto const twice_int = scl::overload_cast<int>(&twice);
[[maybe_unused]]
auto const twice_double = scl::overload_cast<double>(&twice);

static_assert(std::is_same_v<decltype(twice_int), int (* const)(int)>);
static_assert(std::is_same_v<decltype(twice_double), double (* const)(double) noexcept>);
```

Overloads of a member function that differ only in their qualifiers, chosen by the object type:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp qualified -->
```cpp
auto const count_mutable = scl::member_overload_cast<counter &>(&counter::count);
auto const count_const = scl::member_overload_cast<counter const &>(&counter::count);

static_assert(std::is_same_v<decltype(count_mutable), int & (counter::* const)() &>);
static_assert(std::is_same_v<decltype(count_const), int (counter::* const)() const &>);
```

A member function with no ref-qualifier, chosen only for an object type with no reference:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp no_ref_qualifier -->
```cpp
template <typename Type>
using level_cast = decltype(scl::member_overload_cast<Type>(&gauge::level));

[[maybe_unused]]
auto const level_const = scl::member_overload_cast<gauge const>(&gauge::level);

static_assert(std::is_same_v<decltype(level_const), int (gauge::* const)() const>);
static_assert(!scl::is_detected_v<level_cast, gauge const &>);
```

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp call -->
```cpp
    canvas board;
    (board.*resize_two)(3, 4);
    counter clicks;
    (clicks.*count_mutable)() = 7;
    int const count = (clicks.*count_const)();
    std::cout << board.width() << ' ' << twice_int(21) << ' ' << count << '\n'; // 12 42 7
```

## Notes

- The top-level qualifiers `const` and `volatile` of a parameter type are not part of a function
  type, so `void f(int)` and `void f(int const)` are declarations of the same function. For this
  reason the call `overload_cast<int const>(&f)` chooses the overload `f(int)`.

## See also

- [Finding the signature of a callable type](signature.md) - the alias template `scl::signature_t`,
  which applies to the type of the pointer returned by the function object `overload_cast`
- [`example/type_traits/overload_cast/type_traits_overload_cast_example.cpp`](../../../../example/type_traits/overload_cast/type_traits_overload_cast_example.cpp)

---

Previous: [Building the member pointer type from the object type](member_like.md) |
Next: [Finding the signature of a callable type](signature.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
