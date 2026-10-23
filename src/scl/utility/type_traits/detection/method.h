/**
 * @file
 * @brief A macro declaring the templates that check a call of a member function of a given name.
 * @ingroup scl_utility_type_traits
 */

#pragma once

#include <scl/utility/type_traits/detection.h>
#include <scl/utility/type_traits/member_like.h>

/**
 * @def SCL_METHOD_DETECTION(name)
 * @ingroup scl_utility_type_traits
 * @brief The templates checking whether the member function @p name can be called with arguments.
 *
 * The macro may be expanded in a namespace or in a class. The object type `Type`, with its
 * cv-qualifiers and reference, is passed to every template of the expansion as the first argument,
 * followed by the types of the call arguments, for example `has_add_method_v<counter &, int>`.
 * `SCL_METHOD_DETECTION(add)` expands to:
 *
 * @code
 * template <typename Type, typename... Arguments>
 * using add_method_operation =
 *     decltype(std::declval<Type>().add(std::declval<Arguments>()...));
 * template <typename... Arguments>
 * inline static constexpr auto has_add_method_v =
 *     scl::is_detected_v<add_method_operation, Arguments...>;
 * template <typename... Arguments>
 * using add_method_t = scl::detected_t<add_method_operation, Arguments...>;
 *
 * template <typename Type, typename... Arguments>
 * using add_method_exact_operation = decltype((std::declval<Type>().*
 *     static_cast<scl::member_function_like_t<Type,
 *         add_method_operation<Type, Arguments...>(Arguments...)>>(
 *         &std::remove_cvref_t<Type>::add))(std::declval<Arguments>()...));
 * template <typename... Arguments>
 * inline static constexpr auto has_add_method_exact_v =
 *     scl::is_detected_v<add_method_exact_operation, Arguments...>;
 * template <typename... Arguments>
 * using add_method_exact_t = scl::detected_t<add_method_exact_operation, Arguments...>;
 *
 * template <typename Type, typename... Arguments>
 * using add_static_method_operation =
 *     decltype(std::remove_cvref_t<Type>::add(std::declval<Arguments>()...));
 * template <typename... Arguments>
 * inline static constexpr auto has_add_static_method_v =
 *     scl::is_detected_v<add_static_method_operation, Arguments...>;
 * template <typename... Arguments>
 * using add_static_method_t = scl::detected_t<add_static_method_operation, Arguments...>;
 *
 * template <typename Type, typename... Arguments>
 * using add_static_method_exact_operation = decltype((static_cast<
 *     add_static_method_operation<Type, Arguments...> (*)(Arguments...)>(
 *         &std::remove_cvref_t<Type>::add))(std::declval<Arguments>()...));
 * template <typename... Arguments>
 * inline static constexpr auto has_add_static_method_exact_v =
 *     scl::is_detected_v<add_static_method_exact_operation, Arguments...>;
 * template <typename... Arguments>
 * using add_static_method_exact_t =
 *     scl::detected_t<add_static_method_exact_operation, Arguments...>;
 * @endcode
 *
 * The object is passed to the call as `std::declval<Type>()`, an lvalue for a `Type` of the form
 * `X &` and an rvalue for `X` and `X &&`. The variable template `has_name_method_v` is `true` if
 * the call compiles. The arguments may convert implicitly, and a `const` member function can be
 * called on an object without `const` as well.
 *
 * The variable template `has_name_method_exact_v` is `true` if a member function @p name is
 * declared in the class `std::remove_cvref_t<Type>` or in a public base class of it with parameters
 * of exactly the types `Arguments...` and with the qualifiers matching `Type`, and the plain call
 * compiles. Matching means that the member function has
 * the cv-qualifiers of `Type` and the ref-qualifier `&` for `X &`, `&&` for `X &&` and none for
 * `X`. The top-level `const` and `volatile` of the types `Arguments...` are not compared. The alias
 * template `name_method_exact_t` is the result type of the plain call, so for an ambiguous plain
 * call the variable template is `false`. On MSVC the access is not checked in the call of
 * a non-static member function, so there the variable template `has_name_method_v` is `true` also
 * for a private or a protected member function. The specifier `noexcept` is not compared, and it
 * can be checked with the operator `noexcept` applied to a call.
 *
 * The variable template `has_name_static_method_v` is `true` if the call
 * `std::remove_cvref_t<Type>::name(std::declval<Arguments>()...)` compiles. The variable template
 * `has_name_static_method_exact_v` is `true` if a static member function @p name with parameters
 * of exactly the types `Arguments...` is declared and that call compiles. On MSVC the non-strict
 * static form finds a private or a protected static member function only where the class declares
 * other overloads of @p name.
 * For a class with no member @p name every variable template is `false`.
 *
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp declare
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp call
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp exact
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp properties
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp static
 *
 * @param name Name of the member function, an identifier.
 */
#define SCL_METHOD_DETECTION(name)                                                                                   \
    template <typename Type, typename... Arguments>                                                                  \
    using name##_method_operation = decltype(::std::declval<Type>().name(::std::declval<Arguments>()...));           \
    template <typename... Arguments>                                                                                 \
    inline static constexpr auto has_##name##_method_v =                                                             \
        ::scl::is_detected_v<name##_method_operation, Arguments...>;                                                 \
    template <typename... Arguments>                                                                                 \
    using name##_method_t = ::scl::detected_t<name##_method_operation, Arguments...>;                                \
                                                                                                                     \
    template <typename Type, typename... Arguments>                                                                  \
    using name##_method_exact_operation = decltype((                                                                 \
        ::std::declval<Type>().*                                                                                     \
        static_cast<::scl::member_function_like_t<Type, name##_method_operation<Type, Arguments...>(Arguments...)>>( \
            &::std::remove_cvref_t<Type>::name))(::std::declval<Arguments>()...));                                   \
    template <typename... Arguments>                                                                                 \
    inline static constexpr auto has_##name##_method_exact_v =                                                       \
        ::scl::is_detected_v<name##_method_exact_operation, Arguments...>;                                           \
    template <typename... Arguments>                                                                                 \
    using name##_method_exact_t = ::scl::detected_t<name##_method_exact_operation, Arguments...>;                    \
                                                                                                                     \
    template <typename Type, typename... Arguments>                                                                  \
    using name##_static_method_operation = decltype(::std::remove_cvref_t<Type>::name(                               \
        ::std::declval<Arguments>()...));                                                                            \
    template <typename... Arguments>                                                                                 \
    inline static constexpr auto has_##name##_static_method_v =                                                      \
        ::scl::is_detected_v<name##_static_method_operation, Arguments...>;                                          \
    template <typename... Arguments>                                                                                 \
    using name##_static_method_t = ::scl::detected_t<name##_static_method_operation, Arguments...>;                  \
                                                                                                                     \
    template <typename Type, typename... Arguments>                                                                  \
    using name##_static_method_exact_operation = decltype((static_cast<                                              \
        name##_static_method_operation<Type, Arguments...> (*)(Arguments...)>(                                       \
        &::std::remove_cvref_t<Type>::name))(::std::declval<Arguments>()...));                                       \
    template <typename... Arguments>                                                                                 \
    inline static constexpr auto has_##name##_static_method_exact_v =                                                \
        ::scl::is_detected_v<name##_static_method_exact_operation, Arguments...>;                                    \
    template <typename... Arguments>                                                                                 \
    using name##_static_method_exact_t =                                                                             \
        ::scl::detected_t<name##_static_method_exact_operation, Arguments...>;
