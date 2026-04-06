#pragma once

/// @file djb2.h
/// @brief djb2a — Daniel J. Bernstein's hash (XOR variant), 64-bit.

#include <concepts>
#include <cstdint>
#include <numeric>
#include <ranges>

namespace scl::hash
{
    /// @brief Computes a djb2a 64-bit hash over an arbitrary byte range.
    ///
    /// Implements the XOR variant of Daniel J. Bernstein's djb2 hash
    /// (sometimes written as djb2a or "xor" variant). For each element @c c:
    /// @code
    ///   h = h * 33 ^ static_cast<std::uint8_t>(c);
    ///   // equivalently: h = ((h << 5) + h) ^ c
    /// @endcode
    ///
    /// djb2a differs from plain djb2 (which uses `+` instead of `^`) in that
    /// the XOR step provides better avalanche behaviour for similar inputs.
    ///
    /// The function is `constexpr`, allowing compile-time hash computation.
    ///
    /// @note **Chaining:** pass the result of a previous call as @p h:
    ///       @code
    ///       auto h = scl::hash::djb2(first_range);
    ///       h     = scl::hash::djb2(second_range, h);
    ///       @endcode
    ///
    /// @tparam Range  Any type satisfying `std::ranges::range` whose elements
    ///                are convertible to `std::uint8_t` — e.g. a string literal,
    ///                `std::string_view`, `std::string`, `std::span<std::byte>`.
    /// @param  range  Input range to hash.
    /// @note   String literals (e.g. `"hello"`) include the null terminator in the
    ///         hash. Use `std::string_view{"hello"}` to hash only the characters.
    /// @param  h      Initial hash value. Defaults to the traditional djb2
    ///                seed `5381`.
    /// @return 64-bit djb2a digest of the input range.
    ///
    /// @par Compile-time example
    /// @code
    /// constexpr auto h = scl::hash::djb2("hello");
    /// static_assert(h != 0);
    /// @endcode
    template <::std::ranges::range Range>
    constexpr ::std::uint64_t djb2(Range const & range, ::std::uint64_t h = 5381ull)
        requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
    {
        return ::std::accumulate(::std::ranges::begin(range), ::std::ranges::end(range), h,
            [](::std::uint64_t acc, auto c) noexcept {
            return (acc * 33ull) ^ static_cast<::std::uint8_t>(c);
        });
    }

    /// @brief Callable wrapper around @ref djb2 for use with @ref scl::hash::key.
    struct djb2_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
        {
            return ::scl::hash::djb2(range);
        }
    };

} // namespace scl::hash
