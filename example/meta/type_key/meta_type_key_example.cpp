/**
 * @example meta_type_key_example.cpp
 * @brief Demonstrates a user-side hashable handle for scl::type_key.
 *
 * scl::type_key ships without hashing support on purpose: the right hash value
 * depends on the consumer. This example shows the safe recipe — a small
 * copyable handle that caches a hash value derived from key.name() only, never
 * from addresses. Addresses differ between modules for the same type (and
 * between TUs for same-named anonymous-namespace types), so an address-based
 * hash value would break the "equal keys have equal hashes" contract that any
 * hash container relies on.
 */

#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/meta/type_key.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

// A hashable value-type handle for the non-copyable scl::type_key: keeps a
// pointer to the per-type constant and caches the name-derived hash value, so
// it is computed once per handle instead of on every container probe.
class hash_key
{
public:
    explicit constexpr hash_key(::scl::type_key const & key) noexcept
        : m_key{&key}
        , m_hash{static_cast<::std::size_t>(::scl::hash::fnv1a(key.name()))}
    {}

    template <typename T>
    [[nodiscard]]
    static constexpr hash_key of() noexcept
    {
        return hash_key{::scl::type_key_of<T>()};
    }

    [[nodiscard]]
    constexpr ::scl::type_key const & key() const noexcept
    {
        return *m_key;
    }

    [[nodiscard]]
    constexpr ::std::size_t hash() const noexcept
    {
        return m_hash;
    }

    // Equality delegates to the underlying keys: same-named anonymous-
    // namespace types from different TUs share a hash value (equal name strings),
    // so the hash value alone must never decide a match.
    friend constexpr bool operator==(hash_key const & left, hash_key const & right) noexcept
    {
        return left.key() == right.key();
    }

    friend constexpr bool operator==(hash_key const & left, ::scl::type_key const & right) noexcept
    {
        return left.key() == right;
    }

private:
    ::scl::type_key const * m_key;
    ::std::size_t m_hash;
};

template <>
struct std::hash<hash_key>
{
    ::std::size_t operator()(hash_key const & key) const noexcept
    {
        return key.hash(); // cached hash value, no recomputation per probe
    }
};

namespace
{
    struct duck
    {};
} // namespace

struct goose
{};

// ============================================================================
// Pattern 1 — the handle stays consistent with type_key identity
// ============================================================================

static void show_consistency()
{
    constexpr auto duck_key = hash_key::of<duck>();

    // Equal keys (same type) always produce the same hash value.
    ::std::cout << "duck hash stable:     " << (duck_key.hash() == hash_key::of<duck>().hash()) << '\n'; // 1

    // The handle compares against the type_key it was built from.
    ::std::cout << "duck matches its key: " << (duck_key == ::scl::type_key_of<duck>()) << '\n'; // 1

    // Distinct types render distinct names, so their hash values differ here.
    ::std::cout << "duck != goose hash:   " << (duck_key.hash() != hash_key::of<goose>().hash()) << '\n'; // 1
}

// ============================================================================
// Pattern 2 — the handle keys an unordered container directly
// ============================================================================

static void show_registry()
{
    // std::hash<hash_key> is specialized above, so the handle keys the map
    // as-is; hash collisions are resolved by the container itself through
    // operator==, which compares the underlying keys.
    ::std::unordered_map<hash_key, ::std::string> registry;
    registry.emplace(hash_key::of<duck>(), "duck");
    registry.emplace(hash_key::of<goose>(), "goose");

    auto const lookup = [&registry](hash_key const & key) -> ::std::string_view {
        auto const found = registry.find(key);
        return found == registry.end() ? "<unknown>" : ::std::string_view{found->second};
    };

    ::std::cout << "lookup duck:  " << lookup(hash_key::of<duck>()) << '\n';  // duck
    ::std::cout << "lookup goose: " << lookup(hash_key::of<goose>()) << '\n'; // goose
    ::std::cout << "lookup int:   " << lookup(hash_key::of<int>()) << '\n';   // <unknown>
}

int main(int, char **)
{
    ::std::cout << "=== Hash / equality consistency ===\n";
    show_consistency();

    ::std::cout << "\n=== Handle-keyed registry ===\n";
    show_registry();

    return {};
}
