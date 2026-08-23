#pragma once

#include <type_traits>

/**
 * @file overload_cast.h
 * @brief Utility for disambiguating function and member function overloads.
 * @ingroup scl_utility_type_traits
 * @details
 * This file provides the `overload_cast` variable template, which simplifies 
 * the process of selecting a specific signature from a set of overloaded 
 * functions or member functions. 
 * 
 * It is particularly useful when taking the address of a function that has 
 * multiple overloads differing by:
 * - Argument types.
 * - Const, volatile, or const-qualifiers volatile.
 * - Reference qualifiers (lvalue & or rvalue &&).
 * - Noexcept specifications.
 * 
 * By using `overload_cast<Args...>(ptr)`, the compiler can explicitly identify 
 * the intended overload without the need for verbose and error-prone 
 * manual `static_cast` to function pointer types.
 */

namespace scl::detail
{
    template <typename... Args>
    struct overload_cast
    {
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...)) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) const) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) const &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) const &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) volatile) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) volatile noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) volatile &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) volatile & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto operator()(R (Class::*ptr)(Args...) volatile &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) volatile && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const volatile) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const volatile noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const volatile &) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const volatile & noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const volatile &&) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename Class, typename R>
        [[nodiscard]]
        constexpr auto
        operator()(R (Class::*ptr)(Args...) const volatile && noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }

        template <typename R>
        [[nodiscard]]
        constexpr auto operator()(R (*ptr)(Args...)) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
        template <typename R>
        [[nodiscard]]
        constexpr auto operator()(R (*ptr)(Args...) noexcept) const noexcept -> decltype(ptr)
        {
            return ptr;
        }
    };
} // namespace scl::detail

namespace scl
{
#ifndef DOXYGEN
    template <typename... Args>
    inline constexpr detail::overload_cast<Args...> overload_cast{};
#endif

} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

// The real declaration names its type in `scl::detail`, which the reference excludes: a
// reader would be given a type they cannot reach. Doxygen sees the type unspecified instead.
#ifdef DOXYGEN
namespace scl
{
    /**
     * @brief A helper utility to select a specific overload of a function or member function.
     * @ingroup scl_utility_type_traits
     *
     * Its call operators disambiguate overloaded functions by their argument types
     * (`Args...`) and their qualifiers — `const`, `volatile`, lvalue and rvalue reference,
     * and `noexcept`.
     *
     * @tparam Args  The exact argument types used to identify the desired overload.
     *
     * @code
     * struct X {
     *   void foo(int) &;
     *   void foo(double) const &;
     *   void foo(int) const &&;
     * };
     *
     * void bar(int);
     * void bar(double) noexcept;
     *
     * auto pf1 = overload_cast<int>(&X::foo);        // selects void foo(int) &
     * auto pf2 = overload_cast<double>(&X::foo);     // selects void foo(double) const &
     * auto f1  = overload_cast<int>(&bar);           // selects void bar(int)
     * auto f2  = overload_cast<double>(&bar);        // selects void bar(double) noexcept
     * @endcode
     */
    template <typename... Args>
    inline constexpr auto overload_cast;
} // namespace scl
#endif
