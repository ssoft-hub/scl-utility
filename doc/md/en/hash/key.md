# Hash Keys and Hashers

- Header: `#include <scl/utility/hash.h>`
- Individual headers: `#include <scl/utility/hash/key.h>`, `<scl/utility/hash/fnv1a.h>`, etc.

## Overview

The `scl::hash` module provides a set of non-cryptographic hash functions and a
strongly-typed hash-value wrapper `key<Hasher>`. Together they enable:

- **Compile-time string hashing** — all functions and `key` are `constexpr`.
- **`switch`/`case` dispatch on strings** — `key` converts implicitly to an
  integer, making it usable as a case label.
- **STL container integration** — `std::hash<key<Hasher>>` is specialised.
- **String-keyed template parameters (NTTP)** — `key` is a structural type,
  so it may appear as a non-type template argument (C++20).

All hash functions accept a range satisfying the concept `std::ranges::range` whose element
is one trivially copyable byte of data and which is not a bounded array - a
`std::string_view` object, a `std::string` object, a `std::span<std::byte>` object and byte
vectors among them. A hash function also accepts a range that only a non-`const` reference
traverses. The note on arrays below gives the reason a bounded array stands outside that set.

A wider element — `wchar_t`, `char16_t`, `char32_t`, or any arithmetic type — is
rejected at compile time rather than truncated to its low byte, which would let two
inputs differing above that byte produce one hash value.

`scl::hash::byte_view` is where such a range says which bytes it means:

```cpp
#include <scl/utility/hash/byte_view.h>
#include <scl/utility/hash/fnv1a.h>

#include <string_view>

constexpr auto value = scl::hash::fnv1a(scl::hash::byte_view(std::u16string_view{u"start"}));
```

Each element contributes `sizeof(element)` bytes, least significant first, whatever the
host's own byte order — two machines hash one input alike. A byte-sized element passes
through unchanged, so saying `byte_view` where none is needed changes nothing.

The function `byte_view` refuses a floating-point element, because its bytes tell apart
values that compare equal (`0.0` against `-0.0`, one `NaN` against another). It spells an
element of every integer type, the type `wchar_t` among them.

The width of the types `wchar_t`, `int`, `long` and `size_t` is the platform's own. The type
`wchar_t` is two bytes on Windows and four elsewhere, so a hash value taken over such an
element is comparable within one build rather than across platforms. The standard fixes the
width of the types `char16_t` and `char32_t`, so one text occupies the same number of bytes
in them on every platform, and the caller should take an element of a fixed width where a
hash value has to compare equal across builds for different platforms.

> **Note on arrays.**
> The bytes a range spans are the bytes hashed, and the type of an array does not show
> which of its bytes the caller counts as data. An array declared `char buffer[64]` and
> holding three characters is sixty-four elements to its type and three to its author, and
> nothing in the type says which is meant. A string literal poses the same question with
> the count already fixed, since the literal `"hello"` has the type `char const[6]`, whose
> sixth element is a terminator
> the text does not contain, and no caller wrote that bound of six. The refusal reaches an
> array of any element type - an array declared `std::uint8_t buffer[64]` and carrying
> three bytes of payload is the same case.
>
> The caller should name the bytes instead, and every spelling then answers the published
> value of its algorithm over them:
>
> ```cpp
> #include <scl/utility/hash/fnv1a.h>
>
> #include <span>
> #include <string_view>
>
> using namespace std::string_view_literals;
>
> char buffer[64]{'a', 'b', 'c'};                       // three elements from a C interface
>
> auto const named = scl::hash::fnv1a("hello"sv);                // the five characters
> auto const text = scl::hash::fnv1a(std::string_view{buffer});  // the text the buffer holds
> auto const whole = scl::hash::fnv1a(std::span{buffer});        // every element of the buffer
> ```
>
> Naming the bytes of a filled buffer takes the length the caller knows.
> `std::string_view{buffer}` stops at a terminator, which a buffer holding bytes rather
> than text need not carry, so `std::string_view{buffer, length}` or
> `std::span{buffer, length}` is what names the bytes the caller put there.
>
> The refusal follows what the type system can tell apart, not where the wrong answer
> actually arises. A string literal and a buffer a caller declares share one kind of
> type - `"hello"` is a `char const[6]`, and so is `constexpr char declared[6]`. A
> constraint that refuses the literal therefore refuses the declared buffer with it,
> having no way to tell the two apart. The refusal reaches the bounded array and stops
> there: the type `std::array<char, 64>` reports sixty-four for the reason `char[64]` does, is
> taken all the same, and a partially filled one answers for its whole bound. A
> `std::string_view` object, a `std::span` object, a `std::array` object and a container
> are all taken.
>
> The rule reaches only the array itself, so the view `std::views::all(buffer)` is taken
> and reaches the whole bound of the array.

