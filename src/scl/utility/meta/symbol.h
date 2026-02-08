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
 * - scl::symbol_name<S>:
 *     Extracts the fully qualified name of the symbol S (global function, class method, or data member).
 *     Performs aggressive cleanup of compiler decorations, address operators (&), parentheses, and whitespace.
 * - scl::symbol_short_name<S>:
 *     Extracts only the terminal identifier of the symbol S.
 *     Useful for reflection-like tasks where only the member or function name is required.
 */

inline void lah4l4tjla6_f() {}

namespace scl::detail
{
    template <auto S>
    constexpr std::string_view symbol_pattern_text()
    {
#ifdef __cpp_lib_source_location
        return std::source_location::current().function_name();
#elif defined(_MSC_VER)
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    inline static constexpr auto symbol_prefix_length()
    {
        return symbol_pattern_text<lah4l4tjla6_f>().find("lah4l4tjla6_f");
    }

    inline static constexpr auto symbol_suffix_length()
    {
        constexpr auto text = symbol_pattern_text<lah4l4tjla6_f>();
        return text.length() - symbol_prefix_length() - std::string_view("lah4l4tjla6_f").length();
    }
} // namespace scl::detail

namespace scl
{
    /**
     * @brief Retrieves the qualified string name of a symbol at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam S Pointer to the symbol (global function, class method, or data member).
     * @return A std::string_view containing the qualified name of the symbol.
     * 
     * @details This function extracts the symbol name from the decorated signature 
     * and performs aggressive cleanup of compiler-specific artifacts such as:
     * - Address operators ('&')
     * - Parentheses used for grouping (common on MSVC for global variables)
     * - Leading and trailing whitespace
     * 
     * @code
     * struct Processor { void run() {} };
     * constexpr auto name = scl::symbol_name<&Processor::run>(); // "Processor::run"
     * 
     * int global_var = 0;
     * constexpr auto var_name = scl::symbol_name<&global_var>(); // "global_var"
     * @endcode
     */
    template <auto S>
    constexpr std::string_view symbol_name()
    {
        constexpr auto text = detail::symbol_pattern_text<S>();
        constexpr auto suffix_length = detail::symbol_suffix_length();

#ifdef _MSC_VER
        // MSVC: Trim suffix and find the last space/bracket to skip the type name
        constexpr auto pre_result = text.substr(0, text.length() - suffix_length);
        constexpr auto pos = pre_result.find_last_of(" >");
        constexpr auto result = (pos != std::string_view::npos) ? pre_result.substr(pos + 1) : pre_result;
#else
        // GCC/Clang: Use stable prefix calculation
        constexpr auto prefix_length = detail::symbol_prefix_length();
        constexpr auto result = text.substr(prefix_length, text.length() - prefix_length - suffix_length);
#endif

        constexpr auto junk = " ()&";
        constexpr auto begin_pos = result.find_first_not_of(junk);
        if constexpr (begin_pos == std::string_view::npos)
            return {};
        constexpr auto last_pos = result.find_last_not_of(junk);
        return result.substr(begin_pos, last_pos - begin_pos + 1);
    }

    /**
     * @brief Retrieves the short string name of a symbol at compile-time.
     * @ingroup scl_utility_meta
     * 
     * @tparam S Pointer to the symbol.
     * @return A std::string_view containing only the symbol identifier (e.g., "run").
     * 
     * @details Strips all namespace and class qualifiers from the result of scl::symbol_name<S>().
     * Ideal for reflection-like tasks where only the member name is required.
     * 
     * @code
     * namespace sys { struct Clock { static void tick() {} }; }
     * constexpr auto name = scl::symbol_short_name<sys::Clock::tick>(); // "tick"
     * @endcode
     */
    template <auto S>
    constexpr std::string_view symbol_short_name()
    {
        constexpr auto result = symbol_name<S>();
        if constexpr (constexpr auto pos = result.find_last_of(':'); pos != std::string_view::npos)
            return result.substr(pos + 1);
        else
            return result;
    }

} // namespace scl
