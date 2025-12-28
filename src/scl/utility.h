#pragma once

/**
 * @file
 * @brief Includes all ScL.Utility headers.
 */

#include <scl/utility/preprocessor.h>
#include <scl/utility/type_traits.h>

/**
 * @namespace scl
 * @brief Root namespace of ScL project
 */

/**
 * @mainpage ScL.Utility
 *
 * @section intro Overview
 *
 * ScL.Utility is a header-only C++20 module providing common utilities not
 * available in the standard library. It is part of the ScL toolkit and has
 * no external dependencies.
 *
 * @section start Quick start
 *
 * Include the umbrella header and link against the CMake target:
 * @code{.cmake}
 * target_link_libraries(your_target PRIVATE scl::utility)
 * @endcode
 * @code{.cpp}
 * #include <scl/utility.h>
 *
 * static_assert(scl::type_name<int>() == "int");
 * @endcode
 *
 * @section features Features
 *
 * - **Meta** (@ref scl_utility_meta) — compile-time type, enum, and symbol
 *   name extraction without RTTI.
 * - **Preprocessor** (@ref scl_utility_preprocessor) — macro utilities
 *   including token forwarding (SCL_FORWARD) and a compile-time counter
 *   (SCL_COUNTER_VALUE / SCL_COUNTER_NEXT).
 * - **Type traits** (@ref scl_utility_type_traits) — detection idiom,
 *   forward_like, member pointer decomposition, and overload_cast.
 */
