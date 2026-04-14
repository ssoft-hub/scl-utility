#pragma once

/**
 * @file
 * @brief Function-frequency hint macros: SCL_HOT and SCL_COLD.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_HOT
 * @brief Marks a function as called frequently (hot path).
 * @ingroup scl_utility_attribute
 * @details
 * Instructs the compiler to optimise the function more aggressively and
 * place it in a section of the binary that improves instruction-cache
 * locality for hot code. Branches toward a @c SCL_HOT call site are
 * implicitly biased as likely.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(gnu::hot) (GCC, Clang):
 *       @c [[gnu::hot]]
 *  2. @c __has_attribute(hot) (GCC, Clang older):
 *       @c __attribute__((hot))
 *  3. Fallback: empty (no hint)
 *
 * @code{.cpp}
 * SCL_HOT void dispatch(const Event& e);
 *
 * SCL_HOT int lookup(std::uint32_t key);
 * @endcode
 */

/**
 * @def SCL_COLD
 * @brief Marks a function as called rarely (cold path).
 * @ingroup scl_utility_attribute
 * @details
 * Instructs the compiler to optimise the function for size rather than
 * speed and to place it in a cold section of the binary, away from hot
 * code. Branches toward a @c SCL_COLD call site are implicitly biased as
 * unlikely. Combines naturally with @ref SCL_NOINLINE and @ref SCL_NORETURN.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(gnu::cold) (GCC, Clang):
 *       @c [[gnu::cold]]
 *  2. @c __has_attribute(cold) (GCC, Clang older):
 *       @c __attribute__((cold))
 *  3. Fallback: empty (no hint)
 *
 * @code{.cpp}
 * SCL_COLD SCL_NOINLINE void report_oom(std::size_t requested);
 *
 * SCL_COLD SCL_NORETURN void fatal(const char* msg);
 * @endcode
 */

#ifndef SCL_HOT
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_HOT
#elif __has_cpp_attribute(gnu::hot)
#define SCL_HOT [[gnu::hot]]
#elif defined(__has_attribute) && __has_attribute(hot)
#define SCL_HOT __attribute__((hot))
#else
#define SCL_HOT
#endif
#endif

#ifndef SCL_COLD
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_COLD
#elif __has_cpp_attribute(gnu::cold)
#define SCL_COLD [[gnu::cold]]
#elif defined(__has_attribute) && __has_attribute(cold)
#define SCL_COLD __attribute__((cold))
#else
#define SCL_COLD
#endif
#endif
