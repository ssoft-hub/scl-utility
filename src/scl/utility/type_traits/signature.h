#pragma once

/**
 * @file
 * @brief Properties of a callable type at compile time: its function type, result, parameters,
 *        class and qualifiers.
 * @ingroup scl_utility_type_traits
 */

#include <scl/utility/concepts/type_category.h>

#include <cstddef>
#include <tuple>
#include <type_traits>

namespace scl::detail
{
    template <typename Callable>
    struct signature
    {};

    template <typename Object, typename Function>
    struct callable_signature;

    template <typename Function>
    struct function_parts
    {};

    template <typename Result, typename... Parameters>
    struct function_parts<Result(Parameters...)>
    {
        using type = Result(Parameters...);
        using result_type = Result;
        using parameter_types = ::std::tuple<Parameters...>;
        static constexpr bool is_noexcept = false;
        static constexpr bool is_variadic = false;
    };

    template <typename Result, typename... Parameters>
    struct function_parts<Result(Parameters...) noexcept>
    {
        using type = Result(Parameters...) noexcept;
        using result_type = Result;
        using parameter_types = ::std::tuple<Parameters...>;
        static constexpr bool is_noexcept = true;
        static constexpr bool is_variadic = false;
    };

    template <typename Result, typename... Parameters>
    struct function_parts<Result(Parameters..., ...)>
    {
        using type = Result(Parameters..., ...);
        using result_type = Result;
        using parameter_types = ::std::tuple<Parameters...>;
        static constexpr bool is_noexcept = false;
        static constexpr bool is_variadic = true;
    };

    template <typename Result, typename... Parameters>
    struct function_parts<Result(Parameters..., ...) noexcept>
    {
        using type = Result(Parameters..., ...) noexcept;
        using result_type = Result;
        using parameter_types = ::std::tuple<Parameters...>;
        static constexpr bool is_noexcept = true;
        static constexpr bool is_variadic = true;
    };

    template <typename Object>
    struct object_parts
    {
        using object_type = Object;
        static constexpr bool is_const = ::std::is_const_v<::std::remove_reference_t<Object>>;
        static constexpr bool is_volatile = ::std::is_volatile_v<::std::remove_reference_t<Object>>;
        static constexpr bool is_lvalue_reference = ::std::is_lvalue_reference_v<Object>;
        static constexpr bool is_rvalue_reference = ::std::is_rvalue_reference_v<Object>;
    };

    template <>
    struct object_parts<void>
    {
        static constexpr bool is_const = false;
        static constexpr bool is_volatile = false;
        static constexpr bool is_lvalue_reference = false;
        static constexpr bool is_rvalue_reference = false;
    };

    template <typename Object, typename Function>
    struct callable_signature
        : ::scl::detail::function_parts<Function>
        , ::scl::detail::object_parts<Object>
    {
        template <typename Other>
        using with_object = ::scl::detail::callable_signature<Other, Function>;
    };

    // A function type with a calling convention other than the default matches none of the shapes.
    template <typename Function>
    concept plain_function = requires { typename ::scl::detail::function_parts<Function>::type; };

    template <::scl::detail::plain_function Function>
    struct signature<Function *> : ::scl::detail::callable_signature<void, Function>
    {};

// MSVC counts an empty macro argument as none (C4003), so an absent qualifier takes a marker.
#define SCL_DETAIL_NO_QUALIFIER

#define SCL_DETAIL_MEMBER_SIGNATURE(qualifiers, object, exception)                        \
    template <typename Result, typename Class, typename... Parameters>                    \
    struct signature<Result (Class::*)(Parameters...) qualifiers>                         \
        : ::scl::detail::callable_signature<object, Result(Parameters...) exception>      \
    {};                                                                                   \
    template <typename Result, typename Class, typename... Parameters>                    \
    struct signature<Result (Class::*)(Parameters..., ...) qualifiers>                    \
        : ::scl::detail::callable_signature<object, Result(Parameters..., ...) exception> \
    {}

