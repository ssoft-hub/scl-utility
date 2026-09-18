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
     * Satisfied when the five operations a range-for over @p Range performs, and the
     * conversion of a reference to the element type it reads by, all throw nothing. The rest
     * is excluded by the constraints standing beside this one on the same declaration, where
     * the concept std::ranges::range makes destroying an iterator or a sentinel nothrow and
     * the concept byte_element makes copying or destroying an element nothrow.
     *
     * The two accesses are weighed through the customisation point objects std::ranges::begin
     * and std::ranges::end, which carry a decay-copy of what they answer. A standard library
     * reading that copy into the specification answers false for an iterator whose move can
     * throw, where the loop initialises from a prvalue and moves nothing, so this concept
     * answers the stricter of the two readings.
     */
    template <typename Range>
    concept nothrow_iterable = ::std::ranges::range<Range> &&
        ::std::is_nothrow_convertible_v<::std::ranges::range_reference_t<Range>, ::std::ranges::range_value_t<Range>> &&
        requires(Range & range, ::std::ranges::iterator_t<Range> & position, ::std::ranges::sentinel_t<Range> & last) {
            {
                ::std::ranges::begin(range)
            } noexcept;
            {
                ::std::ranges::end(range)
            } noexcept;
            {
                position != last
            } noexcept;
            {
                ++position
            } noexcept;
            {
                *position
            } noexcept;
        };

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

    /// Satisfied when a hash function traversing @p Range as the function template @ref read_only
    /// hands it throws nothing.
    template <typename Range>
    concept nothrow_traversable = ::scl::hash::detail::nothrow_iterable<
        decltype(::scl::hash::detail::read_only(::std::declval<Range &&>()))>;

    template <::scl::hash::concepts::byte_element Element>
    [[nodiscard]]
    SCL_FORCE_INLINE constexpr ::std::uint8_t as_byte(Element const value) noexcept
    {
        return ::std::bit_cast<::std::uint8_t>(value);
    }

} // namespace scl::hash::detail
