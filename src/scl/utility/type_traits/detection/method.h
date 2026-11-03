/**
 * @file
 * @brief A macro declaring the templates that check a call of a member function of a given name.
 * @ingroup scl_utility_type_traits
 */

#pragma once

#include <scl/utility/type_traits/detail/exact.h>
#include <scl/utility/type_traits/detection.h>

#include <type_traits>
#include <utility>

/**
 * @def SCL_METHOD_DETECTION(name)
 * @ingroup scl_utility_type_traits
 * @brief The templates checking whether the member function @p name can be called with arguments.
 *
 * The macro should be expanded in a separate namespace or in a class, so that the names of
 * the declared templates do not collide with other declarations. The object type `Type`, with its
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
 * using add_method_exact_operation = unspecified;
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
 * using add_static_method_exact_operation = unspecified;
 * template <typename... Arguments>
 * inline static constexpr auto has_add_static_method_exact_v =
 *     scl::is_detected_v<add_static_method_exact_operation, Arguments...>;
 * template <typename... Arguments>
 * using add_static_method_exact_t =
 *     scl::detected_t<add_static_method_exact_operation, Arguments...>;
 *
 * template <typename Type, typename... Arguments>
 * using add_explicit_method_exact_operation = unspecified;
 * template <typename... Arguments>
 * inline static constexpr auto has_add_explicit_method_exact_v =
 *     scl::is_detected_v<add_explicit_method_exact_operation, Arguments...>;
 * template <typename... Arguments>
 * using add_explicit_method_exact_t =
 *     scl::detected_t<add_explicit_method_exact_operation, Arguments...>;
 * @endcode
 *
 * The object is passed to the call as `std::declval<Type>()`, an lvalue for a `Type` of the form
 * `X &` and an rvalue for `X` and `X &&`. The variable template `has_name_method_v` is `true` if
 * the call compiles. The arguments may convert implicitly, and a `const` member function can be
 * called on an object without `const` as well.
 *
 * The variable template `has_name_method_exact_v` is `true` if a member function @p name is
 * declared in the class `std::remove_cvref_t<Type>` or in a public base class of it with parameters
 * of exactly the types `Arguments...` and with the qualifiers matching `Type`, and the member
 * function is neither deleted nor inaccessible. Matching means that the member function has
 * the cv-qualifiers of `Type` and the ref-qualifier `&` for `X &`, `&&` for `X &&` and none for
 * `X`. The top-level `const` and `volatile` of the types `Arguments...` are not compared. The alias
 * template `name_method_exact_t` is the result type of the member function found. The member
 * function is found even where the plain call is ambiguous. Where the overloads of @p name include
 * a member function template, only the overload whose result type is that of the plain call is
 * found, so for an ambiguous plain call the variable template is `false`. On MSVC the address
 * of a deleted member function is not rejected during substitution, so the plain call has to
 * compile as well. The access is not checked there in the call of a non-static member function,
 * so the variable template `has_name_method_v` is `true` also for a private or a protected member
 * function, and the exact form finds such a member function only where the class declares other
 * overloads of @p name.
 * The specifier `noexcept` is not compared, and it can be checked with the operator `noexcept`
 * applied to a call. Where the plain call is ambiguous, it shows in the type of the pointer
 * `scl::member_overload_cast<Type, Arguments...>(&X::name)`. This form finds only a member function
 * with an implicit object parameter.
 *
 * The variable template `has_name_static_method_v` is `true` if the call
 * `std::remove_cvref_t<Type>::name(std::declval<Arguments>()...)` compiles. The variable template
 * `has_name_static_method_exact_v` is `true` if a static member function @p name with parameters
 * of exactly the types `Arguments...` is declared, even where that call is ambiguous. The address
 * of a member function with an explicit object parameter has the type of the address of a static
 * member function, so the static exact form is `true` for it as well, where its object parameter
 * is the first of the types `Arguments...`. On MSVC the static exact form requires that call to
 * compile, so there it is `false` for a member function with an explicit object parameter, and both
 * static forms find a private or a protected static member function only where the class declares
 * other overloads of @p name.
 *
 * The variable template `has_name_explicit_method_exact_v` is `true` if a member function @p name
 * with an explicit object parameter of exactly the type `Type` and with parameters of exactly
 * the types `Arguments...` is declared, and is neither deleted nor inaccessible. Such a member
 * function declared in a base class is found only where its object parameter is of the type `Type`,
 * as for a template `this Self && self`. A static member function @p name taking the object first
 * differs from such a member function only in that it can be called as
 * `std::remove_cvref_t<Type>::name(object, arguments...)`. Where that call compiles, the variable
 * template is `false`, so a member function with an explicit object parameter is not found beside
 * such a static one. Where that call is ambiguous, for example for two static overloads taking
 * the object first, the variable template is `true` even though no member function with an explicit
 * object parameter is declared. On MSVC the plain call has to compile in this form as well, so
 * there the variable template is `false` in that case. For a class with no member @p name every
 * variable template is `false`.
 *
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp declare
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp call
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp exact
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp properties
 * @snippet type_traits/method_detection/type_traits_method_detection_example.cpp static
 *
 * @param name Name of the member function, an identifier.
 */
