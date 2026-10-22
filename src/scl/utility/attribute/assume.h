#pragma once

/**
 * @file
 * @brief Optimizer hint macros: SCL_ASSUME and SCL_UNREACHABLE.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

#ifdef __has_cpp_attribute
#if __cplusplus > 202002L || (defined(_MSVC_LANG) && _MSVC_LANG > 202002L)
#if __has_cpp_attribute(assume)
#define SCL_DETAIL_ASSUME_CXX23(expr) [[assume(expr)]]
#endif
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_ASSUME_MSVC(expr) __assume(expr)
#endif

#ifdef __has_builtin
#if __has_builtin(__builtin_assume)
#define SCL_DETAIL_ASSUME_BUILTIN(expr) __builtin_assume(expr)
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__assume__)
#define SCL_DETAIL_ASSUME_ATTRIBUTE(expr) __attribute__((__assume__(expr)))
#endif
#endif

#ifdef __has_builtin
#if __has_builtin(__builtin_unreachable)
#define SCL_DETAIL_ASSUME_UNREACHABLE(expr) \
    (static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())
#endif
#endif

#define SCL_DETAIL_ASSUME_FALLBACK(expr) ((void)(expr))

#ifndef SCL_ASSUME
#ifdef SCL_DETAIL_ASSUME_CXX23
#define SCL_ASSUME(expr) SCL_DETAIL_ASSUME_CXX23(expr)
#elif defined(SCL_DETAIL_ASSUME_MSVC)
#define SCL_ASSUME(expr) SCL_DETAIL_ASSUME_MSVC(expr)
#elif defined(SCL_DETAIL_ASSUME_BUILTIN)
#define SCL_ASSUME(expr) SCL_DETAIL_ASSUME_BUILTIN(expr)
#elif defined(SCL_DETAIL_ASSUME_ATTRIBUTE)
#define SCL_ASSUME(expr) SCL_DETAIL_ASSUME_ATTRIBUTE(expr)
#elif defined(SCL_DETAIL_ASSUME_UNREACHABLE)
#define SCL_ASSUME(expr) SCL_DETAIL_ASSUME_UNREACHABLE(expr)
#else
#define SCL_ASSUME(expr) SCL_DETAIL_ASSUME_FALLBACK(expr)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_ASSUME(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_ASSUME_CXX23(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_ASSUME_MSVC(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_ASSUME_BUILTIN(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_ASSUME_ATTRIBUTE(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_ASSUME_UNREACHABLE(scl_detail_condition)),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_ASSUME_FALLBACK(scl_detail_condition))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_ASSUME));

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_UNREACHABLE_MSVC() __assume(false)
#endif

#ifdef __has_builtin
#if __has_builtin(__builtin_unreachable)
#define SCL_DETAIL_UNREACHABLE_BUILTIN() __builtin_unreachable()
#endif
#endif

#define SCL_DETAIL_UNREACHABLE_FALLBACK() ((void)0)

#ifndef SCL_UNREACHABLE
#ifdef SCL_DETAIL_UNREACHABLE_MSVC
#define SCL_UNREACHABLE() SCL_DETAIL_UNREACHABLE_MSVC()
#elif defined(SCL_DETAIL_UNREACHABLE_BUILTIN)
#define SCL_UNREACHABLE() SCL_DETAIL_UNREACHABLE_BUILTIN()
#else
#define SCL_UNREACHABLE() SCL_DETAIL_UNREACHABLE_FALLBACK()
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_UNREACHABLE()),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNREACHABLE_MSVC()),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNREACHABLE_BUILTIN()),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNREACHABLE_FALLBACK())),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_UNREACHABLE));
