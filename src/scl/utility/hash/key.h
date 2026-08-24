#pragma once

/**
 * @file
 * @brief Strongly-typed hash value parameterized by a hash function.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/hash/concepts.h>
#include <scl/utility/hash/siphash.h>

#include <concepts>
#include <cstddef>
#include <functional>
#include <type_traits>
#include <utility>

namespace scl::hash::concepts
{
    /**
     * @brief Concept satisfied by any default-constructible callable that maps
     *        a byte range to an integral hash value and exposes a `result_type` typedef.
     * @ingroup scl_utility_hash
     *
     * Used to constrain the `Hasher` parameter of @ref scl::hash::key.
     */
    // clang-format off
    template <typename H>
    concept byte_hasher = ::std::default_initializable<H>
            && requires { typename H::result_type; }
            && ::std::integral<typename H::result_type>;
    // clang-format on
} // namespace scl::hash::concepts

namespace scl::hash
{
    /**
     * @brief Strongly-typed hash value parameterized by a hash function.
     * @ingroup scl_utility_hash
     *
     * Wraps the hash value produced by @p Hasher in a named type, preventing
     * accidental mixing of raw integers with hash values. The `value_type` and
     * width of the hash value are derived automatically from the hasher's return type:
     *
     * | Hasher                | `value_type`        |
     * |-----------------------|---------------------|
     * | `fnv1a_hasher`        | `std::uint64_t`     |
     * | `djb2_hasher`         | `std::uint64_t`     |
     * | `sdbm_hasher`         | `std::uint64_t`     |
     * | `siphash_hasher<Key>` | `std::uint64_t`     |
     * | `jenkins_ota_hasher`  | `std::uint32_t`     |
     *
     * Key properties:
     * - **`constexpr`** — hash value computed at compile time.
     * - **Comparable** — `==`, `!=`, `<`, `<=`, `>`, `>=` via defaulted `<=>`.
     * - **`switch`/`case` label** — implicit conversion to `value_type` enables
     *   string-dispatching without `if`-`else` chains.
     * - **STL-compatible** — `std::hash<key<Hasher>>` is specialised for use in
     *   `std::unordered_map` / `std::unordered_set`.
     *
     * @tparam Hasher  A type satisfying @ref scl::hash::concepts::byte_hasher.
     *                 Defaults to `fnv1a_hasher`.
     *
     * @par Compile-time example (default FNV-1a hasher)
     * @code
     * constexpr scl::hash::key id{"my_event"};
     * static_assert(id == scl::hash::key{"my_event"});
     * static_assert(id != scl::hash::key{"other"});
     * @endcode
     *
     * @par Compile-time example (SipHash hasher)
     * @code
     * constexpr scl::hash::siphash_key my_key{0xdeadbeefull, 0xcafebabeull};
     * using sip_key = scl::hash::key<scl::hash::siphash_hasher<my_key>>;
     * constexpr sip_key id{"my_event"};
     * @endcode
     *
     * @par Switch/case dispatching
     * @code
     * int handle(std::string_view command) {
     *     switch (scl::hash::key<>{command}) {
     *         case scl::hash::key<>{"start"}: return 1;
     *         case scl::hash::key<>{"stop"}:  return 2;
     *         default: return 0;
     *     }
     * }
     * @endcode
     *
     * @note A key holds the hash of the text it is given, whatever spells it: a character
     *       array's terminating zero is not part of that text, so `key<>{"start"}` equals
     *       the key built from `std::string_view{"start"}` or `std::string`. An array of
     *       any other element type is data and keeps every byte, a trailing zero included.
     */
    template <concepts::byte_hasher Hasher = siphash_hasher<>>
    struct key
    {
        using hasher_type = Hasher;
        using value_type = Hasher::result_type;

        value_type value{};

        template <::std::ranges::range Range>
        explicit constexpr key(Range const & range) noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
            : value{Hasher{}(range)}
        {}

        // NOLINTBEGIN(*-avoid-c-arrays): a bounded array is what a literal is
        template <::scl::hash::concepts::byte_element Element, ::std::size_t Size>
        explicit consteval key(Element const (&data)[Size]) noexcept
            : value{Hasher{}(data)}
        {}
        // NOLINTEND(*-avoid-c-arrays)

        [[nodiscard]]
        constexpr operator value_type() const noexcept
        {
            return value;
        }

        [[nodiscard]]
        friend constexpr auto operator<=>(key const &, key const &) noexcept = default;
    };

} // namespace scl::hash

/**
 * @brief `std::hash` partial specialisation for all `scl::hash::key<Hasher>`.
 * @ingroup scl_utility_hash
 */
template <typename Hasher>
// NOLINTNEXTLINE(bugprone-std-namespace-modification)
struct std::hash<::scl::hash::key<Hasher>>
{
    [[nodiscard]]
    constexpr ::std::size_t operator()(::scl::hash::key<Hasher> const & k) const noexcept
    {
        return ::std::hash<typename ::scl::hash::key<Hasher>::value_type>{}(k.value);
    }
};

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::hash::key::hasher_type
 * @brief The hasher type used to produce this hash value.
 */

/**
 * @typedef scl::hash::key::value_type
 * @brief Underlying integer type — deduced from the hasher's `result_type`.
 */

/**
 * @var scl::hash::key::value
 * @brief The hash value itself.
 */

/**
 * @fn scl::hash::key::key(Element const (&data)[Size])
 * @brief Constructs the hash value from a bounded array at translation time.
 *
 * A bounded array is content the translation already holds, so the key built from one is
 * a constant or the program is ill-formed. A sequence known only at run time is spelled
 * as a view, which selects the constructor below; neither spelling changes the value.
 *
 * @tparam Element  Element type, one byte wide.
 * @tparam Size     Number of elements the array holds.
 * @param  data     Array to hash. A trailing zero in an array of `char` or `char8_t` is a
 *                  string terminator and is not hashed; in an array of any other element
 *                  type it is a byte of data and is.
 */

/**
 * @fn scl::hash::key::key(Range const & range)
 * @brief Constructs the hash value by hashing @p range with @p Hasher.
 *
 * @tparam Range  Any type satisfying `std::ranges::range` whose elements are one byte
 *                wide. See @ref scl::hash::concepts::byte_element.
 * @param  range  Input range (e.g. `std::string`, `std::span<std::byte>`).
 */

/**
 * @fn scl::hash::key::operator value_type() const
 * @brief Implicit conversion to the raw integer hash value.
 * @return The digest `Hasher` produced for the range this key was built from, which is
 *         what lets a key stand where an integer is expected.
 */

/**
 * @fn scl::hash::key::operator<=>(key const &, key const &)
 * @brief Three-way comparison (generates ==, !=, <, <=, >, >=).
 */

/**
 * @fn std::hash< ::scl::hash::key< Hasher > >::operator()(::scl::hash::key< Hasher > const & k) const
 * @brief Returns the hash value already held by @p k, narrowed to `std::size_t`.
 *
 * The stored value is the hash: `key` is produced by a hasher, so rehashing it would
 * only cost work without adding distribution.
 *
 * @param  k  Key whose hash value is returned.
 * @return The stored hash value as `std::size_t`.
 */
