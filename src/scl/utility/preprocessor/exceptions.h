#pragma once

/**
 * @file
 * @brief Exception availability detection (C++20).
 * @ingroup scl_utility_preprocessor
 * @details
 * - ::SCL_HAS_EXCEPTIONS:
 *     `1` when the translation unit is compiled with exceptions enabled, `0` otherwise.
 */

#if defined(_CPPUNWIND) || defined(__EXCEPTIONS)
#define SCL_HAS_EXCEPTIONS 1
// Nested rather than `defined(__has_feature) && __has_feature(...)`: a compiler without
// __has_feature still parses the whole expression, and the call form is a syntax error there.
#elif defined(__has_feature)
#if __has_feature(cxx_exceptions)
#define SCL_HAS_EXCEPTIONS 1
#else
#define SCL_HAS_EXCEPTIONS 0
#endif
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
 * Expands to `1` with exceptions and `0` without them, derived from whichever signal
 * the compiler offers: `_CPPUNWIND` (MSVC, clang-cl and Clang for the MSVC target),
 * `__EXCEPTIONS` (GCC and Clang for a GNU target) or `__has_feature(cxx_exceptions)` (Clang).
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
