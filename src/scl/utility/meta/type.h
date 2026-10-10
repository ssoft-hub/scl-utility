#pragma once

#include <string_view>
#include <type_traits>

/**
 * @file
 * @brief Traits for compile-time type name extraction (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::type_name<T>:
 *     Extracts the fully qualified name of the template type T.
 *     The rendering is compiler-specific and is not normalized: MSVC keeps the
 *     'class'/'struct'/'union'/'enum' prefix, at the top level and inside template arguments
 *     alike, where GCC and Clang omit it, and the standard library decides how its own
 *     types spell themselves. It is meant for display, not for identity - ::scl::type_key
 *     is what compares.
 * - ::scl::type_short_name<T>:
 *     Extracts only the unqualified identifier of a class, union, enumeration or fundamental
 *     type T, with const, volatile and a reference dropped.
 *     Strips all leading namespace and class scope qualifiers, the MSVC
 *     'class'/'struct'/'union'/'enum' prefix, and the template arguments; the name the
 *     compiler generates for a closure type or an unnamed class or enumeration is kept whole.
 */

struct p8qim3n2a_t
{};

namespace scl::detail
{
    constexpr char char_or_null_at(::std::string_view str, ::std::size_t index) noexcept
    {
        return index < str.size() ? str.substr(index, 1).front() : '\0';
    }

