#pragma once

/**
 * @file
 * @brief Names of types, constants and symbols, and the identity of types, at compile time.
 * @details The header includes every header of the meta group.
 */

/**
 * @defgroup scl_utility_meta ScL Meta Utilities
 * @brief Names of types, constants and symbols, and the identity of types, at compile time.
 * @details
 * The function templates ::scl::type_name and ::scl::type_short_name return the name of a type,
 * ::scl::enum_name and ::scl::enum_short_name the name of a constant of an enumeration, and
 * ::scl::symbol_name and ::scl::symbol_short_name the name of a function, a variable or
 * a member, all evaluated at compile time without RTTI. The class ::scl::type_key, which
 * ::scl::type_key_of returns, identifies a type, and the class template ::scl::is_tu_local tells
 * whether a type is local to its translation unit. The names a type and an enumeration value
 * have at run time are returned by the function templates of the group @ref scl_utility_runtime.
 * @{
 */

#include <scl/utility/meta/enum.h>
#include <scl/utility/meta/symbol.h>
#include <scl/utility/meta/tu_local.h>
#include <scl/utility/meta/type.h>
#include <scl/utility/meta/type_key.h>

/** @} */
