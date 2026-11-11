#pragma once

/**
 * @file
 * @brief Concepts of the categories and the properties of a type.
 * @ingroup scl_utility_concepts
 */

#include <type_traits>

namespace scl::concepts
{

    /**
     * @brief Satisfied by every type that is not a reference type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is not a reference, that is, if it is an object
     * type, a function type or the type `void`. The standard library has no trait for this check,
     * so the concept is written as the negation of the trait `std::is_reference_v<T>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group. The concepts ::scl::concepts::object,
     * ::scl::concepts::fundamental, ::scl::concepts::function and ::scl::concepts::void_type are
     * declared through it, and their constraints are stricter than its constraint.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp non_reference
     * @snippet concepts/reference/concepts_reference_example.cpp box
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept non_reference = !::std::is_reference_v<T>;

    /**
     * @brief Satisfied by every type other than a function type, a reference type and `void`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is not a function type, a reference type or
     * the type `void`. An array is an object type too. The result of the check equals the value of
     * the trait `std::is_object_v<T>`. For a type with cv-qualifiers the concept gives the same
     * result as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::non_reference, so its constraint is stricter
     * than the constraint of ::scl::concepts::non_reference. For an object type the compiler
     * chooses a function template constrained by the concept over one constrained by
     * ::scl::concepts::non_reference.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp object
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept object = ::scl::concepts::non_reference<T> && ::std::is_object_v<T>;

    /**
     * @brief Satisfied by every type that is not a fundamental type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is not fundamental, that is, if it is a pointer,
     * a pointer to a member, a reference, an array, a function, a class, a union or an enumeration.
     * The result of the check equals the value of the trait `std::is_compound_v<T>`. For a type
     * with cv-qualifiers the concept gives the same result as for the type without them.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp compound
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept compound = ::std::is_compound_v<T>;

    /**
     * @brief Satisfied by an arithmetic type, by `void` and by `std::nullptr_t`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an arithmetic type, the type `void` or
     * the type `std::nullptr_t`. The result of the check equals the value of the trait
     * `std::is_fundamental_v<T>`. For a type with cv-qualifiers the concept gives the same result
     * as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::non_reference, so its constraint is stricter
     * than the constraint of ::scl::concepts::non_reference. For a fundamental type the compiler
     * chooses a function template constrained by the concept over one constrained by
     * ::scl::concepts::non_reference.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp fundamental
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept fundamental = ::scl::concepts::non_reference<T> && ::std::is_fundamental_v<T>;

    /**
     * @brief Satisfied by a type whose objects can be copied as bytes.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if its objects can be copied as bytes, that is, if
     * the type is a scalar type, a trivially copyable class or an array of such types. The object
     * representation of such an object can be copied with `std::memcpy` into another object of
     * the same type, and after the copy the second object holds the same value. Each of the two
     * objects must be neither a base class subobject nor a member declared with the attribute
     * `[[no_unique_address]]`, since such a subobject can occupy bytes of another member of
     * the enclosing object, and the copy overwrites that member. The result of the check equals
     * the value of the trait `std::is_trivially_copyable_v<T>`. For a type with cv-qualifiers
     * the concept gives the same result as for the type without them, and a reference type does not
     * satisfy the concept.
     *
     * The type `std::remove_all_extents_t<T>` must be a complete type or `void`, as
     * `std::is_trivially_copyable` requires. The concept is declared through
     * ::scl::concepts::object, so its constraint is stricter than the constraint of that concept
     * and of every concept it is declared through. For a trivially copyable type the compiler
     * chooses a function template constrained by the concept over one constrained by any of them.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp trivially_copyable
     * @snippet concepts/type_property/concepts_type_property_example.cpp copy_bytes
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept trivially_copyable = ::scl::concepts::object<T> && ::std::is_trivially_copyable_v<T>;

    /**
     * @brief Satisfied by a standard-layout type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it has standard layout, that is, if it is a scalar
     * type, a standard-layout class or an array of such types. A class with a virtual function or
     * a virtual base class has no standard layout. The result of the check equals the value of
     * the trait `std::is_standard_layout_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The type `std::remove_all_extents_t<T>` must be a complete type or `void`, as
     * `std::is_standard_layout` requires. The concept is declared through ::scl::concepts::object,
     * so its constraint is stricter than the constraint of that concept and of every concept it is
     * declared through. For a standard-layout type the compiler chooses a function template
     * constrained by the concept over one constrained by any of them.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp standard_layout
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept standard_layout = ::scl::concepts::object<T> && ::std::is_standard_layout_v<T>;

    /**
     * @brief Satisfied by an aggregate, that is, an array or a restricted class or union.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an aggregate, that is, an array or a class or
     * a union with no user-declared or inherited constructor, no virtual function, no private or
     * protected direct non-static data member, no virtual base class, direct or indirect, and no
     * private or protected direct base class. The result of the check equals the value of the trait
     * `std::is_aggregate_v<T>`. For a type with cv-qualifiers the concept gives the same result as
     * for the type without them, and a reference type does not satisfy the concept.
     *
     * The type must be an array type, a complete type or `void`, as `std::is_aggregate` requires.
     * The concept is declared through ::scl::concepts::object and ::scl::concepts::compound, so its
     * constraint is stricter than the constraints of those concepts and of every concept they are
     * declared through. For an aggregate the compiler chooses a function template constrained by
     * the concept over one constrained by any of them. A call is ambiguous between a template
     * constrained by the concept and one constrained by ::scl::concepts::class_type for a class
     * that is an aggregate, and between a template constrained by the concept and one constrained
     * by ::scl::concepts::union_type for a union that is an aggregate.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp aggregate
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept aggregate = ::scl::concepts::object<T> && ::scl::concepts::compound<T> &&
        ::std::is_aggregate_v<T>;

    /**
     * @brief Satisfied by a scalar type, such as an arithmetic type, an enumeration or a pointer.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an arithmetic type, an enumeration,
     * a pointer, a pointer to a member or the type `std::nullptr_t`. The result of the check equals
     * the value of the trait `std::is_scalar_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The concept is declared through ::scl::concepts::object, ::scl::concepts::trivially_copyable
     * and ::scl::concepts::standard_layout, since the standard counts every scalar type among
     * the trivially copyable types and the standard-layout types. So its constraint is stricter
     * than the constraints of
     * those concepts and of ::scl::concepts::non_reference, and for the type `int` the compiler
     * chooses a function template constrained by the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp scalar
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept scalar = ::scl::concepts::object<T> && ::std::is_scalar_v<T> &&
        ::scl::concepts::trivially_copyable<T> && ::scl::concepts::standard_layout<T>;

    /**
     * @brief Satisfied by an integral type and by a floating-point type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an integral type or a floating-point type,
     * `bool` and the character types included. An enumeration does not satisfy the concept, since
     * it is not an arithmetic type. The result of the check equals the value of the trait
     * `std::is_arithmetic_v<T>`. For a type with cv-qualifiers the concept gives the same result as
     * for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::fundamental and ::scl::concepts::scalar, so
     * its constraint is stricter than the constraints of those concepts and of every concept they
     * are declared through. For an arithmetic type the compiler chooses a function template
     * constrained by the concept over one constrained by any of them. The concept `std::integral`
     * of the standard library is declared through its trait alone, so for the type `int` the choice
     * between a template constrained by `std::integral` and a template constrained by the concept
     * is ambiguous.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp arithmetic
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept arithmetic = ::scl::concepts::fundamental<T> && ::scl::concepts::scalar<T> &&
        ::std::is_arithmetic_v<T>;

    /**
     * @brief Satisfied by the type `void` with or without cv-qualifiers.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is the type `void`. The result of the check
     * equals the value of the trait `std::is_void_v<T>`. For a type with cv-qualifiers the concept
     * gives the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The concept is declared through ::scl::concepts::non_reference and
     * ::scl::concepts::fundamental, so its constraint is stricter than the constraints of
     * ::scl::concepts::non_reference and ::scl::concepts::fundamental. For the type `void`
     * the compiler chooses a function template constrained by the concept over one constrained by
     * either of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp void_type
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept void_type = ::scl::concepts::non_reference<T> && ::scl::concepts::fundamental<T> &&
        ::std::is_void_v<T>;

    /**
     * @brief Satisfied by the type `std::nullptr_t` with or without cv-qualifiers.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is the type `std::nullptr_t`, that is, the type
     * of the literal `nullptr`. The result of the check equals the value of the trait
     * `std::is_null_pointer_v<T>`. For a type with cv-qualifiers the concept gives the same result
     * as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::fundamental and ::scl::concepts::scalar, so
     * its constraint is stricter than the constraints of those concepts and of every concept they
     * are declared through. For the type `std::nullptr_t` the compiler chooses a function template
     * constrained by the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp null_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept null_pointer = ::scl::concepts::fundamental<T> && ::scl::concepts::scalar<T> &&
        ::std::is_null_pointer_v<T>;

    /**
     * @brief Satisfied by an enumeration type, scoped or unscoped.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an enumeration, scoped or unscoped.
     * The result of the check equals the value of the trait `std::is_enum_v<T>`. For a type with
     * cv-qualifiers the concept gives the same result as for the type without them, and a reference
     * type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::scalar and ::scl::concepts::compound, so its
     * constraint is stricter than the constraints of those concepts and of every concept they are
     * declared through. For an enumeration the compiler chooses a function template constrained by
     * the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp enum_type
     * @snippet concepts/type_category/concepts_type_category_example.cpp underlying
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept enum_type = ::scl::concepts::scalar<T> && ::scl::concepts::compound<T> && ::std::is_enum_v<T>;

    /**
     * @brief Satisfied by a constant whose type is an enumeration type.
     * @ingroup scl_utility_concepts
     *
     * The concept checks a constant, not a type. A constant satisfies the concept if its type is
     * an enumeration. A template parameter declared as `::scl::concepts::enum_type auto V` accepts
     * the same constants. The concept is meant for a `requires` clause and for any other place
     * where a constant, rather than its type, is at hand. The result of the check equals the value
     * of the trait `std::is_enum_v<decltype(V)>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group. The module does not support overloads that
     * differ only in the constraint on the type of a constant template parameter, such as
     * `::scl::concepts::enum_type auto V` and `::scl::concepts::scoped_enum auto V`, since for
     * a scoped enumeration Clang and GCC 16 choose the overload constrained by
     * ::scl::concepts::scoped_enum, while earlier versions of GCC and MSVC reject the call as
     * ambiguous.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp enum_value
     *
     * @tparam V Constant to check.
     */
    template <auto V>
    concept enum_value = ::std::is_enum_v<decltype(V)>;

