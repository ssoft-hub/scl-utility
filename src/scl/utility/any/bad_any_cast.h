#pragma once

/**
 * @file bad_any_cast.h
 * @brief Exception thrown by a failed ScL Any cast.
 * @ingroup scl_utility_any
 */

#include <typeinfo>

namespace scl
{
    // Never std::bad_any_cast under RTTI: a base type that depends on SCL_HAS_RTTI
    // is an ODR trap for a binary linking RTTI and -fno-rtti translation units.
    struct bad_any_cast : ::std::bad_cast
    {};
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/**
 * @struct scl::bad_any_cast
 * @ingroup scl_utility_any
 * @brief Exception thrown by the throwing form of @ref scl::any_cast on a type
 *        mismatch
 *
 * Derives from `std::bad_cast` in every configuration — never from
 * `std::bad_any_cast`, so this class stays a single, RTTI-independent type and
 * a `catch (std::bad_cast const &)` handler catches it regardless of build.
 */
