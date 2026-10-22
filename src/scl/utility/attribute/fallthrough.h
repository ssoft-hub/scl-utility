#pragma once

/**
 * @file
 * @brief Intentional switch fall-through annotation macro: SCL_FALLTHROUGH.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_FALLTHROUGH
 * @brief Suppresses the implicit fall-through warning in a @c switch statement.
 * @ingroup scl_utility_attribute
 * @details
 * Place as a statement at the end of a @c case body that intentionally falls
 * through to the next @c case. This silences @c -Wimplicit-fallthrough and the
 * equivalent MSVC warning C26819.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(fallthrough) (C++17):
 *       @c [[fallthrough]]
 *  2. @c __has_attribute(fallthrough) (older GCC and Clang):
 *       @c __attribute__((fallthrough))
 *  3. Fallback: @c ((void)0) — silent no-op
 *
 * @code{.cpp}
 * switch (code) {
 * case 1:
 *     prepare();
 *     SCL_FALLTHROUGH;
 * case 2:
 *     execute();
 *     break;
 * }
 * @endcode
 */

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(fallthrough)
#define SCL_DETAIL_FALLTHROUGH_STD [[fallthrough]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__fallthrough__)
#define SCL_DETAIL_FALLTHROUGH_ATTRIBUTE __attribute__((__fallthrough__))
#endif
#endif

#define SCL_DETAIL_FALLTHROUGH_FALLBACK ((void)0)

#ifndef SCL_FALLTHROUGH
#ifdef SCL_DETAIL_FALLTHROUGH_STD
#define SCL_FALLTHROUGH SCL_DETAIL_FALLTHROUGH_STD
#elif defined(SCL_DETAIL_FALLTHROUGH_ATTRIBUTE)
#define SCL_FALLTHROUGH SCL_DETAIL_FALLTHROUGH_ATTRIBUTE
#else
#define SCL_FALLTHROUGH SCL_DETAIL_FALLTHROUGH_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_FALLTHROUGH),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FALLTHROUGH_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FALLTHROUGH_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FALLTHROUGH_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_FALLTHROUGH));
