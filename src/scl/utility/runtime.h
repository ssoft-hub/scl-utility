#pragma once

/**
 * @file
 * @brief ScL runtime type utilities.
 * @details This header aggregates all ScL runtime type utilities.
 */

/**
 * @defgroup scl_utility_runtime ScL Runtime Utilities
 * @brief Names read from a value while the program runs.
 * @details
 * ::scl::type_name(obj) and ::scl::type_short_name(obj) name, through RTTI, the dynamic type
 * of an object of a polymorphic class and the static type of any other object; they are
 * declared only where `SCL_HAS_RTTI` is `1`. ::scl::enum_string spells the number an
 * enumeration value holds and needs no RTTI. The names a type has at compile time are in
 * @ref scl_utility_meta.
 *
 * Code that builds with and without RTTI branches on the macro:
 * @snippet runtime/type_name/runtime_type_name_example.cpp no_rtti
 * @{
 */

#include <scl/utility/runtime/enum.h>
#include <scl/utility/runtime/type.h>

/** @} */ // end of group scl_utility_runtime
