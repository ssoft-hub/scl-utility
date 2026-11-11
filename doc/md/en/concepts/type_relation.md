# Concepts of type relations

Applying the concepts of type relations determines whether one class is a base class of another
class, a virtual base class included.

- Header: `#include <scl/utility/concepts/type_relation.h>`

```cpp
#define SCL_HAS_VIRTUAL_BASE_OF_CONCEPT /* 1 or 0 */
```

```cpp
namespace scl::concepts
{
    template <typename Base, typename Derived>
    concept base_of = ::scl::concepts::class_type<Base> && ::scl::concepts::class_type<Derived> &&
        ::std::is_base_of_v<Base, Derived>;

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
#if defined(__cpp_lib_is_virtual_base_of) && __cpp_lib_is_virtual_base_of >= 202406L
    template <typename Base, typename Derived>
    concept virtual_base_of = ::scl::concepts::base_of<Base, Derived> &&
        ::std::is_virtual_base_of_v<Base, Derived>;
#else
    template <typename Base, typename Derived>
    concept virtual_base_of = ::scl::concepts::base_of<Base, Derived> &&
        __builtin_is_virtual_base_of(Base, Derived);
#endif
#endif
}
```

## Semantics

### General rules

- **Cv-qualifiers and references.** The cv-qualifiers of both types do not count. If at least one of
  the types is a reference, the pair satisfies neither concept.
- **Template parameter.** A concept can be given in the declaration of a template parameter, such as
  `template <scl::concepts::base_of<Derived> Base>`. The template parameter is then checked as
  the type `Base`.
- **Complete type.** If both types are classes other than unions and differ, the type `Derived` must
  be complete, as the traits `std::is_base_of` and `std::is_virtual_base_of` require. A caller must
  not check with the concepts a type that breaks this requirement.

### `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT`

- **Value.** The macro tells whether the concept `virtual_base_of` is declared. The macro expands to
  `1` if the standard library provides the trait `std::is_virtual_base_of` (the macro
  `__cpp_lib_is_virtual_base_of` is at least `202406L`) or the compiler provides the built-in
  function `__builtin_is_virtual_base_of`. Otherwise the macro expands to `0`, and the concept
  `virtual_base_of` is not declared.
- **Testing the macro.** The macro is always defined, so a caller should test it with `#if`, not
  with `#ifdef`. Under `#if` a misspelled name of the macro reads as `0` and draws the warning
  `-Wundef` where that option of GCC or Clang is given, while under `#ifdef` it reads as false with
  no warning.

### `base_of<Base, Derived>`

- **Condition.** The pair of checked types satisfies the concept if both types are classes other
  than unions and the class `Base` is the class `Derived` or a base class of it, direct or indirect.
- **Access and repetition.** The access to the base class does not count, and the class `Base` may
  occur in the class `Derived` as a base class more than once. This is where the concept differs
  from the concept `std::derived_from`, which requires a public and unambiguous base class.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_base_of_v<Base, Derived>`.

### `virtual_base_of<Base, Derived>`

- **Declaration.** The concept is declared only if the macro `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT`
  expands to `1`.
- **Condition.** The pair of checked types satisfies the concept if the class `Base` is a virtual
  base class of the class `Derived`, direct or indirect. A class is not a virtual base class of
  itself.
- **Access and repetition.** The access to the base class does not count. The class `Derived` may
  hold the class `Base` both as a virtual and as a non-virtual base class.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_virtual_base_of_v<Base, Derived>` of C++26.

### Choice of an overload

- **Rule.** The concept `base_of` is declared through the concept [`class_type`](type_category.md)
  of each of the two types, and the concept `virtual_base_of` is declared through the concept
  `base_of`. So the constraint of each of them is stricter than the constraints of the concepts
  `class_type`, `object`, `compound` and `non_reference` of each type.
- **Choice of a template.** The compiler chooses a function template constrained by the concept
  `virtual_base_of` over one constrained by the concept `base_of`, and a function template
  constrained by the concept `base_of` over one constrained by the concept `class_type`.

## Examples

The types used in the examples:

<!-- snippet: example/concepts/type_relation/concepts_type_relation_example.cpp types -->
```cpp
struct node
{};

struct left_branch : virtual node
{};

struct right_branch : virtual node
{};

struct joined
    : left_branch
    , right_branch
{};

struct plain : node
{};

struct hidden : private node
{};
```

Checking a base class:

<!-- snippet: example/concepts/type_relation/concepts_type_relation_example.cpp base_of -->
```cpp
static_assert(scl::concepts::base_of<node, plain>);
static_assert(scl::concepts::base_of<node, hidden>);
static_assert(scl::concepts::base_of<node, joined>);
static_assert(scl::concepts::base_of<node, node>);
static_assert(!scl::concepts::base_of<plain, node>);
static_assert(!scl::concepts::base_of<int, int>);
```

Checking a virtual base class, if the macro `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT` expands to `1`:

<!-- snippet: example/concepts/type_relation/concepts_type_relation_example.cpp virtual_base_of -->
```cpp
static_assert(scl::concepts::virtual_base_of<node, left_branch>);
static_assert(scl::concepts::virtual_base_of<node, joined>);
static_assert(!scl::concepts::virtual_base_of<node, plain>);
static_assert(!scl::concepts::virtual_base_of<node, node>);
static_assert(!scl::concepts::virtual_base_of<left_branch, node>);
```

## See also

- [`example/concepts/type_relation/concepts_type_relation_example.cpp`](../../../../example/concepts/type_relation/concepts_type_relation_example.cpp)

---

Previous: [Concepts of type properties](type_property.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
