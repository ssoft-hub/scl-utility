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
 * Expands to `1` with RTTI and `0` without it, derived from whichever signal the
 * compiler offers: `_CPPRTTI` (MSVC), `__GXX_RTTI` (GCC and Clang in GNU mode) or
 * `__has_feature(cxx_rtti)` (Clang). The language offers no standard feature-test
 * macro for RTTI, which is why this one exists.
 *
 * Always defined, so interrogate it with `#if` — a misspelled name is then caught by
 * `-Wundef` instead of quietly evaluating to false, as it would under `#ifdef`.
 *
 * ScL declarations that need `typeid` are gated on it, so code portable across both
 * configurations branches on the same macro:
 * @code
 * #include <scl/utility/preprocessor/rtti.h>
 *
 * #if SCL_HAS_RTTI
 * #include <scl/utility/runtime/type.h>
 * std::string name_of(auto const & object) { return ::scl::type_name(object); }
 * #else
 * std::string name_of(auto const &) { return "<unknown>"; }
 * #endif
 * @endcode
 */