---

## Hash Algorithms

### FNV-1a — `scl::hash::fnv1a`

```cpp
#include <scl/utility/hash/fnv1a.h>

using namespace std::string_view_literals;

constexpr auto h = scl::hash::fnv1a("hello"sv);
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

**Chaining** two ranges into one hash value:

```cpp
#include <scl/utility/hash/fnv1a.h>

#include <cstdint>
#include <string_view>

std::uint64_t chained()
{
    auto h = scl::hash::fnv1a(std::string_view{"foo"});
    h      = scl::hash::fnv1a(std::string_view{"bar"}, h);
    return h;   // == fnv1a(std::string_view{"foobar"})
}
```

---

### djb2a — `scl::hash::djb2`

```cpp
#include <scl/utility/hash/djb2.h>

using namespace std::string_view_literals;

constexpr auto h = scl::hash::djb2("hello"sv);
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

using namespace std::string_view_literals;

constexpr auto h = scl::hash::sdbm("hello"sv);
```

| Property | Value |
|---|---|
| Width | 64-bit |
| Seed | `0` |
| Algorithm | `h = byte + (h << 6) + (h << 16) - h` |
| Chaining | Yes |

**SDBM** originated from the sdbm database library. The mixed-shift formula
spreads bits efficiently across the hash value and performs well for both short and
long keys with repeated substrings.

---

### Jenkins One-at-a-Time — `scl::hash::jenkins_ota`

```cpp
#include <scl/utility/hash/jenkins_ota.h>

using namespace std::string_view_literals;

constexpr auto h = scl::hash::jenkins_ota("hello"sv);
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
input bit. Use a 64-bit algorithm when a 64-bit hash value is required.

---

### SipHash-2-4 — `scl::hash::siphash`

```cpp
#include <scl/utility/hash/siphash.h>

using namespace std::string_view_literals;

constexpr scl::hash::siphash_key my_key{0xdeadbeefull, 0xcafebabeull};

constexpr auto with_default = scl::hash::siphash("hello"sv);          // default key
constexpr auto with_own     = scl::hash::siphash("hello"sv, my_key);  // a caller's own key
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
#include <scl/utility/hash/fnv1a.h>

#include <string_view>

using namespace std::string_view_literals;

scl::hash::fnv1a_hasher h;
auto value = h("hello"sv);   // same as scl::hash::fnv1a("hello"sv)
```

`siphash_hasher<Key>` embeds the key as a non-type template parameter, making
two instantiations with different keys **distinct types**:

```cpp
#include <scl/utility/hash/siphash.h>

#include <type_traits>

constexpr scl::hash::siphash_key key_a{1, 2};
constexpr scl::hash::siphash_key key_b{3, 4};

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

A hasher of a caller's own stands as the template argument of the class template
`key<Hasher>` once it is default-constructible and names an integral `result_type` member
type; the constructor of `key` calls it, so it also needs the call operator
`operator()(Range&&)`.

Each hash function states the refusal of a bounded array in its own signature, and no shared
mechanism states it for the whole set of them, so the author of a hasher of their own should
constrain its call operator with the concept `scl::hash::concepts::hashable_range`, which
carries the element rule and the array rule together:

```cpp
struct rolling_hasher {
    using result_type = std::uint64_t;

    template <scl::hash::concepts::hashable_range Range>
    constexpr result_type operator()(Range&& range) const;
};
```

With the constraint left unstated, the hasher takes an array of type `char[64]` and hashes
all sixty-four of its elements, including the elements past the payload the caller wrote into
it. Building an object of type `key<rolling_hasher>` from that array is ill-formed all the
same, because the constraint stands in the constructor of the class template `key`, whatever
hasher that template holds.

---

## `key<Hasher>` — Strongly-Typed Hash Value

```cpp
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/key.h>

#include <string_view>

using namespace std::string_view_literals;
using namespace scl::hash;

constexpr key<> id{"my_event"sv};           // default: siphash_hasher<>
constexpr key<fnv1a_hasher> fnv_id{"x"sv};
```

`key<Hasher>` wraps the integer hash value produced by `Hasher` in a named type,
preventing accidental mixing of raw integers with hash values. The `value_type`
is deduced from `Hasher::result_type`.

| Member | Description |
|---|---|
| `hasher_type` | The `Hasher` type |
| `value_type` | `Hasher::result_type` — the underlying integer type |
| `value` | The hash value itself |
| `operator value_type()` | Implicit conversion to the raw integer |
| `operator<=>` | Three-way comparison (`==`, `!=`, `<`, `<=`, `>`, `>=`) |

### Construction

```cpp
#include <scl/utility/hash/key.h>

