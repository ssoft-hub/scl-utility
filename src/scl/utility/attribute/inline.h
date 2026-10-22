#pragma once

/**
 * @file
 * @brief Force-inline and no-inline function annotation macros.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

/**
 * @def SCL_FORCE_INLINE
 * @brief Requests that the compiler always inline the annotated function.
 * @ingroup scl_utility_attribute
 * @details
 * Place before the return type: @c SCL_FORCE_INLINE int foo() { ... }
 * The GCC/Clang branch embeds @c inline in the expansion — do not add it manually.
 *
 * Detection order:
 *  1. MSVC other than clang-cl:
 *       @c __forceinline
 *  2. @c __has_cpp_attribute(gnu::always_inline) (GCC, Clang):
 *       @c [[gnu::always_inline]] inline
 *  3. @c __has_attribute(always_inline) (older GCC and Clang):
 *       @c __attribute__((always_inline)) inline
 *  4. Fallback: @c inline
 *
 * @code{.cpp}
 * SCL_FORCE_INLINE int add(int a, int b) { return a + b; }
 * @endcode
 */

/**
 * @def SCL_NOINLINE
 * @brief Requests that the compiler never inline the annotated function.
 * @ingroup scl_utility_attribute
 * @details
 * Place before the return type: @c SCL_NOINLINE void cold_path();
 *
 * Detection order:
 *  1. MSVC other than clang-cl:
 *       @c __declspec(noinline)
 *  2. @c __has_cpp_attribute(gnu::noinline) (GCC, Clang):
 *       @c [[gnu::noinline]]
 *  3. @c __has_attribute(noinline) (older GCC and Clang):
 *       @c __attribute__((noinline))
 *  4. Fallback: empty (function compiles without hint)
 *
 * @code{.cpp}
 * SCL_NOINLINE void cold_path();
 * @endcode
 */

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
