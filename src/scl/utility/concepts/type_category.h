#pragma once

/**
 * @file
 * @brief Concepts of the categories of a type, such as enumeration, pointer, array and class.
 * @ingroup scl_utility_concepts
 */

#include <type_traits>

namespace scl::concepts
{

    /**
     * @brief Satisfied by an enumeration type, scoped or unscoped.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an enumeration, scoped or unscoped.
     * The result of the check equals the value of the trait `std::is_enum_v<T>`. For a type with
     * cv-qualifiers the concept gives the same result as for the type without them, and a reference
     * type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp enum_type
     * @snippet concepts/type_category/concepts_type_category_example.cpp underlying
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept enum_type = ::std::is_enum_v<T>;

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
     * than the constraint of that concept. For a scoped enumeration the compiler chooses a function
     * template constrained by the concept over one constrained by ::scl::concepts::enum_type.
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
     * @brief Satisfied by the type `void` with or without cv-qualifiers.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is the type `void`. The result of the check
     * equals the value of the trait `std::is_void_v<T>`. For a type with cv-qualifiers the concept
     * gives the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp void_type
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept void_type = ::std::is_void_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept pointer = ::std::is_pointer_v<T>;

    /**
     * @brief Satisfied by the type `std::nullptr_t` with or without cv-qualifiers.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is the type `std::nullptr_t`, that is, the type
     * of the literal `nullptr`. The result of the check equals the value of the trait
     * `std::is_null_pointer_v<T>`. For a type with cv-qualifiers the concept gives the same result
     * as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp null_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept null_pointer = ::std::is_null_pointer_v<T>;

    /**
     * @brief Satisfied by a pointer to a data member and by a pointer to a member function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a pointer to a data member or a pointer to
     * a member function. The result of the check equals the value of the trait
     * `std::is_member_pointer_v<T>`. For a type with cv-qualifiers the concept gives the same
     * result as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp member_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept member_pointer = ::std::is_member_pointer_v<T>;

    /**
     * @brief Satisfied by a pointer to a data member.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a pointer to a data member. The result of
     * the check equals the value of the trait `std::is_member_object_pointer_v<T>`. For a type with
     * cv-qualifiers the concept gives the same result as for the type without them, and a reference
     * type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp member_object_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept member_object_pointer = ::std::is_member_object_pointer_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp member_function_pointer
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept member_function_pointer = ::std::is_member_function_pointer_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp array
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept array = ::std::is_array_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp bounded_array
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept bounded_array = ::std::is_bounded_array_v<T>;

    /**
     * @brief Satisfied by an array type with an unknown bound, such as `int[]`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an array with an unknown bound, the type
     * `int[][3]` included. The result of the check equals the value of the trait
     * `std::is_unbounded_array_v<T>`. For a type with cv-qualifiers the concept gives the same
     * result as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp unbounded_array
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept unbounded_array = ::std::is_unbounded_array_v<T>;

    /**
     * @brief Satisfied by a function type, such as `void(int)`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a function type, such as `void(int)`, with
     * cv-qualifiers, a reference qualifier or the specifier `noexcept` included. A pointer to
     * a function and a reference to a function do not satisfy the concept. The result of the check
     * equals the value of the trait `std::is_function_v<T>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp function
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept function = ::std::is_function_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp class_type
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept class_type = ::std::is_class_v<T>;

    /**
     * @brief Satisfied by a union type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a union. The result of the check equals
     * the value of the trait `std::is_union_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp union_type
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept union_type = ::std::is_union_v<T>;

    /**
     * @brief Satisfied by an arithmetic type, by `void` and by `std::nullptr_t`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an arithmetic type, the type `void` or
     * the type `std::nullptr_t`. The result of the check equals the value of the trait
     * `std::is_fundamental_v<T>`. For a type with cv-qualifiers the concept gives the same result
     * as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp fundamental
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept fundamental = ::std::is_fundamental_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp arithmetic
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept arithmetic = ::std::is_arithmetic_v<T>;

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
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp scalar
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept scalar = ::std::is_scalar_v<T>;

    /**
     * @brief Satisfied by every type other than a function type, a reference type and `void`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is not a function type, a reference type or
     * the type `void`. An array is an object type too. The result of the check equals the value of
     * the trait `std::is_object_v<T>`. For a type with cv-qualifiers the concept gives the same
     * result as for the type without them, and a reference type does not satisfy the concept.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_category/concepts_type_category_example.cpp object
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept object = ::std::is_object_v<T>;

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

} // namespace scl::concepts
