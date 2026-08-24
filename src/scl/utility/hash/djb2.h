#pragma once

/**
 * @file djb2.h
 * @brief djb2a — Daniel J. Bernstein's hash (XOR variant), 64-bit.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/hash/constant_bytes.h>

#include <concepts>
#include <cstddef>
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
    [[nodiscard]]
    constexpr ::std::uint64_t djb2(Range const & range, ::std::uint64_t h = 5381ull)
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
    {
        auto const text = detail::without_terminator(range);
        return ::std::accumulate(::std::ranges::begin(text), ::std::ranges::end(text), h,
            [](::std::uint64_t acc, auto c) noexcept { return (acc * 33ull) ^ detail::as_byte(c); });
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
     * foo(::scl::hash::djb2(name));                        // a constant, on every compiler
     *
     * char buffer[64];
     * auto const size = receive(buffer);
     * foo(::scl::hash::djb2(::std::string_view{buffer, size}));   // run time
     * @endcode
     */
    // NOLINTBEGIN(*-avoid-c-arrays): a bounded array is what a literal is
    template <::scl::hash::concepts::byte_element Element, ::std::size_t Size>
    [[nodiscard]]
    consteval ::std::uint64_t djb2(Element const (&data)[Size], ::std::uint64_t h = 5381ull)
    {
        // Explicit, so the array stays bounded - what decides if a trailing zero is text.
        return ::scl::hash::djb2<Element const[Size]>(data, h);
    }
    // NOLINTEND(*-avoid-c-arrays)

    /**
     * @brief Hashes the bytes of a bounded array at translation time.
     * @ingroup scl_utility_hash
     *
     * @ref scl::hash::byte_view answers a @ref scl::hash::constant_bytes for a bounded
     * array, which is the shape a range of wider elements takes when the translation
     * already holds it. Hashing one is therefore a constant, on the same terms as the
     * overload above: the result is a constant or the program is ill-formed.
     *
     * @tparam Capacity  Bytes the array occupied, terminator included.
     * @param  bytes     Bytes to hash, as @ref scl::hash::byte_view spelled them.
     * @param  h     Initial hash value, as the range overload takes it.
     * @return Hash value of @p bytes.
     *
     * @par Example
     * @code
     * foo(::scl::hash::djb2(::scl::hash::byte_view(u"event.started")));  // a constant
     * @endcode
     */
    template <::std::size_t Capacity>
    [[nodiscard]]
    consteval ::std::uint64_t djb2(constant_bytes<Capacity> const & bytes, ::std::uint64_t h = 5381ull)
    {
        // Explicit, so this overload does not select itself.
        return ::scl::hash::djb2<constant_bytes<Capacity>>(bytes, h);
    }

    /**
     * @brief Callable wrapper around @ref djb2 for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct djb2_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            // Named explicitly: the argument is a parameter, so the consteval overload
            // would make this operator immediate and reject every run-time call.
            return ::scl::hash::djb2<Range>(range);
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
