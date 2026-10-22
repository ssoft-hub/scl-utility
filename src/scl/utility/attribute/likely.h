#pragma once

/**
 * @file
 * @brief The macros `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR` and `SCL_UNLIKELY_EXPR`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(likely)
#define SCL_DETAIL_LIKELY_STD [[likely]]
#endif
#endif

#define SCL_DETAIL_LIKELY_FALLBACK

#ifndef SCL_LIKELY
#ifdef SCL_DETAIL_LIKELY_STD
#define SCL_LIKELY SCL_DETAIL_LIKELY_STD
#else
#define SCL_LIKELY SCL_DETAIL_LIKELY_FALLBACK
#endif
#endif

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_LIKELY),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_LIKELY));
// clang-format on

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(unlikely)
#define SCL_DETAIL_UNLIKELY_STD [[unlikely]]
#endif
#endif

#define SCL_DETAIL_UNLIKELY_FALLBACK

#ifndef SCL_UNLIKELY
#ifdef SCL_DETAIL_UNLIKELY_STD
#define SCL_UNLIKELY SCL_DETAIL_UNLIKELY_STD
#else
#define SCL_UNLIKELY SCL_DETAIL_UNLIKELY_FALLBACK
#endif
#endif

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_UNLIKELY),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_UNLIKELY));
// clang-format on

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_LIKELY_EXPR_MSVC(expr) (static_cast<bool>(!!(expr)))
#endif

#ifdef __has_builtin
#if __has_builtin(__builtin_expect)
#define SCL_DETAIL_LIKELY_EXPR_BUILTIN(expr) (static_cast<bool>(__builtin_expect(!!(expr), 1)))
#endif
#endif

#define SCL_DETAIL_LIKELY_EXPR_FALLBACK(expr) (static_cast<bool>(!!(expr)))

#ifndef SCL_LIKELY_EXPR
#ifdef SCL_DETAIL_LIKELY_EXPR_MSVC
#define SCL_LIKELY_EXPR(expr) SCL_DETAIL_LIKELY_EXPR_MSVC(expr)
#elif defined(SCL_DETAIL_LIKELY_EXPR_BUILTIN)
#define SCL_LIKELY_EXPR(expr) SCL_DETAIL_LIKELY_EXPR_BUILTIN(expr)
#else
#define SCL_LIKELY_EXPR(expr) SCL_DETAIL_LIKELY_EXPR_FALLBACK(expr)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_LIKELY_EXPR(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_EXPR_MSVC(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_EXPR_BUILTIN(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_EXPR_FALLBACK(scl_detail_condition))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_LIKELY_EXPR));

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_UNLIKELY_EXPR_MSVC(expr) (static_cast<bool>(!!(expr)))
#endif

#ifdef __has_builtin
#if __has_builtin(__builtin_expect)
#define SCL_DETAIL_UNLIKELY_EXPR_BUILTIN(expr) (static_cast<bool>(__builtin_expect(!!(expr), 0)))
#endif
#endif

#define SCL_DETAIL_UNLIKELY_EXPR_FALLBACK(expr) (static_cast<bool>(!!(expr)))

#ifndef SCL_UNLIKELY_EXPR
#ifdef SCL_DETAIL_UNLIKELY_EXPR_MSVC
#define SCL_UNLIKELY_EXPR(expr) SCL_DETAIL_UNLIKELY_EXPR_MSVC(expr)
#elif defined(SCL_DETAIL_UNLIKELY_EXPR_BUILTIN)
#define SCL_UNLIKELY_EXPR(expr) SCL_DETAIL_UNLIKELY_EXPR_BUILTIN(expr)
#else
#define SCL_UNLIKELY_EXPR(expr) SCL_DETAIL_UNLIKELY_EXPR_FALLBACK(expr)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_UNLIKELY_EXPR(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_EXPR_MSVC(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_EXPR_BUILTIN(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_EXPR_FALLBACK(scl_detail_condition))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_UNLIKELY_EXPR));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_LIKELY
 * @brief The marked branch is the path the program usually takes.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the statement after the macro is the path the program usually
 *   takes (`SCL_LIKELY`) or rarely takes (`SCL_UNLIKELY`). The optimizer may lay out the code for
 *   that path.
 * - **Place:** either macro is written before the statement of a branch of `if` or `else`, or
 *   before a `case` or `default` label, and never inside a condition.
 *
 * | Condition, first match | `SCL_LIKELY` | `SCL_UNLIKELY` |
 * |---|---|---|
 * | `__has_cpp_attribute(likely)`, `__has_cpp_attribute(unlikely)` (C++20) | `[[likely]]` | `[[unlikely]]` |
 * | none of the above | nothing | nothing |
 *
 * The macros expand to `[[likely]]` and `[[unlikely]]` on GCC, Clang and MSVC in C++20.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_LIKELY` | `[[likely]]`, empty | `[[likely]]`, empty | `[[likely]]`, empty |
 * | `SCL_UNLIKELY` | `[[unlikely]]`, empty | `[[unlikely]]`, empty | `[[unlikely]]`, empty |
 *
 * @snippet attribute/likely/attribute_likely_example.cpp likely
 */

/**
 * @def SCL_UNLIKELY
 * @brief The marked branch is the path the program rarely takes.
 * @ingroup scl_utility_attribute
 * @details
 * @copydetails SCL_LIKELY
 */

/**
 * @def SCL_LIKELY_EXPR(expr)
 * @brief The expression `expr` is usually `true`.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** `expr` is usually `true` (`SCL_LIKELY_EXPR`) or usually `false`
 *   (`SCL_UNLIKELY_EXPR`).
 * - **The value:** the value of the expansion is the truth value of `expr`, of the type `bool` on
 *   every compiler.
 * - **MSVC:** no hint is given on MSVC, and only the truth test is left in the expansion.
 * - **Place:** either macro is written in any expression, the condition of `if`, `while` or `?:`
 *   included.
 *
 * | Condition, first match | `SCL_LIKELY_EXPR(expr)` | `SCL_UNLIKELY_EXPR(expr)` |
 * |---|---|---|
 * | MSVC other than clang-cl | `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
 * | `__has_builtin(__builtin_expect)` (GCC, Clang) | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))` |
 * | none of the above | `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
 *
 * The macros expand to `(static_cast<bool>(!!(expr)))` on MSVC, and to the expansions with
 * `__builtin_expect` on GCC and Clang.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_LIKELY_EXPR(expr)` | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
 * | `SCL_UNLIKELY_EXPR(expr)` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
 *
 * @param expr An expression the operator `!` can be applied to.
 *
 * @snippet attribute/likely/attribute_likely_example.cpp likely_expr
 */

/**
 * @def SCL_UNLIKELY_EXPR(expr)
 * @brief The expression `expr` is usually `false`.
 * @ingroup scl_utility_attribute
 * @details
 * @copydetails SCL_LIKELY_EXPR
 */
