/**
 * @file
 * @brief A macro declaring the templates that check a call of a function of a given name.
 * @ingroup scl_utility_type_traits
 */

#pragma once

#include <scl/utility/type_traits/detail/exact.h>
#include <scl/utility/type_traits/detection.h>

#include <utility>

/**
 * @def SCL_FUNCTION_DETECTION(name)
 * @ingroup scl_utility_type_traits
 * @brief The templates checking whether the function @p name can be called with given arguments.
 *
 * The macro should be expanded after the declarations of the function to check, in a separate
 * namespace or in a class, so that the names of the declared templates do not collide with other
 * declarations. `SCL_FUNCTION_DETECTION(foo)` expands to:
 *
 * @code
 * template <typename... Arguments>
 * using foo_function_operation = decltype(foo(std::declval<Arguments>()...));
 * template <typename... Arguments>
 * inline static constexpr auto has_foo_function_v =
 *     scl::is_detected_v<foo_function_operation, Arguments...>;
 * template <typename... Arguments>
 * using foo_function_t = scl::detected_t<foo_function_operation, Arguments...>;
 *
 * template <typename... Arguments>
 * using foo_function_exact_operation = unspecified;
 * template <typename... Arguments>
 * inline static constexpr auto has_foo_function_exact_v =
 *     scl::is_detected_v<foo_function_exact_operation, Arguments...>;
 * template <typename... Arguments>
 * using foo_function_exact_t = scl::detected_t<foo_function_exact_operation, Arguments...>;
 * @endcode
 *
 * The variable template `has_name_function_v<Arguments...>` is `true` where the call
 * `name(std::declval<Arguments>()...)` compiles. The arguments may convert implicitly.
 * The function for the call is looked up where the macro is expanded and in the namespaces and
 * the classes of the arguments. The variable template is `true` also where @p name is a pointer to
 * a function or a function object whose call with these arguments compiles.
 *
 * The variable template `has_name_function_exact_v<Arguments...>` is `true` if a function @p name
 * whose parameters are exactly the types `Arguments...` is declared before the macro is expanded,
 * and the function is not deleted. The top-level `const` and `volatile` of the types
 * `Arguments...` are not compared, since they are not part of a function type. The alias template
 * `name_function_exact_t` is the result type of the function found. The function is found even
 * where the plain call is ambiguous, as for the overloads `f(int)` and `f(int &&)`. Where
 * the overloads of @p name include a function template, only the overload whose result type is
 * that of the plain call is found, so for an ambiguous plain call the variable template is
 * `false`. On MSVC, where the address of a deleted function is not rejected during substitution,
 * the plain call has to compile as well. Where the macro is expanded in a class, a static member
 * function of a base class that the class cannot access is not supported.
 *
 * The exact form is built on the expression `&name`, and the name in it is looked up only where
 * the macro is expanded, without argument-dependent lookup. A friend function defined inside
 * a class is visible only to argument-dependent lookup, so the variable template is `false` for it,
 * and for a pointer to a function or a function object as well. The specifier `noexcept` is not
 * compared, and it can be checked with the operator `noexcept` applied to a call. Where the plain
 * call is ambiguous, it shows in the type of the pointer `scl::overload_cast<Arguments...>(&name)`.
 * The build ends with an error where no declaration of @p name is visible at the place
 * of expansion.
 *
 * @snippet type_traits/function_detection/type_traits_function_detection_example.cpp declare
 * @snippet type_traits/function_detection/type_traits_function_detection_example.cpp call
 * @snippet type_traits/function_detection/type_traits_function_detection_example.cpp exact
 * @snippet type_traits/function_detection/type_traits_function_detection_example.cpp properties
 * @snippet type_traits/function_detection/type_traits_function_detection_example.cpp adl
 *
 * @param name Name of the function, an identifier.
 */
#define SCL_FUNCTION_DETECTION(name)                                                      \
    template <typename... Arguments>                                                      \
    using name##_function_operation = decltype(name(::std::declval<Arguments>()...));     \
    template <typename... Arguments>                                                      \
    inline static constexpr auto has_##name##_function_v =                                \
        ::scl::is_detected_v<name##_function_operation, Arguments...>;                    \
    template <typename... Arguments>                                                      \
    using name##_function_t = ::scl::detected_t<name##_function_operation, Arguments...>; \
                                                                                          \
    template <typename... Arguments>                                                      \
    using name##_function_exact_operation = decltype(::scl::detail::exact_function<       \
        ::scl::detected_t<name##_function_operation, Arguments...>,                       \
        Arguments...>::select(&name)(::std::declval<Arguments>()...));                    \
    template <typename... Arguments>                                                      \
    inline static constexpr auto has_##name##_function_exact_v =                          \
        ::scl::is_detected_v<name##_function_exact_operation, Arguments...>;              \
    template <typename... Arguments>                                                      \
    using name##_function_exact_t = ::scl::detected_t<name##_function_exact_operation, Arguments...>;
