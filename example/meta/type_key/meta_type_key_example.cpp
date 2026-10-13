/**
 * @example meta_type_key_example.cpp
 * @brief Compares types by their keys, and keys an unordered container with a handle of them.
 *
 * scl::type_key comes with no hash, since the right hash value depends on its consumer.
 * The handle below caches a hash value of key.name() alone, never of an address: the address of
 * the key of one type differs between modules, so a hash value of it would break the rule that
 * equal keys hash alike.
 */

#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/meta/type_key.h>

#include <cstddef>
#include <functional>
#include <iostream>
#include <string>
#include <string_view>
#include <unordered_map>

//! [type_key_of]
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
//! [type_key_of]

//! [hash_key]
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
//! [hash_key]

int main(int, char **)
{
    //! [registry]
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
    //! [registry]

    return {};
}
