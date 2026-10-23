#pragma once

/**
 * @file
 * @brief The macro `SCL_LIFETIMEBOUND`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

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

// clang-format off
static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_LIFETIMEBOUND),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_CLANG),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_LIFETIMEBOUND_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_LIFETIMEBOUND));
// clang-format on

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_LIFETIMEBOUND
 * @brief The result of the function refers to the object of the marked parameter or of `*this`.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the reference, pointer or view the function returns refers to
 *   the object the parameter refers to. The same is stated of `*this` on a member function.
 * - **What Clang reports:** Clang reports the warning `-Wdangling` where the result outlives
 *   the object, such as a `std::string_view` initialised from
 *   `first_word(std::string{"hello world"})`, whose argument is destroyed at the end of the full
 *   expression.
 * - **What MSVC reports:** MSVC reports warning C26815 where the result outlives the object, in
 *   code analysis with `/analyze /analyze:plugin EspXEngine.dll`. MSVC reports nothing without that
 *   plugin.
 * - **GCC:** the macro expands to nothing on GCC. GCC issues its warning `-Wdangling-reference`,
 *   part of `-Wall`, by an analysis of its own, and that analysis is the same with the macro and
 *   without it.
 * - **Place:** the macro is written after the declarator of a parameter, or after the qualifiers of
 *   a member function.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | MSVC other than clang-cl, `__has_cpp_attribute(msvc::lifetimebound)`, neither `msvc` nor `lifetimebound` a macro | `[[msvc::lifetimebound]]` |
 * | `__has_cpp_attribute(_Clang::__lifetimebound__)` (Clang) | `[[_Clang::__lifetimebound__]]` |
 * | `__has_attribute(__lifetimebound__)` | `__attribute__((__lifetimebound__))` |
 * | none of the above | nothing |
 *
 * The macro expands to `[[msvc::lifetimebound]]` on MSVC 19.44, to `[[_Clang::__lifetimebound__]]`
 * on Clang and to nothing on GCC.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_LIFETIMEBOUND` | empty | `[[_Clang::__lifetimebound__]]`, `__attribute__((__lifetimebound__))`, empty | `[[msvc::lifetimebound]]`, empty |
 *
 * @snippet attribute/lifetimebound/attribute_lifetimebound_example.cpp lifetimebound
 */
