#pragma once

/**
 * @file
 * @brief The macro `SCL_MAYBE_UNUSED`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(maybe_unused)
#define SCL_DETAIL_MAYBE_UNUSED_STD [[maybe_unused]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__unused__)
#define SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE __attribute__((__unused__))
#endif
#endif

#define SCL_DETAIL_MAYBE_UNUSED_FALLBACK

#ifndef SCL_MAYBE_UNUSED
#ifdef SCL_DETAIL_MAYBE_UNUSED_STD
#define SCL_MAYBE_UNUSED SCL_DETAIL_MAYBE_UNUSED_STD
#elif defined(SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE)
#define SCL_MAYBE_UNUSED SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE
#else
#define SCL_MAYBE_UNUSED SCL_DETAIL_MAYBE_UNUSED_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_MAYBE_UNUSED),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_MAYBE_UNUSED_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_MAYBE_UNUSED_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_MAYBE_UNUSED_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_MAYBE_UNUSED));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_MAYBE_UNUSED
 * @brief The marked entity may go unused in some build, with no warning about it.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the entity may go unused, such as a parameter read only by
 *   an assertion in a build without `NDEBUG`. The compiler issues no warning about the unused
 *   entity, and the builds with the macro and without it differ in that warning alone.
 * - **Place:** the macro is written before the declaration of a variable, a parameter or
 *   a function, and after `struct`, `class`, `enum` or `enum class` in the declaration of a type.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | `__has_cpp_attribute(maybe_unused)` (C++17) | `[[maybe_unused]]` |
 * | `__has_attribute(__unused__)` | `__attribute__((__unused__))` |
 * | none of the above | nothing |
 *
 * The macro expands to `[[maybe_unused]]` on GCC, Clang and MSVC in C++20.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_MAYBE_UNUSED` | `[[maybe_unused]]`, `__attribute__((__unused__))`, empty | `[[maybe_unused]]`, `__attribute__((__unused__))`, empty | `[[maybe_unused]]`, empty |
 *
 * @snippet attribute/maybe_unused/attribute_maybe_unused_example.cpp maybe_unused
 */
