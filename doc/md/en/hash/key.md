# Hash Keys and Hashers

- Header: `#include <scl/utility/hash.h>`
- Individual headers: `#include <scl/utility/hash/key.h>`, `<scl/utility/hash/fnv1a.h>`, etc.

## Overview

The `scl::hash` module provides a set of non-cryptographic hash functions and a
strongly-typed digest wrapper `key<Hasher>`. Together they enable:

- **Compile-time string hashing** — all functions and `key` are `constexpr`.
- **`switch`/`case` dispatch on strings** — `key` converts implicitly to an
  integer, making it usable as a case label.
- **STL container integration** — `std::hash<key<Hasher>>` is specialised.
- **String-keyed template parameters (NTTP)** — `key` is a structural type,
  so it may appear as a non-type template argument (C++20).

All hash functions accept any `std::ranges::range` whose element type is
convertible to `std::uint8_t` — including string literals, `std::string_view`,
`std::string`, `std::span<std::byte>`, and byte vectors.

> **Note on string literals vs `std::string_view`.**
> A string literal `"hello"` is a `const char[6]` — the range includes the
> null terminator `\0`. `std::string_view{"hello"}` covers only the five
> printable characters. These inputs produce **different** digests. Use
> `std::string_view` when precise byte control is needed (e.g. chaining,
> reference-vector tests).

---

## Hash Algorithms

### FNV-1a — `scl::hash::fnv1a`

```cpp
#include <scl/utility/hash/fnv1a.h>

constexpr auto h = scl::hash::fnv1a("hello");
```

| Property | Value |
|---|---|
| Width | 64-bit |
| Seed / offset basis | `14695981039346656037` (`0xcbf29ce484222325`) |
| Algorithm | `h ^= byte; h *= 1099511628211` |
| Chaining | Yes — pass previous result as the second argument |

**FNV-1a** (Fowler–Noll–Vo, variant 1a) is a classic non-cryptographic hash
widely used in compilers, linkers, and embedded systems. The XOR-then-multiply
order gives better avalanche for similar inputs compared to the original FNV-1
(multiply-then-add).

**Chaining** two ranges into one digest:

```cpp
auto h = scl::hash::fnv1a(std::string_view{"foo"});
h      = scl::hash::fnv1a(std::string_view{"bar"}, h);
// h == fnv1a(std::string_view{"foobar"})
```

---

### djb2a — `scl::hash::djb2`

```cpp
#include <scl/utility/hash/djb2.h>

constexpr auto h = scl::hash::djb2("hello");
```

| Property | Value |
|---|---|
| Width | 64-bit |
| Seed | `5381` |
| Algorithm | `h = h * 33 ^ byte` |
| Chaining | Yes |

**djb2a** (Daniel J. Bernstein, XOR variant) is renowned for its simplicity
and good distribution on short ASCII keys. The seed `5381` and multiplier `33`
were chosen empirically. The XOR variant (`djb2a`) outperforms the additive
variant (`djb2`) for inputs with similar prefixes.

---

### SDBM — `scl::hash::sdbm`

```cpp
#include <scl/utility/hash/sdbm.h>

constexpr auto h = scl::hash::sdbm("hello");
```

| Property | Value |
|---|---|
| Width | 64-bit |
| Seed | `0` |
| Algorithm | `h = byte + (h << 6) + (h << 16) - h` |
| Chaining | Yes |

**SDBM** originated from the sdbm database library. The mixed-shift formula
spreads bits efficiently across the digest and performs well for both short and
long keys with repeated substrings.

---

### Jenkins One-at-a-Time — `scl::hash::jenkins_ota`

```cpp
#include <scl/utility/hash/jenkins_ota.h>

constexpr auto h = scl::hash::jenkins_ota("hello");
```

| Property | Value |
|---|---|
| Width | **32-bit** |
| Seed | `0` |
| Algorithm | Per-byte mix + finalization avalanche |
| Chaining | No (finalization step is non-associative) |

**Jenkins OAT** (Bob Jenkins, 1997) is the only 32-bit algorithm in this
module. It applies a three-step finalization pass (`h += h<<3; h ^= h>>11;
h += h<<15`) that ensures full avalanche — every output bit depends on every
input bit. Use a 64-bit algorithm when a 64-bit digest is required.

