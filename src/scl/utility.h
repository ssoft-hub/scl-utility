#pragma once

/**
 * @file
 * @brief Includes all ScL Utility headers.
 */

#include <scl/utility/any.h>
#include <scl/utility/attribute.h>
#include <scl/utility/concepts.h>
#include <scl/utility/flags.h>
#include <scl/utility/hash.h>
#include <scl/utility/hierarchy.h>
#include <scl/utility/meta.h>
#include <scl/utility/preprocessor.h>
#include <scl/utility/runtime.h>
#include <scl/utility/type_traits.h>

/**
 * @namespace scl
 * @brief Root namespace of ScL project
 */

/**
 * @mainpage ScL Utility
 *
 * @section intro Overview
 *
 * ScL Utility is a header-only C++20 module providing common utilities not
 * available in the standard library. It is part of the ScL Toolkit and has
 * no external dependencies.
 *
 * @section install Getting the module
 *
 * The module is header-only, so its `src` directory on the include path is enough:
 * @code{.cmake}
 * target_include_directories(your_target PRIVATE path/to/scl-utility/src)
 * @endcode
 *
 * The `scl::utility` target comes from the toolkit. From an installed one:
 * @code{.cmake}
 * find_package(scl REQUIRED COMPONENTS utility)
 * target_link_libraries(your_target PRIVATE scl::utility)
 * @endcode
 *
 * From the super-project built in tree:
 * @code{.cmake}
 * add_subdirectory(path/to/scl-kit)   # or FetchContent_MakeAvailable(scl-kit)
 * target_link_libraries(your_target PRIVATE scl::utility)
 * @endcode
 *
 * The module carries no `CMakeLists.txt` of its own: its CMake entry point is
 * `project/cmake/`, which the super-project adds per module.
 *
 * @code{.cpp}
 * #include <scl/utility.h>            // every group
 * #include <scl/utility/meta.h>       // one group
 * #include <scl/utility/meta/type.h>  // one header
 * @endcode
 *
 * @section start Quick start
 *
 * Six programs, one per topic that reads in a few lines. Each is compiled as an example,
 * so what follows is the code that builds, not a paraphrase of it.
 *
 * Names of types and enumerators at compile time, without RTTI:
 * @snippet quick_start/meta/quick_start_meta_example.cpp quick_start
 *
 * A string as a `switch` label and as a template parameter:
 * @snippet quick_start/hash/quick_start_hash_example.cpp quick_start
 *
 * A type-safe set of scoped-enum values:
 * @snippet quick_start/flags/quick_start_flags_example.cpp quick_start
 *
 * One function accepting any argument type, and one chain choosing the branch:
 * @snippet quick_start/any/quick_start_any_example.cpp quick_start
 *
 * A parent-child tree whose relationships stay queryable and whose changes reach an
 * observer:
 * @snippet quick_start/hierarchy/quick_start_hierarchy_example.cpp quick_start
 *
 * Picking an overload and carrying a value category over:
 * @snippet quick_start/type_traits/quick_start_type_traits_example.cpp quick_start
 *
 * @section features Features
 *
 * - **Any** (@ref scl_utility_any) — erased views over a value: any_view stores one,
 *   any_arg takes one as a parameter and also binds temporaries, any_switch runs the
 *   branch matching the type held, and any_cast recovers the value.
 * - **Attribute** (@ref scl_utility_attribute) — portable macros for
 *   [[no_unique_address]], optimizer hints (SCL_ASSUME, SCL_UNREACHABLE),
 *   and inlining control (SCL_FORCE_INLINE, SCL_NOINLINE).
 * - **Concepts** (@ref scl_utility_concepts) — concept wrappers for
 *   std type traits that have no std concept equivalent: reference, qualifier,
 *   type-category, and type-property classifiers.
 * - **Flags** (@ref scl_utility_flags) — a set of scoped-enum values with set algebra,
 *   set predicates, and a bidirectional range over the values it holds.
 * - **Hash** (@ref scl_utility_hash) — non-cryptographic hashes (fnv1a, djb2, sdbm,
 *   jenkins_ota, siphash) and key, a strongly-typed hash value usable as a case label,
 *   a container key and a non-type template parameter.
 * - **Hierarchy** (@ref scl_utility_hierarchy) — parent-child trees: node, tree with
 *   observer notifications, observer_tuple, and relationship algorithms reachable
 *   through ADL.
 * - **Meta** (@ref scl_utility_meta) — compile-time type, enum, and symbol
 *   name extraction without RTTI, and type_key, an identity key that tells TU-local
 *   types apart.
 * - **Preprocessor** (@ref scl_utility_preprocessor) — macro utilities
 *   including token forwarding (SCL_FORWARD) and a compile-time counter
 *   (SCL_COUNTER_VALUE / SCL_COUNTER_NEXT).
 * - **Runtime** (@ref scl_utility_runtime) — the runtime counterparts of the meta
 *   utilities: type_name(obj) through RTTI, and enum_value for any enum value.
 * - **Type traits** (@ref scl_utility_type_traits) — detection idiom,
 *   forward_like, member pointer decomposition, and overload_cast.
 */
