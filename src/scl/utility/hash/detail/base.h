#pragma once

/**
 * @file base.h
 * @brief Shared input preparation for the ScL hash functions.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/nodiscard.h>

#include <concepts>
#include <cstdint>
#include <ranges>
#include <type_traits>

namespace scl::hash::detail
{
    // A wider code unit reaches the digest as its low byte alone, so a terminator rule
    // for it would tidy up a value the hash functions already lose.
    template <typename Type>
    concept character = ::std::same_as<Type, char> || ::std::same_as<Type, char8_t>;

    template <::std::ranges::range Range>
    SCL_NODISCARD SCL_FORCE_INLINE constexpr auto
    without_terminator(Range const & range SCL_LIFETIMEBOUND) noexcept
        requires ::std::convertible_to<::std::ranges::range_value_t<Range>, ::std::uint8_t>
    {
        auto const first = ::std::ranges::begin(range);
        auto last = ::std::ranges::end(range);

        // A character array carries a terminator its text does not; an array of any
        // other element type is data, where zero is a value like every other.
        if constexpr (::std::is_bounded_array_v<Range> && character<::std::ranges::range_value_t<Range>>)
        {
            if (first != last && static_cast<::std::uint8_t>(*(last - 1)) == 0U)
                --last;
        }

        return ::std::ranges::subrange{first, last};
    }
} // namespace scl::hash::detail
