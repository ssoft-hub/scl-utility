#pragma once

#include <type_traits>

/**
 * @file
 * @brief The type of a pointer to a member, qualified as the type of the object it is used on.
 * @ingroup scl_utility_type_traits
 */

namespace scl::detail
{
    enum class member_function_reference : unsigned char
    {
        none,
        lvalue,
        rvalue
    };

    template <typename Type>
    inline constexpr ::scl::detail::member_function_reference member_function_reference_of =
        ::scl::detail::member_function_reference::none;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_reference member_function_reference_of<Type &> =
        ::scl::detail::member_function_reference::lvalue;
    template <typename Type>
    inline constexpr ::scl::detail::member_function_reference member_function_reference_of<Type &&> =
        ::scl::detail::member_function_reference::rvalue;

    template <typename Class, typename Signature, ::scl::detail::member_function_reference Reference>
    struct member_function_forms;

    template <typename Class, typename Result, typename... Arguments>
    struct member_function_forms<Class, Result(Arguments...), ::scl::detail::member_function_reference::none>
    {
        using plain = Result (Class::*)(Arguments...);
        using c = Result (Class::*)(Arguments...) const;
        using v = Result (Class::*)(Arguments...) volatile;
        using cv = Result (Class::*)(Arguments...) const volatile;
    };

    template <typename Class, typename Result, typename... Arguments>
    struct member_function_forms<
        Class, Result(Arguments...) noexcept, ::scl::detail::member_function_reference::none>
    {
        using plain = Result (Class::*)(Arguments...) noexcept;
        using c = Result (Class::*)(Arguments...) const noexcept;
        using v = Result (Class::*)(Arguments...) volatile noexcept;
        using cv = Result (Class::*)(Arguments...) const volatile noexcept;
    };

    template <typename Class, typename Result, typename... Arguments>
    struct member_function_forms<Class, Result(Arguments...), ::scl::detail::member_function_reference::lvalue>
    {
        using plain = Result (Class::*)(Arguments...) &;
        using c = Result (Class::*)(Arguments...) const &;
        using v = Result (Class::*)(Arguments...) volatile &;
        using cv = Result (Class::*)(Arguments...) const volatile &;
    };

    template <typename Class, typename Result, typename... Arguments>
    struct member_function_forms<
        Class, Result(Arguments...) noexcept, ::scl::detail::member_function_reference::lvalue>
    {
        using plain = Result (Class::*)(Arguments...) & noexcept;
        using c = Result (Class::*)(Arguments...) const & noexcept;
        using v = Result (Class::*)(Arguments...) volatile & noexcept;
        using cv = Result (Class::*)(Arguments...) const volatile & noexcept;
    };

    template <typename Class, typename Result, typename... Arguments>
    struct member_function_forms<Class, Result(Arguments...), ::scl::detail::member_function_reference::rvalue>
    {
        using plain = Result (Class::*)(Arguments...) &&;
        using c = Result (Class::*)(Arguments...) const &&;
        using v = Result (Class::*)(Arguments...) volatile &&;
        using cv = Result (Class::*)(Arguments...) const volatile &&;
    };

    template <typename Class, typename Result, typename... Arguments>
    struct member_function_forms<
        Class, Result(Arguments...) noexcept, ::scl::detail::member_function_reference::rvalue>
    {
        using plain = Result (Class::*)(Arguments...) && noexcept;
        using c = Result (Class::*)(Arguments...) const && noexcept;
        using v = Result (Class::*)(Arguments...) volatile && noexcept;
        using cv = Result (Class::*)(Arguments...) const volatile && noexcept;
    };

    template <typename Forms, typename Object>
    using member_function_cv_t = ::std::conditional_t<
        ::std::is_const_v<Object>,
        ::std::conditional_t<::std::is_volatile_v<Object>, typename Forms::cv, typename Forms::c>,
        ::std::conditional_t<::std::is_volatile_v<Object>, typename Forms::v, typename Forms::plain>>;

    template <typename Type, typename Signature>
    struct member_function_build
    {
        static_assert(
            ::std::is_class_v<::std::remove_cvref_t<Type>>,
            "member_function_like_t requires Type to be a class type");

        using type = ::scl::detail::member_function_cv_t<
            ::scl::detail::member_function_forms<
                ::std::remove_cvref_t<Type>, Signature, ::scl::detail::member_function_reference_of<Type>>,
            ::std::remove_reference_t<Type>>;
    };

    template <typename Type, typename Signature>
    struct member_function_like
    {
        static_assert(
            ::std::is_function_v<Signature>,
            "::scl::member_function_like_t requires a function type like Result(Arguments...) or Result(Arguments...) noexcept");
    };

    template <typename Type, typename Result, typename... Arguments>
    struct member_function_like<Type, Result(Arguments...)>
        : ::scl::detail::member_function_build<Type, Result(Arguments...)>
    {};

    template <typename Type, typename Result, typename... Arguments>
    struct member_function_like<Type, Result(Arguments...) noexcept>
        : ::scl::detail::member_function_build<Type, Result(Arguments...) noexcept>
    {};

