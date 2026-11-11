# Concepts of reference types

Applying the concepts of reference types determines whether the checked type is a reference and, if
it is, which kind, an lvalue reference or an rvalue reference.

- Header: `#include <scl/utility/concepts/reference.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept reference = ::scl::concepts::compound<T> && ::std::is_reference_v<T>;

    template <typename T>
    concept lvalue_reference = ::scl::concepts::reference<T> && ::std::is_lvalue_reference_v<T>;

    template <typename T>
    concept rvalue_reference = ::scl::concepts::reference<T> && ::std::is_rvalue_reference_v<T>;
}
```

The concept `non_reference`, the opposite of the concept `reference`, is declared in the header
[`type_category.h`](type_category.md), which the header `reference.h` includes:

```cpp
namespace scl::concepts
{
    template <typename T>
    concept non_reference = !::std::is_reference_v<T>;
}
```

## Semantics

### `reference<T>`

- **Condition.** The checked type satisfies the concept if it is a reference of any kind, an lvalue
  reference or an rvalue reference, whatever the type it refers to.
- **Pointer.** A pointer does not satisfy the concept, since it is not a reference.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_reference_v<T>`.

### `lvalue_reference<T>`

- **Condition.** The checked type satisfies the concept if it is an lvalue reference, such as
  `int &` or `int const &`.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_lvalue_reference_v<T>`.

### `rvalue_reference<T>`

- **Condition.** The checked type satisfies the concept if it is an rvalue reference, such as
  `int &&` or `int const &&`.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_rvalue_reference_v<T>`.

### `non_reference<T>`

- **Condition.** The checked type satisfies the concept if it is not a reference, that is, if it is
  an object type, a function type or the type `void`.
- **Standard-library trait.** The standard library has no trait for this check, so the concept is
  written as the negation of the trait `std::is_reference_v<T>`.

### Forwarding reference

- **Deduction.** For the parameter `Argument &&` of a function template the type `Argument` is
  deduced from the argument. For an lvalue argument an lvalue reference is deduced, and for
  an rvalue argument a type with no reference is deduced.
- **Result.** So such a type `Argument` satisfies the concept `lvalue_reference` only for an lvalue
  argument and does not satisfy the concept `rvalue_reference` for any argument.

### Choice of an overload

- **Rule.** Each concept of reference types is declared through the concept
  [`compound`](type_category.md) or through the concept `reference`, so its constraint is stricter
  than the constraints of those concepts. If the checked type satisfies two concepts and
  the constraint of one of them is stricter, the compiler chooses the function template constrained
  by the stricter concept.
- **Weaker constraints.** For each concept the concepts with a weaker constraint are listed:

| Concept | Concepts with a weaker constraint |
|---|---|
| `reference` | `compound` |
| `lvalue_reference` | `reference`, `compound` |
| `rvalue_reference` | `reference`, `compound` |

- **The concept `non_reference`.** The concept `non_reference` is declared through no other concept,
  so its constraint is not stricter than the constraint of any other concept of the group.
  The concepts `object`, `fundamental`, `function` and `void_type` of the page
  [Concepts of type categories](type_category.md), however, are declared through the concept
  `non_reference`, and their constraints are stricter than the constraint of `non_reference`.

## Examples

Checking a reference of any kind:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp reference -->
```cpp
static_assert(scl::concepts::reference<int &>);
static_assert(scl::concepts::reference<int const &&>);
static_assert(!scl::concepts::reference<int>);
static_assert(!scl::concepts::reference<int *>);
```

Checking an lvalue reference:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp lvalue_reference -->
```cpp
static_assert(scl::concepts::lvalue_reference<int &>);
static_assert(scl::concepts::lvalue_reference<int const &>);
static_assert(!scl::concepts::lvalue_reference<int &&>);
static_assert(!scl::concepts::lvalue_reference<int>);
```

Checking an rvalue reference:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp rvalue_reference -->
```cpp
static_assert(scl::concepts::rvalue_reference<int &&>);
static_assert(scl::concepts::rvalue_reference<int const &&>);
static_assert(!scl::concepts::rvalue_reference<int &>);
static_assert(!scl::concepts::rvalue_reference<int>);
```

Checking a type that is not a reference:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp non_reference -->
```cpp
static_assert(scl::concepts::non_reference<int>);
static_assert(scl::concepts::non_reference<int const>);
static_assert(scl::concepts::non_reference<int *>);
static_assert(!scl::concepts::non_reference<int &>);
static_assert(!scl::concepts::non_reference<int &&>);
```

Checking the value category of an argument passed through a forwarding reference:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp forwarding -->
```cpp
template <typename Argument>
std::string_view category(Argument &&)
{
    if constexpr (scl::concepts::lvalue_reference<Argument>)
        return "lvalue";
    else
        return "rvalue";
}
```

Forbidding a reference type for a member of a class. The parameter of the class template `box` is
constrained by the concept `non_reference`, since a member of a reference type can outlive
the object it refers to:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp box -->
```cpp
template <scl::concepts::non_reference Value>
struct box
{
    Value value;
};
```

Choosing an overload by the kind of a reference. For an lvalue reference the compiler chooses
the overload constrained by the concept `lvalue_reference`, and for an rvalue reference it chooses
the overload constrained by the concept `reference`:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp binding -->
```cpp
template <scl::concepts::reference Type>
constexpr std::string_view binding()
{
    return "reference";
}

template <scl::concepts::lvalue_reference Type>
constexpr std::string_view binding()
{
    return "lvalue reference";
}
```

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp use -->
```cpp
    std::string text{"text"};
    std::cout << category(text) << '\n';                     // lvalue
    std::cout << category(std::string{"temporary"}) << '\n'; // rvalue

    box<std::string> const label{text};
    std::cout << label.value << '\n'; // text

    std::cout << binding<int &>() << '\n';  // lvalue reference
    std::cout << binding<int &&>() << '\n'; // reference
```

## See also

- [`example/concepts/reference/concepts_reference_example.cpp`](../../../../example/concepts/reference/concepts_reference_example.cpp)

---

Previous: [Concepts of cv-qualifiers](qualifier.md) |
Next: [Concepts of type categories](type_category.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
