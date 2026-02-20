#pragma once

#include <string_view>
#include <type_traits>

#ifdef __cpp_lib_source_location
#include <source_location>
#endif

/**
 * @file
 * @brief Compile-time enumeration introspection utilities (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::enum_name<V>:
 *     Retrieves the qualified string name of the enum member V.
 *     Ensures cross-platform consistency by stripping the 'enum ' prefix on MSVC.
 * - ::scl::enum_short_name<V>:
 *     Retrieves only the identifier of the enum member V without type or namespace qualifiers.
 * - ::scl::Enum:
 *     Concept that validates if a type is an enumeration (scoped or unscoped).
 */

enum we5r256sg_e // NOLINT(cppcoreguidelines-use-enum-class, performance-enum-size)
{
    we5r256sg_v
};

namespace scl::detail
{
    template <auto V>
        requires ::std::is_enum_v<decltype(V)>
    constexpr ::std::string_view enum_name_pattern_text()
    {
#ifdef _MSC_VER
        return __FUNCSIG__;
#elif defined __cpp_lib_source_location
        return ::std::source_location::current().function_name();
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    constexpr auto enum_prefix_lenght()
    {
        constexpr auto msvc_prefix_length =
            enum_name_pattern_text<we5r256sg_e::we5r256sg_v>().find("enum we5r256sg_e::we5r256sg_v");
        constexpr auto full_prefix_length =
            enum_name_pattern_text<we5r256sg_e::we5r256sg_v>().find("we5r256sg_e::we5r256sg_v");
        constexpr auto short_prefix_length = enum_name_pattern_text<we5r256sg_e::we5r256sg_v>().find("we5r256sg_v");
        if constexpr (msvc_prefix_length != ::std::string_view::npos)
            return msvc_prefix_length;
        else if constexpr (full_prefix_length != ::std::string_view::npos)
            return full_prefix_length;
        else if constexpr (short_prefix_length != ::std::string_view::npos)
            return short_prefix_length;
        else
            return 0;
    }

    constexpr auto enum_suffix_lenght()
    {
        constexpr auto text = enum_name_pattern_text<we5r256sg_v>(); // NOLINT(cppcoreguidelines-use-enum-class)
        return text.length() - enum_prefix_lenght() - ::std::string_view("we5r256sg_v").length();
    }
} // namespace scl::detail

namespace scl
{
    /**
     * @brief Retrieves the qualified string name of an enum member at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam V The enum value whose name needs to be extracted.
     * @return A ::std::string_view containing the qualified name of the enum member.
     * 
     * @details Extracts the enum member name from the function signature. On MSVC, 
     * it automatically removes the 'enum ' prefix for consistency.
     * 
     * @note The result usually includes the enum type name (e.g., "Color::Red") 
     * depending on how the enum was defined and the compiler used.
     * 
     * @code
     * enum class Color { Red };
     * constexpr auto name = ::scl::enum_name<Color::Red>(); // Returns "Color::Red"
     * @endcode
     */
    template <auto V>
        requires ::std::is_enum_v<decltype(V)>
    constexpr ::std::string_view enum_name()
    {
        constexpr auto text = detail::enum_name_pattern_text<V>();
        constexpr auto prefix_length = detail::enum_prefix_lenght();
        constexpr auto suffix_length = detail::enum_suffix_lenght();

        constexpr auto result = text.substr(prefix_length, text.length() - prefix_length - suffix_length);

        // Strip MSVC-specific 'enum ' prefix
        if constexpr (result.starts_with("enum "))
            return result.substr(5);
        else
            return result;
    }

    /**
     * @brief Retrieves the short string name of an enum member at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam V The enum value.
     * @return A ::std::string_view containing only the member name (e.g., "Red").
     * 
     * @details This is a convenience wrapper that takes the result of ::scl::enum_name<V>() 
     * and strips all type and namespace qualifiers.
     * 
     * @code
     * namespace gfx { enum class Mode { Fast }; }
     * constexpr auto name = ::scl::enum_short_name<gfx::Mode::Fast>(); // Returns "Fast"
     * @endcode
     */
    template <auto V>
        requires ::std::is_enum_v<decltype(V)>
    constexpr ::std::string_view enum_short_name()
    {
        constexpr auto result = enum_name<V>();
        constexpr auto pos = result.find_last_of(':');
        if constexpr (pos != ::std::string_view::npos)
            return result.substr(pos + 1);
        else
            return result;
    }

} // namespace scl
