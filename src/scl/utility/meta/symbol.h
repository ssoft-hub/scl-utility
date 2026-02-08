#pragma once

#include <string_view>

#ifdef __cpp_lib_source_location
#include <source_location>
#endif

/**
 * @file
 * @brief Compile-time symbol (function, method, property) name extraction (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::symbol_name<S>:
 *     Extracts the fully qualified name of the symbol S (global function, class method, or data member).
 *     Performs aggressive cleanup of compiler decorations, address operators (&), parentheses, and whitespace.
 * - ::scl::symbol_short_name<S>:
 *     Extracts only the terminal identifier of the symbol S.
 *     Useful for reflection-like tasks where only the member or function name is required.
 */

inline void lah4l4tjla6_f() {}

namespace scl::detail
{
    template <auto S>
    constexpr ::std::string_view symbol_pattern_text()
    {
#ifdef _MSC_VER
        return __FUNCSIG__;
#elif defined __cpp_lib_source_location
        return ::std::source_location::current().function_name();
#else
        return __PRETTY_FUNCTION__;
#endif
    }

#ifdef _MSC_VER
    // MSVC-specific: use bracket extraction like type_name
    constexpr ::std::size_t symbol_prefix_length()
    {
        constexpr auto text = symbol_pattern_text<lah4l4tjla6_f>();
        constexpr auto close_pattern = text.rfind(">(");
        if constexpr (close_pattern == ::std::string_view::npos)
            return 0;
        constexpr auto before_close = text.substr(0, close_pattern);
        constexpr auto open_bracket = before_close.find_last_of('<');
        return (open_bracket != ::std::string_view::npos) ? open_bracket + 1 : 0;
    }

    constexpr auto symbol_suffix_length()
    {
        constexpr auto text = symbol_pattern_text<lah4l4tjla6_f>();
        constexpr auto close_pattern = text.rfind(">(");
        return (close_pattern != ::std::string_view::npos) ? text.length() - close_pattern : 0;
    }

    // Helper to strip MSVC function signature decorations
    // Format: "void __cdecl Namespace::func(args)" -> "Namespace::func"
    constexpr ::std::string_view strip_msvc_function_decorations(::std::string_view name)
    {
        // Find the last space before the function name (after __cdecl/__stdcall/etc)
        auto last_space = name.find_last_of(' ');
        if (last_space != ::std::string_view::npos)
        {
            auto after_space = name.substr(last_space + 1);
            // Check if this looks like a function name (contains :: or just identifier before '(')
            auto paren = after_space.find('(');
            if (paren != ::std::string_view::npos)
            {
                // Return just the name before '('
                return after_space.substr(0, paren);
            }
        }
        return name;
    }
#else
    // GCC/Clang: use marker search
    constexpr auto symbol_prefix_length()
    {
        return symbol_pattern_text<lah4l4tjla6_f>().find("lah4l4tjla6_f");
    }

    constexpr auto symbol_suffix_length()
    {
        constexpr auto text = symbol_pattern_text<lah4l4tjla6_f>();
        return text.length() - symbol_prefix_length() - ::std::string_view("lah4l4tjla6_f").length();
    }
#endif
} // namespace scl::detail

namespace scl
{
    /**
     * @brief Retrieves the qualified string name of a symbol at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam S Pointer to the symbol (global function, class method, or data member).
     * @return A ::std::string_view containing the qualified name of the symbol.
     * 
     * @details This function extracts the symbol name from the decorated signature 
     * and performs aggressive cleanup of compiler-specific artifacts such as:
     * - Address operators ('&')
     * - Parentheses used for grouping (common on MSVC for global variables)
     * - Leading and trailing whitespace
     * 
     * @code
     * struct Processor { void run() {} };
     * constexpr auto name = ::scl::symbol_name<&Processor::run>(); // "Processor::run"
     * 
     * int global_var = 0;
     * constexpr auto var_name = ::scl::symbol_name<&global_var>(); // "global_var"
     * @endcode
     */
    template <auto S>
    constexpr ::std::string_view symbol_name()
    {
        constexpr auto text = detail::symbol_pattern_text<S>();
        constexpr auto prefix_length = detail::symbol_prefix_length();
        constexpr auto suffix_length = detail::symbol_suffix_length();

        constexpr auto result = text.substr(prefix_length, text.length() - prefix_length - suffix_length);

#ifdef _MSC_VER
        // MSVC includes function signature like "void __cdecl Name(args)"
        constexpr auto cleaned = detail::strip_msvc_function_decorations(result);
#else
        constexpr auto cleaned = result;
#endif

        // Clean up junk characters (spaces, parens, ampersands)
        constexpr auto junk = " ()&";
        constexpr auto begin_pos = cleaned.find_first_not_of(junk);
        if constexpr (begin_pos == ::std::string_view::npos)
            return {};
        constexpr auto last_pos = cleaned.find_last_not_of(junk);
        return cleaned.substr(begin_pos, last_pos - begin_pos + 1);
    }

    /**
     * @brief Retrieves the short string name of a symbol at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam S Pointer to the symbol.
     * @return A ::std::string_view containing only the symbol identifier (e.g., "run").
     * 
     * @details Strips all namespace and class qualifiers from the result of ::scl::symbol_name<S>().
     * Ideal for reflection-like tasks where only the member name is required.
     * 
     * @code
     * namespace sys { struct Clock { static void tick() {} }; }
     * constexpr auto name = ::scl::symbol_short_name<sys::Clock::tick>(); // "tick"
     * @endcode
     */
    template <auto S>
    constexpr ::std::string_view symbol_short_name()
    {
        constexpr auto result = symbol_name<S>();
        constexpr auto pos = result.find_last_of(':');
        if constexpr (pos != ::std::string_view::npos)
            return result.substr(pos + 1);
        else
            return result;
    }

} // namespace scl
