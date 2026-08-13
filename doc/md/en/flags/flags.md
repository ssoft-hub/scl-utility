# Flags

A type-safe bitmask over a scoped enum.

- Header: `#include <scl/utility/flags.h>`

## Overview

`scl::flags<Enum, bit_count>` stores one bit per enumerator: the bit index of a
flag equals the enumerator's underlying value (`One == 0`, `Two == 1`, ...), so
enumerators need no explicit values. Flags combine through bitwise operators
without ever casting to the underlying integer.

The bits live in a `std::array<std::byte>` sized to `bit_count` (default 32), so
every operation is usable in constant evaluation on the C++20 baseline — unlike
`std::bitset`, whose query and mutate members only became `constexpr` in C++23.

Only scoped enumerations are accepted; a non-scoped `enum` is rejected by a
`static_assert`. An enumerator whose ordinal is `>= bit_count` is out of range:
the constructor and `operator[]` throw `std::out_of_range` at runtime and are
ill-formed in constant evaluation. The predicate queries never throw — an
out-of-range ordinal simply reads as not set. Where
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `0` the same call ends the
program through `std::abort()`, since a precondition violation has no value to return
in place of the answer.

## Features

- One bit per enumerator ordinal; no explicit enumerator values required
- `constexpr`-capable throughout (C++20)
- Set algebra `| & ^ -` and compound `|= &= ^= -=` in flags-flags and flags-`Enum` forms
- `operator[]` membership test
- `all_of` / `any_of` / `none_of` predicates in variadic-flag and whole-mask
  (subset / intersection / disjoint) forms
- Whole-mask `any` / `none`
- Bidirectional range over the set flags, ascending by ordinal

## API reference

### Construction

```cpp
enum class permission { read, write, execute, remove };
using permissions = scl::flags<permission>;

permissions none{};                                  // no flags
permissions rw{permission::read, permission::write}; // two flags
```

A braced empty list selects the default (empty) constructor. Passing a value
whose ordinal is `>= bit_count` throws `std::out_of_range`.

The second template parameter sets the storage width; use it when the default of
32 bits is not the right size:

```cpp
using small = scl::flags<permission, 4>; // 4-bit storage
```

### Membership and predicates

```cpp
constexpr permissions p{permission::read, permission::write};

p[permission::read];                                   // true
p.all_of(permission::read, permission::write);         // true (every listed flag)
p.any_of(permission::execute);                         // false (at least one)
p.none_of(permission::remove);                         // true (none)
p.all_of();                                            // true  (empty pack)
p.any_of();                                            // false (empty pack)
```

The same predicates accept another `flags` for set relations:

```cpp
p.all_of(permissions{permission::read});   // subset:       every flag of the argument is set
p.any_of(permissions{permission::write});  // intersection: they share a set flag
p.none_of(permissions{permission::remove}); // disjoint:     they share no set flag
```

Whole-mask queries:

```cpp
p.any();   // at least one flag set
p.none();  // no flag set
```

There is no "every flag is set" query, because the set that counts as *every* flag is the
caller's to name, and `all_of` is what asks about it:

```cpp
constexpr permissions all_permissions{permission::read, permission::write,
    permission::execute, permission::remove};

p.all_of(all_permissions); // false: p holds two of the four
```

### Combination

```cpp
constexpr permissions a{permission::read, permission::write};
constexpr permissions b{permission::write, permission::execute};

a | b;                    // union
a & b;                    // intersection
a ^ b;                    // symmetric difference
a - b;                    // difference: set in a, not in b
a | permission::execute;  // flags-Enum form (flags on the left)

permissions m{permission::read};
m |= permission::write;   // compound assignment (flags and Enum forms)
m -= permission::read;    // and the difference in place
```

There is no complement operator. A complement is taken against a universe, and a `flags`
knows only `capacity` — its storage width, which an enumeration is free to underfill or to
spread its enumerators across. Complementing over the width would hand back ordinals no
enumerator names. Name the universe instead:

```cpp
constexpr permissions all_permissions{permission::read, permission::write,
    permission::execute, permission::remove};

all_permissions - a;      // {execute, remove} — the complement of a within that set
all_permissions ^ a;      // the same, since a is a subset of it
```

### Iteration

`flags` is a bidirectional range whose elements are its *set* flags, as `Enum`
values in ascending ordinal order. `size()` is the number of set flags (the
population count), distinct from the static `capacity` (the bit width).

```cpp
permissions granted{permission::read, permission::execute};

for (permission flag : granted) { /* read, then execute */ }

for (auto it = granted.rbegin(); it != granted.rend(); ++it) { /* execute, then read */ }

granted.size();      // 2  (set flags)
permissions::capacity; // 32 (bit width)
```

Because it models `std::ranges::bidirectional_range` and `std::ranges::sized_range`,
`flags` composes with the `<ranges>` views and algorithms.

## See also

- [`example/flags/common/flags_common_example.cpp`](../../../../example/flags/common/flags_common_example.cpp) —
  runnable version: combining and querying flags, set algebra between two masks,
  and iterating the set flags in both directions.
- [Russian documentation](../../ru/flags/flags.md)
