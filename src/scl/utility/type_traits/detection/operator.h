/**
 * @file
 * @brief Traits telling at compile time whether an operator applies to given operand types.
 * @ingroup scl_utility_type_traits
 */

#pragma once

#include <scl/utility/preprocessor/forward.h>
#include <scl/utility/type_traits/detail/exact.h>
#include <scl/utility/type_traits/detection.h>

#include <type_traits>
#include <utility>

#ifndef SCL_DOXYGEN

#define SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION(op, name)                                             \
    namespace scl                                                                                     \
    {                                                                                                 \
        template <typename Type, typename... Arguments>                                               \
        using name##_member_operation = decltype(::std::declval<Type>().operator op /**/ (            \
            ::std::declval<Arguments>()...));                                                         \
        template <typename... Arguments>                                                              \
        inline constexpr auto has_##name##_member_v =                                                 \
            ::scl::is_detected_v<name##_member_operation, Arguments...>;                              \
        template <typename... Arguments>                                                              \
        using name##_member_t = ::scl::detected_t<name##_member_operation, Arguments...>;             \
    }                                                                                                 \
    namespace scl                                                                                     \
    {                                                                                                 \
        template <typename Type, typename... Arguments>                                               \
        using name##_member_exact_operation = decltype((                                              \
            ::std::declval<Type>().*                                                                  \
            ::scl::detail::exact_member<                                                              \
                Type, ::scl::detected_t<name##_member_operation, Type, Arguments...>,                 \
                Arguments...>::select(&::std::remove_cvref_t<Type>::operator op /**/))(               \
            ::std::declval<Arguments>()...));                                                         \
        template <typename... Arguments>                                                              \
        inline constexpr auto has_##name##_member_exact_v =                                           \
            ::scl::is_detected_v<name##_member_exact_operation, Arguments...>;                        \
        template <typename... Arguments>                                                              \
        using name##_member_exact_t = ::scl::detected_t<name##_member_exact_operation, Arguments...>; \
    }

#define SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(op, name)                                         \
    namespace scl                                                                                    \
    {                                                                                                \
        template <typename Type>                                                                     \
        using name##_operation = decltype(op /**/ ::std::declval<Type>());                           \
        template <typename... Arguments>                                                             \
        inline constexpr auto has_##name##_v = ::scl::is_detected_v<name##_operation, Arguments...>; \
        template <typename... Arguments>                                                             \
        using name##_t = ::scl::detected_t<name##_operation, Arguments...>;                          \
    }                                                                                                \
    SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION(SCL_FORWARD(op), name)

#define SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION(op, name)                                                 \
    namespace scl                                                                                             \
    {                                                                                                         \
        template <typename Type>                                                                              \
        using name##_operation = decltype(::std::declval<Type>() op /**/);                                    \
        template <typename... Arguments>                                                                      \
        inline constexpr auto has_##name##_v = ::scl::is_detected_v<name##_operation, Arguments...>;          \
        template <typename... Arguments>                                                                      \
        using name##_t = ::scl::detected_t<name##_operation, Arguments...>;                                   \
    }                                                                                                         \
    namespace scl                                                                                             \
    {                                                                                                         \
        template <typename Type>                                                                              \
        using name##_member_operation = decltype(::std::declval<Type>().operator op /**/ (                    \
            ::std::declval<int>()));                                                                          \
        template <typename Type>                                                                              \
        inline constexpr auto has_##name##_member_v = ::scl::is_detected_v<name##_member_operation, Type>;    \
        template <typename Type>                                                                              \
        using name##_member_t = ::scl::detected_t<name##_member_operation, Type>;                             \
    }                                                                                                         \
    namespace scl                                                                                             \
    {                                                                                                         \
        template <typename Type>                                                                              \
        using name##_member_exact_operation = decltype((                                                      \
            ::std::declval<Type>().*                                                                          \
            ::scl::detail::exact_member<Type, ::scl::detected_t<name##_member_operation, Type>, int>::select( \
                &::std::remove_cvref_t<Type>::operator op /**/))(::std::declval<int>()));                     \
        template <typename Type>                                                                              \
        inline constexpr auto has_##name##_member_exact_v =                                                   \
            ::scl::is_detected_v<name##_member_exact_operation, Type>;                                        \
        template <typename Type>                                                                              \
        using name##_member_exact_t = ::scl::detected_t<name##_member_exact_operation, Type>;                 \
    }

#define SCL_DETAIL_BINARY_OPERATOR_DETECTION(op, name)                                               \
    namespace scl                                                                                    \
    {                                                                                                \
        template <typename Left, typename Right>                                                     \
        using name##_operation = decltype(::std::declval<Left>() op /**/ ::std::declval<Right>());   \
        template <typename... Arguments>                                                             \
        inline constexpr auto has_##name##_v = ::scl::is_detected_v<name##_operation, Arguments...>; \
        template <typename... Arguments>                                                             \
        using name##_t = ::scl::detected_t<name##_operation, Arguments...>;                          \
    }                                                                                                \
    SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION(SCL_FORWARD(op), name)

#define SCL_DETAIL_ARROW_OPERATOR_DETECTION(op, name)                                                \
    namespace scl                                                                                    \
    {                                                                                                \
        template <typename Type, typename... Arguments>                                              \
        using name##_operation = decltype(::std::declval<Type>().operator op /**/ (                  \
            ::std::declval<Arguments>()...));                                                        \
        template <typename... Arguments>                                                             \
        inline constexpr auto has_##name##_v = ::scl::is_detected_v<name##_operation, Arguments...>; \
        template <typename... Arguments>                                                             \
        using name##_t = ::scl::detected_t<name##_operation, Arguments...>;                          \
    }                                                                                                \
    SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION(SCL_FORWARD(op), name)

SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(&, address_of)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(*, indirection)
SCL_DETAIL_ARROW_OPERATOR_DETECTION(->*, arrow_to_pointer)
SCL_DETAIL_ARROW_OPERATOR_DETECTION(->, arrow)
// cppcheck-suppress syntaxError
SCL_DETAIL_BINARY_OPERATOR_DETECTION(SCL_FORWARD(, ), comma)

namespace scl
{
    template <typename Type, typename Argument>
    using subscript_operation = decltype(::std::declval<Type>()[::std::declval<Argument>()]);
    template <typename... Arguments>
    inline constexpr auto has_subscript_v = ::scl::is_detected_v<subscript_operation, Arguments...>;
    template <typename... Arguments>
    using subscript_t = ::scl::detected_t<subscript_operation, Arguments...>;
} // namespace scl
SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION([], subscript)

namespace scl
{
    template <typename Type, typename... Arguments>
    using call_operation = decltype(::std::declval<Type>()(::std::declval<Arguments>()...));
    template <typename... Arguments>
    inline constexpr auto has_call_v = ::scl::is_detected_v<call_operation, Arguments...>;
    template <typename... Arguments>
    using call_t = ::scl::detected_t<call_operation, Arguments...>;
} // namespace scl
SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION((), call)

SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(+, unary_plus)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(-, negate)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(++, prefix_increment)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(--, prefix_decrement)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(~, bit_not)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(!, logical_not)

SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION(++, postfix_increment) // NOLINT(bugprone-macro-repeated-side-effects)
SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION(--, postfix_decrement) // NOLINT(bugprone-macro-repeated-side-effects)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(==, equal_to)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(!=, not_equal_to)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(<, less)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(<=, less_equal)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>, greater)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>=, greater_equal)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(*, multiplies)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(/, divides)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(%, modulus)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(+, plus)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(-, minus)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(<<, left_shift)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>>, right_shift)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(&, bit_and)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(|, bit_or)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(^, bit_xor)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(&&, logical_and)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(||, logical_or)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(=, assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(*=, multiplies_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(/=, divides_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(%=, modulus_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(+=, plus_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(-=, minus_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(<<=, left_shift_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>>=, right_shift_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(&=, bit_and_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(|=, bit_or_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(^=, bit_xor_assign)