    /**
     * @brief Satisfied by a scoped enumeration type, declared with `enum class` or `enum struct`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a scoped enumeration, that is, one declared
     * with `enum class` or `enum struct`. If the standard library provides the trait
     * `std::is_scoped_enum` of C++23 (the macro `__cpp_lib_is_scoped_enum` is at least `202011L`),
     * the concept is built on that trait. Otherwise the concept checks that the enumeration does
     * not convert implicitly to its underlying type. Both definitions give the same result. For
     * a type with cv-qualifiers the concept gives the same result as for the type without them, and
     * a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::enum_type, so its constraint is stricter
     * than the constraint of that concept and of every concept it is declared through. For a scoped
     * enumeration the compiler chooses a function template constrained by the concept over one
     * constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp scoped_enum
     * @snippet concepts/type_category/concepts_type_category_example.cpp describe
     *
     * @tparam T Type to check.
     */
#if (defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L) || defined(SCL_DOXYGEN)
    template <typename T>
    concept scoped_enum = ::scl::concepts::enum_type<T> && ::std::is_scoped_enum_v<T>;
#else
    template <typename T>
    concept scoped_enum = ::scl::concepts::enum_type<T> &&
        !::std::is_convertible_v<T, ::std::underlying_type_t<T>>;
#endif

