#pragma once

/**
 * @file
 * @brief ScL preprocessor utilities for C++.
 * @details This header aggregates all ScL preprocessor utilities.
 */

/**
 * @defgroup scl_utility_preprocessor ScL Preprocessor Utilities (C++)
 * @brief Macros of the build, a list passed through a macro and counters of compile time.
 * @details
 * - @ref SCL_HAS_RTTI and @ref SCL_HAS_EXCEPTIONS expand to `1` where the translation unit has RTTI
 *   and exceptions, and @ref SCL_HAS_THREADS where the standard library supports threads.
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
