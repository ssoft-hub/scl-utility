# flags benchmarks

- Timing: `benchmark/flags/`, target `utility_flags_gbench`
- Code size: `benchmark/flags/flags_size.cpp`, target `utility_flags_size`
- Method, environment and the cross-group summary:
  [measuring the optimisation attributes](../attribute/benchmark.md)

## Scope

`SCL_FORCE_INLINE`, `SCL_HOT` and `SCL_LIKELY_EXPR` on the sparse walk behind the iterator,
each measured against the same code without it. The annotated surface is the whole hot
path: `has_bit`, `next_set`, `prev_set`, `size`, `begin`, `end`, and the iterator's
constructor, dereference, increment and decrement.

The input is `flags<bit>` over 32 bits with every fourth bit set - a spread the branch
predictor cannot memorise, and eight stops of the walk rather than a degenerate one.

## Timing

Mean of 20 repetitions. The between-run spread on the right is the range the same binary
covers across cells whose patch touched another group.

| compiler | case | baseline | `FORCE_INLINE` | `HOT` | `LIKELY_EXPR` | spread |
|---|---|---|---|---|---|---|
| GCC 16 | `flags_iterate` | 25.8 | 25.6 | 25.7 | 26.2 | 25.6-26.1 |
| GCC 16 | `flags_all` | 1.67 | 1.67 | 1.67 | **1.79** | 1.67-1.72 |
| Clang 22 | `flags_iterate` | 20.2 | **28.9** | 20.4 | 20.2 | 20.0-20.6 |
| Clang 22 | `flags_all` | 0.319 | 0.328 | 0.320 | 0.321 | 0.317-0.331 |
| MSVC 19.44 | `flags_iterate` | 29.1 | 29.1 | - | 29.1 | 29.1-29.4 |
| MSVC 19.44 | `flags_all` | 4.01 | 3.90 | - | 3.85 | 3.95-4.01 |

`SCL_HOT` and `SCL_LIKELY_EXPR` are defined away on MSVC. `flags_or`, `flags_and`,
`flags_complement`, `flags_any` and `flags_any_of` land at or under two nanoseconds because
the optimiser folds them against a compile-time input; they measure the harness rather than
the operation and are left out. Only `flags_iterate` does real work.

## Code size

`.text` of `libutility_flags_size.a` at `-Os`, eight wrappers:

| variant | `.text` |
|---|---|
| without | 452 |
| with `SCL_FORCE_INLINE` | 480 |

Per symbol, the 28 bytes are where the annotation puts them. Without it `has_bit` stays a
single 30-byte weak function that `flags_iterate` and `flags_all` both call, at 58 and 26
bytes each. With it `has_bit` disappears as a symbol and its callers grow to 90 and 52. The
growth is duplication across call sites, as in `hash`, but there are three call sites here
rather than fifteen.

## What the numbers say

Forcing the inline of the walk makes it **43% slower under Clang** - 20.2 to 28.9 ns,
against a spread near 3%. Neither GCC nor MSVC shows it. Inlining `has_bit` into the loop
body of `next_set` costs Clang a transformation it makes when the call is left out of line,
where it has a small self-contained loop to work on.

`SCL_LIKELY_EXPR` on the loop conditions costs GCC 7% on `flags_all`, which is 1.67 ns to
begin with, and moves nothing under Clang. The hint says the loop condition is usually
true; the loop does terminate, and biasing the layout toward the body does not pay.

`SCL_HOT` moves nothing on either compiler that implements it.

## Verdict

None of the three is worth applying, by the criterion in
[whether each attribute is worth applying](../attribute/benchmark.md#whether-each-attribute-is-worth-applying):
each has to gain on at least one compiler and lose on none.

| Attribute | Applied | Reason |
|---|---|---|
| `SCL_FORCE_INLINE` | no | costs 43% under Clang on the only case that does real work, and gains on no compiler. Also costs 28 bytes of `.text` at `-Os` |
| `SCL_LIKELY_EXPR` | no | costs 7% under GCC on `flags_all` and gains on no compiler |
| `SCL_HOT` | no | no effect under GCC or Clang, and defined away on MSVC |

`flags` therefore carries no optimisation macro: the one case where an annotation would
matter is the case each of them makes worse or leaves alone.

The walk itself is worth changing. `next_set` tests one bit at a time where the underlying
storage is bytes, and `std::countr_zero` over a byte would replace up to eight tests with
one instruction. That is a change to the algorithm rather than to its annotations, and it
is why iteration is the one expensive operation on `flags` while every other lands at two
nanoseconds or less.
