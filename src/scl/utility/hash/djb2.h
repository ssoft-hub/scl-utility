#pragma once

/**
 * @file djb2.h
 * @brief djb2a — Daniel J. Bernstein's hash (XOR variant), 64-bit.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/nodiscard.h>

#include <concepts>
#include <cstdint>
#include <numeric>
#include <ranges>

#include "detail/base.h"

namespace scl::hash
{
    /**
     * @brief Computes a djb2a 64-bit hash over an arbitrary byte range.
     * @ingroup scl_utility_hash
     *
     * Implements the XOR variant of Daniel J. Bernstein's djb2 hash
     * (sometimes written as djb2a or "xor" variant). For each element @c c:
     * @code
     *   h = h * 33 ^ static_cast<std::uint8_t>(c);
     *   // equivalently: h = ((h << 5) + h) ^ c
     * @endcode
     *
     * djb2a differs from plain djb2 (which uses `+` instead of `^`) in that
     * the XOR step provides better avalanche behaviour for similar inputs.
     *
     * The function is `constexpr`, allowing compile-time hash computation.
     *
     * @note **Chaining:** pass the result of a previous call as @p h:
     *       @code
     *       auto h = scl::hash::djb2(first_range);
     *       h     = scl::hash::djb2(second_range, h);
     *       @endcode
     *
     * @tparam Range  Any type satisfying `std::ranges::range` whose elements
     *                are one byte wide — e.g. a string literal, `std::string_view`,
     *                `std::string`, `std::span<std::byte>`, a byte vector. See
     *                @ref scl::hash::concepts::byte_element.
     * @param  range  Input range to hash.
     * @note   The text is hashed, however it is spelled: a character array's terminating
     *         zero is left out, so `djb2("hello")` equals `djb2(std::string_view{"hello"})`.
     *         An array that does not end in zero, and an array of any other element type,
     *         is hashed whole — a zero byte is data there, not a terminator.
     * @param  h      Initial hash value. Defaults to the traditional djb2
     *                seed `5381`.
     * @return 64-bit djb2a hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * constexpr auto h = scl::hash::djb2("hello");
     * static_assert(h != 0);
     * @endcode
     */
    template <::std::ranges::range Range>
    SCL_NODISCARD SCL_FORCE_INLINE constexpr ::std::uint64_t
    djb2(Range const & range, ::std::uint64_t h = 5381ull)
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
    {
        auto const text = detail::without_terminator(range);
        return ::std::accumulate(::std::ranges::begin(text), ::std::ranges::end(text), h,
            [](::std::uint64_t acc, auto c) noexcept { return (acc * 33ull) ^ detail::as_byte(c); });
    }

    /**
     * @brief Callable wrapper around @ref djb2 for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct djb2_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        SCL_NODISCARD SCL_FORCE_INLINE constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            return ::scl::hash::djb2(range);
        }
    };

} // namespace scl::hash

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::hash::djb2_hasher::result_type
 * @brief Hash value type produced by this hasher — `std::uint64_t`.
 */

/**
 * @fn scl::hash::djb2_hasher::operator()(Range const & range) const
 * @brief Hashes @p range with @ref scl::hash::djb2 and its default seed.
 *
 * @tparam Range  Any type satisfying `std::ranges::range` whose elements are
 *                convertible to `std::uint8_t`.
 * @param  range  Input range to hash.
 * @return 64-bit djb2a hash value of @p range.
 */
