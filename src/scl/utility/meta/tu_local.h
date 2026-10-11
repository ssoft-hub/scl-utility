#pragma once

#include <scl/utility/meta/type.h>

#include <array>
#include <string_view>
#include <type_traits>

/**
 * @file
 * @brief Whether a type is local to its translation unit.
 * @ingroup scl_utility_meta
 */

namespace scl::detail
{
    template <typename T>
    struct marker_box
    {};

    struct marker_ext_probe
    {};

    // The marker is what the anonymous probe's rendered name has and an
    // external probe of the same shape does not, up to the last "::".
    constexpr ::std::string_view derive_marker(::std::string_view anon, ::std::string_view ext) noexcept
    {
        ::std::size_t i = 0;
        while (i < anon.size() && i < ext.size() && anon.at(i) == ext.at(i))
            ++i;
        auto const end = anon.rfind("::");
        return (end == ::std::string_view::npos || end < i) ? ::std::string_view{} : anon.substr(i, end - i);
    }

    template <::std::size_t N>
    constexpr ::std::array<char, N> materialize_marker(::std::string_view text) noexcept
    {
        ::std::array<char, N> marker{};
        for (::std::size_t i = 0; i < N; ++i)
            marker.at(i) = text.at(i);
        return marker;
    }

    namespace
    {
        struct marker_anon_probe
        {};

        constexpr auto marker_plain_text = ::scl::detail::derive_marker(
            ::scl::type_name<marker_anon_probe>(), ::scl::type_name<marker_ext_probe>());

        // MSVC spells the marker differently when the type is nested inside
        // template arguments, hence a second marker from box-wrapped probes.
        constexpr auto marker_nested_text = ::scl::detail::derive_marker(
            ::scl::type_name<marker_box<marker_anon_probe>>(),
            ::scl::type_name<marker_box<marker_ext_probe>>());

        // Materialized as arrays compared by content below: an inline entity
        // may name an internal-linkage const object only when the object is
        // not odr-used and has the same value in every TU ([basic.def.odr]).
        // A string_view pointing into the per-TU signature literal breaks the
        // same-value requirement; the character contents do not.
        constexpr auto marker_plain = ::scl::detail::materialize_marker<marker_plain_text.size()>(marker_plain_text);
        constexpr auto marker_nested =
            ::scl::detail::materialize_marker<marker_nested_text.size()>(marker_nested_text);
    } // namespace

    // ODR: the marker is passed by value on purpose; binding a reference to
    // the internal-linkage marker object inside these inline functions would
    // odr-use it and void the [basic.def.odr] carve-out relied on above.
    // cppcheck-suppress-begin passedByValue
    template <::std::size_t N>
    constexpr bool
    marker_matches_at(::std::string_view name, ::std::size_t pos, ::std::array<char, N> marker) noexcept
    {
        // An empty marker means the derivation degenerated; it must never
        // match, otherwise every type would classify as TU-local.
        if constexpr (N == 0)
        {
            return false;
        }
        else
        {
            if (name.size() - pos < N)
                return false;
            for (::std::size_t i = 0; i < N; ++i)
                if (name.at(pos + i) != marker.at(i))
                    return false;
            return true;
        }
    }
    // cppcheck-suppress-end passedByValue

    constexpr ::std::size_t
    skip_quoted(::std::string_view name, ::std::size_t pos, char const closing) noexcept
    {
        for (++pos; pos < name.size(); ++pos)
            if (name.at(pos) == '\\')
                ++pos;
            else if (name.at(pos) == closing)
                return pos + 1;
        return pos;
    }

    constexpr bool contains_anonymous_namespace_marker(::std::string_view name) noexcept
    {
        for (::std::size_t pos = 0; pos < name.size();)
        {
            if (::scl::detail::marker_matches_at(name, pos, marker_plain) ||
                ::scl::detail::marker_matches_at(name, pos, marker_nested))
                return true;

            char const ch = name.at(pos);
            if (ch == '"' || ch == '\'')
            {
                // A string or a character argument of a template could spell the
                // marker inside the name of an external type.
                pos = ::scl::detail::skip_quoted(name, pos, ch);
            }
            else if (ch == '`')
            {
                // An MSVC `...' quotation that is not the marker (checked
                // above). Consume it whole so its closing quote is not
                // mistaken for a character-literal opener.
                pos = ::scl::detail::skip_quoted(name, pos, '\'');
            }
            else
            {
                ++pos;
            }
        }
        return false;
    }

