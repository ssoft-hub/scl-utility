#pragma once

/**
 * @file
 * @brief The macro `SCL_NO_UNIQUE_ADDRESS`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_NO_UNIQUE_ADDRESS
 * @brief The marked empty member may share its address with another member.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the member needs no address distinct from the other members.
 *   The compiler may place an empty member at the address of another, and then no byte of the class
 *   is given to it.
 * - **The spelling by target:** `__has_cpp_attribute(no_unique_address)` is `0` on MSVC, and on
 *   Clang 22, clang-cl included, for the MSVC ABI. The layout changes there only under
 *   `[[msvc::no_unique_address]]`. GCC and Clang take the standard spelling for the Itanium ABI.
 * - **Two members of one type:** two members of one type keep distinct addresses under the macro,
 *   yet either may still share its address with a member of another type.
 * - **Place:** the macro is written before the declaration of a non-static data member.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | `__has_cpp_attribute(no_unique_address)` | `[[no_unique_address]]` |
 * | `__has_cpp_attribute(msvc::no_unique_address)`, `msvc` not a macro | `[[msvc::no_unique_address]]` |
 * | none of the above | nothing, and the empty member takes a byte |
 *
 * The macro expands to `[[no_unique_address]]` on GCC and to `[[msvc::no_unique_address]]` on MSVC.
 * It expands to `[[no_unique_address]]` on Clang 22 for the Itanium ABI, and to
 * `[[msvc::no_unique_address]]` for the MSVC ABI.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_NO_UNIQUE_ADDRESS` | `[[no_unique_address]]`, empty | `[[no_unique_address]]`, empty; for the MSVC ABI `[[msvc::no_unique_address]]`, empty | `[[msvc::no_unique_address]]`, empty |
 *
 * @snippet attribute/no_unique_address/attribute_no_unique_address_example.cpp no_unique_address
 */
