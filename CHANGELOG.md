# Changelog

All notable changes to the ScL Utility module will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/).

## [Unreleased]

### Added

- `scl::hash::constant_bytes` (`#include <scl/utility/hash/constant_bytes.h>`) - the bytes of
  a range the translation already holds. `scl::hash::byte_view` answers one for a bounded
  array, and only for a bounded array, through a `consteval` overload: an array declared with
  its contents is content the translation has, so its bytes are a value rather than a view
  reading through to something else. Each hash function and `scl::hash::key` take one through
  a `consteval` overload of their own, which makes `hash(byte_view(u"name"))` a constant on
  every compiler where it was the optimiser's choice before - GCC folded it, Clang and MSVC
  left the loop to run on every call.

  Nothing else produces a `constant_bytes`, which is what lets those overloads exist without
  standing in the way of a shape known only at run time: a `std::u16string_view` still selects
  the lazy adapter and is still hashed at run time. Declaring one is not the intended use, and
  hashing one filled at run time does not compile.

  The count of bytes that are text is a member rather than a second template parameter.
  Whether an array ends in a terminator is a property of its contents, not of its type -
  `char16_t const raw[3]{u'A', u'B', u'C'}` carries none - and a return type cannot depend on
  the contents of the argument that produced it.

- `scl::concepts::scoped_enum` - satisfied by an `enum class` or `enum struct` and by
  nothing else. `std::is_scoped_enum` arrives in C++23 and no standard concept spells it,
  which is what this group is for. Uses the trait where the standard library offers it and
  derives the answer from the conversion otherwise.

- `scl::any_mutable_view` (`#include <scl/utility/any/any_mutable_view.h>`) — `scl::any_view`
  granting write access. The Any group tells its handles apart by whether a handle may be
  stored and whether it writes, and one of the four combinations had no handle of its own:
  `scl::any_view` is storable and reads, `scl::any_arg` writes and reads but is valid only
  for the duration of a call, and a write that has to outlive the call setting it up — a
  slot filled later, a binding that pushes a value back — had nowhere to go.
  The new view is the reading one without the narrowing to `const`: two pointers wide,
  trivially copyable, owning nothing. It pays for the write by refusing a `const` object and
  a temporary of any constness outright, at the point of binding rather than at the cast; a
  `volatile` referent binds and is reached under the same qualifier-coverage rule the rest
  of the group obeys. A handle's own cv-qualification governs the handle alone and takes no
  part in a request: a `const` view still hands out a `T *`, and a `volatile` view over an
  unqualified object answers `any_cast<T>`. It converts to `scl::any_view`, which drops the
  write, and to `scl::any_arg`, which keeps it for the duration of a call; neither converts
  back. `scl::any_switch` takes one as a subject, an `in_case<T &>` branch reaches the
  caller's object through it, and an `scl::any` built or assigned from one stores a copy of
  the referent. Identity queries and constant evaluation behave as `scl::any_view`'s
  do, the P2738 limit on casts included. See
  [`doc/md/en/any/any_mutable_view.md`](doc/md/en/any/any_mutable_view.md).

- `scl::any_anchor` (`#include <scl/utility/any/any_anchor.h>`) — a companion object a
  caller declares beside a value so that `scl::any_view` and `scl::any_arg` can cast to
  that value during constant evaluation on the C++20 baseline. Recovering a typed pointer
  from `void const *` is not a constant expression before P2738 (C++26); a downcast to a
  class the object really is, is one, and an anchor is that class — a type description
  carrying the typed pointer. A handle over an `scl::any` needs none, since the object
  already sits in a holder. The anchor is read only during constant evaluation, so a
  binding costs run time nothing, and the type stays in the interface on C++26, where the
  recovery folds on its own, so that C++20 code keeps compiling. See
  [`doc/md/en/any/any_anchor.md`](doc/md/en/any/any_anchor.md).

