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
 * the compiler offers: `_CPPUNWIND` (MSVC), `__EXCEPTIONS` (GCC and Clang) or
 * `__has_feature(cxx_exceptions)` (Clang). The language offers no standard feature-test
 * macro for exceptions, which is why this one exists.
 *
 * Always defined, so interrogate it with `#if` — a misspelled name is then caught by
 * `-Wundef` instead of quietly evaluating to false, as it would under `#ifdef`.
 *
 * ScL declarations that answer a failed request by throwing are gated on it, so code
 * portable across both configurations branches on the same macro:
 * @code
 * #include <scl/utility/any.h>
 * #include <scl/utility/preprocessor/exceptions.h>
 *
 * #if SCL_HAS_EXCEPTIONS
 * std::string const & text = scl::any_cast<std::string const &>(view);
 * #else
 * std::string const & text = *scl::any_cast<std::string>(&view);
 * #endif
 * @endcode
 *
 * @see SCL_HAS_RTTI — the same treatment for RTTI
 */
