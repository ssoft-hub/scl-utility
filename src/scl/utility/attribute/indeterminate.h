#pragma once

/**
 * @file
 * @brief Indeterminate-value annotation macro: SCL_INDETERMINATE.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_INDETERMINATE
 * @brief Explicitly marks a variable as having an indeterminate value,
 *        opting in to uninitialised storage for performance.
 * @ingroup scl_utility_attribute
 * @details
 * Corresponds to the C++26 @c [[indeterminate]] attribute.  Apply it to a
 * variable declaration to tell the compiler that the initial value is
 * intentionally indeterminate — suppressing any mandatory zero- or
 * default-initialisation that would otherwise be inserted.
 *
 * Reading the value before writing to it is @b undefined behaviour.
 * Use only when the variable is unconditionally written before its first read.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(indeterminate) (C++26):
 *       @c [[indeterminate]]
 *  2. Fallback: empty (variable is initialised normally; no UB, no benefit)
 *
 * @code{.cpp}
 * void fill(float* dst, float val, std::size_t n) {
 *     SCL_INDETERMINATE float tmp;  // no zero-init; written unconditionally
 *     tmp = val * 2.0f;
 *     for (std::size_t i = 0; i < n; ++i)
 *         dst[i] = tmp;
 * }
 * @endcode
 */

#ifdef __has_cpp_attribute
#ifndef indeterminate
#if __has_cpp_attribute(indeterminate)
#define SCL_DETAIL_INDETERMINATE_STD [[indeterminate]]
#endif
#endif
#endif

#define SCL_DETAIL_INDETERMINATE_FALLBACK

#ifndef SCL_INDETERMINATE
#ifdef SCL_DETAIL_INDETERMINATE_STD
#define SCL_INDETERMINATE SCL_DETAIL_INDETERMINATE_STD
#else
#define SCL_INDETERMINATE SCL_DETAIL_INDETERMINATE_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_INDETERMINATE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_INDETERMINATE_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_INDETERMINATE_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_INDETERMINATE));