- `scl::any` (`#include <scl/utility/any.h>`) — an owning holder of a single object of any
  type, doing what `std::any` does in the two configurations `std::any` cannot serve. It
  names types through `scl::type_key` instead of `typeid`, so it compiles with RTTI
  disabled, and its whole lifecycle — construction, `emplace`, `any_cast`, destruction —
  runs during constant evaluation. `sizeof(scl::any)` is two pointers, the size `std::any`
  has on libstdc++ and less than it has elsewhere. An object of at most `sizeof(void *)`
  bytes that is nothrow-movable and no more aligned than a pointer is stored inside the any;
  anything else is allocated
  through the allocator, which `scl::basic_any<Allocator, Capacity>` takes as a parameter
  along with a wider in-place capacity. A stored type has to be destructible without throwing
  and constructible from the arguments given, and whatever alignment it asks for is served: an
  allocated block carries the room to align the object inside itself. An array is refused,
  since the pointer it decays to owns nothing that outlives the call; the type of a string
  literal is the exception, its elements standing for as long as the program runs, and
  `std::array` is what stores the elements themselves. An immovable type is
  admitted because it is allocated and the any moves by handing over the pointer. Copying is
  not a constructor — the type is move-only, which is what lets a non-copyable object be stored
  at all. A copy is asked for through `scl::any::try_copy()`, which answers an empty any when
  the stored type has no copy constructor; `is_copyable()` reports that ahead of the
  attempt. An
  allocator with state is supported, `std::pmr::polymorphic_allocator` included, and it
  travels with the object on move and on swap: storage acquired by one allocator can only
  be released by that same one. `scl::any_view`, `scl::any_arg` and `scl::any_switch`
  accept an `scl::any` and reach its **content**, not the box: `type_name()` answers the
  stored type and a cast costs the same one key comparison it costs over a plain lvalue.
  The view refers into the any's own storage, so emptying, reassigning or moving the any
  leaves it dangling, exactly as `std::string_view` does over `std::string`. The direction
  reverses as well: `scl::any::try_copy(source)` names the copy a container is to take of the
  **object** the source stands for, never of the source itself, and both a constructor and an
  assignment of any specialisation take it. The source is a handle or another container, the
  result reaches nothing on its own, and an allocator may be named beside it with
  `std::allocator_arg`. An object with no copy constructor, and one that is an array, leave
  the container empty, which is why the copy is named rather than reached by a conversion: no
  handle and no container converts to another on its own. Storing the handle itself is the
  explicit spelling `scl::any{std::in_place_type<scl::any_view>, view}`. Replacing an allocated object
  with one the block still holds, allocated as well, keeps the storage already held, so a
  narrower type reuses a block taken for a wider one and repeating it with one type stops
  consuming a `std::pmr::monotonic_buffer_resource`, which never reclaims what it hands out.
  `emplace` does that for any type, since it is defined to destroy first, and leaves the any
  empty when a constructor throws; assignment reaches it while the stored type moves without
  throwing and is no wider than 256 bytes - the value it takes aside is what keeps the stored
  object alive until the replacement stands, so a throwing constructor leaves the any
  untouched. A referent assigned through an `scl::any_view` or an `scl::any_arg` is taken on
  the same terms. The stored object is a new one either way: the assignment operator of the
  stored type is never called, as with `std::any`. Assigning the stored object to its own any,
  as a value or through a handle, does nothing at all, which is what keeps a value whose type
  cannot be copied, where a replacement would leave the any empty.
  `reserve_space_for<T>()` acquires a block ahead of the object that will fill it,
  `shrink_to_fit()` gives back what a narrower object left unused, and `has_space_for<T>()`
  answers whether the storage already held has the room for a `T`; the first two are requests
  rather than guarantees, on the terms the reference states. The in-place capacity is
  `buffer_capacity`, at least `sizeof(void *)`.

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

