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
     *                are one byte wide — e.g. a string literal, `std::string_view`,
     *                `std::string`, `std::span<std::byte>`, a byte vector. See
     *                @ref scl::hash::concepts::byte_element.
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
    [[nodiscard]]
    constexpr ::std::uint64_t sdbm(Range const & range, ::std::uint64_t h = 0ull)
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
    {
        auto const text = detail::without_terminator(range);
        return ::std::accumulate(::std::ranges::begin(text), ::std::ranges::end(text), h,
            [](::std::uint64_t acc, auto c) noexcept {
            return detail::as_byte(c) + (acc << 6) + (acc << 16) - acc;
        });
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
     * @param  h     Initial hash value, as the range overload takes it.
     * @return Hash value of @p data.
     *
     * @par Example
     * @code
     * constexpr char name[] = "event.started";
     * foo(::scl::hash::sdbm(name));                        // a constant, on every compiler
     *
     * char buffer[64];
     * auto const size = receive(buffer);
     * foo(::scl::hash::sdbm(::std::string_view{buffer, size}));   // run time
     * @endcode
     */
    // NOLINTBEGIN(*-avoid-c-arrays): a bounded array is what a literal is
    template <::scl::hash::concepts::byte_element Element, ::std::size_t Size>
    [[nodiscard]]
    consteval ::std::uint64_t sdbm(Element const (&data)[Size], ::std::uint64_t h = 0ull)
    {
        // Explicit, so the array stays bounded - what decides if a trailing zero is text.
        return ::scl::hash::sdbm<Element const[Size]>(data, h);
    }
    // NOLINTEND(*-avoid-c-arrays)

    /**
     * @brief Callable wrapper around @ref sdbm for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct sdbm_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            // Named explicitly: the argument is a parameter, so the consteval overload
            // would make this operator immediate and reject every run-time call.
            return ::scl::hash::sdbm<Range>(range);
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