    template <typename Object, typename Member>
    using add_const_like_t = ::std::conditional_t<::std::is_const_v<Object>, Member const, Member>;

    template <typename Object, typename Member>
    using add_cv_like_t = ::std::conditional_t<
        ::std::is_volatile_v<Object>, ::scl::detail::add_const_like_t<Object, Member> volatile,
        ::scl::detail::add_const_like_t<Object, Member>>;

    template <typename Class, typename Member>
    using member_object_pointer_t = Member Class::*;

    template <typename Type, typename Member>
    struct member_object_like
    {
        static_assert(
            ::std::is_class_v<::std::remove_cvref_t<Type>> || ::std::is_union_v<::std::remove_cvref_t<Type>>,
            "member_object_like_t requires Type to be a class or union type");
        static_assert(
            !::std::is_function_v<Member>,
            "member_object_like_t expects a data member type; use member_function_like_t for member functions");

        using type = ::scl::detail::member_object_pointer_t<
            ::std::remove_cvref_t<Type>, ::scl::detail::add_cv_like_t<::std::remove_reference_t<Type>, Member>>;
    };

    template <typename Type, typename T, bool IsFunction = ::std::is_function_v<T>>
    struct member_like;

    template <typename Type, typename Signature>
    struct member_like<Type, Signature, true> : ::scl::detail::member_function_like<Type, Signature>
    {};

    template <typename Type, typename Member>
    struct member_like<Type, Member, false> : ::scl::detail::member_object_like<Type, Member>
    {};

} // namespace scl::detail

namespace scl
{
#ifndef SCL_DOXYGEN
    template <typename Type, typename Signature>
    using member_function_like_t = ::scl::detail::member_function_like<Type, Signature>::type;

    template <typename Type, typename Member>
    using member_object_like_t = ::scl::detail::member_object_like<Type, Member>::type;

    template <typename Type, typename Member>
    using member_like_t = ::scl::detail::member_like<Type, Member>::type;
#endif
} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef SCL_DOXYGEN
namespace scl
{
    template <typename Type, typename Signature>
    using member_function_like_t = unspecified;

    template <typename Type, typename Member>
    using member_object_like_t = unspecified;

    template <typename Type, typename Member>
    using member_like_t = unspecified;
} // namespace scl
#endif

// =============================================================================
// Documentation
// =============================================================================

/**
 * @typedef scl::member_function_like_t
 * @ingroup scl_utility_type_traits
 * @brief The pointer to a member function of @p Signature, qualified as the object type @p Type.
 *
 * The class is `std::remove_cvref_t<Type>`. The member function has the result and the parameters
 * of @p Signature, the cv-qualifiers and the ref-qualifier of @p Type and the `noexcept` of
 * @p Signature. For a @p Type that is not a reference the member function has no ref-qualifier.
 * @p Signature is `Result(Parameters...)` or `Result(Parameters...) noexcept`. The alias template
 * is not defined for another function type, a C-style variadic one or one with cv- or
 * ref-qualifiers, and the substitution of such a type fails without a build error. For
 * a @p Signature of one of these two forms the build ends with an error if
 * `std::remove_cvref_t<Type>` is not a class. The build also ends with an error if @p Signature is
 * not a function type.
 *
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp types
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp function
 *
 * @tparam Type Type of the object the member function is called on.
 * @tparam Signature Function type of the member function.
 * @see ::scl::member_like_t, ::scl::signature::object_t
 */

/**
 * @typedef scl::member_object_like_t
 * @ingroup scl_utility_type_traits
 * @brief The pointer to a data member of the type @p Member, qualified as the object type @p Type.
 *
 * The class is `std::remove_cvref_t<Type>`, and the data member has the type @p Member with
 * the `const` and `volatile` of @p Type added. The cv-qualifiers of @p Member stay, and
 * the reference of @p Type is dropped. The build ends with an error unless @p Member is an object
 * type and `std::remove_cvref_t<Type>` is a class or a union.
 *
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp types
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp object
 *
 * @tparam Type Type of the object the data member is read through.
 * @tparam Member Type of the data member.
 * @see ::scl::member_like_t
 */

/**
 * @typedef scl::member_like_t
 * @ingroup scl_utility_type_traits
 * @brief The pointer to a member function or a data member, chosen by the type @p Member.
 *
 * The alias template is ::scl::member_function_like_t<Type, Member> where @p Member is a function
 * type, and ::scl::member_object_like_t<Type, Member> otherwise. The type of a pointer to
 * a function is not a function type, so for it the alias template is the type of a pointer to
 * a data member. The build ends with an error if @p Member is a reference, a reference to
 * a function included.
 *
 * A variable declared `auto` cannot be initialized with the address of member functions that differ
 * only in their qualifiers. The overload for the type of the object is chosen by casting
 * the address to this pointer type. In a function template the type `Object &&` of a forwarded
 * object is passed as @p Type.
 *
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp types
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp member_like
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp overload
 * @snippet type_traits/member_like/type_traits_member_like_example.cpp use
 *
 * @tparam Type Type of the object the member is used on.
 * @tparam Member Function type of a member function or type of a data member.
 * @see ::scl::signature::object_t
 */