- `<scl/utility/any/any_cast.h>` - the shared `scl::any_cast` of the group, and
  `scl::any_cast_traits`, which a type specialises to be served by it. A specialisation
  states what its type reaches through one name, `access`, which answers a pointer to the
  object or `nullptr`; its result type is the result type of the cast, and the access rule is
  stated by it, while what the answer points at is compared with the spelling up to
  cv-qualification. A specialisation forwards the work to the source itself and holds no
  algorithm of its own, so it needs no access the source does not already grant. Three shared
  functions serve every source: one takes a pointer and answers `nullptr`, one binds a
  reference to the object and throws, one produces an object of its own and throws. The object
  spelling is offered for a type that can be copied from a `const` object, or where the source
  hands the object over instead - which it does where the source has no name at the call site
  and its trait declares `movable_tag`, the only way a type that moves and does not copy is
  read at all. A type outside the library joins the cast by writing one specialisation in
  namespace `scl` rather than adding an overload to it.
  Each function is offered exactly where the call it forwards compiles and answers such a
  pointer, so a source that cannot answer is turned away at the call rather than inside the
  library, and the exception specification of the pointer form follows from the trait. A
  source is never null inside `access`, which the cast checks once for every specialisation.
  Whether a cast is offered is a constraint, and a compiler remembers the answer per source,
  so the header defining a source belongs before the first question about it. Each header of
  the group now names only what it needs, so a subject spelled as one of the handles needs
  that handle's own header alongside `<scl/utility/any/any_switch.h>`; `<scl/utility/any.h>`
  still pulls in the whole group.
  See [`doc/md/en/any/any_cast.md`](doc/md/en/any/any_cast.md).

- `<scl/utility/any/std_any.h>` - what lets `scl::any_cast` read a `std::any` directly,
  as the `scl::any_cast_traits` specialisation for it. A cast over a box answers `nullptr`
  for a null pointer, an empty box and a box holding another type alike; the box owns what it
  holds, so its own constness reaches the boxed object exactly as it does for `scl::any`. The
  reference forms over a box named as an lvalue hand over a
  copy or bind the stored object without one, throwing `scl::bad_any_cast` on a mismatch, and
  a `volatile` box is refused at the call. The header is where the
  `std::any` interop now lives in full, and it is the only header of the group that names
  `std::any` at all: a cast that never names a `std::any` never instantiates the
  specialisation, so no cast pays for the ability, though `<scl/utility/any.h>` pulls the
  header in along with the rest of the group. Empty without RTTI, and the reference forms are
  declared only where exceptions are available. See
  [`doc/md/en/any/std_any.md`](doc/md/en/any/std_any.md).

### Changed

- The ten members `scl::flags` iteration goes through carry `SCL_FORCE_INLINE`, and the two
  branches inside `next_set` and `prev_set` carry `[[unlikely]]` and `[[likely]]`. MSVC 19.44
  emits a call to `next_set` at `/O2` and iteration pays for it: over four paired rounds
  against a null channel whose p90 sits under 0.7%, forward iteration gains 10 to 16 per cent
  and reverse iteration 32 to 57 - a 32-bit mask walks backwards in 38.7 ns where it took
  91.0. The hints are what GCC 13.1.0 gains from, 5 to 12 per cent on reverse iteration;
  Clang 22.1.8 pays 3 per cent for them on one case and is unmoved otherwise. `.text` on
  `arm-none-eabi-g++` grows from 1044 to 1136 bytes in a translation unit that walks two
  widths. Annotating the whole class rather than the walk costs 202 bytes and buys nothing
  further. `doc/md/en/flags/benchmark.md` carries the full matrix, including the four
  annotations that were measured and left out.
- The block boundary inside SipHash carries a `[[unlikely]]` branch hint. One iteration in
  eight completes an eight-byte block, so the annotation states what the loop already does;
  what it buys is code layout rather than prediction accuracy. Measured over four paired
  rounds against a null channel whose p90 sits between 0.8% and 1.4%: MSVC 19.44 gains 9 to
  11 per cent on a 63-byte input, Clang 22.1.8 is unmoved, and GCC 13.1.0 pays between one
  and three. `.text` on `arm-none-eabi-g++` is unchanged at 1950 bytes. `doc/md/en/hash/benchmark.md`
  carries the full matrix, including the two annotations that were measured and left out.

- **Breaking.** The hash entry point taking a bounded array is `consteval`: a string
  literal, or an array declared with its contents, hashes at translation time or the
  program is ill-formed. `scl::hash::key` gains the matching constructor. Without it,
  whether a literal is folded to a value is the optimiser's choice, and the three
  supported compilers do not agree - GCC folds all five algorithms at `-O2`, Clang folds
  every one but SipHash, and MSVC folds none, leaving 40 to 75 ns of loop on every call.

  A sequence whose contents are known only at run time is now spelled as a view:
  `std::string_view` for a character element, `std::span` for any other. Neither changes
  the hash value - `std::string_view` stops at the same trailing zero the array form drops,
  and the terminator rule never applied to a non-character element, so a `std::span` over
  one covers exactly the bytes the array form covered. `std::array` is a container rather
  than a bounded array and is unaffected.

  A raw array filled at run time no longer compiles, and no compiler's wording points at
  the fix - GCC reports a value not usable in a constant expression, Clang a call to a
  consteval function that is not one, MSVC `C7595`. The fix is always to spell the view.

