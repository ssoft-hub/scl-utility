# Concepts

Applying the concepts of the group determines, while the program compiles, whether a type has
a property, and constrains a template parameter by that property. Each concept of the group
corresponds to a type trait of the standard library for which the standard library has no concept of
its own. Concepts for traits that have one, such as `std::is_integral` and `std::is_floating_point`,
are not part of the group.

A concept is given in the declaration of a template parameter, such as
`template <scl::concepts::pointer Pointer>` or `scl::concepts::pointer auto`. A trait of
the standard library with the suffix `_v` cannot be given there, since it is a value of the type
`bool` rather than a concept. Such a trait is written in the `requires` clause of the template, for
example `template <typename T> requires std::is_pointer_v<T>`.

Each concept is declared through its trait and through the concepts of the wider sets of types to
which all the types satisfying it belong. For example, the concept `pointer` is declared through
the concepts `scalar` and `compound`. So the constraint of the concept `pointer` is stricter than
the constraints of those concepts, and for a pointer the compiler chooses a function template
constrained by the concept `pointer` over one constrained by the concept `scalar` or `compound`.
Each page of the group lists, for each concept declared through other concepts, the concepts with
a weaker constraint.

The name of a concept is the name of its trait without the prefix `is_`, as the names of
the concepts `std::integral` and `std::floating_point` of the standard library are. For example,
the concept `scl::concepts::pointer` corresponds to the trait `std::is_pointer_v<T>`. If the name
without the prefix is a keyword of the language, the suffix `_type` is added to it, hence the names
`enum_type`, `void_type`, `class_type` and `union_type`. The concepts of the cv-qualifiers are
an exception, since the name of each of them is the designation of the qualifier with the suffix
`_qualified`, such as `const_qualified`, `volatile_qualified` and `cv_qualified`. For three concepts
the result is not the value of one trait for the checked type. The concept `cv_qualified` is
declared through the concepts `const_qualified` and `volatile_qualified`, the concept
`non_reference` is written as the negation of the trait `std::is_reference_v`, and the concept
`enum_value` checks the type of a constant with the trait `std::is_enum_v`.

Every concept of the group is declared in the namespace `scl::concepts`.

- Header: `#include <scl/utility/concepts.h>`

| Header | Page | Declares |
|---|---|---|
| `<scl/utility/concepts/qualifier.h>` | [Concepts of cv-qualifiers](qualifier.md) | `const_qualified`, `volatile_qualified`, `cv_qualified` |
| `<scl/utility/concepts/reference.h>` | [Concepts of reference types](reference.md) | `reference`, `lvalue_reference`, `rvalue_reference` |
| `<scl/utility/concepts/type_category.h>` | [Concepts of type categories](type_category.md) | `non_reference`, `enum_type`, `enum_value`, `scoped_enum`, `void_type`, `pointer`, `null_pointer`, `member_pointer`, `member_object_pointer`, `member_function_pointer`, `array`, `bounded_array`, `unbounded_array`, `function`, `class_type`, `union_type`, `fundamental`, `arithmetic`, `scalar`, `object`, `compound` |
| `<scl/utility/concepts/type_category.h>` | [Concepts of type properties](type_property.md) | `trivially_copyable`, `standard_layout`, `empty`, `polymorphic`, `abstract`, `final`, `aggregate`, `implicit_lifetime`, `SCL_HAS_IMPLICIT_LIFETIME_CONCEPT` |
| `<scl/utility/concepts/type_relation.h>` | [Concepts of type relations](type_relation.md) | `base_of`, `virtual_base_of`, `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT` |

## Documentation of the group

### Concepts

- [Concepts of cv-qualifiers](qualifier.md) - checking the qualifiers `const` and `volatile` of
  the type itself
- [Concepts of reference types](reference.md) - checking whether a type is a reference and of which
  kind
- [Concepts of type categories](type_category.md) - checking the primary or composite category of
  a type
- [Concepts of type properties](type_property.md) - checking the properties of a type the standard
  defines apart from the categories
- [Concepts of type relations](type_relation.md) - checking whether one class is a base class of
  another

### Examples

- [`example/concepts/qualifier`](../../../../example/concepts/qualifier/concepts_qualifier_example.cpp)
- [`example/concepts/reference`](../../../../example/concepts/reference/concepts_reference_example.cpp)
- [`example/concepts/type_category`](../../../../example/concepts/type_category/concepts_type_category_example.cpp)
- [`example/concepts/type_property`](../../../../example/concepts/type_property/concepts_type_property_example.cpp)
- [`example/concepts/type_relation`](../../../../example/concepts/type_relation/concepts_type_relation_example.cpp)

---

Next: [Concepts of cv-qualifiers](qualifier.md) | [Back to the main page](../Main.md)
