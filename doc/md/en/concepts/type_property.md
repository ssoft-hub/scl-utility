# Concepts of type properties

Applying the concepts of type properties determines whether the checked type can be copied as bytes,
whether it has standard layout, whether it is an aggregate, whether its objects can be created
implicitly, for a class or a union also whether it is final, and for a class also whether it is
empty, polymorphic or abstract. The standard defines these properties apart from the type
categories.

- Header: `#include <scl/utility/concepts/type_category.h>`

```cpp
#define SCL_HAS_IMPLICIT_LIFETIME_CONCEPT /* 1 or 0 */
```

```cpp
namespace scl::concepts
{
    template <typename T>
    concept trivially_copyable = ::scl::concepts::object<T> && ::std::is_trivially_copyable_v<T>;

    template <typename T>
    concept standard_layout = ::scl::concepts::object<T> && ::std::is_standard_layout_v<T>;

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    template <typename T>
    concept implicit_lifetime = ::scl::concepts::object<T> && ::std::is_implicit_lifetime_v<T>;
#else
    template <typename T>
    concept implicit_lifetime = ::scl::concepts::object<T> && __builtin_is_implicit_lifetime(T);
#endif
#endif

    template <typename T>
    concept aggregate = ::scl::concepts::object<T> && ::scl::concepts::compound<T> &&
        ::std::is_aggregate_v<T>;

    template <typename T>
    concept empty = ::scl::concepts::class_type<T> && ::std::is_empty_v<T>;

    template <typename T>
    concept polymorphic = ::scl::concepts::class_type<T> && ::std::is_polymorphic_v<T>;

    template <typename T>
    concept abstract = ::scl::concepts::polymorphic<T> && ::std::is_abstract_v<T>;

    template <typename T>
    concept final = (::scl::concepts::class_type<T> || ::scl::concepts::union_type<T>) &&
        ::std::is_final_v<T>;
}
```

## Semantics

### General rules

- **Cv-qualifiers.** For a type with cv-qualifiers each concept gives the same result as for
  the type without them.
- **References.** A reference type satisfies no concept of the page.
- **Complete type.** The checked type must meet the requirements that the standard-library trait
  underlying the concept places on its argument. The subsection of each concept states its
  requirement. A caller must not check with the concepts a type that breaks these requirements.

### `SCL_HAS_IMPLICIT_LIFETIME_CONCEPT`

- **Value.** The macro tells whether the concept `implicit_lifetime` is declared. The macro expands
  to `1` if the standard library provides the trait `std::is_implicit_lifetime` (the macro
  `__cpp_lib_is_implicit_lifetime` is at least `202302L`) or the compiler provides the built-in
  function `__builtin_is_implicit_lifetime`. Otherwise the macro expands to `0`, and the concept
  `implicit_lifetime` is not declared.
- **Testing the macro.** The macro is always defined, so a caller should test it with `#if`, not
  with `#ifdef`. Under `#if` a misspelled name of the macro reads as `0` and draws the warning
  `-Wundef` where that option of GCC or Clang is given, while under `#ifdef` it reads as false with
  no warning.

### `trivially_copyable<T>`

- **Condition.** The checked type satisfies the concept if its objects can be copied as bytes, that
  is, if the type is a scalar type, a trivially copyable class or an array of such types.
- **Copy.** The object representation of such an object can be copied with `std::memcpy` into
  another object of the same type, and after the copy the second object holds the same value. Each
  of the two objects must be neither a base class subobject nor a member declared with the attribute
  `[[no_unique_address]]`, since such a subobject can occupy bytes of another member of
  the enclosing object, and the copy overwrites that member.
- **Complete type.** The type `std::remove_all_extents_t<T>` must be a complete type or `void`.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_trivially_copyable_v<T>`.

### `standard_layout<T>`

- **Condition.** The checked type satisfies the concept if it has standard layout, that is, if it is
  a scalar type, a standard-layout class or an array of such types. A class with a virtual function
  or a virtual base class has no standard layout.
- **Complete type.** The type `std::remove_all_extents_t<T>` must be a complete type or `void`.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_standard_layout_v<T>`.

### `empty<T>`

- **Condition.** The checked type satisfies the concept if it is an empty class, that is, a class
  other than a union that has no non-static data member other than a subobject of zero size, no
  virtual function, no virtual base class and no base class that is not empty.
- **The attribute `[[no_unique_address]]`.** An empty member with this attribute is a subobject of
  zero size only under the Itanium ABI, which GCC and Clang follow on every platform other than
  the MSVC one. Under the MSVC ABI, which MSVC follows and Clang follows when compiling for the MSVC
  platform, the attribute has no effect, and a class with such a member is not empty.
