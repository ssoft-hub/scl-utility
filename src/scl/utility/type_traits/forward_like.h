#pragma once

#include <type_traits>

/**
 * @file
 * @brief Helpers for forwarding objects with combined cv/ref qualifiers (`forward_like_t`).
 * @ingroup scl_utility_type_traits
 *
 * Contains type trait and forwarding utility that repeat the logic of C++23 std::forward_like.
 */

namespace scl::detail
{
    // Add const from From onto To if From is const (ignoring reference)
    template <typename From, typename To>
    using add_const_from_t =
        ::std::conditional_t<::std::is_const_v<::std::remove_reference_t<From>>, ::std::add_const_t<To>, To>;

    // Add volatile from From onto To if From is volatile (ignoring reference)
    template <typename From, typename To>
    using add_volatile_from_t =
        ::std::conditional_t<::std::is_volatile_v<::std::remove_reference_t<From>>, ::std::add_volatile_t<To>, To>;

    // Add both const and volatile from From to To (ignoring reference)
    template <typename From, typename To>
    using add_cv_from_t = add_const_from_t<From, add_volatile_from_t<From, To>>;

    // Copy ref from From onto To: if From is &, add &, if && add &&, else no reference.
    //s Don't add reference to void.
    template <typename From, typename To>
    using add_reference_like_t = ::std::conditional_t<::std::is_lvalue_reference_v<From>,
        ::std::conditional_t<::std::is_void_v<To>, To, ::std::add_lvalue_reference_t<To>>,
        ::std::conditional_t<::std::is_rvalue_reference_v<From>,
            ::std::conditional_t<::std::is_void_v<To>, To, ::std::add_rvalue_reference_t<To>>,
            To>>;

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Make a type `Type` "look like" `Base` for cv-ref qualifiers (matches ::std::forward_like rules).
     * @ingroup scl_utility_type_traits
     *
     * @tparam Base  Type providing qualifiers/ref to propagate.
     * @tparam Type  Type to which qualifiers/ref are applied.
     *
     * Semantics:
     * - CV-qualifiers:
     *      Applies `const` and `volatile` from `Base` onto `remove_reference_t<Type>`.
     *      (i.e. union of qualifiers, not replacement)
     * - Reference:
     *      Applies reference category of `Base` (if any).
     *      If `Base` is lvalue ref, result is lvalue ref; if rvalue ref, rvalue ref; else no reference.
     *      Reference is omitted for void.
     *
     * @par Examples
     * @code{.cpp}
     *    using P0 = forward_like_t<int const &, double>            // double const &
     *    using P1 = forward_like_t<int &&, float const>            // float const &&
     *    using P2 = forward_like_t<int volatile, char>             // char volatile
     *    using P3 = forward_like_t<int const volatile &, short>    // short const volatile &
     *    using P4 = forward_like_t<int, double&&>                  // double
     * @endcode
     */
    template <typename Base, typename Type>
    using forward_like_t = ::scl::detail::add_reference_like_t<Base,
        ::scl::detail::add_cv_from_t<Base, ::std::remove_reference_t<Type>>>;

    /**
     * @brief Forwards a value with the cv-ref qualifiers of `Base` applied to `T`'s type,
     *        matching ::std::forward_like semantics.
     * @ingroup scl_utility_type_traits
     *
     * Example: `forward_like<const T&, x>(some_val)` will forward `some_val`
     * as a `const` lvalue reference if `T` is const.
     */
    template <typename Base, typename T>
    constexpr decltype(auto) forward_like(T && t) noexcept
    {
        return static_cast<forward_like_t<Base, T &&>>(t);
    }

} // namespace scl
