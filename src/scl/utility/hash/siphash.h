#pragma once

/// @file siphash.h
/// @brief SipHash-2-4: 64-bit keyed hash designed for string hashing in hash tables.

#include <bit>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <utility>

namespace scl::hash
{
    /// @brief 128-bit secret key for SipHash.
    ///
    /// For **hash-flooding protection** (e.g. in `std::unordered_map`), initialise
    /// with a random value at program startup. For **compile-time identifiers** and
    /// `switch`/`case` dispatch, a fixed key is sufficient.
    struct siphash_key
    {
        ::std::uint64_t k0{};
        ::std::uint64_t k1{};
    };

    namespace detail
    {
        /// @brief Single SipHash mixing round.
        constexpr void
        sip_round(::std::uint64_t & v0, ::std::uint64_t & v1, ::std::uint64_t & v2, ::std::uint64_t & v3) noexcept
        {
            v0 += v1;
            v1 = ::std::rotl(v1, 13);
            v1 ^= v0;
            v0 = ::std::rotl(v0, 32);
            v2 += v3;
            v3 = ::std::rotl(v3, 16);
            v3 ^= v2;
            v0 += v3;
            v3 = ::std::rotl(v3, 21);
            v3 ^= v0;
            v2 += v1;
            v1 = ::std::rotl(v1, 17);
            v1 ^= v2;
            v2 = ::std::rotl(v2, 32);
        }
    } // namespace detail

    /// @brief Default key for non-security-sensitive use.
    ///
    /// These are the standard test-vector key bytes from the SipHash paper
    /// (Aumasson & Bernstein, 2012): bytes 0x00–0x0f in little-endian order.
    /// **Do not use this key where hash-flooding resistance is required.**
    inline constexpr siphash_key siphash_default_key{
        .k0 = 0x0706050403020100ull, // bytes 00..07
        .k1 = 0x0f0e0d0c0b0a0908ull  // bytes 08..0f
    };

    /// @brief Computes a SipHash-2-4 64-bit hash over an arbitrary byte range.
    ///
    /// SipHash-2-4 (2 compression rounds, 4 finalization rounds) is a fast,
    /// short-input hash with a 128-bit secret key. It was designed by Jean-Philippe
    /// Aumasson and Daniel J. Bernstein specifically to replace ad-hoc string hash
    /// functions in hash tables, providing both good distribution and resistance to
    /// hash-flooding attacks.
    ///
    /// It is used as the default string hash in Python (≥ 3.4), Rust, Ruby, and Perl.
    ///
    /// The algorithm processes input in 8-byte (64-bit) little-endian blocks:
    /// @code
    ///   // Compression (c = 2 rounds per block):
    ///   v3 ^= block;
    ///   SipRound × 2;
    ///   v0 ^= block;
    ///
    ///   // Finalization (d = 4 rounds):
    ///   v2 ^= 0xff;
    ///   SipRound × 4;
    ///   return v0 ^ v1 ^ v2 ^ v3;
    /// @endcode
    ///
    /// The function is `constexpr`, allowing compile-time hash computation.
    ///
    /// @tparam Range  Any type satisfying `std::ranges::range` whose elements
    ///                are convertible to `std::uint8_t` — e.g. a string literal,
    ///                `std::string_view`, `std::string`, `std::span<std::byte>`.
    /// @param  range  Input range to hash.
    /// @note   String literals (e.g. `"hello"`) include the null terminator in the
    ///         hash. Use `std::string_view{"hello"}` to hash only the characters.
    /// @param  key    128-bit secret key. Defaults to @ref siphash_default_key.
    ///                For security-sensitive use, provide a randomly generated key.
    /// @return 64-bit SipHash-2-4 digest.
    ///
    /// @par Compile-time example
    /// @code
    /// constexpr scl::hash::siphash_key my_key{0xdeadbeefcafeull, 0xabad1deaull};
    /// constexpr auto h = scl::hash::siphash("hello", my_key);
    /// static_assert(h != 0);
    /// @endcode
    ///
    /// @see https://www.131002.net/siphash/ — original paper and reference vectors
    template <::std::ranges::range Range>
        requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
    constexpr ::std::uint64_t siphash(Range const & range, siphash_key const key = siphash_default_key)
    {
        // State initialised from key XOR'd with magic constants spelling
        // "somepseudorandomlygeneratedbytes".
        ::std::uint64_t v0 = key.k0 ^ 0x736f6d6570736575ull;
        ::std::uint64_t v1 = key.k1 ^ 0x646f72616e646f6dull;
        ::std::uint64_t v2 = key.k0 ^ 0x6c7967656e657261ull;
        ::std::uint64_t v3 = key.k1 ^ 0x7465646279746573ull;

        ::std::uint64_t m = 0; // current 8-byte block (little-endian)
        ::std::size_t len = 0;
        int shift = 0; // bits filled in m (0, 8, 16, ..., 56)

        for (auto const c : range)
        {
            m |= static_cast<::std::uint64_t>(static_cast<::std::uint8_t>(c)) << shift;
            shift += 8;
            ++len;

            if (shift == 64) // full 8-byte block ready
            {
                v3 ^= m;
                detail::sip_round(v0, v1, v2, v3); // c = 2
                detail::sip_round(v0, v1, v2, v3);
                v0 ^= m;
                m = 0;
                shift = 0;
            }
        }

        // Last (partial) block: high byte encodes message length mod 256.
        m |= static_cast<::std::uint64_t>(len & 0xffull) << 56;
        v3 ^= m;
        detail::sip_round(v0, v1, v2, v3); // c = 2
        detail::sip_round(v0, v1, v2, v3);
        v0 ^= m;

        // Finalization: d = 4 rounds.
        v2 ^= 0xffull;
        detail::sip_round(v0, v1, v2, v3);
        detail::sip_round(v0, v1, v2, v3);
        detail::sip_round(v0, v1, v2, v3);
        detail::sip_round(v0, v1, v2, v3);

        return v0 ^ v1 ^ v2 ^ v3;
    }

    /// @brief Callable wrapper around @ref siphash for use with @ref scl::hash::key.
    ///
    /// The secret key is embedded as a non-type template parameter, so two
    /// instantiations with different keys are distinct types.
    ///
    /// @tparam Key  128-bit SipHash key. Defaults to @ref siphash_default_key.
    template <siphash_key Key = siphash_default_key>
    struct siphash_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
            requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
        constexpr result_type operator()(Range const & range) const noexcept
        {
            return ::scl::hash::siphash(range, Key);
        }
    };

} // namespace scl::hash
