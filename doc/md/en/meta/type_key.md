# Type identity key

RTTI-free type identity that distinguishes same-named types declared in anonymous namespaces of
different translation units.

- Header: `#include <scl/utility/meta/type_key.h>`

Contents:
- [`type_key`](#type_key)
- [`type_key_of<T>`](#type_key_of)
- [`is_tu_local<T>`](#is_tu_local)

---

## type_key

`scl::type_key` identifies one type. Two keys compare equal when they identify the same type.
The key holds that type's name, obtained at compile time, beside a per-translation-unit
discriminator.

- Header: `#include <scl/utility/meta/type_key.h>`
- Declaration:

```cpp
class type_key
{
public:
    constexpr type_key() = default;
    constexpr type_key(type_key const &) = default;
    constexpr type_key(type_key &&) = default;
    constexpr type_key & operator=(type_key const &) = default;
    constexpr type_key & operator=(type_key &&) = default;
    constexpr ~type_key() = default;

    constexpr std::string_view name() const noexcept;

    friend constexpr bool operator==(type_key const &, type_key const &) noexcept;
    // != is synthesised from ==
};
```

### Semantics

`name()` answers the name of the identified type as a string - the one
[`type_name<T>()`](type_name.md) produces.

The discriminator is never handed out. It is `nullptr` for a type with external linkage; for a
type from an anonymous namespace it holds the address of an anchor object, one per translation
unit, so same-named types from different translation units carry different discriminators.

Comparison starts with addresses. `type_key_of<T>()` answers a reference to a constant, one per
type, so comparing two such references costs a single pointer comparison. Where the addresses
differ, the contents are compared: name and discriminator. For a type declared at namespace
scope this yields neither a false match nor a false reject. Same-named types from anonymous
namespaces differ by discriminator; an external type used from an executable and from a shared
library compares by name with a null discriminator on both sides, and unmerged inline
instantiations on Windows do not break it.

Only `type_key_of<T>()` hands out a key that identifies a type. Building one by hand, pairing
one type's name with another type's discriminator, is impossible: the members are private and
nothing modifies a key. The one key a caller writes for itself is the empty one,
`scl::type_key{}`. It identifies no type and equals no key `type_key_of` produces.

A key is a **value**: it copies, moves and assigns. A copy identifies the type its source
identifies, and equals it.

Keys carry no order: neither `operator<=>` nor `std::less`. Identity rests on object addresses,
whose relative order between unrelated objects the standard leaves unspecified and constant
expressions cannot read. Nor does a hash function come with the key; the example at the end of
the page writes one and puts a key into an unordered container.

### Key lifetime

A key points into the module - executable or shared library - that produced it: `name()` at
that module's string literal, the discriminator at its anchor. **A key must not outlive the
module that produced it.** After `FreeLibrary` or `dlclose` both pointers dangle, and a module
loaded afterwards at the same base address can make a stale key compare equal to a fresh,
unrelated one. Destroy stored keys before unloading the module that produced them.

---

## type_key_of<T>

Returns a reference to the `type_key` that identifies `T`.

- Header: `#include <scl/utility/meta/type_key.h>`
- Declaration: `template <typename T> constexpr type_key const & type_key_of() noexcept;`

The key sits in an `inline constexpr` variable, one per type. Inside one module every call to
`type_key_of<T>()` for an external `T` reaches the same object, so comparing against a stored
reference costs one address comparison. Code that keeps a key for long - a type-erasure wrapper,
say - holds either that reference, which keeps the fast path, or a copy, which compares by
content.

### Precondition

`T` must be declared at namespace scope. Local classes and closure types stay outside the
contract: a compiler writes their name without a reliable marker of locality, and no check at
compile time catches the violation. Clang writes a local class as its bare name, so its key is
byte-identical to the key of a same-named type at namespace scope, and two genuinely different
types compare equal:

```cpp
struct duck {};                     // namespace scope
int main() {
    struct duck {};                 // local class - outside the contract
    // passes on Clang: a false match against the ordinary external type
    static_assert(scl::type_key_of<::duck>() == scl::type_key_of<duck>());
}
```

GCC writes `main()::duck` and MSVC `struct main::duck`, and those do not collide. To give such
a type a well-defined key, move it into an anonymous namespace of the same translation unit:

```cpp
namespace { struct duck {}; }             // well-defined key
```

### Examples

```cpp
#include <scl/utility/meta/type_key.h>

namespace { struct duck {}; }
struct goose {};

constexpr auto & anon_key = scl::type_key_of<duck>();
constexpr auto & ext_key  = scl::type_key_of<goose>();

static_assert(anon_key == scl::type_key_of<duck>());
static_assert(anon_key != ext_key);
static_assert(&scl::type_key_of<goose>() == &ext_key);  // one object per type
```

Same-named types from anonymous namespaces of different translation units produce keys with
equal `name()` strings and different discriminators. The keys compare unequal, which is exactly
what a type-erasure facility needs before casting back from an erased state.

### Typical Use Cases

Type-erased storage - `any`-like wrappers, views over a function argument - that has to recover
the typed pointer by comparing a stored key. No RTTI, and none of the undefined behaviour a
comparison by name alone would allow when two anonymous namespaces spell the same name.

---

## is_tu_local<T>

Answers whether `T` is local to the current translation unit, that is, declared in an anonymous
namespace either directly or as part of a compound type.

- Header: `#include <scl/utility/meta/tu_local.h>`
- Declaration:

```cpp
template <typename T> struct is_tu_local;                    // : std::bool_constant<...>
template <typename T> inline constexpr bool is_tu_local_v;   // = is_tu_local<T>::value
```

### Semantics

The compiler's anonymous-namespace marker is derived at compile time: the name of a probe type
local to the translation unit is compared against the name of an external probe of the same
shape, and the marker that differs is then looked for in the name of `T`. Two markers are
derived, because MSVC writes it one way for a top-level type and another for a type inside a
template argument. Deriving it rather than listing it keeps the trait working on a compiler
nobody has tested it against.

A marker is accepted anywhere outside string and character literals, so compound types are
covered: a pointer to a local type, a reference to one, and a template specialization over one
are local themselves. A string or character template parameter is written inside a quoted
literal, as character codes on MSVC, and literal contents are skipped - so a parameter that
spells the marker cannot put an external type in the local class by mistake.

### Example

```cpp
#include <scl/utility/meta/tu_local.h>
#include <vector>

namespace { struct duck {}; }
struct goose {};

static_assert(scl::is_tu_local_v<duck>);
static_assert(scl::is_tu_local_v<duck *>);
static_assert(scl::is_tu_local_v<std::vector<duck>>);
static_assert(!scl::is_tu_local_v<goose>);
static_assert(!scl::is_tu_local_v<int>);
```

## See also

- [`example/meta/type_key/meta_type_key_example.cpp`](../../../../example/meta/type_key/meta_type_key_example.cpp) -
  runnable version: a hashable handle over `type_key`, kept consistent with the key's
  identity and used directly as an unordered-container key.
