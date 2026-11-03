/**
 * @file
 * @brief Whether an expression over given types is well-formed, and which type it yields.
 *
 * The templates of the detection idiom take their names from Library Fundamentals TS v2.
 * @ingroup scl_utility_type_traits
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
        using value_t = ::std::false_type;
        using type = Default;
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
        using value_t = ::std::true_type;
        using type = Operation<Arguments...>;
    };

} // namespace scl::detail

namespace scl
{
    struct nonesuch
    {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch(nonesuch const &) = delete;
        nonesuch(nonesuch &&) = delete;
        void operator=(nonesuch const &) = delete;
        void operator=(nonesuch &&) = delete;
    };

#ifndef SCL_DOXYGEN
    template <template <typename...> class Operation, typename... Arguments>
    using is_detected = ::scl::detail::detector<void, void, Operation, Arguments...>::value_t;
#endif

    template <template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_v = ::scl::is_detected<Operation, Arguments...>::value;

#ifndef SCL_DOXYGEN
    template <template <typename...> class Operation, typename... Arguments>
    using detected_t = ::scl::detail::detector<::scl::nonesuch, void, Operation, Arguments...>::type;

    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or = ::scl::detail::detector<Default, void, Operation, Arguments...>;
#endif

    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or_t = ::scl::detected_or<Default, Operation, Arguments...>::type;

    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    using is_detected_exact = ::std::bool_constant<(
        ::scl::is_detected_v<Operation, Arguments...> &&
        ::std::is_same_v<Expected, ::scl::detected_t<Operation, Arguments...>>)>;

    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_exact_v =
        ::scl::is_detected_exact<Expected, Operation, Arguments...>::value;

    template <typename To, template <typename...> class Operation, typename... Arguments>
    using is_detected_convertible = ::std::bool_constant<(
        ::scl::is_detected_v<Operation, Arguments...> &&
        ::std::is_convertible_v<::scl::detected_t<Operation, Arguments...>, To>)>;

    template <typename To, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_convertible_v =
        ::scl::is_detected_convertible<To, Operation, Arguments...>::value;

} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef SCL_DOXYGEN
namespace scl
{
    template <template <typename...> class Operation, typename... Arguments>
    using is_detected = unspecified;

    template <template <typename...> class Operation, typename... Arguments>
    using detected_t = unspecified;

    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or = unspecified;
} // namespace scl
#endif

// =============================================================================
// Documentation
// =============================================================================

/**
 * @struct scl::nonesuch
 * @ingroup scl_utility_type_traits
 * @brief The type ::scl::detected_t names where `Operation<Arguments...>` is ill-formed.
 *
 * No object of the type can be created, copied, moved or destroyed.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp detected_t
 */

/**
 * @fn scl::nonesuch::nonesuch()
 * @brief Deleted, so no object of ::scl::nonesuch is created.
 */

/**
 * @fn scl::nonesuch::~nonesuch()
 * @brief Deleted, so no object of ::scl::nonesuch is destroyed.
 */

/**
 * @fn scl::nonesuch::nonesuch(nonesuch const &)
 * @brief Deleted, so no object of ::scl::nonesuch is copied.
 */

/**
 * @fn scl::nonesuch::nonesuch(nonesuch &&)
 * @brief Deleted, so no object of ::scl::nonesuch is moved.
 */

/**
 * @fn scl::nonesuch::operator=(nonesuch const &)
 * @brief Deleted, so no object of ::scl::nonesuch is assigned.
 */

/**
 * @fn scl::nonesuch::operator=(nonesuch &&)
 * @brief Deleted, so no object of ::scl::nonesuch is assigned.
 */

/**
 * @typedef scl::is_detected
 * @ingroup scl_utility_type_traits
 * @brief Whether `Operation<Arguments...>` is well-formed, as a `std::bool_constant`.
 *
 * An operation is an alias template or a class template. The operation is well-formed if
 * substituting @p Arguments into its declaration succeeds. The declaration of an alias template is
 * the type it names, such as `decltype(std::declval<T const &>().size())`. The body of a class
 * template is not checked, so the expression to check should be written in an alias template. If
 * the expression calls a specialization of a template with a deduced return type, such as of
 * a function template or a generic lambda expression, the body of that specialization is
 * instantiated. An error in that body is not a substitution failure, so the build ends with
 * an error instead of the value `false`.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp operations
 * @snippet type_traits/detection/type_traits_detection_example.cpp is_detected
 *
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 * @see ::scl::is_detected_v, ::scl::detected_t
 */

/**
 * @var scl::is_detected_v
 * @ingroup scl_utility_type_traits
 * @brief Whether `Operation<Arguments...>` is well-formed, the value of ::scl::is_detected.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp is_detected
 * @snippet type_traits/detection/type_traits_detection_example.cpp dispatch
 *
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 */

/**
 * @typedef scl::detected_t
 * @ingroup scl_utility_type_traits
 * @brief The type `Operation<Arguments...>` where it is well-formed, ::scl::nonesuch otherwise.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp detected_t
 *
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 * @see ::scl::detected_or_t
 */

/**
 * @typedef scl::detected_or
 * @ingroup scl_utility_type_traits
 * @brief A class with the members `type`, the operation or @p Default, and `value_t`.
 *
 * The member `type` is `Operation<Arguments...>` where the operation is well-formed and @p Default
 * otherwise. The member `value_t` is ::scl::is_detected of the same operation.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp detected_or
 *
 * @tparam Default Type named where the operation is ill-formed.
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 * @see ::scl::detected_or_t
 */

/**
 * @typedef scl::detected_or_t
 * @ingroup scl_utility_type_traits
 * @brief The type `Operation<Arguments...>` where it is well-formed, @p Default otherwise.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp detected_or
 *
 * @tparam Default Type named where the operation is ill-formed.
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 */

/**
 * @typedef scl::is_detected_exact
 * @ingroup scl_utility_type_traits
 * @brief Whether the operation is well-formed and its type is @p Expected.
 *
 * The alias template is `std::true_type` if the operation is well-formed and ::scl::detected_t of it
 * is @p Expected, and `std::false_type` otherwise, whatever @p Expected is.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp exact
 *
 * @tparam Expected Type the operation must yield.
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 * @see ::scl::is_detected_exact_v
 */

/**
 * @var scl::is_detected_exact_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the operation yields @p Expected, the value of ::scl::is_detected_exact.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp exact
 *
 * @tparam Expected Type the operation must yield.
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 */

/**
 * @typedef scl::is_detected_convertible
 * @ingroup scl_utility_type_traits
 * @brief Whether the operation is well-formed and its type converts to @p To.
 *
 * The alias template is `std::true_type` if the operation is well-formed and ::scl::detected_t of it
 * converts implicitly to @p To, and `std::false_type` otherwise, whatever @p To is.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp exact
 *
 * @tparam To Type the result must convert to.
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 * @see ::scl::is_detected_convertible_v
 */

/**
 * @var scl::is_detected_convertible_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the result converts to @p To, the value of ::scl::is_detected_convertible.
 *
 * @snippet type_traits/detection/type_traits_detection_example.cpp exact
 *
 * @tparam To Type the result must convert to.
 * @tparam Operation Alias template or class template of the operation.
 * @tparam Arguments Types the operation is instantiated with.
 */
