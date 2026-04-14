#pragma once

/**
 * @file
 * @brief Lifetime-bound parameter annotation macro: SCL_LIFETIMEBOUND.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_LIFETIMEBOUND
 * @brief Marks a function parameter (or implicit @c *this) as
 *        lifetime-bound to the return value.
 * @ingroup scl_utility_attribute
 * @details
 * When applied to a parameter, the compiler warns if the returned
 * reference or pointer outlives the annotated argument (dangling
 * reference detection at compile time).
 *
 * **Placement** — after the parameter declaration or after the
 * cv-qualifier of a member function:
 * @code{.cpp}
 * // Parameter
 * std::string_view first(const std::string& s SCL_LIFETIMEBOUND);
 *
 * // Member function (*this is lifetimebound to return value)
 * class Builder {
 *     Builder& set(int v) SCL_LIFETIMEBOUND;
 * };
 * @endcode
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(clang::lifetimebound) (Clang):
 *       @c [[clang::lifetimebound]]
 *  2. @c __has_attribute(lifetimebound) (Clang older):
 *       @c __attribute__((lifetimebound))
 *  3. Fallback: empty (no lifetime analysis; no warning)
 *
 * @note GCC and MSVC do not currently support this attribute.
 *       The fallback is always empty on those compilers.
 *
 * @code{.cpp}
 * const int& clamp(const int& v SCL_LIFETIMEBOUND,
 *                  const int& lo SCL_LIFETIMEBOUND,
 *                  const int& hi SCL_LIFETIMEBOUND);
 *
 * // Dangling reference — Clang warns with SCL_LIFETIMEBOUND:
 * const int& r = clamp(42, 0, 100);  // temporary args destroyed
 * @endcode
 */

#ifndef SCL_LIFETIMEBOUND
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_LIFETIMEBOUND
#elif __has_cpp_attribute(clang::lifetimebound)
#define SCL_LIFETIMEBOUND [[clang::lifetimebound]]
#elif defined(__has_attribute) && __has_attribute(lifetimebound)
#define SCL_LIFETIMEBOUND __attribute__((lifetimebound))
#else
#define SCL_LIFETIMEBOUND
#endif
#endif
