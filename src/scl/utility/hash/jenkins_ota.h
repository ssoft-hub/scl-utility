#pragma once

/**
 * @file jenkins_ota.h
 * @brief Jenkins one-at-a-time (OTA) hash algorithm, 32-bit.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/hash/constant_bytes.h>

#include <concepts>
#include <cstddef>
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
    [[nodiscard]]
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
     * @return Hash value of @p data.
     *
     * @par Example
     * @code
     * constexpr char name[] = "event.started";
     * foo(::scl::hash::jenkins_ota(name));                        // a constant, on every compiler
     *
     * char buffer[64];
     * auto const size = receive(buffer);
     * foo(::scl::hash::jenkins_ota(::std::string_view{buffer, size}));   // run time
     * @endcode
     */
    // NOLINTBEGIN(*-avoid-c-arrays): a bounded array is what a literal is
    template <::scl::hash::concepts::byte_element Element, ::std::size_t Size>
    [[nodiscard]]
    consteval ::std::uint32_t jenkins_ota(Element const (&data)[Size])
    {
        // Explicit, so the array stays bounded - what decides if a trailing zero is text.
        return ::scl::hash::jenkins_ota<Element const[Size]>(data);
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
     * @return Hash value of @p bytes.
     *
     * @par Example
     * @code
     * foo(::scl::hash::jenkins_ota(::scl::hash::byte_view(u"event.started")));  // a constant
     * @endcode
     */
    template <::std::size_t Capacity>
    [[nodiscard]]
    consteval ::std::uint32_t jenkins_ota(constant_bytes<Capacity> const & bytes)
    {
        // Explicit, so this overload does not select itself.
        return ::scl::hash::jenkins_ota<constant_bytes<Capacity>>(bytes);
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
        [[nodiscard]]
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            // Named explicitly: the argument is a parameter, so the consteval overload
            // would make this operator immediate and reject every run-time call.
            return ::scl::hash::jenkins_ota<Range>(range);
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
