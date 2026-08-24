# Hash Benchmarks

- Suite: `benchmark/hash/`
- Configure: `cmake --preset <preset> -DSCL_BUILD_BENCHMARKS=ON`
- Timing target: `utility_hash_gbench`
- Size target: `utility_hash_size`

## What this page answers

Two questions, and they pull in opposite directions.

The first is whether an annotation on a hash function earns its keep: `SCL_FORCE_INLINE`,
`SCL_HOT`, a branch hint on the block boundary inside SipHash. The second is what such an
annotation costs in code size, which is what decides the matter on a device where the
whole image has a budget.

A third question sits beside them and is answered differently: whether hashing a string
literal costs anything at run time at all. That one is settled by the compiler, not by an
annotation, so it is measured per compiler rather than per variant.

## Method

### Pairing

Every variant is compared against a baseline built from the same sources in the same
round, and the rounds run back to back. A machine that warms up over the hour warms up
under both members of every pair, so the shared drift cancels in the difference. Compared
across rounds instead, the same binaries differ by 8 to 12 percentage points in the order
the rounds ran in — a reading of the processor's clock rather than of the library.

The baseline runs twice in each round, first and last, and the difference between those
two runs is reported alongside the results. It is the null channel: two runs of one
binary must agree, so whatever separates them is the floor below which no difference in
this table means anything.

### Settings

Each case runs 15 repetitions of at least 0.5 seconds, the process pinned to one logical
processor and raised to high priority. The median of the repetitions is the case's value.

The length is what the measurement needs rather than what it can afford. A run of a few
seconds spends most of itself on a processor that has not settled at a steady clock, so it
measures the ramp; shortening it does not collect fewer samples of the same thing, it
changes what is sampled. Cut to a fifth of a second and five repetitions, two runs of one
binary separate by 4% at p90 instead of the 1% they separate by here - wider than most of
what this page reports.

## What the annotations are worth

Four rounds on each of three compilers, paired within the round. The floor first, because
no number below it means anything:

| Null channel | median | p90 | worst |
|---|---|---|---|
| Clang 22.1.8 | −0.04% | 1.21% | 5.82% |
| GCC 13.1.0 | −0.05% | 0.82% | 4.21% |
| MSVC 19.44 | +0.06% | 1.43% | 4.56% |

Read anything under roughly 1.5% as noise.

### A branch hint on SipHash's block boundary — the one that pays

One iteration in eight completes an eight-byte block. Marking that branch `[[unlikely]]`:

| Case | Clang | GCC | MSVC |
|---|---|---|---|
| `siphash_string_view_63` | noise | +1.64% | **−10.59%** |
| `siphash_hasher_string_view_63` | noise | +3.13% | **−10.79%** |
| `siphash_char_array_63` | noise | noise | **−9.20%** |
| `siphash_string_view_8` | noise | +1.37% | noise |
| `siphash_char_array_8` | noise | noise | noise |

Tight across rounds: MSVC's −10.59% spans −10.78% to −10.23% over the four. An eight-byte
input reaches the boundary once, so there is nothing there for a hint to shape — the effect
belongs to the loop, not to the call.

It costs nothing in code size, and it is applied. GCC pays between one and three per cent
for it; a build that targets GCC alone and minds that can spell the branch without the hint.

Adding `SCL_HOT` on top changes nothing on MSVC (−9.16%, the same figure) but costs Clang
**+4.1%** on two cases where either annotation alone sat in the noise. The combination is
not the sum of its parts.

### `SCL_FORCE_INLINE` — measured, not applied

What cleared the floor, all four rounds agreeing:

| Case | Clang | MSVC | GCC |
|---|---|---|---|
| `siphash_char_array_8` | **−18.83%** | −6.65% | noise |
| `siphash_string_view_8` | −4.10% | −4.47% | noise |
| `sdbm_char_array_63` | −8.23% | noise | noise |
| `fnv1a_char_array_63` | −6.13% | noise | **+2.22%** |
| `djb2_char_array_63` | −3.24% | noise | noise |
| `jenkins_ota_char_array_63` | −2.31% | noise | noise |
| `siphash_string_view_63` | −1.56% | −1.69% | noise |
| `siphash_hasher_string_view_63` | −1.57% | −2.12% | noise |
| `siphash_char_array_63` | −1.34% | **+2.22%** | noise |
| `sdbm_string_view_8` | noise | noise | **+2.93%** |
| `sdbm_char_array_8` | noise | **+1.86%** | noise |

It is a real gain on Clang for a short SipHash input and a real loss on MSVC for a long
array. On GCC it is noise in one direction and a regression in the other. Set against
**+89% of `.text`** (below), that is not a trade a library should make on its user's behalf,
so the annotation is not applied. A caller who wants it for one hot site can force-inline
their own wrapper around the call, which keeps the cost where the benefit is.

### `SCL_HOT` — measured, not applied

Nothing above the floor except two MSVC cases, `fnv1a_string_view_63` and
`fnv1a_hasher_string_view_63`, at −5.4%.

Those same two cases moved by the same −5.4% under a different variant altogether — force
inlining the hashers' `operator()` and nothing else. Two unrelated edits producing one
figure on one pair of cases points at code layout rather than at either annotation. The
mechanism is not established, so neither annotation claims the gain.

## Code size

`.text` of `libutility_hash_size.a`, `arm-none-eabi-g++` 13.2.1, Cortex-M4, MinSizeRel:

| Variant | bytes | against base |
|---|---|---|
| base | 1950 | — |
| `SCL_FORCE_INLINE` | 3690 | **+1740, +89.2%** |
| `SCL_HOT` | 1950 | 0 |
| branch hint | 1950 | 0 |
| both | 1950 | 0 |

