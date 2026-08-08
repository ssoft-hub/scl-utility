#pragma once

/**
 * @file concepts.h
 * @brief Constraints on what the ScL hash functions accept.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/concepts/type_property.h>

#include <concepts>
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
     * An integer, or an enumeration standing for one. Two types are left out:
     *
     * - A floating-point type. Its bytes tell values apart that compare equal — `0.0`
     *   against `-0.0`, one `NaN` against another — so a hash value taken from them answers a
     *   different question than the comparison does.
     * - `wchar_t`. Its width is what the platform says it is — two bytes on Windows, four
     *   elsewhere — so one text would reach the hash function as a different number of bytes on
     *   each. Spell the encoding that is meant: `char16_t` and `char32_t` are fixed by the
     *   standard and hash alike everywhere.
     *
     * @tparam Type  Element type to check.
     */
    // clang-format off
    template <typename Type>
    concept integer_element = (::std::integral<Type> || ::scl::concepts::enum_type<Type>)
            && !::std::same_as<::std::remove_cv_t<Type>, wchar_t>;
    // clang-format on

} // namespace scl::hash::concepts
