#pragma once

/**
 * @file siphash.h
 * @brief SipHash-2-4: 64-bit keyed hash designed for string hashing in hash tables.
 * @ingroup scl_utility_hash
 */

#include <bit>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <utility>

#include "detail/base.h"

namespace scl::hash
{
    /**
     * @brief 128-bit secret key for SipHash.
     * @ingroup scl_utility_hash
     *
     * For **hash-flooding protection** (e.g. in `std::unordered_map`), initialise
     * with a random value at program startup. For **compile-time identifiers** and
     * `switch`/`case` dispatch, a fixed key is sufficient.
     */
    struct siphash_key
    {
        ::std::uint64_t k0{};
        ::std::uint64_t k1{};
    };

    namespace detail
    {
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

    /**
     * @brief Default key for non-security-sensitive use.
     * @ingroup scl_utility_hash
     *
     * These are the standard test-vector key bytes from the SipHash paper
     * (Aumasson & Bernstein, 2012): bytes 0x00–0x0f in little-endian order.
     * **Do not use this key where hash-flooding resistance is required.**
     */
    inline constexpr siphash_key siphash_default_key{
        .k0 = 0x0706050403020100ull, // bytes 00..07
        .k1 = 0x0f0e0d0c0b0a0908ull  // bytes 08..0f
    };

    /**
     * @brief Computes a SipHash-2-4 64-bit hash over an arbitrary byte range.
     * @ingroup scl_utility_hash
     *
     * SipHash-2-4 (2 compression rounds, 4 finalization rounds) is a fast,
     * short-input hash with a 128-bit secret key. It was designed by Jean-Philippe
     * Aumasson and Daniel J. Bernstein specifically to replace ad-hoc string hash
     * functions in hash tables, providing both good distribution and resistance to
     * hash-flooding attacks.
     *
     * It is used as the default string hash in Python (≥ 3.4), Rust, Ruby, and Perl.
     *
     * The algorithm processes input in 8-byte (64-bit) little-endian blocks:
     * @code
     *   // Compression (c = 2 rounds per block):
     *   v3 ^= block;
     *   SipRound × 2;
     *   v0 ^= block;
     *
     *   // Finalization (d = 4 rounds):
     *   v2 ^= 0xff;
     *   SipRound × 4;
     *   return v0 ^ v1 ^ v2 ^ v3;
     * @endcode
     *
     * The function is `constexpr`, allowing compile-time hash computation.
     *
     * @tparam Range  Any type satisfying `std::ranges::range` whose elements
     *                are one byte wide — e.g. a string literal, `std::string_view`,
     *                `std::string`, `std::span<std::byte>`, a byte vector. See
     *                @ref scl::hash::concepts::byte_element.
     * @param  range  Input range to hash.
     * @note   The text is hashed, however it is spelled: a character array's terminating
     *         zero is left out, so `siphash("hello")` equals
     *         `siphash(std::string_view{"hello"})`. An array that does not end in zero,
     *         and an array of any other element type, is hashed whole — a zero byte is
     *         data there, not a terminator.
     * @param  key    128-bit secret key. Defaults to @ref siphash_default_key.
     *                For security-sensitive use, provide a randomly generated key.
     * @return 64-bit SipHash-2-4 hash value.
     *
     * @par Compile-time example
     * @code
     * constexpr scl::hash::siphash_key my_key{0xdeadbeefcafeull, 0xabad1deaull};
     * constexpr auto h = scl::hash::siphash("hello", my_key);
     * static_assert(h != 0);
     * @endcode
     *
     * @see https://www.131002.net/siphash/ — original paper and reference vectors
     */
    template <::std::ranges::range Range>
    [[nodiscard]]
    constexpr ::std::uint64_t siphash(Range const & range, siphash_key const key = siphash_default_key)
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
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

        for (auto const c : detail::without_terminator(range))
        {
            m |= static_cast<::std::uint64_t>(detail::as_byte(c)) << shift;
            shift += 8;
            ++len;

            // One iteration in eight completes a block, so this is the rarer branch.
            if (shift == 64) [[unlikely]] // full 8-byte block ready
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

    /**
     * @brief Hashes a bounded array at translation time.
     * @ingroup scl_utility_hash
     *
     * A bounded array is content the translation already holds - a string literal, or an
     * array declared with its contents. Hashing it is therefore a constant, and this
     * overload is the one that says so: its result is a constant or the program is
     * ill-formed. Without it, whether the loop survives to run time is the optimiser's
     * choice, and the three supported compilers do not make the same one.
     *
     * A sequence whose contents are known only at run time is spelled as a view -
     * `std::string_view` for a character element, `std::span` for any other - which is
     * what selects the range overload. Neither spelling changes the hash value.
     *
     * @tparam Element  Element type, one byte wide.
     * @tparam Size     Number of elements the array holds.
     * @param  data     Array to hash. A trailing zero in an array of `char` or `char8_t`
     *                  is a string terminator and is not hashed; in an array of any other
     *                  element type it is a byte of data and is.
     * @param  key   Key the hash is taken under, as the range overload takes it.
     * @return Hash value of @p data.
     *
     * @par Example
     * @code
     * constexpr char name[] = "event.started";
     * foo(::scl::hash::siphash(name));                        // a constant, on every compiler
     *
     * char buffer[64];
     * auto const size = receive(buffer);
     * foo(::scl::hash::siphash(::std::string_view{buffer, size}));   // run time
     * @endcode
     */
    // NOLINTBEGIN(*-avoid-c-arrays): a bounded array is what a literal is
    template <::scl::hash::concepts::byte_element Element, ::std::size_t Size>
    [[nodiscard]]
    consteval ::std::uint64_t
    siphash(Element const (&data)[Size], siphash_key const key = siphash_default_key)
    {
        // Explicit, so the array stays bounded - what decides if a trailing zero is text.
        return ::scl::hash::siphash<Element const[Size]>(data, key);
    }
    // NOLINTEND(*-avoid-c-arrays)

    /**
     * @brief Callable wrapper around @ref siphash for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     *
     * The secret key is embedded as a non-type template parameter, so two
     * instantiations with different keys are distinct types.
     *
     * @tparam Key  128-bit SipHash key. Defaults to @ref siphash_default_key.
     */
    template <siphash_key Key = siphash_default_key>
    struct siphash_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            // Named explicitly: the argument is a parameter, so the consteval overload
            // would make this operator immediate and reject every run-time call.
            return ::scl::hash::siphash<Range>(range, Key);
        }
    };

} // namespace scl::hash

// =============================================================================
// Documentation
// =============================================================================

/**
 * @var scl::hash::siphash_key::k0
 * @brief Low half of the 128-bit secret key.
 */

/**
 * @var scl::hash::siphash_key::k1
 * @brief High half of the 128-bit secret key.
 */

/**
 * @typedef scl::hash::siphash_hasher::result_type
 * @brief Hash value type produced by this hasher — `std::uint64_t`.
 */

/**
 * @fn scl::hash::siphash_hasher::operator()(Range const & range) const
 * @brief Hashes @p range with @ref scl::hash::siphash under the embedded key.
 *
 * @tparam Range  Any type satisfying `std::ranges::range` whose elements are
 *                convertible to `std::uint8_t`.
 * @param  range  Input range to hash.
 * @return 64-bit SipHash-2-4 hash value of @p range.
 */
