#pragma once

#include <scl/utility/meta/type.h>

#include <string_view>

/**
 * @file
 * @brief Names of functions, variables and members at compile time.
 * @ingroup scl_utility_meta
 */

namespace scl::detail
{
    // Its name occurs in the signature of its pattern function only at the template argument.
    constexpr void lah4l4tjla6_f() noexcept {}

    template <auto S>
    constexpr ::std::string_view symbol_pattern_text() noexcept
    {
#if defined(_MSC_VER) && !defined(__clang__)
        return __FUNCSIG__;
#else
        return __PRETTY_FUNCTION__;
#endif
    }

    constexpr bool enclosed_whole(::std::string_view text) noexcept
    {
        int depth = 0;
        for (::std::size_t index = 0; index < text.size(); ++index)
        {
            auto const character = ::scl::detail::char_or_null_at(text, index);
            depth += character == '(' ? 1 : character == ')' ? -1 : 0;
            if (depth == 0)
                return index + 1 == text.size();
        }
        return false;
    }

    // Clang opens a name with (anonymous namespace), whose parentheses belong to the name.
    constexpr ::std::string_view bare_symbol(::std::string_view text) noexcept
    {
        while (true)
        {
            auto const first = text.find_first_not_of(' ');
            if (first == ::std::string_view::npos)
                return {};
            text = text.substr(first, text.find_last_not_of(' ') - first + 1);
            if (text.starts_with('&'))
                text.remove_prefix(1);
            else if (text.starts_with('(') && ::scl::detail::enclosed_whole(text))
                text = text.substr(1, text.size() - 2);
            else
                return text;
        }
    }

#if defined(_MSC_VER) && !defined(__clang__)
    constexpr ::std::size_t symbol_prefix_length() noexcept
    {
        constexpr auto text = ::scl::detail::symbol_pattern_text<lah4l4tjla6_f>();
        constexpr auto close_pattern = text.rfind(">(");
        if constexpr (close_pattern == ::std::string_view::npos)
            return 0;
        constexpr auto before_close = text.substr(0, close_pattern);
        constexpr auto open_bracket = before_close.find_last_of('<');
        return (open_bracket != ::std::string_view::npos) ? open_bracket + 1 : 0;
    }

    constexpr auto symbol_suffix_length() noexcept
    {
        constexpr auto text = ::scl::detail::symbol_pattern_text<lah4l4tjla6_f>();
        constexpr auto close_pattern = text.rfind(">(");
        return (close_pattern != ::std::string_view::npos) ? text.length() - close_pattern : 0;
    }

    // Where the keyword operator, which ends a text, opens; npos where no such keyword ends it.
    constexpr ::std::size_t msvc_operator_start(::std::string_view text) noexcept
    {
        constexpr ::std::string_view keyword = "operator";
        auto const end = text.find_last_not_of(' ');
        if (end == ::std::string_view::npos || !text.substr(0, end + 1).ends_with(keyword))
            return ::std::string_view::npos;
        auto const start = end + 1 - keyword.size();
        return start > 0 && ::scl::detail::identifier_char(::scl::detail::char_or_null_at(text, start - 1))
            ? ::std::string_view::npos
            : start;
    }

    // Where the symbol after the keyword operator at an index ends, () and spaces included.
    constexpr ::std::size_t msvc_operator_symbol_end(::std::string_view text, ::std::size_t index) noexcept
    {
        index = text.find_first_not_of(' ', index);
        if (index == ::std::string_view::npos)
            return text.size();
        if (text.substr(index).starts_with("()"))
            return index + 2;
        while (index < text.size() &&
            ::scl::detail::operator_symbol_char(::scl::detail::char_or_null_at(text, index)))
            ++index;
        return index;
    }

    // Where the scan of a declarator goes on after an index: past the symbol of an operator whose
    // keyword ends right before the index, or at the index itself.
    constexpr ::std::size_t msvc_operator_skip(::std::string_view text, ::std::size_t index) noexcept
    {
        if (::scl::detail::msvc_operator_start(text.substr(0, index)) == ::std::string_view::npos)
            return index;
        auto const end = ::scl::detail::msvc_operator_symbol_end(text, index);
        return end > index ? end : index;
    }

    // In an argument of a class template, MSVC writes a static data member of a pointer to function
    // type as (__cdecl* name)(int).
    constexpr ::std::string_view msvc_pointer_group(::std::string_view text, ::std::size_t open) noexcept
    {
        auto const inner = text.substr(open + 1);
        auto const star = inner.find('*');
        if (!inner.starts_with("__") || star == ::std::string_view::npos)
            return {};
        auto const name = inner.substr(star + 1);
        return name.substr(0, name.find(')'));
    }

