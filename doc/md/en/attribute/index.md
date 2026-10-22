# Attribute

The macros of the attribute group expand to an attribute or a compiler hint in the form the compiler
at hand accepts, and to nothing, or to a plain substitute such as `inline` or `((void)0)`, where
a compiler has no such attribute. One macro is written instead of a chain of `#if` over compilers.

- Header: `#include <scl/utility/attribute.h>`

| Header | Page | Defines |
|---|---|---|
| `<scl/utility/attribute/assume.h>` | [`SCL_ASSUME`](assume.md) | `SCL_ASSUME`, `SCL_UNREACHABLE` |
| `<scl/utility/attribute/deprecated.h>` | [`SCL_DEPRECATED`](deprecated.md) | `SCL_DEPRECATED`, `SCL_DEPRECATED_MSG` |
| `<scl/utility/attribute/fallthrough.h>` | [`SCL_FALLTHROUGH`](fallthrough.md) | `SCL_FALLTHROUGH` |
| `<scl/utility/attribute/inline.h>` | [`SCL_FORCE_INLINE`](inline.md) | `SCL_FORCE_INLINE`, `SCL_NOINLINE` |
| `<scl/utility/attribute/hotcold.h>` | [`SCL_HOT`](hotcold.md) | `SCL_HOT`, `SCL_COLD` |
| `<scl/utility/attribute/indeterminate.h>` | [`SCL_INDETERMINATE`](indeterminate.md) | `SCL_INDETERMINATE` |
| `<scl/utility/attribute/lifetimebound.h>` | [`SCL_LIFETIMEBOUND`](lifetimebound.md) | `SCL_LIFETIMEBOUND` |
| `<scl/utility/attribute/likely.h>` | [`SCL_LIKELY`](likely.md) | `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR` |
| `<scl/utility/attribute/maybe_unused.h>` | [`SCL_MAYBE_UNUSED`](maybe_unused.md) | `SCL_MAYBE_UNUSED` |
| `<scl/utility/attribute/nodiscard.h>` | [`SCL_NODISCARD`](nodiscard.md) | `SCL_NODISCARD`, `SCL_NODISCARD_MSG` |
| `<scl/utility/attribute/noreturn.h>` | [`SCL_NORETURN`](noreturn.md) | `SCL_NORETURN` |
| `<scl/utility/attribute/no_unique_address.h>` | [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) | `SCL_NO_UNIQUE_ADDRESS` |
| `<scl/utility/attribute/unsequenced.h>` | [`SCL_UNSEQUENCED`](unsequenced.md) | `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE` |

## Two kinds of macros

Each macro of the group carries either a fact about the code or a hint to the optimizer.

| Kind | Macros | What decides where it is written |
|---|---|---|
| A fact about the code | `SCL_DEPRECATED`, `SCL_FALLTHROUGH`, `SCL_LIFETIMEBOUND`, `SCL_MAYBE_UNUSED`, `SCL_NODISCARD`, `SCL_NORETURN`, `SCL_NO_UNIQUE_ADDRESS` | what the declaration means: a result that must be read, a function that never returns |
| A hint to the optimizer | `SCL_ASSUME`, `SCL_UNREACHABLE`, `SCL_FORCE_INLINE`, `SCL_NOINLINE`, `SCL_HOT`, `SCL_COLD`, `SCL_INDETERMINATE`, `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`, `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE` | a measurement at the place the hint is written |

The optimizer acts on a hint at the place the hint is written, and on one compiler the same hint can
make one place faster and another slower. A gain in speed is promised on no page of the group.
The gain of a hint is known only from a measurement of a program, for the places measured and for no
other.

Some macros of both kinds carry a promise, which the compiler takes as true and does not check.
The behaviour of the program is undefined where such a promise is false and the expansion carries
the attribute or the hint of the promise, so a promise must be kept on every compiler. The promise
of each such macro and the rows of its table that carry it are stated on its page.

The macro `SCL_FORCE_INLINE` is a hint that also carries `inline`, so a function defined in a header
with it may be defined in every translation unit in which the header is included.

## Replacing a macro

Every macro of the group is defined under `#ifndef`, so a definition made before the first header of
the module that is included stays, and that definition is expanded in every header of the module.
The definition must be spelled as a row of the table of the macro, a row whose condition holds on
the compiler at hand, or as the last row of that table. The build with any other text ends with
an error of a `static_assert` whose message holds the name of the macro. Spaces count only between
two identifiers, so `[[ __gnu__::__hot__ ]]` and `[[__gnu__::__hot__]]` are spellings of one row.

A definition on the command line of the compiler, such as `-DSCL_FORCE_INLINE=inline -DSCL_HOT=`, is
seen in every translation unit at once. `inline` stays in such a build, and the two hints are
dropped. `SCL_DEPRECATED_MSG` and `SCL_NODISCARD_MSG` expand to the definition of `SCL_DEPRECATED`
and `SCL_NODISCARD` where only the base macro is defined before the header, and the message is then
lost.

