#pragma once

/**
 * @file
 * @brief Portable [[no_unique_address]] attribute macro.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_NO_UNIQUE_ADDRESS
 * @brief Portable [[no_unique_address]] (C++20).
 * @ingroup scl_utility_attribute
 * @details
 * Detection uses @c __has_cpp_attribute to select the right spelling:
 *  1. @c __has_cpp_attribute(no_unique_address) &rarr; @c [[no_unique_address]]
 *  2. @c __has_cpp_attribute(msvc::no_unique_address) &rarr; @c [[msvc::no_unique_address]]
 *  3. Fallback: empty (no layout optimisation; EBO not guaranteed)
 *
 * MSVC &lt; 19.30 (VS&nbsp;2019 &le;&nbsp;16.9) does not recognise the standard
 * spelling, so step&nbsp;1 returns 0 and step&nbsp;2 picks up the vendor form.
 * Clang-cl supports the standard attribute; @c __has_cpp_attribute returns
 * non-zero for it and the vendor form is never needed.
 *
 * @code{.cpp}
 * struct Empty {};
 * struct S { SCL_NO_UNIQUE_ADDRESS Empty e; int value; };
 * static_assert(sizeof(S) == sizeof(int));
 * @endcode
 */
#ifndef SCL_NO_UNIQUE_ADDRESS
#if __has_cpp_attribute(no_unique_address)
#define SCL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
#define SCL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define SCL_NO_UNIQUE_ADDRESS
#endif
#endif
