# Concepts of type categories

Applying the concepts of type categories determines to which category the checked type belongs.
The standard divides all types into the primary categories `void`, `std::nullptr_t`, integral types,
floating-point types, arrays, pointers, lvalue references, rvalue references, pointers to data
members, pointers to member functions, enumerations, unions, classes and functions. Composite
categories, such as arithmetic types, scalar types and object types, join several primary
categories. The concepts of reference types are described on the page
[Concepts of reference types](reference.md).

- Header: `#include <scl/utility/concepts/type_category.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept enum_type = ::std::is_enum_v<T>;

    template <auto V>
    concept enum_value = ::std::is_enum_v<decltype(V)>;

#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    template <typename T>
    concept scoped_enum = ::scl::concepts::enum_type<T> && ::std::is_scoped_enum_v<T>;
#else
    template <typename T>
    concept scoped_enum = ::scl::concepts::enum_type<T> &&
        !::std::is_convertible_v<T, ::std::underlying_type_t<T>>;
#endif

    template <typename T>
    concept void_type = ::std::is_void_v<T>;

    template <typename T>
    concept pointer = ::std::is_pointer_v<T>;

    template <typename T>
    concept null_pointer = ::std::is_null_pointer_v<T>;

    template <typename T>
    concept member_pointer = ::std::is_member_pointer_v<T>;

    template <typename T>
    concept member_object_pointer = ::std::is_member_object_pointer_v<T>;

    template <typename T>
    concept member_function_pointer = ::std::is_member_function_pointer_v<T>;

    template <typename T>
    concept array = ::std::is_array_v<T>;

    template <typename T>
    concept bounded_array = ::std::is_bounded_array_v<T>;

    template <typename T>
    concept unbounded_array = ::std::is_unbounded_array_v<T>;

    template <typename T>
    concept function = ::std::is_function_v<T>;

    template <typename T>
    concept class_type = ::std::is_class_v<T>;

    template <typename T>
    concept union_type = ::std::is_union_v<T>;

    template <typename T>
    concept fundamental = ::std::is_fundamental_v<T>;

    template <typename T>
    concept arithmetic = ::std::is_arithmetic_v<T>;

    template <typename T>
    concept scalar = ::std::is_scalar_v<T>;

    template <typename T>
    concept object = ::std::is_object_v<T>;

    template <typename T>
    concept compound = ::std::is_compound_v<T>;
}
```

## Semantics

### General rules

- **Cv-qualifiers.** For a type with cv-qualifiers each concept but `enum_value` gives the same
  result as for the type without them.
- **References.** A reference to a type of any category itself belongs to the category of
  references, so a reference type satisfies no concept but `compound`.
- **Standard-library trait.** The result of the check of each concept other than `non_reference` and
  `enum_value` equals the value of the trait of the standard library whose name is the prefix `is_`
  followed by the name of the concept without the suffix `_type`. For example, the result of
  the concept `pointer` equals the value of the trait `std::is_pointer_v<T>`, and the result of
  the concept `enum_type` equals the value of the trait `std::is_enum_v<T>`.

### Enumerations

- **`enum_type<T>`.** The checked type satisfies the concept if it is an enumeration, scoped or
  unscoped.
- **`scoped_enum<T>`.** The checked type satisfies the concept if it is a scoped enumeration, that
  is, one declared with `enum class` or `enum struct`. If the standard library provides the trait
  `std::is_scoped_enum` of C++23 (the macro `__cpp_lib_is_scoped_enum` is at least `202011L`),
  the concept is built on that trait. Otherwise the concept checks that the enumeration does not
  convert implicitly to its underlying type. Both definitions give the same result.

### Constant of an enumeration

- **Condition.** The concept `enum_value<V>` checks a constant, not a type. A constant satisfies
  the concept if its type is an enumeration.
