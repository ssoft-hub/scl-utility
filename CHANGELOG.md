# Changelog

All notable changes to the ScL Utility module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Fixed

- Fixed a compiler crash (`SIGSEGV` in clang's `Sema`) building
  `utility_type_traits_gtest` on macOS, caused by an oversized test
  translation unit forcing too many template instantiations at once.

### Changed

- Test infrastructure: sources named `*_shared.cpp` under `test/<subdir>/` now build
  into a companion `SHARED` library linked into the subdirectory's test targets,
  enabling tests across a real module (DLL/so) boundary.

### Added

- **Meta** — RTTI-free type identity key (`#include <scl/utility/meta/type_key.h>`):
  - `scl::type_key` — equality-comparable identity key of a type: the pair of its
    `type_name<T>()` string and a per-translation-unit discriminator, encapsulated
    behind a class (private members, deleted default constructor, `name()` accessor;
    `type_key_of` is the sole factory, so hand-built keys cannot exist). For
    namespace-scope types it never falsely matches (same-named anonymous-namespace
    types from different TUs carry different per-TU anchors) and never falsely rejects
    (an external type compares by name across shared-library boundaries, immune to
    unmerged inline instantiations on Windows). Fully usable in constant expressions;
    intentionally unordered, hashing deferred until a real consumer exists. A key must
    not outlive the module that produced it.
  - `scl::type_key_of<T>()` — returns a reference to the per-type `inline constexpr`
    key, so equality against a stored reference short-circuits on address identity;
    keys from another module fall back to content comparison. The key itself is
    non-copyable and non-movable (reference semantics, as `std::type_info`):
    consumers hold `type_key const &` or `type_key const *`.
  - `scl::is_tu_local<T>` / `is_tu_local_v` (`#include <scl/utility/meta/tu_local.h>`) —
    detects types declared in an anonymous namespace (including as a component of a
    compound type). The compiler's anonymous-namespace marker is derived at compile
    time from probe types rather than hardcoded, and the search skips string and
    character literal contents, so a string non-type template parameter spelling the
    marker cannot misclassify an external type.

- **Preprocessor** — RTTI availability as public API
  (`#include <scl/utility/preprocessor/rtti.h>`):
  - `SCL_HAS_RTTI` — `1` when the translation unit is compiled with RTTI, `0`
    otherwise, derived from `_CPPRTTI`, `__GXX_RTTI` or `__has_feature(cxx_rtti)`.
    The language has no standard feature-test macro for RTTI, and ScL declarations
    needing `typeid` are gated on this one, so portable user code branches on the
    same macro. Always defined, so `#if` interrogates it and `-Wundef` catches a
    misspelling. Detection moved here out of `runtime/type.h`, where it used to be
    a side effect of including an unrelated header.

- **Flags** — type-safe bitmask over a scoped enum (`#include <scl/utility/flags.h>`):
  - `scl::flags<Enum, bit_count>` — a bitmask where each flag occupies the bit at
    its enumerator ordinal, backed by `std::array<std::byte>` so every operation
    is usable in constant evaluation (unlike `std::bitset` before C++23). A
    non-scoped `enum` is rejected by `static_assert`; an ordinal `>= bit_count`
    throws `std::out_of_range` (ill-formed in constant evaluation).
  - Construction from zero or more flags, `operator[]` membership test, equality,
    bitwise `~ | & ^` and compound `|= &= ^=` (flags and single-flag forms).
  - `all_of`/`any_of`/`none_of` predicates in variadic-flag and `flags`
    (subset/intersection/disjoint) forms; whole-mask `any`/`none`/`all`.
  - A bidirectional range over the set flags (`begin`/`end`/`rbegin`/`rend`,
    `size`, nested `const_iterator`) that composes with `<ranges>`.

- **Hierarchy** — parent-child tree structures (`#include <scl/utility/hierarchy.h>`):
  - `scl::hierarchy::node<Payload, Allocator>` — a tree node that owns its children
    in a `std::list`; copy deep-clones, move is O(1).
    Provides `push_back`, `push_front`, `insert`, `emplace` variants, `erase`
    (single + range), `take`/`take_first`/`take_last`, 6 `transfer` splice overloads,
    and standard iteration. All methods carry computed `noexcept(…)` specifiers
    with `requires` constraints.
  - `scl::hierarchy::tree<Payload, Observer, Allocator>` — a root-level node list
    that notifies an `Observer` on insert, erase, clear, and payload change.
    `reference`/`const_reference` proxy types expose the same API as `node`.
  - `scl::hierarchy::observer_tuple<Tree, Observers...>` — fan-out adapter that
    forwards each notification to N inner observers via `std::apply`.
  - Free algorithm functions (`#include <scl/utility/hierarchy/algorithm.h>`):
    `are_identical`, `has_parent`, `parent_of`, `is_parent_of`, `is_ancestor_of`,
    `are_sibling` — adapt to any type via ADL (`adl_parent`,
    `adl_has_parent`, `adl_identity`).

- **Concepts** — type-classifying concept wrappers for std type traits that have no
  std concept equivalent (`#include <scl/utility/concepts.h>`):
  - `scl::concepts::reference`, `lvalue_reference`, `rvalue_reference`
  - `scl::concepts::const_type`, `volatile_type`, `cv_type`
  - `scl::concepts::enum_type`, `void_type`, `pointer`, `null_pointer`
  - `scl::concepts::member_pointer`, `member_object_pointer`, `member_function_pointer`
  - `scl::concepts::array_type`, `bounded_array`, `unbounded_array`
  - `scl::concepts::function_type`, `class_type`, `union_type`
  - `scl::concepts::fundamental`, `arithmetic`, `scalar`, `object_type`, `compound`
  - `scl::concepts::trivial`, `trivially_copyable`, `standard_layout`
  - `scl::concepts::empty_type`, `polymorphic`, `abstract_type`, `final_type`, `aggregate`

- **Runtime** — runtime type name extraction via RTTI (`#include <scl/utility/runtime.h>`):
  - `::scl::type_name(obj)` — demangled fully qualified name of the dynamic type;
    uses `abi::__cxa_demangle` on GCC/Clang, `typeid().name()` as-is on MSVC.
  - `::scl::type_short_name(obj)` — unqualified identifier only (no namespaces, no template arguments).
  - Both functions support polymorphism and are available only when RTTI is enabled.
  - `::scl::enum_value(value)` — string representation of an enum value as `"TypeName::N"`,
    where N is the underlying numeric value; does not require RTTI.

- `AGENTS.md` — development guidelines for contributors and agents:
  code conventions, branching rules, required pre-commit checks
  (clang-format, clang-tidy, cppcheck), commit message format,
  Doxygen documentation requirements, and compatibility constraints.

### CI

- `script/ci/deploy_versioned_docs.sh` — builds versioned Doxygen HTML for
  all version tags (immutable) and `dev` (latest); injects version-selector
  nav bar; manages `versions.json` and root `index.html` on the pages branch.
- `.gitlab-ci.yml` pages job: switched to `archlinux:latest` for up-to-date
  Doxygen; triggers on version tags; deploys via `deploy_versioned_docs.sh`.
- `.github/workflows/docs.yml`: runs in `archlinux:latest` container; installs
  git before checkout; deploys to `gh-pages` via `deploy_versioned_docs.sh`.
- `script/ci/mirror_github.sh`: exclude `pages` branch from GitHub mirror;
  preserve `gh-pages` from `--prune`; add `--prune-tags`.

## [0.1.4] - 2026-04-15

### Added

- **Attribute** — portable compiler attribute macros (`#include <scl/utility/attribute.h>`):
  - `SCL_NO_UNIQUE_ADDRESS` — portable `[[no_unique_address]]`; uses
    `[[msvc::no_unique_address]]` on MSVC, `[[no_unique_address]]` elsewhere.
  - `SCL_ASSUME(expr)` — optimizer hint that `expr` is always true;
    `__assume` on MSVC, `__builtin_assume` on Clang, `if (!(expr)) __builtin_unreachable()` on GCC.
  - `SCL_UNREACHABLE()` — marks a code path as unreachable;
    `__assume(0)` on MSVC, `__builtin_unreachable()` on GCC/Clang.
  - `SCL_FORCE_INLINE` / `SCL_NOINLINE` — inlining control via
    `__forceinline`/`__declspec(noinline)` on MSVC, `__attribute__` on GCC/Clang.
  - `SCL_NORETURN` — `[[noreturn]]` (C++11) with MSVC/GCC/Clang fallbacks.
  - `SCL_DEPRECATED` / `SCL_DEPRECATED_MSG(msg)` — `[[deprecated]]` /
    `[[deprecated(msg)]]` (C++14) with MSVC and GCC/Clang fallbacks.
  - `SCL_FALLTHROUGH` — `[[fallthrough]]` (C++17) with GCC attribute fallback
    and `((void)0)` no-op.
  - `SCL_NODISCARD` / `SCL_NODISCARD_MSG(msg)` — `[[nodiscard]]` (C++17) and
    `[[nodiscard(msg)]]` (C++20) with `warn_unused_result` fallback.
  - `SCL_MAYBE_UNUSED` — `[[maybe_unused]]` (C++17) with `__attribute__((unused))` fallback.
  - `SCL_LIKELY` / `SCL_UNLIKELY` — C++20 `[[likely]]`/`[[unlikely]]` statement
    attributes; empty fallback on older compilers.
  - `SCL_LIKELY_EXPR(expr)` / `SCL_UNLIKELY_EXPR(expr)` — expression-level
    branch hints via `__builtin_expect`; identity fallback on MSVC.
  - `SCL_UNSEQUENCED` — pure no-global-read function annotation;
    `[[gnu::const]]` / `__attribute__((const))` on GCC/Clang, maps to C++26
    `[[unsequenced]]` semantics.
  - `SCL_REPRODUCIBLE` — pure read-only-global function annotation;
    `[[gnu::pure]]` / `__attribute__((pure))` on GCC/Clang, maps to C++26
    `[[reproducible]]` semantics.
  - `SCL_INDETERMINATE` — C++26 `[[indeterminate]]` opt-in to uninitialised
    storage; empty fallback.
  - `SCL_HOT` / `SCL_COLD` — function-frequency hints via `[[gnu::hot]]` /
    `[[gnu::cold]]`; empty fallback on MSVC.
  - `SCL_LIFETIMEBOUND` — dangling-reference detection via
    `[[clang::lifetimebound]]`; empty fallback on GCC and MSVC.
- **Hash** — non-cryptographic compile-time hash utilities (`#include <scl/utility/hash.h>`):
  - `fnv1a`, `djb2`, `sdbm` — 64-bit hashes over any byte range; chainable
    via `operator()`.
  - `jenkins_ota` — 32-bit Jenkins one-at-a-time hash.
  - `siphash` — SipHash-2-4 64-bit keyed hash (hash-flooding resistant).
  - `key<Hasher>` — strongly-typed digest wrapping a hash value; usable as
    `switch`/`case` label, STL unordered container key, and non-type template
    parameter (C++20 NTTP).

---

## [0.1.3] - 2026-03-15

### Fixed

- **forward_like** — `add_reference_like_t` now treats non-reference `Base` the same as
  rvalue reference, matching C++23 `std::forward_like` semantics. Previously a non-reference
  `Base` produced a non-reference result, which caused `executor_access::get` to return by
  value via a forwarding reference and triggered a Clang error (dangling rvalue-reference
  member in `wrapper_guard`).
- **CMake** — deferred `gtest_discover_tests` to test time (`DISCOVERY_MODE PRE_TEST`) for
  MinGW compatibility.

---

## [0.1.2] - 2026-03-02

### Fixed

- **Meta** — Clang on Windows now uses the `__PRETTY_FUNCTION__`-based path for
  `type_name`, `enum_name`, and `symbol_name`. Previously, Clang defined `_MSC_VER`
  on Windows but used `[S = ...]` / `[T = ...]` format instead of MSVC angle-bracket
  style, causing prefix/suffix detection to return 0 and produce empty names.
  The `_MSC_VER` guards are now conditioned on `!defined(__clang__)`.
- Updated type_name tests to reflect that Clang omits `struct`/`class` keywords and
  does not add a space before `>>` in nested templates.

### Added

- GitHub mirror CI with automated PR auto-close workflow.
- Doxygen documentation deployed to GitLab Pages and GitHub Pages (badges in README).

---

## [0.1.1] - 2026-02-20

### Fixed

- **Meta** — added `noexcept` specifier to all public and internal `constexpr` functions
  in `type.h`, `enum.h`, `symbol.h`. All functions operate on `std::string_view` and
  cannot throw.
- Corrected documentation inaccuracies in English and Russian versions.

---

## [0.1.0] - 2026-02-15

### Added

- **Meta** — compile-time name extraction utilities:
  - `type_name<T>` / `type_short_name<T>` — type name retrieval with MSVC normalization.
  - `enum_name<V>` / `enum_short_name<V>` — enum member name retrieval.
  - `symbol_name<S>` / `symbol_short_name<S>` — function and data member name retrieval.
- **Preprocessor** — macro utilities:
  - `SCL_FORWARD(...)` — identity variadic macro for token forwarding.
  - `SCL_COUNTER_VALUE(Tag)` / `SCL_COUNTER_NEXT(Tag)` — compile-time counter via ADL.
- **Type traits** — extended traits and helpers:
  - Detection idiom (`is_detected`, `detected_t`, `detected_or`).
  - `forward_like` — forward with value category of another type.
  - `member_like` — member pointer trait decomposition.
  - `overload_cast<Args...>` — overloaded function pointer disambiguation.
- Documentation in English and Russian.
- GoogleTest, doctest, and Catch2 test suites (193 tests).
- CMake build system with MSVC, MinGW, and Clang support.