    template <typename T>
    struct tu_local_impl
        : ::std::bool_constant<::scl::detail::contains_anonymous_namespace_marker(::scl::type_name<T>())>
    {};

    template <typename T>
    struct tu_local_impl<T *> : tu_local_impl<::std::remove_cv_t<T>>
    {};

    template <typename T>
    struct tu_local_impl<T &> : tu_local_impl<::std::remove_cv_t<T>>
    {};

    template <typename T>
    struct tu_local_impl<T &&> : tu_local_impl<::std::remove_cv_t<T>>
    {};

    template <typename T, ::std::size_t N>
    struct tu_local_impl<T[N]> // NOLINT(*-avoid-c-arrays): matches an array, declares none
        : tu_local_impl<::std::remove_cv_t<T>>
    {};

    template <typename T>
    struct tu_local_impl<T[]> // NOLINT(*-avoid-c-arrays): matches an array, declares none
        : tu_local_impl<::std::remove_cv_t<T>>
    {};

    template <typename T, typename C>
    struct tu_local_impl<T C::*>
        : ::std::bool_constant<tu_local_impl<C>::value || tu_local_impl<::std::remove_cv_t<T>>::value>
    {};

    template <typename R, typename... Args>
    struct tu_local_impl<R(Args...)>
        : ::std::bool_constant<tu_local_impl<::std::remove_cv_t<R>>::value ||
              (tu_local_impl<::std::remove_cv_t<Args>>::value || ...)>
    {};

    template <typename R, typename... Args>
    struct tu_local_impl<R(Args...) noexcept>
        : ::std::bool_constant<tu_local_impl<::std::remove_cv_t<R>>::value ||
              (tu_local_impl<::std::remove_cv_t<Args>>::value || ...)>
    {};

    template <template <typename...> class Template, typename... Args>
    struct tu_local_impl<Template<Args...>>
        : ::std::bool_constant<
              ::scl::detail::contains_anonymous_namespace_marker(::scl::type_name<Template<Args...>>()) ||
              (tu_local_impl<::std::remove_cv_t<Args>>::value || ...)>
    {};

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Tells whether the type T is local to its translation unit.
     * @ingroup scl_utility_meta
     *
     * @tparam T The type to classify: neither a local class nor a closure type, nor a type built
     *         of one.
     *
     * The class template derives from ::std::true_type where `T`, with `const` and `volatile`
     * dropped, is declared in an unnamed namespace or is built of such a type: a pointer,
     * a reference or an array of it, a function type or a pointer to a member whose types include
     * it, or a specialization of a class template with type parameters over it. It derives from
     * ::std::false_type otherwise. A type declared in an unnamed namespace has internal linkage,
     * and a declaration of the same name in another translation unit declares another type.
     *
     * A specialization of a class template with a parameter that is not a type, and a function
     * type with a cv-qualifier, a ref-qualifier or an ellipsis, are classified by the name
     * the compiler spells for them. A string or a character that a template argument holds is not
     * read as part of that name.
     *
     * @note A local class and a closure type are outside the contract: compilers spell them
     * without a reliable mark of locality.
     *
     * @snippet meta/is_tu_local/meta_is_tu_local_example.cpp is_tu_local
     */
    template <typename T>
    struct is_tu_local
#ifndef SCL_DOXYGEN
        : ::scl::detail::tu_local_impl<::std::remove_cv_t<T>>
#endif
    {};

    /**
     * @brief Whether the type T is local to its translation unit, as ::scl::is_tu_local tells.
     * @ingroup scl_utility_meta
     * @tparam T The type to classify: neither a local class nor a closure type, nor a type built
     *         of one.
     */
    template <typename T>
    inline constexpr bool is_tu_local_v = is_tu_local<T>::value;

} // namespace scl
