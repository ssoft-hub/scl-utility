#pragma once

/**
 * @file base.h
 * @brief Shared input preparation for the ScL hash functions.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/hash/concepts.h>

#include <bit>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <type_traits>

namespace scl::hash::detail
{
    // Only these two spell text a terminator belongs to; std::byte and the one-byte
    // element types carry data, where a zero is a value like every other.
    template <typename Type>
    concept character = ::std::same_as<Type, char> || ::std::same_as<Type, char8_t>;

    template <::scl::hash::concepts::byte_element Element>
    [[nodiscard]]
    SCL_FORCE_INLINE constexpr ::std::uint8_t as_byte(Element const value) noexcept
    {
        return ::std::bit_cast<::std::uint8_t>(value);
    }

    template <::std::ranges::range Range>
    [[nodiscard]]
    SCL_FORCE_INLINE constexpr auto without_terminator(Range const & range SCL_LIFETIMEBOUND) noexcept
        requires ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>
    {
        auto const first = ::std::ranges::begin(range);
        auto last = ::std::ranges::end(range);

        // A character array carries a terminator its text does not; an array of any
        // other element type is data, where zero is a value like every other.
        if constexpr (::std::is_bounded_array_v<Range> && character<::std::ranges::range_value_t<Range>>)
        {
            if (first != last && as_byte(*(last - 1)) == 0U)
                --last;
        }

        return ::std::ranges::subrange{first, last};
    }
} // namespace scl::hash::detail
