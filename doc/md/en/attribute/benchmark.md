# Measuring the optimisation attributes

How the effect of `SCL_FORCE_INLINE`, `SCL_HOT` and the branch hints is measured, and what
the measurements found across every annotated group. Per-group detail lives in
[hash](../hash/benchmark.md), [flags](../flags/benchmark.md),
[hierarchy](../hierarchy/benchmark.md) and [any](../any/benchmark.md).

## Environment

| | |
|---|---|
| Timing | GCC 16.0.1 and Clang 22.1.8, x86-64 Linux, Release (`-O3`), 16 CPUs at 2304 MHz |
| | MSVC 19.44.35228 (Visual Studio 2022), x64 Windows, Release |
| Code size | GCC 13.2.1, `arm-none-eabi`, Cortex-M4, `-Os` (`MinSizeRel`) |
| Repetitions | 20, aggregates only |

## Reproducing

```sh
script/ci/build.sh clang-x64 Release -DSCL_BUILD_BENCHMARKS=ON -DSCL_BUILD_TESTS=OFF
SCL_BENCHMARK_TAG=baseline SCL_BENCHMARK_REPETITIONS=20 script/ci/run_benchmarks.sh clang-x64

cmake --preset arm-none-eabi -DSCL_BUILD_BENCHMARKS=ON && cmake --build --preset arm-none-eabi
script/ci/run_size.sh arm-none-eabi
```

Each run is written as JSON under `build/<preset>/benchmark-results/`; `SCL_BENCHMARK_TAG`
names it, so a pair survives a branch switch. `3rdparty/benchmark/tools/compare.py` reads
two of them and adds a significance test, given `numpy` and `scipy`.

## Method

An attribute is measured against the same code without it, one attribute and one group at
a time. The baseline carries no optimisation attribute anywhere, so a group under test
starts from the same state as the groups that are not.

Four rules decide whether a difference counts.

**The spread within a run is not the threshold.** Every cell of the matrix runs all three
groups, although its patch touches one. A patch to `flags` leaves the `hash` binary
untouched, so those cells measure the same executable repeatedly and show what the host
alone contributes. That spread is several times the coefficient of variation Google
Benchmark reports inside one run: `fnv1a_string_view` under GCC reads 40.6 to 42.3 ns
across seven cells built from identical sources, while the coefficient of variation within
each of them stays under 1%. A difference smaller than the between-run spread is not a
result.

**A surviving difference is re-tested by interleaving.** Both binaries are kept and
measured in turn, several times over. A difference that belongs to the binary reproduces
in every alternation; one that belongs to the state of the machine does not.

**A surviving difference is isolated by a control.** The attribute is applied to every
function in the group except the one whose case moved. If the case still moves, the cause
is elsewhere - code layout, or another function in the same binary.

**Cases too noisy to resolve are named rather than reported.** `hierarchy_walk_recursive`
keeps a coefficient of variation of 5.7 to 7.4% at 20 repetitions and moved by 14% between
two cells built from identical sources. It cannot support a conclusion at the scale these
attributes work at. Cases that land under a nanosecond fold against a compile-time input
and measure the harness rather than the operation.

## What the macros are on each compiler

Two of them are defined away on MSVC, so a measurement there would report the host and
nothing else.

| Macro | GCC, Clang | MSVC |
|---|---|---|
| `SCL_FORCE_INLINE` | `[[gnu::always_inline]] inline` | `__forceinline` |
| `SCL_HOT` | `[[gnu::hot]]` | empty |
| `SCL_LIKELY` / `SCL_UNLIKELY` | `[[likely]]` / `[[unlikely]]` | `[[likely]]` / `[[unlikely]]` |
| `SCL_LIKELY_EXPR` / `SCL_UNLIKELY_EXPR` | `__builtin_expect` | `(!!(expr))` |

## Results

Every entry that is not "no effect" survived interleaving and a control. Percentages are
against the same group's unannotated baseline.

| Group | Attribute | GCC 16 | Clang 22 | MSVC 19.44 |
|---|---|---|---|---|
| hash | `SCL_FORCE_INLINE` | no effect | `fnv1a_char_array` **-5.8%**, `siphash` **-6.4%** | `siphash` **-9%** |
| hash | `SCL_HOT` | no effect | `siphash` **-5.5%** | defined away |
| hash | `SCL_UNLIKELY` | no effect | `siphash` **-6.1%** | `siphash` **-16.5%** |
| flags | `SCL_FORCE_INLINE` | no effect | `flags_iterate` **+43%** | no effect |
| flags | `SCL_HOT` | no effect | no effect | defined away |
| flags | `SCL_LIKELY_EXPR` | `flags_all` **+7%** | no effect | defined away |
| hierarchy | `SCL_FORCE_INLINE` | no effect | no effect | no effect |
| hierarchy | `SCL_HOT` | no effect | no effect | defined away |
| hierarchy | `SCL_LIKELY` / `SCL_UNLIKELY` | no effect | `is_ancestor_of` **+28%** | no effect |
| any | `SCL_HOT`, `SCL_LIKELY` / `SCL_UNLIKELY` | no effect | not resolvable | no effect |

