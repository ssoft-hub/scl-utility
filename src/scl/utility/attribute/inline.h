#pragma once

/**
 * @file
 * @brief Force-inline and no-inline function annotation macros.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_FORCE_INLINE
 * @brief Requests that the compiler always inline the decorated function.
 * @ingroup scl_utility_attribute
 * @details
 * Place before the return type: @c SCL_FORCE_INLINE int foo() { ... }
 * The GCC/Clang branch embeds @c inline in the expansion — do not add it manually.
 *
 * Detection order:
 *  1. MSVC native (not Clang-cl):
 *       @c __forceinline
 *  2. @c __has_cpp_attribute(gnu::always_inline) (GCC, Clang):
 *       @c [[gnu::always_inline]] inline
 *  3. Fallback: @c inline
 *
 * @code{.cpp}
 * SCL_FORCE_INLINE int add(int a, int b) { return a + b; }
 * @endcode
 */

/**
 * @def SCL_NOINLINE
 * @brief Requests that the compiler never inline the decorated function.
 * @ingroup scl_utility_attribute
 * @details
 * Place before the return type: @c SCL_NOINLINE void cold_path();
 *
 * Detection order:
 *  1. MSVC native (not Clang-cl):
 *       @c __declspec(noinline)
 *  2. @c __has_cpp_attribute(gnu::noinline) (GCC, Clang):
 *       @c [[gnu::noinline]]
 *  3. Fallback: empty (function compiles without hint)
 *
 * @code{.cpp}
 * SCL_NOINLINE void cold_path();
 * @endcode
 */

#ifndef SCL_FORCE_INLINE
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_FORCE_INLINE __forceinline
#elif __has_cpp_attribute(gnu::always_inline)
#define SCL_FORCE_INLINE [[gnu::always_inline]] inline
#else
#define SCL_FORCE_INLINE inline
#endif
#endif

#ifndef SCL_NOINLINE
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_NOINLINE __declspec(noinline)
#elif __has_cpp_attribute(gnu::noinline)
#define SCL_NOINLINE [[gnu::noinline]]
#else
#define SCL_NOINLINE
#endif
#endif
