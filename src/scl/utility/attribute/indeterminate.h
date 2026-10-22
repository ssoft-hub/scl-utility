#pragma once

/**
 * @file
 * @brief The macro `SCL_INDETERMINATE`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#ifdef __has_cpp_attribute
#ifndef indeterminate
#if __has_cpp_attribute(indeterminate)
#define SCL_DETAIL_INDETERMINATE_STD [[indeterminate]]
#endif
#endif
#endif

#define SCL_DETAIL_INDETERMINATE_FALLBACK

#ifndef SCL_INDETERMINATE
#ifdef SCL_DETAIL_INDETERMINATE_STD
#define SCL_INDETERMINATE SCL_DETAIL_INDETERMINATE_STD
#else
#define SCL_INDETERMINATE SCL_DETAIL_INDETERMINATE_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_INDETERMINATE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_INDETERMINATE_STD),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_INDETERMINATE_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_INDETERMINATE));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_INDETERMINATE
 * @brief The storage of the marked variable or parameter is not read before it is written.
 * @ingroup scl_utility_attribute
 * @details
 * - **Without an initializer:** a variable with automatic storage duration declared with no
 *   initializer stays uninitialised whatever the macro expands to, except for what a default
 *   constructor initialises.
 * - **What the compiler is told:** the storage is not read before it is written. The implementation
 *   gives such a variable an erroneous value in C++26, and a read of it before the first write is
 *   erroneous behaviour. The implementation may skip that value under the attribute
 *   `[[indeterminate]]`.
 * - **Without the attribute:** a read before the first write is undefined behaviour before C++26,
 *   and erroneous behaviour in C++26.
 * - **A false promise:** a read of such storage before it is written is undefined behaviour under
 *   `[[indeterminate]]`.
 * - **Place:** the macro is written before the definition of a variable with automatic storage
 *   duration, or before a parameter in the first declaration of its function. A program is
 *   ill-formed, no diagnostic required, where a parameter is declared with the attribute in
 *   the first declaration of its function in one translation unit and without it in the first
 *   declaration in another.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | `__has_cpp_attribute(indeterminate)` (C++26), `indeterminate` not a macro | `[[indeterminate]]` |
 * | none of the above | nothing |
 *
 * The macro expands to `[[indeterminate]]` where `__has_cpp_attribute(indeterminate)` is not `0`
 * and `indeterminate` is not a macro. GCC 13, Clang 22 and MSVC 19.44 report `0` in C++20, so
 * the macro expands to nothing there.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_INDETERMINATE` | empty | empty | empty |
 *
 * @snippet attribute/indeterminate/attribute_indeterminate_example.cpp indeterminate
 */
