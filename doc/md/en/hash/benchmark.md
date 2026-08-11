# Hash benchmarks

- Timing: `benchmark/hash/`, target `utility_hash_gbench`
- Code size: `benchmark/hash/hash_size.cpp`, target `utility_hash_size`
- Method, environment and the cross-group summary:
  [measuring the optimisation attributes](../attribute/benchmark.md)

## Scope

`SCL_FORCE_INLINE`, `SCL_HOT` and `SCL_UNLIKELY` on the five public hash functions, their
hashers and the two helpers in `hash/detail/base.h`, each measured against the same code
without it. Input is 63 bytes, as `std::string_view`, `std::string` and
`std::array<char, 63>`.

## Timing

Mean of 20 repetitions. The between-run spread on the right is the range the same binary
covers across cells whose patch touched another group; a difference has to leave it to
count.

### GCC 16.0.1, `-O3`

| case | baseline | `FORCE_INLINE` | `HOT` | `UNLIKELY` | spread |
|---|---|---|---|---|---|
| `djb2_string_view` | 40.9 | 41.1 | 42.3 | 41.0 | 40.8-41.7 |
| `fnv1a_string_view` | 40.6 | 41.0 | 41.9 | 41.0 | 40.6-42.3 |
| `fnv1a_char_array` | 40.8 | 41.1 | 42.2 | 41.1 | 40.8-41.7 |
| `siphash_string_view` | 52.3 | 53.6 | 53.1 | 52.0 | 51.9-52.9 |

Nothing leaves the spread. GCC gains nothing from any of the three.

### Clang 22.1.8, `-O3`

| case | baseline | `FORCE_INLINE` | `HOT` | `UNLIKELY` | spread |
|---|---|---|---|---|---|
| `djb2_string_view` | 37.1 | 37.2 | 37.2 | 37.7 | 36.9-38.9 |
| `fnv1a_string_view` | 37.6 | 37.6 | 37.6 | 37.6 | 37.5-38.2 |
| `fnv1a_char_array` | 36.4 | **34.3** | 36.2 | 36.3 | 36.0-36.4 |
| `siphash_string_view` | 64.0 | **59.9** | **60.1** | **60.1** | 63.1-64.6 |

### MSVC 19.44, Release

| case | baseline | `FORCE_INLINE` | `UNLIKELY` | spread |
|---|---|---|---|---|
| `djb2_string_view` | 39.6 | 38.6 | 38.8 | 38.8-40.2 |
| `fnv1a_string_view` | 42.4 | 41.5 | 41.3 | 41.1-42.4 |
| `fnv1a_char_array` | 46.2 | 45.0 | 44.9 | 44.5-46.2 |
| `siphash_string_view` | 71.7 | **64.7** | **58.8** | 70.9-71.8 |

`SCL_HOT` is empty on MSVC and is not measured there.

### Confirmation

The three `siphash` figures were re-measured by interleaving the saved binaries, which
separates an effect belonging to the binary from one belonging to the state of the host.
Four alternations under Clang put the baseline at 64.1 to 65.2 ns and `SCL_HOT` at 60.6 to
61.5, with no overlap. Four alternations under MSVC put the baseline at 72.1 to 72.5,
`SCL_UNLIKELY` at 59.6 to 61.4 and `SCL_FORCE_INLINE` at 65.8 to 66.3; `fnv1a_string_view`
measured in the same rounds stayed at 41.6 to 42.3 throughout, so the host was steady.

A control applied each attribute to the four other hash functions and left `siphash`
alone. `siphash` then returned to baseline, which places the cause in its own annotation
rather than in the layout of the code around it.

`SCL_HOT` and `SCL_UNLIKELY` were also measured together, since each moves the same case
on its own. Four alternations under Clang put the hint alone at 62.5 to 63.1 ns and the
pair at 62.2 to 62.5, while `fnv1a_string_view` measured in the same rounds spanned 39.6
to 40.2. The 0.5% between them is inside the control's own spread: the two attributes
reach the same decision rather than adding to each other. Under MSVC the question does
not arise, as `SCL_HOT` is empty there, and under GCC neither moves anything alone.

Note for anyone repeating this: `SCL_HOT` has to precede `SCL_FORCE_INLINE`. The latter
ends in the `inline` specifier, and an attribute list cannot follow it.

## Code size

`.text` of `libutility_hash_size.a` at `-Os`, fifteen wrappers over the five functions:

| variant | `.text` | vs baseline |
|---|---|---|
| no `SCL_FORCE_INLINE` | 1950 | |
| on `detail::sip_round` only | 3926 | +101% |
| on the whole public surface | 3690 | +89% |

SipHash-2-4 calls `sip_round` six times per hash, which is why forcing that one helper
outweighs forcing everything.

At `-O3` the annotation costs nothing: defining `SCL_FORCE_INLINE=inline` there produces a
byte-identical object file, because the optimiser inlines the same bodies on its own. The
89% belongs to `-Os`, where it does not.

### What that figure is, and is not

It is fifteen instantiations - five functions over three input types - each reachable from
outside, so nothing is discarded. It measures how the annotation scales with the number of
call sites, not what the library costs in a firmware image.

A single call site pays nothing. One wrapper around `fnv1a` over `std::string_view`, same
flags, compiles to 60 bytes of `.text` either way: at `-Os` the optimiser already inlines a
small body called once.

The growth comes from repetition. Without the annotation the compiler keeps one out-of-line
body and lets several wrappers call it; with it every wrapper gets its own copy. A consumer
pays in proportion to how many distinct instantiations it calls, and `--gc-sections` drops
the ones it does not.

The cost of duplication is measurable in time as well. `benchmark/hash/icache_gbench.cpp`
runs the same hash from 1024 distinct call sites and from one, for equal work: the many-site
form is 20% slower under GCC and 14% slower under Clang. That is the ceiling the annotation
approaches on a target whose instruction cache the code no longer fits.

## Verdict

Two of the four are worth applying, by the criterion in
[whether each attribute is worth applying](../attribute/benchmark.md#whether-each-attribute-is-worth-applying):
a gain on at least one compiler and a loss on none.

| Attribute | Applied | Reason |
|---|---|---|
| `SCL_FORCE_INLINE` | **yes** | 5.8% on `fnv1a` over an array under Clang, 9% on `siphash` under MSVC, nothing under GCC. Its cost is `.text` at `-Os`, which a size-constrained build defines away |
| `SCL_UNLIKELY` | **yes** | 16.5% under MSVC and 6.1% under Clang on `siphash`, nothing under GCC, no cost in size |
| `SCL_HOT` | no | 5.5% on `siphash` under Clang alone, on the case `SCL_UNLIKELY` already improves by 6.1%. Added on top of that hint it buys 0.5%, inside the spread of the control case measured in the same rounds, so the two reach the same decision rather than adding up |
| `SCL_ASSUME` | no | no site: the only invariant available, `shift < 64` after the loop, is about a variable nothing reads afterwards |

`SCL_FORCE_INLINE` covers the five functions, their hashers and `detail::sip_round`;
`SCL_UNLIKELY` covers the block-completion branch of `siphash`.

A build that cannot afford the size defines `SCL_FORCE_INLINE=inline`, which every
attribute macro supports through its own `#ifndef` - see
[inline attributes](../attribute/inline.md).
