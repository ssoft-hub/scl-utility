#pragma once

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/meta/type.h>

#include <string_view>

/**
 * @file
 * @brief Names of the constants of enumerations at compile time.
 * @ingroup scl_utility_meta
 */

namespace scl::detail
{
    // Its name occurs in the signature of its pattern function only at the template argument.
    enum we5r256sg_e // NOLINT(cppcoreguidelines-use-enum-class, performance-enum-size)
    {
        we5r256sg_v
    };

    template <::scl::concepts::enum_type auto V>
    constexpr ::std::string_view enum_name_pattern_text() noexcept
    {
#ifdef _MSC_VER
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    constexpr auto enum_prefix_length() noexcept
    {
        constexpr auto text = ::scl::detail::enum_name_pattern_text<we5r256sg_v>();
        return ::scl::detail::find_anchor(text, "scl::detail::we5r256sg_v").position;
    }

    constexpr auto enum_suffix_length() noexcept
    {
        constexpr auto text = ::scl::detail::enum_name_pattern_text<we5r256sg_v>();
        constexpr auto anchor = ::scl::detail::find_anchor(text, "scl::detail::we5r256sg_v");
        return text.length() - anchor.position - anchor.length;
    }

    // A value no constant has is spelled as a cast, (Type)42 or (enum Type)0x2a: an opening
    // parenthesis and, after the one that closes it, a number or, for an underlying bool with
    // MSVC, true or false to the end.
    constexpr bool names_constant(::std::string_view text) noexcept
    {
        if (!text.starts_with('('))
            return true;
        ::std::size_t depth = 0;
        for (::std::size_t index = 0; index < text.size(); ++index)
        {
            auto const character = text.substr(index, 1).front();
            if (character == '(')
                ++depth;
            if (character != ')' || --depth != 0)
                continue;
            auto const tail = text.substr(index + 1);
            constexpr ::std::string_view number = "-0123456789abcdefABCDEFx";
            if (tail == "true" || tail == "false")
                return false;
            // A constant of a type in an unnamed namespace has :: after that parenthesis.
            return tail.empty() || tail.find_first_not_of(number) != ::std::string_view::npos;
        }
        return true;
    }
} // namespace scl::detail

namespace scl
{
    /**
     * @brief Returns the full name of the constant V of an enumeration.
     * @ingroup scl_utility_meta
     *
     * @tparam V A value of an enumeration type.
     * @return The name of the constant of the type of @p V that has the value @p V, such as
     *         `app::Color::Red` for a constant of a scoped enumeration and `app::Active` for one
     *         of an unscoped enumeration; an empty view where no constant of that type has
     *         the value @p V.
     *
     * @warning The result is for display: its spelling can differ between compilers, as for a type
     * in an unnamed namespace, and may change in a later version of the module.
     *
     * @snippet meta/enum_name/meta_enum_name_example.cpp enum_name
     */
    template <::scl::concepts::enum_type auto V>
    [[nodiscard]]
    constexpr ::std::string_view enum_name() noexcept
    {
        constexpr auto text = ::scl::detail::enum_name_pattern_text<V>();
        constexpr auto prefix_length = ::scl::detail::enum_prefix_length();
        constexpr auto suffix_length = ::scl::detail::enum_suffix_length();

        constexpr auto result = text.substr(prefix_length, text.length() - prefix_length - suffix_length);

        if constexpr (!::scl::detail::names_constant(result))
            return {};
        else
            return result;
    }

    /**
     * @brief Returns the name of the constant V of an enumeration alone.
     * @ingroup scl_utility_meta
     *
     * @tparam V A value of an enumeration type.
     * @return The name of the constant of the type of @p V that has the value @p V alone, such
     *         as `Red`; an empty view where no constant of that type has the value @p V.
     *
     * @warning The result is for display: its spelling may change in a later version of
     * the module.
     *
     * @snippet meta/enum_name/meta_enum_name_example.cpp enum_short_name
     */
    template <::scl::concepts::enum_type auto V>
    [[nodiscard]]
    constexpr ::std::string_view enum_short_name() noexcept
    {
        constexpr auto result = ::scl::enum_name<V>();
        constexpr auto pos = result.find_last_of(':');
        if constexpr (pos != ::std::string_view::npos)
            return result.substr(pos + 1);
        else
            return result;
    }

} // namespace scl
