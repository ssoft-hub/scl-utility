#pragma once

/**
 * @file
 * @brief The macros `SCL_FORCE_INLINE` and `SCL_NOINLINE`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_FORCE_INLINE_MSVC __forceinline
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__always_inline__)
#define SCL_DETAIL_FORCE_INLINE_GNU [[__gnu__::__always_inline__]] inline
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__always_inline__)
#define SCL_DETAIL_FORCE_INLINE_ATTRIBUTE __attribute__((__always_inline__)) inline
#endif
#endif

#define SCL_DETAIL_FORCE_INLINE_FALLBACK inline

#ifndef SCL_FORCE_INLINE
#ifdef SCL_DETAIL_FORCE_INLINE_MSVC
#define SCL_FORCE_INLINE SCL_DETAIL_FORCE_INLINE_MSVC
#elif defined(SCL_DETAIL_FORCE_INLINE_GNU)
#define SCL_FORCE_INLINE SCL_DETAIL_FORCE_INLINE_GNU
#elif defined(SCL_DETAIL_FORCE_INLINE_ATTRIBUTE)
#define SCL_FORCE_INLINE SCL_DETAIL_FORCE_INLINE_ATTRIBUTE
#else
#define SCL_FORCE_INLINE SCL_DETAIL_FORCE_INLINE_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_FORCE_INLINE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FORCE_INLINE_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FORCE_INLINE_GNU),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FORCE_INLINE_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_FORCE_INLINE_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_FORCE_INLINE));

#if defined(_MSC_VER) && !defined(__clang__)
#ifndef noinline
#define SCL_DETAIL_NOINLINE_MSVC __declspec(noinline)
#endif
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__noinline__)
#define SCL_DETAIL_NOINLINE_GNU [[__gnu__::__noinline__]]
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__noinline__)
#define SCL_DETAIL_NOINLINE_ATTRIBUTE __attribute__((__noinline__))
#endif
#endif

#define SCL_DETAIL_NOINLINE_FALLBACK

#ifndef SCL_NOINLINE
#if defined(_MSC_VER) && !defined(__clang__) && defined(noinline)
#error "No spelling of SCL_NOINLINE remains for MSVC under the macro noinline; SCL_NOINLINE must be defined before the header as an empty macro"
#endif
#endif

#ifndef SCL_NOINLINE
#ifdef SCL_DETAIL_NOINLINE_MSVC
#define SCL_NOINLINE SCL_DETAIL_NOINLINE_MSVC
#elif defined(SCL_DETAIL_NOINLINE_GNU)
#define SCL_NOINLINE SCL_DETAIL_NOINLINE_GNU
#elif defined(SCL_DETAIL_NOINLINE_ATTRIBUTE)
#define SCL_NOINLINE SCL_DETAIL_NOINLINE_ATTRIBUTE
#else
#define SCL_NOINLINE SCL_DETAIL_NOINLINE_FALLBACK
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(SCL_DETAIL_ATTRIBUTE_TEXT(SCL_NOINLINE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NOINLINE_MSVC),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NOINLINE_GNU),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NOINLINE_ATTRIBUTE),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_NOINLINE_FALLBACK)),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_NOINLINE));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_FORCE_INLINE
 * @brief The compiler is asked to inline the marked function at every call.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the function is to be inlined at every call, whatever
 *   the heuristics of the compiler. Clang and MSVC keep a direct call they cannot inline as a call,
 *   and GCC rejects it, as described under **Recursion**. MSVC inlines nothing at `/Od`.
 * - **`inline` included:** every expansion contains the keyword `inline` or `__forceinline`, so
 *   a function defined in a header with the macro may be defined in every translation unit in which
 *   the header is included. `inline` must not be written beside the macro, since GCC rejects
 *   a second `inline`.
 * - **Recursion:** GCC rejects a recursive function under the macro with
 *   `inlining failed in call to 'always_inline'` at `-O0`, `-O1` and `-Og`, and at every other
 *   level unless it turns the recursion into a loop. Clang accepts such a function at every level.
 *   MSVC accepts it, does not inline it and, under `/W4` with `/O1` or `/O2`, reports warning
 *   C4714.
 * - **Place:** the macro is written before the declaration of a function, after every other macro
 *   of the group there. The expansion ends in `inline` on GCC and Clang, and a standard attribute
 *   after it is an error.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | MSVC other than clang-cl | `__forceinline` |
 * | `__has_cpp_attribute(__gnu__::__always_inline__)` (GCC, Clang) | `[[__gnu__::__always_inline__]] inline` |
 * | `__has_attribute(__always_inline__)` | `__attribute__((__always_inline__)) inline` |
 * | none of the above | `inline` |
 *
 * The macro expands to `__forceinline` on MSVC and to `[[__gnu__::__always_inline__]] inline` on
 * GCC and Clang.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_FORCE_INLINE` | `[[__gnu__::__always_inline__]] inline`, `__attribute__((__always_inline__)) inline`, `inline` | `[[__gnu__::__always_inline__]] inline`, `__attribute__((__always_inline__)) inline`, `inline` | `__forceinline`, `inline` |
 *
 * @snippet attribute/inline/attribute_inline_example.cpp force_inline
 */

/**
 * @def SCL_NOINLINE
 * @brief The compiler is asked never to inline the marked function.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told:** the function is not to be inlined into a caller, so it keeps
 *   a frame of its own in the call stack. GCC may still make a specialised copy of it at `-O3`, and
 *   a call of a function with no side effect whose result goes unread may still be dropped.
 * - **Place:** the macro is written before the declaration of a function, after every macro of
 *   the group that expands to a standard attribute. MSVC drops `[[nodiscard]]` and
 *   `[[maybe_unused]]` written after `__declspec(noinline)`, and reports the drop only by
 *   the warning C5240, which is off by default.
 *
 * | Condition, first match | Expansion |
 * |---|---|
 * | MSVC other than clang-cl | `__declspec(noinline)` |
 * | `__has_cpp_attribute(__gnu__::__noinline__)` (GCC, Clang) | `[[__gnu__::__noinline__]]` |
 * | `__has_attribute(__noinline__)` | `__attribute__((__noinline__))` |
 * | none of the above | nothing |
 *
 * The macro expands to `__declspec(noinline)` on MSVC and to `[[__gnu__::__noinline__]]` on GCC and
 * Clang.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_NOINLINE` | `[[__gnu__::__noinline__]]`, `__attribute__((__noinline__))`, empty | `[[__gnu__::__noinline__]]`, `__attribute__((__noinline__))`, empty | `__declspec(noinline)`, empty |
 *
 * @snippet attribute/inline/attribute_inline_example.cpp noinline
 */
