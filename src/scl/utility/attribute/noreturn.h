#pragma once

/**
 * @file
 * @brief Non-returning function annotation macro: SCL_NORETURN.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_NORETURN
 * @brief Marks a function that never returns to its caller.
 * @ingroup scl_utility_attribute
 * @details
 * Apply before the return type. The compiler may assume that any call to a
 * @c SCL_NORETURN function is a dead end and optimize accordingly.
 * If a decorated function does return, the behaviour is **undefined**.
 *
 * Typical uses: @c std::terminate wrappers, @c throw helpers,
 * infinite-loop entry points.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(noreturn) (C++11):
 *       @c [[noreturn]] — standard form
 *  2. MSVC native (not Clang-cl):
 *       @c __declspec(noreturn)
 *  3. @c __has_attribute(noreturn) (GCC, Clang):
 *       @c __attribute__((noreturn))
 *  4. Fallback: empty (no annotation; compiler warning may fire)
 *
 * @code{.cpp}
 * SCL_NORETURN void fatal(const char* msg);
 *
 * int divide(int a, int b) {
 *     if (b == 0) fatal("division by zero");
 *     return a / b;  // no "not all paths return" warning
 * }
 * @endcode
 */

#ifndef SCL_NORETURN
#if __has_cpp_attribute(noreturn)
#define SCL_NORETURN [[noreturn]]
#elif defined(_MSC_VER) && !defined(__clang__)
#define SCL_NORETURN __declspec(noreturn)
#elif defined(__has_attribute) && __has_attribute(noreturn)
#define SCL_NORETURN __attribute__((noreturn))
#else
#define SCL_NORETURN
#endif
#endif