---

### SipHash-2-4 — `scl::hash::siphash`

```cpp
#include <scl/utility/hash/siphash.h>

constexpr auto h = scl::hash::siphash("hello");             // default key
constexpr auto h = scl::hash::siphash("hello", my_key);    // custom key
```

| Property | Value |
|---|---|
| Width | 64-bit |
| Key | 128-bit (`siphash_key{k0, k1}`) |
| Rounds | 2 compression + 4 finalization (SipHash-**2-4**) |
| Chaining | No (keyed, stateful) |
| Default key | Paper test-vector key (`siphash_default_key`) |

**SipHash-2-4** (Aumasson & Bernstein, 2012) was designed specifically to
replace ad-hoc string hash functions in hash tables. It provides strong
protection against **hash-flooding attacks**: an adversary who does not know
the key cannot craft inputs that collide. It is the default string hash in
Python ≥ 3.4, Rust, Ruby, and Perl.

#### Keys

```cpp
// Fixed key — suitable for compile-time identifiers and switch/case dispatch.
constexpr scl::hash::siphash_key compile_key{0xdeadbeefcafeull, 0xabad1deaull};

// Runtime random key — required for hash-flooding protection.
scl::hash::siphash_key runtime_key{
    generate_random_uint64(),
    generate_random_uint64()
};
```

`siphash_default_key` uses the standard paper test-vector bytes
(`0x00..0x0f`). **Do not use it where hash-flooding resistance is required.**

---

## Hasher Types

Each algorithm ships with a callable wrapper struct that satisfies the
`scl::hash::concepts::byte_hasher` concept:

| Struct | `result_type` | Algorithm |
|---|---|---|
| `fnv1a_hasher` | `std::uint64_t` | FNV-1a |
| `djb2_hasher` | `std::uint64_t` | djb2a |
| `sdbm_hasher` | `std::uint64_t` | SDBM |
| `jenkins_ota_hasher` | `std::uint32_t` | Jenkins OAT |
| `siphash_hasher<Key>` | `std::uint64_t` | SipHash-2-4 |

```cpp
scl::hash::fnv1a_hasher h;
auto digest = h("hello");   // same as scl::hash::fnv1a("hello")
```

`siphash_hasher<Key>` embeds the key as a non-type template parameter, making
two instantiations with different keys **distinct types**:

```cpp
using hasher_a = scl::hash::siphash_hasher<key_a>;
using hasher_b = scl::hash::siphash_hasher<key_b>;
static_assert(!std::is_same_v<hasher_a, hasher_b>);
```

### `byte_hasher` Concept

```cpp
template <typename H>
concept byte_hasher =
    std::default_initializable<H> &&
    requires { typename H::result_type; } &&
    std::integral<typename H::result_type>;
```

Any custom hasher that exposes `result_type` and `operator()(Range&&)` can be
used with `key<>`.

---

## `key<Hasher>` — Strongly-Typed Digest

```cpp
#include <scl/utility/hash/key.h>

using namespace scl::hash;

constexpr key<> id{"my_event"};           // default: siphash_hasher<>
constexpr key<fnv1a_hasher> fnv_id{"x"};
```

`key<Hasher>` wraps the integer digest produced by `Hasher` in a named type,
preventing accidental mixing of raw integers with hash values. The `value_type`
is deduced from `Hasher::result_type`.

| Member | Description |
|---|---|
| `hasher_type` | The `Hasher` type |
| `value_type` | `Hasher::result_type` — the underlying integer type |
| `value` | Raw digest |
| `operator value_type()` | Implicit conversion to the raw integer |
| `operator<=>` | Three-way comparison (`==`, `!=`, `<`, `<=`, `>`, `>=`) |

### Construction

```cpp
constexpr key<> a{"hello"};              // string literal (includes '\0')
constexpr key<> b{std::string_view{"hello"}};  // 5 bytes, no '\0'
// a != b  — different byte sequences
```

### `switch`/`case` Dispatch

The implicit conversion to `value_type` lets a `key` appear as a `case` label.
This replaces long `if`/`else if` chains with a zero-overhead integer switch:

