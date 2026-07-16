#pragma once

/// @file flags.h
/// @brief Type-safe bitmask container over a scoped enum.
/// @ingroup scl_utility_flags

#include <scl/utility/concepts/type_category.h>

#include <algorithm>
#include <array>
#include <concepts>
#include <cstddef>
#include <iterator>
#include <stdexcept>
#include <type_traits>

/**
 * @defgroup scl_utility_flags ScL Flags
 * @ingroup scl_utility
 * @brief Type-safe bitmask container `scl::flags` over a scoped `enum class`.
 */

namespace scl
{
    template <concepts::enum_type Enum, ::std::size_t bit_count = 32> // NOLINT(readability-identifier-naming)
    class flags
    {
        static_assert(!::std::is_convertible_v<Enum, ::std::underlying_type_t<Enum>>,
            "scl::flags requires a scoped enum (enum class)");

    public:
        class const_iterator;

        using enum_type = Enum;
        using value_type = Enum;
        using size_type = ::std::size_t;
        using difference_type = ::std::ptrdiff_t;
        using iterator = const_iterator;
        using reverse_iterator = ::std::reverse_iterator<const_iterator>;
        using const_reverse_iterator = ::std::reverse_iterator<const_iterator>;

    public:
        static constexpr ::std::size_t capacity = bit_count;

    private:
        static constexpr ::std::size_t byte_count = (bit_count + 7) / 8;
        ::std::array<::std::byte, byte_count> m_bits{};

    public:
        constexpr flags() noexcept = default;

        template <::std::same_as<Enum>... Values>
        constexpr flags(Values... values)
        {
            (set_bit(ordinal(values)), ...);
        }

        [[nodiscard]]
        constexpr bool operator==(flags const & other) const noexcept
        {
            return m_bits == other.m_bits;
        }

        [[nodiscard]]
        constexpr bool operator[](Enum value) const
        {
            auto const bit = ordinal(value);
            if (bit >= bit_count)
                throw ::std::out_of_range("scl::flags: enumerator ordinal exceeds bit_count");
            return has_bit(bit);
        }

        template <::std::same_as<Enum>... Values>
        [[nodiscard]]
        constexpr bool all_of(Values... values) const
        {
            return (has_bit(ordinal(values)) && ...);
        }

        template <::std::same_as<Enum>... Values>
        [[nodiscard]]
        constexpr bool any_of(Values... values) const
        {
            return (has_bit(ordinal(values)) || ...);
        }

        template <::std::same_as<Enum>... Values>
        [[nodiscard]]
        constexpr bool none_of(Values... values) const
        {
            return !any_of(values...);
        }

        [[nodiscard]]
        constexpr bool all_of(flags const & other) const noexcept
        {
            return (*this & other) == other;
        }

        [[nodiscard]]
        constexpr bool any_of(flags const & other) const noexcept
        {
            return (*this & other).any();
        }

        [[nodiscard]]
        constexpr bool none_of(flags const & other) const noexcept
        {
            return (*this & other).none();
        }

        [[nodiscard]]
        constexpr bool any() const noexcept
        {
            return ::std::ranges::any_of(m_bits, [](::std::byte const bits) {
                return bits != ::std::byte{};
            });
        }

        [[nodiscard]]
        constexpr bool none() const noexcept
        {
            return !any();
        }

        [[nodiscard]]
        constexpr bool all() const noexcept
        {
            for (::std::size_t position = 0; position < bit_count; ++position)
                if (!has_bit(position))
                    return false;
            return true;
        }

        [[nodiscard]]
        constexpr explicit operator bool() const noexcept
        {
            return any();
        }

        [[nodiscard]]
        constexpr flags operator~() const noexcept
        {
            flags result;
            for (::std::size_t index = 0; index < byte_count; ++index)
                result.m_bits[index] = ~m_bits[index];
            result.trim();
            return result;
        }

        [[nodiscard]]
        constexpr flags operator|(flags const & other) const noexcept
        {
            flags result;
            for (::std::size_t index = 0; index < byte_count; ++index)
                result.m_bits[index] = m_bits[index] | other.m_bits[index];
            return result;
        }

        [[nodiscard]]
        constexpr flags operator&(flags const & other) const noexcept
        {
            flags result;
            for (::std::size_t index = 0; index < byte_count; ++index)
                result.m_bits[index] = m_bits[index] & other.m_bits[index];
            return result;
        }

        [[nodiscard]]
        constexpr flags operator^(flags const & other) const noexcept
        {
            flags result;
            for (::std::size_t index = 0; index < byte_count; ++index)
                result.m_bits[index] = m_bits[index] ^ other.m_bits[index];
            return result;
        }

