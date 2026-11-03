#pragma once

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
        constexpr auto operator()(R (Class::*ptr)(Arguments...)) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) const) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) const &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) const &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Arguments...) volatile) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) volatile noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) volatile &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) volatile & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) volatile &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) volatile && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const volatile) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const volatile noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const volatile &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const volatile & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const volatile &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(/**/
            R (Class::*ptr)(Arguments...) const volatile && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename R>
        [[nodiscard]]
        constexpr auto operator()(R (*ptr)(Arguments...)) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename R>
        [[nodiscard]]
        constexpr auto operator()(R (*ptr)(Arguments...) noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
    };
} // namespace scl::detail

namespace scl
{
#ifndef SCL_DOXYGEN
    template <typename... Arguments>
    inline constexpr ::scl::detail::overload_cast<Arguments...> overload_cast{};
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
     * a template.
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
} // namespace scl
#endif
