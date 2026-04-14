#pragma once

/**
 * @file
 * @brief Deprecation annotation macros: SCL_DEPRECATED and SCL_DEPRECATED_MSG.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_DEPRECATED
 * @brief Marks a declaration as deprecated.
 * @ingroup scl_utility_attribute
 * @details
 * Causes the compiler to emit a warning whenever the decorated entity is used.
 * Apply before the entity: function, class, variable, enum, typedef, etc.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(deprecated) (C++14):
 *       @c [[deprecated]]
 *  2. MSVC native (not Clang-cl):
 *       @c __declspec(deprecated)
 *  3. @c __has_attribute(deprecated) (GCC, Clang):
 *       @c __attribute__((deprecated))
 *  4. Fallback: empty (no annotation; no warning emitted)
 *
 * @code{.cpp}
 * SCL_DEPRECATED void old_api();
 *
 * SCL_DEPRECATED struct LegacyHandle { int fd; };
 * @endcode
 */

/**
 * @def SCL_DEPRECATED_MSG(msg)
 * @brief Marks a declaration as deprecated with a custom message.
 * @ingroup scl_utility_attribute
 * @details
 * Like @ref SCL_DEPRECATED but includes a string literal @p msg in the
 * compiler diagnostic. On compilers that do not support message-bearing
 * deprecation the macro falls back to @ref SCL_DEPRECATED (warning without
 * message).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(deprecated) (C++14):
 *       @c [[deprecated(msg)]]
 *  2. MSVC native (not Clang-cl):
 *       @c __declspec(deprecated(msg))
 *  3. @c __has_attribute(deprecated) (GCC, Clang):
 *       @c __attribute__((deprecated(msg)))
 *  4. Fallback: empty
 *
 * @param msg A string literal describing the deprecation reason.
 *
 * @code{.cpp}
 * SCL_DEPRECATED_MSG("Use new_api() instead") void old_api();
 * @endcode
 */

#ifndef SCL_DEPRECATED
#if __has_cpp_attribute(deprecated)
#define SCL_DEPRECATED [[deprecated]]
#define SCL_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#elif defined(_MSC_VER) && !defined(__clang__)
#define SCL_DEPRECATED __declspec(deprecated)
#define SCL_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#elif defined(__has_attribute) && __has_attribute(deprecated)
#define SCL_DEPRECATED __attribute__((deprecated))
#define SCL_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#else
#define SCL_DEPRECATED
#define SCL_DEPRECATED_MSG(msg)
#endif
#endif

#ifndef SCL_DEPRECATED_MSG
#define SCL_DEPRECATED_MSG(msg) SCL_DEPRECATED
#endif