- **Template parameter.** A template parameter declared as `scl::concepts::enum_type auto V` accepts
  the same constants. The concept `enum_value` is meant for a `requires` clause and for any other
  place where a constant, rather than its type, is at hand.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_enum_v<decltype(V)>`.
- **Choice of an overload.** The concept is declared through no other concept, so its constraint is
  not stricter than the constraint of any other concept of the group.
- **Limit of the module.** The module does not support overloads that differ only in the constraint
  on the type of a constant template parameter, such as `scl::concepts::enum_type auto V` and
  `scl::concepts::scoped_enum auto V`, since for a scoped enumeration Clang and GCC 16 choose
  the overload constrained by the concept `scoped_enum`, while earlier versions of GCC and MSVC
  reject the call as ambiguous.

### `void` and pointers

- **`void_type<T>`.** The checked type satisfies the concept if it is the type `void`.
- **`pointer<T>`.** The checked type satisfies the concept if it is a pointer to an object, to
  `void` or to a function. A pointer to a member and the type `std::nullptr_t` do not satisfy
  the concept.
- **`null_pointer<T>`.** The checked type satisfies the concept if it is the type `std::nullptr_t`,
  that is, the type of the literal `nullptr`.
- **`member_pointer<T>`.** The checked type satisfies the concept if it is a pointer to a data
  member or a pointer to a member function.
- **`member_object_pointer<T>`.** The checked type satisfies the concept if it is a pointer to
  a data member.
- **`member_function_pointer<T>`.** The checked type satisfies the concept if it is a pointer to
  a member function, whatever the qualifiers of the member function.

### Arrays and functions

- **`array<T>`.** The checked type satisfies the concept if it is an array with a known bound, such
  as `int[3]`, or with an unknown bound, such as `int[]`. A pointer to an element does not satisfy
  the concept, since it is not an array.
- **`bounded_array<T>`.** The checked type satisfies the concept if it is an array with a known
  bound. For example, the array `int[2][3]` is an array with a known bound whose elements are such
  arrays too.
- **`unbounded_array<T>`.** The checked type satisfies the concept if it is an array with an unknown
  bound, the type `int[][3]` included.
- **`function<T>`.** The checked type satisfies the concept if it is a function type, such as
  `void(int)`, with cv-qualifiers, a reference qualifier or the specifier `noexcept` included.
  A pointer to a function and a reference to a function do not satisfy the concept.

### Classes and unions

- **`class_type<T>`.** The checked type satisfies the concept if it is a class other than a union.
  The closure type of a lambda expression is a class too. A union and an enumeration do not satisfy
  the concept.
- **`union_type<T>`.** The checked type satisfies the concept if it is a union.

### Composite categories

- **`fundamental<T>`.** The checked type satisfies the concept if it is an arithmetic type, the type
  `void` or the type `std::nullptr_t`.
- **`arithmetic<T>`.** The checked type satisfies the concept if it is an integral type or
  a floating-point type, `bool` and the character types included. An enumeration does not satisfy
  the concept, since it is not an arithmetic type.
- **`scalar<T>`.** The checked type satisfies the concept if it is an arithmetic type,
  an enumeration, a pointer, a pointer to a member or the type `std::nullptr_t`.
- **`object<T>`.** The checked type satisfies the concept if it is not a function type, a reference
  type or the type `void`. An array is an object type too.
- **`compound<T>`.** The checked type satisfies the concept if it is not fundamental, that is, if it
  is a pointer, a pointer to a member, a reference, an array, a function, a class, a union or
  an enumeration.

### Choice of an overload

- **Rule.** Each concept of a category but `scoped_enum` is declared through its trait alone, so its
  constraint is not stricter than the constraint of any other concept of the group. The concept
  `scoped_enum` is declared through the concept `enum_type`, so its constraint is stricter than
  the constraint of `enum_type`, and for a scoped enumeration the compiler chooses a function
  template constrained by the concept `scoped_enum` over one constrained by the concept `enum_type`.
- **Ambiguous choice.** If a type satisfies two concepts and neither constraint is stricter than
  the other, the choice between function templates constrained by those concepts is ambiguous. For
  example, for an array with a known bound the choice between templates constrained by the concept
  `array` and by the concept `bounded_array` is ambiguous.

## Examples

The types used in the examples:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp types -->
```cpp
enum plain_color
{
    red,
    green
};

enum class direction : unsigned char
{
    north,
    south
};

struct point
{
    int x;
    int y;
};

union number
{
    int integer;
    float real;
};
```

### Enumerations

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp enum_type -->
```cpp
static_assert(scl::concepts::enum_type<plain_color>);
static_assert(scl::concepts::enum_type<direction const>);
static_assert(!scl::concepts::enum_type<int>);
static_assert(!scl::concepts::enum_type<direction &>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp enum_value -->
```cpp
static_assert(scl::concepts::enum_value<direction::north>);
static_assert(scl::concepts::enum_value<red>);
static_assert(!scl::concepts::enum_value<42>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp scoped_enum -->
```cpp
static_assert(scl::concepts::scoped_enum<direction>);
static_assert(scl::concepts::scoped_enum<direction const>);
static_assert(!scl::concepts::scoped_enum<plain_color>);
static_assert(!scl::concepts::scoped_enum<int>);
```

Getting the underlying value of an enumeration. The function `to_underlying` gets it from
the argument, and the variable template `underlying_of` gets it from the template parameter:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp underlying -->
```cpp
template <scl::concepts::enum_type Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum value) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(value);
}

template <auto Value>
    requires scl::concepts::enum_value<Value>
constexpr auto underlying_of = to_underlying(Value);

static_assert(underlying_of<direction::south> == 1);
```

Choosing an overload by the kind of an enumeration. For a scoped enumeration the compiler chooses
the overload constrained by the concept `scoped_enum`:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp describe -->
```cpp
template <scl::concepts::enum_type Enum>
std::string_view describe(Enum)
{
    return "enumeration";
}

template <scl::concepts::scoped_enum Enum>
std::string_view describe(Enum)
{
    return "scoped enumeration";
}
```

The calls of the function `describe`:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp use -->
```cpp
    std::cout << describe(green) << '\n';            // enumeration
    std::cout << describe(direction::north) << '\n'; // scoped enumeration
