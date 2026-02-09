/**
     * @file
     * @ingroup scl_utility_type_traits
     * @brief Operator detection traits: compile-time aliases and value checks (Detection Idiom)
     * @details
     * This header declares a comprehensive suite of compile-time detection aliases and constexpr values
     * for C++ operators using the Detection Idiom (N4502), implemented with `decltype` and `std::declval`.
 *
     * For each supported operator, the following symbols are provided (expanded via internal macros in the
     * non-Doxygen branch of this header; documented here for Doxygen users):
 *
     * - Operation (expression) aliases:
     *   - `name_operation<...>`: SFINAE-friendly alias representing the operator expression (built-in or free function).
     *   - `has_name_v<...>`: constexpr boolean; true if `name_operation<...>` is well-formed.
     *   - `name_t<...>`: alias to the detected expression type, or `scl::nonesuch` on failure.
 *
     * - Member detection aliases:
     *   - `name_member_operation<Type, ...>`: alias representing `std::declval<Type>().operator op(...)`.
     *   - `has_name_member_v<...>`: constexpr boolean; true if the member-form expression is well-formed.
     *   - `name_member_t<...>`: alias to the detected result type for the member-form expression, or `scl::nonesuch`.
 *
     * - Exact member detection aliases:
     *   - `name_member_exact_operation<Type, ...>`: alias using pointer-to-member to require an actual class member with the
     *     exact signature implied by the non-exact member operation (including ref-qualifiers via `scl::member_function_like_t`).
     *   - `has_name_member_exact_v<...>`: constexpr boolean; true if the pointer-to-member expression is well-formed.
     *   - `name_member_exact_t<...>`: alias to the detected result type for the exact member expression, or `scl::nonesuch`.
 *
     * Operators covered:
     * - Prefix unary: `&` (address_of), `*` (indirection), `+` (unary_plus), `-` (unary_minus),
     *   `++` (prefix_increment), `--` (prefix_decrement), `~` (bitwise_not), `!` (logical_not)
     * - Postfix unary: `++` (postfix_increment), `--` (postfix_decrement)
     * - Binary comparisons: `==` (equal_to), `!=` (not_equal_to), `<` (less), `<=` (less_equal), `>` (greater), `>=` (greater_equal)
     * - Binary arithmetic/bitwise: `*` (multiply), `/` (divide), `%` (modulo), `+` (plus), `-` (minus),
     *   `<<` (left_shift), `>>` (right_shift), `&` (bitwise_and), `|` (bitwise_or), `^` (bitwise_xor)
     * - Logical binary: `&&` (logical_and), `||` (logical_or)
     * - Assignment: `=` (assign), `+=` (plus_assign), `-=` (minus_assign), `*=` (multiply_assign), `/=` (divide_assign),
     *   `%=` (modulo_assign), `<<=` (left_shift_assign), `>>=` (right_shift_assign), `&=` (bitwise_and_assign),
     *   `|=` (bitwise_or_assign), `^=` (bitwise_xor_assign)
     * - Special: `->*` (arrow_to_pointer), `->` (arrow), `,` (comma), `[]` (subscript), `()` (call)
 *
     * Implementation notes:
     * - All public symbols live in the `scl` namespace.
     * - Detection is implemented via alias templates using `decltype` and `std::declval`.
     * - Exact member detection uses pointer-to-member and `scl::member_function_like_t` to enforce exact signature and ref-qualifiers.
     */

#pragma once

#include <scl/utility/preprocessor/forward.h>
#include <scl/utility/type_traits/detection.h>
#include <scl/utility/type_traits/member_like.h>

#ifndef DOXYGEN

#define SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION(op, name)                                                                \
    namespace scl                                                                                                        \
    {                                                                                                                    \
        template <typename Type, typename... Arguments>                                                                  \
        using name##_member_operation =                                                                                  \
            decltype(::std::declval<Type>().operator op /**/ (::std::declval<Arguments>()...));                          \
        template <typename... Arguments>                                                                                 \
        inline constexpr auto has_##name##_member_v =                                                                    \
            ::scl::is_detected_v<name##_member_operation, Arguments...>;                                                 \
        template <typename... Arguments>                                                                                 \
        using name##_member_t = ::scl::detected_t<name##_member_operation, Arguments...>;                                \
    }                                                                                                                    \
    namespace scl                                                                                                        \
    {                                                                                                                    \
        template <typename Type, typename... Arguments>                                                                  \
        using name##_member_exact_operation = decltype((::std::declval<Type>().*                                         \
            static_cast<::scl::member_function_like_t<Type, name##_member_operation<Type, Arguments...>(Arguments...)>>( \
                &::std::remove_cvref_t<Type>::operator op /**/))(::std::declval<Arguments>()...));                       \
        template <typename... Arguments>                                                                                 \
        inline constexpr auto has_##name##_member_exact_v =                                                              \
            ::scl::is_detected_v<name##_member_exact_operation, Arguments...>;                                           \
        template <typename... Arguments>                                                                                 \
        using name##_member_exact_t = ::scl::detected_t<name##_member_exact_operation, Arguments...>;                    \
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

#define SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION(op, name)                                              \
    namespace scl                                                                                          \
    {                                                                                                      \
        template <typename Type>                                                                           \
        using name##_operation = decltype(::std::declval<Type>() op /**/);                                 \
        template <typename... Arguments>                                                                   \
        inline constexpr auto has_##name##_v = ::scl::is_detected_v<name##_operation, Arguments...>;       \
        template <typename... Arguments>                                                                   \
        using name##_t = ::scl::detected_t<name##_operation, Arguments...>;                                \
    }                                                                                                      \
    namespace scl                                                                                          \
    {                                                                                                      \
        template <typename Type>                                                                           \
        using name##_member_operation =                                                                    \
            decltype(::std::declval<Type>().operator op /**/ (::std::declval<int>()));                     \
        template <typename Type>                                                                           \
        inline constexpr auto has_##name##_member_v = ::scl::is_detected_v<name##_member_operation, Type>; \
        template <typename Type>                                                                           \
        using name##_member_t = ::scl::detected_t<name##_member_operation, Type>;                          \
    }                                                                                                      \
    namespace scl                                                                                          \
    {                                                                                                      \
        template <typename Type>                                                                           \
        using name##_member_exact_operation = decltype((::std::declval<Type>().*                           \
            static_cast<::scl::member_function_like_t<Type, name##_member_operation<Type>(int)>>(          \
                &::std::remove_cvref_t<Type>::operator op /**/))(::std::declval<int>()));                  \
        template <typename Type>                                                                           \
        inline constexpr auto has_##name##_member_exact_v =                                                \
            ::scl::is_detected_v<name##_member_exact_operation, Type>;                                     \
        template <typename Type>                                                                           \
        using name##_member_exact_t = ::scl::detected_t<name##_member_exact_operation, Type>;              \
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
        using name##_operation =                                                                     \
            decltype(::std::declval<Type>().operator op /**/ (::std::declval<Arguments>()...));      \
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
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(-, unary_minus)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(++, prefix_increment)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(--, prefix_decrement)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(~, bitwise_not)
SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION(!, logical_not)

SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION(++, postfix_increment)
SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION(--, postfix_decrement)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(==, equal_to)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(!=, not_equal_to)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(<, less)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(<=, less_equal)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>, greater)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>=, greater_equal)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(*, multiply)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(/, divide)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(%, modulo)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(+, plus)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(-, minus)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(<<, left_shift)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>>, right_shift)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(&, bitwise_and)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(|, bitwise_or)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(^, bitwise_xor)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(&&, logical_and)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(||, logical_or)

