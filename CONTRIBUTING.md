# Contributing to ScL Utility

## Issues

A reporter should open an issue for one concern, where no open issue carries that concern
already.

| Part | Force | Rule |
|---|---|---|
| Title | must | `Type(scope): Subject`, the scope optional; the subject imperative, capitalised, with no final period; the whole title at most 80 characters |
| Type | should | one of the table below |
| Body of a `Fix` | must | the sections `Problem`, `Steps to reproduce`, `Expected behaviour`, `Actual behaviour`, `Acceptance criteria` and `Environment` |
| Body of any other type | must | the sections `Goal` and `Acceptance criteria` |
| `Goal`, `Problem` | must | the symptom or the missing capability, its cost and what triggers it; no history of the discovery, no alternative weighed |
| `Steps to reproduce` | must | one action per step |
| Acceptance criterion | must | one condition, `Given ..., when ..., then ...`, ticked by one observation of what the work delivers by the time the issue closes |
| Plan of checks | must | none in the body |

```
Fix(hash): Refuse a range the hash bodies cannot iterate
```

| Type | What the issue covers |
|---|---|
| `Feat` | functionality a user does not have yet |
| `Fix` | behaviour that departs from what the module states it does |
| `Refactor` | the shape of the code or the text, with nothing a user sees changed |
| `Perf` | a measured cost - time, memory, size - that the work brings down |
| `Docs` | what a reader is told, in documentation or in a comment |
| `Test` | behaviour no test reaches yet |
| `Chore` | the build, the tooling, a dependency or a module reference |
| `Ci` | the pipeline running the checks |
| `Style` | formatting alone, with no logic and no wording changed |

## Workflow

| Step | Force | Rule |
|---|---|---|
| Issue | should | a change starts from an issue |
| Branch | must | named as the Commits and branches section states |
| Changelog | must | a key change has an entry in `CHANGELOG.md`, new or corrected, as the Changelog section states |
| Merge request | must | targets `dev`; titled `<issue-num>: Subject`, where `<issue-num>` is the identifier of the issue, or in the form of a commit subject where the request has no issue; the description carries the sections `Problem`, `Summary`, `Implementation` and `Test plan` |
| Merge | must | the request is merged into `dev` once its checks have passed and its approvals are given |

## Licence

By opening a merge request, a contributor affirms the right to contribute the content of the
request and contributes it under the terms of `LICENSE.md`.

## Building

### CMake

A host project builds the module: the module carries no top-level `CMakeLists.txt` and no
presets.

| The host adds | Targets |
|---|---|
| `project/cmake` | `scl::utility`, an interface target |
| `project/cmake/test` | `utility_<group>_<framework>` per `test/<group>/` |
| `project/cmake/example` | `utility_<group>_<name>_example` per example directory |
| `project/cmake/benchmark` | `utility_<group>_gbench` and `utility_<group>_size` per `benchmark/<group>/` |

The CMake lists under `project/cmake/test` and `project/cmake/benchmark` read `GTEST_FOUND`,
`DOCTEST_FOUND`, `CATCH2_FOUND` and `BENCHMARK_FOUND`, which the host project sets, and create
no target for a framework that is not found; `utility_<group>_size` needs no framework.

## Rules that reject a change

- A contributor must not add a runtime dependency beyond the standard library.
- A contributor must not add a `.c` or `.cpp` file under `src/`: such a file turns the
  interface target into a compiled library, with no diagnostic from the build.

## Writing

| Subject | Force | Rule |
|---|---|---|
| Language | must | English: identifiers, comments, documentation blocks, commits, issues, merge requests. A page under `doc/md/<language>/` is written in its language |
| Punctuation | must | keyboard characters: `-` for a dash, `"` and `'` for quotes, `...` for an ellipsis |
| Comments | should | only a lint suppression, a formatter hint, a section marker or a fact the code cannot show; the Documentation section of this file governs documentation blocks |

## Code style

A contributor should leave formatting to `clang-format -i <file>`, which applies layout,
qualifier order, include order and pointer alignment from `.clang-format`.

