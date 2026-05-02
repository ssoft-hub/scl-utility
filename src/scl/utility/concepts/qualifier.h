#pragma once

/// @file
/// @brief CV-qualifier-classifying concepts.
/// @ingroup scl_utility_concepts

#include <type_traits>

namespace scl::concepts
{

    /// @brief Satisfied when T is const-qualified.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @note Reference types are never top-level const-qualified:
    ///       `const_type<int const&>` is false even though `int const&`
    ///       references a const object.
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::const_type<int const>);
    /// static_assert( scl::concepts::const_type<int const volatile>);
    /// static_assert(!scl::concepts::const_type<int>);
    /// static_assert(!scl::concepts::const_type<int volatile>);
    ///
    /// template <scl::concepts::const_type T>
    /// void readonly(T&);              // accepts only const-qualified T
    /// @endcode
    template <typename T>
    concept const_type = ::std::is_const_v<T>;

    /// @brief Satisfied when T is volatile-qualified.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @note Reference types are never top-level volatile-qualified:
    ///       `volatile_type<int volatile&>` is false.
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::volatile_type<int volatile>);
    /// static_assert( scl::concepts::volatile_type<int const volatile>);
    /// static_assert(!scl::concepts::volatile_type<int>);
    /// static_assert(!scl::concepts::volatile_type<int const>);
    /// @endcode
    template <typename T>
    concept volatile_type = ::std::is_volatile_v<T>;

    /// @brief Satisfied when T is both const- and volatile-qualified.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @note Reference types satisfy neither `const_type` nor `volatile_type`,
    ///       therefore `cv_type<int const volatile&>` is also false.
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::cv_type<int const volatile>);
    /// static_assert(!scl::concepts::cv_type<int const>);
    /// static_assert(!scl::concepts::cv_type<int volatile>);
    /// static_assert(!scl::concepts::cv_type<int>);
    /// @endcode
    template <typename T>
    concept cv_type = ::std::is_const_v<T> && ::std::is_volatile_v<T>;

} // namespace scl::concepts
