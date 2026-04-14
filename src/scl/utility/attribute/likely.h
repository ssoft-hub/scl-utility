#pragma once

/**
 * @file
 * @brief Branch-prediction hint macros: SCL_LIKELY, SCL_UNLIKELY,
 *        SCL_LIKELY_EXPR, SCL_UNLIKELY_EXPR.
 * @ingroup scl_utility_attribute
 */

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
 * Wraps a boolean or integer expression and returns its value unchanged.
 * On GCC and Clang the hint is communicated via @c __builtin_expect.
 * On MSVC (which uses profile-guided optimisation) the expression passes
 * through unmodified.
 *
 * Detection order:
 *  1. @c __has_builtin(__builtin_expect) (GCC, Clang):
 *       @c __builtin_expect(!!(expr), 1)
 *  2. Fallback: @c (!!(expr))
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
 *  1. @c __has_builtin(__builtin_expect) (GCC, Clang):
 *       @c __builtin_expect(!!(expr), 0)
 *  2. Fallback: @c (!!(expr))
 *
 * @param expr An expression convertible to @c bool (or @c int).
 *
 * @code{.cpp}
 * if (SCL_UNLIKELY_EXPR(error_code != 0)) {
 *     handle_error(error_code);
 * }
 * @endcode
 */

#ifndef SCL_LIKELY
#if __has_cpp_attribute(likely)
#define SCL_LIKELY [[likely]]
#else
#define SCL_LIKELY
#endif
#endif

#ifndef SCL_UNLIKELY
#if __has_cpp_attribute(unlikely)
#define SCL_UNLIKELY [[unlikely]]
#else
#define SCL_UNLIKELY
#endif
#endif

#ifndef SCL_LIKELY_EXPR
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_LIKELY_EXPR(expr) (!!(expr))
#define SCL_UNLIKELY_EXPR(expr) (!!(expr))
#elif defined(__has_builtin) && __has_builtin(__builtin_expect)
#define SCL_LIKELY_EXPR(expr) __builtin_expect(!!(expr), 1)
#define SCL_UNLIKELY_EXPR(expr) __builtin_expect(!!(expr), 0)
#else
#define SCL_LIKELY_EXPR(expr) (!!(expr))
#define SCL_UNLIKELY_EXPR(expr) (!!(expr))
#endif
#endif

#ifndef SCL_UNLIKELY_EXPR
#define SCL_UNLIKELY_EXPR(expr) (!!(expr))
#endif