| Rule | Force | Example |
|---|---|---|
| `lower_case` for namespaces, types, functions, variables, members and aliases | must | `::scl::hash::key`, `::scl::hash::byte_view()` |
| `CamelCase` for template parameters | must | `template <typename ValueArgument>` |
| `UPPER_CASE` for macros, prefixed `SCL_` under `src/` | must | `SCL_HAS_EXCEPTIONS` |
| `m_` before the name of a private non-static data member | should | `m_parent` |
| A namespace-qualified name starts from the global namespace | must | `::std::size_t`, `::std::is_object_v<Type>`, `::scl::detail::any_holder_base` |
| Every header opens with `#pragma once` | must | |
| Root namespace `scl`; a group's own namespace where it has one; internals in a nested `detail` | must | `::scl::hash`, `::scl::hierarchy`, `::scl::concepts`, `::scl::detail` |
| A new header joins the umbrella of its group | must | `src/scl/utility/meta/type.h` in `src/scl/utility/meta.h` |
| `typename` for a type parameter, `class` for a template template parameter | should | `template <typename...> class Operation` |
| A standard attribute is spelled as itself inside the library | should | `[[nodiscard]]`, not `SCL_NODISCARD` |
| `constexpr` and `noexcept` wherever the declaration admits them | should | |

A contributor should use an `SCL_*` macro from `attribute/` for an annotation whose spelling or
availability differs by toolchain or standard level, such as `SCL_NO_UNIQUE_ADDRESS`.

A contributor must place the `requires` clause of a function after the declarator: after the
parameter list, `const`, `noexcept` and a trailing return type, before `= delete`, `= default`
and a member initializer list.

```cpp
template <typename Type>
[[nodiscard]]
constexpr Type * any_cast(Type * arg) noexcept
    requires(::std::is_object_v<Type>)
{ ... }
```

A contributor must call a `__has_*` operator only inside a branch opened by a `defined` test
of it. The call on the same line as the test is a syntax error on a compiler lacking the
operator, since the whole line is parsed before it is evaluated.

```cpp
// Defective
#elif defined(__has_feature) && __has_feature(cxx_exceptions)

// Correct
#elif defined(__has_feature)
#if __has_feature(cxx_exceptions)
```

An empty block comment `/**/` at the end of a declarator line is a break hint for
clang-format: the declarator stays whole on its line, and `noexcept(...)` moves to the next.
A contributor should put it only where the formatter would otherwise break the declarator
itself.

```cpp
[[nodiscard]]
constexpr auto begin() /**/
    noexcept(noexcept(cursor{::std::ranges::begin(source), ::std::ranges::end(source)}))
```

A contributor must list the members of a class body in three sections: types and aliases,
then data, then functions. Within a section the contributor must order the access levels
`public`, `protected`, `private`, put the static members of one access level before the
non-static ones, and close the access level with its `friend` declarations, hidden friends
included. A contributor must give a member the narrowest access its callers allow.

```cpp
template <typename Payload>
class node
{
public:
    using payload = Payload;

private:
    using nodes = ::std::list<node>;

    friend class ::scl::hierarchy::tree<Payload>;

private:
    node * m_parent{};
    nodes m_nodes;

public:
    constexpr node() noexcept = default;

    [[nodiscard]]
    constexpr bool empty() const noexcept;

private:
    static constexpr void link(node & parent, node & child) noexcept;

    friend bool operator==(node const &, node const &) = default;
};
```

## Compatibility

| Change | Force | Requirement |
|---|---|---|
| A change that breaks a consumer | must | the commit subject marked with `!` and a `CHANGELOG.md` entry naming what breaks |
| A feature beyond C++20 that makes an implementation simpler or more efficient | should | an alternative implementation beside the C++20 one |
| Choosing between the implementations | must | a feature test: a `__cpp_*` macro, `__has_include`, `__has_cpp_attribute`; never a compiler version |
| A compiler extension | must | behind a probe of the extension: `__has_attribute`, `__has_builtin`, `__has_cpp_attribute`, `__has_include`; a test of the compiler only where no probe exists |

## Source file naming

`<group>` is a header's directory under `src/scl/utility/`; a header at that root, such as
`flags.h`, is a group of its own. `test/umbrella/` holds the tests of the umbrella headers,
and `example/quick_start/<group>/` holds the programs `README.md` quotes.

