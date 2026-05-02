#pragma once

/**
 * @file
 * @brief Runtime enum value representation (C++20).
 * @ingroup scl_utility_runtime
 * @details
 * - ::scl::enum_value(value):
 *     Returns a string of the form "TypeName::N" where N is the underlying
 *     numeric value cast to the enum's underlying type.
 */

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/meta/type.h>

#include <string>
#include <type_traits>

#ifdef __cpp_lib_format
#include <format>
#else
#include <array>
#include <charconv>
#include <iterator>
#endif

namespace scl
{
    /**
     * @brief Returns a string representation of an enum value as "TypeName::N".
     * @ingroup scl_utility_runtime
     * @tparam E Enum type (deduced automatically).
     * @param  value The enum value to represent.
     * @return String of the form "TypeName::N" where N is the underlying numeric value.
     *
     * @note Does not require RTTI. The type name is extracted at compile time.
     *
     * @code{.cpp}
     * enum class Color : int { Red = 1, Green = 2 };
     * ::scl::enum_value(Color::Red);  // "Color::1"
     * ::scl::enum_value(Color{42});   // "Color::42"
     *
     * enum class Flags : unsigned { A = 1, B = 2 };
     * ::scl::enum_value(Flags::B);    // "Flags::2"
     * @endcode
     */
    template <concepts::enum_type E>
    ::std::string enum_value(E value)
    {
#ifdef __cpp_lib_format
        return ::std::format("{}::{}", ::scl::type_short_name<E>(),
            static_cast<::std::underlying_type_t<E>>(value));
#else
        auto const type = ::scl::type_short_name<E>();
        ::std::array<char, 32> buf{};
        ::std::to_chars(buf.data(), ::std::next(buf.data(), static_cast<::std::ptrdiff_t>(buf.size())),
            static_cast<::std::underlying_type_t<E>>(value));
        ::std::string result;
        result.reserve(type.size() + 2 + buf.size());
        result += type;
        result += "::";
        result += buf.data();
        return result;
#endif
    }

} // namespace scl
