#pragma once

/**
 * @file
 * @brief The name of an object's type and the number of an enumeration value at run time.
 * @details The header includes every header of the runtime group.
 */

/**
 * @defgroup scl_utility_runtime ScL Runtime Utilities
 * @brief The name of an object's type and the number of an enumeration value at run time.
 * @details
 * The function template ::scl::type_name(obj) returns, through RTTI, the name of the dynamic type
 * of an object of a polymorphic class and the name of the static type of any other object. The
 * function template ::scl::type_short_name(obj) returns that name without qualifiers and template
 * arguments. Both are declared only where the macro `SCL_HAS_RTTI` is `1`. The function template
 * ::scl::enum_string spells the value a variable of an enumeration type holds and needs no RTTI;
 * the concept ::scl::concepts::enum_string_format states the function object it takes.
 * The names a type has at compile time are returned by the function templates of the group @ref
 * scl_utility_meta.
 *
 * A caller whose code is built with and without RTTI should test the macro `SCL_HAS_RTTI`:
 * @snippet runtime/type_name/runtime_type_name_example.cpp no_rtti
 * @{
 */

#include <scl/utility/runtime/enum.h>
#include <scl/utility/runtime/type.h>

/** @} */ // end of group scl_utility_runtime