    // The part of a declarator MSVC writes that ends at the parameter list outside angle brackets.
    constexpr ::std::string_view msvc_declarator_head(::std::string_view text) noexcept
    {
        int depth = 0;
        bool quoted = false;
        for (::std::size_t index = 0; index < text.size(); ++index)
        {
            auto const character = ::scl::detail::char_or_null_at(text, index);
            if (character == '`' || character == '\'')
                quoted = character == '`';
            else if (quoted)
                continue;
            else if (auto const skip = ::scl::detail::msvc_operator_skip(text, index); skip != index)
                index = skip - 1;
            else if (character == '<' || character == '>')
                depth += character == '<' ? 1 : -1;
            else if (character == '(' && depth == 0)
                return ::scl::detail::msvc_pointer_group(text, index).empty()
                    ? text.substr(0, index)
                    : ::scl::detail::msvc_pointer_group(text, index);
        }
        return text;
    }

    // Where the last keyword operator outside angle brackets opens; npos where there is none.
    constexpr ::std::size_t msvc_operator_keyword(::std::string_view text) noexcept
    {
        auto found = ::std::string_view::npos;
        int depth = 0;
        bool quoted = false;
        for (::std::size_t index = 0; index < text.size(); ++index)
        {
            auto const character = ::scl::detail::char_or_null_at(text, index);
            if (character == '`' || character == '\'')
                quoted = character == '`';
            else if (quoted)
                continue;
            else if (auto const skip = ::scl::detail::msvc_operator_skip(text, index); skip != index)
            {
                if (depth == 0)
                    found = ::scl::detail::msvc_operator_start(text.substr(0, index));
                index = skip - 1;
            }
            else if (character == '<' || character == '>')
                depth += character == '<' ? 1 : -1;
        }
        return found;
    }

    // The name a declarator head ends with: after the last space outside angle brackets and
    // the quotation of `anonymous namespace', an operator kept whole from its keyword.
    constexpr ::std::string_view msvc_entity_name(::std::string_view text) noexcept
    {
        auto const head = ::scl::detail::msvc_declarator_head(text);
        auto const keyword = ::scl::detail::msvc_operator_keyword(head);
        auto index = keyword != ::std::string_view::npos ? keyword : head.size();
        int depth = 0;
        bool quoted = false;
        for (; index > 0; --index)
        {
            auto const character = ::scl::detail::char_or_null_at(head, index - 1);
            if (character == '\'' || character == '`')
                quoted = character == '\'';
            else if (quoted)
                continue;
            else if (auto const start = ::scl::detail::operator_start(head, index - 1); start != index - 1)
                index = start + 1;
            else if (character == '<' || character == '>')
                depth += character == '>' ? 1 : -1;
            else if (character == ' ' && depth == 0)
                break;
        }
        return head.substr(index);
    }

    // MSVC writes a pointer to a member function of a class with several bases or a virtual base
    // as an aggregate of the pointer and one or two offsets, {&Class::name,0} or {&Class::name,0,0}.
    constexpr ::std::string_view msvc_member_pointer_target(::std::string_view text) noexcept
    {
        if (!text.starts_with('{') || !text.ends_with('}'))
            return text;
        text = text.substr(1, text.size() - 2);
        for (int field = 0; field < 2; ++field)
        {
            auto const comma = text.find_last_of(',');
            if (comma == ::std::string_view::npos || comma + 1 == text.size() ||
                text.find_first_not_of("-0123456789", comma + 1) != ::std::string_view::npos)
                break;
            text = text.substr(0, comma);
        }
        return text;
    }

    // The thunk of such an aggregate reads [thunk]: __cdecl Class::`vcall'{8,{flat}}' }', whose name
    // ends at the quote after the brace closing the first brace following `vcall', or, for
    // an operator, Class::operator~{8,{flat}}' }', whose name ends before its first brace.
    constexpr ::std::string_view msvc_thunk_name(::std::string_view text) noexcept
    {
        constexpr ::std::string_view thunk = "[thunk]: ";
        auto const at = text.find(thunk);
        if (at == ::std::string_view::npos)
            return text;
        text = text.substr(at + thunk.size());
        text = text.substr(text.find(' ') + 1);
        auto const open = text.find("'{");
        if (open == ::std::string_view::npos)
            return text.substr(0, text.find('{'));
        int depth = 0;
        for (auto index = open + 1; index < text.size(); ++index)
        {
            auto const character = ::scl::detail::char_or_null_at(text, index);
            depth += character == '{' ? 1 : character == '}' ? -1 : 0;
            if (depth == 0)
                return text.substr(0, index + 2);
        }
        return text;
    }

