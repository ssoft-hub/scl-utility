#pragma once

/**
 * @file
 * @brief The macros `SCL_ASSUME` and `SCL_UNREACHABLE`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_ASSUME(expr)
 * @brief The optimizer takes the condition `expr` as true where the macro is written.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** `expr` is `true` each time execution reaches the statement.
 *   The optimizer may drop a branch or a check that the condition makes redundant.
 * - **A false promise:** the behaviour of the program is undefined where `expr` is `false` at that
 *   point and the expansion is taken from one of the first five rows of the table.
 * - **The argument:** `expr` is not evaluated under the first four rows of the table and is
 *   evaluated under the last two, so an expression with no side effects should be passed.
 * - **`SCL_ASSUME(false)`:** the promise is that of `SCL_UNREACHABLE`, yet Clang still reports
 *   the warning `-Wreturn-type` where a function ends after `[[assume(false)]]`.
 *   `SCL_UNREACHABLE()` should be written for that promise.
 * - **Place:** the macro is written as a statement of its own, closed by a semicolon.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | `__has_cpp_attribute(assume)`, where `__cplusplus` or `_MSVC_LANG` is above `202002L` | `[[assume(expr)]]` |
 * | MSVC other than clang-cl | `__assume(expr)` |
 * | `__has_builtin(__builtin_assume)` (Clang) | `__builtin_assume(expr)` |
 * | `__has_attribute(__assume__)` | `__attribute__((__assume__(expr)))` |
 * | `__has_builtin(__builtin_unreachable)` | `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())` |
 * | none of the above | `((void)(expr))` |
 *
 * The macro expands to `__assume(expr)` on MSVC, to `__builtin_assume(expr)` on Clang and to
 * `__attribute__((__assume__(expr)))` on GCC in C++20. It expands to `[[assume(expr)]]` only where
 * `__cplusplus` or `_MSVC_LANG` is above `202002L` and `__has_cpp_attribute(assume)` is not `0`, as
 * on GCC 13 and Clang 22 under `-std=c++23`. MSVC 19.44 reports `0` for `assume` under
 * `/std:c++latest`, so the macro keeps `__assume(expr)` there.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_ASSUME(expr)` | `__attribute__((__assume__(expr)))`, `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())`, `((void)(expr))` | `__builtin_assume(expr)`, `__attribute__((__assume__(expr)))`, `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())`, `((void)(expr))` | `__assume(expr)`, `((void)(expr))` |
 *
 * `[[assume(expr)]]` is admitted as well in C++23 on GCC and on Clang 19 and later.
 *
 * @param expr The condition taken as `true`.
 *
 * @snippet attribute/assume/attribute_assume_example.cpp assume
 */

/**
 * @def SCL_UNREACHABLE()
 * @brief The optimizer takes the point of the macro as never reached.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** execution never reaches the statement. The compiler stops
 *   warning that a function returning a value may end without one.
 * - **A false promise:** the behaviour of the program is undefined where execution reaches
 *   the statement and the expansion is taken from one of the first two rows of the table.
 * - **Place:** the macro is written as a statement of its own, closed by a semicolon.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | MSVC other than clang-cl | `__assume(false)` |
 * | `__has_builtin(__builtin_unreachable)` (GCC, Clang) | `__builtin_unreachable()` |
 * | none of the above | `((void)0)` |
 *
 * The macro expands to `__assume(false)` on MSVC and to `__builtin_unreachable()` on GCC and Clang.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_UNREACHABLE()` | `__builtin_unreachable()`, `((void)0)` | `__builtin_unreachable()`, `((void)0)` | `__assume(false)`, `((void)0)` |
 *
 * @snippet attribute/assume/attribute_assume_example.cpp unreachable
 */
