/**
 * @ingroup scl_utility_type_traits
 * @file function_detection.hpp
 * @brief Provides macros for generating global/free function detection traits.
 */

#pragma once

#include <scl/utility/type_traits/detection.h>

/**
 * @ingroup scl_utility_type_traits
 * @brief Generates a suite of traits to detect the existence of a specific free function.
 * 
 * For a given name `foo`, this macro generates:
 * 
 * 1. **Basic Detection** (Supports ADL):
 *    - `foo_function_operation<Args...>`: The detection alias using `foo(declval<Args>()...)`.
 *    - `has_foo_function_v<Args...>`: Boolean constant.
 *    - `foo_function_t<Args...>`: Resulting return type.
 * 
 * 2. **Exact Detection** (Checks for a specific symbol pointer):
 *    - `foo_function_exact_operation<Args...>`: Uses `&foo` to verify a specific function exists.
 *    - `has_foo_function_exact_v<Args...>`: Boolean constant.
 *    - `foo_function_exact_t<Args...>`: Resulting return type.
 * 
 * @param name The name of the function to detect.
 * 
 * @warning CRITICAL: This macro MUST be invoked AFTER the target function has been declared.
 * Since the macro expansion takes the address of the function (&name), the symbol must 
 * be visible to the compiler at the point of expansion, or a compilation error will occur.
 */
#define SCL_FUNCTION_DETECTION(name)                                                      \
    template <typename... Arguments>                                                      \
    using name##_function_operation = decltype(name(::std::declval<Arguments>()...));     \
    template <typename... Arguments>                                                      \
    inline constexpr auto has_##name##_function_v =                                       \
        ::scl::is_detected_v<name##_function_operation, Arguments...>;                    \
    template <typename... Arguments>                                                      \
    using name##_function_t = ::scl::detected_t<name##_function_operation, Arguments...>; \
                                                                                          \
    template <typename... Arguments>                                                      \
    using name##_function_exact_operation = decltype((static_cast<                        \
        name##_function_operation<Arguments...> (*)(Arguments...)>(&name))(               \
        ::std::declval<Arguments>()...));                                                 \
    template <typename... Arguments>                                                      \
    inline constexpr auto has_##name##_function_exact_v =                                 \
        ::scl::is_detected_v<name##_function_exact_operation, Arguments...>;              \
    template <typename... Arguments>                                                      \
    using name##_function_exact_t = ::scl::detected_t<name##_function_exact_operation, Arguments...>;