    SCL_DETAIL_MEMBER_SIGNATURE(SCL_DETAIL_NO_QUALIFIER, Class, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(noexcept, Class, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(&, Class &, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(& noexcept, Class &, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(&&, Class &&, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(&& noexcept, Class &&, noexcept);

    SCL_DETAIL_MEMBER_SIGNATURE(const, Class const, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(const noexcept, Class const, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(const &, Class const &, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(const & noexcept, Class const &, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(const &&, Class const &&, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(const && noexcept, Class const &&, noexcept);

    SCL_DETAIL_MEMBER_SIGNATURE(volatile, Class volatile, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(volatile noexcept, Class volatile, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(volatile &, Class volatile &, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(volatile & noexcept, Class volatile &, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(volatile &&, Class volatile &&, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(volatile && noexcept, Class volatile &&, noexcept);

    SCL_DETAIL_MEMBER_SIGNATURE(const volatile, Class const volatile, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(const volatile noexcept, Class const volatile, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(const volatile &, Class const volatile &, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(const volatile & noexcept, Class const volatile &, noexcept);
    SCL_DETAIL_MEMBER_SIGNATURE(const volatile &&, Class const volatile &&, SCL_DETAIL_NO_QUALIFIER);
    SCL_DETAIL_MEMBER_SIGNATURE(const volatile && noexcept, Class const volatile &&, noexcept);

#undef SCL_DETAIL_MEMBER_SIGNATURE
#undef SCL_DETAIL_NO_QUALIFIER

    // A call operator declared in a base class still names Callable, not the base.
    template <typename Object, typename Callable>
    struct object_like
    {
        using bare_type = ::std::remove_reference_t<Object>;
        using const_type = ::std::conditional_t<::std::is_const_v<bare_type>, Callable const, Callable>;
        using cv_type = ::std::conditional_t<::std::is_volatile_v<bare_type>, const_type volatile, const_type>;
        using type = ::std::conditional_t<::std::is_lvalue_reference_v<Object>,
            cv_type &,
            ::std::conditional_t<::std::is_rvalue_reference_v<Object>, cv_type &&, cv_type>>;
    };

    template <typename Object, typename Callable>
    using object_like_t = ::scl::detail::object_like<Object, Callable>::type;

    template <typename Pointer>
    concept member_signature = requires { typename ::scl::detail::signature<Pointer>::object_type; };

    // A static member function reached through an object is an lvalue, whose address can be taken;
    // any other member function is a prvalue usable only in a call ([expr.ref]).
    template <typename Pointer, typename Callable>
    concept explicit_object_pointer = ::scl::concepts::pointer<Pointer> &&
        !requires(Callable & callable) { &callable.operator(); };

    template <typename Pointer>
    struct explicit_object_signature
    {};

    template <typename Result, typename Object, typename... Parameters>
    struct explicit_object_signature<Result (*)(Object, Parameters...)>
        : ::scl::detail::callable_signature<Object, Result(Parameters...)>
    {};

    template <typename Result, typename Object, typename... Parameters>
    struct explicit_object_signature<Result (*)(Object, Parameters...) noexcept>
        : ::scl::detail::callable_signature<Object, Result(Parameters...) noexcept>
    {};

    template <typename Result, typename Object, typename... Parameters>
    struct explicit_object_signature<Result (*)(Object, Parameters..., ...)>
        : ::scl::detail::callable_signature<Object, Result(Parameters..., ...)>
    {};

    template <typename Result, typename Object, typename... Parameters>
    struct explicit_object_signature<Result (*)(Object, Parameters..., ...) noexcept>
        : ::scl::detail::callable_signature<Object, Result(Parameters..., ...) noexcept>
    {};

    // The address of a static call operator is a pointer to a function.
    template <typename Callable, typename Pointer>
    struct call_operator_signature : ::scl::detail::signature<Pointer>
    {};

    template <typename Callable, ::scl::detail::member_signature Pointer>
    struct call_operator_signature<Callable, Pointer>
        : ::scl::detail::signature<Pointer>::template with_object<
              ::scl::detail::object_like_t<typename ::scl::detail::signature<Pointer>::object_type, Callable>>
    {};

    template <typename Callable, ::scl::detail::explicit_object_pointer<Callable> Pointer>
    struct call_operator_signature<Callable, Pointer> : ::scl::detail::explicit_object_signature<Pointer>
    {};

    template <typename Callable>
    concept call_operator_class = (::std::is_class_v<Callable> || ::std::is_union_v<Callable>) &&
        requires { &Callable::operator(); };

    template <::scl::detail::call_operator_class Callable>
    struct signature<Callable>
        : ::scl::detail::call_operator_signature<Callable, decltype(&Callable::operator())>
    {};

    template <typename Callable>
    using signature_of = ::scl::detail::signature<::std::decay_t<Callable>>;

    template <typename Callable>
    concept has_signature = requires { typename ::scl::detail::signature_of<Callable>::type; };

    template <typename Callable, ::std::size_t Index>
    concept parameter_index = ::scl::detail::has_signature<Callable> &&
        (Index < ::std::tuple_size_v<typename ::scl::detail::signature_of<Callable>::parameter_types>);

    template <typename Callable, typename Index>
    struct signature_parameter
    {};

    template <::std::size_t Index, ::scl::detail::parameter_index<Index> Callable>
    struct signature_parameter<Callable, ::std::integral_constant<::std::size_t, Index>>
    {
        using type = ::std::tuple_element_t<Index, typename ::scl::detail::signature_of<Callable>::parameter_types>;
    };

} // namespace scl::detail

namespace scl
{
    template <typename Callable>
    using signature_t = ::scl::detail::signature_of<Callable>::type;

} // namespace scl

namespace scl::signature
{
    template <typename Callable>
    using result_t = ::scl::detail::signature_of<Callable>::result_type;

    template <typename Callable>
    using parameters_t = ::scl::detail::signature_of<Callable>::parameter_types;

    template <typename Callable>
    inline constexpr ::std::size_t parameter_count_v =
        ::std::tuple_size_v<::scl::signature::parameters_t<Callable>>;

    template <typename Callable, ::std::size_t Index>
    using parameter_t =
        ::scl::detail::signature_parameter<Callable, ::std::integral_constant<::std::size_t, Index>>::type;

    template <typename Callable>
    using object_t = ::scl::detail::signature_of<Callable>::object_type;

    template <typename Callable>
    inline constexpr bool is_const_v = ::scl::detail::signature_of<Callable>::is_const;

    template <typename Callable>
    inline constexpr bool is_volatile_v = ::scl::detail::signature_of<Callable>::is_volatile;

    template <typename Callable>
    inline constexpr bool is_lvalue_reference_v = ::scl::detail::signature_of<Callable>::is_lvalue_reference;

    template <typename Callable>
    inline constexpr bool is_rvalue_reference_v = ::scl::detail::signature_of<Callable>::is_rvalue_reference;

    template <typename Callable>
    inline constexpr bool is_noexcept_v = ::scl::detail::signature_of<Callable>::is_noexcept;

    template <typename Callable>
    inline constexpr bool is_variadic_v = ::scl::detail::signature_of<Callable>::is_variadic;

} // namespace scl::signature

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @namespace scl::signature
 * @ingroup scl_utility_type_traits
 * @brief The parts of the signature of a callable type: result, parameters, object type,
 *        qualifiers, `noexcept` and a variable number of arguments, beside the function type
 *        ::scl::signature_t.
 *
 * Every template of the namespace takes the same callable types as ::scl::signature_t: a function
 * type, a pointer or a reference to a function, a pointer to a member function, and a class or a
 * union with one call operator that is not a template, the type of a lambda included.
 *
 * | Template | For `int (widget::*)(long, char) const & noexcept` |
 * |---|---|
 * | ::scl::signature_t | `int(long, char) noexcept` |
 * | ::scl::signature::result_t | `int` |
 * | ::scl::signature::parameters_t | `std::tuple<long, char>` |
 * | ::scl::signature::parameter_count_v | `2` |
 * | ::scl::signature::parameter_t | `char` for the index `1` |
 * | ::scl::signature::object_t | `widget const &` |
 * | ::scl::signature::is_const_v | `true` |
 * | ::scl::signature::is_volatile_v | `false` |
 * | ::scl::signature::is_lvalue_reference_v | `true` |
 * | ::scl::signature::is_rvalue_reference_v | `false` |
 * | ::scl::signature::is_noexcept_v | `true` |
 * | ::scl::signature::is_variadic_v | `false` |
 */

/**
 * @typedef scl::signature_t
 * @ingroup scl_utility_type_traits
 * @brief The function type of the callable type `Callable`, with `noexcept` and a trailing `...`,
 *        without a pointer, a class or cv- and ref-qualifiers.
 *
 * `Callable` is one of:
 * - a function type, such as `int(long)`;
 * - a pointer or a reference to a function;
 * - a pointer to a member function with any combination of cv-qualifiers, a ref-qualifier and
 *   `noexcept`;
 * - a class or a union with one call operator that is not a template, the type of a lambda
 *   included; a class with cv-qualifiers or a reference to a class gives the signature of the
 *   class.
 *
 * Each of them may be a C-style variadic function, one whose parameter list ends with `...`. The
 * explicit object parameter of a call operator (C++23) is not among the parameters, and a static
 * call operator (C++23) keeps every parameter. A class must be complete: asking about a class
 * before its definition makes the program ill-formed, no diagnostic required.
 *
 * A generic lambda, a class with an overloaded call operator, a type that is not callable, a
 * pointer to a data member, a function type with cv- or ref-qualifiers and a function with a
 * calling convention other than the default, such as `__vectorcall`, or `__stdcall` on 32-bit x86,
 * have no signature, and the alias template names no type for them, so a requires-expression
 * tells whether a signature exists. One overload of an overloaded function is selected with the
 * function object ::scl::overload_cast first.
 *
 * A member function:
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp signature
 *
 * A function, a lambda and a variadic function:
 * @snippet type_traits/signature/type_traits_signature_example.cpp function
 * @snippet type_traits/signature/type_traits_signature_example.cpp lambda
 * @snippet type_traits/signature/type_traits_signature_example.cpp variadic
 *
 * A call operator with an explicit object parameter (C++23):
 * @snippet type_traits/signature/type_traits_signature_example.cpp explicit
 *
 * No signature, and an overload selected first:
 * @snippet type_traits/signature/type_traits_signature_example.cpp absent
 * @snippet type_traits/signature/type_traits_signature_example.cpp overload
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::object_t, ::scl::member_like_t
 */

/**
 * @typedef scl::signature::result_t
 * @ingroup scl_utility_type_traits
 * @brief The result type of the function type ::scl::signature_t<Callable>.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp function
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature_t
 */

/**
 * @typedef scl::signature::parameters_t
 * @ingroup scl_utility_type_traits
 * @brief `std::tuple` of the parameter types of the function type ::scl::signature_t<Callable>,
 *        in the order of declaration, a trailing `...` left out.
 *
 * The explicit object parameter of a call operator is not among the types; the parameters of a
 * static call operator all are.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp signature
 * @snippet type_traits/signature/type_traits_signature_example.cpp variadic
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::parameter_t, ::scl::signature::parameter_count_v
 */

/**
 * @var scl::signature::parameter_count_v
 * @ingroup scl_utility_type_traits
 * @brief The number of parameters of the function type ::scl::signature_t<Callable>, a trailing
 *        `...` left out.
 *
 * The value is the number of declared parameters. Whether a variadic function takes more
 * arguments is told by ::scl::signature::is_variadic_v, so exactly two arguments are
 * `parameter_count_v<F> == 2 && !is_variadic_v<F>`.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp function
 * @snippet type_traits/signature/type_traits_signature_example.cpp variadic
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::parameters_t, ::scl::signature::is_variadic_v
 */

/**
 * @typedef scl::signature::parameter_t
 * @ingroup scl_utility_type_traits
 * @brief The parameter type at the index `Index` of the function type ::scl::signature_t<Callable>.
 *
 * For an index not below ::scl::signature::parameter_count_v<Callable> the alias template names no
 * type, so a requires-expression tells whether the parameter exists.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp signature
 * @snippet type_traits/signature/type_traits_signature_example.cpp index
 *
 * A constraint on the parameter a callback declares:
 * @snippet type_traits/signature/type_traits_signature_example.cpp absent
 * @snippet type_traits/signature/type_traits_signature_example.cpp constrain
 *
 * @tparam Callable Callable type.
 * @tparam Index Index of the parameter.
 * @see ::scl::signature::parameters_t
 */

/**
 * @typedef scl::signature::object_t
 * @ingroup scl_utility_type_traits
 * @brief The type of the object a member function or a call operator is called on: its class
 *        with the cv-qualifiers and the ref-qualifier the member function is declared with.
 *
 * For a member function declared `const &` of the class `widget` it is `widget const &`, and for
 * a member function with no qualifiers it is `widget`; the class itself is
 * `std::remove_cvref_t<scl::signature::object_t<Callable>>`.
 *
 * The alias template ::scl::member_like_t takes the object type in this form, so
 * `scl::member_like_t<scl::signature::object_t<P>, scl::signature_t<P>>` is the pointer to a
 * member function `P` itself, where `P` is not variadic:
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp object
 *
 * For a class with a call operator it is that class, even where the call operator is declared in a
 * base class. For a call operator with an explicit object parameter it is the type of that
 * parameter, the base class included where the call operator is declared there. A function and a
 * static call operator have no object, and the alias template names no type for them.
 *
 * A lambda, a member function with the ref-qualifier `&&` and a call operator declared in a base
 * class:
 * @snippet type_traits/signature/type_traits_signature_example.cpp lambda
 * @snippet type_traits/signature/type_traits_signature_example.cpp rvalue
 * @snippet type_traits/signature/type_traits_signature_example.cpp inherited
 * @snippet type_traits/signature/type_traits_signature_example.cpp explicit
 *
 * @tparam Callable Callable type.
 * @see ::scl::member_like_t
 */

/**
 * @var scl::signature::is_const_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the member function or the call operator is declared `const`; `false` for a
 *        function.
 *
 * The call operator of a lambda is `const` unless the lambda is declared `mutable`. For a call
 * operator with an explicit object parameter the value is read from the type of that parameter.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp qualifiers
 * @snippet type_traits/signature/type_traits_signature_example.cpp lambda
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::is_volatile_v, ::scl::signature::object_t
 */

/**
 * @var scl::signature::is_volatile_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the member function or the call operator is declared `volatile`; `false` for a
 *        function.
 *
 * For a call operator with an explicit object parameter the value is read from the type of that
 * parameter.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp qualifiers
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::is_const_v, ::scl::signature::object_t
 */

/**
 * @var scl::signature::is_lvalue_reference_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the member function or the call operator is declared with the ref-qualifier
 *        `&`; `false` for a function.
 *
 * For a member function it equals `std::is_lvalue_reference_v` of
 * ::scl::signature::object_t<Callable>. With ::scl::signature::is_rvalue_reference_v also `false`,
 * the member function has no ref-qualifier.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp qualifiers
 * @snippet type_traits/signature/type_traits_signature_example.cpp rvalue
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::is_rvalue_reference_v
 */

/**
 * @var scl::signature::is_rvalue_reference_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the member function or the call operator is declared with the ref-qualifier
 *        `&&`; `false` for a function.
 *
 * For a member function it equals `std::is_rvalue_reference_v` of
 * ::scl::signature::object_t<Callable>.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp rvalue
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::is_lvalue_reference_v
 */

/**
 * @var scl::signature::is_noexcept_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the function, the member function or the call operator is declared `noexcept`.
 *
 * The value follows the declaration rather than a call: for the function
 * `void f(std::string) noexcept` it is `true`, although copying an argument into the parameter can
 * throw. The function type ::scl::signature_t<Callable> keeps `noexcept` as well.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp types
 * @snippet type_traits/signature/type_traits_signature_example.cpp qualifiers
 * @snippet type_traits/signature/type_traits_signature_example.cpp function
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature_t
 */

/**
 * @var scl::signature::is_variadic_v
 * @ingroup scl_utility_type_traits
 * @brief Whether the function, the member function or the call operator is a C-style variadic
 *        function, one whose parameter list ends with `...`, as `int(char const *, ...)` does.
 *
 * The trailing `...` stays in ::scl::signature_t<Callable> and is left out of
 * ::scl::signature::parameters_t and ::scl::signature::parameter_count_v. A variadic template is
 * not meant: a specialization of one is already a concrete type.
 *
 * @snippet type_traits/signature/type_traits_signature_example.cpp variadic
 *
 * @tparam Callable Callable type.
 * @see ::scl::signature::parameter_count_v
 */
