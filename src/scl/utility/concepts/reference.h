#pragma once

/**
 * @file
 * @brief Concepts of reference types.
 * @ingroup scl_utility_concepts
 */

#include <scl/utility/concepts/type_category.h>

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
     * The concept is declared through ::scl::concepts::compound, so its constraint is stricter than
     * the constraint of ::scl::concepts::compound. For a reference the compiler chooses a function
     * template constrained by the concept over one constrained by ::scl::concepts::compound.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp reference
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept reference = ::scl::concepts::compound<T> && ::std::is_reference_v<T>;

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
     * The concept is declared through ::scl::concepts::reference, so its constraint is stricter
     * than the constraints of ::scl::concepts::reference and ::scl::concepts::compound. For
     * an lvalue reference the compiler chooses a function template constrained by the concept over
     * one constrained by either of them.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp lvalue_reference
     * @snippet concepts/reference/concepts_reference_example.cpp forwarding
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept lvalue_reference = ::scl::concepts::reference<T> && ::std::is_lvalue_reference_v<T>;

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
     * The concept is declared through ::scl::concepts::reference, so its constraint is stricter
     * than the constraints of ::scl::concepts::reference and ::scl::concepts::compound. For
     * an rvalue reference the compiler chooses a function template constrained by the concept over
     * one constrained by either of them.
     *
     * @snippet concepts/reference/concepts_reference_example.cpp rvalue_reference
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept rvalue_reference = ::scl::concepts::reference<T> && ::std::is_rvalue_reference_v<T>;

} // namespace scl::concepts
