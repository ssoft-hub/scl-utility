# Changelog

All notable changes to the ScL Utility module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- [any] Added `scl::any` and `basic_any`, owning holders with no RTTI that work in constant evaluation
- [any] Added `scl::any_view`, a non-owning view that reads a value or a `std::any`
- [any] Added `scl::any_mutable_view`, a non-owning view that writes through
- [any] Added `scl::any_arg`, a parameter-only handle that binds any value for one call
- [any] Added `scl::any_cast` and `scl::bad_any_cast`, the cast of every any handle and its failure
- [any] Added `scl::any_switch`, a chain of typed branches over an any handle
- [any] Added `scl::any_anchor`, which lets a handle cast in constant evaluation on C++20
- [concepts] Added concepts for the standard type traits that have no standard concept
- [flags] Added `scl::flags`, a set of enumeration values usable in constant evaluation
- [hash] Added `scl::hash::byte_view`, presenting wider elements as the bytes a hash takes
- [hash] Added `hashable_range` and `byte_element`, stating what a hash function accepts
- [hierarchy] Added `scl::hierarchy::node` and `tree`, a tree that notifies an observer
- [hierarchy] Added `observer_tuple` and the ADL algorithms such as `parent_of`
- [meta] Added `scl::type_key`, an identity of a type that needs no RTTI
- [meta] Added `scl::is_tu_local`, telling a type declared in an anonymous namespace
- [preprocessor] Added `SCL_HAS_RTTI` and `SCL_HAS_EXCEPTIONS`, telling what a build enables
- [runtime] Added `scl::type_name(object)` and `type_short_name(object)` for a dynamic type
- [runtime] Added `scl::enum_value`, rendering an enumerator as `TypeName::N`
- Added runnable examples, a Quick start among them, each linked from the page of its subject
- Added a version selector to the published reference, with one version per release

### Changed

- [hash] Broke `fnv1a("text")`: every hash function and `key` refuse a bounded array
- [hash] Refused a range of elements wider than a byte, a breaking change
- [hash] Took the range of every hash function as a forwarding reference, so `fnv1a<T>(lvalue)` breaks
- [hash] Made `noexcept` of every hash function, hasher and `key` follow the range it reads
- Marked the entry points of `hash`, `meta`, `forward_like` and `overload_cast` `[[nodiscard]]`
- Documented every public member in the reference, each with a detailed entry
- Moved the Markdown guides from `doc/<language>/` to `doc/md/<language>/`

### Fixed

- [attribute] Preprocessed every attribute header on a compiler lacking a `__has_*` operator
- [attribute] Kept a caller's predefined macro, such as `SCL_NODISCARD_MSG` alone, on inclusion
- [attribute] Applied `SCL_FORCE_INLINE` and `SCL_NOINLINE` where `__has_cpp_attribute` misses `gnu::`
- [hash] Accepted in `djb2` and `sdbm` a range whose end is a sentinel of its own type
- [hash] Accepted a view that cannot be read through a `const` reference
- [hash] Accepted `std::byte` and other single-byte trivially copyable elements
- [meta] Documented that MSVC keeps the class key in `type_name`, a name to show, not compare
- Included `hash.h` in `<scl/utility.h>`, which left the hash group out
- Fixed the installation steps, which added a directory with no `CMakeLists.txt`

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
  - `key<Hasher>` — strongly-typed wrapper over a hash value; usable as
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
