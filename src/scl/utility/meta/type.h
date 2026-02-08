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
 * - scl::type_name<T>:
 *     Extracts the fully qualified name of the template type T.
 *     Automatically normalizes compiler-specific output by stripping 'class'/'struct'/'union' prefixes on MSVC.
 * - scl::type_short_name<T>:
 *     Extracts only the unqualified identifier of the type T.
 *     Strips all leading namespace and class scope qualifiers.
 */

struct p8qim3n2a_t
{};

namespace scl::detail
{

    template <typename T>
    constexpr ::std::string_view type_name_pattern_text()
    {
#ifdef __cpp_lib_source_location
        return ::std::source_location::current().function_name();
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    inline static constexpr auto type_name_pattern_prefix_length()
    {
        constexpr auto msvc_prefix_length = type_name_pattern_text<p8qim3n2a_t>().find("struct p8qim3n2a_t");
        constexpr auto prefix_length = type_name_pattern_text<p8qim3n2a_t>().find("p8qim3n2a_t");
        if constexpr (msvc_prefix_length != ::std::string_view::npos)
            return msvc_prefix_length;
        else
            return prefix_length;
    }

    inline static constexpr auto type_name_pattern_suffix_length()
    {
        constexpr auto suffix = type_name_pattern_text<p8qim3n2a_t>().length() -
            type_name_pattern_prefix_length() - ::std::string_view("p8qim3n2a_t").length();
        return suffix;
    }

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Retrieves the string name of the template type T at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam T The type whose name needs to be extracted.
     * @return A std::string_view containing the human-readable name of the type.
     * 
     * @details This function uses compiler-specific macros (`__FUNCSIG__` on MSVC 
     * and `__PRETTY_FUNCTION__` on GCC/Clang) to extract the type name from 
     * the decorated function signature. The result is evaluated at compile-time.
     * 
     * @note On MSVC, the function automatically strips 'struct ', 'class ', 
     * and 'union ' prefixes from user-defined types to ensure cross-platform 
     * consistency with GCC and Clang.
     * 
     * @code
     * struct MyType {};
     * constexpr auto name = scl::type_name<MyType>(); // Returns "MyType"
     * constexpr auto int_name = scl::type_name<int>(); // Returns "int"
     * @endcode
     */
    template <typename T>
    constexpr auto type_name()
    {
        constexpr auto pattern_text = detail::type_name_pattern_text<T>();
        constexpr auto prefix_length = detail::type_name_pattern_prefix_length();
        constexpr auto suffix_length = detail::type_name_pattern_suffix_length();

        static_assert(!pattern_text.empty());

        constexpr auto text_length = pattern_text.length() - prefix_length - suffix_length;
        constexpr auto result = pattern_text.substr(prefix_length, text_length);

        if constexpr (result.starts_with("struct "))
            return result.substr(7);
        else if constexpr (result.starts_with("class "))
            return result.substr(6);
        else if constexpr (result.starts_with("union "))
            return result.substr(6);
        else
            return result;
    }

    /**
     * @brief Retrieves the short name of the template type T at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam T The type whose name needs to be extracted.
     * @return A std::string_view containing the name of the type without namespaces or class qualifiers.
     * 
     * @details This function first extracts the full name using scl::type_name<T>() and 
     * then strips all leading namespace and class scopes by finding the last '::' delimiter.
     * 
     * @code
     * namespace app::core { struct Task {}; }
     * constexpr auto full = scl::type_name<app::core::Task>();  // "app::core::Task"
     * constexpr auto name = scl::type_short_name<app::core::Task>(); // "Task"
     * @endcode
     */
    template <typename T>
    constexpr std::string_view type_short_name()
    {
        constexpr auto result = type_name<T>();
        if constexpr (constexpr auto pos = result.find_last_of(':'); pos != std::string_view::npos)
            return result.substr(pos + 1);
        else
            return result;
    }

} // namespace scl
