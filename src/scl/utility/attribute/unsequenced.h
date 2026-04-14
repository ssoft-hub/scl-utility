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
 *        reordered, deduplicated, or eliminated.
 * @ingroup scl_utility_attribute
 * @details
 * Corresponds to the GCC/Clang @c [[gnu::const]] / @c __attribute__((const))
 * extension, which is the pre-C++26 equivalent of @c [[unsequenced]].
 *
 * **Usage — wrap the entire return-type + declarator:**
 * @code{.cpp}
 * SCL_UNSEQUENCED(int square(int x)) { return x * x; }
 * SCL_UNSEQUENCED(std::size_t popcount(std::uint64_t v));
 * @endcode
 *
 * The macro inserts the attribute in the position appropriate for each form:
 * @code{.cpp}
 * int square(int x) [[unsequenced]] { ... }        // C++26  (suffix)
 * [[gnu::const]] int square(int x) { ... }         // GCC / Clang (prefix)
 * __attribute__((const)) int square(int x) { ... } // older GCC / Clang
 *             int square(int x) { ... }            // MSVC / unknown
 * @endcode
 *
 * On C++26 the attribute is appended as a suffix after @c __VA_ARGS__ and
 * before the function body — the body @c { ... } is not part of the macro
 * argument, so the expansion is valid in both declaration and definition.
 *
 * A function decorated with @c SCL_UNSEQUENCED must satisfy all of:
 * - Does not modify any state observable outside the function.
 * - Does not read any mutable global or thread-local state.
 * - Return value depends solely on the arguments.
 *
 * Because calls may be deduplicated, do **not** pass arguments with side
 * effects (e.g. @c f(++i)).
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(unsequenced) (C++26):
 *       @c [[unsequenced]] suffix — @c __VA_ARGS__ @c [[unsequenced]]
 *  2. @c __has_cpp_attribute(gnu::const) (GCC, Clang):
 *       @c [[gnu::const]] prefix — @c [[gnu::const]] @c __VA_ARGS__
 *  3. @c __has_attribute(const) (GCC, Clang older):
 *       @c __attribute__((const)) prefix
 *  4. Fallback: identity — no annotation
 */

/**
 * @def SCL_REPRODUCIBLE(...)
 * @brief Wrapping macro that marks a function as having no observable side
 *        effects but which may read mutable global or pointer-reachable memory
 *        — calls with identical visible state may be deduplicated.
 * @ingroup scl_utility_attribute
 * @details
 * Corresponds to the C++26 @c [[reproducible]] attribute and the GCC/Clang
 * @c [[gnu::pure]] / @c __attribute__((pure)) extensions.
 * Weaker than @ref SCL_UNSEQUENCED: the function may read global or
 * pointer-reachable memory but must not modify any state observable outside it.
 *
 * **Usage — wrap the entire return-type + declarator:**
 * @code{.cpp}
 * SCL_REPRODUCIBLE(std::size_t strlen_safe(const char* s));
 * SCL_REPRODUCIBLE(bool is_sorted(const int* data, std::size_t n));
 * @endcode
 *
 * Detection order:
 *  1. @c __has_cpp_attribute(reproducible) (C++26):
 *       @c [[reproducible]] suffix — @c __VA_ARGS__ @c [[reproducible]]
 *  2. @c __has_cpp_attribute(gnu::pure) (GCC, Clang):
 *       @c [[gnu::pure]] prefix
 *  3. @c __has_attribute(pure) (GCC, Clang older):
 *       @c __attribute__((pure)) prefix
 *  4. Fallback: identity — no annotation
 */

#ifndef SCL_UNSEQUENCED
#if __has_cpp_attribute(unsequenced)
#define SCL_UNSEQUENCED(...) __VA_ARGS__ [[unsequenced]]
#elif defined(_MSC_VER) && !defined(__clang__)
#define SCL_UNSEQUENCED(...) __VA_ARGS__
#elif __has_cpp_attribute(gnu::const)
#define SCL_UNSEQUENCED(...) [[gnu::const]] __VA_ARGS__
#elif defined(__has_attribute) && __has_attribute(const)
#define SCL_UNSEQUENCED(...) __attribute__((const)) __VA_ARGS__
#else
#define SCL_UNSEQUENCED(...) __VA_ARGS__
#endif
#endif

#ifndef SCL_REPRODUCIBLE
#if __has_cpp_attribute(reproducible)
#define SCL_REPRODUCIBLE(...) __VA_ARGS__ [[reproducible]]
#elif defined(_MSC_VER) && !defined(__clang__)
#define SCL_REPRODUCIBLE(...) __VA_ARGS__
#elif __has_cpp_attribute(gnu::pure)
#define SCL_REPRODUCIBLE(...) [[gnu::pure]] __VA_ARGS__
#elif defined(__has_attribute) && __has_attribute(pure)
#define SCL_REPRODUCIBLE(...) __attribute__((pure)) __VA_ARGS__
#else
#define SCL_REPRODUCIBLE(...) __VA_ARGS__
#endif
#endif
