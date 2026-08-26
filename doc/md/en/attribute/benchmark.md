# Attribute Measurements Across Groups

Four groups were measured against every optimisation attribute that has a site in them:
[hash](../hash/benchmark.md), [flags](../flags/benchmark.md),
[hierarchy](../hierarchy/benchmark.md) and [any](../any/benchmark.md). Each group page
carries its own matrix; this one answers the question the group pages cannot: for a given
attribute, is it worth applying at all.

The short answer is that nothing generalises. The same attribute is worth 57 per cent in one
group, nothing in the next, and a regression in the third.

## What is applied, and where

| Attribute | hash | flags | hierarchy | any |
|---|---|---|---|---|
| `SCL_FORCE_INLINE` | rejected | **applied** to the walk | **applied** to the walk | rejected |
| `SCL_HOT` | rejected | rejected | rejected | **removed** |
| `[[likely]]` / `[[unlikely]]` | **applied** to one branch | **applied** to two | rejected | **removed** |
| `SCL_LIKELY_EXPR` | no site | rejected | no site | **removed** |
| `SCL_COLD` | no site | rejected | no site | no site |

"Removed" is the `any` group: it arrived annotated, and the measurement took the annotations
away again.

## `SCL_FORCE_INLINE`

| Group | Clang | GCC | MSVC | `.text` | Verdict |
|---|---|---|---|---|---|
| hash | -1.3% to -18.8% | noise, +2.2%, +2.9% | -1.7% to -6.7%, +1.9%, +2.2% | **+89.2%** | rejected: the size |
| flags, whole class | noise | +11.0% on `size` | -10.0% to -56.9% on iteration | +19.3% | rejected: the walk does the same |
| flags, the walk | noise | noise | -10.1% to -57.5% on iteration | **+8.8%** | **applied** |
| hierarchy, the walk | noise | -27.3% on one case | noise | 0 | **applied** |
| any, the casts | -58.6% to -86.6%, **+32.4%** on a switch | -31.8% on one case | -50.0%, **+10.0%** on an empty handle | -12 bytes | rejected: the regressions |

Where it pays, it pays because MSVC leaves small header members out of line at `/O2`. A
translation unit that walks a `scl::flags` emits two `call`s to `next_set` without the
annotation and none with it; the same reading holds for the proxy boundary in `hierarchy`,
where GCC is the one leaving the call.

Where it does not pay, the reason is usually the size. The hash group's `.text` grows by 89
per cent because every call site gets its own copy of the hash body, and duplication has a
second cost the size table does not show: past a few dozen call sites executed together,
the copies stop fitting in the instruction cache and the annotation turns against itself -
on MSVC a walk from a thousand sites costs 44 per cent more than the same walk from one.

## `SCL_HOT`

| Group | Result |
|---|---|
| hash | nothing above the floor; +4.1% on Clang when combined with the branch hint |
| flags | nothing; expands to nothing on MSVC |
| hierarchy | nothing; one MSVC cell at +1.88% with a spread crossing zero |
| any | Clang: **+10 to +16 per cent** on the two miss paths, -13% on the empty one |

Not one group found a case for it, and in the group that shipped with it, it was making the
common paths slower. Nowhere applied.

## Branch hints

| Group | Clang | GCC | MSVC | Verdict |
|---|---|---|---|---|
| hash, SipHash's block boundary | noise | +1.4% to +3.1% | **-9.2% to -10.8%** | **applied** |
| flags, the two walk branches | +3.0% on one case | **-2.6% to -12.0%** | noise | **applied** |
| hierarchy, the ancestor climb | -15.0% once, not reproduced twice | noise | noise | rejected |
| any, the null checks | nothing at all | nothing | nothing | removed |
| any, the successful return | noise | ±9% by case | noise | removed |
| any, the assignment guards | +8% one case, -9% another | noise | noise | removed |

They cost nothing in code size in every group, which is why the bar for keeping one is only
that it help somewhere and hurt nowhere. Two cleared it. The `hierarchy` hint is the
instructive failure: a first pass put it at -15.0% with a spread from -26.3% to -8.6%, and
two later passes - the hint alone, and the shipped configuration - read noise. One reading
out of three does not carry an annotation.

## Code size

`.text` of each group's size library, `arm-none-eabi-g++` 13.2.1, Cortex-M4, MinSizeRel:

| Group | base | with force inlining | with every other attribute |
|---|---|---|---|
| hash | 1950 | 3690 (+89.2%) | 1950 |
| flags | 1044 | 1136 the walk, 1246 the whole class | 1044 |
| hierarchy | 422 | 422 | 422 |
| any | 252 | 240 | 252 |

Only force inlining ever moves it, and its cost is not a property of the attribute but of
what is being duplicated: a SipHash body at three call sites is 1740 bytes, a `flags` walk
is 92, and a `scl::any` cast is -12, because inlining it lets the shared symbol go.

Every other attribute on this page is free in code size, on every group.

## Method

Every variant is compared against a baseline built from the same sources in the same round,
and the rounds run back to back, so a machine that warms up over the hour warms up under
both members of every pair. The baseline runs twice per round, first and last: two runs of
one binary must agree, and what separates them is the floor below which no difference means
anything. Each case runs 15 repetitions of at least 0.5 seconds, pinned to one logical
processor at high priority, and the median of the repetitions is the case's value.

Floors ranged from 0.35% to 6.47% at p90 across the passes on the four pages. Where a pass
came apart - one round on Clang in the flags group, whose two baseline runs differ by 6.2% -
the round is named and dropped rather than averaged in.

### The control

Every group ran at least one variant that annotates something the case under test does not
reach: the nested iterator in `flags`, `algorithm.h` in `hierarchy`, the switch chain in
`any`. Its purpose is to size the layout effect, and on these suites that effect is large:
a `flags` case that builds no iterator moved by -6.4% under three variants including the one
that touches only the iterator, and a `hierarchy` case moved -1.95% under a header it never
enters. Any figure under about two per cent on these pages is read against that.

## Trading speed for size

The only attribute with a size cost is the one that duplicates code, and the two groups that
apply it - `flags` and `hierarchy` - cost 92 bytes and nothing. A build that wants those
bytes back, or one that inlines a walk at more call sites than its instruction cache holds,
defines the macro empty before including the header:

```cpp
#define SCL_FORCE_INLINE
#include <scl/utility/flags.h>
```

Every attribute macro in `scl::utility::attribute` is declared inside an outer `#ifndef`, so
a definition that arrives first wins. That governs the macros the library uses; it cannot
remove a standard attribute spelled directly, which is how both applied branch hints are
written - and those cost nothing in code size anyway.

## See also

- [hash](../hash/benchmark.md), [flags](../flags/benchmark.md),
  [hierarchy](../hierarchy/benchmark.md), [any](../any/benchmark.md) - the group matrices
- [Russian documentation](../../ru/attribute/benchmark.md)
