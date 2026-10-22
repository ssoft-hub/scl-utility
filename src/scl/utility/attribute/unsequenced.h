#pragma once

/**
 * @file
 * @brief The macros `SCL_UNSEQUENCED` and `SCL_REPRODUCIBLE`.
 * @ingroup scl_utility_attribute
 */

#include <scl/utility/attribute/detail/spelling.h>

// GCC 15 and later report [[unsequenced]] in C++ and ignore it there.
#if !defined(__GNUC__) || defined(__clang__)
#ifdef __has_cpp_attribute
#ifndef unsequenced
#if __has_cpp_attribute(unsequenced)
#define SCL_DETAIL_UNSEQUENCED_C23(...) __VA_ARGS__ [[unsequenced]]
#endif
#endif
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_UNSEQUENCED_MSVC(...) __VA_ARGS__
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__const__)
#define SCL_DETAIL_UNSEQUENCED_GNU(...) [[__gnu__::__const__]] __VA_ARGS__
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__const__)
#define SCL_DETAIL_UNSEQUENCED_ATTRIBUTE(...) __attribute__((__const__)) __VA_ARGS__
#endif
#endif

#define SCL_DETAIL_UNSEQUENCED_FALLBACK(...) __VA_ARGS__

#ifndef SCL_UNSEQUENCED
#ifdef SCL_DETAIL_UNSEQUENCED_C23
#define SCL_UNSEQUENCED(...) SCL_DETAIL_UNSEQUENCED_C23(__VA_ARGS__)
#elif defined(SCL_DETAIL_UNSEQUENCED_MSVC)
#define SCL_UNSEQUENCED(...) SCL_DETAIL_UNSEQUENCED_MSVC(__VA_ARGS__)
#elif defined(SCL_DETAIL_UNSEQUENCED_GNU)
#define SCL_UNSEQUENCED(...) SCL_DETAIL_UNSEQUENCED_GNU(__VA_ARGS__)
#elif defined(SCL_DETAIL_UNSEQUENCED_ATTRIBUTE)
#define SCL_UNSEQUENCED(...) SCL_DETAIL_UNSEQUENCED_ATTRIBUTE(__VA_ARGS__)
#else
#define SCL_UNSEQUENCED(...) SCL_DETAIL_UNSEQUENCED_FALLBACK(__VA_ARGS__)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_UNSEQUENCED(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNSEQUENCED_C23(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNSEQUENCED_MSVC(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNSEQUENCED_GNU(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNSEQUENCED_ATTRIBUTE(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_UNSEQUENCED_FALLBACK(int scl_detail_declarator(int)))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_UNSEQUENCED));

// GCC 15 and later report [[reproducible]] in C++ and ignore it there.
#if !defined(__GNUC__) || defined(__clang__)
#ifdef __has_cpp_attribute
#ifndef reproducible
#if __has_cpp_attribute(reproducible)
#define SCL_DETAIL_REPRODUCIBLE_C23(...) __VA_ARGS__ [[reproducible]]
#endif
#endif
#endif
#endif

#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_DETAIL_REPRODUCIBLE_MSVC(...) __VA_ARGS__
#endif

#ifdef __has_cpp_attribute
#if __has_cpp_attribute(__gnu__::__pure__)
#define SCL_DETAIL_REPRODUCIBLE_GNU(...) [[__gnu__::__pure__]] __VA_ARGS__
#endif
#endif

#ifdef __has_attribute
#if __has_attribute(__pure__)
#define SCL_DETAIL_REPRODUCIBLE_ATTRIBUTE(...) __attribute__((__pure__)) __VA_ARGS__
#endif
#endif

#define SCL_DETAIL_REPRODUCIBLE_FALLBACK(...) __VA_ARGS__

#ifndef SCL_REPRODUCIBLE
#ifdef SCL_DETAIL_REPRODUCIBLE_C23
#define SCL_REPRODUCIBLE(...) SCL_DETAIL_REPRODUCIBLE_C23(__VA_ARGS__)
#elif defined(SCL_DETAIL_REPRODUCIBLE_MSVC)
#define SCL_REPRODUCIBLE(...) SCL_DETAIL_REPRODUCIBLE_MSVC(__VA_ARGS__)
#elif defined(SCL_DETAIL_REPRODUCIBLE_GNU)
#define SCL_REPRODUCIBLE(...) SCL_DETAIL_REPRODUCIBLE_GNU(__VA_ARGS__)
#elif defined(SCL_DETAIL_REPRODUCIBLE_ATTRIBUTE)
#define SCL_REPRODUCIBLE(...) SCL_DETAIL_REPRODUCIBLE_ATTRIBUTE(__VA_ARGS__)
#else
#define SCL_REPRODUCIBLE(...) SCL_DETAIL_REPRODUCIBLE_FALLBACK(__VA_ARGS__)
#endif
#endif

