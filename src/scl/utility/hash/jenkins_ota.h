#pragma once

/// @file jenkins_ota.h
/// @brief Jenkins one-at-a-time (OTA) hash algorithm, 32-bit.

#include <concepts>
#include <cstdint>
#include <ranges>
#include <utility>

namespace scl::hash
{
    /// @brief Computes a Jenkins one-at-a-time (OTA) 32-bit hash.
    ///
    /// Implements Bob Jenkins' one-at-a-time hash algorithm. Each byte is
    /// mixed into the accumulator with bit-shifts and XOR operations, followed
    /// by a finalization step that ensures full avalanche:
    /// @code
    ///   // Per-byte mixing:
    ///   h += static_cast<std::uint8_t>(c);
    ///   h += h << 10;
    ///   h ^= h >> 6;
    ///
    ///   // Finalization:
    ///   h += h << 3;
    ///   h ^= h >> 11;
    ///   h += h << 15;
    /// @endcode
    ///
    /// @note Jenkins OAT is a **32-bit** algorithm by definition. It returns
    ///       `std::uint32_t`, unlike the 64-bit algorithms in this module.
    ///       Use @ref scl::hash::fnv1a or @ref scl::hash::djb2 when a 64-bit
    ///       digest is required.
    ///
    /// The function is `constexpr`, allowing compile-time hash computation.
    ///
    /// @tparam Range  Any type satisfying `std::ranges::range` whose elements
    ///                are convertible to `std::uint8_t` — e.g. a string literal,
    ///                `std::string_view`, `std::string`, `std::span<std::byte>`.
    /// @param  range  Input range to hash.
    /// @note   String literals (e.g. `"hello"`) include the null terminator in the
    ///         hash. Use `std::string_view{"hello"}` to hash only the characters.
    /// @return 32-bit Jenkins OAT digest of the input range.
    ///
    /// @par Compile-time example
    /// @code
    /// constexpr auto h = scl::hash::jenkins_ota("hello");
    /// static_assert(h != 0);
    /// @endcode
    template <::std::ranges::range Range>
        requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
    constexpr ::std::uint32_t jenkins_ota(Range const & range)
    {
        ::std::uint32_t h = 0;

        for (auto const c : range)
        {
            h += static_cast<::std::uint8_t>(c);
            h += h << 10;
            h ^= h >> 6;
        }

        h += h << 3;
        h ^= h >> 11;
        h += h << 15;

        return h;
    }

    /// @brief Callable wrapper around @ref jenkins_ota for use with @ref scl::hash::key.
    /// @note `value_type` deduced as `std::uint32_t` (Jenkins OAT is 32-bit).
    struct jenkins_ota_hasher
    {
        using result_type = ::std::uint32_t;

        template <::std::ranges::range Range>
            requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
        constexpr result_type operator()(Range const & range) const noexcept
        {
            return ::scl::hash::jenkins_ota(range);
        }
    };

} // namespace scl::hash
