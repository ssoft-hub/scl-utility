#pragma once

#include <string_view>
#include <type_traits>
#include <utility>

/**
 * @file
 * @brief Names of types at compile time.
 * @ingroup scl_utility_meta
 */

namespace scl::detail
{
    // Its name occurs in the signature of its pattern function only at the template argument.
    struct p8qim3n2a_t
    {};

    struct anchor_span
    {
        ::std::size_t position;
        ::std::size_t length;
    };

    // GCC spells a type or a function of the namespace its enclosing function stands in without
    // that namespace, where Clang and MSVC spell the namespace too.
    constexpr ::scl::detail::anchor_span
    find_anchor(::std::string_view text, ::std::string_view qualified) noexcept
    {
        if (auto const position = text.find(qualified); position != ::std::string_view::npos)
            return {.position = position, .length = qualified.size()};
        auto const bare = qualified.substr(qualified.rfind(':') + 1);
        return {.position = text.find(bare), .length = bare.size()};
    }

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
    constexpr ::std::size_t operator_start(::std::string_view str, ::std::size_t index) noexcept
    {
        if (!::scl::detail::operator_symbol_char(::scl::detail::char_or_null_at(str, index)))
            return index;
        auto symbol = index;
        while (symbol > 0 &&
            ::scl::detail::operator_symbol_char(::scl::detail::char_or_null_at(str, symbol - 1)))
            --symbol;
        auto const length = ::scl::detail::angle_operator_length(str.substr(symbol));
        constexpr ::std::string_view keyword = "operator";
        auto const head = str.substr(0, str.substr(0, symbol).find_last_not_of(' ') + 1);
        if (!head.ends_with(keyword) || index >= symbol + length)
            return index;
        auto const start = head.size() - keyword.size();
        return start > 0 && ::scl::detail::identifier_char(::scl::detail::char_or_null_at(str, start - 1)) ? index : start;
    }

    // Whether the operator at an index is ->, ->*, <=> or >>, whose last > may close a template
    // argument list instead.
    constexpr bool glued_angle_symbol(::std::string_view str, ::std::size_t keyword) noexcept
    {
        auto const rest = str.substr(keyword + ::std::string_view{"operator"}.size());
        auto const start = rest.find_first_not_of(' ');
        auto const symbol = start == ::std::string_view::npos ? ::std::string_view{} : rest.substr(start);
        auto const length = ::scl::detail::angle_operator_length(symbol);
        auto const spelled = symbol.substr(0, length);
        return length > 1 && (spelled.ends_with('>') || spelled == "->*");
    }

    struct scope_scan
    {
        ::std::size_t position;
        int bracket_depth;
        unsigned glued_count;
    };

    // Where the scan from the end goes on after the angle bracket at an index.
    constexpr ::std::size_t
    scan_angle(::std::string_view str, ::std::size_t index, scope_scan & result, unsigned shorter_from_end) noexcept
    {
        auto const keyword = ::scl::detail::operator_start(str, index);
        if (keyword == index)
        {
            result.bracket_depth += ::scl::detail::char_or_null_at(str, index) == '>' ? 1 : -1;
            return index;
        }
        if (!::scl::detail::glued_angle_symbol(str, keyword))
            return keyword;
        // No compiler writes :: right after the keyword operator and its symbol, so the > of
        // operator->:: closes an argument list.
        auto const shorter = str.substr(index + 1).starts_with("::") || result.glued_count++ < shorter_from_end;
        if (!shorter)
            return keyword;
        ++result.bracket_depth;
        return keyword;
    }

    // The scan runs from the end, so the bare symbol MSVC writes for an operator scope, as in
    // <=::Local, lies before the last '::' and cannot hide it.
    constexpr scope_scan scan_scopes(::std::string_view str, unsigned shorter_from_end) noexcept
    {
        scope_scan result{.position = ::std::string_view::npos, .bracket_depth = 0, .glued_count = 0};
        for (auto index = str.size(); index > 0;)
        {
            --index;
            switch (::scl::detail::char_or_null_at(str, index))
            {
            case '\'':
            case '"':
                index = ::scl::detail::literal_start(str, index);
                break;
            case '<':
            case '>':
                index = ::scl::detail::scan_angle(str, index, result, shorter_from_end);
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
                    if (result.glued_count == 0)
                        return result;
                }
                break;
            default:
                break;
            }
        }
        return result;
    }

    constexpr auto find_last_scope_operator(::std::string_view str) noexcept
    {
        auto const longest = ::scl::detail::scan_scopes(str, 0U);
        // Each shorter reading counts one more >.
        auto const missing = -longest.bracket_depth;
        if (missing <= 0 || ::std::cmp_greater(missing, longest.glued_count))
            return longest.position;
        // Given to the glued symbols nearest the end, the shorter readings keep the depth highest at
        // every point, so no other reading balances where this one fails.
        return ::scl::detail::scan_scopes(str, static_cast<unsigned>(missing)).position;
    }

    constexpr ::std::string_view short_name_from(::std::string_view full) noexcept
    {
        auto const last_pos = ::scl::detail::find_last_scope_operator(full);
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
        // A pointer, an array, a function or a pointer to a member spells a declarator around
        // the name, which short_name_from does not take apart.
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
        constexpr auto text = ::scl::detail::type_name_pattern_text<p8qim3n2a_t>();
        constexpr auto close_pattern = text.rfind(">(");
        if constexpr (close_pattern == ::std::string_view::npos)
            return 0;
        constexpr auto before_close = text.substr(0, close_pattern);
        constexpr auto open_bracket = before_close.find_last_of('<');
        return (open_bracket != ::std::string_view::npos) ? open_bracket + 1 : 0;
    }

    constexpr ::std::size_t type_name_pattern_suffix_length() noexcept
    {
        constexpr auto text = ::scl::detail::type_name_pattern_text<p8qim3n2a_t>();
        constexpr auto close_pattern = text.rfind(">(");
        return (close_pattern != ::std::string_view::npos) ? text.length() - close_pattern : 0;
    }
