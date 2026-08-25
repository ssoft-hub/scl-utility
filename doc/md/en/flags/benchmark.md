# Flags Benchmarks

- Suite: `benchmark/flags/`
- Configure: `cmake --preset <preset> -DSCL_BUILD_BENCHMARKS=ON`
- Timing target: `utility_flags_gbench`
- Size target: `utility_flags_size`

## What is applied

`SCL_FORCE_INLINE` on the ten members iteration goes through, and `[[unlikely]]` and
`[[likely]]` on the two branches inside `next_set` and `prev_set`. Nothing else.

Measured as it ships, over four paired rounds:

| Case | Clang | GCC | MSVC |
|---|---|---|---|
| `flags_reverse_iterate_32` | noise | **-4.77%** | **-57.50%** |
| `flags_reverse_iterate_256` | noise | **-11.82%** | **-32.06%** |
| `flags_iterate_256` | +2.98% | **-2.10%** | **-15.71%** |
| `flags_iterate_32` | noise | noise | **-10.12%** |
| `flags_size_32` | noise | -2.72% | **+23.47%** |
| `flags_size_256` | noise | -2.24% | noise |
| `flags_index_32` | noise | noise | -6.37% |
| `flags_construct_32` | noise | -9.54% | noise |

Reverse iteration over a 32-bit mask ends at 38.7 ns against the baseline's 91.0. It costs
92 bytes of `.text` and two regressions: Clang pays 2.98% on wide forward iteration for the
hints, and MSVC pays 23.47% - 0.64 ns - on `flags_size_32`. The bottom two rows and GCC's
two `size` rows are layout rather than the annotation, which the control below establishes.

## Method

Every variant is compared against a baseline built from the same sources in the same round.
The baseline runs twice per round, first and last, and the difference between those two runs
is the floor below which nothing in these tables means anything. Each case runs 15
repetitions of at least 0.5 seconds, pinned to one logical processor at high priority; the
median of the repetitions is the case's value.

Every case runs over a mask with every fourth bit set, at 32 bits and again at 256, so every
whole-byte loop in the class is eight times longer in one than in the other while the number
of set bits stays at eight.

One variant annotates the nested iterator and nothing else. For a case that never builds an
iterator that is a control, and on this suite it moves such a case by six per cent - the size
of the layout effect here.

| Null channel | median | p90 | worst |
|---|---|---|---|
| Clang, first pass (rounds 1-3) | -0.23% | 2.56% | 5.71% |
| GCC, first pass | -0.02% | 0.62% | 3.84% |
| MSVC, first pass | -0.09% | 0.54% | 6.82% |
| Clang, second pass | -0.01% | 1.21% | 1.54% |
| GCC, second pass | +0.03% | 0.50% | 1.38% |
| MSVC, second pass | -0.27% | 0.47% | 4.64% |

Clang's first pass rests on three rounds: in the fourth, the two baseline runs differ by 6.2%
at the median.

## What the base costs

| Case | Clang 22.1.8 | GCC 13.1.0 | MSVC 19.44 |
|---|---|---|---|
| `flags_union_32` | 0.256 | 0.222 | 3.350 |
| `flags_union_256` | 0.580 | 0.572 | 2.975 |
| `flags_any_of_pack_32` | 0.340 | 0.464 | 2.246 |
| `flags_index_32` | 0.679 | 0.888 | 3.388 |
| `flags_construct_32` | 1.049 | 1.125 | 2.448 |
| `flags_size_32` | 1.803 | 7.726 | 2.781 |
| `flags_size_256` | 20.722 | 72.218 | 21.320 |
| `flags_iterate_32` | 20.572 | 14.496 | 24.456 |
| `flags_iterate_256` | 26.335 | 25.022 | 36.280 |
| `flags_reverse_iterate_32` | 33.633 | 29.975 | 91.207 |
| `flags_reverse_iterate_256` | 73.645 | 57.162 | 129.178 |

On MSVC nothing measures below 2.2 ns: a union of two 32-bit masks is three instructions and
reads 3.35 ns against Clang's 0.256. That is the benchmark's barrier, which forces both
operands and the result to memory every iteration, so a percentage on any case at or under
about 3.4 ns in that column says nothing about the library.

`size()` on GCC costs three and a half times what the other two charge. No variant on this
page moved it.

## Force inlining, and how far it has to reach

`SCL_FORCE_INLINE` on the class, on what iteration goes through, and on the nested iterator
alone, on MSVC:

| Case | whole class | the walk | the iterator only |
|---|---|---|---|
| `flags_reverse_iterate_32` | **-56.94%** | **-56.96%** | -44.63% |
| `flags_reverse_iterate_256` | **-31.71%** | **-31.74%** | -25.91% |
| `flags_iterate_256` | **-15.76%** | **-15.81%** | -0.95% |
| `flags_iterate_32` | **-10.03%** | **-10.09%** | -1.29% |
| `flags_index_32` | -6.34% | -6.37% | -6.43% |
| `flags_size_32` | noise | +22.92% | noise |

A translation unit that does nothing but walk a `flags`, compiled at `/O2`, emits two
`call`s to `scl::flags<bit,32>::next_set`; with the annotation it emits none. Annotating the
walk - the iterator's members plus `next_set`, `prev_set` and `bits_at` - reproduces the
whole class's result to within six hundredths of a point at ten sites instead of forty-eight.
Annotating the iterator alone keeps the reverse cases and loses the forward ones, which is
where `next_set` is spent.

