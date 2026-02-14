#pragma once

#ifdef __cpp_lib_source_location
#include <source_location>
#endif

#include <string_view>

/**
 * @file
 * @brief Traits for compile-time type name extraction (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::type_name<T>:
 *     Extracts the fully qualified name of the template type T.
 *     Automatically normalizes compiler-specific output by stripping 'class'/'struct'/'union' prefixes on MSVC.
 * - ::scl::type_short_name<T>:
 *     Extracts only the unqualified identifier of the type T.
 *     Strips all leading namespace and class scope qualifiers.
 */

struct p8qim3n2a_t
{};

namespace scl::detail
{
    // Helper to find the last "::" that is NOT inside angle brackets
    inline constexpr auto find_last_scope_operator(::std::string_view str)
    {
        auto last_pos = ::std::string_view::npos;
        int bracket_depth = 0;

        for (::std::size_t i = 0; i < str.size(); ++i)
            if (str[i] == '<')
                ++bracket_depth;
            else if (str[i] == '>')
                --bracket_depth;
            else if (bracket_depth == 0 && str[i] == ':' && i + 1 < str.size() && str[i + 1] == ':')
                last_pos = i;

        return last_pos;
    }

    template <typename T>
    constexpr ::std::string_view type_name_pattern_text()
    {
#if defined(_MSC_VER)
        return __FUNCSIG__;
#elif defined __cpp_lib_source_location
        return ::std::source_location::current().function_name();
#else
        return __PRETTY_FUNCTION__;
#endif
    }

#if defined(_MSC_VER)
    // MSVC-specific extraction using angle brackets
    // Format: "ReturnType __cdecl FunctionName<TYPE>(void)"
    // We need to find the last < before >(void) pattern
    inline constexpr ::std::size_t type_name_pattern_prefix_length()
    {
        constexpr auto text = type_name_pattern_text<p8qim3n2a_t>();
        // Find the closing pattern >( first
        constexpr auto close_pattern = text.rfind(">(");
        if constexpr (close_pattern == ::std::string_view::npos)
            return 0;
        // Find the last < before the closing pattern
        constexpr auto before_close = text.substr(0, close_pattern);
        constexpr auto open_bracket = before_close.find_last_of('<');
        return (open_bracket != ::std::string_view::npos) ? open_bracket + 1 : 0;
    }

    inline constexpr ::std::size_t type_name_pattern_suffix_length()
    {
        constexpr auto text = type_name_pattern_text<p8qim3n2a_t>();
        // Find the closing > followed by (void) pattern
        constexpr auto close_pattern = text.rfind(">(");
        return (close_pattern != ::std::string_view::npos) ? text.length() - close_pattern : 0;
    }
#else
    // GCC/Clang-specific extraction using marker search
    inline constexpr auto type_name_pattern_prefix_length()
    {
        constexpr auto prefix_length = type_name_pattern_text<p8qim3n2a_t>().find("p8qim3n2a_t");
        return prefix_length;
    }

    inline constexpr auto type_name_pattern_suffix_length()
    {
        constexpr auto suffix = type_name_pattern_text<p8qim3n2a_t>().length() -
            type_name_pattern_prefix_length() - ::std::string_view("p8qim3n2a_t").length();
        return suffix;
    }
#endif

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Retrieves the string name of the template type T at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam T The type whose name needs to be extracted.
     * @return A ::std::string_view containing the human-readable name of the type.
     * 
     * @details This function uses compiler-specific macros (`__FUNCSIG__` on MSVC 
     * and `__PRETTY_FUNCTION__` on GCC/Clang) to extract the type name from 
     * the decorated function signature. The result is evaluated at compile-time.
     * 
     * @note On MSVC, the output includes 'struct ', 'class ', 'union ' keywords
     * as part of the type name (e.g., "struct MyType" instead of "MyType").
     * This differs from GCC/Clang which omit these keywords.
     * 
     * @code
     * struct MyType {};
     * constexpr auto name = ::scl::type_name<MyType>(); 
     * // MSVC: Returns "struct MyType"
     * // GCC/Clang: Returns "MyType"
     * constexpr auto int_name = ::scl::type_name<int>(); // Returns "int" on all compilers
     * @endcode
     */
    template <typename T>
    inline constexpr auto type_name()
    {
        constexpr auto pattern_text = detail::type_name_pattern_text<T>();
        constexpr auto prefix_length = detail::type_name_pattern_prefix_length();
        constexpr auto suffix_length = detail::type_name_pattern_suffix_length();

        static_assert(!pattern_text.empty());

        constexpr auto text_length = pattern_text.length() - prefix_length - suffix_length;
        return pattern_text.substr(prefix_length, text_length);
    }

    /**
     * @brief Retrieves the short name of the template type T at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam T The type whose name needs to be extracted.
     * @return A ::std::string_view containing the name of the type without namespaces, class qualifiers, or template arguments.
     * 
     * @details This function first extracts the full name using ::scl::type_name<T>(), 
     * then strips all leading namespace and class scopes by finding the last '::' delimiter,
     * and finally removes template arguments by cutting off everything from '<' onwards.
     * 
     * @code
     * namespace app::core { 
     *     template<typename T> struct Task {}; 
     * }
     * constexpr auto full = ::scl::type_name<app::core::Task<int>>();  // "app::core::Task<int>"
     * constexpr auto name = ::scl::type_short_name<app::core::Task<int>>(); // "Task"
     * @endcode
     */
    template <typename T>
    inline constexpr auto type_short_name()
    {
        constexpr auto result = type_name<T>();

        // Find the last "::" that is NOT inside template arguments (angle brackets)
        constexpr auto last_scope_pos = detail::find_last_scope_operator(result);

        constexpr auto after_scope =
            (last_scope_pos != ::std::string_view::npos) ? result.substr(last_scope_pos + 2) : result;

        // Remove struct/class/union prefix from short name
        constexpr auto without_prefix = after_scope.starts_with("struct ") ? after_scope.substr(7)
            : after_scope.starts_with("class ")                            ? after_scope.substr(6)
            : after_scope.starts_with("union ")                            ? after_scope.substr(6)
            : after_scope.starts_with("enum ")
            ? after_scope.substr(5)
            : after_scope;

        // Remove template arguments (everything from '<' onwards)
        constexpr auto template_start = without_prefix.find('<');
        if constexpr (template_start != ::std::string_view::npos)
            return without_prefix.substr(0, template_start);
        else
            return without_prefix;
    }

} // namespace scl