Code size, `.text` of the `*_size` libraries at `-Os`, with `SCL_FORCE_INLINE` against
without:

| Group | without | with |
|---|---|---|
| hash | 1950 | 3690 (+89%) |
| flags | 452 | 480 (+6%) |
| hierarchy | 84 | 84 |

`SCL_ASSUME` is not in the table. No group offers an invariant the optimiser cannot
already derive, so there is no site to measure.

## Reading the results

**No attribute helps every compiler, and three of them cost more than they return on at
least one.** The two largest effects found are regressions: forcing the inline of the
`flags` walk costs 43% under Clang, and hinting the branches of `is_ancestor_of` costs 28%
there. Both are absent on the other two compilers.

**The one branch hint that pays is compiler-specific and large.** `siphash` fills an
eight-byte block once every eight iterations; hinting that branch as unlikely is worth
16.5% under MSVC and 6% under Clang, and nothing under GCC. A hardware branch predictor
handles the pattern without help, so the gain is in how the compiler lays out and
schedules the two paths, not in prediction accuracy.

**`SCL_FORCE_INLINE` is free at `-O3` and expensive at `-Os`.** At `-O3` both GCC and Clang
already inline the annotated bodies, and defining `SCL_FORCE_INLINE=inline` produces
byte-identical code. At `-Os` the compiler inlines reluctantly, the annotation overrides
that, and the hash surface grows by 89%. The cost of the annotation is therefore paid only
by a size-constrained build, which is the build that can define it away.

## Whether each attribute is worth applying

An attribute is applied when it gains on at least one supported compiler and loses on
none. A cost in code size does not veto it, because a build that cannot afford the size
defines the macro away; a cost in speed does, because the default is speed and nothing
recovers it.

A branch hint is judged differently, because it does not make a function faster: it moves
cost from one path to another, so the question is which path a caller takes. When the
operation is small enough that code layout outweighs the move, no run answers that
question at all - see [any benchmarks](../any/benchmark.md), where the sign of the
difference reverses when unrelated code joins the same translation unit.

| Group | Attribute | Applied | Why |
|---|---|---|---|
| hash | `SCL_FORCE_INLINE` | **yes** | gains on Clang and MSVC, free on GCC; the size it costs is at `-Os` and can be defined away |
| hash | `SCL_UNLIKELY` | **yes** | the largest single gain measured, on two compilers, at no cost on the third and none in size |
| hash | `SCL_HOT` | no | its one effect is on the case `SCL_UNLIKELY` already improves more, and adding it on top of that hint buys nothing |
| flags | `SCL_FORCE_INLINE` | no | costs 43% on the only case that does real work, and gains nowhere |
| flags | `SCL_LIKELY_EXPR` | no | costs 7% under GCC and gains nowhere |
| flags | `SCL_HOT` | no | no effect on either compiler that implements it |
| hierarchy | `SCL_LIKELY` / `SCL_UNLIKELY` | no | costs 28% under Clang and gains nowhere |
| hierarchy | `SCL_FORCE_INLINE`, `SCL_HOT` | no | no effect on any compiler, and no change in code size |
| any | `SCL_HOT`, `SCL_LIKELY` / `SCL_UNLIKELY` | yes, unmeasured | the suite cannot resolve them: the cases run in 2 ns and the differences are code layout, which reverses sign when unrelated code joins the same translation unit |
| every group | `SCL_ASSUME` | no | no site: every invariant available is one the optimiser already derives |

Two of the nine rows are a gain, six are a loss or nothing, and one is a duplicate of a
better row. An optimisation attribute is therefore worth applying only where a
measurement says so for that function on that compiler, and never as a policy applied to
a whole surface.

## Trading speed for size

Every attribute macro is guarded by its own `#ifndef`, so a build defines the ones it
cannot afford:

```sh
cmake --preset arm-none-eabi -DCMAKE_CXX_FLAGS=-DSCL_FORCE_INLINE=inline
```

The default is speed. A build that needs the smaller image asks for it, and the figures
above say what it gives up.
