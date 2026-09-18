#pragma once

/**
 * @file base.h
 * @brief Shared input preparation for the ScL hash functions.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/inline.h>
#include <scl/utility/hash/concepts.h>

#include <bit>
#include <concepts>
#include <cstdint>
#include <ranges>
#include <type_traits>
#include <utility>

namespace scl::hash::detail
{
    /**
     * Satisfied when @p Range can be traversed through a reference to a constant, and its
     * element is the same one when it is. A range answering a wider element to a constant
     * traversal than to a mutable one is left to the mutable one, because the constraint that
     * accepted it weighed the mutable element, and the loop reads by that element type, so
     * a wider one would reach the hash value narrowed to it.
     */
    template <typename Range>
    concept same_element_as_const = ::std::ranges::range<Range const> &&
        ::std::same_as<::std::ranges::range_value_t<Range const>, ::std::ranges::range_value_t<Range>>;

    /**
     * Answers @p range through a reference to a constant where the range admits one, and as it
     * stands where it does not. A container whose copies share one buffer separates that
     * buffer on a read through a non-constant reference, and a hash function only reads. What an accepted range hands out is its own business: a view over a
     * mutable object reaches that object mutably however the view itself is qualified.
     */
    template <typename Range>
    [[nodiscard]]
    constexpr decltype(auto) read_only(Range && range) noexcept
    {
        if constexpr (::scl::hash::detail::same_element_as_const<::std::remove_reference_t<Range>>)
            return ::std::as_const(range);
        else
            return ::std::forward<Range>(range);
    }

    template <::scl::hash::concepts::byte_element Element>
    [[nodiscard]]
    SCL_FORCE_INLINE constexpr ::std::uint8_t as_byte(Element const value) noexcept
    {
        return ::std::bit_cast<::std::uint8_t>(value);
    }

} // namespace scl::hash::detail
