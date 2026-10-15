#pragma once

/**
 * @file
 * @brief Whether the translation unit is compiled with RTTI.
 * @ingroup scl_utility_preprocessor
 */

// __has_feature first, as for exceptions: it follows the language option itself.
#ifdef __has_feature
// Nested rather than `defined(__has_feature) && __has_feature(...)`: a compiler without
// __has_feature still parses the whole expression, and the call form is a syntax error there.
#if __has_feature(cxx_rtti)
#define SCL_HAS_RTTI 1
#else
#define SCL_HAS_RTTI 0
#endif
#elif defined(_CPPRTTI) || defined(__GXX_RTTI)
#define SCL_HAS_RTTI 1
#else
#define SCL_HAS_RTTI 0
#endif

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_HAS_RTTI
 * @brief Whether the translation unit is compiled with RTTI enabled.
 * @ingroup scl_utility_preprocessor
 *
 * Expands to `1` with RTTI and `0` without it, derived from `__has_feature(cxx_rtti)` where
 * the compiler offers it (Clang, clang-cl, GCC 14 and later), and otherwise from `_CPPRTTI`
 * (MSVC) or `__GXX_RTTI` (GCC 13). The language offers no standard feature-test macro for RTTI,
 * which is why this one exists.
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