```

### `void` and pointers

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp void_type -->
```cpp
static_assert(scl::concepts::void_type<void>);
static_assert(scl::concepts::void_type<void const>);
static_assert(!scl::concepts::void_type<void *>);
static_assert(!scl::concepts::void_type<int>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp pointer -->
```cpp
static_assert(scl::concepts::pointer<int *>);
static_assert(scl::concepts::pointer<int const * const>);
static_assert(scl::concepts::pointer<void *>);
static_assert(scl::concepts::pointer<void (*)(int)>);
static_assert(!scl::concepts::pointer<std::nullptr_t>);
static_assert(!scl::concepts::pointer<int point::*>);
static_assert(!scl::concepts::pointer<int *&>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp null_pointer -->
```cpp
static_assert(scl::concepts::null_pointer<std::nullptr_t>);
static_assert(scl::concepts::null_pointer<std::nullptr_t const>);
static_assert(!scl::concepts::null_pointer<void *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp member_pointer -->
```cpp
static_assert(scl::concepts::member_pointer<int point::*>);
static_assert(scl::concepts::member_pointer<int (point::*)() const noexcept>);
static_assert(!scl::concepts::member_pointer<int *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp member_object_pointer -->
```cpp
static_assert(scl::concepts::member_object_pointer<int point::*>);
static_assert(!scl::concepts::member_object_pointer<int (point::*)() const noexcept>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp member_function_pointer -->
```cpp
static_assert(scl::concepts::member_function_pointer<int (point::*)() const noexcept>);
static_assert(!scl::concepts::member_function_pointer<int point::*>);
static_assert(!scl::concepts::member_function_pointer<void (*)()>);
```

### Arrays and functions

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp array -->
```cpp
static_assert(scl::concepts::array<int[3]>);
static_assert(scl::concepts::array<int[]>);
static_assert(!scl::concepts::array<int *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp bounded_array -->
```cpp
static_assert(scl::concepts::bounded_array<int[3]>);
static_assert(scl::concepts::bounded_array<int[2][3]>);
static_assert(!scl::concepts::bounded_array<int[]>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp unbounded_array -->
```cpp
static_assert(scl::concepts::unbounded_array<int[]>);
static_assert(scl::concepts::unbounded_array<int[][3]>);
static_assert(!scl::concepts::unbounded_array<int[3]>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp function -->
```cpp
static_assert(scl::concepts::function<void(int)>);
static_assert(scl::concepts::function<int(int, double) const noexcept>);
static_assert(!scl::concepts::function<void (*)(int)>);
static_assert(!scl::concepts::function<void (&)(int)>);
```

### Classes and unions

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp class_type -->
```cpp
static_assert(scl::concepts::class_type<point>);
static_assert(!scl::concepts::class_type<number>);
static_assert(!scl::concepts::class_type<plain_color>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp union_type -->
```cpp
static_assert(scl::concepts::union_type<number>);
static_assert(!scl::concepts::union_type<point>);
```

### Composite categories

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp fundamental -->
```cpp
static_assert(scl::concepts::fundamental<int>);
static_assert(scl::concepts::fundamental<void>);
static_assert(scl::concepts::fundamental<std::nullptr_t>);
static_assert(!scl::concepts::fundamental<int *>);
static_assert(!scl::concepts::fundamental<plain_color>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp arithmetic -->
```cpp
static_assert(scl::concepts::arithmetic<int>);
static_assert(scl::concepts::arithmetic<bool>);
static_assert(scl::concepts::arithmetic<double const>);
static_assert(!scl::concepts::arithmetic<plain_color>);
static_assert(!scl::concepts::arithmetic<int *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp scalar -->
```cpp
static_assert(scl::concepts::scalar<int>);
static_assert(scl::concepts::scalar<plain_color>);
static_assert(scl::concepts::scalar<int *>);
static_assert(scl::concepts::scalar<int point::*>);
static_assert(scl::concepts::scalar<std::nullptr_t>);
static_assert(!scl::concepts::scalar<point>);
static_assert(!scl::concepts::scalar<int[3]>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp object -->
```cpp
static_assert(scl::concepts::object<int>);
static_assert(scl::concepts::object<int[3]>);
static_assert(scl::concepts::object<point>);
static_assert(!scl::concepts::object<void>);
static_assert(!scl::concepts::object<int &>);
static_assert(!scl::concepts::object<void(int)>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp compound -->
```cpp
static_assert(scl::concepts::compound<int *>);
static_assert(scl::concepts::compound<int &>);
static_assert(scl::concepts::compound<point>);
static_assert(scl::concepts::compound<plain_color>);
static_assert(scl::concepts::compound<void(int)>);
static_assert(!scl::concepts::compound<int>);
static_assert(!scl::concepts::compound<void>);
```

## See also

- [`example/concepts/type_category/concepts_type_category_example.cpp`](../../../../example/concepts/type_category/concepts_type_category_example.cpp)

---

Previous: [Concepts of reference types](reference.md) |
Next: [Concepts of type properties](type_property.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
