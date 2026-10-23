#pragma once

/**
 * @file member_qualifiers.h
 * @internal
 * @brief The qualifiers of a member function, read from the type of the object it is called on.
 * @ingroup scl_utility_type_traits
 */

#include <type_traits>

namespace scl::detail
{
    enum class member_function_cv : unsigned char
    {
        none,
        const_only,
        volatile_only,
        const_volatile
    };

    enum class member_function_reference : unsigned char
    {
        none,
        lvalue,
        rvalue
    };

    template <typename Type>
    inline constexpr ::scl::detail::member_function_cv member_function_cv_of =
        ::scl::detail::member_function_cv::none;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_cv member_function_cv_of<Type const> =
        ::scl::detail::member_function_cv::const_only;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_cv member_function_cv_of<Type volatile> =
        ::scl::detail::member_function_cv::volatile_only;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_cv member_function_cv_of<Type const volatile> =
        ::scl::detail::member_function_cv::const_volatile;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_cv member_function_cv_of<Type &> =
        ::scl::detail::member_function_cv_of<Type>;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_cv member_function_cv_of<Type &&> =
        ::scl::detail::member_function_cv_of<Type>;

    template <typename Type>
    inline constexpr ::scl::detail::member_function_reference member_function_reference_of =
        ::scl::detail::member_function_reference::none;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_reference member_function_reference_of<Type &> =
        ::scl::detail::member_function_reference::lvalue;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_reference member_function_reference_of<Type &&> =
        ::scl::detail::member_function_reference::rvalue;
} // namespace scl::detail
