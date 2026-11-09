# Concepts of cv-qualifiers

Applying the cv-qualifier concepts determines whether the checked type carries the qualifier
`const`, the qualifier `volatile` or both at once. A qualifier of the type a pointer points to or
a reference refers to does not count.

- Header: `#include <scl/utility/concepts/qualifier.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept const_qualified = ::std::is_const_v<T>;

    template <typename T>
    concept volatile_qualified = ::std::is_volatile_v<T>;

    template <typename T>
    concept cv_qualified = ::scl::concepts::const_qualified<T> && ::scl::concepts::volatile_qualified<T>;
}
```

## Semantics

### `const_qualified<T>`

- **Condition.** The checked type satisfies the concept if it carries the qualifier `const`, such as
  `int const` or `int * const`.
- **Array.** An array of `const` elements, such as `int const[3]`, is also a `const` type.
- **Pointer.** The type `int const *` does not satisfy the concept, since the qualifier belongs to
  the type the pointer points to, not to the pointer itself.
- **Reference.** The type `int const &` does not satisfy the concept, since a reference cannot carry
  qualifiers of its own.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_const_v<T>`.
- **Choice of an overload.** The concept is declared through no other concept, so its constraint is
  not stricter than the constraint of any other concept of the group.

### `volatile_qualified<T>`

- **Condition.** The checked type satisfies the concept if it carries the qualifier `volatile`.
- **Array, pointer and reference.** The qualifier counts by the same rules as the qualifier `const`,
  so the types `int volatile` and `int volatile[3]` satisfy the concept, while the types
  `int volatile *` and `int volatile &` do not.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_volatile_v<T>`.
- **Choice of an overload.** The concept is declared through no other concept, so its constraint is
  not stricter than the constraint of any other concept of the group.

### `cv_qualified<T>`

- **Condition.** The checked type satisfies the concept if it carries both qualifiers at once, such
  as `int const volatile`. A type with only one of them does not satisfy the concept.
- **Standard-library trait.** The standard library has no trait for this check.
- **Choice of an overload.** The concept is declared through the concepts `const_qualified` and
  `volatile_qualified`, so its constraint is stricter than the constraints of those concepts. For
  a type with both qualifiers the compiler chooses a function template constrained by the concept
  `cv_qualified` over one constrained by the concept `const_qualified` or `volatile_qualified`. If
  no template constrained by
  the concept `cv_qualified` is declared, and both a template constrained by the concept
  `const_qualified` and a template constrained by the concept `volatile_qualified` accept such
  a type, the compiler rejects the call as ambiguous.

## Examples

Checking the qualifier `const`:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp const_qualified -->
```cpp
static_assert(scl::concepts::const_qualified<int const>);
static_assert(scl::concepts::const_qualified<int const volatile>);
static_assert(scl::concepts::const_qualified<int * const>);
static_assert(scl::concepts::const_qualified<int const[3]>);
static_assert(!scl::concepts::const_qualified<int>);
static_assert(!scl::concepts::const_qualified<int const *>);
static_assert(!scl::concepts::const_qualified<int const &>);
```

Checking the qualifier `volatile`:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp volatile_qualified -->
```cpp
static_assert(scl::concepts::volatile_qualified<int volatile>);
static_assert(scl::concepts::volatile_qualified<int const volatile>);
static_assert(scl::concepts::volatile_qualified<int volatile[3]>);
static_assert(!scl::concepts::volatile_qualified<int>);
static_assert(!scl::concepts::volatile_qualified<int volatile *>);
static_assert(!scl::concepts::volatile_qualified<int volatile &>);
```

Checking both qualifiers at once:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp cv_qualified -->
```cpp
static_assert(scl::concepts::cv_qualified<int const volatile>);
static_assert(!scl::concepts::cv_qualified<int const>);
static_assert(!scl::concepts::cv_qualified<int volatile>);
static_assert(!scl::concepts::cv_qualified<int const volatile &>);
```

Choosing an overload by the qualifiers of an object. For an object with both qualifiers the overload
constrained by the concept `cv_qualified` is called:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp qualifiers -->
```cpp
template <scl::concepts::const_qualified Value>
std::string_view qualifiers(Value &)
{
    return "const";
}

template <scl::concepts::cv_qualified Value>
std::string_view qualifiers(Value &)
{
    return "const volatile";
}
```

Forbidding a call for a constant object. The function `reset` writes to its argument, so its
template parameter `Value` is constrained by the negation of the concept `const_qualified`. For
a constant object the type `Value` is deduced as `int const`, and the call does not compile, since
this type does not satisfy the constraint:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp reset -->
```cpp
template <typename Value>
void reset(Value & value)
    requires(!scl::concepts::const_qualified<Value>)
{
    value = Value{};
}

template <typename Value>
concept resettable = requires(Value & value) { reset(value); };

static_assert(resettable<int>);
static_assert(!resettable<int const>);
```

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp use -->
```cpp
    int counter = 5;
    reset(counter);
    std::cout << counter << '\n'; // 0

    int const fixed = 1;
    int const volatile shared = 2;
    std::cout << qualifiers(fixed) << '\n';  // const
    std::cout << qualifiers(shared) << '\n'; // const volatile
```

## See also

- [`example/concepts/qualifier/concepts_qualifier_example.cpp`](../../../../example/concepts/qualifier/concepts_qualifier_example.cpp)

---

Next: [Concepts of reference types](reference.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
