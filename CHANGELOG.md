# Changelog

All notable changes to the ScL Utility module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- `scl::flags` subtracts one set from another: `a - b` and `a -= b` keep the flags set in
  `a` and absent from `b`, in the flags-flags and the flags-`Enum` form alike. The
  difference was previously spelled `a ^ (a & b)`, and it is what expresses "everything in
  this set except those" without appealing to a universe the type cannot know.

- `SCL_HAS_EXCEPTIONS` (`#include <scl/utility/preprocessor/exceptions.h>`) — `1` when the
  translation unit is compiled with exceptions, `0` otherwise, derived from `_CPPUNWIND`,
  `__EXCEPTIONS` or `__has_feature(cxx_exceptions)`. The language has no standard
  feature-test macro for exceptions, and the ScL declarations that answer a failed request
  by throwing are gated on this one, so portable user code branches on the same macro.
  Always defined, so `#if` interrogates it and `-Wundef` catches a misspelling.

- A Markdown page per availability macro on both language trees — `preprocessor/rtti.md`
  and `preprocessor/exceptions.md` — stating what the library declares in each
  configuration. `SCL_HAS_RTTI` had no page, so the generated reference was the only
  account of it.

- `scl::hash::byte_view` — a lazy view spelling a range of wider elements as the bytes the
  hash functions take: `fnv1a(byte_view(text))` where `text` is a `std::u16string_view`.
  Each element contributes its bytes least significant first, whatever the host's own byte
  order, so two machines hash one input alike; a byte-sized element passes through
  unchanged. `example/hash/byte_view/hash_byte_view_example.cpp` shows it running.

- An `Examples` page in the generated reference, listing every program under
  `example/` with its description and full source, and an `Examples` section in
  `README.md` linking each of them. The programs were previously reachable only by
  browsing the repository.
- A link to the runnable example from every Markdown page whose subject has one:
  `any_view`, `any_arg`, `any_switch`, `flags`, `hash::key`, `type_key`, `enum_value`
  and both the meta and the runtime `type_name`, on each language's page.
- `example/meta/type_name` — `type_name` and `type_short_name`, and the part of a rendered
  name that depends on the compiler. Both code blocks on the `meta/type_name` pages are
  now taken from it by the `doc-snippets` gate, so what those pages show is code the CI
  compiles on every supported compiler rather than text written beside it.
- A Quick start of six programs — `meta`, `hash`, `flags`, `any`, `hierarchy`,
  `type_traits` — under `example/quick_start/`. `README.md`, both `Main.md` and the
  Doxygen main page show the code of those programs rather than a snippet written
  beside them.

### Changed

- **Breaking.** `scl::type_key` is a value rather than an identity reached through a
  reference. It gains an empty state, spelled `scl::type_key{}`, and copying, moving and
  assignment; `type_key_of<T>()` still answers a reference to the per-type constant, so the
  address fast path in `operator==` survives for a caller that keeps one. What ends is
  holding a key as `type_key const *`: a handle with no type to report answers
  an empty key instead of `nullptr`, which is what lets the identity queries hand out
  a value. Nothing has been released, so the old spelling is gone rather than deprecated.

- **Breaking.** `scl::any_view::type_key()` and `scl::any_argument::type_key()` answer a
  `scl::type_key` by value, the way `scl::basic_any::type_key()` does, and an empty handle
  answers `scl::type_key{}`. A comparison written as `*view.type_key() == key` loses the
  dereference, and one written against `nullptr` compares against `scl::type_key{}`.

- **Breaking.** `scl::hierarchy::adl_identity` answers a `scl::hierarchy::identity` rather
  than a `void const *`, for `scl::hierarchy::node` and for the tree's proxies alike. The
  new token is a value carrying the address it stands for and handing it back to nobody,
  which is what the customization point already promised - callers were told to treat the
  result as opaque, and now the type enforces it. A foreign type adapting the hooks may
  still answer any equality-comparable type of its own.

