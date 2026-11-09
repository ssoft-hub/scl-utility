#pragma once

/**
 * @file
 * @brief CV-qualifier-classifying concepts.
 * @ingroup scl_utility_concepts
 */

#include <type_traits>

namespace scl::concepts
{

    /**
     * @brief Satisfied when T is const-qualified.
     * @ingroup scl_utility_concepts
     * @tparam T type to check
     * @note Reference types are never top-level const-qualified:
     *       `const_qualified<int const&>` is false even though `int const&`
     *       references a const object.
     * @par Example
     * @code{.cpp}
     * static_assert( scl::concepts::const_qualified<int const>);
     * static_assert( scl::concepts::const_qualified<int const volatile>);
     * static_assert(!scl::concepts::const_qualified<int>);
     * static_assert(!scl::concepts::const_qualified<int volatile>);
     *
     * template <scl::concepts::const_qualified T>
     * void readonly(T&);              // accepts only const-qualified T
     * @endcode
     */
    template <typename T>
    concept const_qualified = ::std::is_const_v<T>;

    /**
     * @brief Satisfied when T is volatile-qualified.
     * @ingroup scl_utility_concepts
     * @tparam T type to check
     * @note Reference types are never top-level volatile-qualified:
     *       `volatile_qualified<int volatile&>` is false.
     * @par Example
     * @code{.cpp}
     * static_assert( scl::concepts::volatile_qualified<int volatile>);
     * static_assert( scl::concepts::volatile_qualified<int const volatile>);
     * static_assert(!scl::concepts::volatile_qualified<int>);
     * static_assert(!scl::concepts::volatile_qualified<int const>);
     * @endcode
     */
    template <typename T>
    concept volatile_qualified = ::std::is_volatile_v<T>;

    /**
     * @brief Satisfied when T is both const- and volatile-qualified.
     * @ingroup scl_utility_concepts
     * @tparam T type to check
     * @note Reference types satisfy neither `const_qualified` nor `volatile_qualified`,
     *       therefore `cv_qualified<int const volatile&>` is also false.
     * @par Example
     * @code{.cpp}
     * static_assert( scl::concepts::cv_qualified<int const volatile>);
     * static_assert(!scl::concepts::cv_qualified<int const>);
     * static_assert(!scl::concepts::cv_qualified<int volatile>);
     * static_assert(!scl::concepts::cv_qualified<int>);
     * @endcode
     */
    template <typename T>
    concept cv_qualified = ::std::is_const_v<T> && ::std::is_volatile_v<T>;

} // namespace scl::concepts
