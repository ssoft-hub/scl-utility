/**
 * @file
 * @brief Implementation of C++ Standard Proposal N4502 - Detection Idiom
 * @ingroup scl_utility_type_traits
 * @see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf
 * 
 * This header provides utilities for detecting the validity of expressions
 * at compile-time using SFINAE techniques.
 */
#pragma once

#include <type_traits>

namespace scl::detail
{
    /**
     * @internal
     * @brief Primary template for type detection mechanism
     * @ingroup scl_utility_type_traits
     * @tparam Default The default type to use when detection fails
     * @tparam AlwaysVoid Helper parameter for SFINAE (should be void)
     * @tparam Operation Template template parameter to test
     * @tparam Arguments Arguments to pass to Operation
     * 
     * This specialization is selected when Operation<Arguments...> is ill-formed.
     */
    template <typename Default, typename AlwaysVoid, template <typename...> class Operation, typename... Arguments>
    struct detector
    {
        using value_t = ::std::false_type; ///< Indicates detection failed
        using type = Default;              ///< Falls back to Default type
    };

    /**
     * @internal
     * @brief Specialization when Operation<Arguments...> is well-formed
     * @ingroup scl_utility_type_traits
     * @tparam Default The default type (unused in this specialization)
     * @tparam Operation Template template parameter to test
     * @tparam Arguments Arguments to pass to Operation
     * 
     * This specialization is selected when Operation<Arguments...> is well-formed.
     */
    template <typename Default, template <typename...> class Operation, typename... Arguments>
    struct detector<Default, ::std::void_t<Operation<Arguments...>>, Operation, Arguments...>
    {
        using value_t = ::std::true_type;     ///< Indicates detection succeeded
        using type = Operation<Arguments...>; ///< Resolved to actual type
    };

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Detects whether Operation<Arguments...> is a valid expression
     * @ingroup scl_utility_type_traits
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * Alias to std::true_type if Operation<Arguments...> is well-formed,
     * std::false_type otherwise.
     * 
     * @code{.cpp}
     * template <typename T>
     * using has_foo_t = decltype(std::declval<T>().foo());
     * 
     * static_assert(is_detected<has_foo_t, MyClass>::value);
     * @endcode
     */
    template <template <typename...> class Operation, typename... Arguments>
    using is_detected = typename detail::detector<void, void, Operation, Arguments...>::value_t;

    /**
     * @brief Variable template version of is_detected
     * @ingroup scl_utility_type_traits
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * @code{.cpp}
     * if constexpr (is_detected_v<has_foo_t, MyClass>) { ... }
     * @endcode
     */
    template <template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_v = is_detected<Operation, Arguments...>::value;

    /**
     * @brief Gets the type of Operation<Arguments...> or void if ill-formed
     * @ingroup scl_utility_type_traits
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * @code{.cpp}
     * using result = detected_t<has_foo_t, MyClass>;
     * @endcode
     */
    template <template <typename...> class Operation, typename... Arguments>
    using detected_t = typename detail::detector<void, void, Operation, Arguments...>::type;

    /**
     * @brief Helper for getting detected type or a default
     * @ingroup scl_utility_type_traits
     * @tparam Default Type to use if detection fails
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * Provides nested type 'type' which is either Operation<Arguments...>
     * if well-formed, or Default otherwise.
     */
    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or = detail::detector<Default, void, Operation, Arguments...>;

    /**
     * @brief Gets the type of Operation<Arguments...> or Default if ill-formed
     * @ingroup scl_utility_type_traits
     * @tparam Default Type to use if detection fails
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * @code{.cpp}
     * using result = detected_or_t<int, has_foo_t, MyClass>;
     * @endcode
     */
    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or_t = typename detected_or<Default, Operation, Arguments...>::type;

    /**
     * @brief Checks if Operation<Arguments...> results in exactly Expected type
     * @ingroup scl_utility_type_traits
     * @tparam Expected The type to compare against
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * @code{.cpp}
     * static_assert(is_detected_exact<int, get_value_type_t, MyClass>::value);
     * @endcode
     */
    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    using is_detected_exact = ::std::is_same<Expected, detected_t<Operation, Arguments...>>;

    /**
     * @brief Variable template version of is_detected_exact
     * @ingroup scl_utility_type_traits
     * @tparam Expected The type to compare against
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     */
    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_exact_v = is_detected_exact<Expected, Operation, Arguments...>::value;

    /**
     * @brief Checks if Operation<Arguments... > is convertible to To
     * @ingroup scl_utility_type_traits
     * @tparam To Target type for conversion
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     * 
     * @code{.cpp}
     * static_assert(is_detected_convertible<double, get_value_t, MyClass>::value);
     * @endcode
     */
    template <typename To, template <typename...> class Operation, typename... Arguments>
    using is_detected_convertible = ::std::is_convertible<detected_t<Operation, Arguments...>, To>;

    /**
     * @brief Variable template version of is_detected_convertible
     * @ingroup scl_utility_type_traits
     * @tparam To Target type for conversion
     * @tparam Operation Template to instantiate
     * @tparam Arguments Arguments for the template
     */
    template <typename To, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_convertible_v = is_detected_convertible<To, Operation, Arguments...>::value;

} // namespace scl
