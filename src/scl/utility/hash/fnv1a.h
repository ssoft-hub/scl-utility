#pragma once

/**
 * @file fnv1a.h
 * @brief FNV-1a (Fowler-Noll-Vo, variant 1a) 64-bit hash algorithm.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/nodiscard.h>

#include <concepts>
#include <cstdint>
#include <ranges>
#include <utility>

#include "detail/base.h"

namespace scl::hash
{
    /**
     * @brief Computes an FNV-1a 64-bit hash over an arbitrary byte range.
     * @ingroup scl_utility_hash
     *
     * Implements the FNV-1a variant of the Fowler-Noll-Vo non-cryptographic
     * hash algorithm with 64-bit width. For each element @c c in the range,
     * the algorithm applies:
     * @code
     *   h ^= static_cast<std::uint8_t>(c);
     *   h *= FNV_prime;  // 1099511628211
     * @endcode
     *
     * The function is `constexpr`, allowing compile-time hash computation.
     *
     * @note **Chaining:** pass the result of a previous call as @p h to hash
     *       several ranges into a single hash value:
     *       @code
     *       auto h = scl::hash::fnv1a(first_range);
     *       h     = scl::hash::fnv1a(second_range, h);
     *       @endcode
     *
     * @tparam Range  Any type satisfying `std::ranges::range` whose elements
     *                are one byte wide — e.g. a string literal, `std::string_view`,
     *                `std::string`, `std::span<std::byte>`, a byte vector. See
     *                @ref scl::hash::concepts::byte_element.
     * @param  range  Input range to hash.
     * @note   The text is hashed, however it is spelled: a character array's terminating
     *         zero is left out, so `fnv1a("hello")` equals `fnv1a(std::string_view{"hello"})`.
     *         An array that does not end in zero, and an array of any other element type,
     *         is hashed whole — a zero byte is data there, not a terminator.
     * @param  h      Initial hash value (offset basis).
     *                Defaults to the standard FNV-1a 64-bit offset basis
     *                `14695981039346656037` (`0xcbf29ce484222325`).
     *                An empty range leaves @p h unchanged and returns it as-is.
     * @return 64-bit FNV-1a hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * constexpr auto h = scl::hash::fnv1a("hello");
     * static_assert(h != 0);
     * @endcode
     */
    template <::std::ranges::range Range>
    SCL_NODISCARD SCL_FORCE_INLINE constexpr ::std::uint64_t
    fnv1a(Range const & range, ::std::uint64_t h = 14695981039346656037ull)
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
    {
        for (auto const c : detail::without_terminator(range))
        {
            h ^= detail::as_byte(c);
            h *= 1099511628211ull;
        }
        return h;
    }

    /**
     * @brief Callable wrapper around @ref fnv1a for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct fnv1a_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        SCL_NODISCARD SCL_FORCE_INLINE constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            return ::scl::hash::fnv1a(range);
        }
    };

} // namespace scl::hash

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::hash::fnv1a_hasher::result_type
 * @brief Hash value type produced by this hasher — `std::uint64_t`.
 */

/**
 * @fn scl::hash::fnv1a_hasher::operator()(Range const & range) const
 * @brief Hashes @p range with @ref scl::hash::fnv1a and its default offset basis.
 *
 * @tparam Range  Any type satisfying `std::ranges::range` whose elements are
 *                convertible to `std::uint8_t`.
 * @param  range  Input range to hash.
 * @return 64-bit FNV-1a hash value of @p range.
 */