The symbol table says where the 1740 bytes go. Without the annotation `sip_round` and the
three `siphash<...>` instantiations are emitted once each — 216, 348, 352 and 352 bytes —
and the three call sites are 32, 32 and 36 bytes of wrapper. With it there are no shared
symbols left and the three call sites are 1038, 1038 and 1042 bytes each: one body becomes
one copy per call site.

Three call sites in a translation unit that hashes three ways. A program that hashes in
thirty places pays thirty times.

## The wide-element adapter

A range whose element is wider than a byte reaches the hash through
[`scl::hash::byte_view`](key.md), which answers a lazy view computing each byte on demand. What
that costs, against a narrow range of the same 126 bytes:

| | through `byte_view` | narrow | ratio |
|---|---|---|---|
| Clang | 182.17 ns | 113.15 ns | 1.61× |
| GCC | 173.58 ns | 105.86 ns | 1.64× |
| MSVC | 187.27 ns | 121.65 ns | 1.54× |

Around one and a half times per byte, and the byte count doubles with the element width —
63 `char16_t` are 126 bytes. A `std::u16string_view` key therefore costs roughly three
times what the same text costs as `std::string_view`, which is worth knowing before
choosing the encoding a key travels in.

## Constant folding of a string literal

`scl::hash::fnv1a("literal")` in an ordinary expression — not bound to a constant — may be
folded to a value at translation time or emitted as a loop that runs on every call. The
standard permits both: `constexpr` means compile-time evaluation is allowed, not that it
happens.

Which one you get, at `-O2` and `/O2`:

| Compiler | Folded | Left as work |
|---|---|---|
| GCC 13.1.0 | all five algorithms | — |
| Clang 22.1.8 | `fnv1a`, `djb2`, `sdbm`, `jenkins_ota` | `siphash` |
| MSVC 19.44 | — | all five |

Measured, in nanoseconds per call, against the same call declared `consteval`:

| Case | Clang | GCC | MSVC |
|---|---|---|---|
| `fnv1a` | 0.223 → 0.222 | 0.109 → 0.110 | **41.2 → 1.97** |
| `djb2` | 0.222 → 0.221 | 0.109 → 0.109 | **40.5 → 1.81** |
| `sdbm` | 0.222 → 0.222 | 0.109 → 0.110 | **40.4 → 1.81** |
| `jenkins_ota` | 0.221 → 0.222 | 0.109 → 0.109 | **75.0 → 1.79** |
| `siphash` | **58.4 → 0.223** | 0.109 → 0.109 | **67.1 → 1.98** |

A folded call reaches the floor of an empty loop iteration, so the left column of each
pair says plainly which compiler folded and which did not. MSVC folds none of the five;
Clang folds every one but SipHash.

The remaining 1.8 ns on MSVC is the measurement's own: a literal's hash is loop-invariant,
so each case goes through a non-inlinable wrapper to keep the call from being hoisted out
of the benchmark loop, and MSVC keeps that wrapper as a real call returning a constant.

### Getting the guarantee

Bind the result to a constant, and every compiler folds it:

```cpp
constexpr auto value = ::scl::hash::fnv1a("event.started");
foo(value);
```

A bounded array is a shape the compiler already holds, so the entry point that takes one
is `consteval`: its result is a constant or the program does not compile. A sequence known
only at run time is spelled as a view, which is what selects the `constexpr` entry point.

```cpp
constexpr char name[] = "event.started";
foo(::scl::hash::fnv1a(name));                          // constant, on every compiler

char buffer[64];
auto const size = receive(buffer);
foo(::scl::hash::fnv1a(::std::string_view{buffer, size}));   // run time
```

Passing a raw array whose contents are not known at translation time is a compile error,
and no compiler's wording points at the fix:

| Compiler | Diagnostic |
|---|---|
| GCC | `the value of 'buffer' is not usable in a constant expression` |
| Clang | `call to consteval function 'scl::hash::fnv1a<char, 4ULL>' is not a constant expression` |
| MSVC | `error C7595: call to immediate function is not a constant expression` |

The fix is always the same: spell the run-time sequence as a view. `std::string_view` for
character elements, `std::span` for any other byte element. Neither changes the hash value.
`std::string_view` stops at the same trailing zero the array form drops, and the terminator
rule never applied to a non-character element in the first place, so a `std::span` over one
covers exactly the bytes the array form covered.

`std::array` is a container rather than a bounded array, so it stays a run-time shape and
needs no view.

## Trading speed for size

There is no trade to make. The only annotation the group applies is the branch hint,
and it costs nothing in code size, so a build that needs a smaller image has nothing
to switch off here. The annotation that would have cost 89% of `.text` is measured on
this page and not applied.

What is left to a caller is the other direction. `SCL_FORCE_INLINE` and every other
attribute macro in `scl::utility::attribute` is declared inside an outer `#ifndef`, so
a project may define its own before including the header. That governs the macros the
library uses; it cannot add an annotation to a site that carries none. To force-inline
a hash at one hot call site, annotate a wrapper of your own:

```cpp
#include <scl/utility/attribute/inline.h>

SCL_FORCE_INLINE ::std::uint64_t hash_of_packet(::std::span<::std::byte const> bytes)
{
    return ::scl::hash::siphash(bytes);
}
```

The cost then lands in the one translation unit that asked for it, rather than in
every program that includes the header.
## See also

- [key](key.md) — the strongly-typed hash value and the hashers behind it
- [Russian documentation](../../ru/hash/benchmark.md)
