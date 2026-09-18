#pragma once

/**
 * @file sdbm.h
 * @brief SDBM hash algorithm, 64-bit.
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
     * @brief Computes an SDBM 64-bit hash over a byte range.
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
     *       #include <scl/utility/hash/sdbm.h>
     *
     *       #include <cstdint>
     *       #include <string_view>
     *
     *       using namespace std::string_view_literals;
     *
     *       std::uint64_t chained()
     *       {
     *           auto const h = scl::hash::sdbm("foo"sv);
     *           return scl::hash::sdbm("bar"sv, h);   // == sdbm("foobar"sv)
     *       }
     *       @endcode
     *
     * @tparam Range  Any type satisfying @ref scl::hash::concepts::hashable_range - a range
     *                of trivially copyable non-empty elements one byte wide that is not a
     *                bounded array.
     *                `std::string_view`, `std::string`, `std::span<std::byte>` and a byte
     *                vector are such types.
     * @param  range  Input range to hash.
     * @note   The bytes the range spans are the bytes hashed, and an array is refused.
     *         The caller should name the bytes with a `std::string_view` object - `"text"sv` is
     *         one -
     *         or with a `std::span` object. See @ref scl::hash::concepts::hashable_range for
     *         the reason and for what naming a partly filled buffer takes.
     * @param  h      Initial hash value. Defaults to `0`.
     * @return 64-bit SDBM hash value of the input range.
     *
     * @par Compile-time example
     * @code
     * #include <scl/utility/hash/sdbm.h>
     *
     * #include <string_view>
     *
     * using namespace std::string_view_literals;
     *
     * constexpr auto h = scl::hash::sdbm("hello"sv);
     * static_assert(h != 0);
     * @endcode
     */
    template <::scl::hash::concepts::hashable_range Range>
    [[nodiscard]]
    constexpr ::std::uint64_t sdbm(Range && range, ::std::uint64_t h = 0ull) /**/
        noexcept(::scl::hash::detail::nothrow_traversable<Range>)
    {
        for (::std::ranges::range_value_t<Range> const c :
            ::scl::hash::detail::read_only(::std::forward<Range>(range)))
            // cppcheck-suppress useStlAlgorithm
            h = ::scl::hash::detail::as_byte(c) + (h << 6) + (h << 16) - h;
        return h;
    }

    /**
     * @brief Callable wrapper around @ref sdbm for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct sdbm_hasher
    {
        using result_type = ::std::uint64_t;

        template <::scl::hash::concepts::hashable_range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range && range) const /**/
            noexcept(noexcept(::scl::hash::sdbm(::std::forward<Range>(range))))
        {
            return ::scl::hash::sdbm(::std::forward<Range>(range));
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
 * @fn scl::hash::sdbm_hasher::operator()(Range && range) const
 * @brief Hashes @p range with @ref scl::hash::sdbm and its default seed.
 *
 * @tparam Range  Any type satisfying @ref scl::hash::concepts::hashable_range - a range of
 *                trivially copyable non-empty elements one byte wide that is not a bounded
 *                array.
 * @param  range  Input range to hash.
 * @return 64-bit sdbm hash value of @p range.
 */
