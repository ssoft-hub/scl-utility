#pragma once

/**
 * @file base.h
 * @brief Shared input preparation for the ScL hash functions.
 * @ingroup scl_utility_hash
 */

#include <scl/utility/attribute/inline.h>
#include <scl/utility/hash/concepts.h>

#include <bit>
#include <cstdint>

namespace scl::hash::detail
{
    template <::scl::hash::concepts::byte_element Element>
    [[nodiscard]]
    SCL_FORCE_INLINE constexpr ::std::uint8_t as_byte(Element const value) noexcept
    {
        return ::std::bit_cast<::std::uint8_t>(value);
    }

} // namespace scl::hash::detail
