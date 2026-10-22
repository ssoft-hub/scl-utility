#pragma once

/**
 * @file
 * @brief Deprecation annotation macros: SCL_DEPRECATED and SCL_DEPRECATED_MSG.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_DEPRECATED
 * @brief Marks a declaration as deprecated.
 * @ingroup scl_utility_attribute
 * @details
 * Causes the compiler to emit a warning whenever the annotated entity is used.
 * Apply before a function, a variable or a typedef, and after the @c class,
 * @c struct or @c enum keyword: @c struct @c SCL_DEPRECATED @c Name.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(deprecated) (C++14):
 *       @c [[deprecated]]
 *  2. MSVC other than clang-cl:
 *       @c __declspec(deprecated)
 *  3. @c __has_attribute(deprecated) (GCC, Clang):
 *       @c __attribute__((deprecated))
 *  4. Fallback: empty (no annotation; no warning emitted)
 *
 * @code{.cpp}
 * SCL_DEPRECATED void old_api();
 *
 * struct SCL_DEPRECATED LegacyHandle { int fd; };
 * @endcode
 */

/**
 * @def SCL_DEPRECATED_MSG(msg)
 * @brief Marks a declaration as deprecated with a custom message.
 * @ingroup scl_utility_attribute
 * @details
 * Like @ref SCL_DEPRECATED but includes a string literal @p msg in the
 * compiler diagnostic. On compilers that do not support message-bearing
 * deprecation the macro falls back to @ref SCL_DEPRECATED (warning without
 * message).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(deprecated) (C++14):
 *       @c [[deprecated(msg)]]
 *  2. MSVC other than clang-cl:
 *       @c __declspec(deprecated(msg))
 *  3. @c __has_attribute(deprecated) (GCC, Clang):
 *       @c __attribute__((deprecated(msg)))
 *  4. Fallback: empty
 *
 * @param msg A string literal describing the deprecation reason.
 *
 * @code{.cpp}
 * SCL_DEPRECATED_MSG("Use new_api() instead") void old_api();
 * @endcode
 */

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(deprecated)
#define SCL_DETAIL_DEPRECATED_STD [[deprecated]]
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_DEPRECATED_MSVC __declspec(deprecated)
#endif

#ifdef __has_attribute
#if __has_attribute(__deprecated__)
#define SCL_DETAIL_DEPRECATED_ATTRIBUTE __attribute__((__deprecated__))
#endif
#endif

#define SCL_DETAIL_DEPRECATED_FALLBACK

#ifdef SCL_DEPRECATED
#define SCL_DETAIL_DEPRECATED_GIVEN
#endif

#ifndef SCL_DEPRECATED
#ifdef SCL_DETAIL_DEPRECATED_STD
#define SCL_DEPRECATED SCL_DETAIL_DEPRECATED_STD
#elif defined(SCL_DETAIL_DEPRECATED_MSVC)
#define SCL_DEPRECATED SCL_DETAIL_DEPRECATED_MSVC
#elif defined(SCL_DETAIL_DEPRECATED_ATTRIBUTE)
#define SCL_DEPRECATED SCL_DETAIL_DEPRECATED_ATTRIBUTE
#else
#define SCL_DEPRECATED SCL_DETAIL_DEPRECATED_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DEPRECATED),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_DEPRECATED));

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(deprecated)
#define SCL_DETAIL_DEPRECATED_MSG_STD(msg) [[deprecated(msg)]]
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_DEPRECATED_MSG_MSVC(msg) __declspec(deprecated(msg))
#endif

#ifdef __has_attribute
#if __has_attribute(__deprecated__)
#define SCL_DETAIL_DEPRECATED_MSG_ATTRIBUTE(msg) __attribute__((__deprecated__(msg)))
#endif
#endif

#define SCL_DETAIL_DEPRECATED_MSG_FALLBACK(msg) SCL_DEPRECATED

#ifndef SCL_DEPRECATED_MSG
#ifdef SCL_DETAIL_DEPRECATED_GIVEN
#define SCL_DEPRECATED_MSG(msg) SCL_DETAIL_DEPRECATED_MSG_FALLBACK(msg)
#elif defined(SCL_DETAIL_DEPRECATED_MSG_STD)
#define SCL_DEPRECATED_MSG(msg) SCL_DETAIL_DEPRECATED_MSG_STD(msg)
#elif defined(SCL_DETAIL_DEPRECATED_MSG_MSVC)
#define SCL_DEPRECATED_MSG(msg) SCL_DETAIL_DEPRECATED_MSG_MSVC(msg)
#elif defined(SCL_DETAIL_DEPRECATED_MSG_ATTRIBUTE)
#define SCL_DEPRECATED_MSG(msg) SCL_DETAIL_DEPRECATED_MSG_ATTRIBUTE(msg)
#else
#define SCL_DEPRECATED_MSG(msg) SCL_DETAIL_DEPRECATED_MSG_FALLBACK(msg)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DEPRECATED_MSG("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_STD("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_MSVC("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_ATTRIBUTE("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_FALLBACK("scl_detail_message"))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_DEPRECATED_MSG));
