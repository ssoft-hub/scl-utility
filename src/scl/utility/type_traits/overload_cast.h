#pragma once

#include <scl/utility/type_traits/detail/exact.h>
#include <scl/utility/type_traits/detail/member_qualifiers.h>

#include <type_traits>

/**
 * @file
 * @brief The pointer to one overload of a function, chosen by its parameter types.
 * @ingroup scl_utility_type_traits
 */

namespace scl::detail
{
    template <typename... Arguments>
    struct overload_cast
    {
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...)) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) & noexcept) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) && noexcept) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) const) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const noexcept) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) const &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const & noexcept) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) const &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const && noexcept) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) volatile) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) volatile noexcept) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) volatile &) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) volatile & noexcept)
            const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) volatile &&) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) volatile && noexcept)
            const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const volatile) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const volatile noexcept)
            const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const volatile &) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const volatile & noexcept)
            const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const volatile &&) const noexcept
            -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(::scl::detail::exact_parameter_t<Arguments>...) const volatile && noexcept)
            const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename R>
        [[nodiscard]]
        constexpr auto operator()(
            R (*ptr)(::scl::detail::exact_parameter_t<Arguments>...)) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename R>
        [[nodiscard]]
        constexpr auto operator()(R (*ptr)(
            ::scl::detail::exact_parameter_t<Arguments>...) noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
    };

    template <::scl::detail::member_function_cv Cv, ::scl::detail::member_function_reference Reference, typename... Arguments>
    struct member_overload_cast;

// NOLINTBEGIN(bugprone-macro-parentheses)
#define SCL_DETAIL_MEMBER_OVERLOAD_CAST(cv, reference, qualifiers)                                                \
    template <typename... Arguments>                                                                              \
    struct member_overload_cast<                                                                                  \
        ::scl::detail::member_function_cv::cv, ::scl::detail::member_function_reference::reference, Arguments...> \
    {                                                                                                             \
        template <typename Class, typename R>                                                                     \
        [[nodiscard]]                                                                                             \
        constexpr auto operator()(R (Class::*ptr)(                                                                \
            ::scl::detail::exact_parameter_t<Arguments>...) qualifiers) const noexcept -> decltype(ptr)           \
        {                                                                                                         \
            return ptr;                                                                                           \
        }                                                                                                         \
        template <typename Class, typename R>                                                                     \
        [[nodiscard]]                                                                                             \
        constexpr auto operator()(R (Class::*ptr)(::scl::detail::exact_parameter_t<                               \
            Arguments>...) qualifiers noexcept) const noexcept -> decltype(ptr)                                   \
        {                                                                                                         \
            return ptr;                                                                                           \
        }                                                                                                         \
    };
    // NOLINTEND(bugprone-macro-parentheses)

    SCL_DETAIL_MEMBER_OVERLOAD_CAST(none, none, )
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(none, lvalue, &)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(none, rvalue, &&)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(const_only, none, const)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(const_only, lvalue, const &)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(const_only, rvalue, const &&)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(volatile_only, none, volatile)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(volatile_only, lvalue, volatile &)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(volatile_only, rvalue, volatile &&)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(const_volatile, none, const volatile)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(const_volatile, lvalue, const volatile &)
    SCL_DETAIL_MEMBER_OVERLOAD_CAST(const_volatile, rvalue, const volatile &&)

#undef SCL_DETAIL_MEMBER_OVERLOAD_CAST
} // namespace scl::detail

namespace scl
{
#ifndef SCL_DOXYGEN
    template <typename... Arguments>
    inline constexpr ::scl::detail::overload_cast<Arguments...> overload_cast{};

    template <typename Type, typename... Arguments>
    inline constexpr ::scl::detail::member_overload_cast<
        ::scl::detail::member_function_cv_of<Type>,
        ::scl::detail::member_function_reference_of<Type>, Arguments...>
        member_overload_cast{};
#endif

} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef SCL_DOXYGEN
namespace scl
{
    /**
     * @brief A function object returning the pointer to the overload whose parameters are @p Arguments.
     * @ingroup scl_utility_type_traits
     *
     * A call with the address of an overload set, `&f` or `&Class::f`, returns the pointer to
     * the overload whose parameters are exactly @p Arguments, whatever its result and `noexcept`.
     * The top-level `const` and `volatile` of @p Arguments are not compared, since they are not part of
     * a function type. A member function may have any cv-qualifiers and ref-qualifier. The returned
     * pointer has the type of that overload, and the call operator is declared `constexpr`,
     * `noexcept` and `[[nodiscard]]`.
     *
     * The qualifiers of the member function itself do not count in the choice. So the call does not
     * compile where two member functions with the parameters @p Arguments differ only in their
     * cv-qualifiers or their ref-qualifier, such as `int f(int)` and `int f(int) const`, nor where
     * no overload has the parameters @p Arguments. Nor does the call compile where the overload set
     * holds a function template, even if the overload with the parameters @p Arguments is not
     * a template. Among member functions that differ only in their qualifiers, one is chosen by
     * ::scl::member_overload_cast.
     *
     * A C-style variadic overload, one whose parameter list ends with `...`, is never selected.
     * An overload set that holds, beside the overload with the parameters @p Arguments, an overload with
     * the same parameters and `...` is not supported, and the outcome of the call for it depends on
     * the compiler.
     *
     * @snippet type_traits/overload_cast/type_traits_overload_cast_example.cpp types
     * @snippet type_traits/overload_cast/type_traits_overload_cast_example.cpp member
     * @snippet type_traits/overload_cast/type_traits_overload_cast_example.cpp function
     *
     * @tparam Arguments Parameter types of the overload to select.
     * @see ::scl::signature_t
     */
    template <typename... Arguments>
    inline constexpr auto overload_cast;

    /**
     * @brief A function object returning the pointer to the member function qualified as @p Type.
     * @ingroup scl_utility_type_traits
     *
     * A call with the address of an overload set of member functions, `&Class::f`, returns
     * the pointer to the member function whose parameters are exactly @p Arguments and whose qualifiers
     * are those of the object type @p Type, whatever its result and `noexcept`. The cv-qualifiers
     * of the member function are those of @p Type, and its ref-qualifier is `&` for an lvalue
     * reference, `&&` for an rvalue reference and none for a type with no reference, as in
     * ::scl::member_function_like_t. So one overload is chosen among member functions that differ
     * only in their qualifiers. A member function with no ref-qualifier is chosen only for a @p Type
     * with no reference, so `int level() const` is chosen for `gauge const`, and with
     * `gauge const &` the call does not compile, although the member function can be called on such
     * an object. The parameter types are compared exactly, their own qualifiers included apart from
     * the top-level ones.
     *
     * The class of the pointer is the class that declares the member function, and the call
     * operator is declared `constexpr`, `noexcept` and `[[nodiscard]]`. A member function with
     * an explicit object parameter is never selected. As for ::scl::overload_cast, the call does
     * not compile where the overload set holds a function template, and a C-style variadic
     * overload is never selected.
     *
     * @snippet type_traits/overload_cast/type_traits_overload_cast_example.cpp types
     * @snippet type_traits/overload_cast/type_traits_overload_cast_example.cpp qualified
     * @snippet type_traits/overload_cast/type_traits_overload_cast_example.cpp no_ref_qualifier
     *
     * @tparam Type Type of the object, with the cv-qualifiers and the reference of the overload.
     * @tparam Arguments Parameter types of the overload to select.
     */
    template <typename Type, typename... Arguments>
    inline constexpr auto member_overload_cast;
} // namespace scl
#endif
