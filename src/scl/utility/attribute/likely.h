#pragma once

/**
 * @file
 * @brief Branch-prediction hint macros: SCL_LIKELY, SCL_UNLIKELY,
 *        SCL_LIKELY_EXPR, SCL_UNLIKELY_EXPR.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_LIKELY
 * @brief Statement attribute that marks a branch as likely to be taken.
 * @ingroup scl_utility_attribute
 * @details
 * Portable wrapper for the C++20 @c [[likely]] statement attribute.
 * Apply it before a statement in an @c if branch or before a label in a
 * @c switch to guide the optimizer's branch-layout decisions.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(likely) (C++20): @c [[likely]]
 *  2. Fallback: empty (no hint; compiles without effect)
 *
 * @code{.cpp}
 * // if branch
 * if (condition) SCL_LIKELY { fast_path(); }
 *
 * // switch label
 * switch (x) {
 * SCL_LIKELY case 0: ...
 * }
 * @endcode
 */

/**
 * @def SCL_UNLIKELY
 * @brief Statement attribute that marks a branch as unlikely to be taken.
 * @ingroup scl_utility_attribute
 * @details
 * Portable wrapper for the C++20 @c [[unlikely]] statement attribute.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(unlikely) (C++20): @c [[unlikely]]
 *  2. Fallback: empty
 *
 * @code{.cpp}
 * if (error) SCL_UNLIKELY { handle_error(); }
 *
 * switch (x) {
 * SCL_UNLIKELY default: ...
 * }
 * @endcode
 */

/**
 * @def SCL_LIKELY_EXPR(expr)
 * @brief Expression wrapper that hints the optimizer @p expr is usually true.
 * @ingroup scl_utility_attribute
 * @details
 * Wraps a boolean or integer expression and yields its truth value, @c 0 or
 * @c 1. On GCC and Clang the hint is communicated via @c __builtin_expect.
 * On MSVC, which relies on profile-guided optimisation instead, the wrapper
 * carries no hint and only the truth test remains.
 *
 * Detection order:
 *  1. MSVC other than clang-cl: @c (!!(expr))
 *  2. @c __has_builtin(__builtin_expect) (GCC, Clang):
 *       @c __builtin_expect(!!(expr), 1)
 *  3. Fallback: @c (!!(expr))
 *
 * @param expr An expression convertible to @c bool (or @c int).
 *
 * @code{.cpp}
 * if (SCL_LIKELY_EXPR(ptr != nullptr)) {
 *     // fast path — branch predictor biased here
 * }
 * @endcode
 */

/**
 * @def SCL_UNLIKELY_EXPR(expr)
 * @brief Expression wrapper that hints the optimizer @p expr is usually false.
 * @ingroup scl_utility_attribute
 * @details
 * Like @ref SCL_LIKELY_EXPR but biases the branch predictor toward the
 * false (zero) outcome.
 *
 * Detection order:
 *  1. MSVC other than clang-cl: @c (!!(expr))
 *  2. @c __has_builtin(__builtin_expect) (GCC, Clang):
 *       @c __builtin_expect(!!(expr), 0)
 *  3. Fallback: @c (!!(expr))
 *
 * @param expr An expression convertible to @c bool (or @c int).
 *
 * @code{.cpp}
 * if (SCL_UNLIKELY_EXPR(error_code != 0)) {
 *     handle_error(error_code);
 * }
 * @endcode
 */

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

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_LIKELY),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIKELY_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_LIKELY));

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

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_UNLIKELY),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_UNLIKELY));

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
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_LIKELY_EXPR(scl_detail_condition)),
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
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_UNLIKELY_EXPR(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_EXPR_MSVC(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_EXPR_BUILTIN(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNLIKELY_EXPR_FALLBACK(scl_detail_condition))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_UNLIKELY_EXPR));
