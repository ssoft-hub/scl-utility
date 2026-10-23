#pragma once

/**
 * @file
 * @brief The macros `SCL_DEPRECATED` and `SCL_DEPRECATED_MSG`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DEPRECATED),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_DEPRECATED));
// clang-format on

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
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DEPRECATED_MSG("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_STD("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_MSVC("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_ATTRIBUTE("scl_detail_message")),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_DEPRECATED_MSG_FALLBACK("scl_detail_message"))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_DEPRECATED_MSG));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_DEPRECATED
 * @brief The compiler warns at every use of the marked entity.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the entity is kept only for old code, and another entity should
 *   be used in new code. The compiler warns at every use of the entity and issues no warning at
 *   the declaration itself.
 * - **The message:** the string literal `msg` of `SCL_DEPRECATED_MSG` is put into the warning.
 *   `SCL_DEPRECATED_MSG` expands to `SCL_DEPRECATED`, and no message is put into the warning, where
 *   no row before the last matches or where `SCL_DEPRECATED` is defined before the header.
 * - **Place:** either macro is written before the declaration of a function or a variable, and
 *   after `struct`, `class`, `enum` or `enum class` in the declaration of a type.
 *
 * | Condition, first match | `SCL_DEPRECATED` | `SCL_DEPRECATED_MSG(msg)` |
 * |---|---|---|
 * | `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated]]` | `[[deprecated(msg)]]` |
 * | MSVC other than clang-cl | `__declspec(deprecated)` | `__declspec(deprecated(msg))` |
 * | `__has_attribute(__deprecated__)` | `__attribute__((__deprecated__))` | `__attribute__((__deprecated__(msg)))` |
 * | none of the above | nothing | `SCL_DEPRECATED` |
 *
 * The macros expand to `[[deprecated]]` and `[[deprecated(msg)]]` on GCC, Clang and MSVC in C++20.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_DEPRECATED` | `[[deprecated]]`, `__attribute__((__deprecated__))`, empty | `[[deprecated]]`, `__attribute__((__deprecated__))`, empty | `[[deprecated]]`, `__declspec(deprecated)`, empty |
 * | `SCL_DEPRECATED_MSG(msg)` | `[[deprecated(msg)]]`, `__attribute__((__deprecated__(msg)))`, `SCL_DEPRECATED` | `[[deprecated(msg)]]`, `__attribute__((__deprecated__(msg)))`, `SCL_DEPRECATED` | `[[deprecated(msg)]]`, `__declspec(deprecated(msg))`, `SCL_DEPRECATED` |
 *
 * @snippet attribute/deprecated/attribute_deprecated_example.cpp deprecated
 */

/**
 * @def SCL_DEPRECATED_MSG(msg)
 * @brief The compiler warns at every use of the marked entity, with the message `msg`.
 * @ingroup scl_utility_attribute
 * @details
 * @copydetails SCL_DEPRECATED
 *
 * @param msg The string literal put into the warning.
 */
