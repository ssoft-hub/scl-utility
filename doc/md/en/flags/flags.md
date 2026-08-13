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

Every block below is taken from the compiled example, so what the page shows is code
that works.

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

A braced empty list selects the default (empty) constructor. Passing a value
whose ordinal is `>= bit_count` throws `std::out_of_range`.

The second template parameter sets the storage width; use it when the default of
32 bits is not the right size:

```cpp
using small = scl::flags<permission, 4>; // 4-bit storage
```

### Membership and predicates

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

`flags` is a bidirectional range whose elements are its *set* flags, as `Enum`
values in ascending ordinal order. `size()` is the number of set flags (the
population count), distinct from the static `capacity` (the bit width).

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
  runnable version: combining and querying flags, set algebra between two masks,
  and iterating the set flags in both directions.
- [Russian documentation](../../ru/flags/flags.md)
