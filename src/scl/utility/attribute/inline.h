#pragma once

/**
 * @file
 * @brief Force-inline and no-inline function annotation macros.
 * @ingroup scl_utility_attribute
 */

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

#ifndef SCL_FORCE_INLINE
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_FORCE_INLINE __forceinline
#endif
#endif

#ifndef SCL_FORCE_INLINE
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(gnu::always_inline)
#define SCL_FORCE_INLINE [[gnu::always_inline]] inline
#endif
#endif
#endif

#ifndef SCL_FORCE_INLINE
#ifdef __has_attribute
#if __has_attribute(always_inline)
#define SCL_FORCE_INLINE __attribute__((always_inline)) inline
#endif
#endif
#endif

#ifndef SCL_FORCE_INLINE
#define SCL_FORCE_INLINE inline
#endif

#ifndef SCL_NOINLINE
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_NOINLINE __declspec(noinline)
#endif
#endif

#ifndef SCL_NOINLINE
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(gnu::noinline)
#define SCL_NOINLINE [[gnu::noinline]]
#endif
#endif
#endif

#ifndef SCL_NOINLINE
#ifdef __has_attribute
#if __has_attribute(noinline)
#define SCL_NOINLINE __attribute__((noinline))
#endif
#endif
#endif

#ifndef SCL_NOINLINE
#define SCL_NOINLINE
#endif