        [[nodiscard]]
        constexpr flags operator|(Enum value) const
        {
            return *this | flags{value};
        }

        [[nodiscard]]
        constexpr flags operator&(Enum value) const
        {
            return *this & flags{value};
        }

        [[nodiscard]]
        constexpr flags operator^(Enum value) const
        {
            return *this ^ flags { value };
        }

        constexpr flags & operator|=(flags const & other) noexcept { return *this = *this | other; }

        constexpr flags & operator&=(flags const & other) noexcept { return *this = *this & other; }

        constexpr flags & operator^=(flags const & other) noexcept { return *this = *this ^ other; }

        constexpr flags & operator|=(Enum value) { return *this = *this | value; }

        constexpr flags & operator&=(Enum value) { return *this = *this & value; }

        constexpr flags & operator^=(Enum value) { return *this = *this ^ value; }

        [[nodiscard]]
        constexpr size_type size() const noexcept
        {
            size_type count = 0;
            for (::std::size_t position = 0; position < bit_count; ++position)
                if (has_bit(position))
                    ++count;
            return count;
        }

        [[nodiscard]]
        constexpr const_iterator begin() const noexcept
        {
            return {this, next_set(0)};
        }

        [[nodiscard]]
        constexpr const_iterator end() const noexcept
        {
            return {this, bit_count};
        }

        [[nodiscard]]
        constexpr const_iterator cbegin() const noexcept
        {
            return begin();
        }

        [[nodiscard]]
        constexpr const_iterator cend() const noexcept
        {
            return end();
        }

        [[nodiscard]]
        constexpr reverse_iterator rbegin() const noexcept
        {
            return reverse_iterator{end()};
        }

        [[nodiscard]]
        constexpr reverse_iterator rend() const noexcept
        {
            return reverse_iterator{begin()};
        }

    private:
        [[nodiscard]]
        static constexpr size_type ordinal(Enum value) noexcept
        {
            return static_cast<size_type>(static_cast<::std::underlying_type_t<Enum>>(value));
        }

        constexpr void set_bit(::std::size_t position)
        {
            if (position >= bit_count)
                throw ::std::out_of_range("scl::flags: enumerator ordinal exceeds bit_count");
            m_bits[position / 8] |= (::std::byte{1} << (position % 8));
        }

        [[nodiscard]]
        constexpr bool has_bit(::std::size_t position) const noexcept
        {
            return position < bit_count &&
                (m_bits[position / 8] & (::std::byte{1} << (position % 8))) != ::std::byte{};
        }

        constexpr void trim() noexcept
        {
            if constexpr (bit_count % 8 != 0)
                m_bits[byte_count - 1] &= static_cast<::std::byte>((1u << (bit_count % 8)) - 1u);
        }

        [[nodiscard]]
        constexpr ::std::size_t next_set(::std::size_t from) const noexcept
        {
            while (from < bit_count && !has_bit(from))
                ++from;
            return from;
        }

        [[nodiscard]]
        constexpr ::std::size_t prev_set(::std::size_t before) const noexcept
        {
            while (before > 0 && !has_bit(before - 1))
                --before;
            return before - 1;
        }
    };

    template <concepts::enum_type Enum, ::std::size_t bit_count> // NOLINT(readability-identifier-naming)
    class flags<Enum, bit_count>::const_iterator
    {
    public:
        using iterator_concept = ::std::bidirectional_iterator_tag;
        using iterator_category = ::std::bidirectional_iterator_tag;
        using value_type = Enum;
        using reference = Enum;
        using pointer = void;
        using difference_type = ::std::ptrdiff_t;

    private:
        flags const * m_owner = nullptr;
        ::std::size_t m_position = 0;

    public:
        constexpr const_iterator() noexcept = default;

        constexpr const_iterator(flags const * owner, ::std::size_t position) noexcept
            : m_owner{owner}
            , m_position{position}
        {}

        [[nodiscard]]
        constexpr Enum operator*() const
        {
            return static_cast<Enum>(m_position);
        }

        constexpr const_iterator & operator++() noexcept
        {
            m_position = m_owner->next_set(m_position + 1);
            return *this;
        }

        [[nodiscard]]
        constexpr const_iterator operator++(int) noexcept
        {
            auto copy = *this;
            ++*this;
            return copy;
        }

        constexpr const_iterator & operator--() noexcept
        {
            m_position = m_owner->prev_set(m_position);
            return *this;
        }

        [[nodiscard]]
        constexpr const_iterator operator--(int) noexcept
        {
            auto copy = *this;
            --*this;
            return copy;
        }

