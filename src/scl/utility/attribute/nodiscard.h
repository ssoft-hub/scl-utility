#pragma once

/**
 * @file
 * @brief Warn-on-discard annotation macros: SCL_NODISCARD and SCL_NODISCARD_MSG.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_NODISCARD
 * @brief Warns if the return value of a function (or an object of a type) is
 *        discarded by the caller.
 * @ingroup scl_utility_attribute
 * @details
 * Apply to a function declaration or to a class/struct/enum declaration.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(nodiscard) ≥ 201907L (C++20):
 *       @c [[nodiscard]]
 *  2. @c __has_cpp_attribute(nodiscard) (C++17):
 *       @c [[nodiscard]]
 *  3. @c __has_attribute(warn_unused_result) (GCC, Clang pre-C++17):
 *       @c __attribute__((warn_unused_result))
 *  4. Fallback: empty (no warning emitted)
 *
 * @code{.cpp}
 * SCL_NODISCARD bool try_connect(const char* host);
 *
 * SCL_NODISCARD struct ErrorCode { int value; };
 * @endcode
 */

/**
 * @def SCL_NODISCARD_MSG(msg)
 * @brief Like @ref SCL_NODISCARD but includes a custom message in the diagnostic.
 * @ingroup scl_utility_attribute
 * @details
 * The message @p msg is a string literal describing why the return value must
 * not be ignored. On compilers where the message form is unavailable the macro
 * falls back to @ref SCL_NODISCARD (warning without message).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(nodiscard) ≥ 201907L (C++20):
 *       @c [[nodiscard(msg)]]
 *  2. @c __has_cpp_attribute(nodiscard) (C++17) or earlier fallback:
 *       @c SCL_NODISCARD (message silently dropped)
 *
 * @param msg A string literal stating the discard consequence.
 *
 * @code{.cpp}
 * SCL_NODISCARD_MSG("ignoring the error code leaks resources")
 * ErrorCode write(int fd, const void* buf, std::size_t n);
 * @endcode
 */

#ifndef SCL_NODISCARD
#if __has_cpp_attribute(nodiscard) >= 201907L
#define SCL_NODISCARD [[nodiscard]]
#define SCL_NODISCARD_MSG(msg) [[nodiscard(msg)]]
#elif __has_cpp_attribute(nodiscard)
#define SCL_NODISCARD [[nodiscard]]
#define SCL_NODISCARD_MSG(msg) [[nodiscard]]
#elif defined(__has_attribute) && __has_attribute(warn_unused_result)
#define SCL_NODISCARD __attribute__((warn_unused_result))
#define SCL_NODISCARD_MSG(msg) __attribute__((warn_unused_result))
#else
#define SCL_NODISCARD
#define SCL_NODISCARD_MSG(msg)
#endif
#endif

#ifndef SCL_NODISCARD_MSG
#define SCL_NODISCARD_MSG(msg) SCL_NODISCARD
#endif