- **Breaking.** The constructor `scl::flags::const_iterator(flags const *, std::size_t)` is
  private, befriended by `scl::flags`, the way `scl::hierarchy::tree::iterator` already keeps
  its own. Only `begin()` and `end()` ever built an iterator over a set, and the default
  constructor the iterator concept demands stays public.

- `scl::flags` finds its set bits a byte at a time. Iteration and `size()` ask
  `std::countr_zero`, `std::countl_zero` and `std::popcount` about a whole storage byte
  instead of testing each bit in turn, so a byte with nothing set costs one test rather
  than eight — iterating a wide, sparse mask is the operation that gains. The order, the
  end position and every result are unchanged, in constant evaluation as well.

- The four `scl::any_cast` overloads that answer a failed request by throwing — the value
  and reference forms over an `any_view` and over an `any_arg` — are declared only where
  `SCL_HAS_EXCEPTIONS` is `1`, as the `typeid` surface is declared only under
  `SCL_HAS_RTTI`. A translation unit compiled with `-fno-exceptions` keeps the
  pointer-returning forms, which report failure with `nullptr` and never threw. Nothing
  changes for a build with exceptions enabled.

- `scl::flags` reports an enumerator ordinal `>= bit_count` with `std::abort()` where
  `SCL_HAS_EXCEPTIONS` is `0`, since the constructor and `operator[]` cannot answer such a
  call at all — it is a precondition violation, not a query that failed. The call stays
  ill-formed in constant evaluation either way, and a build with exceptions still throws
  `std::out_of_range` as before.

- Example, test and benchmark sources follow one naming rule, written in `AGENTS.md` and
  `CONTRIBUTING.md`: an example is `example/<group>/<name>/<group>_<name>_example.cpp`, a
  test is `test/<group>/<subject>_<framework>.cpp`, and a benchmark will read the same way.
  An example base name is all Doxygen has to address `@example` by, and two examples
  sharing one left the second unreachable.
- Every example target is renamed, because an example target now carries the same trailing
  `_example` its source does, the way a test target carries the framework: `utility_any`
  becomes `utility_any_common_example`, `utility_hash_key_nttp` becomes
  `utility_hash_key_nttp_example`, `utility_meta` becomes `utility_meta_type_key_example`,
  and so on for all fifteen. Building one example by name uses the new name; building
  everything is unaffected.
- Five test sources are named after the header they cover rather than after an API it
  declares, and `test/preprocesor` is spelled `preprocessor`. That renames one target,
  `utility_preprocesor_gtest` to `utility_preprocessor_gtest`; the tests it runs are the
  same, except that the `meta` suites are `MetaTypeTest`, `MetaEnumTest` and
  `MetaSymbolTest`, which no longer collide with the `runtime` ones.
- `<scl/utility.h>` now includes every module header. It left out `hash.h` and
  `runtime.h`, so the header the documentation offers as "everything" reached neither
  the hash utilities nor `enum_value` and the runtime type names.
- The installation instructions name paths and targets that exist, and cover the three
  ways to consume the module: its `src` on an include path, the toolkit super-project
  added in tree, and an installed package through
  `find_package(scl COMPONENTS utility)`. The former `add_subdirectory(module/utility)`
  addressed a directory that has no `CMakeLists.txt`.
- The stated CMake requirement is 3.23, which is what the toolkit declares; the pages
  said 3.20.
- `README.md`, both `Main.md` and the Doxygen main page list all ten groups. Any,
  Concepts and Runtime were missing from the README, Concepts and Hierarchy from both
  `Main.md`, and five of the ten from the main page.
- The badge row tells the two CI systems apart and carries the GitHub lint status
  beside the GitLab one.

- The reference now lists and describes every public member. Previously 147 of them —
  most of `scl::hierarchy::tree` and its iterator and reference proxies, but also the
  hash functors, the deleted special members of `scl::type_key` and `scl::any_arg`, and
  the `hierarchy` ADL customization points — carried no text and were dropped from the
  pages entirely rather than shown as undocumented.
