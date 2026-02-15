# Contributing to SCL.Utility

## Building and testing

### Prerequisites

- C++20 compiler: MSVC 19.30+, GCC 13+, or Clang 16+
- CMake 3.20+
- Ninja (recommended)

### Build

From the repository root:

```bash
# All available compilers
cmd //c "script\claude\build.bat all Debug build"

# Single compiler
cmd //c "script\claude\build.bat mingw Debug build"
cmd //c "script\claude\build.bat msvc Debug build"
```

### Run tests

```bash
cmd //c "script\claude\run_tests.bat Debug"
```

## Code style

- **C++20**, header-only — all code goes in `.h` files under `src/scl/utility/`.
- Follow the existing `.clang-format` configuration.
- Use `#pragma once` as include guard.
- Root namespace: `scl`. Implementation details go in `scl::detail` or sub-namespace `::detail`.
- Prefer `constexpr` and `noexcept` where applicable.

## Test naming convention

Test files live in `module/utility/test/` and are named by framework suffix:

| Suffix | Framework | Linked target |
|--------|-----------|---------------|
| `*gtest.cpp` | GoogleTest | `GTest::gtest_main` |
| `*doctest.cpp` | doctest | doctest header-only |
| `*catch2.cpp` | Catch2 | Catch2 |

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
