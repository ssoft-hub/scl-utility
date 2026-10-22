#pragma once

/**
 * @file
 * @brief Attributes and compiler hints in the form the compiler at hand accepts.
 * @details The header includes every header of the attribute group.
 */

/**
 * @defgroup scl_utility_attribute ScL Attribute Macros
 * @brief Attributes and compiler hints in the form the compiler at hand accepts.
 * @details
 * The macros of the attribute group expand to an attribute or a compiler hint in the form
 * the compiler at hand accepts, and to nothing, or to a plain substitute such as `inline` or
 * `((void)0)`, where a compiler has no such attribute. One macro is written instead of a chain of
 * `#if` over compilers.
 *
 * # Two kinds of macros {#scl_utility_attribute_two}
 *
 * Each macro of the group carries either a fact about the code or a hint to the optimizer.
 *
 * | Kind | Macros | What decides where it is written |
 * |---|---|---|
 * | A fact about the code | `SCL_DEPRECATED`, `SCL_FALLTHROUGH`, `SCL_LIFETIMEBOUND`, `SCL_MAYBE_UNUSED`, `SCL_NODISCARD`, `SCL_NORETURN`, `SCL_NO_UNIQUE_ADDRESS` | what the declaration means: a result that must be read, a function that never returns |
 * | A hint to the optimizer | `SCL_ASSUME`, `SCL_UNREACHABLE`, `SCL_FORCE_INLINE`, `SCL_NOINLINE`, `SCL_HOT`, `SCL_COLD`, `SCL_INDETERMINATE`, `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`, `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE` | a measurement at the place the hint is written |
 *
 * The optimizer acts on a hint at the place the hint is written, and on one compiler the same hint
 * can make one place faster and another slower. A gain in speed is promised by no macro of
 * the group. The gain of a hint is known only from a measurement of a program, for the places
 * measured and for no other.
 *
 * Some macros of both kinds carry a promise, which the compiler takes as true and does not check.
 * The behaviour of the program is undefined where such a promise is false and the expansion carries
 * the attribute or the hint of the promise, so a promise must be kept on every compiler.
 * The promise of each such macro and the rows of its table that carry it are stated in
 * the description of the macro.
 *
 * The macro `SCL_FORCE_INLINE` is a hint that also carries `inline`, so a function defined in
 * a header with it may be defined in every translation unit in which the header is included.
 *
 * # Replacing a macro {#scl_utility_attribute_replacing}
 *
 * Every macro of the group is defined under `#ifndef`, so a definition made before the first header
 * of the module that is included stays, and that definition is expanded in every header of
 * the module. The definition must be spelled as a row of the table of the macro, a row whose
 * condition holds on the compiler at hand, or as the last row of that table. The build with any
 * other text ends with an error of a `static_assert` whose message holds the name of the macro.
 * Spaces count only between two identifiers, so `[[ __gnu__::__hot__ ]]` and `[[__gnu__::__hot__]]`
 * are spellings of one row.
 *
 * A definition on the command line of the compiler, such as `-DSCL_FORCE_INLINE=inline -DSCL_HOT=`,
 * is seen in every translation unit at once. `inline` stays in such a build, and the two hints are
 * dropped. `SCL_DEPRECATED_MSG` and `SCL_NODISCARD_MSG` expand to the definition of
 * `SCL_DEPRECATED` and `SCL_NODISCARD` where only the base macro is defined before the header, and
 * the message is then lost.
 *
 * Each macro must have the same definition in every translation unit of one program. Two
 * definitions of one class, inline function or template that differ in an attribute violate the one
 * definition rule, and no diagnostic is required.
 *
 * # Macros of the caller {#scl_utility_attribute_macros}
 *
 * Defining a macro named like an attribute token used in the group, such as `noinline`, `hot`,
 * `msvc` or the C23 name `unsequenced`, is not recommended.
 *
 * The expansions of the group are not changed by such a macro on GCC and Clang. The tokens of GCC
 * and Clang and their namespaces are written with underscores, `[[__gnu__::__noinline__]]` and
 * `__attribute__((__hot__))`. The C23 names `unsequenced` and `reproducible` and the C++26 name
 * `indeterminate` are tested only where they are no macro, and the name `assume` only after C++20,
 * where it is reserved.
 *
 * MSVC and the MSVC ABI have no spelling of an attribute that a macro of the caller leaves
 * unchanged, so the build ends with an error in the cases of the table below, where the attribute
 * would otherwise be lost. The macro of the group to define as empty before the header is named in
 * the message of the error, and the attribute is then not applied on MSVC.
 *
 * | A macro of the caller | Where the build ends with an error | Definition to make before the header |
 * |---|---|---|
 * | `msvc` | for the MSVC ABI: MSVC, clang-cl and Clang | `SCL_NO_UNIQUE_ADDRESS`, as an empty macro in every translation unit of the program |
 * | `msvc` or `lifetimebound` | on MSVC | `SCL_LIFETIMEBOUND`, as an empty macro |
 * | `noinline` | on MSVC | `SCL_NOINLINE`, as an empty macro |
 *
 * The names of the standard attributes, such as `nodiscard` or `noreturn`, must not be defined as
 * macros. The names `likely` and `unlikely` are the exception: they may be function-like macros,
 * and neither is invoked in the expansion of `SCL_LIKELY` or `SCL_UNLIKELY`.
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