- Every member's description now appears in the `Member Documentation` section of its class
  page, not only in the summary table at the top. A one-line description — which is what a
  type alias, a deleted special member or an iterator operator gets — previously stopped at
  the summary, leaving 207 of the 377 public members with no detailed entry.
- The documentation lint gate now holds coverage as well as attachment: a public member
  added without a block fails it, where before it silently disappeared from the reference.
- The hand-written Markdown guides moved from `doc/en/` and `doc/ru/` to `doc/md/en/` and
  `doc/md/ru/`. `doc/` also holds generated output such as `doc/doxygen/`, so written and
  generated documentation no longer sit at the same level.

### Removed

- `scl::flags::operator~` and `scl::flags::all()`. Both were defined over the storage
  width rather than over the flags a caller put in: the complement of a mask whose
  enumeration declares fewer enumerators than `capacity` bits, or spreads them apart,
  returned ordinals no enumerator names, and `all()` asked whether every one of those
  ordinals was set, which such an enumeration can never satisfy. A complement now takes
  the universe it belongs to — `universe - set`, or `universe ^ set` for a subset — and
  `all_of(universe)` asks the whole-set question against the same explicit universe.

- The `ScL Utility` group. It was the declared parent of the module groups but only
  three of the ten ever joined it, so its page listed a third of the library and implied
  the rest sat elsewhere. The module groups are the top level, as the main page already
  presents them.

### Fixed

- A range the hash functions accept is hashed by every bit of every element. An element
  wider than a byte reached the hash function as its low byte alone, so `L"Ā"` and
  `L"Ȁ"` produced one value, as did `L"AB"` and `L"Łł"`; `std::vector<int>`
  and `std::vector<double>` were accepted on the same lossy terms. `wchar_t`, `char16_t`,
  `char32_t` and every arithmetic element are now rejected at compile time — hash the
  bytes of such a range explicitly when that is what is meant — and
  `std::span<std::byte>`, promised in the documentation but rejected by the old
  constraint, works. The new `scl::hash::concepts::byte_element` states the rule, and
  hash values of byte ranges are unchanged.
- A string literal is hashed as the text it spells. Its terminating zero counted as one
  more byte, so `scl::hash::key<>{"start"}` equalled neither the key built from a
  `std::string_view` nor the one built from a `std::string`, and a `switch` over a key
  taken from a runtime string fell through to `default` with nothing reported.
  `fnv1a`, `djb2`, `sdbm`, `jenkins_ota` and `siphash` follow the same rule. It covers
  the last element of an array of `char` or `char8_t` — the character types whose code
  unit is a byte — and nothing else: an array that does not end in zero —
  `char const raw[3]{'a', 'b', 'c'}` — is still hashed whole, and so is an array of any
  other element type, where `std::uint8_t data[4]{1, 2, 3, 0}` keeps all four bytes.
  **The value produced for a string literal changes**: a hash value stored by an earlier
  version no longer matches the one computed now. A literal keeps folding to a constant
  at compile time, and a view, a string or a byte range costs what it did before;
  hashing a character array whose contents are only known at run time may cost slightly
  more, since its length is no longer a constant.
- Documentation blocks that silently failed to reach their target now appear in the
  generated reference: each `scl::any_cast` overload carries its own description, and
  `has_value` / `type_name` / `type_key` are listed and described on `scl::any_view` and
  `scl::any_arg` instead of being left off the class page.
- Doxygen builds the reference with no diagnostics at all:
  `@ref scl::hash::concepts::byte_hasher` and `@ref SCL_UNSEQUENCED` resolve,
  `node::transfer` documents the parameter it actually takes, and
  `std::hash<scl::hash::key>` is placed in the reference rather than reported as
  unplaceable.
- `scl::type_name<T>()` is documented as it behaves. The header of `meta/type.h`
  promised that the `class`/`struct`/`union` prefix is stripped on MSVC, which it never
  was and — as the same file's `@note` and the Markdown pages already said — is not
  meant to be. The prefix reaches template arguments too, and the example on the pages
  asserted a substring that could not hold on MSVC.
