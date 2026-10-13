#pragma once

/**
 * @file
 * @brief RTTI availability detection (C++20).
 * @ingroup scl_utility_preprocessor
 * @details
 * - ::SCL_HAS_RTTI:
 *     `1` when the translation unit is compiled with RTTI enabled, `0` otherwise.
 */

#if defined(_CPPRTTI) || defined(__GXX_RTTI)
#define SCL_HAS_RTTI 1
// Nested rather than `defined(__has_feature) && __has_feature(...)`: a compiler without
// __has_feature still parses the whole expression, and the call form is a syntax error there.
#elif defined(__has_feature)
#if __has_feature(cxx_rtti)
#define SCL_HAS_RTTI 1
#else
#define SCL_HAS_RTTI 0
#endif
#else
#define SCL_HAS_RTTI 0
#endif

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_HAS_RTTI
 * @ingroup scl_utility_preprocessor
 * @brief Whether the translation unit is compiled with RTTI enabled.
 *
 * Expands to `1` with RTTI and `0` without it, derived from whichever signal
 * the compiler offers: `_CPPRTTI` (MSVC, clang-cl and Clang for the MSVC target), `__GXX_RTTI`
 * (GCC and Clang for a GNU target) or `__has_feature(cxx_rtti)` (Clang). The language offers no
 * standard feature-test macro for RTTI, which is why this one exists.
 *
 * Always defined, so a caller tests it with `#if`: a misspelled name then reads as `0` and draws
 * a `-Wundef` warning, while under `#ifdef` it reads as false with no warning.
 *
 * Code built both with RTTI and without it branches on the macro:
 *
 * @snippet preprocessor/rtti/preprocessor_rtti_example.cpp name_of
 *
 * @see SCL_HAS_EXCEPTIONS - the same macro for exceptions
 */