static_assert(
    ::scl::attribute::detail::spelled_as_one_of(
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_REPRODUCIBLE(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_REPRODUCIBLE_C23(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_REPRODUCIBLE_MSVC(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_REPRODUCIBLE_GNU(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_REPRODUCIBLE_ATTRIBUTE(int scl_detail_declarator(int))),
        SCL_DETAIL_ATTRIBUTE_TEXT(SCL_DETAIL_REPRODUCIBLE_FALLBACK(int scl_detail_declarator(int)))),
    SCL_DETAIL_ATTRIBUTE_MESSAGE(SCL_REPRODUCIBLE));

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_UNSEQUENCED(...)
 * @brief The result of the marked function depends on its arguments alone.
 * @ingroup scl_utility_attribute
 * @details
 * - **What the compiler is told by `SCL_UNSEQUENCED`:** the result depends on the arguments alone.
 *   The function changes nothing observable by a caller and reads no memory that may change between
 *   two calls, memory reached through a pointer or a reference argument included. GCC and Clang may
 *   merge two calls with equal arguments, move a call out of a loop and drop a call whose result
 *   goes unread.
 * - **What the compiler is told by `SCL_REPRODUCIBLE`:** the function changes nothing observable by
 *   a caller, and may read global memory and memory a pointer or a reference argument refers to.
 *   The optimizer may merge two calls with no write to memory between them.
 * - **A false promise:** the behaviour of the program is undefined where the expansion is taken
 *   from a row of the table that carries an attribute and the function changes anything observable
 *   by a caller, or reads under `SCL_UNSEQUENCED` memory that may change between two calls.
 * - **An argument with a side effect:** the increment of `f(++i)` happens, yet the call of `f` may
 *   be merged with an earlier one or dropped.
 * - **The C23 spelling:** C23 has the related attributes `[[unsequenced]]` and `[[reproducible]]`,
 *   written after the declarator, with promises of their own. No revision of C++ has taken them.
 *   The first row of each table is there for a compiler that offers them in C++, and under that row
 *   the compiler is told the C23 promise instead of the one above.
 *
 * | Condition, first match | `SCL_UNSEQUENCED(...)` |
 * |---|---|
 * | `__has_cpp_attribute(unsequenced)`, not GCC, `unsequenced` not a macro | `__VA_ARGS__ [[unsequenced]]` |
 * | MSVC other than clang-cl | `__VA_ARGS__` |
 * | `__has_cpp_attribute(__gnu__::__const__)` (GCC, Clang) | `[[__gnu__::__const__]] __VA_ARGS__` |
 * | `__has_attribute(__const__)` | `__attribute__((__const__)) __VA_ARGS__` |
 * | none of the above | `__VA_ARGS__` |
 *
 * | Condition, first match | `SCL_REPRODUCIBLE(...)` |
 * |---|---|
 * | `__has_cpp_attribute(reproducible)`, not GCC, `reproducible` not a macro | `__VA_ARGS__ [[reproducible]]` |
 * | MSVC other than clang-cl | `__VA_ARGS__` |
 * | `__has_cpp_attribute(__gnu__::__pure__)` (GCC, Clang) | `[[__gnu__::__pure__]] __VA_ARGS__` |
 * | `__has_attribute(__pure__)` | `__attribute__((__pure__)) __VA_ARGS__` |
 * | none of the above | `__VA_ARGS__` |
 *
 * The macros expand to `__VA_ARGS__` on MSVC, and to `[[__gnu__::__const__]] __VA_ARGS__` and
 * `[[__gnu__::__pure__]] __VA_ARGS__` on GCC and Clang. Clang 22 reports neither C23 attribute in
 * C++. GCC 15 and later report both and ignore them in C++ with a warning, so the first row is not
 * taken on GCC.
 *
 * A definition of a macro made before its header is kept only as one of the expansions below for
 * the compiler at hand. The build with any other text ends with an error, as described in
 * @ref scl_utility_attribute_replacing.
 *
 * | Macro | GCC | Clang, clang-cl | MSVC |
 * |---|---|---|---|
 * | `SCL_UNSEQUENCED(...)` | `[[__gnu__::__const__]] __VA_ARGS__`, `__attribute__((__const__)) __VA_ARGS__`, `__VA_ARGS__` | `[[__gnu__::__const__]] __VA_ARGS__`, `__attribute__((__const__)) __VA_ARGS__`, `__VA_ARGS__` | `__VA_ARGS__` |
 * | `SCL_REPRODUCIBLE(...)` | `[[__gnu__::__pure__]] __VA_ARGS__`, `__attribute__((__pure__)) __VA_ARGS__`, `__VA_ARGS__` | `[[__gnu__::__pure__]] __VA_ARGS__`, `__attribute__((__pure__)) __VA_ARGS__`, `__VA_ARGS__` | `__VA_ARGS__` |
 *
 * @param ... The return type and the declarator of the function, without the body; the attribute is
 *        placed where each spelling requires it.
 *
 * @snippet attribute/unsequenced/attribute_unsequenced_example.cpp unsequenced
 */

/**
 * @def SCL_REPRODUCIBLE(...)
 * @brief The marked function changes nothing observable by a caller, and may read memory.
 * @ingroup scl_utility_attribute
 * @details
 * @copydetails SCL_UNSEQUENCED
 */
