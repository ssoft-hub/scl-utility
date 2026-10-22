#pragma once

/**
 * @file
 * @brief Portable [[no_unique_address]] attribute macro.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(no_unique_address)
#define SCL_DETAIL_NO_UNIQUE_ADDRESS_STD [[no_unique_address]]
#endif
#endif

#ifdef __has_cpp_attribute
#ifndef msvc
#if __has_cpp_attribute(msvc::no_unique_address)
#define SCL_DETAIL_NO_UNIQUE_ADDRESS_MSVC [[msvc::no_unique_address]]
#endif
#endif
#endif

#define SCL_DETAIL_NO_UNIQUE_ADDRESS_FALLBACK

#ifndef SCL_NO_UNIQUE_ADDRESS
#if defined(_MSC_VER) && defined(msvc) && !defined(SCL_DETAIL_NO_UNIQUE_ADDRESS_STD)
#error "No spelling of SCL_NO_UNIQUE_ADDRESS remains for the MSVC ABI under the macro msvc; SCL_NO_UNIQUE_ADDRESS must be defined before the header as an empty macro, in every translation unit of the program"
#endif
#endif

#ifndef SCL_NO_UNIQUE_ADDRESS
#ifdef SCL_DETAIL_NO_UNIQUE_ADDRESS_STD
#define SCL_NO_UNIQUE_ADDRESS SCL_DETAIL_NO_UNIQUE_ADDRESS_STD
#elif defined(SCL_DETAIL_NO_UNIQUE_ADDRESS_MSVC)
#define SCL_NO_UNIQUE_ADDRESS SCL_DETAIL_NO_UNIQUE_ADDRESS_MSVC
#else
#define SCL_NO_UNIQUE_ADDRESS SCL_DETAIL_NO_UNIQUE_ADDRESS_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_NO_UNIQUE_ADDRESS),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NO_UNIQUE_ADDRESS_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NO_UNIQUE_ADDRESS_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NO_UNIQUE_ADDRESS_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_NO_UNIQUE_ADDRESS));