`flags_index_32` builds no iterator, yet all three variants move it by the same -6.4%: that
is layout. On GCC the same reading disposes of the whole-class variant, which reads +10.98%
on `flags_size_32` where the control reads +8.08% on a case it cannot touch. Clang read noise
throughout except -2.43% on `flags_iterate_256`.

## Branch hints

Five branches carry an obvious direction: two bounds checks that end the program, the two
exits of `next_set`, and the found-a-bit branch of `prev_set`. On GCC:

| Case | GCC |
|---|---|
| `flags_reverse_iterate_256` | **-12.00%** |
| `flags_reverse_iterate_32` | **-4.92%** |
| `flags_iterate_256` | **-2.56%** |
| `flags_construct_32` | -8.94%, layout |

Clang and MSVC read noise on every one. `flags_construct_32` reaches no hinted branch that
fires, and its figure matches the control's -8.91% on the same case.

Only the two hints inside the walk are applied. The bounds checks guard a call to a
`[[noreturn]]` function, which a compiler already biases.

## What cleared the floor nowhere

- `SCL_HOT` on every member of the class. Expands to nothing on MSVC.
- `SCL_COLD` on `detail::reject_flag_ordinal`, which is already `[[noreturn]]`.
- `SCL_LIKELY_EXPR` on the width check inside `has_bit`, the one condition in the class that
  is an operand rather than a statement. Expands to nothing on MSVC.

## Code size

`.text` of `libutility_flags_size.a`, `arm-none-eabi-g++` 13.2.1, Cortex-M4, MinSizeRel:

| Variant | bytes | against base |
|---|---|---|
| base | 1044 | - |
| `SCL_FORCE_INLINE`, whole class | 1246 | **+202, +19.3%** |
| `SCL_FORCE_INLINE`, the walk | 1136 | **+92, +8.8%** |
| `SCL_FORCE_INLINE`, the iterator only | 1044 | 0 |
| `SCL_HOT` | 1044 | 0 |
| branch hints | 1044 | 0 |
| `SCL_LIKELY_EXPR` in `has_bit` | 1044 | 0 |
| `SCL_COLD` on the rejection | 1044 | 0 |

| Symbol | base | the walk | whole class |
|---|---|---|---|
| `flags<32>::next_set` | 48 | inlined | inlined |
| `flags<256>::next_set` | 50 | inlined | inlined |
| `flags<32>::begin` | inlined | 52 | inlined |
| `flags<256>::begin` | inlined | 40 | inlined |
| `flags<32>::operator&` | 40 | 40 | inlined |
| `flags<32>::set_bit` | 28 | 28 | inlined |
| `flags_iterate` wrapper | 36 | 82 | 124 |
| `wide_flags_iterate` wrapper | 48 | 94 | 110 |
| `flags_and` wrapper | 20 | 20 | 76 |
| `flags_construct` wrapper | 30 | 30 | 68 |

Annotating the whole class leaves no shared symbol: every call site carries its own copy.
Annotating the walk moves the boundary one step out instead, `next_set` disappearing into
`begin`, so the cost is less than half. It scales with call sites, not with the library.

## Where the inlined walk stops paying

Every case walks the same mask a thousand times; what differs is how many distinct bodies
those walks are spread over. The baseline alone, microseconds per thousand walks:

| Sites | Clang | GCC | MSVC |
|---|---|---|---|
| 1 | 18.6 | 18.8 | 21.0 |
| 8 | 18.8 | 21.3 | 20.9 |
| 64 | 29.1 | 21.5 | 21.0 |
| 256 | 42.6 | 25.1 | 24.6 |
| 1024 | 70.9 | 54.0 | 40.1 |

What the applied annotation does to each:

| Sites | Clang | GCC | MSVC |
|---|---|---|---|
| 1 | +13.06% | **-6.91%** | **-10.54%** |
| 8 | noise | **-17.32%** | **-8.85%** |
| 64 | +1.99% | **-5.43%** | **+10.25%** |
| 256 | noise | **-1.42%** | **+3.94%** |
| 1024 | noise | **-1.93%** | **+43.79%** |

On MSVC the annotation is worth 9 to 11 per cent up to eight sites and costs 44 per cent at a
thousand; the crossing is between eight and sixty-four. GCC gains at every site count.
Clang's column is layout again - its +13.06% at one site is tight across rounds but reverses
sign between binaries built from the same sources.

A program that walks a `flags` in a handful of hot places gets the gain; one that walks it
from a hundred cold places pays for the copies.

## Trading speed for size

Define `SCL_FORCE_INLINE` empty before including the header - to get the 92 bytes back, or
because the walk would be inlined at more call sites than the instruction cache will hold:

```cpp
#define SCL_FORCE_INLINE
#include <scl/utility/flags.h>
```

Every attribute macro in `scl::utility::attribute` is declared inside an outer `#ifndef`, so
a definition that arrives first wins. That governs the macros the library uses; it cannot
remove the two branch hints, which are standard attributes spelled directly and cost nothing
in code size.

## See also

- [flags](flags.md) - the class itself
- [Russian documentation](../../ru/flags/benchmark.md)