SCL_DETAIL_BINARY_OPERATOR_DETECTION(=, assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(*=, multiply_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(/=, divide_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(%=, modulo_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(+=, plus_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(-=, minus_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(<<=, left_shift_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(>>=, right_shift_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(&=, bitwise_and_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(|=, bitwise_or_assign)
SCL_DETAIL_BINARY_OPERATOR_DETECTION(^=, bitwise_xor_assign)

#undef SCL_DETAIL_PREFIX_UNARY_OPERATOR_DETECTION
#undef SCL_DETAIL_POSTFIX_UNARY_OPERATOR_DETECTION
#undef SCL_DETAIL_BINARY_OPERATOR_DETECTION
#undef SCL_DETAIL_ARROW_OPERATOR_DETECTION
#undef SCL_DETAIL_ARGUMENTS_OPERATOR_DETECTION

#else // DOXYGEN

namespace scl
{
    /**
     * @defgroup scl_utility_operator_detection Operator Detection
     * @ingroup scl_utility_type_traits
     * @brief Compile-time traits for detecting overloaded operators using the Detection Idiom.
     * 
     * This module provides a mechanism to check at compile-time if a specific operator usage is valid for given types.
     * 
     * ### Naming Conventions
     * For an operator named `name` (e.g., `plus`, `address_of`), the following traits are generated:
     * 
     * - `has_name_v<Args...>`: Checks if the operator expression is valid (Unstrict). Supports implicit conversions, free functions, and members.
     * - `has_name_member_v<T, Args...>`: Checks if the member function `operator op` exists (Unstrict). Allows implicit conversions.
     * - `has_name_member_exact_v<T, Args...>`: Checks if the member function `operator op` exists with **exact** argument types (Strict).
     * 
     * @{
     */

    // ============================================================================
    // PREFIX UNARY OPERATORS: &, *, +, -, ++, --, ~, !
    // ============================================================================

    /**
     * @defgroup op_det_unary_prefix Prefix Unary Operators
     * @brief Detection for operators: `&`, `*`, `+`, `-`, `++`, `--`, `~`, `!`
     * @{
     */

    /**
     * @brief Alias for the address-of expression `&std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression used to probe address-of support for a type.
     * Suitable for SFINAE; no runtime evaluation occurs.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using address_of_operation = /* implementation defined */;
    /**
     * @brief Detects address-of operator (`&obj`).
     * @details
     * True if `&std::declval<Type>()` is a well-formed expression (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_address_of_v = /* implementation defined */;
    /**
     * @brief Detected result type of address-of, or `scl::nonesuch`.
     * @details
     * Yields the type produced by `&std::declval<Type>()` if available; otherwise `scl::nonesuch`.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using address_of_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member address-of `std::declval<Type>().operator&()`.
     * @details
     * Probes the availability of a class member `operator&` with arguments convertible from `Arguments...`.
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (if any).
     */
    template <typename Type, typename... Arguments>
    using address_of_member_operation = /* implementation defined */;
    /**
     * @brief Detects member address-of operator (`obj.operator&()`).
     * @details
     * True if `address_of_member_operation<Type, Arguments...>` is well-formed. Implicit conversions are allowed.
     * @tparam Arguments First type is the class type, followed by argument types.
     */
    template <typename... Arguments>
    inline constexpr bool has_address_of_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member address-of, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator&(Arguments...)` if present; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type, followed by argument types.
     */
    template <typename... Arguments>
    using address_of_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member address-of detection.
     * @details
     * Uses pointer-to-member semantics and `member_function_like_t` to constrain lookup to an exact member signature
     * (including cv- and ref-qualifiers). Implicit conversions are not considered.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types.
     */
    template <typename Type, typename... Arguments>
    using address_of_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member address-of operator (exact).
     * @details
     * True if `address_of_member_exact_operation<Type, Arguments...>` is well-formed under exact signature matching.
     * @tparam Arguments First type is the class type, followed by exact argument types.
     */
    template <typename... Arguments>
    inline constexpr bool has_address_of_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member address-of, or `scl::nonesuch`.
     * @details
     * Exposes the exact member `operator&` return type under signature equality; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type, followed by exact argument types.
     */
    template <typename... Arguments>
    using address_of_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the indirection expression `*std::declval<Type>()`.
     * @details
     * Represents the unevaluated unary dereference expression used for detection.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using indirection_operation = /* implementation defined */;
    /**
     * @brief Detects indirection operator (`*ptr`).
     * @details
     * Checks if the expression `*std::declval<Type>()` is well-formed (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_indirection_v = /* implementation defined */;
    /**
     * @brief Detected result type of indirection, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `*std::declval<Type>()` when valid; `scl::nonesuch` otherwise.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using indirection_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member indirection `std::declval<Type>().operator*()`.
     * @details
     * Probes a class member `operator*` without enforcing exact signature (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none for unary `*`).
     */
    template <typename Type, typename... Arguments>
    using indirection_member_operation = /* implementation defined */;
    /**
     * @brief Detects member indirection operator (`obj.operator*()`).
     * @details
     * True if `indirection_member_operation<Type, Arguments...>` is well-formed. Useful for ref-qualified members.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_indirection_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member indirection, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator*()` if present; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using indirection_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member indirection detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers when checking for `operator*` as a true class member.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none for unary `*`).
     */
    template <typename Type, typename... Arguments>
    using indirection_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member indirection operator (`obj.operator*()` exact).
     * @details
     * True if a precisely matching member `operator*` exists; implicit conversions are not considered.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_indirection_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member indirection, or `scl::nonesuch`.
     * @details
     * Exposes the exact member dereference return type; `scl::nonesuch` if not found.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using indirection_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the unary plus expression `+std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression to test unary `+` availability (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using unary_plus_operation = /* implementation defined */;
    /**
     * @brief Detects unary plus operator (`+obj`).
     * @details
     * True if `+std::declval<Type>()` is a well-formed expression.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_unary_plus_v = /* implementation defined */;
    /**
     * @brief Detected result type of unary plus, or `scl::nonesuch`.
     * @details
     * Exposes the return type of unary plus for `Type` when available.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using unary_plus_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member unary plus `std::declval<Type>().operator+()`.
     * @details
     * Detects a member `operator+` on `Type` without enforcing exact signature.
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using unary_plus_member_operation = /* implementation defined */;
    /**
     * @brief Detects member unary plus operator (`obj.operator+()`).
     * @details
     * True if `unary_plus_member_operation<Type>` is well-formed.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_unary_plus_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member unary plus, or `scl::nonesuch`.
     * @details
     * Exposes the member unary plus return type for `Type`, else `scl::nonesuch`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using unary_plus_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member unary plus detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator+`.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using unary_plus_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member unary plus operator (`obj.operator+()` exact).
     * @details
     * True if an exactly matching member `operator+` exists.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_unary_plus_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member unary plus, or `scl::nonesuch`.
     * @details
     * Exposes the return type under exact signature match; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using unary_plus_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the unary minus expression `-std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression to test unary `-` availability (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using unary_minus_operation = /* implementation defined */;
    /**
     * @brief Detects unary minus operator (`-obj`).
     * @details
     * True if `-std::declval<Type>()` is well-formed.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_unary_minus_v = /* implementation defined */;
    /**
     * @brief Detected result type of unary minus, or `scl::nonesuch`.
     * @details
     * Exposes the return type of unary minus for `Type` when available.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using unary_minus_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member unary minus `std::declval<Type>().operator-()`.
     * @details
     * Detects a member `operator-` on `Type` without enforcing exact signature.
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using unary_minus_member_operation = /* implementation defined */;
    /**
     * @brief Detects member unary minus operator (`obj.operator-()`).
     * @details
     * True if `unary_minus_member_operation<Type>` is well-formed.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_unary_minus_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member unary minus, or `scl::nonesuch`.
     * @details
     * Exposes the member unary minus return type for `Type`, else `scl::nonesuch`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using unary_minus_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member unary minus detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator-`.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using unary_minus_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member unary minus operator (`obj.operator-()` exact).
     * @details
     * True if an exactly matching member `operator-` exists on `Type`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_unary_minus_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member unary minus, or `scl::nonesuch`.
     * @details
     * Exposes the return type under exact signature match; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using unary_minus_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the prefix increment expression `++std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression to test prefix `++` (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using prefix_increment_operation = /* implementation defined */;
    /**
     * @brief Detects prefix increment operator (`++obj`).
     * @details
     * True if `++std::declval<Type>()` is a well-formed expression.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_prefix_increment_v = /* implementation defined */;
    /**
     * @brief Detected result type of prefix increment, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by prefix increment for `Type`, else `scl::nonesuch`.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using prefix_increment_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member prefix increment `std::declval<Type>().operator++()`.
     * @details
     * Probes for `Type::operator++()` (implicit conversions allowed in non-exact detection).
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using prefix_increment_member_operation = /* implementation defined */;
    /**
     * @brief Detects member prefix increment operator (`obj.operator++()`).
     * @details
     * True if `prefix_increment_member_operation<Type>` is well-formed. Ref-qualification is honored by the compiler.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_prefix_increment_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member prefix increment, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator++()`; `scl::nonesuch` if unavailable.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using prefix_increment_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member prefix increment detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator++()`.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using prefix_increment_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member prefix increment operator (`obj.operator++()` exact).
     * @details
     * True if an exactly matching `operator++()` exists on `Type`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_prefix_increment_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member prefix increment, or `scl::nonesuch`.
     * @details
     * Useful to validate ref-qualified overloads (e.g., `&` vs `&&`).
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using prefix_increment_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the prefix decrement expression `--std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression to test prefix `--` (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using prefix_decrement_operation = /* implementation defined */;
    /**
     * @brief Detects prefix decrement operator (`--obj`).
     * @details
     * True if `--std::declval<Type>()` is a well-formed expression.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_prefix_decrement_v = /* implementation defined */;
    /**
     * @brief Detected result type of prefix decrement, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by prefix decrement for `Type`, else `scl::nonesuch`.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using prefix_decrement_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member prefix decrement `std::declval<Type>().operator--()`.
     * @details
     * Probes for `Type::operator--()` without exact matching requirements.
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using prefix_decrement_member_operation = /* implementation defined */;
    /**
     * @brief Detects member prefix decrement operator (`obj.operator--()`).
     * @details
     * True if `prefix_decrement_member_operation<Type>` is well-formed.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_prefix_decrement_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member prefix decrement, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator--()` if present.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using prefix_decrement_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member prefix decrement detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator--()`.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using prefix_decrement_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member prefix decrement operator (`obj.operator--()` exact).
     * @details
     * True if an exactly matching `operator--()` exists on `Type`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_prefix_decrement_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member prefix decrement, or `scl::nonesuch`.
     * @details
     * Ensures distinction between ref-qualified members is respected.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using prefix_decrement_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the bitwise NOT expression `~std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression to test `~` availability (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using bitwise_not_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise NOT operator (`~obj`).
     * @details
     * True if `~std::declval<Type>()` is well-formed.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_bitwise_not_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise NOT, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `~obj` when available.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using bitwise_not_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise NOT `std::declval<Type>().operator~()`.
     * @details
     * Detects a member `operator~` on `Type` without enforcing exact signature.
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using bitwise_not_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise NOT operator (`obj.operator~()`).
     * @details
     * True if `bitwise_not_member_operation<Type>` is well-formed; useful for ref-qualified overloads.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_bitwise_not_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise NOT, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator~()` if available; else `scl::nonesuch`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using bitwise_not_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise NOT detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator~`.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using bitwise_not_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise NOT operator (`obj.operator~()` exact).
     * @details
     * True if an exact member `operator~` exists; implicit conversions are not considered.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_bitwise_not_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise NOT, or `scl::nonesuch`.
     * @details
     * Exposes the exact member `operator~` return type; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using bitwise_not_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the logical NOT expression `!std::declval<Type>()`.
     * @details
     * Represents the unevaluated expression to test `!` availability (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using logical_not_operation = /* implementation defined */;
    /**
     * @brief Detects logical NOT operator (`!obj`).
     * @details
     * True if `!std::declval<Type>()` is well-formed.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_logical_not_v = /* implementation defined */;
    /**
     * @brief Detected result type of logical NOT, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `!obj` when available.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using logical_not_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member logical NOT `std::declval<Type>().operator!()`.
     * @details
     * Detects a member `operator!` on `Type` without enforcing exact signature.
     * @tparam Type The class type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using logical_not_member_operation = /* implementation defined */;
    /**
     * @brief Detects member logical NOT operator (`obj.operator!()`).
     * @details
     * True if `logical_not_member_operation<Type>` is well-formed; ref-qualifiers respected.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_logical_not_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member logical NOT, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator!()`; `scl::nonesuch` if unavailable.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using logical_not_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member logical NOT detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator!`.
     * @tparam Type The class type.
     * @tparam Arguments Exact operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using logical_not_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member logical NOT operator (`obj.operator!()` exact).
     * @details
     * True if an exact member `operator!` exists on `Type`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_logical_not_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member logical NOT, or `scl::nonesuch`.
     * @details
     * Exposes the exact member `operator!` return type; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using logical_not_member_exact_t = /* implementation defined */;

    /** @} */

    // ============================================================================
    // POSTFIX UNARY OPERATORS: ++, -- (postfix)
    // ============================================================================

    /**
     * @defgroup op_det_unary_postfix Postfix Unary Operators
     * @brief Detection for operators: `++`, `--` (postfix versions)
     * @{
     */

    /**
     * @brief Alias for the postfix increment expression `std::declval<Type>()++`.
     * @details
     * Represents the unevaluated expression to test postfix `++` (built-in or overloaded).
     * @tparam Type The operand type.
     */
    template <typename Type>
    using postfix_increment_operation = /* implementation defined */;
    /**
     * @brief Detects postfix increment operator (`obj++`).
     * @details
     * True if `std::declval<Type>()++` is well-formed.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_postfix_increment_v = /* implementation defined */;
    /**
     * @brief Detected result type of postfix increment, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by postfix increment; `scl::nonesuch` if not available.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using postfix_increment_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member postfix increment `std::declval<Type>().operator++(int)`.
     * @details
     * Detects a member postfix `operator++(int)` on `Type` without exact signature enforcement.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_increment_member_operation = /* implementation defined */;
    /**
     * @brief Detects member postfix increment operator (`obj.operator++(int)`).
     * @details
     * True if `postfix_increment_member_operation<Type>` is well-formed.
     * @tparam Type The class type.
     */
    template <typename Type>
    inline constexpr bool has_postfix_increment_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member postfix increment, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator++(int)`, else `scl::nonesuch`.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_increment_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member postfix increment detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator++(int)`.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_increment_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member postfix increment operator (`obj.operator++(int)` exact).
     * @details
     * True if an exactly matching postfix increment member exists.
     * @tparam Type The class type.
     */
    template <typename Type>
    inline constexpr bool has_postfix_increment_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member postfix increment, or `scl::nonesuch`.
     * @details
     * Exposes the exact postfix increment return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_increment_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for the postfix decrement expression `std::declval<Type>()--`.
     * @details
     * Represents the unevaluated expression to test postfix `--`.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using postfix_decrement_operation = /* implementation defined */;
    /**
     * @brief Detects postfix decrement operator (`obj--`).
     * @details
     * True if `std::declval<Type>()--` is well-formed.
     * @tparam Type The operand type.
     */
    template <typename Type>
    inline constexpr bool has_postfix_decrement_v = /* implementation defined */;
    /**
     * @brief Detected result type of postfix decrement, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by postfix decrement; `scl::nonesuch` if not available.
     * @tparam Type The operand type.
     */
    template <typename Type>
    using postfix_decrement_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member postfix decrement `std::declval<Type>().operator--(int)`.
     * @details
     * Detects a member postfix `operator--(int)` on `Type` without exact signature enforcement.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_decrement_member_operation = /* implementation defined */;
    /**
     * @brief Detects member postfix decrement operator (`obj.operator--(int)`).
     * @details
     * True if `postfix_decrement_member_operation<Type>` is well-formed.
     * @tparam Type The class type.
     */
    template <typename Type>
    inline constexpr bool has_postfix_decrement_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member postfix decrement, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator--(int)`; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_decrement_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member postfix decrement detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for `Type::operator--(int)`.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_decrement_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member postfix decrement operator (`obj.operator--(int)` exact).
     * @details
     * True if an exactly matching postfix decrement member exists on `Type`.
     * @tparam Type The class type.
     */
    template <typename Type>
    inline constexpr bool has_postfix_decrement_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member postfix decrement, or `scl::nonesuch`.
     * @details
     * Exposes the exact postfix decrement return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     */
    template <typename Type>
    using postfix_decrement_member_exact_t = /* implementation defined */;

    /** @} */

    // ============================================================================
    // BINARY COMPARISON OPERATORS: ==, !=, <, <=, >, >=
    // ============================================================================

    /**
     * @defgroup op_det_binary_comp Comparison Operators
     * @brief Detection for operators: `==`, `!=`, `<`, `<=`, `>`, `>=`
     * @{
     */

    /**
     * @brief Alias for equality expression `std::declval<Left>() == std::declval<Right>()`.
     * @details
     * Represents the unevaluated comparison expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using equal_to_operation = /* implementation defined */;
    /**
     * @brief Detects equality operator (`x == y`).
     * @details
     * True if the equality expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_equal_to_v = /* implementation defined */;
    /**
     * @brief Detected result type of equality, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Left == Right` (typically `bool`) when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using equal_to_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member equality `std::declval<Type>().operator==(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator==` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using equal_to_member_operation = /* implementation defined */;
    /**
     * @brief Detects member equality operator (`obj.operator==(arg)`).
     * @details
     * True if `Type::operator==(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_equal_to_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member equality, or `scl::nonesuch`.
     * @details
     * Exposes the return type of the member equality operator if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using equal_to_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member equality detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator==`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using equal_to_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member equality operator (`obj.operator==(arg)` exact).
     * @details
     * True if an exactly matching `operator==` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_equal_to_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member equality, or `scl::nonesuch`.
     * @details
     * Exposes the exact comparison return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using equal_to_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for inequality expression `std::declval<Left>() != std::declval<Right>()`.
     * @details
     * Represents the unevaluated `!=` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using not_equal_to_operation = /* implementation defined */;
    /**
     * @brief Detects inequality operator (`x != y`).
     * @details
     * True if the inequality expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_not_equal_to_v = /* implementation defined */;
    /**
     * @brief Detected result type of inequality, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Left != Right` (typically `bool`) when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using not_equal_to_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member inequality `std::declval<Type>().operator!=(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator!=` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using not_equal_to_member_operation = /* implementation defined */;
    /**
     * @brief Detects member inequality operator (`obj.operator!=(arg)`).
     * @details
     * True if `Type::operator!=(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_not_equal_to_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member inequality, or `scl::nonesuch`.
     * @details
     * Exposes the return type of the member inequality operator if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using not_equal_to_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member inequality detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator!=`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using not_equal_to_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member inequality operator (`obj.operator!=(arg)` exact).
     * @details
     * True if an exactly matching `operator!=` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_not_equal_to_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member inequality, or `scl::nonesuch`.
     * @details
     * Exposes the exact comparison return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using not_equal_to_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for less-than expression `std::declval<Left>() < std::declval<Right>()`.
     * @details
     * Represents the unevaluated `<` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using less_operation = /* implementation defined */;
    /**
     * @brief Detects less-than operator (`x < y`).
     * @details
     * True if the less-than expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_less_v = /* implementation defined */;
    /**
     * @brief Detected result type of less-than, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Left < Right` (typically `bool`) when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using less_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member less-than `std::declval<Type>().operator<(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator<` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_member_operation = /* implementation defined */;
    /**
     * @brief Detects member less-than operator (`obj.operator<(arg)`).
     * @details
     * True if `Type::operator<(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_less_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member less-than, or `scl::nonesuch`.
     * @details
     * Exposes the return type of the member `operator<` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member less-than detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator<`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member less-than operator (`obj.operator<(arg)` exact).
     * @details
     * True if an exactly matching `operator<` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_less_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member less-than, or `scl::nonesuch`.
     * @details
     * Exposes the exact comparison return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for less-equal expression `std::declval<Left>() <= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `<=` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using less_equal_operation = /* implementation defined */;
    /**
     * @brief Detects less-equal operator (`x <= y`).
     * @details
     * True if the less-equal expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_less_equal_v = /* implementation defined */;
    /**
     * @brief Detected result type of less-equal, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Left <= Right` (typically `bool`) when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using less_equal_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member less-equal `std::declval<Type>().operator<=(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator<=` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_equal_member_operation = /* implementation defined */;
    /**
     * @brief Detects member less-equal operator (`obj.operator<=(arg)`).
     * @details
     * True if `Type::operator<=(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_less_equal_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member less-equal, or `scl::nonesuch`.
     * @details
     * Exposes the return type of the member `operator<=` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_equal_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member less-equal detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator<=`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_equal_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member less-equal operator (`obj.operator<=(arg)` exact).
     * @details
     * True if an exactly matching `operator<=` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_less_equal_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member less-equal, or `scl::nonesuch`.
     * @details
     * Exposes the exact comparison return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using less_equal_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for greater-than expression `std::declval<Left>() > std::declval<Right>()`.
     * @details
     * Represents the unevaluated `>` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using greater_operation = /* implementation defined */;
    /**
     * @brief Detects greater-than operator (`x > y`).
     * @details
     * True if the greater-than expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_greater_v = /* implementation defined */;
    /**
     * @brief Detected result type of greater-than, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Left > Right` (typically `bool`) when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using greater_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member greater-than `std::declval<Type>().operator>(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator>` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_member_operation = /* implementation defined */;
    /**
     * @brief Detects member greater-than operator (`obj.operator>(arg)`).
     * @details
     * True if `Type::operator>(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_greater_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member greater-than, or `scl::nonesuch`.
     * @details
     * Exposes the return type of the member `operator>` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member greater-than detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator>`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member greater-than operator (`obj.operator>(arg)` exact).
     * @details
     * True if an exactly matching `operator>` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_greater_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member greater-than, or `scl::nonesuch`.
     * @details
     * Exposes the exact comparison return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for greater-equal expression `std::declval<Left>() >= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `>=` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using greater_equal_operation = /* implementation defined */;
    /**
     * @brief Detects greater-equal operator (`x >= y`).
     * @details
     * True if the greater-equal expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_greater_equal_v = /* implementation defined */;
    /**
     * @brief Detected result type of greater-equal, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Left >= Right` (typically `bool`) when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using greater_equal_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member greater-equal `std::declval<Type>().operator>=(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator>=` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_equal_member_operation = /* implementation defined */;
    /**
     * @brief Detects member greater-equal operator (`obj.operator>=(arg)`).
     * @details
     * True if `Type::operator>=(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_greater_equal_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member greater-equal, or `scl::nonesuch`.
     * @details
     * Exposes the return type of the member `operator>=` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_equal_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member greater-equal detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator>=`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_equal_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member greater-equal operator (`obj.operator>=(arg)` exact).
     * @details
     * True if an exactly matching `operator>=` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_greater_equal_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member greater-equal, or `scl::nonesuch`.
     * @details
     * Exposes the exact comparison return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using greater_equal_member_exact_t = /* implementation defined */;

    /** @} */

    // ============================================================================
    // BINARY ARITHMETIC & BITWISE: *, /, %, +, -, <<, >>, &, |, ^
    // ============================================================================

    /**
     * @defgroup op_det_binary_arith Arithmetic and Bitwise Operators
     * @brief Detection for operators: `*`, `/`, `%`, `+`, `-`, `<<`, `>>`, `&`, `|`, `^`
     * @{
     */

    /**
     * @brief Alias for multiplication expression `std::declval<Left>()     * std::declval<Right>()`.
     * @details
     * Represents the unevaluated `*` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using multiply_operation = /* implementation defined */;
    /**
     * @brief Detects multiplication operator (`x     * y`).
     * @details
     * True if the product expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_multiply_v = /* implementation defined */;
    /**
     * @brief Detected result type of multiplication, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left     * Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using multiply_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member multiplication `std::declval<Type>().operator*(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator*(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using multiply_member_operation = /* implementation defined */;
    /**
     * @brief Detects member multiplication operator (`obj.operator*(arg)`).
     * @details
     * True if `Type::operator*(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_multiply_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member multiplication, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator*` if available.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using multiply_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member multiplication detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator*`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using multiply_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member multiplication operator (`obj.operator*(arg)` exact).
     * @details
     * True if an exactly matching member `operator*` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_multiply_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member multiplication, or `scl::nonesuch`.
     * @details
     * Exposes the exact multiplication return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using multiply_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for division expression `std::declval<Left>() / std::declval<Right>()`.
     * @details
     * Represents the unevaluated `/` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using divide_operation = /* implementation defined */;
    /**
     * @brief Detects division operator (`x / y`).
     * @details
     * True if the quotient expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_divide_v = /* implementation defined */;
    /**
     * @brief Detected result type of division, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left / Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using divide_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member division `std::declval<Type>().operator/(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator/(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using divide_member_operation = /* implementation defined */;
    /**
     * @brief Detects member division operator (`obj.operator/(arg)`).
     * @details
     * True if `Type::operator/(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_divide_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member division, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator/` if available.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using divide_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member division detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator/`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using divide_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member division operator (`obj.operator/(arg)` exact).
     * @details
     * True if an exactly matching member `operator/` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_divide_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member division, or `scl::nonesuch`.
     * @details
     * Exposes the exact division return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using divide_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for modulo expression `std::declval<Left>() % std::declval<Right>()`.
     * @details
     * Represents the unevaluated `%` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using modulo_operation = /* implementation defined */;
    /**
     * @brief Detects modulo operator (`x % y`).
     * @details
     * True if the remainder expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_modulo_v = /* implementation defined */;
    /**
     * @brief Detected result type of modulo, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left % Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using modulo_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member modulo `std::declval<Type>().operator%(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator%(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using modulo_member_operation = /* implementation defined */;
    /**
     * @brief Detects member modulo operator (`obj.operator%(arg)`).
     * @details
     * True if `Type::operator%(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_modulo_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member modulo, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator%` if available.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using modulo_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member modulo detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator%`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using modulo_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member modulo operator (`obj.operator%(arg)` exact).
     * @details
     * True if an exactly matching member `operator%` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_modulo_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member modulo, or `scl::nonesuch`.
     * @details
     * Exposes the exact modulo return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using modulo_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for addition expression `std::declval<Left>() + std::declval<Right>()`.
     * @details
     * Represents the unevaluated `+` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using plus_operation = /* implementation defined */;
    /**
     * @brief Detects addition operator (`x + y`).
     * @details
     * True if the sum expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_plus_v = /* implementation defined */;
    /**
     * @brief Detected result type of addition, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left + Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using plus_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member addition `std::declval<Type>().operator+(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator+(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using plus_member_operation = /* implementation defined */;
    /**
     * @brief Detects member addition operator (`obj.operator+(arg)`).
     * @details
     * True if `Type::operator+(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_plus_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member addition, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator+` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using plus_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member addition detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator+`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using plus_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member addition operator (`obj.operator+(arg)` exact).
     * @details
     * True if an exactly matching member `operator+` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_plus_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member addition, or `scl::nonesuch`.
     * @details
     * Exposes the exact addition return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using plus_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for subtraction expression `std::declval<Left>() - std::declval<Right>()`.
     * @details
     * Represents the unevaluated `-` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using minus_operation = /* implementation defined */;
    /**
     * @brief Detects subtraction operator (`x - y`).
     * @details
     * True if the difference expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_minus_v = /* implementation defined */;
    /**
     * @brief Detected result type of subtraction, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left - Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using minus_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member subtraction `std::declval<Type>().operator-(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator-(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using minus_member_operation = /* implementation defined */;
    /**
     * @brief Detects member subtraction operator (`obj.operator-(arg)`).
     * @details
     * True if `Type::operator-(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_minus_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member subtraction, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator-` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using minus_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member subtraction detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator-`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using minus_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member subtraction operator (`obj.operator-(arg)` exact).
     * @details
     * True if an exactly matching member `operator-` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_minus_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member subtraction, or `scl::nonesuch`.
     * @details
     * Exposes the exact subtraction return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using minus_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for left-shift expression `std::declval<Left>() << std::declval<Right>()`.
     * @details
     * Represents the unevaluated `<<` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using left_shift_operation = /* implementation defined */;
    /**
     * @brief Detects left-shift operator (`x << y`).
     * @details
     * True if the shift expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_left_shift_v = /* implementation defined */;
    /**
     * @brief Detected result type of left-shift, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left << Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using left_shift_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member left-shift `std::declval<Type>().operator<<(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator<<(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using left_shift_member_operation = /* implementation defined */;
    /**
     * @brief Detects member left-shift operator (`obj.operator<<(arg)`).
     * @details
     * True if `Type::operator<<(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_left_shift_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member left-shift, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator<<` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using left_shift_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member left-shift detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator<<`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using left_shift_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member left-shift operator (`obj.operator<<(arg)` exact).
     * @details
     * True if an exactly matching member `operator<<` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_left_shift_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member left-shift, or `scl::nonesuch`.
     * @details
     * Exposes the exact shift return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using left_shift_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for right-shift expression `std::declval<Left>() >> std::declval<Right>()`.
     * @details
     * Represents the unevaluated `>>` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using right_shift_operation = /* implementation defined */;
    /**
     * @brief Detects right-shift operator (`x >> y`).
     * @details
     * True if the shift expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_right_shift_v = /* implementation defined */;
    /**
     * @brief Detected result type of right-shift, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left >> Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using right_shift_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member right-shift `std::declval<Type>().operator>>(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator>>(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using right_shift_member_operation = /* implementation defined */;
    /**
     * @brief Detects member right-shift operator (`obj.operator>>(arg)`).
     * @details
     * True if `Type::operator>>(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_right_shift_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member right-shift, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator>>` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using right_shift_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member right-shift detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator>>`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using right_shift_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member right-shift operator (`obj.operator>>(arg)` exact).
     * @details
     * True if an exactly matching member `operator>>` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_right_shift_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member right-shift, or `scl::nonesuch`.
     * @details
     * Exposes the exact shift return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using right_shift_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for bitwise AND expression `std::declval<Left>() & std::declval<Right>()`.
     * @details
     * Represents the unevaluated `&` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bitwise_and_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise AND operator (`x & y`).
     * @details
     * True if the bitwise AND expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_bitwise_and_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise AND, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left & Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bitwise_and_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise AND `std::declval<Type>().operator&(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator&(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_and_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise AND operator (`obj.operator&(arg)`).
     * @details
     * True if `Type::operator&(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_and_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise AND, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator&` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_and_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise AND detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator&`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_and_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise AND operator (`obj.operator&(arg)` exact).
     * @details
     * True if an exactly matching member `operator&` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_and_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise AND, or `scl::nonesuch`.
     * @details
     * Exposes the exact bitwise AND return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_and_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for bitwise OR expression `std::declval<Left>() | std::declval<Right>()`.
     * @details
     * Represents the unevaluated `|` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bitwise_or_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise OR operator (`x | y`).
     * @details
     * True if the bitwise OR expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_bitwise_or_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise OR, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left | Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bitwise_or_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise OR `std::declval<Type>().operator|(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator|(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_or_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise OR operator (`obj.operator|(arg)`).
     * @details
     * True if `Type::operator|(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_or_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise OR, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator|` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_or_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise OR detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator|`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_or_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise OR operator (`obj.operator|(arg)` exact).
     * @details
     * True if an exactly matching member `operator|` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_or_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise OR, or `scl::nonesuch`.
     * @details
     * Exposes the exact bitwise OR return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_or_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for bitwise XOR expression `std::declval<Left>() ^ std::declval<Right>()`.
     * @details
     * Represents the unevaluated `^` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bitwise_xor_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise XOR operator (`x ^ y`).
     * @details
     * True if the bitwise XOR expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_bitwise_xor_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise XOR, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left ^ Right`, else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using bitwise_xor_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise XOR `std::declval<Type>().operator^(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator^(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_xor_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise XOR operator (`obj.operator^(arg)`).
     * @details
     * True if `Type::operator^(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_xor_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise XOR, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator^` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_xor_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise XOR detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator^`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_xor_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise XOR operator (`obj.operator^(arg)` exact).
     * @details
     * True if an exactly matching member `operator^` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_xor_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise XOR, or `scl::nonesuch`.
     * @details
     * Exposes the exact bitwise XOR return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using bitwise_xor_member_exact_t = /* implementation defined */;

    /** @} */

    // ============================================================================
    // LOGICAL BINARY: &&, ||
    // ============================================================================

    /**
     * @defgroup op_det_logical Logical Operators
     * @brief Detection for operators: `&&`, `||`
     * @{
     */

    /**
     * @brief Alias for logical AND expression `std::declval<Left>() && std::declval<Right>()`.
     * @details
     * Represents the unevaluated `&&` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using logical_and_operation = /* implementation defined */;
    /**
     * @brief Detects logical AND operator (`x && y`).
     * @details
     * True if the logical AND expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_logical_and_v = /* implementation defined */;
    /**
     * @brief Detected result type of logical AND, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left && Right` when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using logical_and_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member logical AND `std::declval<Type>().operator&&(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator&&(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_and_member_operation = /* implementation defined */;
    /**
     * @brief Detects member logical AND operator (`obj.operator&&(arg)`).
     * @details
     * True if `Type::operator&&(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_logical_and_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member logical AND, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator&&` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_and_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member logical AND detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator&&`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_and_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member logical AND operator (`obj.operator&&(arg)` exact).
     * @details
     * True if an exactly matching `operator&&` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_logical_and_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member logical AND, or `scl::nonesuch`.
     * @details
     * Exposes the exact logical AND return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_and_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for logical OR expression `std::declval<Left>() || std::declval<Right>()`.
     * @details
     * Represents the unevaluated `||` expression (built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using logical_or_operation = /* implementation defined */;
    /**
     * @brief Detects logical OR operator (`x || y`).
     * @details
     * True if the logical OR expression is well-formed for the given operands.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_logical_or_v = /* implementation defined */;
    /**
     * @brief Detected result type of logical OR, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left || Right` when available.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using logical_or_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member logical OR `std::declval<Type>().operator||(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator||(Argument)` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_or_member_operation = /* implementation defined */;
    /**
     * @brief Detects member logical OR operator (`obj.operator||(arg)`).
     * @details
     * True if `Type::operator||(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_logical_or_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member logical OR, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator||` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_or_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member logical OR detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator||`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_or_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member logical OR operator (`obj.operator||(arg)` exact).
     * @details
     * True if an exactly matching `operator||` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_logical_or_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member logical OR, or `scl::nonesuch`.
     * @details
     * Exposes the exact logical OR return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using logical_or_member_exact_t = /* implementation defined */;

    /** @} */

    // ============================================================================
    // ASSIGNMENT OPERATORS: =, +=, -=, *=, /=, %=, <<=, >>=, &=, |=, ^=
    // ============================================================================

    /**
     * @defgroup op_det_assign Assignment Operators
     * @brief Detection for operators: `=`, `+=`, `-=`, `*=`, `/=`, `%=`, `<<=`, `>>=`, `&=`, `|=`, `^=`
     * @{
     */

    /**
     * @brief Alias for assignment expression `std::declval<Left>() = std::declval<Right>()`.
     * @details
     * Represents the unevaluated assignment expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using assign_operation = /* implementation defined */;
    /**
     * @brief Detects assignment operator (`x = y`).
     * @details
     * True if the assignment expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of assignment, or `scl::nonesuch`.
     * @details
     * Exposes the type produced by `Left = Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member assignment `std::declval<Type>().operator=(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator=` on `Type` with `Argument` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member assignment operator (`obj.operator=(arg)`).
     * @details
     * True if `Type::operator=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member assignment, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member assignment detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member assignment operator (`obj.operator=(arg)` exact).
     * @details
     * True if an exactly matching `operator=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member assignment, or `scl::nonesuch`.
     * @details
     * Exposes the exact assignment return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for plus-assign expression `std::declval<Left>() += std::declval<Right>()`.
     * @details
     * Represents the unevaluated `+=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using plus_assign_operation = /* implementation defined */;
    /**
     * @brief Detects plus-assign operator (`x += y`).
     * @details
     * True if the compound addition expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_plus_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of plus-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left += Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using plus_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member plus-assign `std::declval<Type>().operator+=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator+=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using plus_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member plus-assign operator (`obj.operator+=(arg)`).
     * @details
     * True if `Type::operator+=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_plus_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member plus-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator+=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using plus_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member plus-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator+=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using plus_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member plus-assign operator (`obj.operator+=(arg)` exact).
     * @details
     * True if an exactly matching `operator+=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_plus_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member plus-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact plus-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using plus_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for minus-assign expression `std::declval<Left>() -= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `-=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using minus_assign_operation = /* implementation defined */;
    /**
     * @brief Detects minus-assign operator (`x -= y`).
     * @details
     * True if the compound subtraction expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_minus_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of minus-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left -= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using minus_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member minus-assign `std::declval<Type>().operator-=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator-=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using minus_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member minus-assign operator (`obj.operator-=(arg)`).
     * @details
     * True if `Type::operator-=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_minus_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member minus-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator-=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using minus_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member minus-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator-=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using minus_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member minus-assign operator (`obj.operator-=(arg)` exact).
     * @details
     * True if an exactly matching `operator-=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_minus_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member minus-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact minus-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using minus_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for multiply-assign expression `std::declval<Left>() *= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `*=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using multiply_assign_operation = /* implementation defined */;
    /**
     * @brief Detects multiply-assign operator (`x *= y`).
     * @details
     * True if the compound multiplication expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_multiply_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of multiply-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left *= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using multiply_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member multiply-assign `std::declval<Type>().operator*=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator*=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using multiply_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member multiply-assign operator (`obj.operator*=(arg)`).
     * @details
     * True if `Type::operator*=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_multiply_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member multiply-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator*=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using multiply_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member multiply-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator*=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using multiply_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member multiply-assign operator (`obj.operator*=(arg)` exact).
     * @details
     * True if an exactly matching `operator*=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_multiply_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member multiply-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact multiply-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using multiply_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for divide-assign expression `std::declval<Left>() /= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `/=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using divide_assign_operation = /* implementation defined */;
    /**
     * @brief Detects divide-assign operator (`x /= y`).
     * @details
     * True if the compound division expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_divide_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of divide-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left /= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using divide_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member divide-assign `std::declval<Type>().operator/=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator/=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using divide_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member divide-assign operator (`obj.operator/=(arg)`).
     * @details
     * True if `Type::operator/=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_divide_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member divide-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator/=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using divide_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member divide-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator/=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using divide_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member divide-assign operator (`obj.operator/=(arg)` exact).
     * @details
     * True if an exactly matching `operator/=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_divide_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member divide-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact divide-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using divide_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for modulo-assign expression `std::declval<Left>() %= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `%=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using modulo_assign_operation = /* implementation defined */;
    /**
     * @brief Detects modulo-assign operator (`x %= y`).
     * @details
     * True if the compound modulo expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_modulo_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of modulo-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left %= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using modulo_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member modulo-assign `std::declval<Type>().operator%=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator%=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using modulo_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member modulo-assign operator (`obj.operator%=(arg)`).
     * @details
     * True if `Type::operator%=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_modulo_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member modulo-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator%=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using modulo_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member modulo-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator%=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using modulo_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member modulo-assign operator (`obj.operator%=(arg)` exact).
     * @details
     * True if an exactly matching `operator%=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_modulo_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member modulo-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact modulo-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using modulo_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for left-shift-assign expression `std::declval<Left>() <<= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `<<=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using left_shift_assign_operation = /* implementation defined */;
    /**
     * @brief Detects left-shift-assign operator (`x <<= y`).
     * @details
     * True if the compound shift expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_left_shift_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of left-shift-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left <<= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using left_shift_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member left-shift-assign `std::declval<Type>().operator<<=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator<<=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using left_shift_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member left-shift-assign operator (`obj.operator<<=(arg)`).
     * @details
     * True if `Type::operator<<=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_left_shift_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member left-shift-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator<<=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using left_shift_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member left-shift-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator<<=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using left_shift_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member left-shift-assign operator (`obj.operator<<=(arg)` exact).
     * @details
     * True if an exactly matching `operator<<=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_left_shift_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member left-shift-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact left-shift-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using left_shift_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for right-shift-assign expression `std::declval<Left>() >>= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `>>=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using right_shift_assign_operation = /* implementation defined */;
    /**
     * @brief Detects right-shift-assign operator (`x >>= y`).
     * @details
     * True if the compound shift expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_right_shift_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of right-shift-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left >>= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using right_shift_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member right-shift-assign `std::declval<Type>().operator>>=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator>>=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using right_shift_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member right-shift-assign operator (`obj.operator>>=(arg)`).
     * @details
     * True if `Type::operator>>=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_right_shift_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member right-shift-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator>>=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using right_shift_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member right-shift-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator>>=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using right_shift_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member right-shift-assign operator (`obj.operator>>=(arg)` exact).
     * @details
     * True if an exactly matching `operator>>=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_right_shift_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member right-shift-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact right-shift-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using right_shift_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for bitwise AND-assign expression `std::declval<Left>() &= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `&=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using bitwise_and_assign_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise AND-assign operator (`x &= y`).
     * @details
     * True if the compound bitwise AND expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_bitwise_and_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise AND-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left &= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using bitwise_and_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise AND-assign `std::declval<Type>().operator&=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator&=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_and_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise AND-assign operator (`obj.operator&=(arg)`).
     * @details
     * True if `Type::operator&=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_and_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise AND-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator&=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_and_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise AND-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator&=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_and_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise AND-assign operator (`obj.operator&=(arg)` exact).
     * @details
     * True if an exactly matching `operator&=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_and_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise AND-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact bitwise AND-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_and_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for bitwise OR-assign expression `std::declval<Left>() |= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `|=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using bitwise_or_assign_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise OR-assign operator (`x |= y`).
     * @details
     * True if the compound bitwise OR expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_bitwise_or_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise OR-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left |= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using bitwise_or_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise OR-assign `std::declval<Type>().operator|=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator|=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_or_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise OR-assign operator (`obj.operator|=(arg)`).
     * @details
     * True if `Type::operator|=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_or_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise OR-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator|=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_or_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise OR-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator|=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_or_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise OR-assign operator (`obj.operator|=(arg)` exact).
     * @details
     * True if an exactly matching `operator|=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_or_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise OR-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact bitwise OR-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_or_assign_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for bitwise XOR-assign expression `std::declval<Left>() ^= std::declval<Right>()`.
     * @details
     * Represents the unevaluated `^=` expression (built-in or overloaded).
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using bitwise_xor_assign_operation = /* implementation defined */;
    /**
     * @brief Detects bitwise XOR-assign operator (`x ^= y`).
     * @details
     * True if the compound bitwise XOR expression is well-formed for the given operands.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    inline constexpr bool has_bitwise_xor_assign_v = /* implementation defined */;
    /**
     * @brief Detected result type of bitwise XOR-assign, or `scl::nonesuch`.
     * @details
     * Exposes the type returned by `Left ^= Right` (typically `Left&`) when available.
     * @tparam Left Left operand type (LHS).
     * @tparam Right Right operand type (RHS).
     */
    template <typename Left, typename Right>
    using bitwise_xor_assign_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member bitwise XOR-assign `std::declval<Type>().operator^=(std::declval<Argument>())`.
     * @details
     * Probes for `Type::operator^=(Argument)` (implicit conversions allowed).
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_xor_assign_member_operation = /* implementation defined */;
    /**
     * @brief Detects member bitwise XOR-assign operator (`obj.operator^=(arg)`).
     * @details
     * True if `Type::operator^=(Argument)` is callable.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_xor_assign_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member bitwise XOR-assign, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator^=` if present.
     * @tparam Type The class type (LHS).
     * @tparam Argument Right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_xor_assign_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member bitwise XOR-assign detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator^=`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_xor_assign_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member bitwise XOR-assign operator (`obj.operator^=(arg)` exact).
     * @details
     * True if an exactly matching `operator^=` exists on `Type`.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_bitwise_xor_assign_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member bitwise XOR-assign, or `scl::nonesuch`.
     * @details
     * Exposes the exact bitwise XOR-assign return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type (LHS).
     * @tparam Argument Exact right-hand argument type (RHS).
     */
    template <typename Type, typename Argument>
    using bitwise_xor_assign_member_exact_t = /* implementation defined */;

    /** @} */

    // ============================================================================
    // SPECIAL OPERATORS: ->*, ->, ,, [], ()
    // ============================================================================

    /**
     * @defgroup op_det_special Special Operators
     * @brief Detection for operators: `->*`, `->`, `,`, `[]`, `()`
     * @{
     */

    /**
     * @brief Alias for member pointer access `std::declval<Type>().operator->*(std::declval<Arguments>()...)`.
     * @details
     * Probes member pointer access (`->*`) as a class member function; detects appropriate overloads.
     * @tparam Type The object type.
     * @tparam Arguments Pointer-to-member and additional arguments (if any).
     */
    template <typename Type, typename... Arguments>
    using arrow_to_pointer_member_operation = /* implementation defined */;
    /**
     * @brief Detects member pointer access operator (`obj.operator->*(pm)`).
     * @details
     * True if the member pointer access expression is well-formed for the given arguments.
     * @tparam Arguments First type is the class type, followed by pointer-to-member type.
     */
    template <typename... Arguments>
    inline constexpr bool has_arrow_to_pointer_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member pointer access, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `operator->*` when implemented as a member.
     * @tparam Arguments First type is the class type, followed by pointer-to-member type.
     */
    template <typename... Arguments>
    using arrow_to_pointer_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member pointer access detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator->*`.
     * @tparam Type The object type.
     * @tparam Arguments Pointer-to-member type.
     */
    template <typename Type, typename... Arguments>
    using arrow_to_pointer_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member pointer access operator (`obj.operator->*(pm)` exact).
     * @details
     * True if an exactly matching `operator->*` exists on `Type`.
     * @tparam Arguments First type is the class type, followed by pointer-to-member type.
     */
    template <typename... Arguments>
    inline constexpr bool has_arrow_to_pointer_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member pointer access, or `scl::nonesuch`.
     * @details
     * Exposes the exact `operator->*` return type; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type, followed by pointer-to-member type.
     */
    template <typename... Arguments>
    using arrow_to_pointer_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for arrow operator `std::declval<Type>().operator->()`.
     * @details
     * Probes arrow (`->`) as a class member function; detects appropriate overloads.
     * @tparam Type The object type.
     * @tparam Arguments The operator argument types (none).
     */
    template <typename Type, typename... Arguments>
    using arrow_member_operation = /* implementation defined */;
    /**
     * @brief Detects member arrow operator (`obj.operator->()`).
     * @details
     * True if `Type::operator->()` is callable.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_arrow_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member arrow operator, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `operator->` when implemented as a member.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using arrow_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member arrow detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator->`.
     * @tparam Type The object type.
     */
    template <typename Type, typename... Arguments>
    using arrow_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member arrow operator (`obj.operator->()` exact).
     * @details
     * True if an exactly matching `operator->` exists on `Type`.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    inline constexpr bool has_arrow_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member arrow operator, or `scl::nonesuch`.
     * @details
     * Exposes the exact `operator->` return type; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type.
     */
    template <typename... Arguments>
    using arrow_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for comma expression `std::declval<Left>(), std::declval<Right>()`.
     * @details
     * Represents the unevaluated `,` expression (built-in or overloaded), capturing the result type of the RHS.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using comma_operation = /* implementation defined */;
    /**
     * @brief Detects comma operator (`x, y`).
     * @details
     * True if the comma expression is well-formed for the given operands (either built-in or overloaded).
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    inline constexpr bool has_comma_v = /* implementation defined */;
    /**
     * @brief Detected result type of comma operator, or `scl::nonesuch`.
     * @details
     * Exposes the type of the comma expression (typically the RHS type), else `scl::nonesuch`.
     * @tparam Left Left operand type.
     * @tparam Right Right operand type.
     */
    template <typename Left, typename Right>
    using comma_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member comma `std::declval<Type>().operator,(std::declval<Argument>())`.
     * @details
     * Probes for a member `operator,` on `Type` (implicit conversions allowed).
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using comma_member_operation = /* implementation defined */;
    /**
     * @brief Detects member comma operator (`obj.operator,(arg)`).
     * @details
     * True if `Type::operator,(Argument)` is callable.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_comma_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member comma operator, or `scl::nonesuch`.
     * @details
     * Exposes the return type of member `operator,` if present.
     * @tparam Type The class type.
     * @tparam Argument Right-hand argument type.
     */
    template <typename Type, typename Argument>
    using comma_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member comma detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator,`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using comma_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member comma operator (`obj.operator,(arg)` exact).
     * @details
     * True if an exactly matching `operator,` exists on `Type`.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_comma_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member comma operator, or `scl::nonesuch`.
     * @details
     * Exposes the exact comma operator return type; `scl::nonesuch` otherwise.
     * @tparam Type The class type.
     * @tparam Argument Exact right-hand argument type.
     */
    template <typename Type, typename Argument>
    using comma_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for subscript expression `std::declval<Type>()[std::declval<Argument>()]`.
     * @details
     * Represents the unevaluated `[]` expression; detects array-like or custom indexers.
     * @tparam Type The container or operand type.
     * @tparam Argument The index type.
     */
    template <typename Type, typename Argument>
    using subscript_operation = /* implementation defined */;
    /**
     * @brief Detects subscript operator (`obj[index]`).
     * @details
     * True if the subscript expression is well-formed for the given `Type` and `Argument`.
     * @tparam Type The container or operand type.
     * @tparam Argument The index type.
     */
    template <typename Type, typename Argument>
    inline constexpr bool has_subscript_v = /* implementation defined */;
    /**
     * @brief Detected result type of subscript operator, or `scl::nonesuch`.
     * @details
     * Exposes the element access result type; `scl::nonesuch` if unavailable.
     * @tparam Type The container or operand type.
     * @tparam Argument The index type.
     */
    template <typename Type, typename Argument>
    using subscript_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member subscript `std::declval<Type>().operator[](std::declval<Arguments>()...)`.
     * @details
     * Probes for `Type::operator[]` with arguments convertible from `Arguments...`.
     * @tparam Type The class type.
     * @tparam Arguments Exact index argument types.
     */
    template <typename Type, typename... Arguments>
    using subscript_member_operation = /* implementation defined */;
    /**
     * @brief Detects member subscript operator (`obj.operator[](index)`).
     * @details
     * True if `Type::operator[](Arguments...)` is callable.
     * @tparam Arguments First type is the class type, followed by index types.
     */
    template <typename... Arguments>
    inline constexpr bool has_subscript_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member subscript, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator[]` if present.
     * @tparam Arguments First type is the class type, followed by index types.
     */
    template <typename... Arguments>
    using subscript_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member subscript detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator[]`.
     * @tparam Type The class type.
     * @tparam Arguments Exact index argument types.
     */
    template <typename Type, typename... Arguments>
    using subscript_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member subscript operator (`obj.operator[](index)` exact).
     * @details
     * True if an exactly matching `operator[]` exists on `Type`.
     * @tparam Arguments First type is the class type, followed by exact index types.
     */
    template <typename... Arguments>
    inline constexpr bool has_subscript_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member subscript, or `scl::nonesuch`.
     * @details
     * Exposes the exact subscript return type; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type, followed by exact index types.
     */
    template <typename... Arguments>
    using subscript_member_exact_t = /* implementation defined */;

    /**
     * @brief Alias for function-call expression `std::declval<Type>()(std::declval<Arguments>()...)`.
     * @details
     * Represents the unevaluated `()` expression; detects callability of functors or call operators.
     * @tparam Type The callable or functor type.
     * @tparam Arguments The call argument types.
     */
    template <typename Type, typename... Arguments>
    using call_operation = /* implementation defined */;
    /**
     * @brief Detects function call operator (`obj(args...)`).
     * @details
     * True if the call expression is well-formed for the given `Type` and `Arguments...`.
     * @tparam Type The callable or functor type.
     * @tparam Arguments The call argument types.
     */
    template <typename Type, typename... Arguments>
    inline constexpr bool has_call_v = /* implementation defined */;
    /**
     * @brief Detected result type of function call, or `scl::nonesuch`.
     * @details
     * Exposes the result type of `Type(Arguments...)`, else `scl::nonesuch`.
     * @tparam Type The callable or functor type.
     * @tparam Arguments The call argument types.
     */
    template <typename Type, typename... Arguments>
    using call_t = /* implementation defined */;
    /**
     * @brief Alias for invoking member function call `std::declval<Type>().operator()(std::declval<Arguments>()...)`.
     * @details
     * Probes for a member `operator()` with arguments convertible from `Arguments...`.
     * @tparam Type The class type.
     * @tparam Arguments Exact call argument types.
     */
    template <typename Type, typename... Arguments>
    using call_member_operation = /* implementation defined */;
    /**
     * @brief Detects member function call operator (`obj.operator()(args...)`).
     * @details
     * True if `Type::operator()(Arguments...)` is callable.
     * @tparam Arguments First type is the class type, followed by exact call argument types.
     */
    template <typename... Arguments>
    inline constexpr bool has_call_member_v = /* implementation defined */;
    /**
     * @brief Detected result type of member function call, or `scl::nonesuch`.
     * @details
     * Exposes the return type of `Type::operator()` if present.
     * @tparam Arguments First type is the class type, followed by exact call argument types.
     */
    template <typename... Arguments>
    using call_member_t = /* implementation defined */;
    /**
     * @brief Alias for strict member function call detection.
     * @details
     * Enforces exact signature and cv/ref-qualifiers for a true member `operator()`.
     * @tparam Type The class type.
     * @tparam Arguments Exact call argument types.
     */
    template <typename Type, typename... Arguments>
    using call_member_exact_operation = /* implementation defined */;
    /**
     * @brief Detects strict member function call operator (`obj.operator()(args...)` exact).
     * @details
     * True if an exactly matching `operator()` exists on `Type`.
     * @tparam Arguments First type is the class type, followed by exact call argument types.
     */
    template <typename... Arguments>
    inline constexpr bool has_call_member_exact_v = /* implementation defined */;
    /**
     * @brief Detected result type of strict member function call, or `scl::nonesuch`.
     * @details
     * Exposes the exact call operator return type; `scl::nonesuch` otherwise.
     * @tparam Arguments First type is the class type, followed by exact call argument types.
     */
    template <typename... Arguments>
    using call_member_exact_t = /* implementation defined */;

    /** @} */

    /** @} */ // end of scl_utility_operator_detection

} // namespace scl

#endif // DOXYGEN