- Where a template parameter is classified by one of the `scl::concepts` concepts, the
  classification appears in the template parameter list rather than in a trailing
  `requires` clause: every `scl::any_cast` form declared in
  `<scl/utility/any/any_cast.h>` takes a `concepts::object_type` or a
  `concepts::lvalue_reference`, and
  `detail::any_qualifiers_of` takes a `concepts::reference`. The constraint is visible in the
  declaration and a failure names the concept instead of the underlying trait. Which
  specialisations are viable is unchanged.

- `scl::flags` accepts any enumeration, not only a scoped one. The narrowing was a
  `static_assert` in the class body, and nothing in the implementation justified it: the
  only thing read off a value is its underlying integer, and the variadic predicates match
  the enumeration exactly rather than by conversion, so no integer leaks in. Over an
  unscoped enumeration `A | B` is an `int` by integral promotion and there is no
  constructor from one, so the set is spelled `flags{A, B}` there - a compile error rather
  than a silent difference. Code that compiled before compiles unchanged.

- Dropping the result of a hash, a name lookup or a member-pointer cast is a diagnostic.
  `[[nodiscard]]` covers the whole public surface of `hash/` - the five hash functions, every
  hasher `operator()`, `key`'s conversion and its comparison, and the `std::hash`
  specialisation; the entry points of `meta/` (`enum_name`, `enum_short_name`, `symbol_name`,
  `symbol_short_name`, `type_name`, `type_short_name`) and of `runtime/` (`enum_value`,
  `type_name`, `type_short_name`); and `forward_like` with all twenty-six `overload_cast`
  `operator()`s. None of them has an effect other than its return value, so a discarded call
  was always a mistake and nothing said so.

- The library spells a standard attribute as itself. `[[nodiscard]]`, `[[likely]]` and
  `[[unlikely]]` replace the `SCL_` macros at the eight sites that used them, and the
  attribute includes they needed are gone. At the C++20 baseline each of those macros
  expands to exactly the attribute it wraps on every supported compiler, so the indirection
  bought nothing and cost three things: an include, a layout `clang-format` handles natively
  only for the raw form, and a `modernize-use-nodiscard` fix-it that asks for the raw form by
  name. A macro stays where the spelling really differs by toolchain, `SCL_FORCE_INLINE` and
  `SCL_LIFETIMEBOUND` among them, and `attribute/` still offers one for every standard
  attribute for a consumer compiling below C++20.

- `scl::any_view`, `scl::any_arg` and `scl::any_switch` read a value an `scl::any` owns
  during constant evaluation on the C++20 baseline, and an `scl::any` takes a value from
  one of them there too — by construction and by assignment alike, with the default
  allocator. Reaching a stored object is a downcast to the holder an owner keeps it in,
  which a constant expression allows, rather than a recovery from `void const *`, which it
  does not before P2738 (C++26). The run-time path is untouched: a handle stays two
  pointers wide and its cast costs what it did. The limits that remain are stated in
  [`doc/md/en/any/any_view.md`](doc/md/en/any/any_view.md) — a plain lvalue needs an
  `scl::any_anchor`, a `std::any` binding has no constant-evaluation form at all, and an
  `scl::basic_any` with an allocator of its own reads but is not taken from.

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

- Every `scl::any_cast` form that answers a failed request by throwing — the one that produces
  an object, copying it out or moving it out of a source that owns it, and the one that binds
  without a copy — is declared only where `SCL_HAS_EXCEPTIONS` is `1`, as the `typeid` surface
  is declared only under
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

