#pragma once

/**
 * @file spelling.h
 * @brief Comparison of a macro definition with the spellings the attribute group admits.
 * @ingroup scl_utility_attribute
 */

// The variadic form keeps a comma of the expansion in the text.
#define SCL_DETAIL_ATTRIBUTE_TEXT_IMPL(...) #__VA_ARGS__
// The brackets keep an empty expansion a valid argument of the operator # on the traditional
// preprocessor of MSVC.
#define SCL_DETAIL_ATTRIBUTE_TEXT(...) SCL_DETAIL_ATTRIBUTE_TEXT_IMPL([__VA_ARGS__])

#define SCL_DETAIL_ATTRIBUTE_MESSAGE(name) \
    "the macro " #name                     \
    ", defined before the header, must expand to a spelling listed on its page for this compiler"

// A header of the group may be included inside `extern "C"`.
extern "C++" {
namespace scl::attribute::detail
{
    [[nodiscard]]
    constexpr bool is_identifier_character(char character) noexcept
    {
        return character == '_' || (character >= 'a' && character <= 'z') ||
            (character >= 'A' && character <= 'Z') || (character >= '0' && character <= '9');
    }

    // NOLINTBEGIN(cppcoreguidelines-pro-bounds-pointer-arithmetic): a walk over a string literal
    [[nodiscard]]
    constexpr bool skip_spaces(char const *& text) noexcept
    {
        bool skipped = false;
        while (*text == ' ')
        {
            skipped = true;
            ++text;
        }
        return skipped;
    }

    // A space counts only where it separates two identifier characters, so `[[ x ]]` and `[[x]]` are
    // one spelling and `a b` and `ab` are two.
    // A space that splits a punctuator, as in `: :`, passes here and fails where the macro is used.
    [[nodiscard]]
    constexpr bool same_spelling(char const * left, char const * right) noexcept
    {
        char left_previous = ' ';
        char right_previous = ' ';
        while (true)
        {
            bool const left_spaced = skip_spaces(left);
            bool const right_spaced = skip_spaces(right);
            if (*left == '\0' || *right == '\0')
                return *left == '\0' && *right == '\0';
            bool const left_gap = left_spaced && is_identifier_character(left_previous) &&
                is_identifier_character(*left);
            bool const right_gap = right_spaced && is_identifier_character(right_previous) &&
                is_identifier_character(*right);
            if (left_gap != right_gap || *left != *right)
                return false;
            left_previous = *left++;
            right_previous = *right++;
        }
    }
    // A row not defined on this compiler is turned into the text of its own name.
    [[nodiscard]]
    constexpr bool is_row_name(char const * spelling) noexcept
    {
        for (char const * prefix = "[SCL_DETAIL_"; *prefix != '\0'; ++prefix, ++spelling)
            if (*spelling != *prefix)
                return false;
        return true;
    }
    // NOLINTEND(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    template <typename... Spellings>
    [[nodiscard]]
    constexpr bool spelled_as_one_of(char const * text, Spellings... spellings) noexcept
    {
        return ((!is_row_name(spellings) && same_spelling(text, spellings)) || ...);
    }
} // namespace scl::attribute::detail
} // extern "C++"
