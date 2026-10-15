#pragma once

/**
 * @file
 * @brief Build-configuration macros, a list-forwarding macro and compile-time counters.
 * @details The header includes every header of the preprocessor group.
 */

/**
 * @defgroup scl_utility_preprocessor ScL Preprocessor Utilities
 * @brief Build-configuration macros, a list-forwarding macro and compile-time counters.
 * @details
 * - @ref SCL_HAS_RTTI expands to `1` where the translation unit is compiled with RTTI,
 *   @ref SCL_HAS_EXCEPTIONS where it is compiled with exceptions, and @ref SCL_HAS_THREADS where
 *   the standard library supports threads.
 * - @ref SCL_FORWARD passes a comma-separated list through one parameter of another macro.
 * - @ref SCL_COUNTER_VALUE and @ref SCL_COUNTER_NEXT read and advance a counter per tag type.
 * @{
 */

#include <scl/utility/preprocessor/counter.h>
#include <scl/utility/preprocessor/exceptions.h>
#include <scl/utility/preprocessor/forward.h>
#include <scl/utility/preprocessor/rtti.h>
#include <scl/utility/preprocessor/threads.h>

/** @} */ // end of group scl_utility_preprocessor
