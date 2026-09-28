#pragma once

/**
 * @file
 * @brief An enumeration value spelled as its type and its number.
 * @ingroup scl_utility_runtime
 */

#include <scl/utility/concepts/type_category.h>
#include <scl/utility/meta/type.h>
#include <scl/utility/type_traits/signature.h>

#include <array>
#include <charconv>
#include <concepts>
#include <functional>
#include <iterator>
#include <limits>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>

namespace scl::detail
{
    // An integer type of the size and signedness of an underlying type, so that a character or
    // bool underlying type still renders as a number.
    template <typename Underlying>
    struct enum_number
    {
        using type = ::std::conditional_t<::std::is_signed_v<Underlying>,
            ::std::make_signed_t<Underlying>,
            ::std::make_unsigned_t<Underlying>>;
    };

    template <>
    struct enum_number<bool>
    {
        using type = unsigned char;
    };

    template <typename Enum>
    using enum_number_t = ::scl::detail::enum_number<::std::underlying_type_t<Enum>>::type;

    [[nodiscard]]
    inline ::std::string enum_spelling(::std::string_view type, ::std::string_view number)
    {
        ::std::string result;
        result.reserve(type.size() + 2 + number.size());
        result += type;
        result += "::";
        result += number;
        return result;
    }

} // namespace scl::detail

namespace scl::detail::concepts
{
    // The width of an integer type is the number of its value bits, so bool is one bit wide.
    template <typename Parameter, typename Underlying>
    concept takes_number = ::std::integral<Parameter> &&
        ::std::is_signed_v<Parameter> == ::std::is_signed_v<Underlying> &&
        ::std::numeric_limits<Parameter>::digits >= ::std::numeric_limits<Underlying>::digits;

    // A call operator that is a template or is overloaded has no signature, and a function of (...)
    // alone has no first parameter, so only the call is checked for them.
    template <typename Format, typename Underlying>
    concept enum_format_parameter = !::scl::detail::has_signature<Format> ||
        ::scl::signature::parameter_count_v<Format> == 0 ||
        ::scl::detail::concepts::takes_number<::std::remove_cvref_t<::scl::signature::parameter_t<Format, 0>>, Underlying>;

} // namespace scl::detail::concepts

namespace scl::concepts
{
    template <typename Format, typename Enum>
    concept enum_string_format = ::scl::concepts::enum_type<Enum> &&
        ::std::invocable<Format, ::scl::detail::enum_number_t<Enum> const &> &&
        ::std::convertible_to<::std::invoke_result_t<Format, ::scl::detail::enum_number_t<Enum> const &>,
            ::std::string_view> &&
        ::scl::detail::concepts::enum_format_parameter<Format, ::std::underlying_type_t<Enum>>;

} // namespace scl::concepts

namespace scl
{
    template <::scl::concepts::enum_type E, typename Format>
    [[nodiscard]]
    ::std::string enum_string(E value, Format && format)
        requires ::scl::concepts::enum_string_format<Format, E>
    {
        auto const numeric = static_cast<::scl::detail::enum_number_t<E>>(value);
        auto && spelled = ::std::invoke(::std::forward<Format>(format), numeric);
        constexpr auto type = ::scl::type_short_name<E>();
        return ::scl::detail::enum_spelling(type, ::std::forward<decltype(spelled)>(spelled));
    }

    template <::scl::concepts::enum_type E>
    [[nodiscard]]
    ::std::string enum_string(E value)
    {
        using number = ::scl::detail::enum_number_t<E>;
        ::std::array<char, ::std::numeric_limits<number>::digits10 + 2> digits{};
        auto const end =
            ::std::to_chars(digits.data(),
                ::std::next(digits.data(), static_cast<::std::ptrdiff_t>(digits.size())), static_cast<number>(value))
                .ptr;
        constexpr auto type = ::scl::type_short_name<E>();
        return ::scl::detail::enum_spelling(type,
            ::std::string_view{digits.data(), static_cast<::std::size_t>(::std::distance(digits.data(), end))});
    }

} // namespace scl

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @fn scl::enum_string(E value)
 * @ingroup scl_utility_runtime
 * @brief Returns @p value spelled as `Type::N`, where `N` is its number in decimal.
 *
 * The part `Type` is the short name the function template ::scl::type_short_name<E>() returns at
 * compile time, so no RTTI is needed. For an enumeration type with no name, `Type` is the name the
 * compiler generates, which differs between compilers and may change in a later version of the
 * module. The part `N` is the number @p value holds in its underlying type, with the sign of that
 * type. For a character or `bool` underlying type, `N` is a number as well. A value no enumeration
 * constant has is spelled the same way as the value of a constant.
 *
 * @snippet runtime/enum_string/runtime_enum_string_example.cpp named
 * @snippet runtime/enum_string/runtime_enum_string_example.cpp unnamed
 *
 * @tparam E Enumeration type, deduced.
 * @param value Value to spell.
 * @return `Type::N`, for example `Color::1`.
 */

/**
 * @concept scl::concepts::enum_string_format
 * @ingroup scl_utility_runtime
 * @brief A function object ::scl::enum_string can call to spell the number of an @p Enum value.
 *
 * The function object is called with the number as a constant lvalue of an integer type of the size
 * and signedness of the underlying type of @p Enum, or of the type `unsigned char` for the
 * underlying type `bool`. It must return a value of any type implicitly convertible to
 * `std::string_view`: an object of the type `std::string` or `std::string_view`, a reference to
 * one, or a pointer to a null-terminated string.
 *
 * Where the function object has a single signature, the type of its first parameter is determined
 * with the alias template ::scl::signature::parameter_t. That type, with the reference and the
 * cv-qualifiers removed, must be an integral type of the signedness of the underlying type and no
 * narrower than it; `bool` and the character types count as integral types. The width is the number
 * of value bits, so `bool` is one bit wide and too narrow for the underlying type `std::uint8_t`. A
 * parameter that is a reference to non-const, such as `std::uint32_t &`, is refused as well, since
 * the number comes as a constant lvalue.
 *
 * A generic lambda, an object of a class whose call operator is overloaded, and the objects the
 * function templates `std::ref` and `std::bind_front` return have no single signature. For such a
 * function object, and for a function whose parameter list is `(...)` alone, since it has no first
 * parameter, only the call and the type of its result are checked.
 *
 * @snippet runtime/enum_string/runtime_enum_string_example.cpp concept
 *
 * @tparam Format Type of the function object as the call receives it, an lvalue reference type
 *         for an lvalue argument.
 * @tparam Enum Enumeration type whose number the function object spells; for a type that is not
 *         an enumeration the concept is not satisfied.
 * @see ::scl::signature::parameter_t
 */

/**
 * @fn scl::enum_string(E value, Format && format)
 * @ingroup scl_utility_runtime
 * @brief Returns @p value spelled as `Type::` followed by the text @p format returns for it.
 *
 * The function template calls the function object @p format as it is passed, so a lambda declared
 * with the specifier `mutable` is accepted too. The text the function object returns is put after
 * `Type::` unchanged. What the function object takes and returns is stated by the concept
 * ::scl::concepts::enum_string_format.
 *
 * @snippet runtime/enum_string/runtime_enum_string_example.cpp formatted
 *
 * @tparam E Enumeration type, deduced.
 * @tparam Format Type of the function object, deduced.
 * @param value Value to spell.
 * @param format Function object that turns the number into text.
 * @return `Type::` followed by the text of @p format, for example `Grade::01010000`.
 */
