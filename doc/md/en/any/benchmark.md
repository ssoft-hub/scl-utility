# Any Benchmarks

- Suite: `benchmark/any/`
- Configure: `cmake --preset <preset> -DSCL_BUILD_BENCHMARKS=ON`
- Timing target: `utility_any_gbench`
- Size target: `utility_any_size`

## What is applied

Nothing. This is the group that arrived already annotated - `SCL_HOT` on the `any_cast` entry
point, `[[unlikely]]` on the two null checks, `[[likely]]` on the successful return, and
`SCL_UNLIKELY_EXPR` on the two self-assignment guards in `any.h` - and none of the six
survived being measured. All are removed.

Removing them, over four paired rounds:

| Case | Clang | GCC | MSVC |
|---|---|---|---|
| `any_view_cast_empty` | **-9.68%** | **+20.24%** | noise |
| `any_view_cast_miss` | noise | **-7.19%** | noise |
| `any_view_cast_boxed` | **-5.71%** | noise | noise |
| `any_switch_last_branch` | **-3.36%** | noise | noise |
| `any_assign_self` | **-31.83%** | noise | +1.15% |
| `any_assign_view` | **-8.53%** | noise | noise |
| `any_assign_string` | **+3.99%** | noise | noise |
| `any_assign_value` | **+9.15%** | noise | noise |

Five cases better, three worse, no compiler agreeing with another. That is the shape every
one of the six annotations had on its own, which is why none is kept: an annotation that
moves one case down and the next one up by the same amount is not buying anything, and
leaving it in the source claims otherwise.

## Method

Every variant is compared against a baseline built from the same sources in the same round.
Since the group ships annotated, `base` is the library as it was and a `no_*` variant is the
same sources with one annotation taken away. The baseline runs twice per round, first and
last, and the difference between those two runs is the floor below which nothing in these
tables means anything. Each case runs 15 repetitions of at least 0.5 seconds, pinned to one
logical processor at high priority; the median of the repetitions is the case's value.

Every branch a hint marks has a case on each side of it: a cast that hits, one that misses,
one on an empty handle, one that reaches into a boxed `std::any`, an assignment where the
self-assignment guard answers no and one where it answers yes.

| Null channel | median | p90 | worst |
|---|---|---|---|
| Clang, first pass | -0.29% | 3.15% | 5.97% |
| GCC, first pass | -0.22% | 4.31% | 7.06% |
| MSVC, first pass | -0.05% | 0.57% | 1.86% |
| Clang, second pass | -0.06% | 0.35% | 0.40% |
| GCC, second pass | -0.01% | 0.83% | 5.33% |
| MSVC, second pass | -0.07% | 0.42% | 1.09% |

## What the base costs

| Case | Clang 22.1.8 | GCC 13.1.0 | MSVC 19.44 |
|---|---|---|---|
| `any_arg_cast_hit` | 1.641 | 0.218 | 5.969 |
| `any_arg_cast_miss` | 2.316 | 0.218 | 6.642 |
| `any_view_cast_hit` | 1.616 | 0.668 | 2.785 |
| `any_view_cast_miss` | 2.405 | 1.106 | 2.915 |
| `any_view_cast_empty` | 1.369 | 0.463 | 2.215 |
| `any_view_cast_boxed` | 3.828 | 1.338 | 5.046 |
| `any_switch_first_branch` | 1.740 | 0.873 | 8.868 |
| `any_switch_last_branch` | 6.235 | 0.871 | 11.745 |
| `any_assign_self` | 0.664 | 0.444 | 4.612 |
| `any_assign_value` | 2.274 | 4.733 | 9.253 |
| `any_assign_string` | 5.258 | 6.487 | 8.551 |
| `any_assign_view` | 12.578 | 10.137 | 12.137 |

A cast is a handful of instructions on GCC - 0.218 ns is the barrier and nothing else - and
between two and seven nanoseconds on the other two. The self-assignment guard is what makes
`any_assign_self` the cheapest case on the page: it answers yes and the whole body is
skipped, 0.664 ns against 2.274 for the assignment that goes through.

## What each annotation did

Every figure is the effect of taking that one annotation away, and every cell not listed
read noise on all three compilers.

| Annotation | Sites | Effect of removal |
|---|---|---|
| `SCL_HOT` on `any_cast` | 1 | Clang: `any_arg_cast_miss` **-9.13%**, `any_view_cast_miss` **-13.98%**, `any_view_cast_empty` **+14.95%** |
| `[[unlikely]]` on the null checks | 2 | nothing, on any case, on any compiler |
| `[[likely]]` on the successful return | 1 | GCC: `any_view_cast_miss` **-8.90%**, `any_view_cast_empty` **+9.93%** |
| `SCL_UNLIKELY_EXPR` on the guards | 2 | Clang: `any_assign_self` **-31.73%**, `any_assign_view` **-8.59%**, `any_assign_value` **+9.22%**, `any_assign_string` **+4.40%** |

`SCL_HOT` was making the two miss paths 10 to 16 per cent slower and the empty one 13 per
cent faster. The `[[likely]]` on the return was doing the same thing in miniature on GCC.
The guards' hint claims the incoming object is usually not the one already held, which is
true in three of the four assignment cases - and it still made two of them slower. Only the
null checks are unambiguous: they do nothing at all, on any case.

`.text` under `arm-none-eabi-g++` at `MinSizeRel` is 252 bytes with the annotations and 252
without, so nothing on this page is a size trade.

## The `std::any` branch inside `any_cast`

`any_cast` ends with a branch that reaches into a value boxed in a `std::any`. Everything
else about the `std::any` interop is resolved at compile time; this is the only part of it
that costs anything at run time, and it costs a great deal:

| Case | Clang | GCC | MSVC |
|---|---|---|---|
| `any_arg_cast_hit` | **-86.60%** | noise | **-55.94%** |
| `any_view_cast_hit` | **-65.21%** | noise | -1.40% |
| `any_view_cast_miss` | **-67.63%** | **-19.67%** | **-15.58%** |
| `any_view_cast_empty` | **-74.26%** | noise | **-9.94%** |
| `any_switch_first_branch` | **-8.83%** | noise | **-2.60%** |

Those are the numbers with the branch deleted. It is not the branch executing that costs
this: a cast that never sees a `std::any` pays anyway, because the branch pulls a
`type_info` comparison and a terminate path into the function. A translation unit doing
nothing but one cast emits 70 instructions with the branch and 26 without it, and two calls
into the runtime become none.

Two ways to keep the capability were measured beside deleting it. Forcing the entry points
inline recovers most of the gain - Clang `any_arg_cast_hit` -86.62%, MSVC -50.03% - but
costs Clang **+32.40%** on the first branch of a switch chain and MSVC +10.02% on an empty
handle. Taking the branch's body out of line into a `SCL_NOINLINE` helper helps Clang, costs
MSVC between 3 and 13 per cent, and makes the boxed cast itself **+56.91%** dearer on GCC.

Deleting the branch is the only one of the three with no regression anywhere, and it is the
only one that removes a capability: casting through a handle over a `std::any` is what
`any_interop_gtest.cpp` covers in 34 tests and what six pages of this group document. The
decision is not this page's to make; the measurement is here so it can be made with numbers.

## Trading speed for size

Nothing to trade in either direction. The group carries no annotation now, and `.text` never
moved under any variant measured.

## See also

- [any](any.md), [any_view](any_view.md), [any_arg](any_arg.md),
  [any_mutable_view](any_mutable_view.md), [any_switch](any_switch.md) - the group itself
- [Russian documentation](../../ru/any/benchmark.md)
