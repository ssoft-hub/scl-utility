# Changelog

All notable changes to the SCL.Utility module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

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
