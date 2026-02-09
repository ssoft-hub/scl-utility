/**
 * @file
 * @ingroup scl_utility_type_traits
 * @brief Provides macros for generating member function detection traits.
 */

#pragma once

#include <scl/utility/type_traits/detection.h>
#include <scl/utility/type_traits/member_like.h>

/**
 * @ingroup scl_utility_type_traits
 * @brief Generates a suite of traits to detect the existence of a specific member method.
 * 
 * For a given name `foo`, this macro generates:
 * 
 * 1. **Basic Detection** (Checks if `val.foo(args...)` is valid):
 *    - `foo_method_operation<Type, Args...>`: The detection alias.
 *    - `has_foo_method_v<Type, Args...>`: Boolean constant.
 *    - `foo_method_t<Type, Args...>`: Resulting return type (or nonesuch).
 * 
 * 2. **Exact Member Detection** (Checks `&Type::foo` existence and ref-qualifiers):
 *    - `foo_method_exact_operation<Type, Args...>`: The exact detection alias.
 *    - `has_foo_method_exact_v<Type, Args...>`: Boolean constant.
 *    - `foo_method_exact_t<Type, Args...>`: Resulting return type.
 * 
 * 3. **Basic Static Detection** (Checks if `Type::foo(args...)` is valid):
 *    - `foo_static_method_operation<Type, Args...>`: The detection alias.
 *    - `has_foo_static_method_v<Type, Args...>`: Boolean constant.
 *    - `foo_static_method_t<Type, Args...>`: Resulting return type (or nonesuch).
 * 
 * 4. **Exact Static Member Detection** (Checks `&Type::foo` existence):
 *    - `foo_static_method_exact_operation<Type, Args...>`: The exact detection alias.
 *    - `has_foo_static_method_exact_v<Type, Args...>`: Boolean constant.
 *    - `foo_static_method_exact_t<Type, Args...>`: Resulting return type.

 * @param name The name of the member function to detect.
 */
#define SCL_METHOD_DETECTION(name)                                                                          \
    template <typename Type, typename... Arguments>                                                         \
    using name##_method_operation = decltype(::std::declval<Type>().name(::std::declval<Arguments>()...));  \
    template <typename... Arguments>                                                                        \
    inline static constexpr auto has_##name##_method_v =                                                    \
        ::scl::is_detected_v<name##_method_operation, Arguments...>;                                        \
    template <typename... Arguments>                                                                        \
    using name##_method_t = ::scl::detected_t<name##_method_operation, Arguments...>;                       \
                                                                                                            \
    template <typename Type, typename... Arguments>                                                         \
    using name##_method_exact_operation = decltype((::std::declval<Type>().*static_cast<                    \
            ::scl::member_function_like_t<Type, name##_method_operation<Type, Arguments...>(Arguments...)>> \
            (&::std::remove_cvref_t<Type>::name))(::std::declval<Arguments>()...));                         \
    template <typename... Arguments>                                                                        \
    inline static constexpr auto has_##name##_method_exact_v =                                              \
        ::scl::is_detected_v<name##_method_exact_operation, Arguments...>;                                  \
    template <typename... Arguments>                                                                        \
    using name##_method_exact_t = ::scl::detected_t<name##_method_exact_operation, Arguments...>;           \
                                                                                                            \
    template <typename Type, typename... Arguments>                                                         \
    using name##_static_method_operation =                                                                  \
        decltype(::std::remove_cvref_t<Type>::name(::std::declval<Arguments>()...));                        \
    template <typename... Arguments>                                                                        \
    inline static constexpr auto has_##name##_static_method_v =                                             \
        ::scl::is_detected_v<name##_static_method_operation, Arguments...>;                                 \
    template <typename... Arguments>                                                                        \
    using name##_static_method_t = ::scl::detected_t<name##_static_method_operation, Arguments...>;         \
                                                                                                            \
    template <typename Type, typename... Arguments>                                                         \
    using name##_static_method_exact_operation = decltype((static_cast<                                     \
        name##_static_method_operation<Type, Arguments...> (*)(Arguments...)>                               \
        (&::std::remove_cvref_t<Type>::name))(::std::declval<Arguments>()...));                             \
    template <typename... Arguments>                                                                        \
    inline static constexpr auto has_##name##_static_method_exact_v =                                       \
        ::scl::is_detected_v<name##_static_method_exact_operation, Arguments...>;\
    template <typename... Arguments>                                                                        \
    using name##_static_method_exact_t = ::scl::detected_t<name##_static_method_exact_operation, Arguments...>;
