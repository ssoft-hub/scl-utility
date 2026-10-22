#pragma once

/**
 * @file
 * @brief The macros `SCL_NODISCARD` and `SCL_NODISCARD_MSG`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_NODISCARD
 * @brief The compiler warns where a result of the marked function or type is discarded.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** discarding the result of the function is a mistake. The compiler
 *   warns where a call of the function is written as a statement of its own.
 * - **On a type:** the compiler warns where the result of any function returning the type by value
 *   is discarded.
 * - **The message:** the string literal `msg` of `SCL_NODISCARD_MSG` is put into the warning where
 *   `__has_cpp_attribute(nodiscard)` is `201907L` or more. No message is put into the warning
 *   elsewhere, and `SCL_NODISCARD_MSG` expands to `SCL_NODISCARD` where `SCL_NODISCARD` is defined
 *   before the header.
 * - **Place:** either macro is written before the declaration of a function, and after `struct`,
 *   `class`, `enum` or `enum class` in the declaration of a type.
 *
 * | Condition, first match | `SCL_NODISCARD` |
 * |---|---|
 * | `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
 * | `__has_attribute(__warn_unused_result__)` | `__attribute__((__warn_unused_result__))` |
 * | none of the above | nothing |
 *
 * | Condition, first match | `SCL_NODISCARD_MSG(msg)` |
 * |---|---|
 * | `__has_cpp_attribute(nodiscard)` at `201907L` or more (C++20) | `[[nodiscard(msg)]]` |
 * | `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
 * | `__has_attribute(__warn_unused_result__)` | `__attribute__((__warn_unused_result__))` |
 * | none of the above | `SCL_NODISCARD` |
 *
 * The macros expand to `[[nodiscard]]` and `[[nodiscard(msg)]]` on GCC, Clang and MSVC in C++20.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_NODISCARD` | `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, empty | `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, empty | `[[nodiscard]]`, empty |
 * | `SCL_NODISCARD_MSG(msg)` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, `SCL_NODISCARD` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, `SCL_NODISCARD` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `SCL_NODISCARD` |
 *
 * @snippet attribute/nodiscard/attribute_nodiscard_example.cpp nodiscard
 */

/**
 * @def SCL_NODISCARD_MSG(msg)
 * @brief The compiler warns where a marked result is discarded, with the message `msg`.
 * @ingroup scl_utility_attribute
 * @details
 * @copydetails SCL_NODISCARD
 *
 * @param msg The string literal put into the warning.
 */