- **The macro `SCL_NO_UNIQUE_ADDRESS`.** A class with an empty member declared with the macro
  [`SCL_NO_UNIQUE_ADDRESS`](../attribute/no_unique_address.md) is empty under both ABIs if the macro
  expands to the attribute `[[no_unique_address]]` under the Itanium ABI and to the attribute
  `[[msvc::no_unique_address]]` under the MSVC ABI.
- **Complete type.** A class that is not a union must be complete.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_empty_v<T>`.

### `polymorphic<T>`

- **Condition.** The checked type satisfies the concept if it is a class that declares or inherits
  a virtual function.
- **Complete type.** A class that is not a union must be complete.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_polymorphic_v<T>`.

### `abstract<T>`

- **Condition.** The checked type satisfies the concept if it is a class with at least one pure
  virtual function, declared in the class itself or inherited and not overridden in it. An object of
  such a class exists only as a subobject of a derived class.
- **Complete type.** A class that is not a union must be complete.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_abstract_v<T>`.

### `final<T>`

- **Condition.** The checked type satisfies the concept if it is a class or a union declared with
  the specifier `final`. No class can derive from such a type.
- **Complete type.** A class or a union must be complete.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_final_v<T>`.

### `aggregate<T>`

- **Condition.** The checked type satisfies the concept if it is an aggregate, that is, an array or
  a class or a union with no user-declared or inherited constructor, no virtual function, no private
  or protected direct non-static data member, no virtual base class, direct or indirect, and no
  private or protected direct base class.
- **Complete type.** The type must be an array type, a complete type or `void`.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_aggregate_v<T>`.

### `implicit_lifetime<T>`

- **Declaration.** The concept is declared only if the macro `SCL_HAS_IMPLICIT_LIFETIME_CONCEPT`
  expands to `1`.
- **Condition.** The checked type satisfies the concept if its objects can be created implicitly,
  that is, if the type is a scalar type, an array type or an implicit-lifetime class.
  An implicit-lifetime class is an aggregate whose destructor is not user-provided, or a class with
  a trivial eligible constructor and a trivial destructor that is not deleted.
- **Implicit creation.** An object of such a type is created implicitly in the storage allocated,
  for example, by the function `std::malloc`, or in the storage into which, for example,
  the function `std::memcpy` copies bytes, if creating the object would give the program defined
  behaviour.
- **Example.** The class `record` of the examples holds a member of the type `std::string` and still
  satisfies the concept, since it is an aggregate with no user-provided destructor.
- **Complete type.** The type must be an array type, a complete type or `void`.
- **Standard-library trait.** The result of the check equals the value of the trait
  `std::is_implicit_lifetime_v<T>` of C++23.

### Choice of an overload

- **Rule.** Each concept of a property is declared through its trait and through the concept of
  every wider set of types to which all the types satisfying it belong. These sets are described on
  the page [Concepts of type categories](type_category.md), and the concept `abstract` is also
  declared through the concept `polymorphic`. So the constraint of a concept is stricter than
  the constraints of the concepts it is declared through. If the checked type satisfies two concepts
  and the constraint of one of them is stricter, the compiler chooses the function template
  constrained by the stricter concept.
- **Weaker constraints.** For each concept the concepts with a weaker constraint and the weaker
  constraints are listed:

| Concept | Concepts and constraints that are weaker |
|---|---|
| `trivially_copyable` | `object`, `non_reference` |
| `standard_layout` | `object`, `non_reference` |
| `implicit_lifetime` | `object`, `non_reference` |
| `aggregate` | `object`, `non_reference`, `compound` |
| `empty` | `class_type`, `object`, `non_reference`, `compound` |
| `polymorphic` | `class_type`, `object`, `non_reference`, `compound` |
| `abstract` | `polymorphic`, `class_type`, `object`, `non_reference`, `compound` |
| `final` | the constraint `class_type<T> || union_type<T>`, `object`, `non_reference`, `compound` |

- **Concepts of categories.** If the standard counts every type of a category among the types with
  some property, the constraint of the concept of the category is stricter than the constraint of
  the concept of that property. So the constraint of the concept `scalar` is stricter than
  the constraints of the concepts `trivially_copyable` and `standard_layout`, and for the type `int`
  the compiler chooses a function template constrained by the concept `scalar` over one constrained
  by the concept `trivially_copyable`. The constraint of the concept `array` is stricter than
  the constraint of the concept `aggregate`. If the macro `SCL_HAS_IMPLICIT_LIFETIME_CONCEPT`
  expands to `1`, the constraints of the concepts `scalar` and `array` are also stricter than
  the constraint of the concept `implicit_lifetime`.
- **The concept `final`.** The concept `final` is declared through the disjunction of the concepts
  `class_type` and `union_type`, so its constraint is stricter than that disjunction but not
  stricter than either of those concepts alone. If one template is constrained by the concept
  `final` and the other by the concept `class_type`, the choice between those templates is ambiguous
  for a final class. The same holds for the concept `union_type` and a final union.
- **Ambiguous choice.** The choice between templates is also ambiguous when a type satisfies two
  concepts and neither constraint is stricter than the other, such as the concepts
  `trivially_copyable` and `standard_layout` for the type `int`, the concepts `aggregate` and
  `class_type` for a class that is an aggregate, or the concepts `aggregate` and `union_type` for
  a union that is an aggregate.

## Examples

The types used in the examples:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp types -->
```cpp
struct sample
{
    int id;
    double value;
};

