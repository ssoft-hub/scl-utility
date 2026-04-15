#pragma once

/// @file fnv1a.h
/// @brief FNV-1a (Fowler-Noll-Vo, variant 1a) 64-bit hash algorithm.
/// @ingroup scl_utility_hash

#include <concepts>
#include <cstdint>
#include <ranges>
#include <utility>

namespace scl::hash
{
    /// @brief Computes an FNV-1a 64-bit hash over an arbitrary byte range.
    /// @ingroup scl_utility_hash
    ///
    /// Implements the FNV-1a variant of the Fowler-Noll-Vo non-cryptographic
    /// hash algorithm with 64-bit width. For each element @c c in the range,
    /// the algorithm applies:
    /// @code
    ///   h ^= static_cast<std::uint8_t>(c);
    ///   h *= FNV_prime;  // 1099511628211
    /// @endcode
    ///
    /// The function is `constexpr`, allowing compile-time hash computation.
    ///
    /// @note **Chaining:** pass the result of a previous call as @p h to hash
    ///       several ranges into a single digest:
    ///       @code
    ///       auto h = scl::hash::fnv1a(first_range);
    ///       h     = scl::hash::fnv1a(second_range, h);
    ///       @endcode
    ///
    /// @tparam Range  Any type satisfying `std::ranges::range` whose elements
    ///                are convertible to `std::uint8_t` — e.g. a string literal,
    ///                `std::string_view`, `std::string`, `std::span<std::byte>`.
    /// @param  range  Input range to hash.
    /// @note   String literals (e.g. `"hello"`) include the null terminator in the
    ///         hash. Use `std::string_view{"hello"}` to hash only the characters.
    /// @param  h      Initial hash value (offset basis).
    ///                Defaults to the standard FNV-1a 64-bit offset basis
    ///                `14695981039346656037` (`0xcbf29ce484222325`).
    ///                An empty range leaves @p h unchanged and returns it as-is.
    /// @return 64-bit FNV-1a digest of the input range.
    ///
    /// @par Compile-time example
    /// @code
    /// constexpr auto h = scl::hash::fnv1a("hello");
    /// static_assert(h != 0);
    /// @endcode
    template <::std::ranges::range Range>
    constexpr ::std::uint64_t fnv1a(Range const & range, ::std::uint64_t h = 14695981039346656037ull)
        requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
    {
        for (auto const c : range)
        {
            h ^= static_cast<::std::uint8_t>(c);
            h *= 1099511628211ull;
        }
        return h;
    }

    /// @brief Callable wrapper around @ref fnv1a for use with @ref scl::hash::key.
    /// @ingroup scl_utility_hash
    struct fnv1a_hasher
    {
        using result_type = ::std::uint64_t;

        template <::std::ranges::range Range>
        constexpr result_type operator()(Range const & range) const noexcept
            requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
        {
            return ::scl::hash::fnv1a(range);
        }
    };

} // namespace scl::hash
