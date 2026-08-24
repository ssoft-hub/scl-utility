#pragma once

/**
 * @file fnv1a.h
 * @brief FNV-1a (Fowler-Noll-Vo, variant 1a) 64-bit hash algorithm.
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
    [[nodiscard]]
    constexpr ::std::uint64_t fnv1a(Range const & range, ::std::uint64_t h = 14695981039346656037ull)
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
     * foo(::scl::hash::fnv1a(name));                        // a constant, on every compiler
     *
     * char buffer[64];
     * auto const size = receive(buffer);
     * foo(::scl::hash::fnv1a(::std::string_view{buffer, size}));   // run time
     * @endcode
     */
    // NOLINTBEGIN(*-avoid-c-arrays): a bounded array is what a literal is
    template <::scl::hash::concepts::byte_element Element, ::std::size_t Size>
    [[nodiscard]]
    consteval ::std::uint64_t
    fnv1a(Element const (&data)[Size], ::std::uint64_t h = 14695981039346656037ull)
    {
        // Explicit, so the array stays bounded - what decides if a trailing zero is text.
        return ::scl::hash::fnv1a<Element const[Size]>(data, h);
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
     * foo(::scl::hash::fnv1a(::scl::hash::byte_view(u"event.started")));  // a constant
     * @endcode
     */
    template <::std::size_t Capacity>
    [[nodiscard]]
    consteval ::std::uint64_t
    fnv1a(constant_bytes<Capacity> const & bytes, ::std::uint64_t h = 14695981039346656037ull)
    {
        // Explicit, so this overload does not select itself.
        return ::scl::hash::fnv1a<constant_bytes<Capacity>>(bytes, h);
    }

    /**
     * @brief Callable wrapper around @ref fnv1a for use with @ref scl::hash::key.
     * @ingroup scl_utility_hash
     */
    struct fnv1a_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        [[nodiscard]]
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
        {
            // Named explicitly: the argument is a parameter, so the consteval overload
            // would make this operator immediate and reject every run-time call.
            return ::scl::hash::fnv1a<Range>(range);
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
