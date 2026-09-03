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
 * Causes the compiler to emit a warning whenever the annotated entity is used.
 * Apply before a function, a variable or a typedef, and after the @c class,
 * @c struct or @c enum keyword: @c struct @c SCL_DEPRECATED @c Name.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(deprecated) (C++14):
 *       @c [[deprecated]]
 *  2. MSVC other than clang-cl:
 *       @c __declspec(deprecated)
 *  3. @c __has_attribute(deprecated) (GCC, Clang):
 *       @c __attribute__((deprecated))
 *  4. Fallback: empty (no annotation; no warning emitted)
 *
 * @code{.cpp}
 * SCL_DEPRECATED void old_api();
 *
 * struct SCL_DEPRECATED LegacyHandle { int fd; };
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
 *  2. MSVC other than clang-cl:
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
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(deprecated)
#define SCL_DEPRECATED [[deprecated]]
#ifndef SCL_DEPRECATED_MSG
#define SCL_DEPRECATED_MSG(msg) [[deprecated(msg)]]
#endif
#endif
#endif
#endif

#ifndef SCL_DEPRECATED
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DEPRECATED __declspec(deprecated)
#ifndef SCL_DEPRECATED_MSG
#define SCL_DEPRECATED_MSG(msg) __declspec(deprecated(msg))
#endif
#endif
#endif

#ifndef SCL_DEPRECATED
#ifdef __has_attribute
#if __has_attribute(deprecated)
#define SCL_DEPRECATED __attribute__((deprecated))
#ifndef SCL_DEPRECATED_MSG
#define SCL_DEPRECATED_MSG(msg) __attribute__((deprecated(msg)))
#endif
#endif
#endif
#endif

#ifndef SCL_DEPRECATED
#define SCL_DEPRECATED
#endif

#ifndef SCL_DEPRECATED_MSG
#define SCL_DEPRECATED_MSG(msg) SCL_DEPRECATED
#endif
