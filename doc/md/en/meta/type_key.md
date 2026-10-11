# `type_key`

The class `type_key` identifies a type without RTTI, and the function template `type_key_of<T>()`
returns a reference to the key of the type `T`. Types of one name declared in unnamed namespaces of
two translation units get keys that compare unequal.

- Header: `#include <scl/utility/meta/type_key.h>`

```cpp
namespace scl
{
    class type_key
    {
    public:
        constexpr type_key() noexcept = default;
        constexpr type_key(type_key const &) = default;
        constexpr type_key(type_key &&) = default;
        constexpr type_key & operator=(type_key const &) = default;
        constexpr type_key & operator=(type_key &&) = default;
        constexpr ~type_key() = default;

        [[nodiscard]] constexpr ::std::string_view name() const noexcept;

        [[nodiscard]] friend constexpr bool operator==(type_key const & left,
            type_key const & right) noexcept;
    };

    template <typename T, ::std::byte const * Unit = /* anchor of the translation unit or nullptr */>
    [[nodiscard]] constexpr type_key const & type_key_of() noexcept;
}
```

## Semantics

- **What a key holds:** the name [`scl::type_name<T>()`](type_name.md) gives the type, which
  the member function `name()` returns, beside a discriminator no member returns. The discriminator
  is a null pointer for a type with external linkage, apart from a type whose name holds one of
  the symbols described below. For a type local to its translation unit, as
  [`scl::is_tu_local`](is_tu_local.md) tells, it is the address of an object private to that
  translation unit, so types of one name from two translation units carry different discriminators.
- **Comparison:** two keys compare equal when their names and their discriminators are equal, and
  `!=` is the negation of `==`. Two references to one key compare equal at once, so comparing
  a stored reference with the result of `type_key_of<T>()` costs one comparison of addresses when
  the reference is to that key. Types of one name in unnamed namespaces of two translation units
  differ by the discriminator, and a type with external linkage used from an executable and from
  a shared library compares equal by its name, apart from a type whose name holds one of the symbols
  described below.
- **Two types of one name:** with GCC and Clang one name may stand for two types, as the page of
  [`type_short_name<T>()`](type_name.md) states. Both write a pointer to `operator-` before
  a closing `>` as `operator->` and a pointer to `operator<=` before it as `operator<=>`, and Clang
  writes a pointer to `operator>` before it as `operator>>`. GCC also writes a pointer to such
  a specialization, `Tagged<&P::operator- > *`, as `Tagged<&P::operator->*`. With GCC and Clang,
  a type whose name holds `operator->`, `operator->*`, `operator<=>` or `operator>>` carries
  the address of an object of its own as the discriminator. The keys of two such types differ, and
  the key of such a type from another module may compare unequal to its key from this one.
- **Where a key comes from:** only `type_key_of<T>()` returns a key that identifies a type. The data
  members are private, and no member function changes a key apart from assignment. The one key
  a caller writes is the empty one, `scl::type_key{}`, which identifies no type and compares unequal
  to every key `type_key_of<T>()` returns.
- **A key is a value:** it copies, moves and assigns, and a copy identifies the type its source
  identifies.
- **No order and no hash:** keys carry neither `operator<=>` nor `std::less`, since the identity of
  a local type and of a type whose name holds one of the symbols above rests on an address, whose
  relative order between unrelated objects the standard leaves unspecified and constant evaluation
  cannot read. No hash comes with the key; one is written in the example below.
- **One key per type:** the key of a type is an `inline constexpr` variable, so within one module
  every call of `type_key_of<T>()` for a type with external linkage returns a reference to one
  object. A caller that stores a key holds either that reference, which compares by address, or
  a copy, which compares by content. A caller passes `T` alone. The default of `Unit` is the anchor
  of the translation unit for a local type and a null pointer for any other type. The anchor keeps
  the key of a local type in its translation unit, which GCC would otherwise merge with the key of
  a local type of the same name from another translation unit where `T` holds a pointer to a member
  of a type in an unnamed namespace. GCC merges in the same way the specialization of a caller's
  template that depends on `T` alone, such as a variable template holding `&type_key_of<T>()`, so
  for such a type that specialization holds the key of only one translation unit.
