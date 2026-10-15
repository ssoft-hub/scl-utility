#pragma once

/**
 * @file
 * @brief Thread support detection (C++20).
 * @ingroup scl_utility_preprocessor
 * @details
 * - ::SCL_HAS_THREADS:
 *     `1` when the standard library supports threads, `0` otherwise.
 */

// The configuration macros of the standard library come with any of its headers.
#include <version>

#if (defined(__GLIBCXX__) && !defined(_GLIBCXX_HAS_GTHREADS)) || \
    defined(_LIBCPP_HAS_NO_THREADS) || (defined(_LIBCPP_HAS_THREADS) && !_LIBCPP_HAS_THREADS)
#define SCL_HAS_THREADS 0
#else
#define SCL_HAS_THREADS 1
#endif

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_HAS_THREADS
 * @ingroup scl_utility_preprocessor
 * @brief Whether the standard library supports threads.
 *
 * Expands to `1` where the standard library declares `std::mutex` and its `std::thread` can start
 * a thread, and `0` where it is built without threads, as the library of `arm-none-eabi` is. It is
 * derived from the configuration of the library: `_GLIBCXX_HAS_GTHREADS` (libstdc++),
 * `_LIBCPP_HAS_NO_THREADS` or `_LIBCPP_HAS_THREADS` (libc++); every other library counts as
 * supporting threads. The value of the standard macro `__STDCPP_THREADS__` follows the target
 * rather than the library, and Clang defines it for `arm-none-eabi` as well, which is why this one
 * exists.
 *
 * Always defined, so a caller tests it with `#if`: a misspelled name then reads as `0` and draws
 * a `-Wundef` warning, while under `#ifdef` it reads as false with no warning.
 *
 * Code built both for a library with threads and for one without them branches on the macro:
 *
 * @snippet preprocessor/threads/preprocessor_threads_example.cpp next_ticket
 *
 * @see SCL_HAS_RTTI, SCL_HAS_EXCEPTIONS - their values follow the translation unit rather than
 *      the library
 */
