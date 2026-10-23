#pragma once

/**
 * @file
 * @brief The macro `SCL_NORETURN`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(noreturn)
#define SCL_DETAIL_NORETURN_STD [[noreturn]]
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_NORETURN_MSVC __declspec(noreturn)
#endif

#ifdef __has_attribute
#if __has_attribute(__noreturn__)
#define SCL_DETAIL_NORETURN_ATTRIBUTE __attribute__((__noreturn__))
#endif
#endif

#define SCL_DETAIL_NORETURN_FALLBACK

#ifndef SCL_NORETURN
#ifdef SCL_DETAIL_NORETURN_STD
#define SCL_NORETURN SCL_DETAIL_NORETURN_STD
#elif defined(SCL_DETAIL_NORETURN_MSVC)
#define SCL_NORETURN SCL_DETAIL_NORETURN_MSVC
#elif defined(SCL_DETAIL_NORETURN_ATTRIBUTE)
#define SCL_NORETURN SCL_DETAIL_NORETURN_ATTRIBUTE
#else
#define SCL_NORETURN SCL_DETAIL_NORETURN_FALLBACK
#endif
#endif

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_NORETURN),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_NORETURN));
// clang-format on

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_NORETURN
 * @brief The marked function never returns to its caller.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** every call of the function ends the program, throws
 *   an exception, jumps away or never ends. The compiler takes the code after a call as never
 *   reached and stops warning that a function returning a value may end without one.
 * - **A false promise:** the behaviour of the program is undefined where the function returns and
 *   the expansion is taken from a row of the table other than the last.
 * - **Beside other macros:** `SCL_NORETURN` must be written before `SCL_NOINLINE` and
 *   `SCL_FORCE_INLINE`. `[[noreturn]]` after `__declspec(noinline)` or `__forceinline` is error
 *   C3829 on MSVC, and `[[noreturn]]` after the `inline` of `SCL_FORCE_INLINE` is an error on GCC
 *   and Clang too.
 * - **Place:** the macro is written before the declaration of a function.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | `__has_cpp_attribute(noreturn)` (C++11) | `[[noreturn]]` |
 * | MSVC other than clang-cl | `__declspec(noreturn)` |
 * | `__has_attribute(__noreturn__)` | `__attribute__((__noreturn__))` |
 * | none of the above | nothing |
 *
 * The macro expands to `[[noreturn]]` on GCC, Clang and MSVC in C++20.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_NORETURN` | `[[noreturn]]`, `__attribute__((__noreturn__))`, empty | `[[noreturn]]`, `__attribute__((__noreturn__))`, empty | `[[noreturn]]`, `__declspec(noreturn)`, empty |
 *
 * @snippet attribute/noreturn/attribute_noreturn_example.cpp noreturn
 */