        [[nodiscard]]
        constexpr bool operator==(const_iterator const & other) const noexcept
        {
            return m_position == other.m_position;
        }
    };
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/// @class scl::flags
/// @ingroup scl_utility_flags
/// @brief Type-safe bitmask over a scoped enum, where each flag occupies the bit
///        at its enumerator ordinal
///
/// `flags` stores one bit per enumerator: the bit index of a flag equals the
/// enumerator's underlying value (`One == 0`, `Two == 1`, ...), so enumerators
/// need no explicit values.  The bits live in a `std::array<std::byte>` sized to
/// `bit_count`, which keeps every operation usable in constant evaluation on the
/// C++20 baseline (unlike `std::bitset`, whose query and mutate members are not
/// `constexpr` until C++23).
///
/// The type is a bidirectional range over its *set* flags: iterating yields the
/// corresponding `Enum` values in ascending ordinal order, so it composes with
/// the `<ranges>` views and algorithms.  `capacity` is the fixed bit width;
/// `size()` is the number of set flags.
///
/// Only scoped enumerations are accepted; a non-scoped `enum` is rejected by a
/// `static_assert`.  An enumerator whose ordinal is `>= bit_count` is out of
/// range: the offending call throws `std::out_of_range` at runtime and is
/// ill-formed in constant evaluation.  The predicate queries are the exception —
/// they never throw and read an out-of-range ordinal as not set.
///
/// @tparam Enum      Scoped enumeration whose enumerators name the individual bits.
/// @tparam bit_count Storage width in bits (default 32); valid ordinals are `[0, bit_count)`.
///
/// @par Example
/// @code
/// enum class flag_number { One, Two, Three };
/// using flag_numbers = ::scl::flags<flag_number>;
///
/// constexpr flag_numbers f{flag_number::One, flag_number::Two};
/// static_assert(f[flag_number::One]);
/// static_assert(f.all_of(flag_number::One, flag_number::Two));
/// static_assert(f.size() == 2);
/// @endcode

// -----------------------------------------------------------------------------
// Member types and constants
// -----------------------------------------------------------------------------

/// @typedef scl::flags::enum_type
/// @brief The scoped enumeration this bitmask is built over (same as `Enum`).

/// @typedef scl::flags::value_type
/// @brief Element type produced when iterating the set flags (same as `Enum`).

/// @typedef scl::flags::size_type
/// @brief Unsigned integer type for flag counts and ordinals.

/// @typedef scl::flags::difference_type
/// @brief Signed integer type for iterator distances.

/// @typedef scl::flags::iterator
/// @brief Bidirectional iterator over the set flags (alias of `const_iterator`).

/// @typedef scl::flags::reverse_iterator
/// @brief Reverse bidirectional iterator over the set flags.

/// @typedef scl::flags::const_reverse_iterator
/// @brief Immutable reverse iterator over the set flags (same as `reverse_iterator`).

/// @var scl::flags::capacity
/// @brief Fixed storage width in bits (equal to `bit_count`); the number of
///        addressable flag ordinals, distinct from `size()`.

// -----------------------------------------------------------------------------
// Construction
// -----------------------------------------------------------------------------

/// @fn scl::flags::flags()
/// @brief Constructs an empty bitmask with no flags set.

/// @fn scl::flags::flags(Values... values)
/// @brief Constructs a bitmask with the given flags set.
///
/// Accepts zero or more `Enum` values; each sets the bit at its ordinal.  A
/// braced empty list selects the default constructor instead.  Passing a value
/// whose ordinal is `>= bit_count` is out of range (see the class description).
///
/// @tparam Values  Pack of `Enum` values (each must be exactly `Enum`).
/// @param  values  Flags to set.
/// @throws ::std::out_of_range  If any value's ordinal is `>= bit_count`
///                              (ill-formed in constant evaluation).
///
/// @par Example
/// @code
/// flag_numbers none{};                                   // no flags
/// flag_numbers one{flag_number::One};                    // single flag
/// flag_numbers many{flag_number::One, flag_number::Two}; // several flags
/// @endcode

// -----------------------------------------------------------------------------
// Comparison
// -----------------------------------------------------------------------------

/// @fn scl::flags::operator==(flags const & other) const
/// @brief Returns `true` when both bitmasks have exactly the same flags set.
/// @param other  Bitmask to compare against.
/// @return `true` on bit-for-bit equality; `!=` is synthesised from this.

// -----------------------------------------------------------------------------
// Element access
// -----------------------------------------------------------------------------

/// @fn scl::flags::operator[](Enum value) const
/// @brief Tests whether a single flag is set.
/// @param  value  Flag to test.
/// @return `true` if @p value is set.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`
///                              (ill-formed in constant evaluation).

// -----------------------------------------------------------------------------
// Predicate queries
// -----------------------------------------------------------------------------

/// @fn scl::flags::all_of(Values... values) const
/// @brief Returns `true` when every listed flag is set.
///
/// An empty pack is vacuously `true`.  Unlike the constructor and `operator[]`,
/// the predicate never throws: an out-of-range ordinal simply reads as not set.
///
/// @tparam Values  Pack of `Enum` values (each must be exactly `Enum`).
/// @param  values  Flags to test.
/// @return `true` if all of @p values are set.
///
/// @par Example
/// @code
/// constexpr flag_numbers f{flag_number::One, flag_number::Two};
/// static_assert( f.all_of(flag_number::One, flag_number::Two));
/// static_assert(!f.all_of(flag_number::Three));
/// static_assert( f.all_of());                            // vacuously true
/// @endcode

/// @fn scl::flags::any_of(Values... values) const
/// @brief Returns `true` when at least one listed flag is set.
/// @tparam Values  Pack of `Enum` values (each must be exactly `Enum`).
/// @param  values  Flags to test.
/// @return `true` if any of @p values is set; an empty pack is `false`.

/// @fn scl::flags::none_of(Values... values) const
/// @brief Returns `true` when none of the listed flags is set.
/// @tparam Values  Pack of `Enum` values (each must be exactly `Enum`).
/// @param  values  Flags to test.
/// @return `true` if none of @p values is set; an empty pack is `true`.

/// @fn scl::flags::all_of(flags const & other) const
/// @brief Returns `true` when @p other is a subset of this bitmask.
/// @param other  Bitmask whose set flags must all be set here.
/// @return `true` if every flag of @p other is set (`(*this & other) == other`).

/// @fn scl::flags::any_of(flags const & other) const
/// @brief Returns `true` when the two bitmasks share at least one set flag.
/// @param other  Bitmask to intersect with.
/// @return `true` if the intersection is non-empty.

/// @fn scl::flags::none_of(flags const & other) const
/// @brief Returns `true` when the two bitmasks share no set flag.
/// @param other  Bitmask to test disjointness against.
/// @return `true` if the intersection is empty.

// -----------------------------------------------------------------------------
// Whole-mask queries
// -----------------------------------------------------------------------------

/// @fn scl::flags::any() const
/// @brief Returns `true` when at least one bit is set.
/// @return `true` if any of the `bit_count` bits is set.

/// @fn scl::flags::none() const
/// @brief Returns `true` when no bit is set.
/// @return `true` if the bitmask is empty.

/// @fn scl::flags::all() const
/// @brief Returns `true` when every one of the `bit_count` bits is set.
/// @return `true` if all bits (the full storage width) are set.

/// @fn scl::flags::operator bool() const
/// @brief Contextually converts to `true` when at least one flag is set.
/// @return `any()`; explicit, so it participates only in boolean contexts.

// -----------------------------------------------------------------------------
// Bitwise operators
// -----------------------------------------------------------------------------

/// @fn scl::flags::operator~() const
/// @brief Returns the complement over all `bit_count` bits.
/// @return A bitmask with every valid ordinal toggled; padding bits above
///         `bit_count` stay clear.

/// @fn scl::flags::operator|(flags const & other) const
/// @brief Returns the union of the two bitmasks.
/// @param other  Right-hand bitmask.
/// @return A bitmask with the flags of either operand set.

/// @fn scl::flags::operator&(flags const & other) const
/// @brief Returns the intersection of the two bitmasks.
/// @param other  Right-hand bitmask.
/// @return A bitmask with the flags set in both operands.

/// @fn scl::flags::operator^(flags const & other) const
/// @brief Returns the symmetric difference of the two bitmasks.
/// @param other  Right-hand bitmask.
/// @return A bitmask with the flags set in exactly one operand.

/// @fn scl::flags::operator|(Enum value) const
/// @brief Returns the union with a single flag.
/// @param value  Flag to add.
/// @return A copy with @p value additionally set.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`.

/// @fn scl::flags::operator&(Enum value) const
/// @brief Returns the intersection with a single flag.
/// @param value  Flag to keep.
/// @return A bitmask holding at most @p value.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`.

/// @fn scl::flags::operator^(Enum value) const
/// @brief Returns the symmetric difference with a single flag.
/// @param value  Flag to toggle.
/// @return A copy with @p value toggled.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`.

// -----------------------------------------------------------------------------
// Compound assignment
// -----------------------------------------------------------------------------

/// @fn scl::flags::operator|=(flags const & other)
/// @brief Sets the flags of @p other in place (union).
/// @param other  Bitmask to union in.
/// @return `*this`.

/// @fn scl::flags::operator&=(flags const & other)
/// @brief Keeps only the flags also set in @p other (intersection).
/// @param other  Bitmask to intersect with.
/// @return `*this`.

/// @fn scl::flags::operator^=(flags const & other)
/// @brief Toggles the flags set in @p other (symmetric difference).
/// @param other  Bitmask to XOR in.
/// @return `*this`.

/// @fn scl::flags::operator|=(Enum value)
/// @brief Sets a single flag in place.
/// @param value  Flag to add.
/// @return `*this`.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`.

/// @fn scl::flags::operator&=(Enum value)
/// @brief Keeps only a single flag in place.
/// @param value  Flag to keep.
/// @return `*this`.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`.

/// @fn scl::flags::operator^=(Enum value)
/// @brief Toggles a single flag in place.
/// @param value  Flag to toggle.
/// @return `*this`.
/// @throws ::std::out_of_range  If the ordinal is `>= bit_count`.

// -----------------------------------------------------------------------------
// Size and iteration
// -----------------------------------------------------------------------------

/// @fn scl::flags::size() const
/// @brief Returns the number of set flags (the range element count).
/// @return The population count; distinct from `capacity`.

/// @fn scl::flags::begin() const
/// @brief Returns an iterator to the first set flag (lowest ordinal).
/// @return A `const_iterator` positioned at the first set flag.

/// @fn scl::flags::end() const
/// @brief Returns the past-the-end iterator.
/// @return A `const_iterator` one past the last set flag.

/// @fn scl::flags::cbegin() const
/// @brief Returns a const iterator to the first set flag.
/// @return Same as `begin()`.

/// @fn scl::flags::cend() const
/// @brief Returns the const past-the-end iterator.
/// @return Same as `end()`.

/// @fn scl::flags::rbegin() const
/// @brief Returns a reverse iterator to the last set flag (highest ordinal).
/// @return A `reverse_iterator` positioned at the last set flag.

/// @fn scl::flags::rend() const
/// @brief Returns the reverse past-the-end iterator.
/// @return A `reverse_iterator` one before the first set flag.

/// @class scl::flags::const_iterator
/// @brief Bidirectional iterator over the set flags of a `flags`, in ascending
///        ordinal order
///
/// Holds the ordinal of the current set flag; dereference casts it to the `Enum`
/// value.  Advancing scans to the next or previous set bit, so the iterator
/// stays valid only while the `flags` it was obtained from is unchanged —
/// mutating that object invalidates it.

/// @typedef scl::flags::const_iterator::iterator_concept
/// @brief Models `std::bidirectional_iterator`.

/// @typedef scl::flags::const_iterator::iterator_category
/// @brief Legacy bidirectional iterator category tag.

/// @typedef scl::flags::const_iterator::value_type
/// @brief The set flag produced on dereference (`Enum`).

/// @typedef scl::flags::const_iterator::pointer
/// @brief No pointer type is exposed (`void`); the iterator yields prvalues.

/// @typedef scl::flags::const_iterator::reference
/// @brief Dereference result — an `Enum` prvalue, returned by value.

/// @typedef scl::flags::const_iterator::difference_type
/// @brief Signed integer type for iterator distances.

/// @fn scl::flags::const_iterator::const_iterator(flags const * owner, ::std::size_t position)
/// @brief Constructs an iterator over @p owner positioned at bit @p position.
/// @param owner     The flags being iterated.
/// @param position  Current set-bit ordinal (`bit_count` for the end iterator).

/// @fn scl::flags::const_iterator::operator*() const
/// @brief Returns the flag at the current position.
/// @return The `Enum` value whose ordinal is the current position.

/// @fn scl::flags::const_iterator::operator++()
/// @brief Advances to the next set flag (pre-increment).
/// @return `*this`.

/// @fn scl::flags::const_iterator::operator++(int)
/// @brief Advances to the next set flag (post-increment).
/// @return The iterator's value before advancing.

/// @fn scl::flags::const_iterator::operator--()
/// @brief Steps back to the previous set flag (pre-decrement).
/// @return `*this`.

/// @fn scl::flags::const_iterator::operator--(int)
/// @brief Steps back to the previous set flag (post-decrement).
/// @return The iterator's value before stepping back.

/// @fn scl::flags::const_iterator::operator==(const_iterator const & other) const
/// @brief Compares iterator positions.
/// @param other  Iterator to compare against.
/// @return `true` when both are at the same position; `!=` is synthesised.
