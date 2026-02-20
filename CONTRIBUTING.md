# Contributing to ScL.Utility

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

## Test naming convention

Test files live in `module/utility/test/` and are named by framework suffix:

| Suffix | Framework | Linked target |
|--------|-----------|---------------|
| `*_gtest.cpp` | GoogleTest | `GTest::gtest_main` |
| `*_doctest.cpp` | doctest | doctest header-only |
| `*_catch2.cpp` | Catch2 | Catch2 |

Each public component should have tests in at least one framework.

## Documentation

- All public APIs must have Doxygen comments in the header files.
- Markdown documentation is maintained in two languages:
  - English: `doc/en/`
  - Russian: `doc/ru/`
- When adding or modifying a component, update both language versions.
- Follow the existing doc format (see `doc/en/meta/type_name.md` as reference).

## Commit messages

- Use conventional prefixes: `feat:`, `fix:`, `docs:`, `test:`, `build:`, `refactor:`.
- Keep the subject line under 72 characters.
- Reference related issues when applicable.
