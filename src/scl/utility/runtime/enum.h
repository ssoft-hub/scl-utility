#pragma once

/**
 * @file
 * @brief An enumeration value spelled as its type and its number.
 * @ingroup scl_utility_runtime
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
    template <::scl::concepts::enum_type E>
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

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @fn scl::enum_string(E value)
 * @ingroup scl_utility_runtime
 * @brief Returns @p value spelled as `Type::N`, where `N` is its number.
 *
 * `Type` is ::scl::type_short_name<E>(), read at compile time with no RTTI. `N` is the
 * number of @p value in its underlying type, so a value no enumerator names is spelled as
 * well as one that is named, and a character or `bool` underlying type still gives a
 * number.
 *
 * @snippet runtime/enum_string/runtime_enum_string_example.cpp named
 * @snippet runtime/enum_string/runtime_enum_string_example.cpp unnamed
 *
 * @tparam E Enumeration type, deduced.
 * @param value Value to spell.
 * @return `Type::N`, for example `Color::1`.
 */
