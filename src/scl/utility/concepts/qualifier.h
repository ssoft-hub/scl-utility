#pragma once

/**
 * @file
 * @brief Concepts of the top-level cv-qualifiers of a type.
 * @ingroup scl_utility_concepts
 */

#include <type_traits>

namespace scl::concepts
{

    /**
     * @brief Satisfied by a type with the top-level qualifier `const`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it carries the qualifier `const`, such as
     * `int const` or `int * const`. An array of `const` elements, such as `int const[3]`, is also
     * a `const` type. The type `int const *` does not satisfy the concept, since the qualifier
     * belongs to the type the pointer points to, not to the pointer itself. Nor does the type
     * `int const &` satisfy it, since a reference cannot carry qualifiers of its own. The result of
     * the check equals the value of the trait `std::is_const_v<T>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/qualifier/concepts_qualifier_example.cpp const_qualified
     * @snippet concepts/qualifier/concepts_qualifier_example.cpp reset
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept const_qualified = ::std::is_const_v<T>;

    /**
     * @brief Satisfied by a type with the top-level qualifier `volatile`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it carries the qualifier `volatile`. The qualifier
     * counts by the same rules as the qualifier `const`, so the types `int volatile` and
     * `int volatile[3]` satisfy the concept, while the types `int volatile *` and `int volatile &`
     * do not. The result of the check equals the value of the trait `std::is_volatile_v<T>`.
     *
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/qualifier/concepts_qualifier_example.cpp volatile_qualified
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept volatile_qualified = ::std::is_volatile_v<T>;

    /**
     * @brief Satisfied by a type with both top-level qualifiers `const` and `volatile`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it carries both qualifiers at once, such as
     * `int const volatile`. A type with only one of them does not satisfy the concept. The standard
     * library has no trait for this check.
     *
     * The concept is declared through ::scl::concepts::const_qualified and
     * ::scl::concepts::volatile_qualified, so its constraint is stricter than the constraints of
     * those concepts. For a type with both qualifiers the compiler chooses a function template
     * constrained by the concept over one constrained by either of them. If no template constrained
     * by the concept is declared, and both a template constrained by
     * ::scl::concepts::const_qualified and a template constrained by
     * ::scl::concepts::volatile_qualified accept such a type, the compiler rejects the call as
     * ambiguous.
     *
     * @snippet concepts/qualifier/concepts_qualifier_example.cpp cv_qualified
     * @snippet concepts/qualifier/concepts_qualifier_example.cpp qualifiers
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept cv_qualified = ::scl::concepts::const_qualified<T> && ::scl::concepts::volatile_qualified<T>;

} // namespace scl::concepts
