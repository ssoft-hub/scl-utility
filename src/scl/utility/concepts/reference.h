#pragma once

/**
 * @file
 * @brief Concepts of reference types.
 * @ingroup scl_utility_concepts
 */

#include <type_traits>

namespace scl::concepts
{

    /**
     * @brief Satisfied by an lvalue reference type and by an rvalue reference type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a reference of any kind, an lvalue reference
     * or an rvalue reference, whatever the type it refers to. A pointer does not satisfy
     * the concept, since it is not a reference. The result of the check equals the value of
     * the trait `std::is_reference_v<T>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp reference
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept reference = ::std::is_reference_v<T>;

    /**
     * @brief Satisfied by an lvalue reference type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an lvalue reference, such as `int &` or
     * `int const &`. The result of the check equals the value of the trait
     * `std::is_lvalue_reference_v<T>`. For the parameter `Argument &&` of a function template
     * an lvalue reference is deduced as the type `Argument` for an lvalue argument, and a type with
     * no reference for an rvalue argument, so such a type `Argument` satisfies the concept only for
     * an lvalue argument.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp lvalue_reference
     * @snippet concepts/reference/concepts_reference_example.cpp forwarding
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept lvalue_reference = ::std::is_lvalue_reference_v<T>;

    /**
     * @brief Satisfied by an rvalue reference type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an rvalue reference, such as `int &&` or
     * `int const &&`. The result of the check equals the value of the trait
     * `std::is_rvalue_reference_v<T>`. For the parameter `Argument &&` of a function template
     * a type with no reference is deduced as the type `Argument` for an rvalue argument, so such
     * a type `Argument` does not satisfy the concept for any argument.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp rvalue_reference
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept rvalue_reference = ::std::is_rvalue_reference_v<T>;

    /**
     * @brief Satisfied by every type that is not a reference type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is not a reference, that is, if it is an object
     * type, a function type or the type `void`. The standard library has no trait for this check,
     * so the concept is written as the negation of the trait `std::is_reference_v<T>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp non_reference
     * @snippet concepts/reference/concepts_reference_example.cpp box
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept non_reference = !::std::is_reference_v<T>;

} // namespace scl::concepts
