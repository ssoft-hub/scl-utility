# Hierarchy Benchmarks

- Suite: `benchmark/hierarchy/`
- Configure: `cmake --preset <preset> -DSCL_BUILD_BENCHMARKS=ON`
- Timing target: `utility_hierarchy_gbench`
- Size target: `utility_hierarchy_size`

## What is applied

`SCL_FORCE_INLINE` on the range interface of `scl::hierarchy::node` and of
`scl::hierarchy::tree` and on the members of the proxy a tree iterator hands out - 12 sites
in `node.h` and 50 in `tree.h`. Nothing else, and nothing at all in `algorithm.h`.

| Case | Clang | GCC | MSVC |
|---|---|---|---|
| `tree_walk_children` | noise | **-27.3%** | noise |
| `hierarchy_walk_children` | noise | noise | noise |
| `tree_walk_recursive` | noise | noise | noise |
| `hierarchy_is_ancestor_of` | noise | noise | noise |
| `hierarchy_is_ancestor_of_deep` | noise | noise | noise |
| `tree_push_back` | noise | noise | noise |

One case on one compiler. A tree reaches the node through its proxy and GCC leaves the
members a call there, so a walk over a tree's children drops from 2.55 ns to 1.85 - what the
same walk costs over a bare node. `.text` is unchanged at 422 bytes, so there is nothing to
trade against it.

## Method

Every variant is compared against a baseline built from the same sources in the same round.
The baseline runs twice per round, first and last, and the difference between those two runs
is the floor below which nothing in these tables means anything. Each case runs 15
repetitions of at least 0.5 seconds, pinned to one logical processor at high priority; the
median of the repetitions is the case's value. The floor of the pass behind the table above
is 2.61% at p90 on Clang, 4.39% on GCC and 4.10% on MSVC; across the five passes on this page
it ranges from 1.40% to 6.47%.

The tree is eight children wide and four levels deep, 4681 nodes: wide enough that a walk
over one level costs more than the call reaching it, deep enough that the ancestor query has
something to climb. Every case runs through `node` and again through `tree`.

The group is three headers, and a variant covers one at a time. For a case that never enters
a given header, that header's variant is a control - `algorithm.h` is on no walk, and the
containers are on no ancestor query.

## What the base costs

| Case | Clang 22.1.8 | GCC 13.1.0 | MSVC 19.44 |
|---|---|---|---|
| `hierarchy_is_ancestor_of` | 0.525 | 0.392 | 2.846 |
| `hierarchy_is_ancestor_of_deep` | 1.993 | 1.165 | 5.781 |
| `hierarchy_walk_children` | 5.257 | 1.851 | 9.890 |
| `tree_walk_children` | 6.752 | 2.512 | 10.230 |
| `hierarchy_emplace_back` | 80.215 | 42.787 | 80.494 |
| `tree_push_back` | 80.734 | 42.498 | 81.830 |
| `hierarchy_walk_recursive` | 20010 | 7319 | 19635 |
| `tree_walk_recursive` | 24952 | 7501 | 24572 |

Insertion costs the same through the node and through the tree, because the allocation
dominates and the observer notification is a call to an empty function. Reading through the
tree is not free: a walk over the whole tree costs a quarter more on Clang and MSVC, and the
proxy is what separates them.

## Force inlining, and how far it has to reach

`SCL_FORCE_INLINE` per header and narrowed to the walk, on GCC - no other compiler moved on
any case, under any variant:

| Case | `node.h`, 50 sites | `tree.h`, 134 | the walk, 62 | `algorithm.h` (control) |
|---|---|---|---|---|
| `tree_walk_children` | **-26.5%**, **-27.5%** | **-27.0%** | **-27.4%** | noise |
| `hierarchy_walk_children` | noise | noise | noise | noise |
| `tree_walk_recursive` | noise | noise | noise | noise |
| `tree_push_back` | noise | noise | noise | noise |

Either side of the proxy boundary carries the whole gain, and the walk carries it at a third
of the sites. Both sides are annotated rather than the cheaper one: with only one annotated,
the call goes away just as long as the compiler keeps inlining the other, and no byte of code
size is paid for removing that dependency.

The control is what makes the column readable. `algorithm.h` is on no walk, yet annotating it
moved `hierarchy_walk_recursive` by -1.95% in the first pass - the size of the layout effect
on this suite, and what everything under two per cent here has to be read against.

## Branch hints - measured, not applied

`is_ancestor_of` climbs the parent chain, and each step asks two questions whose answer is no
until the last. Marked `[[unlikely]]`, the first pass put Clang at -15.0% on
`hierarchy_is_ancestor_of_deep`, spread -26.3% to -8.6%. Two later passes - the hints alone,
and the shipped configuration - read noise on that case on all three compilers. One reading
out of three does not carry an annotation, so the hints are not applied.

## `SCL_HOT` - measured, nothing

Every member of all three headers, every case, all three compilers: nothing above the floor.
The one cell that cleared it, `tree_push_back` at +1.88% on MSVC, has a spread crossing zero.

## Code size

`.text` of `libutility_hierarchy_size.a`, `arm-none-eabi-g++` 13.2.1, Cortex-M4, MinSizeRel,
summed over sections: **422 bytes under every variant**, base included - the whole of
`node.h`, the whole of `tree.h`, the walk, `algorithm.h`, `SCL_HOT`, the branch hints.

At `-Os` the cross compiler had already inlined everything the annotation asks for. What is
left in the library is the wrappers themselves, `std::list::emplace`, `_M_clear` and the
tree's two recursive notifiers.

### One variant that does not compile

`SCL_FORCE_INLINE` on every member of `tree.h` is not a trade-off on GCC, it is an error:

    error: inlining failed in call to 'always_inline'
    'tree<...>::notify_on_insert(iterator)': recursive inlining

`notify_on_insert` and `notify_on_erase` walk a subtree notifying the observer, so they call
themselves, and GCC rejects `always_inline` on a directly recursive function outright. Clang
and MSVC accept it. Every variant on this page leaves those two members alone.

## Trading speed for size

There is nothing to trade: `.text` is 422 bytes either way. A project that wants the walk left
alone can define `SCL_FORCE_INLINE` empty before including the header, since every attribute
macro in `scl::utility::attribute` is declared inside an outer `#ifndef`.

```cpp
#define SCL_FORCE_INLINE
#include <scl/utility/hierarchy/tree.h>
```

## See also

- [node](node.md), [tree](tree.md), [algorithm](algorithm.md) - the group itself
- [Russian documentation](../../ru/hierarchy/benchmark.md)