```cpp
int handle(scl::hash::key<> cmd)
{
    switch (cmd) {
    case scl::hash::key<>{"start"}:  return 1;
    case scl::hash::key<>{"stop"}:   return 2;
    case scl::hash::key<>{"status"}: return 3;
    default:                          return 0;
    }
}
```

All `case` values are evaluated at compile time — the switch compiles to the
same code as if the integer constants were written by hand.

### STL Containers

`std::hash<key<Hasher>>` is specialised, so `key` works directly as a key in
`std::unordered_map` and `std::unordered_set`:

```cpp
std::unordered_map<scl::hash::key<>, int> registry;
registry[scl::hash::key<>{"alpha"}] = 1;
registry[scl::hash::key<>{"beta"}]  = 2;
```

### Non-Type Template Parameter (NTTP)

`key<Hasher>` is a **structural type** (all members public, underlying type is
a scalar), so it may be used as a non-type template parameter in C++20:

```cpp
// 1. Type tag from a compile-time string
template <scl::hash::key<> Tag>
struct event {};

using start_event = event<scl::hash::key<>{"start"}>;
using stop_event  = event<scl::hash::key<>{"stop"}>;
static_assert(!std::is_same_v<start_event, stop_event>);

// 2. Template specialisation by string key
template <scl::hash::key<> Cmd> struct handler { static constexpr int value = 0; };
template <> struct handler<scl::hash::key<>{"start"}> { static constexpr int value = 1; };
template <> struct handler<scl::hash::key<>{"stop"}>  { static constexpr int value = 2; };

static_assert(handler<scl::hash::key<>{"start"}>::value == 1);
```

---

## Choosing an Algorithm

| Algorithm | Width | Keyed | `constexpr` | Best for |
|---|---|---|---|---|
| SipHash-2-4 | 64-bit | Yes | Yes | Hash tables, hash-flooding protection |
| FNV-1a | 64-bit | No | Yes | Compile-time IDs, `switch`/case, embedded |
| djb2a | 64-bit | No | Yes | Short ASCII keys, simple use |
| SDBM | 64-bit | No | Yes | Database keys, repeated substrings |
| Jenkins OAT | 32-bit | No | Yes | 32-bit environments, legacy interop |

**Default `key<>`** uses `siphash_hasher<>`. If you need hash-flooding
protection at runtime, construct a `siphash_key` from a random source and use
`siphash_hasher<runtime_key>`. For compile-time-only use, any algorithm works.

---

## API Summary

```cpp
namespace scl::hash {

// Free functions
constexpr uint64_t fnv1a(Range&&, uint64_t h = offset_basis);
constexpr uint64_t djb2 (Range&&, uint64_t h = 5381);
constexpr uint64_t sdbm (Range&&, uint64_t h = 0);
constexpr uint32_t jenkins_ota(Range&&);
constexpr uint64_t siphash(Range&&, siphash_key key = siphash_default_key);

// Hasher structs (satisfy byte_hasher)
struct fnv1a_hasher;
struct djb2_hasher;
struct sdbm_hasher;
struct jenkins_ota_hasher;
template <siphash_key Key = siphash_default_key>
struct siphash_hasher;

// Key type
template <byte_hasher Hasher = siphash_hasher<>>
struct key {
    using hasher_type = Hasher;
    using value_type  = typename Hasher::result_type;
    value_type value{};

    constexpr key(Range&&) noexcept;
    constexpr operator value_type() const noexcept;
    friend constexpr auto operator<=>(key const&, key const&) noexcept = default;
};

} // namespace scl::hash

// STL integration
template <typename Hasher>
struct std::hash<scl::hash::key<Hasher>>;
```

---

## References

- Aumasson, J.-P. & Bernstein, D. J. (2012). [SipHash: a fast short-input PRF](https://www.131002.net/siphash/)
- Fowler, G., Noll, L. C. & Vo, P. (1991). FNV Hash — public domain.
- Bernstein, D. J. (1990). djb2 — public domain.
- Jenkins, B. (1997). [Hash Functions](http://www.burtleburtle.net/bob/hash/doobs.html) — public domain.
