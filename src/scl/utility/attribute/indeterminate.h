#pragma once

/**
 * @file
 * @brief Indeterminate-value annotation macro: SCL_INDETERMINATE.
 * @ingroup scl_utility_attribute
 */

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

#ifndef SCL_INDETERMINATE
#if __has_cpp_attribute(indeterminate)
#define SCL_INDETERMINATE [[indeterminate]]
#else
#define SCL_INDETERMINATE
#endif
#endif
