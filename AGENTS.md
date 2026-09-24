# ScL Utility

## Overview

Header-only library of compile-time and low-level utilities for C++20 and later.

## Commands

| Command | Description |
|---------|-------------|
| `script/lint/lint.sh` | Every lint check, from any directory |
| `script/lint/clang_format.sh` | Formatting check alone |
| `script/lint/clang_tidy.sh` | Naming and static analysis, no build tree needed |
| `script/lint/cppcheck.sh` | Static analysis by cppcheck |
| `script/lint/doxygen.sh` | Documentation parse, fails on any diagnostic |
| `script/lint/doc_snippets.sh --write` | Refill snippet blocks from example sources |
| `script/doxygen/build.sh` | Versioned reference into `doc/doxygen/`, from the module root (see Pitfalls) |

## Architecture

| Content | Path |
|---|---|
| Sources | `src/scl/utility/<group>/` |
| Tests | `test/<group>/` |
| Examples | `example/<group>/<name>/` |
| Benchmarks | `benchmark/<group>/` |
| Documentation pages | `doc/md/<language>/<group>/` |
| Lint scripts | `script/lint/` |
| Projects | `project/<tool>/`: `cmake`, `qmake`, `doxygen`, etc. |

## Key Files

- `CONTRIBUTING.md` - rules of development
- `CHANGELOG.md` - an entry for a key change only

## Principles

- Header-only: no `.c` or `.cpp` under `src/`
- Standard library only, no other dependency
- C++20 baseline; a feature beyond C++20 sits behind `__has_include`, `__has_cpp_attribute`
  or a `__cpp_*` macro
- Headers compile with exceptions and RTTI off: `SCL_HAS_EXCEPTIONS`, `SCL_HAS_RTTI` from
  `preprocessor/`
- Groups by logical layer, from the most basic:

| Layer | Groups |
|---|---|
| 0 | `preprocessor`, `attribute` |
| 1 | `type_traits`, `concepts` |
| 2 | `meta`, `runtime` |
| 3 | `hash`, `flags`, `hierarchy`, `any` |

## Finding Related Code

- LSP first: definitions, references, callers
- The umbrella `src/scl/utility/<group>.h` lists every public header of a group
- Grep for exact strings: a macro, an error message, a function name
- Follow includes: `#include <scl/utility/...>` names the group it depends on
- A test is usually named after the header it covers: `hash/djb2.h` -> `test/hash/djb2_gtest.cpp`
- Test files reveal usage patterns and expected behavior

## Validating

```sh
bash script/lint/lint.sh            # every lint check, from any directory
bash script/lint/clang_format.sh    # one of them, when only formatting is in question
```

- Lint scripts need no build tree

## Building

- No top-level `CMakeLists.txt` and no presets: a host project builds the module
- `project/cmake` - the library, target `scl::utility`
- `project/cmake/test` - `utility_<group>_<framework>` per test directory
- `project/cmake/example` - `utility_<group>_<name>_example` per example directory
- `project/cmake/benchmark` - `utility_<group>_gbench`, `utility_<group>_size`
- `project/cmake/test` and `project/cmake/benchmark` rely on the host to find GoogleTest,
  doctest, Catch2 and Google Benchmark; a framework not found gets no target, and
  `utility_<group>_size` needs none
- In a host that builds the tests, one group runs as its own executable,
  `utility_<group>_gtest`, with `--gtest_filter` for a subset; CTest names each GoogleTest test
  `Suite.Test`, so `ctest -R` never matches a GoogleTest target name

## Coding Guidelines

- `lower_case` for namespaces, types, functions, variables, members, aliases
- `CamelCase` for template parameters, `SCL_UPPER_CASE` for macros under `src/`
- East const: `int const & value`, `char const * name`
- `#pragma once`; root namespace `scl`, implementation details in a nested `detail`
- `typename` for a type parameter
- Documentation blocks: `@brief`, `@tparam`, `@param`, `@ingroup scl_utility_<group>` or a
  subgroup of it

## Workflow

- New public header: `src/scl/utility/<group>/<name>.h`, add it to `<group>.h`, test in
  `test/<group>/<name>_gtest.cpp`, then `bash script/lint/lint.sh`
- Edited an example that a page quotes: `bash script/lint/doc_snippets.sh --write`

## Code Quality

| Tool | Configuration | Script |
|---|---|---|
| clang-format | `.clang-format` | `script/lint/clang_format.sh` |
| clang-tidy | `src/.clang-tidy` | `script/lint/clang_tidy.sh` |
| cppcheck | `.cppcheck` | `script/lint/cppcheck.sh` |
| Doxygen | `project/doxygen/Doxyfile` | `script/lint/doxygen.sh` |
| Snippet sync | markers in the Markdown pages | `script/lint/doc_snippets.sh` |

## Pitfalls

- A test or benchmark source without its suffix joins no target: `_gtest`, `_doctest`,
  `_catch2`, `_shared`, `_gbench`, `_size`
- A `.cpp` under `src/` turns the interface target into a compiled library
- `script/doxygen/build.sh` runs `git checkout -f`: commit before running it
