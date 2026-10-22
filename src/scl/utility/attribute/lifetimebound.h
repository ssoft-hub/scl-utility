#pragma once

/**
 * @file
 * @brief Lifetime-bound parameter annotation macro: SCL_LIFETIMEBOUND.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_LIFETIMEBOUND
 * @brief Marks a function parameter (or implicit @c *this) as
 *        lifetime-bound to the return value.
 * @ingroup scl_utility_attribute
 * @details
 * When applied to a parameter, the compiler warns if the returned
 * reference or pointer outlives the annotated argument (dangling
 * reference detection at compile time).
 *
 * **Placement** — after the parameter declaration or after the
 * cv-qualifier of a member function:
 * @code{.cpp}
 * // Parameter
 * std::string_view first(const std::string& s SCL_LIFETIMEBOUND);
 *
 * // Member function (*this is lifetimebound to return value)
 * class Builder {
 *     Builder& set(int v) SCL_LIFETIMEBOUND;
 * };
 * @endcode
 *
 * Detection order:
 *  1. MSVC other than clang-cl: empty (no equivalent)
 *  2. @c __has_cpp_attribute(clang::lifetimebound) (Clang):
 *       @c [[clang::lifetimebound]]
 *  3. @c __has_attribute(lifetimebound) (older Clang):
 *       @c __attribute__((lifetimebound))
 *  4. Fallback: empty (no lifetime analysis; no warning)
 *
 * @note GCC and MSVC do not currently support this attribute; there the macro
 *       expands to nothing.
 *
 * @code{.cpp}
 * const int& clamp(const int& v SCL_LIFETIMEBOUND,
 *                  const int& lo SCL_LIFETIMEBOUND,
 *                  const int& hi SCL_LIFETIMEBOUND);
 *
 * // Dangling reference — Clang warns with SCL_LIFETIMEBOUND:
 * const int& r = clamp(42, 0, 100);  // temporary args destroyed
 * @endcode
 */

#if defined(_MSC_VER) && !defined(__clang__)
#ifdef __has_cpp_attribute
#if !defined(msvc) && !defined(lifetimebound)
#if __has_cpp_attribute(msvc::lifetimebound)
#define SCL_DETAIL_LIFETIMEBOUND_MSVC [[msvc::lifetimebound]]
#endif
#endif
#endif
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(_Clang::__lifetimebound__)
#define SCL_DETAIL_LIFETIMEBOUND_CLANG [[_Clang::__lifetimebound__]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__lifetimebound__)
#define SCL_DETAIL_LIFETIMEBOUND_ATTRIBUTE __attribute__((__lifetimebound__))
#endif
#endif

#define SCL_DETAIL_LIFETIMEBOUND_FALLBACK

#ifndef SCL_LIFETIMEBOUND
#if defined(_MSC_VER) && !defined(__clang__) && (defined(msvc) || defined(lifetimebound))
#error "No spelling of SCL_LIFETIMEBOUND remains for MSVC under the macro msvc or lifetimebound; SCL_LIFETIMEBOUND must be defined before the header as an empty macro"
#endif
#endif

#ifndef SCL_LIFETIMEBOUND
#ifdef SCL_DETAIL_LIFETIMEBOUND_MSVC
#define SCL_LIFETIMEBOUND SCL_DETAIL_LIFETIMEBOUND_MSVC
#elif defined(SCL_DETAIL_LIFETIMEBOUND_CLANG)
#define SCL_LIFETIMEBOUND SCL_DETAIL_LIFETIMEBOUND_CLANG
#elif defined(SCL_DETAIL_LIFETIMEBOUND_ATTRIBUTE)
#define SCL_LIFETIMEBOUND SCL_DETAIL_LIFETIMEBOUND_ATTRIBUTE
#else
#define SCL_LIFETIMEBOUND SCL_DETAIL_LIFETIMEBOUND_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_LIFETIMEBOUND),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_CLANG),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_LIFETIMEBOUND));
