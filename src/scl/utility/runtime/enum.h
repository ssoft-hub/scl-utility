#pragma once

/**
 * @file
 * @brief Runtime enum value representation (C++20).
 * @ingroup scl_utility_runtime
 * @details
 * - ::scl::enum_string(value):
 *     Returns a string of the form "TypeName::N" where N is the value as a number, a
 *     character or bool underlying type included.
 */

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/meta/type.h>

#include <string>
#include <type_traits>
#include <version>

#ifdef __cpp_lib_format
#include <format>
#else
#include <array>
#include <charconv>
#include <iterator>
#endif

namespace scl::detail
{
    // A character or bool underlying type still renders as a number.
    template <typename Enum>
    using enum_number_t =
        ::std::conditional_t<::std::is_signed_v<::std::underlying_type_t<Enum>>, long long, unsigned long long>;

} // namespace scl::detail

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
     * auto const red_name     = ::scl::enum_string(Color::Red);  // "Color::1"
     * auto const other_name   = ::scl::enum_string(Color{42});   // "Color::42"
     *
     * enum class Flags : unsigned { A = 1, B = 2 };
     * auto const flag_name    = ::scl::enum_string(Flags::B);    // "Flags::2"
     * @endcode
     */
    template <concepts::enum_type E>
    [[nodiscard]]
    ::std::string enum_string(E value)
    {
        static_assert(sizeof(::std::underlying_type_t<E>) <= sizeof(long long),
            "enum_string spells an underlying type no wider than long long");
        auto const number = static_cast<::scl::detail::enum_number_t<E>>(value);
#ifdef __cpp_lib_format
        return ::std::format("{}::{}", ::scl::type_short_name<E>(), number);
#else
        auto const type = ::scl::type_short_name<E>();
        ::std::array<char, 32> buf{};
        ::std::to_chars(buf.data(), ::std::next(buf.data(), static_cast<::std::ptrdiff_t>(buf.size())), number);
        ::std::string result;
        result.reserve(type.size() + 2 + buf.size());
        result += type;
        result += "::";
        result += buf.data();
        return result;
#endif
    }

} // namespace scl
