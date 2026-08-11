# Contributing to ScL Utility

## Building and testing

### Prerequisites

- C++20 compiler: MSVC 19.30+, GCC 13+, or Clang 16+
- CMake 3.20+
- Ninja (recommended)

### Build

The module is built as part of the [scl-kit](https://github.com/ssoft-hub/scl-kit) ([GitLab](https://gitlab.com/ssoft-scl/complex/scl-kit)) super-project:

```bash
git clone --recurse-submodules https://github.com/ssoft-hub/scl-kit.git
cd scl-kit

# GCC
cmake -B build/gcc -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER=g++ -DCMAKE_C_COMPILER=gcc
cmake --build build/gcc

# Clang
cmake -B build/clang -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang
cmake --build build/clang

# MSVC (from Developer Command Prompt or after vcvarsall.bat)
cmake -B build/msvc -S . -G Ninja -DCMAKE_BUILD_TYPE=Debug
cmake --build build/msvc

# Run tests
ctest --test-dir build/gcc --output-on-failure
```

### CMake options

| Option | Default | Description |
|--------|---------|-------------|
| SCL_BUILD_TESTS | ON | Build tests |
| SCL_BUILD_EXAMPLES | ON | Build examples |
| SCL_ENABLE_GTEST | ON | Enable GoogleTest-based tests |
| SCL_ENABLE_DOCTEST | ON | Enable doctest-based tests |
| SCL_ENABLE_CATCH2 | ON | Enable Catch2-based tests |

## Code style

- **C++20**, header-only — all code goes in `.h` files under `src/scl/utility/`.
- Follow the existing `.clang-format` configuration.
- Use `#pragma once` as include guard.
- Root namespace: `scl`. Implementation details go in `scl::detail` or sub-namespace `::detail`.
- Prefer `constexpr` and `noexcept` where applicable.

## Code quality checks

All code must pass automated checks before merging. CI runs the following tools:

### clang-format
Enforces consistent code formatting:
```bash
# Check formatting
find src -name '*.h' -o -name '*.hpp' | xargs clang-format --dry-run --Werror

# Auto-fix formatting
find src -name '*.h' -o -name '*.hpp' | xargs clang-format -i
```

### clang-tidy
Static analysis for common issues:
```bash
find src \( -name '*.h' -o -name '*.hpp' \) | while IFS= read -r f; do
  clang-tidy "$f" --quiet --warnings-as-errors='*' -- -std=c++20 -xc++ -Isrc
done
```

### cppcheck
Additional static analysis with configured suppressions (see `.cppcheck`):
```bash
find src \( -name '*.h' -o -name '*.hpp' \) -exec cppcheck \
  --enable=warning,style,performance,portability \
  --std=c++20 \
  --language=c++ \
  --inline-suppr \
  --error-exitcode=1 \
  --suppress=missingIncludeSystem \
  --suppress=unusedFunction \
  -Isrc \
  -UDOXYGEN \
  {} +
```

### Documentation snippets

A Markdown code block introduced by a snippet marker repeats a region of a compiled
program, and CI fails when the two have drifted apart:
```bash
# Check every page
bash script/lint/doc_snippets.sh

# Fill the blocks in from their sources, after editing an example
bash script/lint/doc_snippets.sh --write
```
The write mode is for local work only — CI checks and never writes.

## Source file naming

Examples, tests and benchmarks follow one rule. `<group>` is the header's own directory
under `src/scl/utility/` — a header sitting at that root, such as `flags.h`, is its own
group.

| Tree | Path | Target |
|------|------|--------|
| `example/` | `<group>/<name>/<group>_<name>_example.cpp` | `utility_<group>_<name>_example` |
| `test/` | `<group>/<subject>[_<aspect>]_<framework>.cpp` | `utility_<group>_<framework>` |
| `benchmark/` | `<group>/<subject>[_<aspect>]_<tool>.cpp` | `utility_<group>_<tool>` |

`<subject>` is the header the file covers, followed by an aspect when one header needs
several files. The trailing token is what CMake globs on, and it decides which target the
file joins:

| Suffix | Framework | Linked target |
|--------|-----------|---------------|
| `*_gtest.cpp` | GoogleTest | `GTest::gtest_main` |
| `*_doctest.cpp` | doctest | doctest header-only |
| `*_catch2.cpp` | Catch2 | Catch2 |
| `*_shared.cpp` | — | companion shared library, linked into every test target of the directory |

Each public component should have tests in at least one framework.

Every base name ends in the token naming what the file is built into, and the target ends
in the same token: `example`, or the framework or tool. The trees differ in how many files
share a target — one example is one program, while every test of a group builds into one
executable per framework — so only an example's base name is its whole target name without
the `utility_` prefix. The suffix also keeps an example target apart from the test target
of the same group.

An example carries its group inside the base name because Doxygen resolves `@example` by
base name alone, and only `example/` is in its input; a test's directory already makes it
unique. The `<name>` level is always present — an example covering its group as a whole
is named `common`, as in `example/any/common/any_common_example.cpp`. Every example needs a
directory of its own: all sources under one example root link into a single program, so a
second `main` beside it is a link error.

## Benchmarks

The benchmark `<tool>` token is `gbench`, for Google Benchmark, so `benchmark/hash/`
builds into `utility_hash_gbench`.

Benchmarks are not CTest tests, and they are off by default: a run is long and its numbers
only mean anything in a Release build. There is no benchmark preset, because every preset
in the matrix names a toolchain and "build the benchmarks" is a separate axis - pass the
options to whichever preset you want to measure:

```sh
script/ci/build.sh clang-x64 Release -DSCL_BUILD_BENCHMARKS=ON -DSCL_BUILD_TESTS=OFF
script/ci/run_benchmarks.sh clang-x64
```

`run_benchmarks.sh` fixes the repetition count, so two runs of the same suite are directly
comparable - which is what a before/after figure in an issue or MR has to be. Both scripts
live in the super-project, since the benchmark targets are configured there.

## Documentation

- All public APIs must have Doxygen comments in the header files.
- Markdown documentation is maintained per language under `doc/md/<language>/`:
  - English: `doc/md/en/`
  - Russian: `doc/md/ru/`
- When adding or modifying a component, update every language version.
- A language added under `doc/md/` needs no configuration: the snippet check and the
  documentation build find its pages by path.
- Follow the existing doc format (see `doc/md/en/meta/type_name.md` as reference).

## Commit messages

- Use conventional prefixes: `feat:`, `fix:`, `docs:`, `test:`, `build:`, `refactor:`.
- Keep the subject line under 72 characters.
- Reference related issues when applicable.