Each macro must have the same definition in every translation unit of one program. Two definitions
of one class, inline function or template that differ in an attribute violate the one definition
rule, and no diagnostic is required.

## Macros of the caller

Defining a macro named like an attribute token used in the group, such as `noinline`, `hot`, `msvc`
or the C23 name `unsequenced`, is not recommended.

The expansions of the group are not changed by such a macro on GCC and Clang. The tokens of GCC and
Clang and their namespaces are written with underscores, `[[__gnu__::__noinline__]]` and
`__attribute__((__hot__))`. The C23 names `unsequenced` and `reproducible` and the C++26 name
`indeterminate` are tested only where they are no macro, and the name `assume` only after C++20,
where it is reserved.

MSVC and the MSVC ABI have no spelling of an attribute that a macro of the caller leaves unchanged,
so the build ends with an error in the cases of the table below, where the attribute would otherwise
be lost. The macro of the group to define as empty before the header is named in the message of
the error, and the attribute is then not applied on MSVC.

| A macro of the caller | Where the build ends with an error | Definition to make before the header |
|---|---|---|
| `msvc` | for the MSVC ABI: MSVC, clang-cl and Clang | `SCL_NO_UNIQUE_ADDRESS`, as an empty macro in every translation unit of the program |
| `msvc` or `lifetimebound` | on MSVC | `SCL_LIFETIMEBOUND`, as an empty macro |
| `noinline` | on MSVC | `SCL_NOINLINE`, as an empty macro |

The names of the standard attributes, such as `nodiscard` or `noreturn`, must not be defined as
macros. The names `likely` and `unlikely` are the exception: they may be function-like macros, and
neither is invoked in the expansion of `SCL_LIKELY` or `SCL_UNLIKELY`.

## Documentation of the group

### Macros

- [`SCL_ASSUME`, `SCL_UNREACHABLE`](assume.md) - a condition the optimizer takes as true, and
  a point execution never reaches
- [`SCL_DEPRECATED`, `SCL_DEPRECATED_MSG`](deprecated.md) - an entity the compiler warns about at
  every use
- [`SCL_FALLTHROUGH`](fallthrough.md) - a case of a `switch` that continues into the next one
- [`SCL_FORCE_INLINE`, `SCL_NOINLINE`](inline.md) - a function the compiler is asked to inline at
  every call, and a function it is asked never to inline
- [`SCL_HOT`, `SCL_COLD`](hotcold.md) - a function called often, and a function called rarely
- [`SCL_INDETERMINATE`](indeterminate.md) - a variable written before every read, which needs no
  erroneous value
- [`SCL_LIFETIMEBOUND`](lifetimebound.md) - a result that must not outlive an argument
- [`SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`](likely.md) - the branch
  a program usually takes
- [`SCL_MAYBE_UNUSED`](maybe_unused.md) - an entity a build may leave unused
- [`SCL_NODISCARD`, `SCL_NODISCARD_MSG`](nodiscard.md) - a result the compiler warns about where
  a caller discards it
- [`SCL_NORETURN`](noreturn.md) - a function that never returns to its caller
- [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) - an empty member that may take no storage of its
  own
- [`SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE`](unsequenced.md) - a function whose calls the optimizer may
  merge or drop

### Examples

- [`example/attribute/assume`](../../../../example/attribute/assume/attribute_assume_example.cpp)
- [`example/attribute/deprecated`](../../../../example/attribute/deprecated/attribute_deprecated_example.cpp)
- [`example/attribute/fallthrough`](../../../../example/attribute/fallthrough/attribute_fallthrough_example.cpp)
- [`example/attribute/hotcold`](../../../../example/attribute/hotcold/attribute_hotcold_example.cpp)
- [`example/attribute/indeterminate`](../../../../example/attribute/indeterminate/attribute_indeterminate_example.cpp)
- [`example/attribute/inline`](../../../../example/attribute/inline/attribute_inline_example.cpp)
- [`example/attribute/lifetimebound`](../../../../example/attribute/lifetimebound/attribute_lifetimebound_example.cpp)
- [`example/attribute/likely`](../../../../example/attribute/likely/attribute_likely_example.cpp)
- [`example/attribute/maybe_unused`](../../../../example/attribute/maybe_unused/attribute_maybe_unused_example.cpp)
- [`example/attribute/no_unique_address`](../../../../example/attribute/no_unique_address/attribute_no_unique_address_example.cpp)
- [`example/attribute/nodiscard`](../../../../example/attribute/nodiscard/attribute_nodiscard_example.cpp)
- [`example/attribute/noreturn`](../../../../example/attribute/noreturn/attribute_noreturn_example.cpp)
- [`example/attribute/unsequenced`](../../../../example/attribute/unsequenced/attribute_unsequenced_example.cpp)

---

Next: [`SCL_ASSUME`](assume.md) | [Back to the main page](../Main.md)