#include <array>
#include <string_view>

using namespace std::string_view_literals;
using namespace scl::hash;

constexpr key<> a{"hello"sv};                          // the five characters
constexpr std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
constexpr key<> b{held};                               // the same five, held in storage
static_assert(a == b);                                 // one and the same sequence of bytes
```

A hash function reads a range through a reference to a constant wherever a reference to a
constant traverses it with the same element type. Hashing a container that shares one buffer
with its copies until a write therefore leaves that buffer shared. The reference a range
hands out is fixed by that range alone, and a view over a mutable object reaches that object
mutably, whatever the qualification on the view itself.

The constructor throws nothing where reading the range throws nothing, and its signature
carries that condition as `noexcept(noexcept(Hasher{}(range)))`. The free functions `fnv1a`,
`djb2`, `sdbm`, `jenkins_ota` and `siphash` carry the same conditional guarantee. The
compiler computes it from the `noexcept` markings on the range's own operations, so one type
carries the guarantee under one standard library and not under another. The libstdc++
standard library leaves the iterator operations of the container `std::vector<bool>`
unmarked, whereas the MSVC standard library marks them.

### `switch`/`case` Dispatch

The implicit conversion to `value_type` lets a `key` appear as a `case` label.
This replaces long `if`/`else if` chains with a zero-overhead integer switch:

```cpp
#include <scl/utility/hash/key.h>

#include <string_view>

using namespace std::string_view_literals;

int handle(scl::hash::key<> cmd)
{
    switch (cmd) {
    case scl::hash::key<>{"start"sv}:  return 1;
    case scl::hash::key<>{"stop"sv}:   return 2;
    case scl::hash::key<>{"status"sv}: return 3;
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
#include <scl/utility/hash/key.h>

#include <string_view>
#include <unordered_map>

using namespace std::string_view_literals;

void fill(std::unordered_map<scl::hash::key<>, int> & registry)
{
    registry[scl::hash::key<>{"alpha"sv}] = 1;
    registry[scl::hash::key<>{"beta"sv}]  = 2;
}
```

### Non-Type Template Parameter (NTTP)

`key<Hasher>` is a **structural type** (all members public, underlying type is
a scalar), so it may be used as a non-type template parameter in C++20:

```cpp
#include <scl/utility/hash/key.h>

#include <string_view>

using namespace std::string_view_literals;

// 1. Type tag from a compile-time string-view literal
template <scl::hash::key<> Tag>
struct event {};

using start_event = event<scl::hash::key<>{"start"sv}>;
using stop_event  = event<scl::hash::key<>{"stop"sv}>;
static_assert(!std::is_same_v<start_event, stop_event>);

// 2. Template specialisation by string key
template <scl::hash::key<> Cmd> struct handler { static constexpr int value = 0; };
template <> struct handler<scl::hash::key<>{"start"sv}> { static constexpr int value = 1; };
template <> struct handler<scl::hash::key<>{"stop"sv}>  { static constexpr int value = 2; };

static_assert(handler<scl::hash::key<>{"start"sv}>::value == 1);
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

// The rule on an element, and the rule on a range
namespace concepts {
    template <typename Type> concept byte_element;     // one byte of data
    template <typename Type> concept integer_element;  // byte_view can spell its bytes
    template <typename Range> concept hashable_range;      // the element rule and the array rule
}

// Explicit conversion for a wider element
constexpr auto byte_view(Range&&);   // view of uint8_t, least significant byte first

// Free functions - each noexcept where iterating the range throws nothing
constexpr uint64_t fnv1a(Range&&, uint64_t h = offset_basis) noexcept(...);
constexpr uint64_t djb2 (Range&&, uint64_t h = 5381) noexcept(...);
constexpr uint64_t sdbm (Range&&, uint64_t h = 0) noexcept(...);
constexpr uint32_t jenkins_ota(Range&&) noexcept(...);
constexpr uint64_t siphash(Range&&, siphash_key key = siphash_default_key) noexcept(...);

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
    using value_type  = Hasher::result_type;
    value_type value{};

    explicit constexpr key(Range&& range) noexcept(noexcept(Hasher{}(range)));
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

## See also

- [`example/hash/key_nttp`](../../../../example/hash/key_nttp/hash_key_nttp_example.cpp) —
  runnable version: the class template `key` as a non-type template parameter - a type tag
  from a compile-time string-view literal, a specialisation selected by string value, and
  dispatch on a compile-time key.
- [`example/hash/byte_view`](../../../../example/hash/byte_view/hash_byte_view_example.cpp) —
  runnable version: hashing a `std::u16string_view` and a `std::vector<std::uint32_t>`
  through `byte_view`, and what the fixed byte order buys.