#undef SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION
#undef SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION
#undef SCL_DETAIL_BINARY_OPERATOR_DETECTION
#undef SCL_DETAIL_ARROW_OPERATOR_DETECTION
#undef SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION

#elif defined(SCL_DOXYGEN)

namespace scl
{
    /**
     * @defgroup scl_utility_operator_detection Operator detection
     * @ingroup scl_utility_type_traits
     * @brief Whether an operator applies to given operands, as an expression or a member function.
     *
     * For each operator `name` the templates `name_operation`, `has_name_v` and `name_t` are declared
     * for the expression, `name_member_operation`, `has_name_member_v` and `name_member_t` for
     * the member function, and `name_member_exact_operation`, `has_name_member_exact_v` and
     * `name_member_exact_t` for the member function declared with exactly the given parameters.
     * In the exact form the top-level `const` and `volatile` of a parameter are not compared, since
     * they are not part of a function type, and a member function that is deleted or inaccessible is
     * not found. The specifier `noexcept` is not compared either. Where the call of the member
     * function is ambiguous and the overloads differ not only in their qualifiers, the specifier
     * shows in the type of the pointer that `scl::overload_cast<Arguments...>` returns for
     * the address of the operator. On MSVC the call of the member function has to compile in
     * the exact form. A private or a protected operator is found there by the expression form and
     * the member form, and by the exact form only where the class declares other overloads of
     * the operator.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp types
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp expression
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp member
     * @{
     */

    /**
     * @defgroup scl_utility_operator_detection_prefix Prefix unary operators
     * @{
     */