- `scl::hierarchy::tree::payload_reference` and
  `scl::hierarchy::tree::reference::payload_reference`. Both named a mutable reference to a
  stored payload, and neither the tree nor its proxy ever hands one out: a payload is read
  through `value()`, which answers `const_payload_reference`, and written through
  `set_value()`, which notifies the observer. An alias promising direct write access
  advertised a way past `on_change` that does not exist. `node::payload_reference` stays —
  `node::get()` does return one, a node having no observer to notify.

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

- A wide string literal is hashed as its text, without the terminating element.
  `scl::hash::byte_view` gains an overload for a bounded array of a character type, which
  drops a trailing zero the way the hash functions already drop one from an array of `char`
  or `char8_t`. `byte_view(u"AB")` was six bytes against four for
  `byte_view(std::u16string_view{u"AB"})`, so a `case` label written `u"name"` never matched
  a subject built from a run-time view and nothing reported it - the same defect that was
  fixed for a narrow literal before `byte_view` existed, and that `byte_view` did not carry
  across. The rule stays confined to the last element and to character types: a wide array
  not ending in zero is hashed whole, and an array of any other element keeps its trailing
  zero, where a zero is data rather than a terminator.

- Every attribute macro tests `__has_cpp_attribute` behind `defined(...)`, the way each one
  already tested `__has_attribute` and `__has_builtin`. A preprocessor without the operator
  reads `#if __has_cpp_attribute(nodiscard)` as a syntax error rather than as zero, so on the
  one toolchain class the fallback chains exist for - a compiler old enough to lack
  `[[nodiscard]]` is old enough to lack the operator that asks for it - the header failed to
  preprocess and the fallback below it was unreachable. GCC, Clang and MSVC all provide the
  operator in every language mode, so nothing changes for them.

- `scl::hierarchy::tree::reference` hands out mutable reverse iterators. Its
  `reverse_iterator` aliased the tree's *const* one, so `rbegin()` and `rend()` on a mutable
  proxy reached children that could only be read, while `begin()` and `end()` on the same
  proxy reached children that could be written. Reverse iteration over a proxy was covered by
  no test, which is why the asymmetry went unnoticed; the mutability now follows the proxy —
  `reference` mutable, its `const` overloads and `const_reference` immutable — and four tests
  hold it that way.

- `scl::hierarchy::tree::const_iterator` reports the proxy it actually yields. Its
  `value_type` and `reference` named the tree's *mutable* proxy while `operator*` returned
  the immutable one, so `std::iterator_traits` answered with a type the iterator never
  hands out and a generic algorithm taking `iterator_traits<I>::reference` by reference did
  not compile.

- `operator->` reaches a proxy through a reverse iterator of a tree. Both iterators named
  their `pointer` `void` while offering `operator->`, and `std::reverse_iterator` returns
  that `pointer` from its own `operator->`, so `it->push_back(1)` compiled and
  `rit->push_back(1)` did not — on every standard library. The alias now names what
  `operator->` hands back.

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

- The documentation jobs run one pinned generator,
  `registry.gitlab.com/ssoft-docker/ci-images/doxygen:1.18.0-2`, named by tag and digest, on
  GitLab and on the GitHub mirror alike. They installed Doxygen from a rolling distribution
  before, so the version was decided by the date the pipeline ran and a passing tree started
  failing on a Doxyfile tag the newer release calls obsolete. The publishing job takes its
  generator from the same image, so what is checked and what is published no longer come
  from two independent installations.
- The Doxyfile no longer names the four `CLANG_*` tags. Clang-assisted parsing was off, so
  they changed nothing about the reference, while the pinned generator is built without
  libclang — 140 MB of an image that never called it — and reports each of them as an option
  it was not compiled with, which the documentation check counts as output and fails on.
- The GitHub workflows check out with `actions/checkout@v7`, the current major, instead of
  `v6`.
- The GitLab pipeline runs for a merge request, for `dev` and `main`, and for a version
  tag. A plain branch push started one before: every check filtered itself out, but the
  GitHub mirror took any branch, so pushing a topic branch spent runner minutes copying
  the whole repository. The checks now answer for a merge request alone — the same
  commits reach `dev` through one, and GitHub lints `dev` and `main` at no cost — while
  the mirror waits for `dev` or `main` to move, or for a tag, and carries every branch
  and tag across in that one run. The two clang checks share a job, as do the two
  documentation ones, so a merge request pulls three images instead of five, and
  `clang-tidy` scans headers in parallel (`SCL_LINT_JOBS`, one per core by default),
  which halves the longest check on a two-core runner. On GitHub the lint workflow no
  longer runs for a pull request — the repository is a read-only mirror whose pull
  requests a workflow of its own closes.
