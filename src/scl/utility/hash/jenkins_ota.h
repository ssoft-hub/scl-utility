#pragma once

/**
 * @file jenkins_ota.h
 * @brief Jenkins one-at-a-time (OTA) hash algorithm, 32-bit.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/hash/concepts.h>

#include <cstdint>
#include <ranges>
#include <utility>

#include "detail/base.h"

namespace scl::hash
{
    /**
     * @brief Computes a Jenkins one-at-a-time (OTA) 32-bit hash over a byte range.
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
     * @tparam Range  Any type satisfying @ref scl::hash::concepts::hashable_range - a range
     *                of single trivially copyable bytes that is not a bounded array.
     *                `std::string_view`, `std::string`, `std::span<std::byte>` and a byte
     *                vector are such types.
     * @param  range  Input range to hash.
     * @note   The bytes the range spans are the bytes hashed, and an array is refused.
     *         The caller names the bytes with a `std::string_view` - `"text"sv` is one -
     *         or with a `std::span`. See @ref scl::hash::concepts::hashable_range for
     *         the reason and for what naming a partly filled buffer takes.
     * @return 32-bit Jenkins OAT hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * #include <scl/utility/hash/jenkins_ota.h>
     *
     * #include <string_view>
     *
     * using namespace std::string_view_literals;
     *
     * constexpr auto h = scl::hash::jenkins_ota("hello"sv);
     * static_assert(h != 0);
     * @endcode
     */
    template <::scl::hash::concepts::hashable_range Range>
    [[nodiscard]]
    constexpr ::std::uint32_t jenkins_ota(Range const & range)
    {
        ::std::uint32_t h = 0;

        for (auto const c : range)
        {
            h += ::scl::hash::detail::as_byte(c);
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

        template <::scl::hash::concepts::hashable_range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range const & range) const noexcept
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
 * @tparam Range  Any type satisfying @ref scl::hash::concepts::hashable_range - a range of
 *                single trivially copyable bytes that is not a bounded array.
 * @param  range  Input range to hash.
 * @return 32-bit Jenkins one-at-a-time hash value of @p range.
 */
