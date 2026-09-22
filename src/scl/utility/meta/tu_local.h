#pragma once

#include <scl/utility/meta/type.h>

#include <array>
#include <string_view>
#include <type_traits>

/**
 * @file
 * @brief Trait detecting translation-unit-local (internal linkage) types (C++20).
 * @ingroup scl_utility_meta
 * @details
 * - ::scl::is_tu_local<T> / ::scl::is_tu_local_v<T>:
 *     Detects whether T is declared in an anonymous namespace (directly or as a
 *     component of a compound type), i.e. whether same-named declarations in
 *     other translation units denote distinct types.
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
    // Deriving it instead of hardcoding per-compiler spellings keeps the
    // trait working on an untested compiler; a rendering with no marker at
    // all degenerates to an empty result, which never matches (pinned by the
    // trait tests).
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

        constexpr auto marker_plain_text = derive_marker(type_name<marker_anon_probe>(),
            type_name<marker_ext_probe>());

        // MSVC spells the marker differently when the type is nested inside
        // template arguments, hence a second marker from box-wrapped probes.
        constexpr auto marker_nested_text = derive_marker(type_name<marker_box<marker_anon_probe>>(),
            type_name<marker_box<marker_ext_probe>>());

        // Materialized as arrays compared by content below: an inline entity
        // may name an internal-linkage const object only when the object is
        // not odr-used and has the same value in every TU ([basic.def.odr]).
        // A string_view pointing into the per-TU signature literal breaks the
        // same-value requirement; the character contents do not.
        constexpr auto marker_plain = materialize_marker<marker_plain_text.size()>(marker_plain_text);
        constexpr auto marker_nested = materialize_marker<marker_nested_text.size()>(marker_nested_text);
    } // namespace

    // ODR: the marker is passed by value on purpose — binding a reference to
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
            if (marker_matches_at(name, pos, marker_plain) || marker_matches_at(name, pos, marker_nested))
                return true;

            char const ch = name.at(pos);
            if (ch == '"' || ch == '\'')
            {
                // String and character NTTP values render inside quotes (with
                // backslash escapes); a real marker sits in the qualification,
                // outside any literal. Skipping literal contents keeps an NTTP
                // from injecting a marker into an external type's name.
                pos = skip_quoted(name, pos, ch);
            }
            else if (ch == '`')
            {
                // An MSVC `...' quotation that is not the marker (checked
                // above). Consume it whole so its closing quote is not
                // mistaken for a character-literal opener.
                pos = skip_quoted(name, pos, '\'');
            }
            else
            {
                ++pos;
            }
        }
        return false;
    }

    template <typename T>
    struct tu_local_impl : ::std::bool_constant<contains_anonymous_namespace_marker(type_name<T>())>
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
        : ::std::bool_constant<contains_anonymous_namespace_marker(type_name<Template<Args...>>()) ||
              (tu_local_impl<::std::remove_cv_t<Args>>::value || ...)>
    {};

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Detects whether the type T is local to the current translation unit.
     * @ingroup scl_utility_meta
     *
     * @tparam T The type to classify.
     *
     * @details A type is TU-local when it is declared in an anonymous namespace,
     * or when any component of a compound type is (e.g. a pointer to, reference
     * to, or template specialization over an anonymous-namespace type). Such a
     * type has internal linkage: a same-named declaration in another translation
     * unit denotes a different type.
     *
     * A compound type is decomposed structurally: a pointer, a reference, an
     * array, a function type and a template specialization answer from what
     * they are built from, and a pointer to member from either its class or
     * its member type. A rendering carries no contract, and a release that
     * prints a component unqualified would otherwise classify it as external.
     *
     * What remains, a class type, is classified by searching its rendered name
     * for the compiler's anonymous-namespace marker, derived at compile time by
     * diffing the ::scl::type_name<T>() rendering of a TU-local probe type
     * against an external probe of the same shape. Two markers are derived,
     * because MSVC spells the marker differently for top-level types and for
     * types nested inside template arguments. A marker is accepted anywhere
     * outside string and character literals, so a string non-type template
     * parameter spelling the marker inside its quoted value does not
     * misclassify an external type.
     *
     * @note Local classes and closure types are outside the contract of this
     * trait: compilers render them without a reliable marker (Clang prints a
     * bare name). Only namespace-scope types are classified.
     *
     * @note A template taking a non-type parameter, and a function type with
     * a cv-qualifier, a ref-qualifier or a C-style ellipsis, are classified
     * from the rendering rather than decomposed.
     *
     * @code
     * namespace { struct duck {}; }
     * struct goose {};
     * static_assert(::scl::is_tu_local_v<duck>);
     * static_assert(::scl::is_tu_local_v<std::vector<duck>>);
     * static_assert(!::scl::is_tu_local_v<goose>);
     * @endcode
     */
    template <typename T>
    struct is_tu_local : detail::tu_local_impl<::std::remove_cv_t<T>>
    {};

    /**
     * @brief Convenience variable template for ::scl::is_tu_local.
     * @ingroup scl_utility_meta
     * @tparam T The type to classify.
     */
    template <typename T>
    inline constexpr bool is_tu_local_v = is_tu_local<T>::value;

} // namespace scl
