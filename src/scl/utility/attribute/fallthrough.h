#pragma once

/**
 * @file
 * @brief The macro `SCL_FALLTHROUGH`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(fallthrough)
#define SCL_DETAIL_FALLTHROUGH_STD [[fallthrough]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__fallthrough__)
#define SCL_DETAIL_FALLTHROUGH_ATTRIBUTE __attribute__((__fallthrough__))
#endif
#endif

#define SCL_DETAIL_FALLTHROUGH_FALLBACK ((void)0)

#ifndef SCL_FALLTHROUGH
#ifdef SCL_DETAIL_FALLTHROUGH_STD
#define SCL_FALLTHROUGH SCL_DETAIL_FALLTHROUGH_STD
#elif defined(SCL_DETAIL_FALLTHROUGH_ATTRIBUTE)
#define SCL_FALLTHROUGH SCL_DETAIL_FALLTHROUGH_ATTRIBUTE
#else
#define SCL_FALLTHROUGH SCL_DETAIL_FALLTHROUGH_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_FALLTHROUGH),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FALLTHROUGH_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FALLTHROUGH_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FALLTHROUGH_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_FALLTHROUGH));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_FALLTHROUGH
 * @brief Execution passes on purpose from the marked case of a `switch` into the next case.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the case ends without `break` on purpose. The compiler issues no
 *   warning `-Wimplicit-fallthrough` for it. MSVC issues no warning C5262, which is off by default,
 *   and its code analysis with `/analyze /analyze:plugin EspXEngine.dll` issues no warning C26819.
 *   The flow of control is the same with the macro and without it.
 * - **Place:** the macro is written as the last statement of a case, closed by a semicolon, right
 *   before the next label.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | `__has_cpp_attribute(fallthrough)` (C++17) | `[[fallthrough]]` |
 * | `__has_attribute(__fallthrough__)` | `__attribute__((__fallthrough__))` |
 * | none of the above | `((void)0)` |
 *
 * The macro expands to `[[fallthrough]]` on GCC, Clang and MSVC in C++20.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_FALLTHROUGH` | `[[fallthrough]]`, `__attribute__((__fallthrough__))`, `((void)0)` | `[[fallthrough]]`, `__attribute__((__fallthrough__))`, `((void)0)` | `[[fallthrough]]`, `((void)0)` |
 *
 * @snippet attribute/fallthrough/attribute_fallthrough_example.cpp fallthrough
 */