    template <auto S>
    struct symbol_holder
    {
        static constexpr ::std::string_view text() noexcept { return __FUNCSIG__; }
    };

    // MSVC drops the namespaces and classes of a static data member from the signature of a function
    // template, as in <&count>, and keeps them where the pointer is an argument of a class template.
    template <auto S>
    constexpr ::std::string_view msvc_qualified_symbol(::std::string_view name) noexcept
    {
        if (name.find(':') != ::std::string_view::npos)
            return name;
        constexpr auto text = ::scl::detail::symbol_holder<S>::text();
        constexpr ::std::string_view open = "symbol_holder<";
        constexpr auto begin = text.find(open) + open.size();
        constexpr auto held = text.substr(begin, text.rfind(">::text(") - begin);
        constexpr auto qualified = ::scl::detail::bare_symbol(::scl::detail::msvc_entity_name(held));
        if (qualified.size() > name.size() + 2 && qualified.ends_with(name) &&
            qualified.substr(0, qualified.size() - name.size()).ends_with("::"))
            return qualified;
        return name;
    }
#else
    constexpr auto symbol_prefix_length() noexcept
    {
        constexpr auto text = ::scl::detail::symbol_pattern_text<lah4l4tjla6_f>();
        return ::scl::detail::find_anchor(text, "scl::detail::lah4l4tjla6_f").position;
    }

    constexpr auto symbol_suffix_length() noexcept
    {
        constexpr auto text = ::scl::detail::symbol_pattern_text<lah4l4tjla6_f>();
        constexpr auto anchor = ::scl::detail::find_anchor(text, "scl::detail::lah4l4tjla6_f");
        return text.length() - anchor.position - anchor.length;
    }
#endif

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Returns the name of the entity S points to, with its namespaces and classes.
     * @ingroup scl_utility_meta
     *
     * @tparam S A pointer to a function, a variable, a member function or a data member; the name
     *         of a function converts to a pointer to it.
     * @return The name of the entity, such as `app::Clock::reset` for `&app::Clock::reset`,
     *         without the address operator, the parentheses and the signature the compiler
     *         writes around it.
     *
     * @warning The result is for display: its spelling differs between compilers and may change
     * in a later version of the module.
     *
     * @note GCC writes `__conv_op` in place of the name of a conversion operator, such as
     * `app::Clock::__conv_op`, and MSVC writes the name of a stub the compiler generates in place of
     * the name of a virtual member function, such as ``app::Clock::`vcall'{0}'``.
     *
     * @snippet meta/symbol_name/meta_symbol_name_example.cpp symbol_name
     */
    template <auto S>
    [[nodiscard]]
    constexpr ::std::string_view symbol_name() noexcept
    {
        constexpr auto text = ::scl::detail::symbol_pattern_text<S>();
        constexpr auto prefix_length = ::scl::detail::symbol_prefix_length();
        constexpr auto suffix_length = ::scl::detail::symbol_suffix_length();

        constexpr auto result = text.substr(prefix_length, text.length() - prefix_length - suffix_length);

#if defined(_MSC_VER) && !defined(__clang__)
        constexpr auto target = ::scl::detail::msvc_thunk_name(
            ::scl::detail::msvc_member_pointer_target(result));
        constexpr auto name = ::scl::detail::msvc_qualified_symbol<S>(
            ::scl::detail::bare_symbol(::scl::detail::msvc_entity_name(target)));
#else
        constexpr auto name = ::scl::detail::bare_symbol(result);
#endif
        return name;
    }

    /**
     * @brief Returns the name of the entity S points to, without its namespaces and classes.
     * @ingroup scl_utility_meta
     *
     * @tparam S A pointer to a function, a variable, a member function or a data member; the name
     *         of a function converts to a pointer to it.
     * @return The name of the entity alone, such as `reset` for `&app::Clock::reset`.
     *
     * @warning The result is for display: its spelling differs between compilers and may change
     * in a later version of the module.
     *
     * @snippet meta/symbol_name/meta_symbol_name_example.cpp symbol_short_name
     */
    template <auto S>
    [[nodiscard]]
    constexpr ::std::string_view symbol_short_name() noexcept
    {
        constexpr auto result = ::scl::symbol_name<S>();
        constexpr auto scope = ::scl::detail::find_last_scope_operator(result);
        if constexpr (scope != ::std::string_view::npos)
            return result.substr(scope + 2);
        else
            return result;
    }

} // namespace scl
