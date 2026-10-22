#pragma once

/**
 * @file
 * @brief Warn-on-discard annotation macros: SCL_NODISCARD and SCL_NODISCARD_MSG.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_NODISCARD
 * @brief Warns if the return value of a function (or an object of a type) is
 *        discarded by the caller.
 * @ingroup scl_utility_attribute
 * @details
 * Apply to a function declaration or to a class/struct/enum declaration.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(nodiscard) ≥ 201907L (C++20):
 *       @c [[nodiscard]]
 *  2. @c __has_cpp_attribute(nodiscard) (C++17):
 *       @c [[nodiscard]]
 *  3. @c __has_attribute(warn_unused_result) (older GCC and Clang):
 *       @c __attribute__((warn_unused_result))
 *  4. Fallback: empty (no warning emitted)
 *
 * @code{.cpp}
 * SCL_NODISCARD bool try_connect(const char* host);
 *
 * struct SCL_NODISCARD ErrorCode { int value; };
 * @endcode
 */

/**
 * @def SCL_NODISCARD_MSG(msg)
 * @brief Like @ref SCL_NODISCARD but includes a custom message in the diagnostic.
 * @ingroup scl_utility_attribute
 * @details
 * The message @p msg is a string literal describing why the return value must
 * not be ignored. On compilers where the message form is unavailable the macro
 * falls back to @ref SCL_NODISCARD (warning without message).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(nodiscard) ≥ 201907L (C++20):
 *       @c [[nodiscard(msg)]]
 *  2. @c __has_cpp_attribute(nodiscard) (C++17) or earlier fallback:
 *       @c SCL_NODISCARD (message silently dropped)
 *
 * @param msg A string literal stating the discard consequence.
 *
 * @code{.cpp}
 * SCL_NODISCARD_MSG("ignoring the error code leaks resources")
 * ErrorCode write(int fd, const void* buf, std::size_t n);
 * @endcode
 */

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(nodiscard)
#define SCL_DETAIL_NODISCARD_STD [[nodiscard]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__warn_unused_result__)
#define SCL_DETAIL_NODISCARD_ATTRIBUTE __attribute__((__warn_unused_result__))
#endif
#endif

#define SCL_DETAIL_NODISCARD_FALLBACK

#ifdef SCL_NODISCARD
#define SCL_DETAIL_NODISCARD_GIVEN
#endif

#ifndef SCL_NODISCARD
#ifdef SCL_DETAIL_NODISCARD_STD
#define SCL_NODISCARD SCL_DETAIL_NODISCARD_STD
#elif defined(SCL_DETAIL_NODISCARD_ATTRIBUTE)
#define SCL_NODISCARD SCL_DETAIL_NODISCARD_ATTRIBUTE
#else
#define SCL_NODISCARD SCL_DETAIL_NODISCARD_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_NODISCARD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_NODISCARD));

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(nodiscard) >= 201907L
#define SCL_DETAIL_NODISCARD_MSG_MESSAGE(msg) [[nodiscard(msg)]]
#endif
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(nodiscard)
#define SCL_DETAIL_NODISCARD_MSG_STD(msg) [[nodiscard]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__warn_unused_result__)
#define SCL_DETAIL_NODISCARD_MSG_ATTRIBUTE(msg) __attribute__((__warn_unused_result__))
#endif
#endif

#define SCL_DETAIL_NODISCARD_MSG_FALLBACK(msg) SCL_NODISCARD

#ifndef SCL_NODISCARD_MSG
#ifdef SCL_DETAIL_NODISCARD_GIVEN
#define SCL_NODISCARD_MSG(msg) SCL_DETAIL_NODISCARD_MSG_FALLBACK(msg)
#elif defined(SCL_DETAIL_NODISCARD_MSG_MESSAGE)
#define SCL_NODISCARD_MSG(msg) SCL_DETAIL_NODISCARD_MSG_MESSAGE(msg)
#elif defined(SCL_DETAIL_NODISCARD_MSG_STD)
#define SCL_NODISCARD_MSG(msg) SCL_DETAIL_NODISCARD_MSG_STD(msg)
#elif defined(SCL_DETAIL_NODISCARD_MSG_ATTRIBUTE)
#define SCL_NODISCARD_MSG(msg) SCL_DETAIL_NODISCARD_MSG_ATTRIBUTE(msg)
#else
#define SCL_NODISCARD_MSG(msg) SCL_DETAIL_NODISCARD_MSG_FALLBACK(msg)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_NODISCARD_MSG("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_MSG_MESSAGE("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_MSG_STD("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_MSG_ATTRIBUTE("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NODISCARD_MSG_FALLBACK("scl_detail_message"))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_NODISCARD_MSG));