- **Lifetime:** a key points into the module, an executable or a shared library, that produced it:
  its name into the string of that module, and a discriminator that is not null into the object of
  that module. A key must not outlive the module that produced it. After `FreeLibrary` or `dlclose`
  the name dangles, and so does a discriminator that is not null, and a module loaded later at
  the same address can make a stale key compare equal to the key of another type.
- **Precondition:** `T` is neither a local class nor a closure type, nor a type built of one. No
  check detects a violation: Clang spells a local class `duck` of a function as `duck`, whatever
  namespace the function is in, the name of a class `duck` declared in the global namespace, so
  the keys of the two classes compare equal. A class moved into an unnamed namespace of the same
  translation unit gets a key of its own.

## Examples

Keys of a local type and of a type with external linkage:

<!-- snippet: example/meta/type_key/meta_type_key_example.cpp type_key_of -->
```cpp
namespace
{
    struct duck
    {};
} // namespace

struct goose
{};

constexpr auto & duck_key = scl::type_key_of<duck>();
constexpr auto & goose_key = scl::type_key_of<goose>();

static_assert(duck_key == scl::type_key_of<duck>());
static_assert(duck_key != goose_key);
static_assert(&scl::type_key_of<goose>() == &goose_key); // one object per type
static_assert(scl::type_key{} != goose_key);             // the empty key identifies no type
```

A handle of a key with a hash of its name, which keys an unordered container:

<!-- snippet: example/meta/type_key/meta_type_key_example.cpp hash_key -->
```cpp
// A handle of the key of a type with a hash value of its name, computed once.
class hash_key
{
public:
    explicit constexpr hash_key(scl::type_key const & key) noexcept
        : m_key{&key}
        , m_hash{static_cast<std::size_t>(scl::hash::fnv1a(key.name()))}
    {}

    template <typename T>
    [[nodiscard]]
    static constexpr hash_key of() noexcept
    {
        return hash_key{scl::type_key_of<T>()};
    }

    [[nodiscard]]
    constexpr scl::type_key const & key() const noexcept
    {
        return *m_key;
    }

    [[nodiscard]]
    constexpr std::size_t hash() const noexcept
    {
        return m_hash;
    }

    // Types of one name in unnamed namespaces of two translation units share a hash value, so
    // the keys decide a match.
    friend constexpr bool operator==(hash_key const & left, hash_key const & right) noexcept
    {
        return left.key() == right.key();
    }

private:
    scl::type_key const * m_key;
    std::size_t m_hash;
};

template <>
struct std::hash<hash_key>
{
    std::size_t operator()(hash_key const & key) const noexcept { return key.hash(); }
};
```

<!-- snippet: example/meta/type_key/meta_type_key_example.cpp registry -->
```cpp
    std::unordered_map<hash_key, std::string> registry;
    registry.emplace(hash_key::of<duck>(), "duck");
    registry.emplace(hash_key::of<goose>(), "goose");

    auto const lookup = [&registry](hash_key const & key) -> std::string_view {
        auto const found = registry.find(key);
        return found == registry.end() ? "<unknown>" : std::string_view{found->second};
    };

    std::cout << lookup(hash_key::of<duck>()) << '\n';  // duck
    std::cout << lookup(hash_key::of<goose>()) << '\n'; // goose
    std::cout << lookup(hash_key::of<int>()) << '\n';   // <unknown>
```

## See also

- [`scl::is_tu_local<T>`](is_tu_local.md), which tells whether a type is local to its translation unit
- [`example/meta/type_key/meta_type_key_example.cpp`](../../../../example/meta/type_key/meta_type_key_example.cpp)

---

Previous: [`is_tu_local<T>`](is_tu_local.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
