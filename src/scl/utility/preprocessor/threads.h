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
 * Expands to `1` where the standard library declares `std::mutex` and its `std::thread` can start a
 * thread, and `0` where it is built without threads, as the library of `arm-none-eabi` is. It is
 * derived from the configuration of the library: `_GLIBCXX_HAS_GTHREADS` (libstdc++),
 * `_LIBCPP_HAS_NO_THREADS` or `_LIBCPP_HAS_THREADS` (libc++); every other library counts as
 * supporting threads. The standard macro `__STDCPP_THREADS__` reports the target rather than the
 * library, and Clang defines it for `arm-none-eabi` as well, which is why this one exists.
 *
 * Always defined, so interrogate it with `#if`: a misspelled name is then caught by `-Wundef`
 * instead of quietly evaluating to false, as it would under `#ifdef`.
 *
 * The cache of demangled names behind ::scl::type_name(obj) takes a lock only where it is `1`, so
 * code that has to build with a single-thread library branches on the same macro:
 * @code
 * #include <scl/utility/preprocessor/threads.h>
 *
 * #if SCL_HAS_THREADS
 * #include <mutex>
 * #endif
 *
 * int next_ticket()
 * {
 *     static int ticket = 0;
 * #if SCL_HAS_THREADS
 *     static std::mutex mutex;
 *     std::lock_guard const lock{mutex};
 * #endif
 *     return ++ticket;
 * }
 * @endcode
 *
 * @see SCL_HAS_RTTI, SCL_HAS_EXCEPTIONS - report on the translation unit rather than the library
 */
