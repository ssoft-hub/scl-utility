#pragma once

/**
 * @file
 * @brief Intentional switch fall-through annotation macro: SCL_FALLTHROUGH.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_FALLTHROUGH
 * @brief Suppresses the implicit fall-through warning in a @c switch statement.
 * @ingroup scl_utility_attribute
 * @details
 * Place as a statement at the end of a @c case body that intentionally falls
 * through to the next @c case. This silences @c -Wimplicit-fallthrough and the
 * equivalent MSVC warning C26819.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(fallthrough) (C++17):
 *       @c [[fallthrough]]
 *  2. @c __has_attribute(fallthrough) (GCC ≥ 7, Clang pre-C++17):
 *       @c __attribute__((fallthrough))
 *  3. Fallback: @c ((void)0) — silent no-op
 *
 * @code{.cpp}
 * switch (code) {
 * case 1:
 *     prepare();
 *     SCL_FALLTHROUGH;
 * case 2:
 *     execute();
 *     break;
 * }
 * @endcode
 */

#ifndef SCL_FALLTHROUGH
#if __has_cpp_attribute(fallthrough)
#define SCL_FALLTHROUGH [[fallthrough]]
#elif defined(__has_attribute) && __has_attribute(fallthrough)
#define SCL_FALLTHROUGH __attribute__((fallthrough))
#else
#define SCL_FALLTHROUGH ((void)0)
#endif
#endif
