# Flags

A type-safe set of enumeration values.

- Header: `#include <scl/utility/flags.h>`

## Overview

`scl::flags<Enum, bit_count>` holds the enum values put into it and stores them
as one bit per enumerator: the bit index of a value equals the enumerator's
underlying value (`One == 0`, `Two == 1`, ...), so enumerators need no explicit
values. Sets combine through operators, never through a cast to the underlying
integer.

The bits live in a `std::array<std::byte>` sized to `bit_count` (default 32), so
every operation is usable in constant evaluation on the C++20 baseline — unlike
`std::bitset`, whose query and mutate members only became `constexpr` in C++23.

Any enumeration is accepted, scoped or not: the template parameter is a
`scl::concepts::enum_type`, and nothing in the implementation depends on the enumeration
being scoped - it reads the underlying value and nothing else. Over an unscoped enumeration
`A | B` is an `int` by integral promotion, and `flags` has no constructor from one, so the
set is spelled `flags{A, B}` there rather than built with the bitwise operators.
An enumerator whose ordinal is `>= bit_count` is out of range:
the constructor and `operator[]` throw `std::out_of_range` at runtime and are
ill-formed in constant evaluation. The predicate queries never throw — an
out-of-range ordinal simply reads as not held. Where
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `0` the same call ends the
program through `std::abort()`, since a precondition violation has no value to return
in place of the answer.

## Features

- One bit per enumerator ordinal; no explicit enumerator values required
- `constexpr`-capable throughout (C++20)
- Set algebra `| & ^ -` and compound `|= &= ^= -=` in set-set and set-`Enum` forms
- `operator[]` membership test
- `all_of` / `any_of` / `none_of` predicates over a pack of values and over
  another set (subset / intersection / disjoint)
- `any` / `none` over the whole set
- Bidirectional range over the values held, ascending by ordinal

## API reference

### Construction

Every block below is taken from
[`example/flags/common/flags_common_example.cpp`](../../../../example/flags/common/flags_common_example.cpp),
which the CI compiles and runs, so what the page shows is code that works.

<!-- snippet: example/flags/common/flags_common_example.cpp declare -->
```cpp
enum class permission
{
    read,    // bit 0
    write,   // bit 1
    execute, // bit 2
    remove,  // bit 3
};

using permissions = ::scl::flags<permission>;

// The set the others are measured against. A flags knows its storage width, not which
// values an enumeration declares, so a caller who needs the whole set names it.
constexpr permissions all_permissions{
    permission::read, permission::write, permission::execute, permission::remove};
```

A braced empty list selects the default constructor and holds nothing. Passing a
value whose ordinal is `>= bit_count` throws `std::out_of_range`.

The second template parameter sets the storage width; use it when the default of
32 bits is not the right size:

```cpp
using small = scl::flags<permission, 4>; // 4-bit storage
```

### Membership and predicates

`all_of` / `any_of` / `none_of` take either a pack of values or another `flags`, which is
how a subset, an intersection and a disjoint pair are spelled. An empty pack is vacuously
true for `all_of` and `none_of`, false for `any_of`. There is no "holds every value" query:
which values count as *every* one is the caller's to name, and `all_of` asks against that
set.

<!-- snippet: example/flags/common/flags_common_example.cpp membership -->
```cpp
constexpr permissions granted{permission::read, permission::write};

static_assert(granted[permission::read]);
static_assert(granted.all_of(permission::read, permission::write));
static_assert(granted.any_of(permission::write, permission::execute));
static_assert(granted.none_of(permission::remove));

static_assert(granted.all_of(permissions{permission::read}));    // subset
static_assert(granted.any_of(permissions{permission::write}));   // intersection
static_assert(granted.none_of(permissions{permission::remove})); // disjoint

static_assert(granted.any());
static_assert(!granted.none());
static_assert(!granted.all_of(all_permissions)); // two flags of the four
static_assert(granted.size() == 2);
```

### Combination

Union, intersection, symmetric difference and difference come in the set-set and the
set-`Enum` form, each with a compound counterpart. Every one of them is closed over the
values the operands hold.

There is no complement operator. A complement is taken against the set of all values, and
a `flags` knows only `capacity` — its storage width, which an enumeration is free to
underfill or to spread its enumerators across. Complementing over the width would hand
back ordinals no enumerator names. Name that set instead, and subtract from it.

<!-- snippet: example/flags/common/flags_common_example.cpp algebra -->
```cpp
constexpr permissions required{permission::read, permission::execute};

static_assert((granted | required) == permissions{permission::read, permission::write, permission::execute});
static_assert((granted & required) == permissions{permission::read});
static_assert((granted ^ required) == permissions{permission::write, permission::execute});
static_assert((granted - required) == permissions{permission::write});
static_assert((granted | permission::remove).size() == 3); // flags-Enum form

// A complement is taken against a named set, never against the storage width.
static_assert((all_permissions - granted) == permissions{permission::execute, permission::remove});

// The compound forms mutate in place, so they need an object rather than a constant.
constexpr permissions effective = [] {
    permissions result{permission::read};
    result |= permission::write;
    result -= permission::read;
    return result;
}();
static_assert(effective == permissions{permission::write});
```

### Iteration

`flags` is a bidirectional range over the values it holds, as `Enum` in ascending
ordinal order. `size()` is how many it holds, distinct from the static `capacity`,
which is the storage width.

<!-- snippet: example/flags/common/flags_common_example.cpp iteration -->
```cpp
// Iterating yields the set flags as Enum values, ascending by ordinal; the reverse
// iterators walk the same flags back. size() counts them, capacity is the bit width.
static void print_flags(permissions const & flags)
{
    for (permission const flag : flags)
        ::std::cout << ' ' << static_cast<int>(flag);
    ::std::cout << " |";

    for (auto it = flags.rbegin(); it != flags.rend(); ++it)
        ::std::cout << ' ' << static_cast<int>(*it);

    ::std::cout << " (" << flags.size() << " of " << permissions::capacity << ")\n";
}
```

Because it models `std::ranges::bidirectional_range` and `std::ranges::sized_range`,
`flags` composes with the `<ranges>` views and algorithms.

## See also

- [`example/flags/common/flags_common_example.cpp`](../../../../example/flags/common/flags_common_example.cpp) —
  the program these blocks come from: membership and predicates, set algebra
  between two sets, and iteration in both directions.
- [Russian documentation](../../ru/flags/flags.md)