| Tree | Path | Target |
|---|---|---|
| `example/` | `<group>/<name>/<group>_<name>_example.cpp` | `utility_<group>_<name>_example` |
| `test/` | `<group>/<subject>[_<aspect>]_<framework>.cpp` | `utility_<group>_<framework>` |
| `benchmark/` | `<group>/<subject>[_<aspect>]_<tool>.cpp` | `utility_<group>_<tool>` |

`<subject>` is the header the file covers, `<aspect>` tells apart several files of one
header: `type_key_cross_tu_gtest.cpp`, `enum_fallback_gtest.cpp`.

| Suffix | Framework | Linked with | Where `main` is defined |
|---|---|---|---|
| `*_gtest.cpp` | GoogleTest | `GTest::gtest_main` | the library |
| `*_doctest.cpp` | doctest | `doctest::doctest` | the test source |
| `*_catch2.cpp` | Catch2 v3 | `Catch2::Catch2WithMain` | the library |
| `*_catch2.cpp` | Catch2 v2, target `utility_<group>_catch2_v2` | `Catch2::Catch2` | the test source |
| `*_shared.cpp` | - | a shared library linked into every test of the directory | - |
| `*_gbench.cpp` | Google Benchmark | `benchmark::benchmark_main` | the library |
| `*_size.cpp` | - | nothing, see Benchmarks | - |

- A test or benchmark source without its suffix joins no target and is never compiled, with no
  diagnostic from the build.
- A contributor must repeat the group in the base name of an example: Doxygen resolves
  `@example` by base name alone.
- A contributor must give every example a directory of its own: all sources under it link into
  one program. A contributor should name an example covering its whole group `common`:
  `example/any/common/any_common_example.cpp`.

## Testing

- A contributor adding a public interface must add a test under `test/<group>/`.
- `STATIC_EXPECT_*` from `test/gtest_utils.h` checks a compile-time fact twice, with
  `static_assert` and with the matching `EXPECT_*`.
- A contributor writing a `*_shared.cpp` library must export the symbols the tests use, in
  the way the platform requires: `__declspec(dllexport)` on Windows, for one.

## Checks

```sh
bash script/lint/lint.sh
```

The script runs every check below from any directory and needs no build tree; a script of
the table runs one check alone. The scripts need Bash with GNU findutils and diffutils, and
find each tool on `PATH` or through its variable. A contributor should use the tool versions
of the images the CI uses: another version of clang-format formats differently.

| Script | Tool | Variable | Configuration |
|---|---|---|---|
| `script/lint/clang_format.sh` | clang-format | `CLANG_FORMAT` | `.clang-format` |
| `script/lint/clang_tidy.sh` | clang-tidy | `CLANG_TIDY` | `src/.clang-tidy` |
| `script/lint/cppcheck.sh` | cppcheck | `CPPCHECK` | `.cppcheck` |
| `script/lint/doxygen.sh` | Doxygen | `DOXYGEN` | `project/doxygen/Doxyfile` |
| `script/lint/doc_snippets.sh` | snippet sync | | snippet markers in the Markdown pages |

- Each script is the source of truth for its tool: directories, flags and suppressions live
  there and in the configuration files.
- `bash script/lint/doc_snippets.sh --write` refills the snippet blocks after an example
  changes.

## Benchmarks

- Benchmarks are not CTest tests.
- A contributor must quote a figure in an issue or a merge request only from a Release build.
- A `*_size.cpp` source is compiled into a static library and never linked or run, so a
  bare-metal cross compiler builds it; `size` reads the `.text` section of the library.

## Documentation

A contributor must document every public entity with a Doxygen block. The documentation build
reports an undocumented entity, an undescribed parameter or return value, a stale `@param`, a
block reaching no target and an unknown `@ingroup`. The rules below are the ones it does not
report.

| Rule | Force | Detail |
|---|---|---|
| The description of a block opens with `@brief` in one line | must | a one-line block is `///`, a trailing one `///<`, a longer one `/** */` |
| `@tparam` describes every template parameter | must | |
| `@ingroup scl_utility_<group>`, or a subgroup nested under it, stands on every documented entity at namespace scope, or a `@{ @}` block of that group encloses it | must | an umbrella header that only includes others carries none; `@defgroup` stands in the umbrella or a header of its own |
| A `= default` member carries a block written by hand | must | without one Doxygen drops the member from the class page and reports nothing |
| A private or protected member stays out of the reference | must | its block carries `@internal` |
| An internal entity in public scope takes `@internal` and an `EXCLUDE_SYMBOLS` entry | must | `SCL_DETAIL_*` probes, pattern anchors |

