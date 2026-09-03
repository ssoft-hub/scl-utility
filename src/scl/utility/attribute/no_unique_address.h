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
 *  1. @c __has_cpp_attribute(no_unique_address) -> @c [[no_unique_address]]
 *  2. @c __has_cpp_attribute(msvc::no_unique_address) -> @c [[msvc::no_unique_address]]
 *  3. Fallback: empty (the empty member is not collapsed)
 *
 * The target ABI decides which spelling is available, not the compiler brand. Targeting
 * the MSVC ABI, step 1 returns 0 and step 2 picks up the vendor form, on MSVC, on
 * clang-cl and on Clang alike. Only the vendor form collapses the empty member
 * there. Targeting the Itanium ABI, step 1 answers and step 2 is never reached.
 *
 * @code{.cpp}
 * struct Empty {};
 * struct S { SCL_NO_UNIQUE_ADDRESS Empty e; int value; };
 * static_assert(sizeof(S) == sizeof(int));
 * @endcode
 */
#ifndef SCL_NO_UNIQUE_ADDRESS
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(no_unique_address)
#define SCL_NO_UNIQUE_ADDRESS [[no_unique_address]]
#elif __has_cpp_attribute(msvc::no_unique_address)
#define SCL_NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#endif
#endif
#endif

#ifndef SCL_NO_UNIQUE_ADDRESS
#define SCL_NO_UNIQUE_ADDRESS
#endif