    /**
     * @brief Satisfied by a pointer to an object, to `void` or to a function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a pointer to an object, to `void` or to
     * a function. A pointer to a member and the type `std::nullptr_t` do not satisfy the concept.
     * The result of the check equals the value of the trait `std::is_pointer_v<T>`. For a type with
     * cv-qualifiers the concept gives the same result as for the type without them, and a reference
     * type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::scalar and ::scl::concepts::compound, so its
     * constraint is stricter than the constraints of those concepts and of every concept they are
     * declared through. For a pointer the compiler chooses a function template constrained by
     * the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept pointer = ::scl::concepts::scalar<T> && ::scl::concepts::compound<T> && ::std::is_pointer_v<T>;

    /**
     * @brief Satisfied by a pointer to a data member and by a pointer to a member function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a pointer to a data member or a pointer to
     * a member function. The result of the check equals the value of the trait
     * `std::is_member_pointer_v<T>`. For a type with cv-qualifiers the concept gives the same
     * result as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::scalar and ::scl::concepts::compound, so its
     * constraint is stricter than the constraints of those concepts and of every concept they are
     * declared through. For a pointer to a member the compiler chooses a function template
     * constrained by the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp member_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept member_pointer = ::scl::concepts::scalar<T> && ::scl::concepts::compound<T> &&
        ::std::is_member_pointer_v<T>;

    /**
     * @brief Satisfied by a pointer to a data member.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a pointer to a data member. The result of
     * the check equals the value of the trait `std::is_member_object_pointer_v<T>`. For a type with
     * cv-qualifiers the concept gives the same result as for the type without them, and a reference
     * type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::member_pointer, so its constraint is
     * stricter than the constraint of that concept and of every concept it is declared through. For
     * a pointer to a data member the compiler chooses a function template constrained by
     * the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp member_object_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept member_object_pointer = ::scl::concepts::member_pointer<T> &&
        ::std::is_member_object_pointer_v<T>;

    /**
     * @brief Satisfied by a pointer to a member function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a pointer to a member function, whatever
     * the qualifiers of the member function. The result of the check equals the value of the trait
     * `std::is_member_function_pointer_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The concept is declared through ::scl::concepts::member_pointer, so its constraint is
     * stricter than the constraint of that concept and of every concept it is declared through. For
     * a pointer to a member function the compiler chooses a function template constrained by
     * the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp member_function_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept member_function_pointer = ::scl::concepts::member_pointer<T> &&
        ::std::is_member_function_pointer_v<T>;

    /**
     * @brief Satisfied by an array type, with a known bound or with an unknown one.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an array with a known bound, such as
     * `int[3]`, or with an unknown bound, such as `int[]`. A pointer to an element does not satisfy
     * the concept, since it is not an array. The result of the check equals the value of the trait
     * `std::is_array_v<T>`. For a type with cv-qualifiers the concept gives the same result as for
     * the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::object, ::scl::concepts::compound and
     * ::scl::concepts::aggregate. So its constraint is stricter than
     * the constraints of those concepts and of ::scl::concepts::non_reference, and for an array
     * the compiler chooses a function template constrained by the concept over one constrained by
     * any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp array
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept array = ::scl::concepts::object<T> && ::scl::concepts::compound<T> &&
        ::std::is_array_v<T> && ::scl::concepts::aggregate<T>;

    /**
     * @brief Satisfied by an array type with a known bound, such as `int[3]`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an array with a known bound. For example,
     * the array `int[2][3]` is an array with a known bound whose elements are such arrays too.
     * The result of the check equals the value of the trait `std::is_bounded_array_v<T>`. For
     * a type with cv-qualifiers the concept gives the same result as for the type without them, and
     * a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::array, so its constraint is stricter than
     * the constraint of that concept and of every concept it is declared through. For an array with
     * a known bound the compiler chooses a function template constrained by the concept over one
     * constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp bounded_array
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept bounded_array = ::scl::concepts::array<T> && ::std::is_bounded_array_v<T>;

    /**
     * @brief Satisfied by an array type with an unknown bound, such as `int[]`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an array with an unknown bound, the type
     * `int[][3]` included. The result of the check equals the value of the trait
     * `std::is_unbounded_array_v<T>`. For a type with cv-qualifiers the concept gives the same
     * result as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::array, so its constraint is stricter than
     * the constraint of that concept and of every concept it is declared through. For an array with
     * an unknown bound the compiler chooses a function template constrained by the concept over one
     * constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp unbounded_array
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept unbounded_array = ::scl::concepts::array<T> && ::std::is_unbounded_array_v<T>;

    /**
     * @brief Satisfied by a function type, such as `void(int)`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a function type, such as `void(int)`, with
     * cv-qualifiers, a reference qualifier or the specifier `noexcept` included. A pointer to
     * a function and a reference to a function do not satisfy the concept. The result of the check
     * equals the value of the trait `std::is_function_v<T>`.
     *
     * The concept is declared through ::scl::concepts::non_reference and ::scl::concepts::compound,
     * so its constraint is stricter than the constraints of ::scl::concepts::non_reference and
     * ::scl::concepts::compound. For a function type the compiler chooses a function template
     * constrained by the concept over one constrained by either of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp function
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept function = ::scl::concepts::non_reference<T> && ::scl::concepts::compound<T> &&
        ::std::is_function_v<T>;

    /**
     * @brief Satisfied by a class type other than a union.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class other than a union. The closure type
     * of a lambda expression is a class too. A union and an enumeration do not satisfy the concept.
     * The result of the check equals the value of the trait `std::is_class_v<T>`. For a type with
     * cv-qualifiers the concept gives the same result as for the type without them, and a reference
     * type does not satisfy the concept.
     *
     * The concept is declared through ::scl::concepts::object and ::scl::concepts::compound, so its
     * constraint is stricter than the constraints of those concepts and of every concept they are
     * declared through. For a class the compiler chooses a function template constrained by
     * the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp class_type
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept class_type = ::scl::concepts::object<T> && ::scl::concepts::compound<T> &&
        ::std::is_class_v<T>;

    /**
     * @brief Satisfied by a union type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a union. The result of the check equals
     * the value of the trait `std::is_union_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The concept is declared through ::scl::concepts::object and ::scl::concepts::compound, so its
     * constraint is stricter than the constraints of those concepts and of every concept they are
     * declared through. For a union the compiler chooses a function template constrained by
     * the concept over one constrained by any of them.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp union_type
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept union_type = ::scl::concepts::object<T> && ::scl::concepts::compound<T> &&
        ::std::is_union_v<T>;

    /**
     * @brief Satisfied by a class other than a union that holds no data and nothing virtual.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an empty class, that is, a class other than
     * a union that has no non-static data member other than a subobject of zero size, no virtual
     * function, no virtual base class and no base class that is not empty. The result of the check
     * equals the value of the trait `std::is_empty_v<T>`. For a type with cv-qualifiers the concept
     * gives the same result as for the type without them, and a reference type does not satisfy
     * the concept. An empty member with the attribute `[[no_unique_address]]` is a subobject of
     * zero size only under the Itanium ABI, which GCC and Clang follow on every platform other than
     * the MSVC one. Under the MSVC ABI, which MSVC follows and Clang follows when compiling for
     * the MSVC platform, the attribute has no effect, and a class with such a member is not empty.
     * A class with an empty member declared with the macro ::SCL_NO_UNIQUE_ADDRESS is empty under
     * both ABIs if the macro expands to the attribute `[[no_unique_address]]` under the Itanium ABI
     * and to the attribute `[[msvc::no_unique_address]]` under the MSVC ABI.
     *
     * A class that is not a union must be complete, as `std::is_empty` requires. The concept is
     * declared through ::scl::concepts::class_type, so its constraint is stricter than
     * the constraint of that concept and of every concept it is declared through. For an empty
     * class the compiler chooses a function template constrained by the concept over one
     * constrained by any of them.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp empty
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept empty = ::scl::concepts::class_type<T> && ::std::is_empty_v<T>;

    /**
     * @brief Satisfied by a class that declares or inherits a virtual function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class that declares or inherits a virtual
     * function. The result of the check equals the value of the trait `std::is_polymorphic_v<T>`.
     * For a type with cv-qualifiers the concept gives the same result as for the type without them,
     * and a reference type does not satisfy the concept.
     *
     * A class that is not a union must be complete, as `std::is_polymorphic` requires. The concept
     * is declared through ::scl::concepts::class_type, so its constraint is stricter than
     * the constraint of that concept and of every concept it is declared through. For a polymorphic
     * class the compiler chooses a function template constrained by the concept over one
     * constrained by any of them.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp polymorphic
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept polymorphic = ::scl::concepts::class_type<T> && ::std::is_polymorphic_v<T>;

    /**
     * @brief Satisfied by a class with at least one pure virtual function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class with at least one pure virtual
     * function, declared in the class itself or inherited and not overridden in it. An object of
     * such a class exists only as a subobject of a derived class. The result of the check equals
     * the value of the trait `std::is_abstract_v<T>`. For a type with cv-qualifiers the concept
     * gives the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * A class that is not a union must be complete, as `std::is_abstract` requires. The concept is
     * declared through ::scl::concepts::polymorphic, so its constraint is stricter than
     * the constraint of that concept and of every concept it is declared through. For an abstract
     * class the compiler chooses a function template constrained by the concept over one
     * constrained by any of them.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp abstract
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept abstract = ::scl::concepts::polymorphic<T> && ::std::is_abstract_v<T>;

    /**
     * @brief Satisfied by a class or a union declared with the specifier `final`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class or a union declared with
     * the specifier `final`. No class can derive from such a type. The result of the check equals
     * the value of the trait `std::is_final_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * A class or a union must be complete, as `std::is_final` requires. The concept is declared
     * through the disjunction `::scl::concepts::class_type<T> || ::scl::concepts::union_type<T>`,
     * so its constraint is stricter than that disjunction and than the constraints of
     * ::scl::concepts::object, ::scl::concepts::compound and ::scl::concepts::non_reference, but
     * not stricter than the constraint of ::scl::concepts::class_type or of
     * ::scl::concepts::union_type alone. If one template is constrained by the concept and
     * the other by ::scl::concepts::class_type, the choice between those templates is ambiguous for
     * a final class. The same holds for ::scl::concepts::union_type and a final union.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp final
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept final = (::scl::concepts::class_type<T> || ::scl::concepts::union_type<T>) &&
        ::std::is_final_v<T>;

} // namespace scl::concepts
