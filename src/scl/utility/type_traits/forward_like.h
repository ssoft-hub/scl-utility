#pragma once

#include <scl/utility/attribute/lifetimebound.h>

#include <type_traits>

/**
 * @file
 * @brief The type and the cast that pass an object on with the value category of another type.
 * @ingroup scl_utility_type_traits
 */

namespace scl::detail
{
    // Add const from From onto To if From is const (ignoring reference)
    template <typename From, typename To>
    using add_const_from_t = ::std::conditional_t<
        ::std::is_const_v<::std::remove_reference_t<From>>, ::std::add_const_t<To>, To>;

    // Add volatile from From onto To if From is volatile (ignoring reference)
    template <typename From, typename To>
    using add_volatile_from_t = ::std::conditional_t<
        ::std::is_volatile_v<::std::remove_reference_t<From>>, ::std::add_volatile_t<To>, To>;

    // Add both const and volatile from From to To (ignoring reference)
    template <typename From, typename To>
    using add_cv_from_t =
        ::scl::detail::add_const_from_t<From, ::scl::detail::add_volatile_from_t<From, To>>;

    // The reference of From onto To: & for an lvalue reference, && otherwise, and none for void.
    template <typename From, typename To>
    using add_reference_like_t = ::std::conditional_t<
        ::std::is_lvalue_reference_v<From>,
        ::std::conditional_t<::std::is_void_v<To>, To, ::std::add_lvalue_reference_t<To>>,
        ::std::conditional_t<::std::is_void_v<To>, To, ::std::add_rvalue_reference_t<To>>>;

} // namespace scl::detail

namespace scl
{
#ifndef SCL_DOXYGEN
    template <typename Base, typename Type>
    using forward_like_t = ::scl::detail::add_reference_like_t<
        Base, ::scl::detail::add_cv_from_t<Base, ::std::remove_reference_t<Type>>>;
#endif

    template <typename Base, typename Type>
    [[nodiscard]]
    constexpr decltype(auto) forward_like(Type && t SCL_LIFETIMEBOUND) noexcept // NOLINT(cppcoreguidelines-missing-std-forward)
    {
        return static_cast<::scl::forward_like_t<Base, Type>>(t);
    }

} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef SCL_DOXYGEN
namespace scl
{
    template <typename Base, typename Type>
    using forward_like_t = unspecified;
} // namespace scl
#endif

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::forward_like_t
 * @ingroup scl_utility_type_traits
 * @brief @p Type with the value category of @p Base and the cv-qualifiers of both types.
 *
 * The result is an lvalue reference where @p Base is one and an rvalue reference otherwise, and
 * the reference of @p Type is dropped.
 *
 * The `const` and `volatile` of @p Base are added to those of @p Type, so the result has
 * the cv-qualifiers of both types. For a @p Type that is `void`, with or without cv-qualifiers,
 * the result is `void` with the cv-qualifiers of both types and with no reference.
 *
 * The qualifier `volatile` of @p Base is added to the type this alias template names, and not to
 * the result type of the function template `std::forward_like` of C++23.
 *
 * @snippet type_traits/forward_like/type_traits_forward_like_example.cpp type
 *
 * @tparam Base Type whose value category and cv-qualifiers are given to @p Type.
 * @tparam Type Type to qualify.
 * @see ::scl::forward_like
 */

/**
 * @fn scl::forward_like(Type && t)
 * @ingroup scl_utility_type_traits
 * @brief Returns @p t as ::scl::forward_like_t<Base, Type>.
 *
 * The result is an lvalue where @p Base is an lvalue reference and an rvalue otherwise, with
 * the `const` and `volatile` of @p Base added.
 *
 * A member of an object is passed on with the value category of the object. The argument is cast
 * to a reference type, and nothing is copied or moved by the call. The parameter is marked with
 * @ref SCL_LIFETIMEBOUND, so a compiler that supports the check warns where the returned reference
 * outlives a temporary argument.
 *
 * @snippet type_traits/forward_like/type_traits_forward_like_example.cpp member
 * @snippet type_traits/forward_like/type_traits_forward_like_example.cpp move
 *
 * @tparam Base Type whose value category and cv-qualifiers are given to @p t.
 * @tparam Type Type of the argument, deduced.
 * @param t Object to pass on.
 * @return A reference to @p t.
 */
