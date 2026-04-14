#pragma once

/**
 * @file
 * @brief Suppress unused-entity warnings: SCL_MAYBE_UNUSED.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_MAYBE_UNUSED
 * @brief Suppresses compiler warnings about unused variables, parameters,
 *        functions, and types.
 * @ingroup scl_utility_attribute
 * @details
 * Apply to any named entity that may legitimately go unused depending on
 * build configuration (e.g., debug-only variables, platform-specific params,
 * assert-only values).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(maybe_unused) (C++17):
 *       @c [[maybe_unused]]
 *  2. @c __has_attribute(unused) (GCC, Clang pre-C++17):
 *       @c __attribute__((unused))
 *  3. Fallback: empty (warning may still fire)
 *
 * @code{.cpp}
 * void process(int value, SCL_MAYBE_UNUSED int debug_flags) {
 *     // debug_flags used only in debug builds
 *     assert(debug_flags >= 0);
 *     (void)value;
 * }
 *
 * SCL_MAYBE_UNUSED static void helper() { ... }
 * @endcode
 */

#ifndef SCL_MAYBE_UNUSED
#if __has_cpp_attribute(maybe_unused)
#define SCL_MAYBE_UNUSED [[maybe_unused]]
#elif defined(__has_attribute) && __has_attribute(unused)
#define SCL_MAYBE_UNUSED __attribute__((unused))
#else
#define SCL_MAYBE_UNUSED
#endif
#endif
