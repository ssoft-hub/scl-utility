#pragma once

/**
 * @file
 * @brief The macros `SCL_HOT` and `SCL_COLD`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_HOT_MSVC
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__hot__)
#define SCL_DETAIL_HOT_GNU [[__gnu__::__hot__]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__hot__)
#define SCL_DETAIL_HOT_ATTRIBUTE __attribute__((__hot__))
#endif
#endif

#define SCL_DETAIL_HOT_FALLBACK

#ifndef SCL_HOT
#ifdef SCL_DETAIL_HOT_MSVC
#define SCL_HOT SCL_DETAIL_HOT_MSVC
#elif defined(SCL_DETAIL_HOT_GNU)
#define SCL_HOT SCL_DETAIL_HOT_GNU
#elif defined(SCL_DETAIL_HOT_ATTRIBUTE)
#define SCL_HOT SCL_DETAIL_HOT_ATTRIBUTE
#else
#define SCL_HOT SCL_DETAIL_HOT_FALLBACK
#endif
#endif

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_HOT),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_GNU),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_HOT_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_HOT));
// clang-format on

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_COLD_MSVC
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__cold__)
#define SCL_DETAIL_COLD_GNU [[__gnu__::__cold__]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__cold__)
#define SCL_DETAIL_COLD_ATTRIBUTE __attribute__((__cold__))
#endif
#endif

#define SCL_DETAIL_COLD_FALLBACK

#ifndef SCL_COLD
#ifdef SCL_DETAIL_COLD_MSVC
#define SCL_COLD SCL_DETAIL_COLD_MSVC
#elif defined(SCL_DETAIL_COLD_GNU)
#define SCL_COLD SCL_DETAIL_COLD_GNU
#elif defined(SCL_DETAIL_COLD_ATTRIBUTE)
#define SCL_COLD SCL_DETAIL_COLD_ATTRIBUTE
#else
#define SCL_COLD SCL_DETAIL_COLD_FALLBACK
#endif
#endif

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_COLD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_GNU),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_COLD_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_COLD));
// clang-format on

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_HOT
 * @brief The compiler is told that the marked function is called often.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the function is called often (`SCL_HOT`) or rarely (`SCL_COLD`).
 * - **What GCC and Clang do:** GCC and Clang mark a function under `SCL_HOT` as called often, and
 *   GCC optimizes it harder. Both optimize a function under `SCL_COLD` for size and take a branch
 *   that leads to a call of it as unlikely. Both place hot functions in one section and cold
 *   functions in another, apart from the rest of the code, where the target and the options allow
 *   it. The likelihood of a branch that leads to a call of a hot function stays as it is.
 * - **MSVC:** MSVC has no attribute for a function called often or rarely, and both macros expand
 *   to nothing there.
 * - **One of the two:** `SCL_HOT` and `SCL_COLD` must not both be written on one function. Clang
 *   rejects the pair, and GCC drops one of the two attributes with a warning.
 * - **Place:** either macro is written before the declaration of a function.
 *
 * | Condition, first match | `SCL_HOT` | `SCL_COLD` |
 * |---|---|---|
 * | MSVC other than clang-cl | nothing | nothing |
 * | `__has_cpp_attribute(__gnu__::__hot__)`, `__has_cpp_attribute(__gnu__::__cold__)` (GCC, Clang) | `[[__gnu__::__hot__]]` | `[[__gnu__::__cold__]]` |
 * | `__has_attribute(__hot__)`, `__has_attribute(__cold__)` | `__attribute__((__hot__))` | `__attribute__((__cold__))` |
 * | none of the above | nothing | nothing |
 *
 * The macros expand to nothing on MSVC, and to `[[__gnu__::__hot__]]` and `[[__gnu__::__cold__]]`
 * on GCC and Clang.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_HOT` | `[[__gnu__::__hot__]]`, `__attribute__((__hot__))`, empty | `[[__gnu__::__hot__]]`, `__attribute__((__hot__))`, empty | empty |
 * | `SCL_COLD` | `[[__gnu__::__cold__]]`, `__attribute__((__cold__))`, empty | `[[__gnu__::__cold__]]`, `__attribute__((__cold__))`, empty | empty |
 *
 * @snippet attribute/hotcold/attribute_hotcold_example.cpp hotcold
 */

/**
 * @def SCL_COLD
 * @brief The compiler is told that the marked function is called rarely.
 * @ingroup scl_utility_attribute
 * @details
 * @copydetails SCL_HOT
 */
