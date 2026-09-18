#pragma once

/**
 * @file fnv1a.h
 * @brief FNV-1a (Fowler-Noll-Vo, variant 1a) 64-bit hash algorithm.
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
     * @brief Computes an FNV-1a 64-bit hash over a byte range.
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
     *       #include <scl/utility/hash/fnv1a.h>
     *
     *       #include <cstdint>
     *       #include <string_view>
     *
     *       using namespace std::string_view_literals;
     *
     *       std::uint64_t chained()
     *       {
     *           auto const h = scl::hash::fnv1a("foo"sv);
     *           return scl::hash::fnv1a("bar"sv, h);   // == fnv1a("foobar"sv)
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
     * @param  h      Initial hash value (offset basis).
     *                Defaults to the standard FNV-1a 64-bit offset basis
     *                `14695981039346656037` (`0xcbf29ce484222325`).
     *                An empty range leaves @p h unchanged and returns it as-is.
     * @return 64-bit FNV-1a hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * #include <scl/utility/hash/fnv1a.h>
     *
     * #include <string_view>
     *
     * using namespace std::string_view_literals;
     *
     * constexpr auto h = scl::hash::fnv1a("hello"sv);
     * static_assert(h != 0);
     * @endcode
     */
    template <::scl::hash::concepts::hashable_range Range>
    [[nodiscard]]
    constexpr ::std::uint64_t fnv1a(Range && range, ::std::uint64_t h = 14695981039346656037ull)
    {
        for (::std::ranges::range_value_t<Range> const c :
            ::scl::hash::detail::read_only(::std::forward<Range>(range)))
        {
            h ^= ::scl::hash::detail::as_byte(c);
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

        template <::scl::hash::concepts::hashable_range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range && range) const noexcept
        {
            return ::scl::hash::fnv1a(::std::forward<Range>(range));
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
 * @fn scl::hash::fnv1a_hasher::operator()(Range && range) const
 * @brief Hashes @p range with @ref scl::hash::fnv1a and its default offset basis.
 *
 * @tparam Range  Any type satisfying @ref scl::hash::concepts::hashable_range - a range of
 *                trivially copyable non-empty elements one byte wide that is not a bounded
 *                array.
 * @param  range  Input range to hash.
 * @return 64-bit FNV-1a hash value of @p range.
 */