- The documentation says what a rendered name is for. It is a value to show, not one to
  compare: the prefix aside, inline namespaces and defaulted template arguments diverge
  too, so `std::string` reaches the caller as three different strings on the three
  supported toolchains and no part of a name is guaranteed to agree between them. The
  pages and the header now point at `scl::type_key` for identity and at
  `type_short_name<T>()` for the bare identifier, where the text used to offer matching
  on a substring — which is the same trap one step further in.
- The Quick start says what `type_name<color>()` actually prints. The line was annotated
  `color`, which is the GCC and Clang rendering; MSVC prints `enum color`. The program
  itself was correct, so the `doc-snippets` gate had nothing to catch — a comment states
  a claim the compiler never checks.

- Fixed a compiler crash (`SIGSEGV` in clang's `Sema`) building
  `utility_type_traits_gtest` on macOS, caused by an oversized test
  translation unit forcing too many template instantiations at once.
- Fixed the type-traits test build under Clang 18 on Linux, where a compiler
  crash (`SIGSEGV` in clang's `Sema`) took `utility_type_traits_gtest` out of
  the `clang-x64` and `clang-x64-no-rtti` builds entirely. Clang crashes while
  diagnosing a postfix `++`/`--` detection whose candidate set holds both a
  deleted member operator and a free two-argument one; the test helpers now keep
  the free operators in a namespace of their own, so only the type they belong
  to reaches them.

### CI

- A `doc-snippets` gate: a Markdown code block introduced by an HTML comment naming a
  source file must repeat the region of that file between its `//! [quick_start]`
  markers, so a documented program and its copy in the text cannot drift apart
  unnoticed. `script/lint/doc_snippets.sh` documents the exact spelling.
- The formatting gate covers `test/` and `example/` as well as `src/`, and the
  sources as well as the headers. It previously read `src/*.h` only, so three
  files had drifted away from `.clang-format` with nothing reporting it; they are
  reformatted. Set `SCL_FORMAT_DIRS` to narrow the scan.
- The formatting job runs clang-format 22. On 21 it reads `is_same_v<X &&, ...>`
  as a comparison and demands spaces around the angle brackets, which no other
  version accepts.

### Changed

- Test infrastructure: sources named `*_shared.cpp` under `test/<subdir>/` now build
  into a companion `SHARED` library linked into the subdirectory's test targets,
  enabling tests across a real module (DLL/so) boundary.

### Added

- **Any** — non-owning views over a `std::any` or a typed value
  (`#include <scl/utility/any.h>`):
  - `scl::any_view` — two pointers wide and trivially copyable; constructs from a
    typed lvalue (the RTTI-free *raw* backing, whose `type_name`/`type_key`/
    `has_value` are usable in constant evaluation) or, in RTTI builds, from a
    `std::any` (the *std::any* backing, which delegates casts to `std::any_cast`).
    Rvalues are rejected so a view never outlives its source. `type_key()` hands
    out the `scl::type_key` of the viewed type by pointer (`nullptr` for an empty
    view), so identity is exact even across a module boundary and same-named
    anonymous-namespace types from different translation units never collide.
  - `scl::any_arg` — the parameter-only companion: binds lvalues and rvalues of
    any constness and is valid for the duration of the call. It names
    `scl::any_argument const &`, and that reference is what refuses storage: a
    container of references is ill-formed and the class declares neither a copy
    nor a move constructor, so a data member and `auto` are refused too, while
    passing an argument on binds another reference. No default state, no
    assignment and no conversion to `any_view`, since a view may be stored and an
    argument may not; a callee that only reads for the duration of a call takes an
    `any_arg` of its own. Same backings and identity queries. Unlike the view it also grants write access —
    `any_cast<T>(arg*)` or `any_cast<T &>` on a referent bound without
    cv-qualifiers, the boxed object of a non-`const` `std::any` included — and
    only it does: a referent taken from an `any_view` is narrowed to the read
    access the view promises. Its casts are also constant-evaluable on the C++20
    baseline, where a view's need P2738 (C++26): an argument reaches its referent
    through an anchor the caller materialises per binding, so the argument must be
    a parameter and there is no `constexpr` argument variable. The anchor is a
    descriptor itself and only constant evaluation reads it, so the type stays two
    pointers wide and its run-time behaviour is a view's. Where `__cpp_constexpr`
    reports P2738, the anchor is not compiled at all and both bounds lift: a cast
    folds for an `any_arg` in any position and over a referent adopted from an
    `any_view`, with no change at the call site and none to the layout.
  - `scl::any_cast<T>` — pointer form (`noexcept`, `nullptr` on mismatch) and a
    throwing value/reference form where `T const &` binds with no copy. A cast
    must carry every cv-qualifier the referent was bound with, so a `volatile`
    object is read as `T volatile`, and a write — which carries none — reaches
    only an unqualified referent. For an `any_view` the coverage rule reaches the
    handle itself, so a `volatile` view requires `volatile` in the request the
    same way a `volatile` referent does. An argument adds nothing of its own: it
    is reached through a `const` reference wherever it appears, so its rights come
    from the binding alone. A request naming `std::any` is answered by the box
    itself — the type the identity queries already report for that backing — so a
    `std::any` is taken out of a handle without naming what it holds, and a
    non-`const` binding hands it out for writing. A `std::any` argument converts
    implicitly, so one `any_cast` serves both backings. Recovering the object is
    a runtime step on C++20 (constant-evaluable under P2738/C++26), as with
    `std::any_cast`.
  - `scl::bad_any_cast` — thrown on mismatch; derives from `std::bad_cast` in
    every configuration, never `std::bad_any_cast`, so the type stays
    RTTI-independent and safe to link across mixed RTTI/-fno-rtti builds.
  - `scl::any_switch<Result = void>` — a branch chain replacing a cascade of
    `any_cast` probes: every branch names its type once, the first match runs, and
    the fallback belongs to the same chain. `in_case<T>` selects by the
    qualifier-coverage rule of `any_cast` (`volatile` included), `in_case<void>`
    matches an empty value, and `in_case<std::any>` matches a `std::any` subject
    whole — taking every one of them, and a chain cannot tell a subject held in a
    `std::any` from one bound directly, so only a wider `std::any` case and the
    fallback may follow it. Leave that branch out and the subject is unwrapped so
    the branches see the boxed type. A branch takes an invocable of no arguments or of the matched
    value, and — for a named `Result`, a `void` chain having nothing to convert one
    into — a ready value; `in_case<T>()` with neither is a branch that matches and
    does nothing. Either form may produce a `Result` or the `std::optional<Result>`
    holding one — an empty optional meaning it ran
    and produced nothing — which is what lets one chain stand as a branch of
    another, as a case of it or as its fallback, with an unmatched subject
    falling through as an empty result rather than a wrong one. The optional `or_else` catches what no
    case caught and reads the subject as an `any_arg`. A case an earlier one already
    covers — judged by what it matches, not how it is spelled, so `in_case<T &>`
    after `in_case<T>` or `in_case<T const &>` counts — a second `or_else`, and an
    `in_case` after one are compile errors, so no branch can silently never run.
    The chain holds no subject: `apply(subject)` — spelled `chain(subject)` just as
    well, since a chain is callable — runs at most one branch and
    returns `std::optional<Result>` (nothing for `Result = void`), and
    `has_case(subject)` performs the selection only, `noexcept` and with no branch
    run. So one chain serves any number of subjects, outlives none of them, and
    — the subject being a parameter — selects and runs during constant evaluation
    on the C++20 baseline.

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

- `script/lint/doxygen.sh` — new lint job in both pipelines: fails on every Doxygen
  diagnostic, so neither a block that reaches no entity nor a stale `@param` name can
  reach the published reference.
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
