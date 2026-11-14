# Concepts of type properties

Applying the concepts of type properties determines whether the checked type can be copied as bytes,
whether it has standard layout, whether it is an aggregate, for a class or a union also whether it
is final, and for a class also whether it is empty, polymorphic or abstract. The standard defines
these properties apart from the type categories.

- Header: `#include <scl/utility/concepts/type_property.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept trivially_copyable = ::std::is_trivially_copyable_v<T>;

    template <typename T>
    concept standard_layout = ::std::is_standard_layout_v<T>;

    template <typename T>
    concept empty = ::std::is_empty_v<T>;

    template <typename T>
    concept polymorphic = ::std::is_polymorphic_v<T>;

    template <typename T>
    concept abstract = ::std::is_abstract_v<T>;

    template <typename T>
    concept final = ::std::is_final_v<T>;

    template <typename T>
    concept aggregate = ::std::is_aggregate_v<T>;
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

### Choice of an overload

- **Rule.** Each concept of a property is declared through its trait alone, so its constraint is not
  stricter than the constraint of any other concept of the group.
- **Ambiguous choice.** If a type satisfies two concepts, the choice between function templates
  constrained by those concepts is ambiguous. For example, for an abstract class the choice between
  templates constrained by the concept `abstract` and by the concept `polymorphic` is ambiguous.

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

Copying an object as bytes, allowed only for a type that satisfies the concept `trivially_copyable`:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp copy_bytes -->
```cpp
template <scl::concepts::trivially_copyable Value>
void copy_bytes(Value & target, Value const & source) noexcept
{
    std::memcpy(&target, &source, sizeof(Value));
}
```

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp use -->
```cpp
    sample const source{7, 2.5};
    sample target{};
    copy_bytes(target, source);
    std::cout << target.id << ' ' << target.value << '\n'; // 7 2.5
```

## See also

- [`example/concepts/type_property/concepts_type_property_example.cpp`](../../../../example/concepts/type_property/concepts_type_property_example.cpp)

---

Previous: [Concepts of type categories](type_category.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
