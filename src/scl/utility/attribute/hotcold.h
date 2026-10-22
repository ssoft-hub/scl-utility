#pragma once

/**
 * @file
 * @brief Function-frequency hint macros: SCL_HOT and SCL_COLD.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_HOT
 * @brief Marks a function as called frequently (hot path).
 * @ingroup scl_utility_attribute
 * @details
 * Instructs the compiler to optimise the function more aggressively and
 * place it in a section of the binary that improves instruction-cache
 * locality for hot code. A branch toward a @c SCL_HOT call keeps the
 * probability it would have had unannotated; only @ref SCL_COLD biases one.
 *
 * Detection order:
 *  1. MSVC other than clang-cl: empty (no equivalent)
 *  2. @c __has_cpp_attribute(gnu::hot) (GCC, Clang):
 *       @c [[gnu::hot]]
 *  3. @c __has_attribute(hot) (older GCC and Clang):
 *       @c __attribute__((hot))
 *  4. Fallback: empty (no hint)
 *
 * @code{.cpp}
 * SCL_HOT void dispatch(const Event& e);
 *
 * SCL_HOT int lookup(std::uint32_t key);
 * @endcode
 */

/**
 * @def SCL_COLD
 * @brief Marks a function as called rarely (cold path).
 * @ingroup scl_utility_attribute
 * @details
 * Instructs the compiler to optimise the function for size rather than
 * speed and to place it in a cold section of the binary, away from hot
 * code. Branches toward a @c SCL_COLD call site are implicitly biased as
 * unlikely. Combines naturally with @ref SCL_NOINLINE and @ref SCL_NORETURN.
 *
 * Detection order:
 *  1. MSVC other than clang-cl: empty (no equivalent)
 *  2. @c __has_cpp_attribute(gnu::cold) (GCC, Clang):
 *       @c [[gnu::cold]]
 *  3. @c __has_attribute(cold) (older GCC and Clang):
 *       @c __attribute__((cold))
 *  4. Fallback: empty (no hint)
 *
 * @code{.cpp}
 * SCL_COLD SCL_NOINLINE void report_oom(std::size_t requested);
 *
 * SCL_NORETURN SCL_COLD void fatal(const char* msg);
 * @endcode
 */

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_HOT_MSVC
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__hot__)
#define SCL_DETAIL_HOT_GNU [[__gnu__::__hot__]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__hot__)
#define SCL_DETAIL_HOT_ATTRIBUTE __attribute__((__hot__))
#endif
#endif

#define SCL_DETAIL_HOT_FALLBACK

#ifndef SCL_HOT
#ifdef SCL_DETAIL_HOT_MSVC
#define SCL_HOT SCL_DETAIL_HOT_MSVC
#elif defined(SCL_DETAIL_HOT_GNU)
#define SCL_HOT SCL_DETAIL_HOT_GNU
#elif defined(SCL_DETAIL_HOT_ATTRIBUTE)
#define SCL_HOT SCL_DETAIL_HOT_ATTRIBUTE
#else
#define SCL_HOT SCL_DETAIL_HOT_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_HOT),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_GNU),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_HOT));

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_COLD_MSVC
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__cold__)
#define SCL_DETAIL_COLD_GNU [[__gnu__::__cold__]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__cold__)
#define SCL_DETAIL_COLD_ATTRIBUTE __attribute__((__cold__))
#endif
#endif

#define SCL_DETAIL_COLD_FALLBACK

#ifndef SCL_COLD
#ifdef SCL_DETAIL_COLD_MSVC
#define SCL_COLD SCL_DETAIL_COLD_MSVC
#elif defined(SCL_DETAIL_COLD_GNU)
#define SCL_COLD SCL_DETAIL_COLD_GNU
#elif defined(SCL_DETAIL_COLD_ATTRIBUTE)
#define SCL_COLD SCL_DETAIL_COLD_ATTRIBUTE
#else
#define SCL_COLD SCL_DETAIL_COLD_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_COLD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_GNU),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_COLD));
