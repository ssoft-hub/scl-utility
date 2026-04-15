#pragma once

/**
 * @file
 * @brief ScL portable compiler attribute macros.
 * @details This header aggregates all ScL attribute macros.
 */

/**
 * @defgroup scl_utility_attribute ScL Attribute Macros
 * @brief Portable macros for compiler-specific attributes and optimizer hints.
 * @details
 * - Layout control: SCL_NO_UNIQUE_ADDRESS.
 * - Optimizer hints: SCL_ASSUME, SCL_UNREACHABLE.
 * - Inlining control: SCL_FORCE_INLINE, SCL_NOINLINE.
 * - Control flow: SCL_NORETURN, SCL_FALLTHROUGH.
 * - Diagnostics: SCL_DEPRECATED, SCL_DEPRECATED_MSG,
 *                SCL_NODISCARD, SCL_NODISCARD_MSG, SCL_MAYBE_UNUSED.
 * - Branch prediction: SCL_LIKELY, SCL_UNLIKELY,
 *                      SCL_LIKELY_EXPR, SCL_UNLIKELY_EXPR.
 * - Pure functions: SCL_UNSEQUENCED, SCL_REPRODUCIBLE.
 * - Uninitialised storage: SCL_INDETERMINATE.
 * - Function frequency: SCL_HOT, SCL_COLD.
 * - Lifetime safety: SCL_LIFETIMEBOUND.
 * - All macros degrade gracefully on unknown compilers.
 * @{
 */

#include <scl/utility/attribute/assume.h>
#include <scl/utility/attribute/deprecated.h>
#include <scl/utility/attribute/fallthrough.h>
#include <scl/utility/attribute/hotcold.h>
#include <scl/utility/attribute/indeterminate.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/likely.h>
#include <scl/utility/attribute/maybe_unused.h>
#include <scl/utility/attribute/no_unique_address.h>
#include <scl/utility/attribute/nodiscard.h>
#include <scl/utility/attribute/noreturn.h>
#include <scl/utility/attribute/unsequenced.h>

/** @} */ // end of group scl_utility_attribute