#define SCL_METHOD_DETECTION(name)                                                                         \
    template <typename Type, typename... Arguments>                                                        \
    using name##_method_operation = decltype(::std::declval<Type>().name(::std::declval<Arguments>()...)); \
    template <typename... Arguments>                                                                       \
    inline static constexpr auto has_##name##_method_v =                                                   \
        ::scl::is_detected_v<name##_method_operation, Arguments...>;                                       \
    template <typename... Arguments>                                                                       \
    using name##_method_t = ::scl::detected_t<name##_method_operation, Arguments...>;                      \
                                                                                                           \
    template <typename Type, typename... Arguments>                                                        \
    using name##_method_exact_operation = decltype((                                                       \
        ::std::declval<Type>().*                                                                           \
        ::scl::detail::exact_member<                                                                       \
            Type, ::scl::detected_t<name##_method_operation, Type, Arguments...>,                          \
            Arguments...>::select(&::std::remove_cvref_t<Type>::name))(::std::declval<Arguments>()...));   \
    template <typename... Arguments>                                                                       \
    inline static constexpr auto has_##name##_method_exact_v =                                             \
        ::scl::is_detected_v<name##_method_exact_operation, Arguments...>;                                 \
    template <typename... Arguments>                                                                       \
    using name##_method_exact_t = ::scl::detected_t<name##_method_exact_operation, Arguments...>;          \
                                                                                                           \
    template <typename Type, typename... Arguments>                                                        \
    using name##_static_method_operation = decltype(::std::remove_cvref_t<Type>::name(                     \
        ::std::declval<Arguments>()...));                                                                  \
    template <typename... Arguments>                                                                       \
    inline static constexpr auto has_##name##_static_method_v =                                            \
        ::scl::is_detected_v<name##_static_method_operation, Arguments...>;                                \
    template <typename... Arguments>                                                                       \
    using name##_static_method_t = ::scl::detected_t<name##_static_method_operation, Arguments...>;        \
                                                                                                           \
    template <typename Type, typename... Arguments>                                                        \
    using name##_static_method_exact_operation = decltype((::scl::detail::exact_function<                  \
        ::scl::detected_t<name##_static_method_operation, Type, Arguments...>,                             \
        Arguments...>::select(&::std::remove_cvref_t<Type>::name))(::std::declval<Arguments>()...));       \
    template <typename... Arguments>                                                                       \
    inline static constexpr auto has_##name##_static_method_exact_v =                                      \
        ::scl::is_detected_v<name##_static_method_exact_operation, Arguments...>;                          \
    template <typename... Arguments>                                                                       \
    using name##_static_method_exact_t =                                                                   \
        ::scl::detected_t<name##_static_method_exact_operation, Arguments...>;                             \
                                                                                                           \
    template <typename Type, typename... Arguments>                                                        \
    using name##_explicit_method_exact_operation = ::std::enable_if_t<                                     \
        !::scl::is_detected_v<name##_static_method_operation, Type, Type, Arguments...>,                   \
        decltype((::scl::detail::exact_function<                                                           \
            ::scl::detected_t<name##_method_operation, Type, Arguments...>, Type,                          \
            Arguments...>::select(&::std::remove_cvref_t<Type>::name))(                                    \
            ::std::declval<Type>(), ::std::declval<Arguments>()...))>;                                     \
    template <typename... Arguments>                                                                       \
    inline static constexpr auto has_##name##_explicit_method_exact_v =                                    \
        ::scl::is_detected_v<name##_explicit_method_exact_operation, Arguments...>;                        \
    template <typename... Arguments>                                                                       \
    using name##_explicit_method_exact_t =                                                                 \
        ::scl::detected_t<name##_explicit_method_exact_operation, Arguments...>;
