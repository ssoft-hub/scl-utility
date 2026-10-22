#pragma once

/**
 * @file
 * @brief Suppress unused-entity warnings: SCL_MAYBE_UNUSED.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_MAYBE_UNUSED
 * @brief Suppresses compiler warnings about unused variables, parameters,
 *        functions, and types.
 * @ingroup scl_utility_attribute
 * @details
 * Apply to any named entity that may legitimately go unused depending on
 * build configuration (e.g., debug-only variables, platform-specific params,
 * assert-only values).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(maybe_unused) (C++17):
 *       @c [[maybe_unused]]
 *  2. @c __has_attribute(unused) (older GCC and Clang):
 *       @c __attribute__((unused))
 *  3. Fallback: empty (warning may still fire)
 *
 * @code{.cpp}
 * void process(int value, SCL_MAYBE_UNUSED int debug_flags) {
 *     // debug_flags used only in debug builds
 *     assert(debug_flags >= 0);
 *     (void)value;
 * }
 *
 * SCL_MAYBE_UNUSED static void helper() { ... }
 * @endcode
 */

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(maybe_unused)
#define SCL_DETAIL_MAYBE_UNUSED_STD [[maybe_unused]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__unused__)
#define SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE __attribute__((__unused__))
#endif
#endif

#define SCL_DETAIL_MAYBE_UNUSED_FALLBACK

#ifndef SCL_MAYBE_UNUSED
#ifdef SCL_DETAIL_MAYBE_UNUSED_STD
#define SCL_MAYBE_UNUSED SCL_DETAIL_MAYBE_UNUSED_STD
#elif defined(SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE)
#define SCL_MAYBE_UNUSED SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE
#else
#define SCL_MAYBE_UNUSED SCL_DETAIL_MAYBE_UNUSED_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_MAYBE_UNUSED),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_MAYBE_UNUSED_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_MAYBE_UNUSED_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_MAYBE_UNUSED));