- The documentation check holds description coverage rather than attachment alone.
  `WARN_NO_PARAMDOC` is on, so a documented function that names its target and then leaves a
  parameter or its return value undescribed fails it; the 136 omissions that switch reported
  across `hierarchy`, `hash`, `meta` and `type_traits` are described rather than silenced.
- `scl::basic_any`'s two deleted copy members and its destructor reach the reference with a
  description, and `scl::overload_cast` is declared there without naming a type in
  `scl::detail`, on its own page and on both Markdown pages. A `= delete` or `= default`
  member draws no `WARN_IF_UNDOCUMENTED` warning and is left off its class page instead of
  being listed there undescribed, so those three were absent from the reference with nothing
  reporting it.
- `readability-redundant-typename` is on for `src/`. It was switched off when the Detection
  Idiom aliases looked as though `typename` were mandatory there, which was the C++17 rule;
  P0634R3 made it optional in a defining-type-id, so the diagnostic was right. The check
  exists only from clang-tidy 22 on, so both `clang-tidy` jobs move to that image — a
  contributor writing the keyword where it is redundant now hears about it.
- `project/cmake/benchmark/CMakeLists.txt` builds a `benchmark/` tree with Google Benchmark,
  one target per subdirectory. The naming rule was already written; this is the plumbing it
  was waiting for. The `<tool>` token is `gbench`, so a `benchmark/<group>/` directory builds
  into `utility_<group>_gbench`. No group is measured yet - each arrives with the task that
  measures it, so with no `benchmark/` directory the option configures and produces nothing.
  Benchmarks are off by default and have no preset of their own - see CONTRIBUTING.md
  "Benchmarks" for the two commands.
- The first measured group is `hash`: `benchmark/hash/` times the five hash functions at
  8, 16 and 63 bytes, over a `std::string_view` whose length is a run-time value and over a
  `std::array<char, N>` whose length the compiler sees, plus a `std::string` and a hasher
  at the widest size. Three sizes rather than one because what an annotation is worth
  depends on them: inlining removes per-call overhead, whose share grows as the input
  shrinks, and 8 to 16 bytes is what a hash-map key usually is while 63 is the size the
  claim on this work was first made at. The array form is a `std::array` rather than a
  `char[]`, whose last element would be dropped as a terminator and leave the forms
  measuring different amounts of work. `hash_size.cpp` gives each function a wrapper of its
  own so `-ffunction-sections` puts every one in a section that can be read separately.

  Four cases answer questions the plain timings cannot. A literal goes through a
  non-inlinable wrapper, since its hash is loop-invariant and the benchmark loop would
  otherwise hoist the call out and time an empty loop - which is what makes it visible
  whether a compiler folded the hash to a constant or left the byte loop to run. A wide
  range is timed through `byte_view` against a narrow range of the same 126 bytes, so the
  adapter is the only difference between the two. `icache_gbench.cpp` calls the hash from a
  thousand separate sites, so what an inlined body costs when it is duplicated across call
  sites shows up instead of staying resident in L1 the way a single-site loop keeps it. And
  the accumulated `fnv1a` cases keep the result in a register across iterations rather than
  passing it through a barrier each time, which measures the hash instead of the barrier.
- The second measured group is `flags`: `benchmark/flags/` times construction, the set
  algebra, the predicates, `size()`, indexing and iteration in both directions, over a mask
  with every fourth bit set, at 32 bits and again at 256. Two widths rather than one because
  every whole-byte loop in the class is eight times longer in the wide case while the number
  of set bits stays at eight, which separates a difference the loop carries from one the call
  carries. `icache_gbench.cpp` walks a mask a thousand times in every case and varies only how
  many distinct bodies those walks are spread over, from one site to a thousand, so what an
  inlined walk costs once it stops fitting in the instruction cache is measurable rather than
  assumed. `flags_size.cpp` gives each operation a wrapper of
  its own, at both widths, so `-ffunction-sections` puts every one in a section that can be
  read separately.
