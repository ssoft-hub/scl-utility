#pragma once

/**
 * @file byte_view.h
 * @brief Spells a range of wider elements as the bytes the ScL hash functions take.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/nodiscard.h>
#include <scl/utility/hash/concepts.h>

#include <bit>
#include <cstddef>
#include <cstdint>
#include <ranges>
#include <type_traits>
#include <utility>

namespace scl::hash
{
    namespace detail
    {
        template <concepts::integer_element Element>
        SCL_NODISCARD constexpr ::std::uint8_t
        byte_of(Element const value, ::std::size_t const index) noexcept
        {
            if constexpr (::std::is_enum_v<Element>)
            {
                return byte_of(static_cast<::std::underlying_type_t<Element>>(value), index);
            }
            else if constexpr (sizeof(Element) == 1)
            {
                return ::std::bit_cast<::std::uint8_t>(value);
            }
            else
            {
                using bits_type = ::std::make_unsigned_t<Element>;
                return static_cast<::std::uint8_t>((static_cast<bits_type>(value) >> (8U * index)) & 0xFFU);
            }
        }
    } // namespace detail

    /**
     * @brief Views @p range as the little-endian bytes of its elements.
     * @ingroup scl_utility_hash
     *
     * The hash functions take a range of byte-sized elements, so a range of wider ones
     * has to say which bytes it means before it can be hashed. This is that step, written
     * where a reader can see it:
     * @code
     * auto const value = scl::hash::fnv1a(scl::hash::byte_view(text));  // text is a
     *                                                                   // std::u16string_view
     * @endcode
     *
     * Each element contributes `sizeof(element)` bytes, least significant first, whatever
     * the platform's own byte order — two machines hash one input alike. A byte-sized
     * element passes through unchanged, so the view is also a no-op where none is needed.
     *
     * The result is a lazy view over @p range and reads through to it, so @p range must
     * outlive it.
     *
     * @tparam Range  A sized random-access range whose element satisfies
     *                @ref scl::hash::concepts::integer_element.
     * @param  range  Range to view. Must be a range a view can be taken of and kept —
     *                a `std::string_view`, a `std::span`, or a named container.
     * @return A `std::ranges::view` of `std::uint8_t`.
     *
     * @note A wide string spelled this way is a different input from its narrow
     *       counterpart: `byte_view(u"AB")` carries the zero bytes that `"AB"` does not.
     */
    template <::std::ranges::viewable_range Range>
    SCL_NODISCARD constexpr auto byte_view(Range && range SCL_LIFETIMEBOUND)
        requires ::std::ranges::random_access_range<Range> && ::std::ranges::sized_range<Range> &&
        concepts::integer_element<::std::ranges::range_value_t<Range>>
    {
        using element_type = ::std::ranges::range_value_t<Range>;

        auto elements = ::std::views::all(::std::forward<Range>(range));
        auto const count = static_cast<::std::size_t>(::std::ranges::size(elements));

        return ::std::views::iota(::std::size_t{0}, count * sizeof(element_type)) |
            ::std::views::transform([elements](::std::size_t const index) {
            return detail::byte_of(
                elements[static_cast<::std::ranges::range_difference_t<Range>>(index / sizeof(element_type))],
                index % sizeof(element_type));
        });
    }

} // namespace scl::hash
