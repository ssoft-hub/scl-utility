#pragma once

/**
 * @file exact.h
 * @internal
 * @brief The pointer to the overload whose parameters are exactly the given types.
 * @ingroup scl_utility_type_traits
 *
 * The function `select` deduces the result type from the overload set itself, so an overload
 * with exactly the parameters @p Arguments is found even where the plain call is ambiguous.
 * An overload set holding a function template gives no deduction, so there the overload whose
 * result type is @p Hint, the result type of the plain call, is taken.
 *
 * MSVC does not reject during substitution the address of a deleted function, nor that of
 * an inaccessible one taken from an overload set. The plain call rejects a deleted function there,
 * so on MSVC the deduction is left out, and the overload is always taken by @p Hint, which
 * the plain call has to give.
 *
 * A `void` among @p Arguments names no parameter type. The declarations of `select` would then end
 * the build with an error, so the constraint `requires(!std::is_void_v<Arguments> && ...)` makes
 * naming the class template a substitution failure instead.
 */

#include <type_traits>

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_EXACT_DEDUCED(...)
#else
#define SCL_DETAIL_EXACT_DEDUCED(...) __VA_ARGS__
#endif

namespace scl::detail
{
    template <typename Hint, typename... Arguments>
        requires(!::std::is_void_v<Arguments> && ...)
    struct exact_function
    {
        SCL_DETAIL_EXACT_DEDUCED(
            template <typename Result> static auto select(Result (*function)(Arguments...))->decltype(function);)
        static auto select(Hint (*function)(Arguments...)) -> decltype(function);
    };

    template <typename Type, typename Hint, typename... Arguments>
        requires(!::std::is_void_v<Arguments> && ...)
    struct exact_member
    {
        SCL_DETAIL_EXACT_DEDUCED(
            template <typename Result, typename Class>
            static auto select(Result (Class::*member)(Arguments...))
                ->decltype(member);)
        template <typename Class = Type>
        static auto select(Hint (Class::*member)(Arguments...)) -> decltype(member);
    };

// NOLINTBEGIN(bugprone-macro-parentheses)
#define SCL_DETAIL_EXACT_MEMBER(qualifiers)                                                     \
    template <typename Type, typename Hint, typename... Arguments>                              \
        requires(!::std::is_void_v<Arguments> && ...)                                           \
    struct exact_member<Type qualifiers, Hint, Arguments...>                                    \
    {                                                                                           \
        SCL_DETAIL_EXACT_DEDUCED(                                                               \
            template <typename Result, typename Class>                                          \
            static auto select(Result (Class::*member)(Arguments...) qualifiers)                \
                ->decltype(member);)                                                            \
        template <typename Class = Type>                                                        \
        static auto select(Hint (Class::*member)(Arguments...) qualifiers) -> decltype(member); \
    };
    // NOLINTEND(bugprone-macro-parentheses)

    SCL_DETAIL_EXACT_MEMBER(const)
    SCL_DETAIL_EXACT_MEMBER(volatile)
    SCL_DETAIL_EXACT_MEMBER(const volatile)
    SCL_DETAIL_EXACT_MEMBER(&)
    SCL_DETAIL_EXACT_MEMBER(const &)
    SCL_DETAIL_EXACT_MEMBER(volatile &)
    SCL_DETAIL_EXACT_MEMBER(const volatile &)
    SCL_DETAIL_EXACT_MEMBER(&&)
    SCL_DETAIL_EXACT_MEMBER(const &&)
    SCL_DETAIL_EXACT_MEMBER(volatile &&)
    SCL_DETAIL_EXACT_MEMBER(const volatile &&)

#undef SCL_DETAIL_EXACT_MEMBER
} // namespace scl::detail

#undef SCL_DETAIL_EXACT_DEDUCED
