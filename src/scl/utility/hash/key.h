#pragma once

/// @file
/// @brief Strongly-typed hash digest parameterized by a hash function.

#include <scl/utility/hash/siphash.h>

#include <concepts>
#include <functional>
#include <type_traits>
#include <utility>

namespace scl::hash::concepts
{
    /// @brief Concept satisfied by any default-constructible callable that maps
    ///        a byte range to an integral digest and exposes a `result_type` typedef.
    ///
    /// Used to constrain the `Hasher` parameter of @ref scl::hash::key.
    // clang-format off
    template <typename H>
    concept byte_hasher = ::std::default_initializable<H>
            && requires { typename H::result_type; }
            && ::std::integral<typename H::result_type>;
    // clang-format on
} // namespace scl::hash::concepts

namespace scl::hash
{
    /// @brief Strongly-typed FNV-1a hash digest parameterized by a hash function.
    ///
    /// Wraps the digest produced by @p Hasher in a named type, preventing
    /// accidental mixing of raw integers with hash values. The `value_type` and
    /// width of the digest are derived automatically from the hasher's return type:
    ///
    /// | Hasher                | `value_type`        |
    /// |-----------------------|---------------------|
    /// | `fnv1a_hasher`        | `std::uint64_t`     |
    /// | `djb2_hasher`         | `std::uint64_t`     |
    /// | `sdbm_hasher`         | `std::uint64_t`     |
    /// | `siphash_hasher<Key>` | `std::uint64_t`     |
    /// | `jenkins_ota_hasher`  | `std::uint32_t`     |
    ///
    /// Key properties:
    /// - **`constexpr`** — digest computed at compile time.
    /// - **Comparable** — `==`, `!=`, `<`, `<=`, `>`, `>=` via defaulted `<=>`.
    /// - **`switch`/`case` label** — implicit conversion to `value_type` enables
    ///   string-dispatching without `if`-`else` chains.
    /// - **STL-compatible** — `std::hash<key<Hasher>>` is specialised for use in
    ///   `std::unordered_map` / `std::unordered_set`.
    ///
    /// @tparam Hasher  A type satisfying @ref scl::hash::byte_hasher.
    ///                 Defaults to `fnv1a_hasher`.
    ///
    /// @par Compile-time example (default FNV-1a hasher)
    /// @code
    /// constexpr scl::hash::key id{"my_event"};
    /// static_assert(id == scl::hash::key{"my_event"});
    /// static_assert(id != scl::hash::key{"other"});
    /// @endcode
    ///
    /// @par Compile-time example (SipHash hasher)
    /// @code
    /// constexpr scl::hash::siphash_key my_key{0xdeadbeefULL, 0xcafebabeULL};
    /// using sip_key = scl::hash::key<scl::hash::siphash_hasher<my_key>>;
    /// constexpr sip_key id{"my_event"};
    /// @endcode
    ///
    /// @par Switch/case dispatching
    /// @code
    /// int handle(scl::hash::key<> cmd) {
    ///     switch (cmd) {
    ///         case scl::hash::key<>{"start"}: return 1;
    ///         case scl::hash::key<>{"stop"}:  return 2;
    ///         default: return 0;
    ///     }
    /// }
    /// @endcode
    template <concepts::byte_hasher Hasher = siphash_hasher<>>
    struct key
    {
        /// @brief The hasher type used to produce this digest.
        using hasher_type = Hasher;

        /// @brief Underlying integer type — deduced from the hasher's `result_type`.
        using value_type = typename Hasher::result_type;

        /// @brief Raw digest value.
        value_type value{};

        /// @brief Constructs the digest by hashing @p range with @p Hasher.
        ///
        /// @tparam Range  Any type satisfying `std::ranges::range` whose elements
        ///                are convertible to `std::uint8_t`.
        /// @param  range  Input range (e.g. `std::string`, `std::span<std::byte>`).
        template <::std::ranges::range Range>
            requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
        explicit constexpr key(Range && range) noexcept
            : value{Hasher{}(::std::forward<Range>(range))}
        {}

        /// @brief Implicit conversion to the raw integer digest.
        constexpr operator value_type() const noexcept { return value; }

        /// @brief Three-way comparison (generates ==, !=, <, <=, >, >=).
        friend constexpr auto operator<=>(key const &, key const &) noexcept = default;
    };

} // namespace scl::hash

/// @brief `std::hash` partial specialisation for all `scl::hash::key<Hasher>`.
template <typename Hasher>
struct std::hash<::scl::hash::key<Hasher>>
{
    constexpr ::std::size_t operator()(::scl::hash::key<Hasher> const & k) const noexcept
    {
        return ::std::hash<typename ::scl::hash::key<Hasher>::value_type>{}(k.value);
    }
};
