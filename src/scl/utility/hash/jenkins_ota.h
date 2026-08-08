#pragma once

/**
 * @file jenkins_ota.h
 * @brief Jenkins one-at-a-time (OTA) hash algorithm, 32-bit.
 * @ingroup scl_utility_hash
 */

#include <concepts>
#include <cstdint>
#include <ranges>
#include <utility>

#include "detail/base.h"

namespace scl::hash
{
    /**
     * @brief Computes a Jenkins one-at-a-time (OTA) 32-bit hash.
     * @ingroup scl_utility_hash
     *
     * Implements Bob Jenkins' one-at-a-time hash algorithm. Each byte is
     * mixed into the accumulator with bit-shifts and XOR operations, followed
     * by a finalization step that ensures full avalanche:
     * @code
     *   // Per-byte mixing:
     *   h += static_cast<std::uint8_t>(c);
     *   h += h << 10;
     *   h ^= h >> 6;
     *
     *   // Finalization:
     *   h += h << 3;
     *   h ^= h >> 11;
     *   h += h << 15;
     * @endcode
     *
     * @note Jenkins OAT is a **32-bit** algorithm by definition. It returns
     *       `std::uint32_t`, unlike the 64-bit algorithms in this module.
     *       Use @ref scl::hash::fnv1a or @ref scl::hash::djb2 when a 64-bit
     *       hash value is required.
     *
     * The function is `constexpr`, allowing compile-time hash computation.
     *
     * @tparam Range  Any type satisfying `std::ranges::range` whose elements
     *                are one byte wide — e.g. a string literal, `std::string_view`,
     *                `std::string`, `std::span<std::byte>`, a byte vector. See
     *                @ref scl::hash::concepts::byte_element.
     * @param  range  Input range to hash.
     * @note   The text is hashed, however it is spelled: a character array's terminating
     *         zero is left out, so `jenkins_ota("hello")` equals
     *         `jenkins_ota(std::string_view{"hello"})`. An array that does not end in
     *         zero, and an array of any other element type, is hashed whole — a zero
     *         byte is data there, not a terminator.
     * @return 32-bit Jenkins OAT hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * constexpr auto h = scl::hash::jenkins_ota("hello");
     * static_assert(h != 0);
     * @endcode
     */
    template <::std::ranges::range Range>
    constexpr ::std::uint32_t jenkins_ota(Range const & range)
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
    {
        ::std::uint32_t h = 0;

        for (auto const c : detail::without_terminator(range))
        {
            h += detail::as_byte(c);
            h += h << 10;
            h ^= h >> 6;
        }

        h += h << 3;
        h ^= h >> 11;
        h += h << 15;

        return h;
    }

    /**
     * @brief Callable wrapper around @ref jenkins_ota for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     * @note `value_type` deduced as `std::uint32_t` (Jenkins OAT is 32-bit).
     */
    struct jenkins_ota_hasher
    {
        using result_type = ::std::uint32_t;

        template <::std::ranges::range Range>
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            return ::scl::hash::jenkins_ota(range);
        }
    };

} // namespace scl::hash

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::hash::jenkins_ota_hasher::result_type
 * @brief Hash value type produced by this hasher — `std::uint32_t`, as Jenkins
 *        one-at-a-time is a 32-bit hash.
 */

/**
 * @fn scl::hash::jenkins_ota_hasher::operator()(Range const & range) const
 * @brief Hashes @p range with @ref scl::hash::jenkins_ota and its default seed.
 *
 * @tparam Range  Any type satisfying `std::ranges::range` whose elements are
 *                convertible to `std::uint8_t`.
 * @param  range  Input range to hash.
 * @return 32-bit Jenkins one-at-a-time hash value of @p range.
 */
