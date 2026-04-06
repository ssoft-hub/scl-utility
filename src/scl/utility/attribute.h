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
 * - All macros degrade gracefully on unknown compilers.
 * @{
 */

#include <scl/utility/attribute/assume.h>
#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/no_unique_address.h>

/** @} */ // end of group scl_utility_attribute
