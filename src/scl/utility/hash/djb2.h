#pragma once

/**
 * @file djb2.h
 * @brief djb2a — Daniel J. Bernstein's hash (XOR variant), 64-bit.
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
     * @brief Computes a djb2a 64-bit hash over a byte range.
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
     *       #include <scl/utility/hash/djb2.h>
     *
     *       #include <cstdint>
     *       #include <string_view>
     *
     *       using namespace std::string_view_literals;
     *
     *       std::uint64_t chained()
     *       {
     *           auto const h = scl::hash::djb2("foo"sv);
     *           return scl::hash::djb2("bar"sv, h);   // == djb2("foobar"sv)
     *       }
     *       @endcode
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
     * @param  h      Initial hash value. Defaults to the traditional djb2
     *                seed `5381`.
     * @return 64-bit djb2a hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * #include <scl/utility/hash/djb2.h>
     *
     * #include <string_view>
     *
     * using namespace std::string_view_literals;
     *
     * constexpr auto h = scl::hash::djb2("hello"sv);
     * static_assert(h != 0);
     * @endcode
     */
    template <::scl::hash::concepts::hashable_range Range>
    [[nodiscard]]
    constexpr ::std::uint64_t djb2(Range && range, ::std::uint64_t h = 5381ull)
    {
        for (::std::ranges::range_value_t<Range> const c :
            ::scl::hash::detail::read_only(::std::forward<Range>(range)))
            // cppcheck-suppress useStlAlgorithm
            h = (h * 33ull) ^ ::scl::hash::detail::as_byte(c);
        return h;
    }

    /**
     * @brief Callable wrapper around @ref djb2 for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct djb2_hasher
    {
        using result_type = ::std::uint64_t;

        template <::scl::hash::concepts::hashable_range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range && range) const noexcept
        {
            return ::scl::hash::djb2(::std::forward<Range>(range));
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
 * @fn scl::hash::djb2_hasher::operator()(Range && range) const
 * @brief Hashes @p range with @ref scl::hash::djb2 and its default seed.
 *
 * @tparam Range  Any type satisfying @ref scl::hash::concepts::hashable_range - a range of
 *                trivially copyable non-empty elements one byte wide that is not a bounded
 *                array.
 * @param  range  Input range to hash.
 * @return 64-bit djb2a hash value of @p range.
 */
