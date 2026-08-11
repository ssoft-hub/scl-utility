# ScL Utility — Agent Instructions

## Overview
C++20 header-only library, part of the [ScL Toolkit](https://gitlab.com/ssoft-scl/scl-kit).
No external dependencies. License: Unlicense.
Provides compile-time meta-programming helpers, preprocessor utilities, extended type traits, and hash utilities.

## Source Layout
```
src/scl/utility/     — public headers
  attribute/         — compiler attribute macros
  hash/              — non-cryptographic hash utilities
  meta/              — type/enum/symbol name extraction
  preprocessor/      — macro utilities
  type_traits/       — extended type traits
test/                — unit tests
example/             — usage examples
benchmark/           - Google Benchmark performance measurements
project/cmake/       — CMakeLists.txt
project/doxygen/     — Doxyfile
```

## Code Conventions
- C++20 standard, header-only (`src/scl/utility/`)
- No external dependencies
- Follow existing naming and style — do not invent new patterns
- All public symbols live in namespace `scl::` or `scl::utility::`
- Every file starts with `#pragma once`
- When adding a new header, include it in the corresponding top-level header (e.g. `src/scl/utility/meta.h`)
- Example, test and benchmark sources are named by the rule in **Source file naming** below; every new public API must have a GoogleTest (`*_gtest.cpp`) test
- No comments unless the WHY is non-obvious
- All source comments and identifiers in **English**
- A `requires` clause goes **after** the declaration, never between the template head and
  the declaration:

```cpp
template <typename Type>
[[nodiscard]]
constexpr Type * any_cast(Type * arg) noexcept
    requires(::std::is_object_v<Type>)
{ ... }
```

  It follows every other part of the declarator — parameter list, `const`, `noexcept`,
  trailing return type — and precedes `= delete` / `= default` and a constructor's
  initialiser list. A `requires` *expression* inside a `concept` is a different construct
  and is unaffected.

### Source file naming

```
example/<group>/<name>/<group>_<name>_example.cpp   ->  target utility_<group>_<name>_example
test/<group>/<subject>[_<aspect>]_<framework>.cpp   ->  target utility_<group>_<framework>
benchmark/<group>/<subject>[_<aspect>]_<tool>.cpp   ->  target utility_<group>_<tool>
```

`<group>` is the header's own directory under `src/scl/utility/`; a header sitting at that
root (`flags.h`) is its own group. `<subject>` is the header the file covers, followed by
an aspect when one header needs several files (`type_key_cross_tu_gtest.cpp`,
`enum_fallback_gtest.cpp`).

Every base name ends in the token naming what the file is built into, and the target ends
in the same token: `example` for an example, `<framework>` for a test — `gtest`, `doctest`,
`catch2`, plus `_shared.cpp` for a companion shared library — and `<tool>` for a benchmark.
In `test/` and `benchmark/` the CMake glob keys on it, and every file of a group builds
into one executable per framework; in `example/` one source tree is one program, so an
example's base name is its whole target name without the `utility_` prefix, and the token
is what keeps that target apart from the test target of the same group. Every target name
is derived from the path, so a rename needs no build-file edit.

Only `example/` is read by Doxygen (`INPUT` and `EXAMPLE_PATH` in
`project/doxygen/Doxyfile`), and `@example` addresses a file by base name alone. That is
why an example carries its group inside the base name and a test does not: two examples
sharing a base name leave the second unreachable, while a test's uniqueness is already
covered by its directory.

- The `<name>` level is always present, with no collapsing when it repeats the group. An
  example covering its group as a whole rather than one subject is named `common`:
  `example/any/common/any_common_example.cpp`.
- Every example needs a directory of its own. All sources under one example root link into
  a single program, so a second `main` beside it is a link error. Directories holding only
  sub-directories are pure grouping, and the tree may nest freely.
- The benchmark `<tool>` token is `gbench`, for Google Benchmark: `benchmark/hash/` builds
  into `utility_hash_gbench`. Benchmarks are not CTest tests and are off by default -
  build them by passing the options to any existing preset, and run them through the
  script beside the build one:

```sh
script/ci/build.sh clang-x64 Release -DSCL_BUILD_BENCHMARKS=ON -DSCL_BUILD_TESTS=OFF
script/ci/run_benchmarks.sh clang-x64
```

  Both scripts live in the super-project, since the benchmark targets are configured
  there. A timing quoted in an issue or MR must come from a Release build.

## Required Checks Before Every Commit
Run on every changed `.h` / `.hpp` file:

```sh
# Format check
clang-format --dry-run --Werror <files>

# Apply format
clang-format -i <files>

# Static analysis
clang-tidy <file> -- -std=c++20 -xc++ -Isrc
cppcheck --enable=warning,style,performance,portability \
  --check-level=exhaustive \
  --max-configs=32 \
  --std=c++20 --language=c++ --inline-suppr \
  --error-exitcode=1 --suppress=missingIncludeSystem \
  -Isrc -UDOXYGEN <files>

# Documentation blocks reach their target
bash script/lint/doxygen.sh
```

The format gate checks `src/`, `test/`, `example/` and `benchmark/`, headers and sources
alike, so a changed `.cpp` is subject to it too. `bash script/lint/clang_format.sh` runs it
over the whole tree the way CI does. clang-tidy and cppcheck scan `src/` headers only.

## Branching
- Branch name format: `{user}/feat/{subject}`, `{user}/fix/{subject}`, `{user}/refactor/{subject}`
- **Never commit directly to `dev` or `main`**
- Every commit must be in a buildable state

## Compatibility
- Supported compilers: MSVC 19.30+, GCC 13+, Clang 16+
- Breaking change = removing or renaming any public API symbol; avoid unless necessary
- Use C++ feature test macros (`__cpp_*`, `__has_cpp_attribute`) to guard functionality dependent on std version
- Do not use compiler-specific extensions directly — abstract them via attribute or type_traits helpers in this library

## Before PR/MR
1. Update `CHANGELOG.md` with a description of the change
2. Ensure all commits are in a buildable state
3. For releases: grep for the old version string and update **all** occurrences — `CHANGELOG.md`, `project/doxygen/Doxyfile` (`PROJECT_NUMBER`), `CMakeLists.txt`, `README.md`

## Commit Message Format
Use Conventional Commits. Language: **English only**.

```
type(scope): short description (max 72 chars)

Body describing WHY the change was made, WHAT problem it solves,
and HOW it was approached. Wrap lines at 72 characters.
```

**Required types:** `feat`, `fix`, `refactor`, `docs`, `test`, `chore`, `ci`, `perf`, `style`

- No `Co-Authored-By` or any co-authorship trailers

Example:
```
feat(hash): add SipHash-2-4 keyed 64-bit hash

SipHash provides hash-flooding resistance missing in fnv1a/djb2.
Implements the reference SipHash-2-4 algorithm with a 128-bit key.
Key is passed as two uint64_t values to avoid struct padding issues.
```

## Doxygen Documentation
Every public header, class, struct, function, and type alias must have a Doxygen comment. Language: **English only**.

- A block spanning more than one line is written as `/** */`, one `*` per line; `///` is
  left to a comment that says everything in a single line, and `///<` to a trailing one
- Use `@brief` for one-line description
- Use `@tparam`, `@param`, `@return` where applicable
- Every entity must declare `@ingroup <group>` matching its thematic group
- Groups are defined with `@defgroup` in the top-level module header or dedicated group header

Example:
```cpp
/// @defgroup ScL_Utility_Hash Hash utilities
/// @ingroup ScL_Utility
/// @{

/// @brief Computes FNV-1a 64-bit hash over a byte range.
/// @ingroup ScL_Utility_Hash
/// @tparam Iter  Input iterator over byte-sized elements.
/// @param  first Begin of range.
/// @param  last  End of range.
/// @return 64-bit FNV-1a digest.
template <typename Iter>
constexpr uint64_t fnv1a(Iter first, Iter last) noexcept;

/// @}
```

### Out-of-line blocks

A header keeps its class bodies clean: the blocks live in the `Documentation` section at
the end of the file and name their target with `@class` / `@fn` / `@typedef` / `@var`.
Doxygen matches such a block by the target's rendered signature, and a block that matches
nothing is dropped — the entity reaches the reference with no description at all. Spell the
declaration the way Doxygen renders it, parameter names included: `node(Arguments &&...)`
matches nothing, `node(Arguments &&... arguments)` matches. Run `script/lint/doxygen.sh`
(a lint job in both CI pipelines) after touching a public header; it fails on every Doxygen
diagnostic, an unattached block and a stale `@param` name alike.

Attribute macros are expanded before matching, so `@fn` spells the clean declaration —
`SCL_HOT` and `SCL_LIFETIMEBOUND` never appear in it. A new attribute macro needs no entry
anywhere for this to hold.

Four shapes defeat out-of-line matching in Doxygen 1.15 and 1.16. The first three have a
fix on the declaration side:

- **Two overloads whose parameter lists render the same are one entity.** Parameter
  *names* do not disambiguate, so `any_cast(Wrapper * view)` in one header and
  `any_cast(Wrapper * arg)` in another collapse into a single member and one of the two
  blocks is dropped without a warning. Give the deduced template parameters distinct names
  so the rendered signatures differ, naming each after the role it plays —
  `ValueArgument`, `LValueArgument`, `ConstLValueArgument` for the three `any_cast` forms
  that read and write through an argument. A trailing `requires` clause tells such
  overloads apart for the compiler but not for Doxygen, so the names are what make each
  addressable from an `@fn` block.
- **A `requires` clause needs each conjunct parenthesised** — `requires(A) && (B)`, not
  `requires A && B`. Doxygen drops the leading `::` of a conjunct that follows `&&`, and
  the mangled clause no longer matches the declaration it came from.
- **A function template whose return type is a dependent east-const pointer or reference
  (`Type const *`) must not be both declared and defined.** Doxygen renders the two with
  different spacing and fails to pair them; a `friend` declaration inside the class is
  declaration enough, so the separate namespace-scope one can go.
- **An overload set told apart only by its template parameter list cannot be addressed
  from outside at all.** `get<I>()` against `get<T>()` render identically, and no `@fn`
  spelling separates them — a template argument list in the name is accepted and silently
  ignored. Document those overloads in place, directly above the declaration.

A member re-exported from a private base with `using` has no unique target either: the
base declares each as a `const` / `const volatile` pair, and the member is left out of the
class page entirely. Keep the `using`, and declare the member for Doxygen alone in a single
`Documentation-only declarations` block at the end of the header, which reopens the class:

```cpp
#ifdef DOXYGEN
namespace scl
{
    class any_view
    {
    public:
        constexpr bool has_value() const noexcept;
    };
} // namespace scl
#endif
```

Name a befriended **class** from the root — `friend class ::scl::hierarchy::tree<Payload,
Observer, Allocator>;`, `template <typename, typename...> friend class ::scl::any_switch;`.
An unqualified one whose name matches a member of the befriending class (`friend class
tree;` inside `tree::reference`, which also has a `tree()` accessor) captures that member's
out-of-line block, and nothing is reported, since the block did reach a target.

A befriended **free function** takes the namespace *without* the leading `::` —
`friend constexpr Type * scl::any_cast(...)`. The leading form loses the function's own
definition instead. Qualifying it at all needs a prior namespace-scope declaration, so a
function returning a dependent east-const pointer stays unqualified — see the third trap
above. A hidden friend, defined in the class body like `type_key::operator==`, cannot be
qualified at all.

Judge coverage from the generated HTML, not from the report: a block can be attached, carry
its text in the XML and still not reach the reader. Compare the `memtitle` count on a class
page against its member rows.

### Visibility

The public API is documented in full — every public member carries a block, deleted and
defaulted special members, type aliases and iterator boilerplate included. A one-line
`@brief` is enough where there is nothing more to say.

Private and protected members stay out of the reference. A block written for one, because a
reader of the code needs it, carries `@internal`.

An internal entity declared in *public* scope — a pattern anchor, an `SCL_DETAIL_*` probe —
takes `@internal` **and** an `EXCLUDE_SYMBOLS` entry. The tag alone is not enough: an
`@internal` block is discarded before it counts as documentation, which leaves the entity
reported as undocumented instead of excluded.

## Do Not
- Add runtime dependencies
- Break C++20 compatibility (MSVC 19.30+, GCC 13+, Clang 16+)
- Commit without running clang-format and clang-tidy
- Add implementation (.cpp) files — library is header-only
