#pragma once

/**
 * @file character.h
 * @brief The element types whose array carries a terminator.
 * @ingroup scl_utility_hash
 */

#include <concepts>

namespace scl::hash::detail
{
    // A character array carries a terminating zero its text does not, whatever the element
    // width. Every other element type carries data, where a zero is a value like any other.
    // wchar_t is left out: its width is the platform's, so one text is a different number of
    // bytes on each, and the hash functions reject it for that reason already.
    template <typename Type>
    concept character = ::std::same_as<Type, char> || ::std::same_as<Type, char8_t> ||
        ::std::same_as<Type, char16_t> || ::std::same_as<Type, char32_t>;

} // namespace scl::hash::detail