#else
    constexpr auto type_name_pattern_prefix_length() noexcept
    {
        constexpr auto text = ::scl::detail::type_name_pattern_text<p8qim3n2a_t>();
        return ::scl::detail::find_anchor(text, "scl::detail::p8qim3n2a_t").position;
    }

    constexpr auto type_name_pattern_suffix_length() noexcept
    {
        constexpr auto text = ::scl::detail::type_name_pattern_text<p8qim3n2a_t>();
        constexpr auto anchor = ::scl::detail::find_anchor(text, "scl::detail::p8qim3n2a_t");
        return text.length() - anchor.position - anchor.length;
    }
#endif

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Returns the name of the type T as the compiler spells it.
     * @ingroup scl_utility_meta
     *
     * @tparam T The type to name.
     * @return The name with the namespaces, the classes, `const`, `volatile` and the reference
     *         the type has, such as `const app::Item &` for `app::Item const &` with Clang.
     *
     * The name is taken at compile time from the signature the compiler gives a function template:
     * `__FUNCSIG__` with MSVC, `__PRETTY_FUNCTION__` with GCC and Clang. MSVC writes the key
     * `class`, `struct`, `union` or `enum` before the name of a class, a union or an enumeration,
     * at the top level and in template arguments alike, and GCC and Clang write none. GCC spells
     * a type of the standard library with every namespace its library declares it in, an inline one
     * included, such as `std::__cxx11` with libstdc++, and Clang leaves an inline namespace out.
     * MSVC writes the default arguments of the template of such a type, which GCC and Clang leave
     * out.
     *
     * @warning The result is for display, not for identity: it differs between compilers and
     * standard libraries, and its spelling may change in a later version of the module.
     * Types are compared by the keys ::scl::type_key_of returns.
     *
     * @snippet meta/type_name/meta_type_name_example.cpp type_name
     */
    template <typename T>
    [[nodiscard]]
    constexpr ::std::string_view type_name() noexcept
    {
        constexpr auto pattern_text = ::scl::detail::type_name_pattern_text<T>();
        constexpr auto prefix_length = ::scl::detail::type_name_pattern_prefix_length();
        constexpr auto suffix_length = ::scl::detail::type_name_pattern_suffix_length();

        static_assert(!pattern_text.empty());

        constexpr auto text_length = pattern_text.length() - prefix_length - suffix_length;
        return pattern_text.substr(prefix_length, text_length);
    }

    /**
     * @brief Returns the name of the type T without qualifiers and template arguments.
     * @ingroup scl_utility_meta
     *
     * @tparam T The type to name: a class, a union, an enumeration or a fundamental type, with or
     *         without `const`, `volatile` and a reference.
     * @return For a class, a union or an enumeration, its identifier, such as `Task` for
     *         `app::Task<int> const &`; for a closure type or an unnamed class or enumeration,
     *         the name the compiler generates, such as `<lambda()>` with GCC; for a fundamental
     *         type, the name the compiler spells it with, without its namespace, such as `int`, or
     *         `nullptr_t` for `std::nullptr_t` with GCC and Clang and `nullptr` with MSVC.
     *
     * In the declaration, the constraint `class_enum_or_fundamental` is satisfied by a class,
     * a union, an enumeration and a fundamental type, with or without `const`, `volatile` and
     * a reference. A pointer, an array, a function and a pointer to a member do not satisfy it.
     *
     * @warning The result is for display, as the result of ::scl::type_name<T>() is: its
     * spelling differs between compilers and may change in a later version of the module.
     *
     * @note GCC and Clang spell a pointer to `operator-` before a closing `>` as `operator->`, so
     * two types can get one name. Take a class `P` with both operators and class templates `Holder`
     * and `Wrap`, each with a member template `Inner`. Clang names both
     * `Wrap<Holder<&P::operator- >>::Inner<&P::operator-> >` and
     * `Wrap<Holder<&P::operator-> >::Inner<&P::operator- >>` as
     * `Wrap<Holder<&P::operator->>::Inner<&P::operator->>`, so the function cannot tell the two
     * apart and returns `Wrap` for both, while the first is a specialization of `Inner`.
     *
     * @snippet meta/type_name/meta_type_name_example.cpp type_short_name
     */
    template <typename T>
    [[nodiscard]]
    constexpr ::std::string_view type_short_name() noexcept
#ifdef SCL_DOXYGEN
        requires(see below class_enum_or_fundamental)
#else
        requires(::scl::detail::concepts::short_named<::std::remove_cvref_t<T>>)
#endif
    {
        return ::scl::detail::short_name_from(::scl::type_name<::std::remove_cvref_t<T>>());
    }

} // namespace scl