struct tag
{};

struct shape
{
    virtual ~shape() = default;
    [[nodiscard]]
    virtual double area() const = 0;
};

struct square final : shape
{
    double side = 0.0;

    [[nodiscard]]
    double area() const override
    {
        return side * side;
    }
};

struct counter
{
    counter() = default;
    explicit counter(int start) noexcept
        : count{start}
    {}

    int count = 0;
};

struct record
{
    std::string name;
};
```

Checking whether a type can be copied as bytes:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp trivially_copyable -->
```cpp
static_assert(scl::concepts::trivially_copyable<int>);
static_assert(scl::concepts::trivially_copyable<sample>);
static_assert(!scl::concepts::trivially_copyable<std::string>);
static_assert(!scl::concepts::trivially_copyable<square>);
```

Checking standard layout:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp standard_layout -->
```cpp
static_assert(scl::concepts::standard_layout<int>);
static_assert(scl::concepts::standard_layout<sample>);
static_assert(!scl::concepts::standard_layout<square>);
```

Checking an empty class:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp empty -->
```cpp
static_assert(scl::concepts::empty<tag>);
static_assert(!scl::concepts::empty<sample>);
static_assert(!scl::concepts::empty<shape>);
static_assert(!scl::concepts::empty<int>);
```

Checking a polymorphic class:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp polymorphic -->
```cpp
static_assert(scl::concepts::polymorphic<shape>);
static_assert(scl::concepts::polymorphic<square>);
static_assert(!scl::concepts::polymorphic<sample>);
```

Checking an abstract class:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp abstract -->
```cpp
static_assert(scl::concepts::abstract<shape>);
static_assert(!scl::concepts::abstract<square>);
static_assert(!scl::concepts::abstract<int>);
```

Checking a final class:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp final -->
```cpp
static_assert(scl::concepts::final<square>);
static_assert(!scl::concepts::final<shape>);
static_assert(!scl::concepts::final<int>);
```

Checking an aggregate:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp aggregate -->
```cpp
static_assert(scl::concepts::aggregate<sample>);
static_assert(scl::concepts::aggregate<int[3]>);
static_assert(!scl::concepts::aggregate<counter>);
static_assert(!scl::concepts::aggregate<int>);
```

Checking whether objects can be created implicitly, if the macro `SCL_HAS_IMPLICIT_LIFETIME_CONCEPT`
expands to `1`:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp implicit_lifetime -->
```cpp
static_assert(scl::concepts::implicit_lifetime<sample>);
static_assert(scl::concepts::implicit_lifetime<record>);
static_assert(scl::concepts::implicit_lifetime<int[3]>);
static_assert(!scl::concepts::implicit_lifetime<std::string>);
static_assert(!scl::concepts::implicit_lifetime<square>);
```

Copying an object as bytes, allowed only for a type that satisfies the concept `trivially_copyable`:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp copy_bytes -->
```cpp
template <scl::concepts::trivially_copyable Value>
void copy_bytes(Value & target, Value const & source) noexcept
{
    std::memcpy(&target, &source, sizeof(Value));
}
```

Choosing an overload by the properties of a class. The constraint of the concept `abstract` is
stricter than the constraint of the concept `polymorphic`, and the constraints of both are stricter
than the constraint of the concept `class_type`. For each class the compiler chooses the overload
with the strictest constraint the class satisfies:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp describe -->
```cpp
template <scl::concepts::class_type Class>
constexpr std::string_view describe()
{
    return "class";
}

template <scl::concepts::polymorphic Class>
constexpr std::string_view describe()
{
    return "polymorphic class";
}

template <scl::concepts::abstract Class>
constexpr std::string_view describe()
{
    return "abstract class";
}
```

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp use -->
```cpp
    sample const source{7, 2.5};
    sample target{};
    copy_bytes(target, source);
    std::cout << target.id << ' ' << target.value << '\n'; // 7 2.5

    std::cout << describe<sample>() << '\n'; // class
    std::cout << describe<square>() << '\n'; // polymorphic class
    std::cout << describe<shape>() << '\n';  // abstract class
```

## See also

- [`example/concepts/type_property/concepts_type_property_example.cpp`](../../../../example/concepts/type_property/concepts_type_property_example.cpp)

---

Previous: [Concepts of type categories](type_category.md) |
Next: [Concepts of type relations](type_relation.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