Pages live under `doc/md/<language>/`. A contributor must carry an edit of a page into its
version in every language.

### Out-of-line blocks

A contributor must place a block in the `Documentation` section at the end of the header,
except where the table below says to document in place, and must name its target with
`@class`, `@fn`, `@typedef` or `@var`. A contributor must spell the target the way Doxygen
renders it, parameter names included: `node(Arguments &&... arguments)`, not
`node(Arguments &&...)`. Attribute macros are expanded before matching, so a contributor must
spell an `@fn` without them.

| Shape | Failure | Fix, should |
|---|---|---|
| Two overloads whose parameter lists render the same | one block dropped, no warning | distinct template parameter names by role: `ValueArgument`, `WriteArgument`, `ReadArgument` |
| `requires A && B` | the leading `::` after `&&` is dropped, no match | `requires(A) && (B)` |
| A dependent east-const pointer return, declared and defined | the two render differently, no pairing | drop the namespace-scope declaration; the `friend` one suffices |
| Overloads told apart only by the template parameter list | no `@fn` spelling separates them | document in place, above the declaration |
| A member re-exported from a private base with `using` | left off the class page | declare it once more in a `Documentation-only declarations` block under `#ifdef DOXYGEN`, as the example below shows |
| An unqualified befriended class sharing a member's name | it captures that member's block, no report | name it from the root: `friend class ::scl::hierarchy::tree<...>;` |

```cpp
// Documentation-only declarations

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

## Commits and branches

Commit messages must follow
[Conventional Commits](https://www.conventionalcommits.org/en/v1.0.0/). What the
specification leaves to the project:

| Part | Force | Rule |
|---|---|---|
| `{type}` | must | `feat`, `fix`, `docs`, `style`, `refactor`, `perf`, `test`, `build`, `ci`, `chore`, or one more lowercase word for a recurring kind of work none of these names |
| `(scope)` | should | the group the change touches, or the tool of a `ci` or `build` change |
| `!` | must | marks a change that breaks a consumer; the body says what breaks |
| First line | must | at most 72 characters |
| Body | must | wrapped at 72: why the change was made and what it solves |
| Issue | should | named by the branch and the merge request title; a footer only where neither makes the link plain |

```
fix(hash)!: refuse an array, whose bound is storage rather than content
```

A contributor must name a branch `{user}/{type}/{issue-num}/{subject}`, without
`{issue-num}` where no issue is linked. Every commit of the branch must build, and
`bash script/lint/lint.sh` must pass on it with no error.

## Changelog

`CHANGELOG.md` follows Keep a Changelog; `[Unreleased]` collects the release in progress. The
rules below bind every entry a change adds or corrects.

| Rule | Force |
|---|---|
| An entry records a key change a consumer notices: a new public entity, a change of behaviour, a break, a fix of a visible defect | should |
| A refactoring, a test, a tooling or a pipeline change takes no entry | should |
| Subsections appear once each, in the order `Added`, `Changed`, `Deprecated`, `Removed`, `Fixed` | must |
| An entry is one sentence of at most 100 characters saying what changed, with no detail or retelling | must |
| No two entries of a release cover the same change: the entry already covering it is extended to the change, and no new one is added | must |
| An entry of the release in progress that a change makes untrue is corrected in place | must |
| No entry contradicts the code | must |

```
- [hash] Added `byte_view`, presenting wider elements as the bytes a hash takes
- [hash] Refused a bounded array in every hash function
- [any] Added `any_mutable_view`, a non-owning view that writes through
```

Defective, the second entry retelling the first:

```
- [hash] Added `byte_view`, presenting wider elements as the bytes a hash takes
- [hash] `byte_view` refuses a bounded array and asks for neither random access nor a size
```

## Release

| Step | Force | Rule |
|---|---|---|
| `[Unreleased]` | must | its entries meet the rules of the Changelog section, and its heading is renamed to the version and date |
| Version | must | `SCL_UTILITY_VERSION` of `project/cmake/CMakeLists.txt` and `PROJECT_NUMBER` of `project/doxygen/Doxyfile` carry the new version |
| Released heading | must | a released heading of `CHANGELOG.md` stays as it is |
