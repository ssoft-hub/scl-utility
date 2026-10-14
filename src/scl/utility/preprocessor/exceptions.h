#pragma once

/**
 * @file
 * @brief Exception availability detection (C++20).
 * @ingroup scl_utility_preprocessor
 * @details
 * - ::SCL_HAS_EXCEPTIONS:
 *     `1` when the translation unit is compiled with exceptions enabled, `0` otherwise.
 */

// __has_feature first: Clang defines __EXCEPTIONS under -fexceptions -fno-cxx-exceptions, where
// C++ exceptions stay off.
#ifdef __has_feature
// Nested rather than `defined(__has_feature) && __has_feature(...)`: a compiler without
// __has_feature still parses the whole expression, and the call form is a syntax error there.
#if __has_feature(cxx_exceptions)
#define SCL_HAS_EXCEPTIONS 1
#else
#define SCL_HAS_EXCEPTIONS 0
#endif
#elif defined(_CPPUNWIND) || defined(__EXCEPTIONS)
#define SCL_HAS_EXCEPTIONS 1
#else
#define SCL_HAS_EXCEPTIONS 0
#endif

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_HAS_EXCEPTIONS
 * @ingroup scl_utility_preprocessor
 * @brief Whether the translation unit is compiled with exceptions enabled.
 *
 * Expands to `1` with exceptions and `0` without them, derived from
 * `__has_feature(cxx_exceptions)` where the compiler offers it (Clang, clang-cl, GCC 14 and
 * later), and otherwise from `_CPPUNWIND` (MSVC) or `__EXCEPTIONS` (GCC 13).
 * The language offers no standard feature-test macro for exceptions, which is why this one exists.
 *
 * Always defined, so a caller tests it with `#if`: a misspelled name then reads as `0` and draws
 * a `-Wundef` warning, while under `#ifdef` it reads as false with no warning.
 *
 * Code built both with exceptions and without them branches on the macro:
 *
 * @snippet preprocessor/exceptions/preprocessor_exceptions_example.cpp text_of
 *
 * @see SCL_HAS_RTTI - the same macro for RTTI
 */
