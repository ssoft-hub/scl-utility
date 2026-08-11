# hierarchy benchmarks

- Timing: `benchmark/hierarchy/`, target `utility_hierarchy_gbench`
- Code size: `benchmark/hierarchy/node_size.cpp`, target `utility_hierarchy_size`
- Method, environment and the cross-group summary:
  [measuring the optimisation attributes](../attribute/benchmark.md)

## Scope

`SCL_FORCE_INLINE`, `SCL_HOT` and the branch hints on the iteration and query surface, each
measured against the same code without it: `begin`, `end`, `cbegin` and `cend` on `node`,
and `parent_of`, `has_parent`, `are_identical`, `is_parent_of` and `is_ancestor_of` in
`hierarchy/algorithm.h`. The hints mark the two branches of `is_ancestor_of` - the root
check as unlikely, the direct-parent hit as likely.

The tree is 8 children wide and 4 levels deep, 4681 nodes.

## Timing

Mean of 20 repetitions. The between-run spread on the right is the range the same binary
covers across cells whose patch touched another group.

| compiler | case | baseline | `FORCE_INLINE` | `HOT` | `LIKELY` | spread |
|---|---|---|---|---|---|---|
| GCC 16 | `walk_children` | 1.97 | 1.95 | 1.95 | 1.95 | 1.95-1.97 |
| GCC 16 | `is_ancestor_of` | 0.417 | 0.413 | 0.414 | 0.413 | 0.415-0.433 |
| Clang 22 | `walk_children` | 4.31 | 4.29 | 4.28 | 4.28 | 4.28-4.31 |
| Clang 22 | `is_ancestor_of` | 0.555 | 0.552 | 0.552 | **0.711** | 0.553-0.559 |
| MSVC 19.44 | `walk_children` | 10.0 | 10.0 | - | 9.93 | 9.81-10.0 |
| MSVC 19.44 | `is_ancestor_of` | 2.45 | 2.46 | - | 2.54 | 2.41-2.46 |

`SCL_HOT` is defined away on MSVC. The MSVC hint cell reads high on every case including
the ones its patch cannot reach - `djb2_string_view` from an untouched binary reads 40.2
against 38.6 to 39.0 elsewhere - so its `is_ancestor_of` figure is part of that drift, not
an effect.

`hierarchy_walk_recursive` is excluded. It keeps a coefficient of variation of 5.7 to 7.4%
at 20 repetitions and moved by 14% between two cells built from identical sources; it
cannot resolve a difference at this scale. `hierarchy_build` allocates a node per element
and measures the allocator.

## Code size

`.text` of `libutility_hierarchy_size.a` at `-Os`, four wrappers:

| variant | `.text` |
|---|---|
| without | 84 |
| with `SCL_FORCE_INLINE` | 84 |

Identical to the byte, and the per-symbol figures say why: `walk_children` is 20 bytes,
`walk_recursive` 32, `ancestor_of` and `parent_of` 16 each. The operations are pointer
chasing over a `std::list`, so there is no body large enough for the annotation to
duplicate.

## What the numbers say

Neither `SCL_FORCE_INLINE` nor `SCL_HOT` moves anything on any of the three compilers, and
code size is identical. `node` delegates iteration to `std::list`, so `begin`, `end` and
their const forms are one-line forwarders that the optimiser already inlines.

The branch hints cost **28% under Clang** on `is_ancestor_of` - 0.555 to 0.711 ns against a
spread under 1% - and move nothing under GCC or MSVC. Both hints point the correct way for
this input, since the argument is a direct child of the root, and Clang still loses by
them.

## Verdict

None of the three is worth applying, by the criterion in
[whether each attribute is worth applying](../attribute/benchmark.md#whether-each-attribute-is-worth-applying):
each has to gain on at least one compiler and lose on none.

| Attribute | Applied | Reason |
|---|---|---|
| `SCL_LIKELY` / `SCL_UNLIKELY` | no | costs 28% under Clang on `is_ancestor_of` and gains on no compiler, although both hints point the correct way for this input |
| `SCL_FORCE_INLINE` | no | no effect on any of the three compilers, and `.text` identical to the byte |
| `SCL_HOT` | no | no effect under GCC or Clang, and defined away on MSVC |

`hierarchy` therefore carries no optimisation macro: the surface small enough to annotate
is the surface the compiler already handles, and the one annotation with an effect has a
negative one.
