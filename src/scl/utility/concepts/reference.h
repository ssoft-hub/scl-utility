#pragma once

/// @file
/// @brief Reference-classifying concepts.
/// @ingroup scl_utility_concepts

#include <type_traits>

namespace scl::concepts
{

    /// @brief Satisfied when T is an lvalue or rvalue reference type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::reference<int&>);
    /// static_assert( scl::concepts::reference<int&&>);
    /// static_assert(!scl::concepts::reference<int>);
    ///
    /// template <scl::concepts::reference T>
    /// struct ref_wrapper {};          // matches int& and int&&
    /// @endcode
    template <typename T>
    concept reference = ::std::is_reference_v<T>;

    /// @brief Satisfied when T is an lvalue reference type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::lvalue_reference<int&>);
    /// static_assert(!scl::concepts::lvalue_reference<int&&>);
    /// static_assert(!scl::concepts::lvalue_reference<int>);
    ///
    /// template <scl::concepts::lvalue_reference T>
    /// void pin(T v);                  // only binds to lvalue refs
    /// @endcode
    template <typename T>
    concept lvalue_reference = ::std::is_lvalue_reference_v<T>;

    /// @brief Satisfied when T is an rvalue reference type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::rvalue_reference<int&&>);
    /// static_assert(!scl::concepts::rvalue_reference<int&>);
    /// static_assert(!scl::concepts::rvalue_reference<int>);
    ///
    /// template <scl::concepts::rvalue_reference T>
    /// void sink(T v);                 // only binds to rvalue refs
    /// @endcode
    template <typename T>
    concept rvalue_reference = ::std::is_rvalue_reference_v<T>;

    /// @brief Satisfied when T is not a reference type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::non_reference<int>);
    /// static_assert( scl::concepts::non_reference<int*>);
    /// static_assert( scl::concepts::non_reference<int const>);
    /// static_assert(!scl::concepts::non_reference<int&>);
    /// static_assert(!scl::concepts::non_reference<int&&>);
    /// static_assert(!scl::concepts::non_reference<int const&>);
    ///
    /// // Prevent accidental storage of a dangling reference:
    /// template <scl::concepts::non_reference T>
    /// class box { T value; };
    /// @endcode
    template <typename T>
    concept non_reference = !::std::is_reference_v<T>;

} // namespace scl::concepts
