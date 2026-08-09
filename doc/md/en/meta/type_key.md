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

An equality-comparable identity key of a type: the pair of its compile-time name string and a
per-translation-unit discriminator, exposed through an encapsulated class.

- Header: `#include <scl/utility/meta/type_key.h>`
- Declaration:

```cpp
class type_key
{
public:
    type_key() = delete;
    type_key(type_key const &) = delete;
    type_key(type_key &&) = delete;
    type_key & operator=(type_key const &) = delete;
    type_key & operator=(type_key &&) = delete;

    constexpr std::string_view name() const noexcept;

    friend constexpr bool operator==(type_key const &, type_key const &) noexcept;
    // != is synthesised from ==
};
```

### Semantics

- `name()` is the [`type_name<T>()`](type_name.md) string of the identified type. The private
  discriminator is `nullptr` for ordinary (external-linkage) types; for TU-local types it holds
  the address of an anchor object private to the including translation unit, so same-named
  anonymous-namespace types from different translation units carry different values.
- Equality short-circuits on **address identity** first: keys live in per-type constants (see
  `type_key_of`), so comparing stored references resolves to one pointer comparison. Otherwise
  it falls back to content — name and discriminator both equal. For types declared at namespace
  scope this never falsely matches (same-named TU-local types differ by discriminator) and
  never falsely rejects (an external type used from an executable and a shared library compares
  by name with a null discriminator on both sides — unmerged inline instantiations on Windows
  cannot break it).
- Only `type_key_of<T>()` can produce a key: members are private, the default constructor is
  deleted, and there is no mutating API. A hand-built key pairing one type's name with another
  type's discriminator cannot exist.
- The key is an identity with reference semantics and is therefore **non-copyable and
  non-movable** (the precedent is `std::type_info`): a copy would silently drop the address
  fast path and invite storing keys by value. Consumers hold `type_key const &` or
  `type_key const *`.
- The key is intentionally **not ordered** (no `operator<=>`, no `std::less` support): its
  identity involves object addresses, whose relative order is unspecified between unrelated
  objects and unusable in constant expressions. Hashing support is deferred until a real
  consumer exists.

### Key lifetime

A key is a view into the module (executable or shared library) that produced it: `name()`
points into that module's signature literal and the discriminator into that module's anchor.
**A key must not outlive its producing module.** After `FreeLibrary`/`dlclose` the pointers
dangle, and a module later mapped at the same base address can even resurrect a stale TU-local
key into falsely matching a fresh unrelated one. Drop stored keys when unloading the module
that produced them.

---

## type_key_of<T>

Returns a reference to the per-type `type_key` identifying `T`.

- Header: `#include <scl/utility/meta/type_key.h>`
- Declaration: `template <typename T> constexpr type_key const & type_key_of() noexcept;`

The key is a per-type `inline constexpr` variable: every use of `type_key_of<T>()` for an
external `T` inside one module refers to the same object, which makes equality against a stored
reference a single address comparison. The key is non-copyable, so consumers storing keys
long-term (type-erasure wrappers) hold `type_key const &` or `type_key const *` to the per-type
constant — exactly the form that keeps the fast path.

### Precondition

`T` must be declared at namespace scope. Local classes and closure types are outside the
contract: compilers render them without a reliable marker, and no compile-time guard can detect
the violation. On Clang a local class renders as its bare name, so its key is byte-identical to
the key of a same-named namespace-scope type — two genuinely different types compare equal:

```cpp
struct duck {};                     // namespace scope
int main() {
    struct duck {};                 // local class — outside the contract
    // passes on Clang: a false match against the ordinary external type
    static_assert(scl::type_key_of<::duck>() == scl::type_key_of<duck>());
}
```

(GCC renders `main()::duck` and MSVC `struct main::duck`, which do not collide.) To give such a
type a well-defined key, move it into an anonymous namespace of the same translation unit:

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

Same-named anonymous-namespace types from different translation units produce keys with equal
`name()` strings but different discriminators — the keys compare unequal, which is exactly what
a type-erasure facility needs before a `static_cast` back from an erased state.

### Typical Use Cases

Type-erased storage (`any`-like wrappers, function-argument views) that must recover the typed
pointer by comparing a stored marker, without enabling RTTI and without the undefined behaviour
a string-only comparison would allow on the anonymous-namespace collision.

---

## is_tu_local<T>

Detects whether `T` is local to the current translation unit, i.e. declared in an anonymous
namespace directly or as a component of a compound type.

- Header: `#include <scl/utility/meta/tu_local.h>`
- Declaration:

```cpp
template <typename T> struct is_tu_local;                    // : std::bool_constant<...>
template <typename T> inline constexpr bool is_tu_local_v;   // = is_tu_local<T>::value
```

### Semantics

Detection derives the compiler's anonymous-namespace marker at compile time — by diffing the
`type_name<T>()` rendering of a TU-local probe type against an external probe of the same
shape — and searches the rendered name of `T` for it. Two markers are derived, because MSVC
spells the marker differently for top-level types and for types nested inside template
arguments. Deriving instead of hardcoding keeps the trait working on untested compilers.

A marker is accepted anywhere outside string and character literals, so compound types are
covered: a pointer to, reference to, or template specialization over a TU-local type is
TU-local itself. String and character non-type template parameters render inside quoted
literals (or as character codes on MSVC), and literal contents are skipped — an NTTP spelling
the marker cannot misclassify an external type.

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

- [`example/meta/type_key/meta_type_key_example.cpp`](../../../../example/meta/type_key/meta_type_key_example.cpp) —
  runnable version: a hashable handle over `type_key`, kept consistent with the key's
  identity and used directly as an unordered-container key.
