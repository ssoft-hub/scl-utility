#pragma once

/**
 * @file constant_bytes.h
 * @brief The bytes of a range the translation already holds.
 * @ingroup scl_utility_hash
 */

#include <array>
#include <cstddef>
#include <cstdint>

namespace scl::hash
{

    template <::std::size_t Capacity>
    struct constant_bytes
    {
        ::std::array<::std::uint8_t, Capacity> bytes;
        ::std::size_t count;

        [[nodiscard]]
        constexpr ::std::uint8_t const * begin() const noexcept
        {
            return bytes.data();
        }

        [[nodiscard]]
        constexpr ::std::uint8_t const * end() const noexcept
        {
            return bytes.data() + count;
        }
    };

} // namespace scl::hash

/**
 * @struct scl::hash::constant_bytes
 * @brief The bytes of a range the translation already holds.
 * @ingroup scl_utility_hash
 * @tparam Capacity  Bytes the array it came from occupies, terminator included.
 *
 * @ref scl::hash::byte_view answers one for a bounded array, and only for a bounded array:
 * an array declared with its contents is content the translation has, so its bytes are a
 * value rather than a view reading through to something else. Nothing else produces one,
 * which is what lets a `consteval` hash overload take it without standing in the way of any
 * shape known only at run time.
 *
 * The count is a member rather than a second template parameter because whether the array
 * ends in a terminator is a property of its contents, not of its type, and a return type
 * cannot depend on the contents of the argument that produced it.
 *
 * Declaring one is not the intended use. A hash over it is `consteval`, so a `constant_bytes`
 * filled at run time cannot be hashed - spell such a sequence as a view instead.
 */

/**
 * @var scl::hash::constant_bytes::bytes
 * @brief The bytes themselves, least significant first within each element.
 */

/**
 * @var scl::hash::constant_bytes::count
 * @brief How many of them are text: `Capacity`, less a terminating element if there was one.
 */

/**
 * @fn scl::hash::constant_bytes::begin() const
 * @brief First byte, so the type is a contiguous range.
 * @return Pointer to the first byte.
 */

/**
 * @fn scl::hash::constant_bytes::end() const
 * @brief One past the last byte that is text.
 * @return Pointer one past the last byte that is text.
 */