    constexpr bool identifier_char(char ch) noexcept
    {
        return ch == '_' || (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
    }

    constexpr bool operator_symbol_char(char ch) noexcept
    {
        switch (ch)
        {
        case '<':
        case '>':
        case '=':
        case '!':
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
        case '&':
        case '|':
        case '~':
            return true;
        default:
            return false;
        }
    }

    constexpr bool escaped_at(::std::string_view str, ::std::size_t index) noexcept
    {
        auto const before = str.substr(0, index);
        auto const last_other = before.find_last_not_of('\\');
        auto const backslashes =
            last_other == ::std::string_view::npos ? before.size() : before.size() - last_other - 1;
        return backslashes % 2 != 0;
    }

    // Where a literal of a template argument that ends at an index opens, such as the first quote
    // of '(' or of "a(b"; the index itself where no literal ends there.
    constexpr ::std::size_t literal_start(::std::string_view str, ::std::size_t index) noexcept
    {
        char const ch = ::scl::detail::char_or_null_at(str, index);
        if (ch == '\'' && index >= 2 && ::scl::detail::char_or_null_at(str, index - 2) == '\'')
            return index - 2;
        if (ch != '"')
            return index;
        for (auto open = index; open > 0;)
        {
            open = str.rfind('"', open - 1);
            if (open == ::std::string_view::npos)
                return index;
            if (!::scl::detail::escaped_at(str, open))
                return open;
        }
        return index;
    }

    // The length of the operator symbol holding an angle bracket that opens a string, such as 2
    // for the <= of "<=(int)"; 0 where no such symbol opens it.
    constexpr ::std::size_t angle_operator_length(::std::string_view str) noexcept
    {
        if (str.starts_with("<=>") || str.starts_with("<<=") || str.starts_with(">>=") ||
            str.starts_with("->*"))
            return 3;
        auto const pair = str.substr(0, 2);
        if (pair == "<<" || pair == ">>" || pair == "<=" || pair == ">=" || pair == "->")
            return 2;
        return str.starts_with('<') || str.starts_with('>') ? 1 : 0;
    }

    // Where the keyword operator opens when the character at an index belongs to an angle-bracket
    // symbol after it, such as the < of operator<; the index itself otherwise.
    constexpr ::std::size_t
    operator_start(::std::string_view str, ::std::size_t index, bool shorter) noexcept
    {
        if (!::scl::detail::operator_symbol_char(::scl::detail::char_or_null_at(str, index)))
            return index;
        auto symbol = index;
        while (symbol > 0 &&
            ::scl::detail::operator_symbol_char(::scl::detail::char_or_null_at(str, symbol - 1)))
            --symbol;
        auto length = ::scl::detail::angle_operator_length(str.substr(symbol));
        // The shorter reading leaves the last > of ->, <=> or >> to a template argument list, as in
        // Holder<&operator->, where a pointer to operator- closes it.
        if (shorter && length > 1 && ::scl::detail::char_or_null_at(str, symbol + length - 1) == '>')
            --length;
        constexpr ::std::string_view keyword = "operator";
        auto const head = str.substr(0, str.substr(0, symbol).find_last_not_of(' ') + 1);
        if (!head.ends_with(keyword) || index >= symbol + length)
            return index;
        auto const start = head.size() - keyword.size();
        return start > 0 && ::scl::detail::identifier_char(::scl::detail::char_or_null_at(str, start - 1)) ? index : start;
    }

    // Whether the operator at an index is ->, <=> or >>, whose last > may close a template
    // argument list instead.
    constexpr bool glued_angle_symbol(::std::string_view str, ::std::size_t keyword) noexcept
    {
        auto const rest = str.substr(keyword + ::std::string_view{"operator"}.size());
        auto const start = rest.find_first_not_of(' ');
        auto const symbol = start == ::std::string_view::npos ? ::std::string_view{} : rest.substr(start);
        auto const length = ::scl::detail::angle_operator_length(symbol);
        return length > 1 && symbol.substr(0, length).ends_with('>');
    }

    struct scope_scan
    {
        ::std::size_t position;
        int bracket_depth;
        bool glued;
    };

    // The scan runs from the end, so the bare symbol MSVC writes for an operator scope, as in
    // <=::Local, lies before the last '::' and cannot hide it.
    constexpr scope_scan scan_scopes(::std::string_view str, bool shorter) noexcept
    {
        scope_scan result{.position = ::std::string_view::npos, .bracket_depth = 0, .glued = false};
        for (auto index = str.size(); index > 0;)
        {
            --index;
            switch (char const ch = ::scl::detail::char_or_null_at(str, index))
            {
            case '\'':
            case '"':
                index = ::scl::detail::literal_start(str, index);
                break;
            case '<':
            case '>':
                if (auto const keyword = ::scl::detail::operator_start(str, index, shorter); keyword != index)
                {
                    result.glued = result.glued ||
                        (!shorter && ::scl::detail::glued_angle_symbol(str, keyword));
                    index = keyword;
                }
                else
                {
                    result.bracket_depth += ch == '>' ? 1 : -1;
                }
                break;
            case ')':
            case '}':
                ++result.bracket_depth;
                break;
            case '(':
            case '{':
                --result.bracket_depth;
                break;
            case ':':
                if (result.position == ::std::string_view::npos && result.bracket_depth == 0 &&
                    ::scl::detail::char_or_null_at(str, index - 1) == ':')
                {
                    result.position = index - 1;
                    // Only a glued symbol makes the whole name worth reading for its balance.
                    if (!result.glued)
                        return result;
                }
                break;
            default:
                break;
            }
        }
        return result;
    }

    // Only a name whose brackets fail to balance under the longest reading takes the shorter one.
    constexpr auto find_last_scope_operator(::std::string_view str) noexcept
    {
        auto const longest = ::scl::detail::scan_scopes(str, false);
        if (!longest.glued || longest.bracket_depth == 0)
            return longest.position;
        auto const shorter = ::scl::detail::scan_scopes(str, true);
        return shorter.bracket_depth == 0 ? shorter.position : longest.position;
    }

    constexpr ::std::string_view short_name_from(::std::string_view full) noexcept
    {
        auto const last_pos = find_last_scope_operator(full);
        auto const after = (last_pos != ::std::string_view::npos) ? full.substr(last_pos + 2) : full;
        auto const stripped = after.starts_with("struct ") ? after.substr(7)
            : after.starts_with("class ")                  ? after.substr(6)
            : after.starts_with("union ")                  ? after.substr(6)
            : after.starts_with("enum ")
            ? after.substr(5)
            : after;
        // A compiler-generated name carries no template argument list of its own.
        auto const generated = stripped.starts_with('<') || stripped.starts_with('{') ||
            stripped.starts_with('(');
        auto const tmpl = generated ? ::std::string_view::npos : stripped.find('<');
        return (tmpl != ::std::string_view::npos) ? stripped.substr(0, tmpl) : stripped;
    }

    namespace concepts
    {
        // A pointer, an array, a function or a pointer to a member spells a declarator around the
        // name, which short_name_from does not take apart.
        template <typename T>
        concept short_named = ::std::is_class_v<T> || ::std::is_union_v<T> || ::std::is_enum_v<T> ||
            ::std::is_fundamental_v<T>;
    } // namespace concepts

    template <typename T>
    constexpr ::std::string_view type_name_pattern_text() noexcept
    {
#if defined(_MSC_VER) && !defined(__clang__)
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

#if defined(_MSC_VER) && !defined(__clang__)
    // MSVC renders the signature as "ReturnType __cdecl FunctionName<TYPE>(void)".
    constexpr ::std::size_t type_name_pattern_prefix_length() noexcept
    {
        constexpr auto text = type_name_pattern_text<p8qim3n2a_t>();
        constexpr auto close_pattern = text.rfind(">(");
        if constexpr (close_pattern == ::std::string_view::npos)
            return 0;
        constexpr auto before_close = text.substr(0, close_pattern);
        constexpr auto open_bracket = before_close.find_last_of('<');
        return (open_bracket != ::std::string_view::npos) ? open_bracket + 1 : 0;
    }

    constexpr ::std::size_t type_name_pattern_suffix_length() noexcept
    {
        constexpr auto text = type_name_pattern_text<p8qim3n2a_t>();
        // Find the closing > followed by (void) pattern
        constexpr auto close_pattern = text.rfind(">(");
        return (close_pattern != ::std::string_view::npos) ? text.length() - close_pattern : 0;
    }
#else
    // GCC/Clang/Clang-on-MSVC: use marker search
    constexpr auto type_name_pattern_prefix_length() noexcept
    {
        constexpr auto prefix_length = type_name_pattern_text<p8qim3n2a_t>().find("p8qim3n2a_t");
        return prefix_length;
    }

    constexpr auto type_name_pattern_suffix_length() noexcept
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
     * @note On MSVC, the output includes the 'struct ', 'class ', 'union ' and
     * 'enum ' keywords as part of the type name (e.g., "struct MyType" instead of
     * "MyType", "enum Color" instead of "Color"), inside template arguments as well
     * as at the top level. This differs from GCC/Clang which omit these keywords.
     *
     * @warning The result is for display, not for identity. No part of it is
     * guaranteed to agree between compilers or standard libraries, so it must not
     * be compared against a literal, parsed, or persisted, and its spelling may change in a
     * later version of the module. ::scl::type_key is the comparable type identity;
     * ::scl::type_short_name is the bare identifier of a namespace-scope type.
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
    [[nodiscard]]
    constexpr auto type_name() noexcept
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
     * @tparam T The type whose name needs to be extracted: a class, a union, an enumeration or a
     *         fundamental type, with or without const, volatile and a reference. A pointer, an
     *         array, a function or a pointer to a member does not satisfy the constraint.
     * @return A ::std::string_view containing the name of the type without const, volatile, a
     *         reference, namespaces, class qualifiers, or template arguments; for a closure type or
     *         an unnamed class or enumeration, the name the compiler generates.
     *
     * @details This function first extracts the full name using ::scl::type_name<T>() of the type
     * with const, volatile and a reference removed, then strips all leading namespace and class
     * scopes by finding the last '::' delimiter outside brackets, and finally removes template
     * arguments by cutting off everything from '<' onwards. A name that opens with a bracket, which
     * a compiler generates for a closure type or an unnamed class or enumeration, is kept whole.
     *
     * @warning The result is for display, as the result of ::scl::type_name<T>() is: its
     * spelling differs between compilers and may change in a later version of the module.
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
    [[nodiscard]]
    constexpr auto type_short_name() noexcept
        requires(::scl::detail::concepts::short_named<::std::remove_cvref_t<T>>)
    {
        return detail::short_name_from(type_name<::std::remove_cvref_t<T>>());
    }

} // namespace scl