- The third measured group is `hierarchy`: `benchmark/hierarchy/` builds a tree eight children
  wide and four levels deep, then times the walk over one level and over the whole tree, the
  ancestor and sibling queries at one step and at full depth, and an insertion. The tree is
  that shape so a walk costs more than the call reaching it and the ancestor query has
  something to climb. `tree_gbench.cpp` repeats the walk and the insertion through
  `scl::hierarchy::tree`, whose mutations also notify an observer, so the wrapper is measured
  rather than assumed to cost what the node costs. `hierarchy_size.cpp` gives each operation a
  wrapper of its own for `-ffunction-sections`.
- A second benchmark `<tool>` token, `size`: a `benchmark/<group>/*_size.cpp` source builds
  into `utility_<group>_size`, a static library whose sources are compiled to be measured and
  are never linked or run. That is what lets them build for a bare-metal cross compiler, so a
  change that trades code size for speed can be judged on both.
- A `doc-snippets` gate: a Markdown code block introduced by an HTML comment naming a
  source file must repeat the region of that file between its `//! [quick_start]`
  markers, so a documented program and its copy in the text cannot drift apart
  unnoticed. `script/lint/doc_snippets.sh` documents the exact spelling.
- The formatting gate covers `test/`, `example/` and `benchmark/` as well as `src/`, and
  the sources as well as the headers. It previously read `src/*.h` only, so three
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

- **Any** — non-owning views over a typed value
  (`#include <scl/utility/any.h>`):
  - `scl::any_view` — two pointers wide and trivially copyable; constructs from a
    typed lvalue of any type, whose `type_name`/`type_key`/`has_value` are usable in
    constant evaluation. A `std::any` is such a type like any other and is named as
    the box it is, so nothing about the view is conditional on a build setting.
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
    `any_arg` of its own. Binds the same types and answers the same identity queries. Unlike the view it also grants write access —
    `any_cast<T>(arg*)` or `any_cast<T &>` on a referent bound without
    cv-qualifiers, a non-`const` `std::any` itself included — and
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
  - `scl::any_cast<T>` — one cast for every source, declared in
    `<scl/utility/any/any_cast.h>`: a pointer form (`noexcept`, `nullptr` on
    mismatch) and two throwing forms, one binding `T &`/`T const &` without a copy
    and one copying out. The spelling asked for and the source's access rule decide
    the result type; the binding decides whether it is answered. A cast must carry
    every cv-qualifier the referent was bound with, so a `volatile` object is read
    as `T volatile`, and a write — which carries none — reaches only an unqualified
    referent. A handle's own cv-qualification governs the handle and not what it
    refers to, so it takes no part in the request: a `volatile` view over an
    unqualified object answers `any_cast<T>`, and the rights come from the binding
    alone. An owner is the exception, since it keeps the object as a subobject of
    its own: a `const` owner answers `T const *`, and a `volatile` one is refused by
    every form the trait governs rather than failing inside the library. A handle
    names the type it is bound to and nothing else, so a request naming `std::any` is
    answered by the box itself — the type the identity queries already report for
    such a binding — and a request
    naming any other type answers `nullptr` or throws: reading the object a box
    holds is what `<scl/utility/any/std_any.h>` is for. A non-`const` binding hands
    the box out for writing. Recovering the object is a runtime step on C++20
    (constant-evaluable under P2738/C++26), as with `std::any_cast`.
  - `scl::bad_any_cast` — thrown on mismatch; derives from `std::bad_cast` in
    every configuration, never `std::bad_any_cast`, so the type stays
    RTTI-independent and safe to link across mixed RTTI/-fno-rtti builds.
  - `scl::any_switch<Result = void>` — a branch chain replacing a cascade of
    `any_cast` probes: every branch names its type once, the first match runs, and
    the fallback belongs to the same chain. `in_case<T>` selects by the
    qualifier-coverage rule of `any_cast` (`volatile` included), `in_case<void>`
    matches an empty value, and `std::any` is a case type like any other:
    `in_case<std::any>` matches a subject bound to one, no branch reads what that
    box holds, and a branch may stand on either side of it.
    A branch takes an invocable of no arguments or of the matched
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
