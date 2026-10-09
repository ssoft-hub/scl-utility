# Contributing to ScL Utility

## Taking part

A person should ask a question about the module in the Telegram group
[scl_kit](https://t.me/scl_kit), not in an issue.

A person should report a defect or suggest an enhancement in an
[issue of the project](https://gitlab.com/ssoft-scl/scl-utility/-/issues): a defect in an issue of
type `Fix`, an enhancement in an issue of the type the table of the Issues section gives it. A merge
request should implement an enhancement only once the maintainers have agreed to it in the issue.
A person without an account on GitLab may send a report or a suggestion by mail to
`contact-project+ssoft-scl-scl-utility-32906655-issue-@incoming.gitlab.com`: the mail becomes an
issue, and the answers come back by mail.

## Conduct

Every participant of the project must follow the
[Contributor Covenant 2.1](https://www.contributor-covenant.org/version/2/1/code_of_conduct/) in
issues, merge requests and every other space of the project. A participant should report a
violation in a private message on Telegram to [@te_ssoft](https://t.me/te_ssoft). The maintainers
must enforce the Covenant as its Enforcement Guidelines state.

## Joining the team

A contributor whose merge requests have been merged may ask to join the team with
`Request access` in the actions menu at the upper right of the page of the project. The
maintainers should decide on the request by the merged requests of the contributor, and should
grant the role `Developer` to a request they accept.

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
| Criteria of a `Fix` | must | one criterion for a test covering the steps to reproduce that fails against the behaviour before the change, and one for that test passing against the change |
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
| Merge | must | the request is merged into `dev` once its checks have passed, its approvals are given and every acceptance criterion of its issue is checked |
| Close | must | the issue closes after the merge of the request that meets its criteria |

### From a clone to a merge request

The steps below carry a change from its issue to its merge request; the Workflow table above
binds the issue, the branch name, the changelog and the request itself.

| Step | Force | Rule |
|---|---|---|
| Clone | must | a member clones the project: `git clone https://gitlab.com/ssoft-scl/scl-utility.git`; a person who is not a member forks it on GitLab, clones the fork and adds the project as the remote `upstream`: `git remote add upstream https://gitlab.com/ssoft-scl/scl-utility.git` |
| Branch | must | from the current branch `dev` of the project: `git fetch origin` and `git switch -c <branch> origin/dev` in a clone of the project, `git fetch upstream` and `git switch -c <branch> upstream/dev` in a clone of a fork |
| Build and test | must | the tests build and pass in a host project, as the Building section states |
| Commits | must | as the Commits and branches section states |
| Push | must | `git push -u origin <branch>` |
| Merge request | must | into [ssoft-scl/scl-utility](https://gitlab.com/ssoft-scl/scl-utility), also from a fork |

### Merge requests from a fork

The author of a merge request from a fork should allow commits from members who can merge to the
target branch, so a maintainer can rebase the branch onto the branch `dev`.

The pipeline of such a request runs in the fork, and the author must start its checks there and
see them pass. A maintainer should then read the request and run its pipeline in this project; the
merge waits for the checks of that pipeline.

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

The host below, in a directory `host`, builds the GoogleTest tests of the clone that the variable
`SCL_UTILITY_DIR` names, against an installed GoogleTest:

```cmake
cmake_minimum_required(VERSION 3.23)
project(scl_utility_host LANGUAGES CXX)

set(SCL_UTILITY_DIR "" CACHE PATH "Clone of scl-utility")
find_package(GTest REQUIRED)

enable_testing()
add_subdirectory(${SCL_UTILITY_DIR}/project/cmake scl-utility)
add_subdirectory(${SCL_UTILITY_DIR}/project/cmake/test scl-utility-test)
```

Without an installed GoogleTest, the lines below fetch it, in place of
`find_package(GTest REQUIRED)`:

```cmake
include(FetchContent)
FetchContent_Declare(googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.tar.gz)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
set(GTEST_FOUND TRUE)
```

```sh
cmake -S host -B build -DSCL_UTILITY_DIR=<path to the clone>
cmake --build build --config Debug
ctest --test-dir build -C Debug
```

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
| A namespace-qualified name in a header under `src/` starts from the global namespace | must | `::std::size_t`, `::std::is_object_v<Type>`, `::scl::detail::holder_base` |
| A header under `src/` calls a function of the library by a qualified name | must | `::scl::enum_string(value)`, not `enum_string(value)` |
| A program under `example/` writes names the way a caller of the module usually does, without the leading `::` | should | `scl::enum_string(value)`, `std::size_t` |
| Every header opens with `#pragma once` | must | |
| Root namespace `scl`; a group's own namespace where it has one; every namespace-scope entity that is not part of the interface in a nested `detail`, a macro under the prefix `SCL_DETAIL_`, except an entity no means can place in `detail` | must | `::scl::hash`, `::scl::hierarchy`, `::scl::concepts`, `::scl::detail`, `SCL_DETAIL_<NAME>` |
| A concept is declared in a namespace named `concepts`, nested in the namespace it belongs to: outside it, a concept reads as a class template | must | `::scl::concepts::enum_type`, `::scl::hash::concepts::hashable_range`, `::scl::detail::concepts::number_like` |
| A new header joins the umbrella of its group | must | `src/scl/utility/meta/type.h` in `src/scl/utility/meta.h` |
| `typename` for a type parameter, `class` for a template template parameter | should | `template <typename...> class Operation` |
| A standard attribute is spelled as itself inside the library | should | `[[nodiscard]]`, not `SCL_NODISCARD` |
| `constexpr` and `noexcept` wherever the declaration admits them | should | |

The leading `::` and the qualified call protect a header from the names of the code that
includes it. An unqualified call finds, through argument-dependent lookup, a function of the
namespace of an argument's type as well (for an argument of a caller's type, the caller's own
namespace), and the program runs that function with no diagnostic where it fits the argument
better. Tests, examples and benchmarks are such code: no header includes them, and
the two rules do not bind them.

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

`<subject>` is what the file covers, an entity, an operation or a property of the group, and
`<aspect>` tells apart several files of one subject: `type_key_cross_tu_gtest.cpp`,
`type_key_boundary_gtest.cpp`.

| Suffix | Framework | Where `main` is defined |
|---|---|---|
| `*_gtest.cpp` | GoogleTest | the framework |
| `*_doctest.cpp` | doctest | the test source |
| `*_catch2.cpp` | Catch2 v3 | the framework |
| `*_catch2.cpp` | Catch2 v2, target `utility_<group>_catch2_v2` | the test source |
| `*_shared.cpp` | none, a shared library for the tests of its directory | - |
| `*_gbench.cpp` | Google Benchmark | the framework |
| `*_size.cpp` | none, see Benchmarks | - |

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
- A contributor must define no `main` in a `*_size.cpp` source: it builds with a bare-metal cross
  compiler and is never linked or run.

## Documentation

A contributor must document every entity of the public interface, a public or protected member of a
type included, with a Doxygen block. A private member, an entity of the namespace `detail` and a
macro under the prefix `SCL_DETAIL_` need no block and may carry one; an entity the Code style
section leaves outside `detail` needs a block as an entity of the interface does. For such an entity
the documentation build reports an undocumented entity, an undescribed parameter or return value, a
stale `@param`, a block reaching no target and an unknown `@ingroup`. The rules below are the ones
it does not report, and they bind every block alike.

| Rule | Force | Detail |
|---|---|---|
| The description of a block opens with `@brief` in one line | must | a one-line block is `///`, a longer one `/** */` |
| `@tparam` describes every template parameter | must | |
| `@ingroup scl_utility_<group>`, or a subgroup nested under it, stands on every documented entity at namespace scope, or a `@{ @}` block of that group encloses it | must | an umbrella header that only includes others carries none; `@defgroup` stands in the umbrella or a header of its own |
| A `= default` or `= delete` member of the interface carries a block written by hand | must | without one Doxygen drops the member from the class page and reports nothing |
| The block of an entity that is not part of the interface carries the tag `@internal` | must | |
| An example of use in a block comes through the command `@snippet` from a program under `example/`; a `@code` block may illustrate the declaration itself | must | the build compiles the program and Doxygen reports a marker it does not find in a block the reference shows, while a `@code` example of use goes stale with no report |

Pages live under `doc/md/<language>/`. A contributor must carry an edit of a page into its
version in every language.

### Where a block stands

| Entity | Force | Where its block stands |
|---|---|---|
| A free-standing entity defined outside a type, such as a type, a free function, a variable, a concept or a macro | may | whole above its definition |
| A member of a type, a hidden friend included | must | apart from the declaration of the type, except an overload the Out-of-line blocks table documents in place |
| A type, inside its braces | must | none, the exception of the row above aside; a comment there follows the Writing section |

### Out-of-line blocks

A contributor must place a block that stands apart from its declaration in the `Documentation`
section at the end of the header. A contributor must name the target with the command `@class`,
`@fn`, `@typedef` or `@var`, a concept with the command `@concept`, and a macro with the command
`@def`, parameters included: `@def SCL_ASSUME(expr)`. A contributor must write one block for a
macro, describing every branch that defines it: Doxygen reads only the branch active where the macro
`SCL_DOXYGEN` is defined, and merges a second block for the same macro into the first with no
report. A contributor must spell the target the way Doxygen renders it, parameter names included:
`node(Arguments &&... arguments)`, not `node(Arguments &&...)`. Attribute macros are expanded before
matching, so a contributor must spell an `@fn` without them.

| Shape | Failure | Fix, should |
|---|---|---|
| Two overloads whose parameter lists render the same | one block dropped, no warning | distinct template parameter names by role: `ValueArgument`, `WriteArgument`, `ReadArgument` |
| `requires A && B` | the leading `::` after `&&` is dropped, no match | `requires(A) && (B)` |
| A dependent east-const pointer return, declared and defined | the two render differently, no pairing | drop the namespace-scope declaration; the `friend` one suffices |
| Overloads no `@fn` spelling separates, told apart only by the template parameter list or only by their `requires` clauses | both blocks attach to one overload, and the build reports the other as undocumented | rewrite the overloads so an `@fn` spelling separates them, the template parameter names of the first row first; where no rewrite does, document in place, above the declaration, inside the body of the type for a member |
| A member re-exported from a private base with `using` | left off the class page | declare it once more in a `Documentation-only declarations` block under `#ifdef SCL_DOXYGEN`, as the example below shows |
| An unqualified befriended class sharing a member's name | it captures that member's block, no report | name it from the root: `friend class ::scl::hierarchy::tree<...>;` |

```cpp
// Documentation-only declarations

#ifdef SCL_DOXYGEN
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

### Names of `detail`

A public declaration must name no entity of `detail` in the declaration Doxygen reads, where the
macro `SCL_DOXYGEN` is defined, and its block must state what a hidden part states.

| Where the name stands | Fix, must |
|---|---|
| A constraint a caller needs to name in its own code | a concept of the interface |
| Any other `requires` clause | `requires(see below <case>)` under `#ifdef SCL_DOXYGEN`, the clause itself under `#else`; `<case>` names the case the clause admits, so overloads told apart by their constraints read apart |
| A `noexcept` expression | `noexcept(see below)` under `#ifdef SCL_DOXYGEN`, the expression itself under `#else` |
| A base class, a `friend` function declaration | the base or the `friend` declaration under `#ifndef SCL_DOXYGEN` |
| The definition of an alias template, the type of a variable template, the body of a concept | the declaration under `#ifndef SCL_DOXYGEN`, and a declaration without the name in the `Documentation-only declarations` block: `auto` for the type of a variable template, `unspecified` for the definition of an alias template or the body of a concept |

```cpp
template <typename Type>
constexpr Type * view_of(Type & object)
#ifdef SCL_DOXYGEN
    noexcept(see below)
    requires(see below viewable)
#else
    noexcept(::scl::detail::nothrow_viewable_v<Type>)
    requires(::scl::detail::viewable_v<Type>)
#endif
{ ... }

// Documentation-only declarations

#ifdef SCL_DOXYGEN
namespace scl
{
    template <typename... Args>
    inline constexpr auto overload_cast;

    namespace concepts
    {
        template <typename Format, typename Enum>
        concept enum_string_format = unspecified;
    } // namespace concepts
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
