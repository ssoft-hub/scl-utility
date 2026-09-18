#pragma once

/**
 * @file concepts.h
 * @brief Constraints on what the ScL hash functions accept.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/concepts/type_property.h>

#include <concepts>
#include <ranges>
#include <type_traits>

namespace scl::hash::concepts
{
    /**
     * @brief Satisfied when a value of @p Type is exactly one byte of data.
     * @ingroup scl_utility_hash
     *
     * The hash functions read a range one byte per element, so an element must be
     * a byte for every bit of it to reach the hash value. A wider one — `wchar_t`,
     * `char16_t`, `char32_t`, or any arithmetic type — is rejected rather than
     * truncated: silently hashing its low byte alone would let two inputs differing
     * above that byte produce one value. Hash the bytes of such a range explicitly
     * when that is what is meant.
     *
     * An empty type is rejected too. It occupies a byte without owning it, so there
     * is no value to read.
     *
     * @tparam Type  Element type to check.
     *
     * @par Example
     * @code{.cpp}
     * #include <scl/utility/hash/concepts.h>
     *
     * #include <cstddef>
     *
     * static_assert( scl::hash::concepts::byte_element<char>);
     * static_assert( scl::hash::concepts::byte_element<std::byte>);
     * static_assert(!scl::hash::concepts::byte_element<char16_t>);
     * static_assert(!scl::hash::concepts::byte_element<double>);
     * @endcode
     */
    // clang-format off
    template <typename Type>
    concept byte_element = sizeof(Type) == 1
            && ::scl::concepts::trivially_copyable<Type>
            && !::scl::concepts::empty_type<Type>;
    // clang-format on

    /**
     * @brief Satisfied when a value of @p Type has bytes @ref scl::hash::byte_view can spell.
     * @ingroup scl_utility_hash
     *
     * An integer, or an enumeration standing for one. A floating-point type is left out,
     * because its bytes tell values apart that compare equal - `0.0` against `-0.0`, one
     * `NaN` against another - so a hash value taken from them answers a different question
     * than the comparison does.
     *
     * The width of `wchar_t`, `int`, `long` and `std::size_t` is the platform's own, so a hash
     * value taken over such an element is comparable within one build rather than across
     * platforms. `char16_t` and `char32_t` are fixed by the standard and spell one text alike
     * everywhere.
     *
     * @tparam Type  Element type to check.
     */
    template <typename Type>
    concept integer_element = ::std::integral<Type> || ::scl::concepts::enum_type<Type>;

    /**
     * @brief Satisfied when @p Range is what the ScL hash functions take.
     * @ingroup scl_utility_hash
     *
     * A range whose element is a single byte of data, and which is not a bounded array. A hash
     * function states this concept in its own signature to refuse what an array would answer
     * for.
     *
     * A bounded array reports the length it was declared with, which is the storage it was
     * given rather than the content a caller put in it. `char buffer[64]` holding three
     * characters is sixty-four elements to its type and three to its author, and nothing in the
     * type says which is meant.
     *
     * The refusal follows what the type system can tell apart, not where the wrong answer
     * actually arises. A string literal and a buffer a caller declares share one kind of
     * type - `"hello"` is a `char const[6]`, and so is `constexpr char declared[6]`. A
     * constraint that refuses the literal therefore refuses the declared buffer with it, having
     * no way to tell the two apart. The refusal reaches the bounded array and stops there,
     * and the type `std::array<char, 64>` reports sixty-four for the reason `char[64]` does,
     * is taken all the same, and answers for its whole bound when partially filled.
     *
     * Naming the bytes of a filled buffer takes the length the caller knows.
     * `std::string_view{buffer}` stops at a terminator, which a buffer holding bytes rather
     * than text need not carry, so `std::string_view{buffer, length}` or
     * `std::span{buffer, length}` is what names the bytes the caller put there.
     *
     * The rule reaches only the array itself, so a view over one, `std::views::all(buffer)`,
     * is taken and reaches the whole bound.
     *
     * @tparam Range  Range to check.
     *
     * @par Example
     * @code{.cpp}
     * #include <scl/utility/hash/concepts.h>
     *
     * #include <cstdint>
     * #include <ranges>
     *
     * template <scl::hash::concepts::hashable_range Range>
     * constexpr std::uint64_t rolling(Range && range)
     * {
     *     std::uint64_t h = 0;
     *     for (std::ranges::range_value_t<Range> const c : range)
     *         h = (h * 31ull) + static_cast<std::uint8_t>(c);
     *     return h;
     * }
     *
     * // char buffer[64]; rolling(buffer) does not compile.
     * @endcode
     *
     * @see scl::hash::concepts::byte_element
     */
    template <typename Range>
    concept hashable_range = ::std::ranges::range<Range> &&
        !::scl::concepts::bounded_array<::std::remove_cvref_t<Range>> &&
        ::scl::hash::concepts::byte_element<::std::ranges::range_value_t<Range>>;

} // namespace scl::hash::concepts
