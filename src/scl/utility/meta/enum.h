#pragma once

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/meta/type.h>

#include <string_view>

/**
 * @file
 * @brief Compile-time enumeration introspection utilities (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::enum_name<V>:
 *     Retrieves the qualified string name of the enum member V, or an empty string where no
 *     constant of its type has the value V.
 * - ::scl::enum_short_name<V>:
 *     Retrieves only the identifier of the enum member V without type or namespace qualifiers,
 *     or an empty string where no constant of its type has the value V.
 * - ::scl::concepts::enum_type:
 *     Concept satisfied by any enumeration type (scoped or unscoped).
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
     * @brief Retrieves the qualified string name of an enum member at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam V The enum value whose name needs to be extracted.
     * @return A ::std::string_view containing the qualified name of the enum member, or an empty
     *         ::std::string_view where no constant of the type of @p V has the value @p V.
     * 
     * @note The result usually includes the enum type name (e.g., "Color::Red") 
     * depending on how the enum was defined and the compiler used.
     *
     * @warning The result is for display: its spelling can differ between compilers, as for a type
     * in an unnamed namespace, and may change in a later version of the module.
     * 
     * @code
     * enum class Color { Red };
     * constexpr auto name = ::scl::enum_name<Color::Red>(); // Returns "Color::Red"
     * @endcode
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
     * @brief Retrieves the short string name of an enum member at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam V The enum value.
     * @return A ::std::string_view containing only the member name (e.g., "Red"), or an empty
     *         ::std::string_view where no constant of the type of @p V has the value @p V.
     *
     * @warning The result is for display: its spelling may change in a later version of
     * the module.
     * 
     * @details This is a convenience wrapper that takes the result of ::scl::enum_name<V>() 
     * and strips all type and namespace qualifiers.
     * 
     * @code
     * namespace gfx { enum class Mode { Fast }; }
     * constexpr auto name = ::scl::enum_short_name<gfx::Mode::Fast>(); // Returns "Fast"
     * @endcode
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
