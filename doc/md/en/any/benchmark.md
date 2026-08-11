# any benchmarks

- Timing: `benchmark/any/`, target `utility_any_gbench`
- Method, environment and the cross-group summary:
  [measuring the optimisation attributes](../attribute/benchmark.md)

## Scope

`SCL_HOT` on the three `any_cast` overloads, and `SCL_UNLIKELY` / `SCL_LIKELY` on the
branch chain inside them, measured against the same code without them. These annotations
predate the measurement suite: this page exists because the library carried them on no
evidence.

There is no code-size suite yet. `any/` reaches `std::any` and `typeid`, and its
reference-returning casts throw, so it does not build under the bare-metal preset. RTTI is
already optional through `SCL_HAS_RTTI`; exceptions are not. Once `SCL_HAS_EXCEPTIONS`
gates the three throwing overloads the group compiles for that target, and the suite is
added here to match the other groups.

## Timing

Mean of 10 repetitions, in nanoseconds. `A` carries the annotations, `B` has them
stripped.

| compiler | | `cast_hit` | `cast_miss` | `cast_empty` | `cast_boxed` | `arg_hit` | `arg_miss` |
|---|---|---|---|---|---|---|---|
| Clang 22 | A | 2.02 | 2.26 | 2.01 | 1.77 | 2.02 | 2.02 |
| Clang 22 | B | 1.76 | 2.01 | 1.88 | 2.01 | 2.01 | 2.01 |
| GCC 16 | A | 1.27 | 1.27 | 0.532 | 1.01 | 0.251 | 0.247 |
| GCC 16 | B | 1.26 | 1.26 | 0.536 | 1.00 | 0.255 | 0.249 |
| MSVC 19.44 | A | 5.68 | 6.50 | 2.92 | 5.57 | 5.31 | 5.23 |
| MSVC 19.44 | B | 5.65 | 6.40 | 2.93 | 5.57 | 5.30 | 5.04 |

The chain `any_switch` builds, over four branches:

| compiler | first branch | last branch | fallback |
|---|---|---|---|
| Clang 22 | 2.02 | 5.82 | 6.09 |
| GCC 16 | 0.742 | 0.745 | 0.749 |
| MSVC 19.44 | 9.73 | 17.0 | 17.1 |

A chain runs `any_cast` once per branch until one accepts, so its cost is linear in how
far down the match sits. The annotations do not move any of the three figures. Under GCC
the three are equal and under a nanosecond, which means the chain folds against a subject
the optimiser can see through; those rows measure the harness.

## What the numbers say

**The suite cannot resolve these annotations.** GCC and MSVC show nothing outside the
variation the same binary produces between runs - under MSVC that variation is 6% on
`cast_hit`, wider than any difference between `A` and `B`.

Clang shows differences of up to 14%, and they are not the annotations. An earlier run of
this suite, before the three `any_switch` cases were added to the same translation unit,
put `A` ahead on `cast_miss`, `arg_hit` and `arg_miss` and behind on `cast_empty` and
`cast_boxed`. Adding those cases changed no line of `any_cast`, and the signs reversed.
Each run is self-consistent across its rounds, so each is a property of the binary it
measured rather than of the annotation under test.

The scale explains it. These cases run in about 2 ns and the differences are about
0.25 ns, which is what moving code across a cache line costs. The `hash` cases run in 40
to 70 ns, where an effect of the same relative size is 2 to 10 ns and survives the
isolation control; here nothing does.

## Verdict

| Attribute | Applied | Reason |
|---|---|---|
| `SCL_HOT` | yes, unmeasured | the suite cannot resolve it on any of the three compilers |
| `SCL_UNLIKELY` on the empty branch | yes, unmeasured | as above |
| `SCL_LIKELY` on the type-match branch | yes, unmeasured | as above |

This is the one place in the library where an optimisation attribute rests on the shape of
the code rather than on a figure: a view without a descriptor is the rare case and a
matching type key is the common one, which is what the hints say. Removing them is equally
defensible, since nothing measured argues for keeping them. What is not defensible is
citing a performance figure for them, and none is cited.

Resolving them needs a method this suite does not have - a harness that pins code layout,
or a workload where `any_cast` runs often enough for the difference to accumulate beyond
one cache line's worth of placement.
