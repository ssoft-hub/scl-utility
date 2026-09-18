#pragma once

/**
 * @file byte_view.h
 * @brief Spells a range of wider elements as the bytes the ScL hash functions take.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/hash/concepts.h>

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <utility>

#include "detail/base.h"

namespace scl::hash
{
    namespace detail
    {
        template <::std::ranges::input_range Base>
            requires(::std::ranges::view<Base>) &&
            (::scl::hash::concepts::integer_element<::std::ranges::range_value_t<Base>>)
        class byte_range : public ::std::ranges::view_interface<byte_range<Base>>
        {
        private:
            using element_type = ::std::ranges::range_value_t<Base>;

            /// The source as it is traversed, through a reference to a constant where that
            /// reads the same elements, and as it stands where it does not.
            using source_type =
                ::std::conditional_t<::scl::hash::detail::same_element_as_const<Base>, Base const, Base>;

            using bits_type = ::scl::hash::detail::element_bits_t<element_type>;

            class cursor
            {
            public:
                using value_type = ::std::uint8_t;
                using difference_type = ::std::ptrdiff_t;
                using iterator_concept = ::std::conditional_t<::std::ranges::forward_range<source_type>,
                    ::std::forward_iterator_tag,
                    ::std::input_iterator_tag>;
                // The reference is a prvalue, which the legacy category cannot express.
                using iterator_category = ::std::input_iterator_tag;

            private:
                using iterator_type = ::std::ranges::iterator_t<source_type>;
                using sentinel_type = ::std::ranges::sentinel_t<source_type>;

            private:
                iterator_type position{};
                sentinel_type last{};
                bits_type remaining{};
                ::std::size_t index{0};

                static constexpr bool nothrow_traversal = ::std::is_nothrow_move_constructible_v<iterator_type> &&
                    ::std::is_nothrow_move_constructible_v<sentinel_type> &&
                    noexcept(::std::declval<iterator_type &>() == ::std::declval<sentinel_type &>()) &&
                    noexcept(++::std::declval<iterator_type &>()) &&
                    noexcept(element_type(*::std::declval<iterator_type &>()));

            public:
                cursor() = default;

                constexpr cursor(iterator_type position, sentinel_type last) /**/
                    noexcept(nothrow_traversal)
                    : position{::std::move(position)}
                    , last{::std::move(last)}
                {
                    if (!(this->position == this->last))
                        remaining = ::scl::hash::detail::element_bits(element_type(*this->position));
                }

                [[nodiscard]]
                constexpr value_type operator*() const noexcept
                {
                    return static_cast<value_type>(remaining & 0xFFU);
                }

                constexpr cursor & operator++() /**/
                    noexcept(nothrow_traversal)
                {
                    if (++index == byte_range::element_width)
                    {
                        index = 0;
                        ++position;
                        if (!(position == last))
                            remaining = ::scl::hash::detail::element_bits(element_type(*position));
                    }
                    else
                    {
                        remaining = static_cast<bits_type>(remaining >> 8U);
                    }
                    return *this;
                }

                constexpr void operator++(int) /**/
                    noexcept(nothrow_traversal)
                {
                    ++*this;
                }

                constexpr cursor operator++(int) /**/
                    noexcept(nothrow_traversal && ::std::is_nothrow_copy_constructible_v<iterator_type> &&
                        ::std::is_nothrow_copy_constructible_v<sentinel_type>)
                    requires(::std::ranges::forward_range<source_type>)
                {
                    auto taken = *this;
                    ++*this;
                    return taken;
                }

                [[nodiscard]]
                friend constexpr bool operator==(cursor const & left, cursor const & right) /**/
                    noexcept(noexcept(left.position == right.position))
                    requires(::std::equality_comparable<iterator_type>)
                {
                    return left.position == right.position && left.index == right.index;
                }

                [[nodiscard]]
                friend constexpr bool
                operator==(cursor const & left, [[maybe_unused]] ::std::default_sentinel_t const sentinel) /**/
                    noexcept(noexcept(left.position == left.last))
                {
                    return left.position == left.last;
                }
            };

        private:
            Base source{};

            static constexpr ::std::size_t element_width = sizeof(element_type);

        public:
            byte_range() = default;

            constexpr explicit byte_range(Base source) /**/
                noexcept(::std::is_nothrow_move_constructible_v<Base>)
                : source{::std::move(source)}
            {}

            [[nodiscard]]
            constexpr auto begin() /**/
                noexcept(noexcept(cursor{::std::ranges::begin(source), ::std::ranges::end(source)}))
                requires(!::scl::hash::detail::same_element_as_const<Base>)
            {
                return cursor{::std::ranges::begin(source), ::std::ranges::end(source)};
            }

            [[nodiscard]]
            constexpr auto begin() const /**/
                noexcept(noexcept(cursor{::std::ranges::begin(source), ::std::ranges::end(source)}))
                requires(::scl::hash::detail::same_element_as_const<Base>)
            {
                return cursor{::std::ranges::begin(source), ::std::ranges::end(source)};
            }

            [[nodiscard]]
            constexpr auto end() const noexcept
            {
                return ::std::default_sentinel;
            }

            [[nodiscard]]
            constexpr auto size() const /**/
                noexcept(noexcept(::std::ranges::size(source)))
                requires(::std::ranges::sized_range<Base const>)
            {
                return ::std::ranges::size(source) * element_width;
            }

            [[nodiscard]]
            constexpr auto size() /**/
                noexcept(noexcept(::std::ranges::size(source)))
                requires(::std::ranges::sized_range<Base>) && (!::std::ranges::sized_range<Base const>)
            {
                return ::std::ranges::size(source) * element_width;
            }
        };
    } // namespace detail

    /**
     * @brief Views @p range as the little-endian bytes of its elements.
     * @ingroup scl_utility_hash
     *
     * The hash functions take a range of byte-sized elements, so a range of wider ones
     * has to say which bytes it means before it can be hashed. This is that step, written
     * where a reader can see it:
     * @code
     * #include <scl/utility/hash/byte_view.h>
     * #include <scl/utility/hash/fnv1a.h>
     *
     * #include <string_view>
     *
     * using namespace std::string_view_literals;
     *
     * constexpr auto text = u"start"sv;
     * auto const value = scl::hash::fnv1a(scl::hash::byte_view(text));
     * @endcode
     *
     * Each element contributes `sizeof(element)` bytes, least significant first, whatever
     * the platform's own byte order - two machines hash one input alike. A byte-sized
     * element passes through unchanged, so the view is also a no-op where none is needed.
     *
     * The result is a lazy view. Where @p range is a named container the view reads through
     * to it, so that container must outlive the view; a named view is copied into the
     * result and a container handed over as a temporary is moved into it, and neither asks
     * anything of the caller. The view is forward where the source is forward, sized where
     * the source is sized, and iterated through a `const` reference where the source is. It
     * is never more than a forward range, whatever the source offers.
     *
     * @tparam Range  Any range whose element satisfies
     *                @ref scl::hash::concepts::integer_element and which is not a bounded
     *                array. The array rule is the one the concept
     *                @ref scl::hash::concepts::hashable_range states, over the wider element
     *                this view takes. Neither random access nor a reported size is asked
     *                for, and a `std::list` container, a `std::forward_list` container and
     *                a filtered view are spelled the way a `std::span` object is.
     * @param  range  Range to view. Must be a range a view can be taken of and kept -
     *                a `std::string_view` object, a `std::span` object, or a container,
     *                named or handed over as a temporary.
     * @return A `std::ranges::view` of `std::uint8_t`.
     *
     * @note An array is refused here for the reason the concept
     *       @ref scl::hash::concepts::hashable_range gives. `byte_view(u"AB"sv)` is the bytes
     *       of the two characters named, and `byte_view(std::span{buffer})` is the bytes of
     *       every element of the buffer; both say how much of the storage is the input, and
     *       the type of an array does not.
     *
     * @note A wide string is a different input from its narrow counterpart, since every
     *       element contributes its full width, and `byte_view(u"AB"sv)` is four bytes where
     *       `byte_view("AB"sv)` is two.
     */
    template <::std::ranges::viewable_range Range>
    [[nodiscard]]
    constexpr auto byte_view(Range && range) /**/
        noexcept(noexcept(::scl::hash::detail::byte_range<::scl::hash::detail::viewed_source_t<Range>>{
            ::std::views::all(::scl::hash::detail::view_source(::std::forward<Range>(range)))}))
        requires(!::scl::concepts::bounded_array<::std::remove_cvref_t<Range>>) &&
        (::scl::hash::concepts::integer_element<::std::ranges::range_value_t<Range>>)
    {
        return ::scl::hash::detail::byte_range<::scl::hash::detail::viewed_source_t<Range>>{
            ::std::views::all(::scl::hash::detail::view_source(::std::forward<Range>(range)))};
    }

} // namespace scl::hash
