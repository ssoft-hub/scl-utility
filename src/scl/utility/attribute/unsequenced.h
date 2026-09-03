#pragma once

/**
 * @file
 * @brief Pure-function annotation macros: SCL_UNSEQUENCED and SCL_REPRODUCIBLE.
 * @ingroup scl_utility_attribute
 */

/**
 * @def SCL_UNSEQUENCED(...)
 * @brief Wrapping macro that marks a function as having no observable side
 *        effects and reading no mutable global state — calls may be freely
 *        reordered, merged, or eliminated.
 * @ingroup scl_utility_attribute
 * @details
 * Corresponds to the GCC/Clang @c [[gnu::const]] / @c __attribute__((const))
 * extension. C23 spells the same guarantee @c [[unsequenced]] (WG14 N2956); no
 * C++ revision has adopted it, and the probe below is there for one that does.
 *
 * **Usage — wrap the entire return-type + declarator:**
 * @code{.cpp}
 * SCL_UNSEQUENCED(int square(int x)) { return x * x; }
 * SCL_UNSEQUENCED(std::size_t popcount(std::uint64_t v));
 * @endcode
 *
 * The macro inserts the attribute in the position appropriate for each form:
 * @code{.cpp}
 * int square(int x) [[unsequenced]] { ... }        // C23 spelling (suffix)
 * [[gnu::const]] int square(int x) { ... }         // GCC / Clang (prefix)
 * __attribute__((const)) int square(int x) { ... } // older GCC / Clang
 *             int square(int x) { ... }            // MSVC / unknown
 * @endcode
 *
 * In the suffix form the attribute is appended after @c __VA_ARGS__ and
 * before the function body — the body @c { ... } is not part of the macro
 * argument, so the expansion is valid in both declaration and definition.
 *
 * A function annotated with @c SCL_UNSEQUENCED must satisfy all of:
 * - Does not modify any state observable outside the function.
 * - Does not read any mutable global or thread-local state.
 * - Return value depends solely on the arguments.
 *
 * Because a call may be shared with an earlier one or dropped, do **not** rely
 * on a call happening: @c f(++i) still increments, but @c f may not run.
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(unsequenced) (the C23 spelling):
 *       @c [[unsequenced]] suffix — @c __VA_ARGS__ @c [[unsequenced]]
 *  2. MSVC other than clang-cl: identity — no annotation
 *  3. @c __has_cpp_attribute(gnu::const) (GCC, Clang):
 *       @c [[gnu::const]] prefix — @c [[gnu::const]] @c __VA_ARGS__
 *  4. @c __has_attribute(const) (older GCC and Clang):
 *       @c __attribute__((const)) prefix
 *  5. Fallback: identity — no annotation
 */

/**
 * @def SCL_REPRODUCIBLE(...)
 * @brief Wrapping macro that marks a function as having no observable side
 *        effects but which may read mutable global or pointer-reachable memory
 *        — calls with identical visible state may be merged.
 * @ingroup scl_utility_attribute
 * @details
 * Corresponds to the C23 @c [[reproducible]] attribute and the GCC/Clang
 * @c [[gnu::pure]] / @c __attribute__((pure)) extensions.
 * Weaker than @ref SCL_UNSEQUENCED — the function may read global or
 * pointer-reachable memory but must not modify any state observable outside it.
 *
 * **Usage — wrap the entire return-type + declarator:**
 * @code{.cpp}
 * SCL_REPRODUCIBLE(std::size_t strlen_safe(const char* s));
 * SCL_REPRODUCIBLE(bool is_sorted(const int* data, std::size_t n));
 * @endcode
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(reproducible) (the C23 spelling):
 *       @c [[reproducible]] suffix — @c __VA_ARGS__ @c [[reproducible]]
 *  2. MSVC other than clang-cl: identity — no annotation
 *  3. @c __has_cpp_attribute(gnu::pure) (GCC, Clang):
 *       @c [[gnu::pure]] prefix
 *  4. @c __has_attribute(pure) (older GCC and Clang):
 *       @c __attribute__((pure)) prefix
 *  5. Fallback: identity — no annotation
 */

#ifndef SCL_UNSEQUENCED
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(unsequenced)
#define SCL_UNSEQUENCED(...) __VA_ARGS__ [[unsequenced]]
#endif
#endif
#endif

#ifndef SCL_UNSEQUENCED
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_UNSEQUENCED(...) __VA_ARGS__
#endif
#endif

#ifndef SCL_UNSEQUENCED
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(gnu::const)
#define SCL_UNSEQUENCED(...) [[gnu::const]] __VA_ARGS__
#endif
#endif
#endif

#ifndef SCL_UNSEQUENCED
#ifdef __has_attribute
#if __has_attribute(const)
#define SCL_UNSEQUENCED(...) __attribute__((const)) __VA_ARGS__
#endif
#endif
#endif

#ifndef SCL_UNSEQUENCED
#define SCL_UNSEQUENCED(...) __VA_ARGS__
#endif

#ifndef SCL_REPRODUCIBLE
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(reproducible)
#define SCL_REPRODUCIBLE(...) __VA_ARGS__ [[reproducible]]
#endif
#endif
#endif

#ifndef SCL_REPRODUCIBLE
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_REPRODUCIBLE(...) __VA_ARGS__
#endif
#endif

#ifndef SCL_REPRODUCIBLE
#ifdef __has_cpp_attribute
#if __has_cpp_attribute(gnu::pure)
#define SCL_REPRODUCIBLE(...) [[gnu::pure]] __VA_ARGS__
#endif
#endif
#endif

#ifndef SCL_REPRODUCIBLE
#ifdef __has_attribute
#if __has_attribute(pure)
#define SCL_REPRODUCIBLE(...) __attribute__((pure)) __VA_ARGS__
#endif
#endif
#endif

#ifndef SCL_REPRODUCIBLE
#define SCL_REPRODUCIBLE(...) __VA_ARGS__
#endif
