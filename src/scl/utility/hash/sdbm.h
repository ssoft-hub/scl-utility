#pragma once

/**
 * @file sdbm.h
 * @brief SDBM hash algorithm, 64-bit.
 * @ingroup scl_utility_hash
 */

#include <concepts>
#include <cstdint>
#include <numeric>
#include <ranges>

#include "detail/base.h"

namespace scl::hash
{
    /**
     * @brief Computes an SDBM 64-bit hash over an arbitrary byte range.
     * @ingroup scl_utility_hash
     *
     * Implements the SDBM hash algorithm (originating from the sdbm database
     * library). For each element @c c:
     * @code
     *   h = static_cast<std::uint8_t>(c) + (h << 6) + (h << 16) - h;
     * @endcode
     *
     * SDBM is known for good distribution across short keys and is widely
     * used in database and compiler applications.
     *
     * The function is `constexpr`, allowing compile-time hash computation.
     *
     * @note **Chaining:** pass the result of a previous call as @p h:
     *       @code
     *       auto h = scl::hash::sdbm(first_range);
     *       h     = scl::hash::sdbm(second_range, h);
     *       @endcode
     *
     * @tparam Range  Any type satisfying `std::ranges::range` whose elements
     *                are convertible to `std::uint8_t` — e.g. a string literal,
     *                `std::string_view`, `std::string`, `std::span<std::byte>`.
     * @param  range  Input range to hash.
     * @note   The text is hashed, however it is spelled: a character array's terminating
     *         zero is left out, so `sdbm("hello")` equals `sdbm(std::string_view{"hello"})`.
     *         An array that does not end in zero, and an array of any other element type,
     *         is hashed whole — a zero byte is data there, not a terminator.
     * @param  h      Initial hash value. Defaults to `0`.
     * @return 64-bit SDBM hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * constexpr auto h = scl::hash::sdbm("hello");
     * static_assert(h != 0);
     * @endcode
     */
    template <::std::ranges::range Range>
    constexpr ::std::uint64_t sdbm(Range const & range, ::std::uint64_t h = 0ull)
        requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
    {
        auto const text = detail::without_terminator(range);
        return ::std::accumulate(::std::ranges::begin(text), ::std::ranges::end(text), h,
            [](::std::uint64_t acc, auto c) noexcept {
            return static_cast<::std::uint8_t>(c) + (acc << 6) + (acc << 16) - acc;
        });
    }

    /**
     * @brief Callable wrapper around @ref sdbm for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct sdbm_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
        {
            return ::scl::hash::sdbm(range);
        }
    };

} // namespace scl::hash

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::hash::sdbm_hasher::result_type
 * @brief Hash value type produced by this hasher — `std::uint64_t`.
 */

/**
 * @fn scl::hash::sdbm_hasher::operator()(Range const & range) const
 * @brief Hashes @p range with @ref scl::hash::sdbm and its default seed.
 *
 * @tparam Range  Any type satisfying `std::ranges::range` whose elements are
 *                convertible to `std::uint8_t`.
 * @param  range  Input range to hash.
 * @return 64-bit sdbm hash value of @p range.
 */
