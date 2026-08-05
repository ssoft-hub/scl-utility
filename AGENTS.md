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
- Test files named `*_gtest.cpp`, `*_doctest.cpp`, or `*_catch2.cpp`; every new public API must have a GoogleTest (`*_gtest.cpp`) test
- No comments unless the WHY is non-obvious
- All source comments and identifiers in **English**

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

Attribute macros are expanded by Doxygen, so `@fn` spells the clean declaration —
`SCL_HOT` and `SCL_LIFETIMEBOUND` never appear in it. The Doxyfile expands every macro
(`MACRO_EXPANSION = YES` with `EXPAND_ONLY_PREDEF = NO`) using the definition found in the
source, reached through `INCLUDE_PATH`, so a new attribute macro needs no entry anywhere
and the macros keep their own `@def` pages — which a `PREDEFINED` override would take
away, its `#ifndef` guard hiding the definition it documents.

Four shapes defeat out-of-line matching in Doxygen 1.15 and 1.16. The first three have a
fix on the declaration side:

- **Two overloads whose parameter lists render the same are one entity.** Parameter
  *names* do not disambiguate, so `any_cast(Wrapper * view)` in one header and
  `any_cast(Wrapper * arg)` in another collapse into a single member and one of the two
  blocks is dropped without a warning. Give the deduced template parameters distinct names
  (`View` against `Wrapper`) so the rendered signatures differ.
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

`WARN_AS_ERROR` stays `NO`, and the gate greps the report instead: `WARN_AS_ERROR` would
also fail on `WARN_IF_UNDOCUMENTED`, and members that never had a block are a coverage gap
with its own gate to come — a different defect from a block that was written and then lost.
Everything Doxygen does report is a finding, so the report is expected to stay empty.

## Do Not
- Add runtime dependencies
- Break C++20 compatibility (MSVC 19.30+, GCC 13+, Clang 16+)
- Commit without running clang-format and clang-tidy
- Add implementation (.cpp) files — library is header-only
