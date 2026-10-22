#pragma once

/**
 * @file
 * @brief Non-returning function annotation macro: SCL_NORETURN.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_NORETURN
 * @brief Marks a function that never returns to its caller.
 * @ingroup scl_utility_attribute
 * @details
 * Apply before the return type. The compiler may assume that any call to a
 * @c SCL_NORETURN function is a dead end and optimize accordingly.
 * If an annotated function does return, the behaviour is **undefined**.
 *
 * Typical uses: @c std::terminate wrappers, @c throw helpers,
 * infinite-loop entry points.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(noreturn) (C++11):
 *       @c [[noreturn]] — standard form
 *  2. MSVC other than clang-cl:
 *       @c __declspec(noreturn)
 *  3. @c __has_attribute(noreturn) (GCC, Clang):
 *       @c __attribute__((noreturn))
 *  4. Fallback: empty (no annotation; compiler warning may fire)
 *
 * @code{.cpp}
 * SCL_NORETURN void fatal(const char* msg);
 *
 * int divide(int a, int b) {
 *     if (b == 0) fatal("division by zero");
 *     return a / b;  // no "not all paths return" warning
 * }
 * @endcode
 */

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

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_NORETURN),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NORETURN_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_NORETURN));