    /**
     * @brief The type of `&std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using address_of_operation = decltype(&::std::declval<Type>());
    /**
     * @brief Whether ::scl::address_of_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Arguments Template arguments of ::scl::address_of_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_address_of_v = ::scl::is_detected_v<::scl::address_of_operation, Arguments...>;
    /**
     * @brief ::scl::address_of_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Arguments Template arguments of ::scl::address_of_operation.
     */
    template <typename... Arguments>
    using address_of_t = ::scl::detected_t<::scl::address_of_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator&(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using address_of_member_operation = decltype(::std::declval<Type>().operator&(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::address_of_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_address_of_member_v =
        ::scl::is_detected_v<::scl::address_of_member_operation, Arguments...>;
    /**
     * @brief ::scl::address_of_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using address_of_member_t = ::scl::detected_t<::scl::address_of_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator&` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator&` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator&`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using address_of_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator&` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_address_of_member_exact_v =
        ::scl::is_detected_v<::scl::address_of_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::address_of_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp address_of
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using address_of_member_exact_t =
        ::scl::detected_t<::scl::address_of_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `*std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using indirection_operation = decltype(*::std::declval<Type>());
    /**
     * @brief Whether ::scl::indirection_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Arguments Template arguments of ::scl::indirection_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_indirection_v =
        ::scl::is_detected_v<::scl::indirection_operation, Arguments...>;
    /**
     * @brief ::scl::indirection_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Arguments Template arguments of ::scl::indirection_operation.
     */
    template <typename... Arguments>
    using indirection_t = ::scl::detected_t<::scl::indirection_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator*(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using indirection_member_operation = decltype(::std::declval<Type>().operator*(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::indirection_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_indirection_member_v =
        ::scl::is_detected_v<::scl::indirection_member_operation, Arguments...>;
    /**
     * @brief ::scl::indirection_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using indirection_member_t = ::scl::detected_t<::scl::indirection_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator*` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator*` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator*`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using indirection_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator*` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_indirection_member_exact_v =
        ::scl::is_detected_v<::scl::indirection_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::indirection_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp indirection
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using indirection_member_exact_t =
        ::scl::detected_t<::scl::indirection_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `+std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using unary_plus_operation = decltype(+::std::declval<Type>());
    /**
     * @brief Whether ::scl::unary_plus_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Arguments Template arguments of ::scl::unary_plus_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_unary_plus_v = ::scl::is_detected_v<::scl::unary_plus_operation, Arguments...>;
    /**
     * @brief ::scl::unary_plus_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Arguments Template arguments of ::scl::unary_plus_operation.
     */
    template <typename... Arguments>
    using unary_plus_t = ::scl::detected_t<::scl::unary_plus_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator+(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using unary_plus_member_operation = decltype(::std::declval<Type>().operator+(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::unary_plus_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_unary_plus_member_v =
        ::scl::is_detected_v<::scl::unary_plus_member_operation, Arguments...>;
    /**
     * @brief ::scl::unary_plus_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using unary_plus_member_t = ::scl::detected_t<::scl::unary_plus_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator+` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator+` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator+`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using unary_plus_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator+` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_unary_plus_member_exact_v =
        ::scl::is_detected_v<::scl::unary_plus_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::unary_plus_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp unary_plus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using unary_plus_member_exact_t =
        ::scl::detected_t<::scl::unary_plus_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `-std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using negate_operation = decltype(-::std::declval<Type>());
    /**
     * @brief Whether ::scl::negate_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Arguments Template arguments of ::scl::negate_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_negate_v = ::scl::is_detected_v<::scl::negate_operation, Arguments...>;
    /**
     * @brief ::scl::negate_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Arguments Template arguments of ::scl::negate_operation.
     */
    template <typename... Arguments>
    using negate_t = ::scl::detected_t<::scl::negate_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator-(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using negate_member_operation = decltype(::std::declval<Type>().operator-(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::negate_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_negate_member_v =
        ::scl::is_detected_v<::scl::negate_member_operation, Arguments...>;
    /**
     * @brief ::scl::negate_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using negate_member_t = ::scl::detected_t<::scl::negate_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator-` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator-` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator-`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using negate_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator-` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_negate_member_exact_v =
        ::scl::is_detected_v<::scl::negate_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::negate_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp negate
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using negate_member_exact_t = ::scl::detected_t<::scl::negate_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `++std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using prefix_increment_operation = decltype(++::std::declval<Type>());
    /**
     * @brief Whether ::scl::prefix_increment_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Arguments Template arguments of ::scl::prefix_increment_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_prefix_increment_v =
        ::scl::is_detected_v<::scl::prefix_increment_operation, Arguments...>;
    /**
     * @brief ::scl::prefix_increment_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Arguments Template arguments of ::scl::prefix_increment_operation.
     */
    template <typename... Arguments>
    using prefix_increment_t = ::scl::detected_t<::scl::prefix_increment_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator++(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using prefix_increment_member_operation = decltype(::std::declval<Type>().operator++(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::prefix_increment_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_prefix_increment_member_v =
        ::scl::is_detected_v<::scl::prefix_increment_member_operation, Arguments...>;
    /**
     * @brief ::scl::prefix_increment_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using prefix_increment_member_t =
        ::scl::detected_t<::scl::prefix_increment_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator++` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator++` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator++`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using prefix_increment_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator++` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_prefix_increment_member_exact_v =
        ::scl::is_detected_v<::scl::prefix_increment_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::prefix_increment_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_increment
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using prefix_increment_member_exact_t =
        ::scl::detected_t<::scl::prefix_increment_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `--std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using prefix_decrement_operation = decltype(--::std::declval<Type>());
    /**
     * @brief Whether ::scl::prefix_decrement_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Arguments Template arguments of ::scl::prefix_decrement_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_prefix_decrement_v =
        ::scl::is_detected_v<::scl::prefix_decrement_operation, Arguments...>;
    /**
     * @brief ::scl::prefix_decrement_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Arguments Template arguments of ::scl::prefix_decrement_operation.
     */
    template <typename... Arguments>
    using prefix_decrement_t = ::scl::detected_t<::scl::prefix_decrement_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator--(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using prefix_decrement_member_operation = decltype(::std::declval<Type>().operator--(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::prefix_decrement_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_prefix_decrement_member_v =
        ::scl::is_detected_v<::scl::prefix_decrement_member_operation, Arguments...>;
    /**
     * @brief ::scl::prefix_decrement_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using prefix_decrement_member_t =
        ::scl::detected_t<::scl::prefix_decrement_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator--` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator--` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator--`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using prefix_decrement_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator--` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_prefix_decrement_member_exact_v =
        ::scl::is_detected_v<::scl::prefix_decrement_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::prefix_decrement_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp prefix_decrement
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using prefix_decrement_member_exact_t =
        ::scl::detected_t<::scl::prefix_decrement_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `~std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using bit_not_operation = decltype(~::std::declval<Type>());
    /**
     * @brief Whether ::scl::bit_not_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Arguments Template arguments of ::scl::bit_not_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_not_v = ::scl::is_detected_v<::scl::bit_not_operation, Arguments...>;
    /**
     * @brief ::scl::bit_not_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Arguments Template arguments of ::scl::bit_not_operation.
     */
    template <typename... Arguments>
    using bit_not_t = ::scl::detected_t<::scl::bit_not_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator~(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_not_member_operation = decltype(::std::declval<Type>().operator~(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_not_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_not_member_v =
        ::scl::is_detected_v<::scl::bit_not_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_not_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_not_member_t = ::scl::detected_t<::scl::bit_not_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator~` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator~` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator~`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_not_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator~` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_not_member_exact_v =
        ::scl::is_detected_v<::scl::bit_not_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_not_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_not
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_not_member_exact_t = ::scl::detected_t<::scl::bit_not_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `!std::declval<Type>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using logical_not_operation = decltype(!::std::declval<Type>());
    /**
     * @brief Whether ::scl::logical_not_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Arguments Template arguments of ::scl::logical_not_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_not_v =
        ::scl::is_detected_v<::scl::logical_not_operation, Arguments...>;
    /**
     * @brief ::scl::logical_not_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Arguments Template arguments of ::scl::logical_not_operation.
     */
    template <typename... Arguments>
    using logical_not_t = ::scl::detected_t<::scl::logical_not_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator!(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using logical_not_member_operation = decltype(::std::declval<Type>().operator!(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::logical_not_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_not_member_v =
        ::scl::is_detected_v<::scl::logical_not_member_operation, Arguments...>;
    /**
     * @brief ::scl::logical_not_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using logical_not_member_t = ::scl::detected_t<::scl::logical_not_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator!` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator!` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator!`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using logical_not_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator!` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_not_member_exact_v =
        ::scl::is_detected_v<::scl::logical_not_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::logical_not_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_not
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using logical_not_member_exact_t =
        ::scl::detected_t<::scl::logical_not_member_exact_operation, Arguments...>;

    /** @} */

    /**
     * @defgroup scl_utility_operator_detection_postfix Postfix unary operators
     * @{
     */

    /**
     * @brief The type of `std::declval<Type>()++`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using postfix_increment_operation = decltype(::std::declval<Type>()++);
    /**
     * @brief Whether ::scl::postfix_increment_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Arguments Template arguments of ::scl::postfix_increment_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_postfix_increment_v =
        ::scl::is_detected_v<::scl::postfix_increment_operation, Arguments...>;
    /**
     * @brief ::scl::postfix_increment_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Arguments Template arguments of ::scl::postfix_increment_operation.
     */
    template <typename... Arguments>
    using postfix_increment_t = ::scl::detected_t<::scl::postfix_increment_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator++(0)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_increment_member_operation = decltype(::std::declval<Type>().operator++(
        ::std::declval<int>()));
    /**
     * @brief Whether ::scl::postfix_increment_member_operation is well-formed for @p Type.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    inline constexpr auto has_postfix_increment_member_v =
        ::scl::is_detected_v<::scl::postfix_increment_member_operation, Type>;
    /**
     * @brief ::scl::postfix_increment_member_operation for @p Type, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_increment_member_t = ::scl::detected_t<::scl::postfix_increment_member_operation, Type>;

    /**
     * @brief The type of the call of the member function `operator++` with the parameter `int`.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator++` with the parameter `int` and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator++`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_increment_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator++` is declared exactly for the object type @p Type.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    inline constexpr auto has_postfix_increment_member_exact_v =
        ::scl::is_detected_v<::scl::postfix_increment_member_exact_operation, Type>;
    /**
     * @brief ::scl::postfix_increment_member_exact_operation for @p Type, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_increment
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_increment_member_exact_t =
        ::scl::detected_t<::scl::postfix_increment_member_exact_operation, Type>;

    /**
     * @brief The type of `std::declval<Type>()--`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Operand type.
     */
    template <typename Type>
    using postfix_decrement_operation = decltype(::std::declval<Type>()--);
    /**
     * @brief Whether ::scl::postfix_decrement_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Arguments Template arguments of ::scl::postfix_decrement_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_postfix_decrement_v =
        ::scl::is_detected_v<::scl::postfix_decrement_operation, Arguments...>;
    /**
     * @brief ::scl::postfix_decrement_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Arguments Template arguments of ::scl::postfix_decrement_operation.
     */
    template <typename... Arguments>
    using postfix_decrement_t = ::scl::detected_t<::scl::postfix_decrement_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator--(0)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_decrement_member_operation = decltype(::std::declval<Type>().operator--(
        ::std::declval<int>()));
    /**
     * @brief Whether ::scl::postfix_decrement_member_operation is well-formed for @p Type.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    inline constexpr auto has_postfix_decrement_member_v =
        ::scl::is_detected_v<::scl::postfix_decrement_member_operation, Type>;
    /**
     * @brief ::scl::postfix_decrement_member_operation for @p Type, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_decrement_member_t = ::scl::detected_t<::scl::postfix_decrement_member_operation, Type>;

    /**
     * @brief The type of the call of the member function `operator--` with the parameter `int`.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator--` with the parameter `int` and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator--`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_decrement_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator--` is declared exactly for the object type @p Type.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    inline constexpr auto has_postfix_decrement_member_exact_v =
        ::scl::is_detected_v<::scl::postfix_decrement_member_exact_operation, Type>;
    /**
     * @brief ::scl::postfix_decrement_member_exact_operation for @p Type, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp postfix_decrement
     *
     * @tparam Type Type of the object.
     */
    template <typename Type>
    using postfix_decrement_member_exact_t =
        ::scl::detected_t<::scl::postfix_decrement_member_exact_operation, Type>;

    /** @} */

    /**
     * @defgroup scl_utility_operator_detection_comparison Comparison operators
     * @{
     */

    /**
     * @brief The type of `std::declval<Left>() == std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using equal_to_operation = decltype(::std::declval<Left>() == ::std::declval<Right>());
    /**
     * @brief Whether ::scl::equal_to_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Arguments Template arguments of ::scl::equal_to_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_equal_to_v = ::scl::is_detected_v<::scl::equal_to_operation, Arguments...>;
    /**
     * @brief ::scl::equal_to_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Arguments Template arguments of ::scl::equal_to_operation.
     */
    template <typename... Arguments>
    using equal_to_t = ::scl::detected_t<::scl::equal_to_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator==(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using equal_to_member_operation = decltype(::std::declval<Type>().operator==(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::equal_to_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_equal_to_member_v =
        ::scl::is_detected_v<::scl::equal_to_member_operation, Arguments...>;
    /**
     * @brief ::scl::equal_to_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using equal_to_member_t = ::scl::detected_t<::scl::equal_to_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator==` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator==` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator==`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using equal_to_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator==` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_equal_to_member_exact_v =
        ::scl::is_detected_v<::scl::equal_to_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::equal_to_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp equal_to
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using equal_to_member_exact_t = ::scl::detected_t<::scl::equal_to_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() != std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using not_equal_to_operation = decltype(::std::declval<Left>() != ::std::declval<Right>());
    /**
     * @brief Whether ::scl::not_equal_to_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Arguments Template arguments of ::scl::not_equal_to_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_not_equal_to_v =
        ::scl::is_detected_v<::scl::not_equal_to_operation, Arguments...>;
    /**
     * @brief ::scl::not_equal_to_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Arguments Template arguments of ::scl::not_equal_to_operation.
     */
    template <typename... Arguments>
    using not_equal_to_t = ::scl::detected_t<::scl::not_equal_to_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator!=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using not_equal_to_member_operation = decltype(::std::declval<Type>().operator!=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::not_equal_to_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_not_equal_to_member_v =
        ::scl::is_detected_v<::scl::not_equal_to_member_operation, Arguments...>;
    /**
     * @brief ::scl::not_equal_to_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using not_equal_to_member_t = ::scl::detected_t<::scl::not_equal_to_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator!=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator!=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator!=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using not_equal_to_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator!=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_not_equal_to_member_exact_v =
        ::scl::is_detected_v<::scl::not_equal_to_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::not_equal_to_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp not_equal_to
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using not_equal_to_member_exact_t =
        ::scl::detected_t<::scl::not_equal_to_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() < std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using less_operation = decltype(::std::declval<Left>() < ::std::declval<Right>());
    /**
     * @brief Whether ::scl::less_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Arguments Template arguments of ::scl::less_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_less_v = ::scl::is_detected_v<::scl::less_operation, Arguments...>;
    /**
     * @brief ::scl::less_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Arguments Template arguments of ::scl::less_operation.
     */
    template <typename... Arguments>
    using less_t = ::scl::detected_t<::scl::less_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator<(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using less_member_operation = decltype(::std::declval<Type>().operator<(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::less_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_less_member_v =
        ::scl::is_detected_v<::scl::less_member_operation, Arguments...>;
    /**
     * @brief ::scl::less_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using less_member_t = ::scl::detected_t<::scl::less_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator<` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator<` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator<`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using less_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator<` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_less_member_exact_v =
        ::scl::is_detected_v<::scl::less_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::less_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using less_member_exact_t = ::scl::detected_t<::scl::less_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() <= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using less_equal_operation = decltype(::std::declval<Left>() <= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::less_equal_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Arguments Template arguments of ::scl::less_equal_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_less_equal_v = ::scl::is_detected_v<::scl::less_equal_operation, Arguments...>;
    /**
     * @brief ::scl::less_equal_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Arguments Template arguments of ::scl::less_equal_operation.
     */
    template <typename... Arguments>
    using less_equal_t = ::scl::detected_t<::scl::less_equal_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator<=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using less_equal_member_operation = decltype(::std::declval<Type>().operator<=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::less_equal_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_less_equal_member_v =
        ::scl::is_detected_v<::scl::less_equal_member_operation, Arguments...>;
    /**
     * @brief ::scl::less_equal_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using less_equal_member_t = ::scl::detected_t<::scl::less_equal_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator<=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator<=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator<=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using less_equal_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator<=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_less_equal_member_exact_v =
        ::scl::is_detected_v<::scl::less_equal_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::less_equal_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp less_equal
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using less_equal_member_exact_t =
        ::scl::detected_t<::scl::less_equal_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() > std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using greater_operation = decltype(::std::declval<Left>() > ::std::declval<Right>());
    /**
     * @brief Whether ::scl::greater_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Arguments Template arguments of ::scl::greater_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_greater_v = ::scl::is_detected_v<::scl::greater_operation, Arguments...>;
    /**
     * @brief ::scl::greater_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Arguments Template arguments of ::scl::greater_operation.
     */
    template <typename... Arguments>
    using greater_t = ::scl::detected_t<::scl::greater_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator>(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using greater_member_operation = decltype(::std::declval<Type>().operator>(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::greater_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_greater_member_v =
        ::scl::is_detected_v<::scl::greater_member_operation, Arguments...>;
    /**
     * @brief ::scl::greater_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using greater_member_t = ::scl::detected_t<::scl::greater_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator>` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator>` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator>`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using greater_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator>` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_greater_member_exact_v =
        ::scl::is_detected_v<::scl::greater_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::greater_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using greater_member_exact_t = ::scl::detected_t<::scl::greater_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() >= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using greater_equal_operation = decltype(::std::declval<Left>() >= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::greater_equal_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Arguments Template arguments of ::scl::greater_equal_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_greater_equal_v =
        ::scl::is_detected_v<::scl::greater_equal_operation, Arguments...>;
    /**
     * @brief ::scl::greater_equal_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Arguments Template arguments of ::scl::greater_equal_operation.
     */
    template <typename... Arguments>
    using greater_equal_t = ::scl::detected_t<::scl::greater_equal_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator>=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using greater_equal_member_operation = decltype(::std::declval<Type>().operator>=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::greater_equal_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_greater_equal_member_v =
        ::scl::is_detected_v<::scl::greater_equal_member_operation, Arguments...>;
    /**
     * @brief ::scl::greater_equal_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using greater_equal_member_t = ::scl::detected_t<::scl::greater_equal_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator>=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator>=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator>=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using greater_equal_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator>=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_greater_equal_member_exact_v =
        ::scl::is_detected_v<::scl::greater_equal_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::greater_equal_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp greater_equal
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using greater_equal_member_exact_t =
        ::scl::detected_t<::scl::greater_equal_member_exact_operation, Arguments...>;

    /** @} */

    /**
     * @defgroup scl_utility_operator_detection_arithmetic Arithmetic and bitwise operators
     * @{
     */

    /**
     * @brief The type of `std::declval<Left>() * std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using multiplies_operation = decltype(::std::declval<Left>() * ::std::declval<Right>());
    /**
     * @brief Whether ::scl::multiplies_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Arguments Template arguments of ::scl::multiplies_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_multiplies_v = ::scl::is_detected_v<::scl::multiplies_operation, Arguments...>;
    /**
     * @brief ::scl::multiplies_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Arguments Template arguments of ::scl::multiplies_operation.
     */
    template <typename... Arguments>
    using multiplies_t = ::scl::detected_t<::scl::multiplies_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator*(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using multiplies_member_operation = decltype(::std::declval<Type>().operator*(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::multiplies_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_multiplies_member_v =
        ::scl::is_detected_v<::scl::multiplies_member_operation, Arguments...>;
    /**
     * @brief ::scl::multiplies_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using multiplies_member_t = ::scl::detected_t<::scl::multiplies_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator*` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator*` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator*`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using multiplies_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator*` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_multiplies_member_exact_v =
        ::scl::is_detected_v<::scl::multiplies_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::multiplies_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using multiplies_member_exact_t =
        ::scl::detected_t<::scl::multiplies_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() / std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using divides_operation = decltype(::std::declval<Left>() / ::std::declval<Right>());
    /**
     * @brief Whether ::scl::divides_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Arguments Template arguments of ::scl::divides_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_divides_v = ::scl::is_detected_v<::scl::divides_operation, Arguments...>;
    /**
     * @brief ::scl::divides_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Arguments Template arguments of ::scl::divides_operation.
     */
    template <typename... Arguments>
    using divides_t = ::scl::detected_t<::scl::divides_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator/(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using divides_member_operation = decltype(::std::declval<Type>().operator/(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::divides_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_divides_member_v =
        ::scl::is_detected_v<::scl::divides_member_operation, Arguments...>;
    /**
     * @brief ::scl::divides_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using divides_member_t = ::scl::detected_t<::scl::divides_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator/` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator/` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator/`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using divides_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator/` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_divides_member_exact_v =
        ::scl::is_detected_v<::scl::divides_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::divides_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using divides_member_exact_t = ::scl::detected_t<::scl::divides_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() % std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using modulus_operation = decltype(::std::declval<Left>() % ::std::declval<Right>());
    /**
     * @brief Whether ::scl::modulus_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Arguments Template arguments of ::scl::modulus_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_modulus_v = ::scl::is_detected_v<::scl::modulus_operation, Arguments...>;
    /**
     * @brief ::scl::modulus_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Arguments Template arguments of ::scl::modulus_operation.
     */
    template <typename... Arguments>
    using modulus_t = ::scl::detected_t<::scl::modulus_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator%(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using modulus_member_operation = decltype(::std::declval<Type>().operator%(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::modulus_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_modulus_member_v =
        ::scl::is_detected_v<::scl::modulus_member_operation, Arguments...>;
    /**
     * @brief ::scl::modulus_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using modulus_member_t = ::scl::detected_t<::scl::modulus_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator%` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator%` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator%`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using modulus_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator%` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_modulus_member_exact_v =
        ::scl::is_detected_v<::scl::modulus_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::modulus_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using modulus_member_exact_t = ::scl::detected_t<::scl::modulus_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() + std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using plus_operation = decltype(::std::declval<Left>() + ::std::declval<Right>());
    /**
     * @brief Whether ::scl::plus_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Arguments Template arguments of ::scl::plus_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_plus_v = ::scl::is_detected_v<::scl::plus_operation, Arguments...>;
    /**
     * @brief ::scl::plus_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Arguments Template arguments of ::scl::plus_operation.
     */
    template <typename... Arguments>
    using plus_t = ::scl::detected_t<::scl::plus_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator+(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using plus_member_operation = decltype(::std::declval<Type>().operator+(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::plus_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_plus_member_v =
        ::scl::is_detected_v<::scl::plus_member_operation, Arguments...>;
    /**
     * @brief ::scl::plus_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using plus_member_t = ::scl::detected_t<::scl::plus_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator+` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator+` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator+`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using plus_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator+` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_plus_member_exact_v =
        ::scl::is_detected_v<::scl::plus_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::plus_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using plus_member_exact_t = ::scl::detected_t<::scl::plus_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() - std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using minus_operation = decltype(::std::declval<Left>() - ::std::declval<Right>());
    /**
     * @brief Whether ::scl::minus_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Arguments Template arguments of ::scl::minus_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_minus_v = ::scl::is_detected_v<::scl::minus_operation, Arguments...>;
    /**
     * @brief ::scl::minus_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Arguments Template arguments of ::scl::minus_operation.
     */
    template <typename... Arguments>
    using minus_t = ::scl::detected_t<::scl::minus_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator-(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using minus_member_operation = decltype(::std::declval<Type>().operator-(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::minus_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_minus_member_v =
        ::scl::is_detected_v<::scl::minus_member_operation, Arguments...>;
    /**
     * @brief ::scl::minus_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using minus_member_t = ::scl::detected_t<::scl::minus_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator-` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator-` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator-`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using minus_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator-` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_minus_member_exact_v =
        ::scl::is_detected_v<::scl::minus_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::minus_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using minus_member_exact_t = ::scl::detected_t<::scl::minus_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() << std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using left_shift_operation = decltype(::std::declval<Left>() << ::std::declval<Right>());
    /**
     * @brief Whether ::scl::left_shift_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Arguments Template arguments of ::scl::left_shift_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_left_shift_v = ::scl::is_detected_v<::scl::left_shift_operation, Arguments...>;
    /**
     * @brief ::scl::left_shift_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Arguments Template arguments of ::scl::left_shift_operation.
     */
    template <typename... Arguments>
    using left_shift_t = ::scl::detected_t<::scl::left_shift_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator<<(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using left_shift_member_operation = decltype(::std::declval<Type>().operator<<(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::left_shift_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_left_shift_member_v =
        ::scl::is_detected_v<::scl::left_shift_member_operation, Arguments...>;
    /**
     * @brief ::scl::left_shift_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using left_shift_member_t = ::scl::detected_t<::scl::left_shift_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator<<` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator<<` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator<<`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using left_shift_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator<<` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_left_shift_member_exact_v =
        ::scl::is_detected_v<::scl::left_shift_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::left_shift_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using left_shift_member_exact_t =
        ::scl::detected_t<::scl::left_shift_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() >> std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using right_shift_operation = decltype(::std::declval<Left>() >> ::std::declval<Right>());
    /**
     * @brief Whether ::scl::right_shift_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Arguments Template arguments of ::scl::right_shift_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_right_shift_v =
        ::scl::is_detected_v<::scl::right_shift_operation, Arguments...>;
    /**
     * @brief ::scl::right_shift_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Arguments Template arguments of ::scl::right_shift_operation.
     */
    template <typename... Arguments>
    using right_shift_t = ::scl::detected_t<::scl::right_shift_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator>>(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using right_shift_member_operation = decltype(::std::declval<Type>().operator>>(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::right_shift_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_right_shift_member_v =
        ::scl::is_detected_v<::scl::right_shift_member_operation, Arguments...>;
    /**
     * @brief ::scl::right_shift_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using right_shift_member_t = ::scl::detected_t<::scl::right_shift_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator>>` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator>>` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator>>`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using right_shift_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator>>` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_right_shift_member_exact_v =
        ::scl::is_detected_v<::scl::right_shift_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::right_shift_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using right_shift_member_exact_t =
        ::scl::detected_t<::scl::right_shift_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() & std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bit_and_operation = decltype(::std::declval<Left>() & ::std::declval<Right>());
    /**
     * @brief Whether ::scl::bit_and_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Arguments Template arguments of ::scl::bit_and_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_and_v = ::scl::is_detected_v<::scl::bit_and_operation, Arguments...>;
    /**
     * @brief ::scl::bit_and_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Arguments Template arguments of ::scl::bit_and_operation.
     */
    template <typename... Arguments>
    using bit_and_t = ::scl::detected_t<::scl::bit_and_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator&(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_and_member_operation = decltype(::std::declval<Type>().operator&(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_and_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_and_member_v =
        ::scl::is_detected_v<::scl::bit_and_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_and_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_and_member_t = ::scl::detected_t<::scl::bit_and_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator&` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator&` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator&`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_and_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator&` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_and_member_exact_v =
        ::scl::is_detected_v<::scl::bit_and_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_and_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_and_member_exact_t = ::scl::detected_t<::scl::bit_and_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() | std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bit_or_operation = decltype(::std::declval<Left>() | ::std::declval<Right>());
    /**
     * @brief Whether ::scl::bit_or_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Arguments Template arguments of ::scl::bit_or_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_or_v = ::scl::is_detected_v<::scl::bit_or_operation, Arguments...>;
    /**
     * @brief ::scl::bit_or_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Arguments Template arguments of ::scl::bit_or_operation.
     */
    template <typename... Arguments>
    using bit_or_t = ::scl::detected_t<::scl::bit_or_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator|(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_or_member_operation = decltype(::std::declval<Type>().operator|(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_or_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_or_member_v =
        ::scl::is_detected_v<::scl::bit_or_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_or_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_or_member_t = ::scl::detected_t<::scl::bit_or_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator|` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator|` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator|`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_or_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator|` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_or_member_exact_v =
        ::scl::is_detected_v<::scl::bit_or_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_or_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_or_member_exact_t = ::scl::detected_t<::scl::bit_or_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() ^ std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bit_xor_operation = decltype(::std::declval<Left>() ^ ::std::declval<Right>());
    /**
     * @brief Whether ::scl::bit_xor_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Arguments Template arguments of ::scl::bit_xor_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_xor_v = ::scl::is_detected_v<::scl::bit_xor_operation, Arguments...>;
    /**
     * @brief ::scl::bit_xor_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Arguments Template arguments of ::scl::bit_xor_operation.
     */
    template <typename... Arguments>
    using bit_xor_t = ::scl::detected_t<::scl::bit_xor_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator^(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_xor_member_operation = decltype(::std::declval<Type>().operator^(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_xor_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_xor_member_v =
        ::scl::is_detected_v<::scl::bit_xor_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_xor_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_xor_member_t = ::scl::detected_t<::scl::bit_xor_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator^` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator^` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator^`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_xor_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator^` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_xor_member_exact_v =
        ::scl::is_detected_v<::scl::bit_xor_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_xor_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_xor_member_exact_t = ::scl::detected_t<::scl::bit_xor_member_exact_operation, Arguments...>;

    /** @} */

    /**
     * @defgroup scl_utility_operator_detection_logical Logical operators
     * @{
     */

    /**
     * @brief The type of `std::declval<Left>() && std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using logical_and_operation = decltype(::std::declval<Left>() && ::std::declval<Right>());
    /**
     * @brief Whether ::scl::logical_and_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Arguments Template arguments of ::scl::logical_and_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_and_v =
        ::scl::is_detected_v<::scl::logical_and_operation, Arguments...>;
    /**
     * @brief ::scl::logical_and_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Arguments Template arguments of ::scl::logical_and_operation.
     */
    template <typename... Arguments>
    using logical_and_t = ::scl::detected_t<::scl::logical_and_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator&&(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using logical_and_member_operation = decltype(::std::declval<Type>().operator&&(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::logical_and_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_and_member_v =
        ::scl::is_detected_v<::scl::logical_and_member_operation, Arguments...>;
    /**
     * @brief ::scl::logical_and_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using logical_and_member_t = ::scl::detected_t<::scl::logical_and_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator&&` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator&&` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator&&`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using logical_and_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator&&` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_and_member_exact_v =
        ::scl::is_detected_v<::scl::logical_and_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::logical_and_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_and
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using logical_and_member_exact_t =
        ::scl::detected_t<::scl::logical_and_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() || std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using logical_or_operation = decltype(::std::declval<Left>() || ::std::declval<Right>());
    /**
     * @brief Whether ::scl::logical_or_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Arguments Template arguments of ::scl::logical_or_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_or_v = ::scl::is_detected_v<::scl::logical_or_operation, Arguments...>;
    /**
     * @brief ::scl::logical_or_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Arguments Template arguments of ::scl::logical_or_operation.
     */
    template <typename... Arguments>
    using logical_or_t = ::scl::detected_t<::scl::logical_or_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator||(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using logical_or_member_operation = decltype(::std::declval<Type>().operator||(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::logical_or_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_or_member_v =
        ::scl::is_detected_v<::scl::logical_or_member_operation, Arguments...>;
    /**
     * @brief ::scl::logical_or_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using logical_or_member_t = ::scl::detected_t<::scl::logical_or_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator||` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator||` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator||`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using logical_or_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator||` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_logical_or_member_exact_v =
        ::scl::is_detected_v<::scl::logical_or_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::logical_or_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp logical_or
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using logical_or_member_exact_t =
        ::scl::detected_t<::scl::logical_or_member_exact_operation, Arguments...>;

    /** @} */

    /**
     * @defgroup scl_utility_operator_detection_assignment Assignment operators
     * @{
     */

    /**
     * @brief The type of `std::declval<Left>() = std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using assign_operation = decltype(::std::declval<Left>() = ::std::declval<Right>());
    /**
     * @brief Whether ::scl::assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Arguments Template arguments of ::scl::assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_assign_v = ::scl::is_detected_v<::scl::assign_operation, Arguments...>;
    /**
     * @brief ::scl::assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Arguments Template arguments of ::scl::assign_operation.
     */
    template <typename... Arguments>
    using assign_t = ::scl::detected_t<::scl::assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using assign_member_operation = decltype(::std::declval<Type>().operator=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_assign_member_v =
        ::scl::is_detected_v<::scl::assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using assign_member_t = ::scl::detected_t<::scl::assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_assign_member_exact_v =
        ::scl::is_detected_v<::scl::assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using assign_member_exact_t = ::scl::detected_t<::scl::assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() *= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using multiplies_assign_operation = decltype(::std::declval<Left>() *= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::multiplies_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Arguments Template arguments of ::scl::multiplies_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_multiplies_assign_v =
        ::scl::is_detected_v<::scl::multiplies_assign_operation, Arguments...>;
    /**
     * @brief ::scl::multiplies_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Arguments Template arguments of ::scl::multiplies_assign_operation.
     */
    template <typename... Arguments>
    using multiplies_assign_t = ::scl::detected_t<::scl::multiplies_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator*=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using multiplies_assign_member_operation = decltype(::std::declval<Type>().operator*=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::multiplies_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_multiplies_assign_member_v =
        ::scl::is_detected_v<::scl::multiplies_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::multiplies_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using multiplies_assign_member_t =
        ::scl::detected_t<::scl::multiplies_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator*=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator*=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator*=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using multiplies_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator*=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_multiplies_assign_member_exact_v =
        ::scl::is_detected_v<::scl::multiplies_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::multiplies_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp multiplies_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using multiplies_assign_member_exact_t =
        ::scl::detected_t<::scl::multiplies_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() /= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using divides_assign_operation = decltype(::std::declval<Left>() /= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::divides_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Arguments Template arguments of ::scl::divides_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_divides_assign_v =
        ::scl::is_detected_v<::scl::divides_assign_operation, Arguments...>;
    /**
     * @brief ::scl::divides_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Arguments Template arguments of ::scl::divides_assign_operation.
     */
    template <typename... Arguments>
    using divides_assign_t = ::scl::detected_t<::scl::divides_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator/=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using divides_assign_member_operation = decltype(::std::declval<Type>().operator/=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::divides_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_divides_assign_member_v =
        ::scl::is_detected_v<::scl::divides_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::divides_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using divides_assign_member_t = ::scl::detected_t<::scl::divides_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator/=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator/=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator/=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using divides_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator/=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_divides_assign_member_exact_v =
        ::scl::is_detected_v<::scl::divides_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::divides_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp divides_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using divides_assign_member_exact_t =
        ::scl::detected_t<::scl::divides_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() %= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using modulus_assign_operation = decltype(::std::declval<Left>() %= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::modulus_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Arguments Template arguments of ::scl::modulus_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_modulus_assign_v =
        ::scl::is_detected_v<::scl::modulus_assign_operation, Arguments...>;
    /**
     * @brief ::scl::modulus_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Arguments Template arguments of ::scl::modulus_assign_operation.
     */
    template <typename... Arguments>
    using modulus_assign_t = ::scl::detected_t<::scl::modulus_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator%=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using modulus_assign_member_operation = decltype(::std::declval<Type>().operator%=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::modulus_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_modulus_assign_member_v =
        ::scl::is_detected_v<::scl::modulus_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::modulus_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using modulus_assign_member_t = ::scl::detected_t<::scl::modulus_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator%=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator%=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator%=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using modulus_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator%=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_modulus_assign_member_exact_v =
        ::scl::is_detected_v<::scl::modulus_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::modulus_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp modulus_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using modulus_assign_member_exact_t =
        ::scl::detected_t<::scl::modulus_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() += std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using plus_assign_operation = decltype(::std::declval<Left>() += ::std::declval<Right>());
    /**
     * @brief Whether ::scl::plus_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Arguments Template arguments of ::scl::plus_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_plus_assign_v =
        ::scl::is_detected_v<::scl::plus_assign_operation, Arguments...>;
    /**
     * @brief ::scl::plus_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Arguments Template arguments of ::scl::plus_assign_operation.
     */
    template <typename... Arguments>
    using plus_assign_t = ::scl::detected_t<::scl::plus_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator+=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using plus_assign_member_operation = decltype(::std::declval<Type>().operator+=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::plus_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_plus_assign_member_v =
        ::scl::is_detected_v<::scl::plus_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::plus_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using plus_assign_member_t = ::scl::detected_t<::scl::plus_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator+=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator+=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator+=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using plus_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator+=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_plus_assign_member_exact_v =
        ::scl::is_detected_v<::scl::plus_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::plus_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp plus_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using plus_assign_member_exact_t =
        ::scl::detected_t<::scl::plus_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() -= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using minus_assign_operation = decltype(::std::declval<Left>() -= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::minus_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Arguments Template arguments of ::scl::minus_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_minus_assign_v =
        ::scl::is_detected_v<::scl::minus_assign_operation, Arguments...>;
    /**
     * @brief ::scl::minus_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Arguments Template arguments of ::scl::minus_assign_operation.
     */
    template <typename... Arguments>
    using minus_assign_t = ::scl::detected_t<::scl::minus_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator-=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using minus_assign_member_operation = decltype(::std::declval<Type>().operator-=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::minus_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_minus_assign_member_v =
        ::scl::is_detected_v<::scl::minus_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::minus_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using minus_assign_member_t = ::scl::detected_t<::scl::minus_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator-=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator-=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator-=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using minus_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator-=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_minus_assign_member_exact_v =
        ::scl::is_detected_v<::scl::minus_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::minus_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp minus_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using minus_assign_member_exact_t =
        ::scl::detected_t<::scl::minus_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() <<= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using left_shift_assign_operation = decltype(::std::declval<Left>() <<= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::left_shift_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Arguments Template arguments of ::scl::left_shift_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_left_shift_assign_v =
        ::scl::is_detected_v<::scl::left_shift_assign_operation, Arguments...>;
    /**
     * @brief ::scl::left_shift_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Arguments Template arguments of ::scl::left_shift_assign_operation.
     */
    template <typename... Arguments>
    using left_shift_assign_t = ::scl::detected_t<::scl::left_shift_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator<<=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using left_shift_assign_member_operation = decltype(::std::declval<Type>().operator<<=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::left_shift_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_left_shift_assign_member_v =
        ::scl::is_detected_v<::scl::left_shift_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::left_shift_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using left_shift_assign_member_t =
        ::scl::detected_t<::scl::left_shift_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator<<=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator<<=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator<<=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using left_shift_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator<<=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_left_shift_assign_member_exact_v =
        ::scl::is_detected_v<::scl::left_shift_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::left_shift_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp left_shift_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using left_shift_assign_member_exact_t =
        ::scl::detected_t<::scl::left_shift_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() >>= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using right_shift_assign_operation = decltype(::std::declval<Left>() >>= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::right_shift_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Arguments Template arguments of ::scl::right_shift_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_right_shift_assign_v =
        ::scl::is_detected_v<::scl::right_shift_assign_operation, Arguments...>;
    /**
     * @brief ::scl::right_shift_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Arguments Template arguments of ::scl::right_shift_assign_operation.
     */
    template <typename... Arguments>
    using right_shift_assign_t = ::scl::detected_t<::scl::right_shift_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator>>=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using right_shift_assign_member_operation = decltype(::std::declval<Type>().operator>>=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::right_shift_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_right_shift_assign_member_v =
        ::scl::is_detected_v<::scl::right_shift_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::right_shift_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using right_shift_assign_member_t =
        ::scl::detected_t<::scl::right_shift_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator>>=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator>>=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator>>=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using right_shift_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator>>=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_right_shift_assign_member_exact_v =
        ::scl::is_detected_v<::scl::right_shift_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::right_shift_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp right_shift_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using right_shift_assign_member_exact_t =
        ::scl::detected_t<::scl::right_shift_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() &= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bit_and_assign_operation = decltype(::std::declval<Left>() &= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::bit_and_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Arguments Template arguments of ::scl::bit_and_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_and_assign_v =
        ::scl::is_detected_v<::scl::bit_and_assign_operation, Arguments...>;
    /**
     * @brief ::scl::bit_and_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Arguments Template arguments of ::scl::bit_and_assign_operation.
     */
    template <typename... Arguments>
    using bit_and_assign_t = ::scl::detected_t<::scl::bit_and_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator&=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_and_assign_member_operation = decltype(::std::declval<Type>().operator&=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_and_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_and_assign_member_v =
        ::scl::is_detected_v<::scl::bit_and_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_and_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_and_assign_member_t = ::scl::detected_t<::scl::bit_and_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator&=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator&=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator&=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_and_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator&=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_and_assign_member_exact_v =
        ::scl::is_detected_v<::scl::bit_and_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_and_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_and_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_and_assign_member_exact_t =
        ::scl::detected_t<::scl::bit_and_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() |= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bit_or_assign_operation = decltype(::std::declval<Left>() |= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::bit_or_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Arguments Template arguments of ::scl::bit_or_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_or_assign_v =
        ::scl::is_detected_v<::scl::bit_or_assign_operation, Arguments...>;
    /**
     * @brief ::scl::bit_or_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Arguments Template arguments of ::scl::bit_or_assign_operation.
     */
    template <typename... Arguments>
    using bit_or_assign_t = ::scl::detected_t<::scl::bit_or_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator|=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_or_assign_member_operation = decltype(::std::declval<Type>().operator|=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_or_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_or_assign_member_v =
        ::scl::is_detected_v<::scl::bit_or_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_or_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_or_assign_member_t = ::scl::detected_t<::scl::bit_or_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator|=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator|=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator|=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_or_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator|=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_or_assign_member_exact_v =
        ::scl::is_detected_v<::scl::bit_or_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_or_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_or_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_or_assign_member_exact_t =
        ::scl::detected_t<::scl::bit_or_assign_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>() ^= std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bit_xor_assign_operation = decltype(::std::declval<Left>() ^= ::std::declval<Right>());
    /**
     * @brief Whether ::scl::bit_xor_assign_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Arguments Template arguments of ::scl::bit_xor_assign_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_xor_assign_v =
        ::scl::is_detected_v<::scl::bit_xor_assign_operation, Arguments...>;
    /**
     * @brief ::scl::bit_xor_assign_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Arguments Template arguments of ::scl::bit_xor_assign_operation.
     */
    template <typename... Arguments>
    using bit_xor_assign_t = ::scl::detected_t<::scl::bit_xor_assign_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator^=(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_xor_assign_member_operation = decltype(::std::declval<Type>().operator^=(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::bit_xor_assign_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_xor_assign_member_v =
        ::scl::is_detected_v<::scl::bit_xor_assign_member_operation, Arguments...>;
    /**
     * @brief ::scl::bit_xor_assign_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using bit_xor_assign_member_t = ::scl::detected_t<::scl::bit_xor_assign_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator^=` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator^=` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator^=`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using bit_xor_assign_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator^=` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_bit_xor_assign_member_exact_v =
        ::scl::is_detected_v<::scl::bit_xor_assign_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::bit_xor_assign_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp bit_xor_assign
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using bit_xor_assign_member_exact_t =
        ::scl::detected_t<::scl::bit_xor_assign_member_exact_operation, Arguments...>;

    /** @} */

    /**
     * @defgroup scl_utility_operator_detection_special Arrow, comma, subscript and call operators
     * @{
     */

    /**
     * @brief The type of `std::declval<Type>().operator->*(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using arrow_to_pointer_operation = decltype(::std::declval<Type>().operator->*(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::arrow_to_pointer_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Arguments Template arguments of ::scl::arrow_to_pointer_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_arrow_to_pointer_v =
        ::scl::is_detected_v<::scl::arrow_to_pointer_operation, Arguments...>;
    /**
     * @brief ::scl::arrow_to_pointer_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Arguments Template arguments of ::scl::arrow_to_pointer_operation.
     */
    template <typename... Arguments>
    using arrow_to_pointer_t = ::scl::detected_t<::scl::arrow_to_pointer_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator->*(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using arrow_to_pointer_member_operation = decltype(::std::declval<Type>().operator->*(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::arrow_to_pointer_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_arrow_to_pointer_member_v =
        ::scl::is_detected_v<::scl::arrow_to_pointer_member_operation, Arguments...>;
    /**
     * @brief ::scl::arrow_to_pointer_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using arrow_to_pointer_member_t =
        ::scl::detected_t<::scl::arrow_to_pointer_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator->*` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator->*` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator->*`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using arrow_to_pointer_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator->*` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_arrow_to_pointer_member_exact_v =
        ::scl::is_detected_v<::scl::arrow_to_pointer_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::arrow_to_pointer_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow_to_pointer
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using arrow_to_pointer_member_exact_t =
        ::scl::detected_t<::scl::arrow_to_pointer_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator->(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using arrow_operation = decltype(::std::declval<Type>().operator->(::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::arrow_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Arguments Template arguments of ::scl::arrow_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_arrow_v = ::scl::is_detected_v<::scl::arrow_operation, Arguments...>;
    /**
     * @brief ::scl::arrow_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Arguments Template arguments of ::scl::arrow_operation.
     */
    template <typename... Arguments>
    using arrow_t = ::scl::detected_t<::scl::arrow_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator->(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using arrow_member_operation = decltype(::std::declval<Type>().operator->(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::arrow_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_arrow_member_v =
        ::scl::is_detected_v<::scl::arrow_member_operation, Arguments...>;
    /**
     * @brief ::scl::arrow_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using arrow_member_t = ::scl::detected_t<::scl::arrow_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator->` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator->` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator->`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using arrow_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator->` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_arrow_member_exact_v =
        ::scl::is_detected_v<::scl::arrow_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::arrow_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp arrow
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using arrow_member_exact_t = ::scl::detected_t<::scl::arrow_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Left>(), std::declval<Right>()`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using comma_operation = decltype(::std::declval<Left>(), ::std::declval<Right>());
    /**
     * @brief Whether ::scl::comma_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Arguments Template arguments of ::scl::comma_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_comma_v = ::scl::is_detected_v<::scl::comma_operation, Arguments...>;
    /**
     * @brief ::scl::comma_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Arguments Template arguments of ::scl::comma_operation.
     */
    template <typename... Arguments>
    using comma_t = ::scl::detected_t<::scl::comma_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator ,(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using comma_member_operation =
        decltype(::std::declval<Type>().operator,(::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::comma_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_comma_member_v =
        ::scl::is_detected_v<::scl::comma_member_operation, Arguments...>;
    /**
     * @brief ::scl::comma_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using comma_member_t = ::scl::detected_t<::scl::comma_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator ,` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator ,` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator ,`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using comma_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator ,` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_comma_member_exact_v =
        ::scl::is_detected_v<::scl::comma_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::comma_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp comma
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using comma_member_exact_t = ::scl::detected_t<::scl::comma_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>()[std::declval<Argument>()]`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Type Type of the object.
     * @tparam Argument Type of the index.
     */
    template <typename Type, typename Argument>
    using subscript_operation = decltype(::std::declval<Type>()[::std::declval<Argument>()]);
    /**
     * @brief Whether ::scl::subscript_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Arguments Template arguments of ::scl::subscript_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_subscript_v = ::scl::is_detected_v<::scl::subscript_operation, Arguments...>;
    /**
     * @brief ::scl::subscript_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Arguments Template arguments of ::scl::subscript_operation.
     */
    template <typename... Arguments>
    using subscript_t = ::scl::detected_t<::scl::subscript_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator[](std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using subscript_member_operation = decltype(::std::declval<Type>().operator[](
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::subscript_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_subscript_member_v =
        ::scl::is_detected_v<::scl::subscript_member_operation, Arguments...>;
    /**
     * @brief ::scl::subscript_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using subscript_member_t = ::scl::detected_t<::scl::subscript_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator[]` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator[]` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator[]`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using subscript_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator[]` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_subscript_member_exact_v =
        ::scl::is_detected_v<::scl::subscript_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::subscript_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp subscript
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using subscript_member_exact_t = ::scl::detected_t<::scl::subscript_member_exact_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>()(std::declval<Arguments>()...)`.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using call_operation = decltype(::std::declval<Type>()(::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::call_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Arguments Template arguments of ::scl::call_operation.
     */
    template <typename... Arguments>
    inline constexpr auto has_call_v = ::scl::is_detected_v<::scl::call_operation, Arguments...>;
    /**
     * @brief ::scl::call_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Arguments Template arguments of ::scl::call_operation.
     */
    template <typename... Arguments>
    using call_t = ::scl::detected_t<::scl::call_operation, Arguments...>;

    /**
     * @brief The type of `std::declval<Type>().operator()(std::declval<Arguments>()...)`.
     *
     * A built-in operator and a free function are not found.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using call_member_operation = decltype(::std::declval<Type>().operator()(
        ::std::declval<Arguments>()...));
    /**
     * @brief Whether ::scl::call_member_operation is well-formed for @p Arguments.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    inline constexpr auto has_call_member_v =
        ::scl::is_detected_v<::scl::call_member_operation, Arguments...>;
    /**
     * @brief ::scl::call_member_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Arguments Type of the object, then argument types.
     */
    template <typename... Arguments>
    using call_member_t = ::scl::detected_t<::scl::call_member_operation, Arguments...>;

    /**
     * @brief The type of the call of the member function `operator()` with exactly these parameters.
     *
     * The operation is well-formed only where the class `std::remove_cvref_t<Type>` or a public
     * base class of it declares `operator()` with exactly those parameters and with
     * the cv-qualifiers and the ref-qualifier of @p Type. Where the overloads of `operator()`
     * include a member function template, only the overload whose result type is that of the plain
     * call is found, so for an ambiguous plain call the operation is ill-formed. The `noexcept` of
     * the member function is not compared.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Type Type of the object.
     * @tparam Arguments Argument types.
     */
    template <typename Type, typename... Arguments>
    using call_member_exact_operation = unspecified;
    /**
     * @brief Whether `operator()` is declared exactly for the given object and parameter types.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    inline constexpr auto has_call_member_exact_v =
        ::scl::is_detected_v<::scl::call_member_exact_operation, Arguments...>;
    /**
     * @brief ::scl::call_member_exact_operation for @p Arguments, or ::scl::nonesuch.
     *
     * @snippet type_traits/operator_detection/type_traits_operator_detection_example.cpp call
     *
     * @tparam Arguments Type of the object, then parameter types.
     */
    template <typename... Arguments>
    using call_member_exact_t = ::scl::detected_t<::scl::call_member_exact_operation, Arguments...>;

    /** @} */

    /** @} */
} // namespace scl

#endif
