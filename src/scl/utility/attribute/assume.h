#pragma once

/**
 * @file
 * @brief Optimizer hint macros: SCL_ASSUME and SCL_UNREACHABLE.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_ASSUME(expr)
 * @brief Informs the optimizer that @p expr is always true at this point.
 * @ingroup scl_utility_attribute
 * @details
 * In the first four branches @p expr is @b not evaluated at runtime; in the
 * last two it is.
 * Do NOT pass expressions with side effects.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(assume) (C++23):
 *       @c [[assume(expr)]] — standard form, forward-compatible
 *  2. MSVC other than clang-cl:
 *       @c __assume(expr)
 *  3. @c __has_builtin(__builtin_assume) (Clang):
 *       @c __builtin_assume(expr)
 *  4. @c __has_attribute(assume):
 *       @c __attribute__((assume(expr)))
 *  5. @c __has_builtin(__builtin_unreachable):
 *       @c (static_cast&lt;bool&gt;(expr) ? (void)0 : __builtin_unreachable())
 *  6. Fallback: @c ((void)(expr))
 *
 * @param expr A boolean expression assumed to be always true.
 *
 * @code{.cpp}
 * void process(int* p) {
 *     SCL_ASSUME(p != nullptr);
 *     *p = 42; // optimizer may elide null-check
 * }
 * @endcode
 */

/**
 * @def SCL_UNREACHABLE()
 * @brief Marks the current location as unreachable code.
 * @ingroup scl_utility_attribute
 * @details
 * Invoking this macro in a code path that IS reached is undefined behaviour.
 * Use it to suppress "not all control paths return a value" warnings and
 * to guide the optimizer in dead-code elimination.
 *
 * Detection order:
 *  1. MSVC other than clang-cl:
 *       @c __assume(false)
 *  2. @c __has_builtin(__builtin_unreachable) (GCC, Clang):
 *       @c __builtin_unreachable()
 *  3. Fallback: @c ((void)0)
 *
 * @code{.cpp}
 * int f(Color c) {
 *     switch (c) {
 *     case Color::Red:   return 1;
 *     case Color::Green: return 2;
 *     }
 *     SCL_UNREACHABLE();
 * }
 * @endcode
 */

#ifndef SCL_ASSUME
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(assume)
#define SCL_ASSUME(expr) [[assume(expr)]]
#endif
#endif
#endif

#ifndef SCL_ASSUME
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_ASSUME(expr) __assume(expr)
#endif
#endif

#ifndef SCL_ASSUME
#ifdef __has_builtin
#if __has_builtin(__builtin_assume)
#define SCL_ASSUME(expr) __builtin_assume(expr)
#endif
#endif
#endif

#ifndef SCL_ASSUME
#ifdef __has_attribute
#if __has_attribute(assume)
#define SCL_ASSUME(expr) __attribute__((assume(expr)))
#endif
#endif
#endif

#ifndef SCL_ASSUME
#ifdef __has_builtin
#if __has_builtin(__builtin_unreachable)
#define SCL_ASSUME(expr) (static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())
#endif
#endif
#endif

#ifndef SCL_ASSUME
#define SCL_ASSUME(expr) ((void)(expr))
#endif

#ifndef SCL_UNREACHABLE
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_UNREACHABLE() __assume(false)
#endif
#endif

#ifndef SCL_UNREACHABLE
#ifdef __has_builtin
#if __has_builtin(__builtin_unreachable)
#define SCL_UNREACHABLE() __builtin_unreachable()
#endif
#endif
#endif

#ifndef SCL_UNREACHABLE
#define SCL_UNREACHABLE() ((void)0)
#endif
