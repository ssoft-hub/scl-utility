#pragma once

#include <type_traits>

/**
 * @file
 * @brief The type of a pointer to a member, qualified as the type of the object it is used on.
 * @ingroup scl_utility_type_traits
 */

namespace scl::detail
{
    template <typename Type, typename Signature>
    struct member_function_like;

    enum class member_function_noexcept : bool
    {
        disabled = false,
        enabled = true
    };

    // Common builder parametrized by Noex (controls noexcept on the member function)
    template <typename Type, typename Result, ::scl::detail::member_function_noexcept Noexcept, typename... Arguments>
    struct build_member_function_like
    {
    private:
        static_assert(
            ::std::is_class_v<::std::remove_cvref_t<Type>>,
            "member_function_like_t requires Type to be a class type");

        using class_t = ::std::remove_cvref_t<Type>;

        // All cv/ref forms in one place; noexcept is controlled by Noexcept
        using un = Result (class_t::*)(Arguments...);
        using c = Result (class_t::*)(Arguments...) const;
        using v = Result (class_t::*)(Arguments...) volatile;
        using cv = Result (class_t::*)(Arguments...) const volatile;

        using un_l = Result (class_t::*)(Arguments...) &;
        using un_r = Result (class_t::*)(Arguments...) &&;
        using c_l = Result (class_t::*)(Arguments...) const &;
        using c_r = Result (class_t::*)(Arguments...) const &&;
        using v_l = Result (class_t::*)(Arguments...) volatile &;
        using v_r = Result (class_t::*)(Arguments...) volatile &&;
        using cv_l = Result (class_t::*)(Arguments...) const volatile &;
        using cv_r = Result (class_t::*)(Arguments...) const volatile &&;

        static constexpr bool is_c = ::std::is_const_v<::std::remove_reference_t<Type>>;
        static constexpr bool is_v = ::std::is_volatile_v<::std::remove_reference_t<Type>>;
        static constexpr bool is_l = ::std::is_lvalue_reference_v<Type>;
        static constexpr bool is_r = ::std::is_rvalue_reference_v<Type>;

    public:
        using type = ::std::conditional_t<
            is_l,
            // lvalue-qualified member functions
            ::std::conditional_t<
                (is_c && is_v), cv_l, ::std::conditional_t<is_c, c_l, ::std::conditional_t<is_v, v_l, un_l>>>,
            ::std::conditional_t<
                is_r,
                // rvalue-qualified member functions
                ::std::conditional_t<
                    (is_c && is_v), cv_r, ::std::conditional_t<is_c, c_r, ::std::conditional_t<is_v, v_r, un_r>>>,
                // no ref-qualifier
                ::std::conditional_t<
                    (is_c && is_v), cv, ::std::conditional_t<is_c, c, ::std::conditional_t<is_v, v, un>>>>>;
    };

    template <typename Type, typename Result, typename... Arguments>
    struct build_member_function_like<Type, Result, ::scl::detail::member_function_noexcept::enabled, Arguments...>
    {
    private:
        static_assert(
            ::std::is_class_v<::std::remove_cvref_t<Type>>,
            "member_function_like_t requires Type to be a class type");

        using class_t = ::std::remove_cvref_t<Type>;

        // All cv/ref forms in one place; noexcept is controlled by Noexcept
        using un = Result (class_t::*)(Arguments...) noexcept;
        using c = Result (class_t::*)(Arguments...) const noexcept;
        using v = Result (class_t::*)(Arguments...) volatile noexcept;
        using cv = Result (class_t::*)(Arguments...) const volatile noexcept;

        using un_l = Result (class_t::*)(Arguments...) & noexcept;
        using un_r = Result (class_t::*)(Arguments...) && noexcept;
        using c_l = Result (class_t::*)(Arguments...) const & noexcept;
        using c_r = Result (class_t::*)(Arguments...) const && noexcept;
        using v_l = Result (class_t::*)(Arguments...) volatile & noexcept;
        using v_r = Result (class_t::*)(Arguments...) volatile && noexcept;
        using cv_l = Result (class_t::*)(Arguments...) const volatile & noexcept;
        using cv_r = Result (class_t::*)(Arguments...) const volatile && noexcept;

        static constexpr bool is_c = ::std::is_const_v<::std::remove_reference_t<Type>>;
        static constexpr bool is_v = ::std::is_volatile_v<::std::remove_reference_t<Type>>;
        static constexpr bool is_l = ::std::is_lvalue_reference_v<Type>;
        static constexpr bool is_r = ::std::is_rvalue_reference_v<Type>;

    public:
        using type = ::std::conditional_t<
            is_l,
            // lvalue-qualified member functions
            ::std::conditional_t<
                (is_c && is_v), cv_l, ::std::conditional_t<is_c, c_l, ::std::conditional_t<is_v, v_l, un_l>>>,
            ::std::conditional_t<
                is_r,
                // rvalue-qualified member functions
                ::std::conditional_t<
                    (is_c && is_v), cv_r, ::std::conditional_t<is_c, c_r, ::std::conditional_t<is_v, v_r, un_r>>>,
                // no ref-qualifier
                ::std::conditional_t<
                    (is_c && is_v), cv, ::std::conditional_t<is_c, c, ::std::conditional_t<is_v, v, un>>>>>;
    };

    // Entry: Signature without noexcept
    template <typename Type, typename Result, typename... Arguments>
    struct member_function_like<Type, Result(Arguments...)>
        : ::scl::detail::build_member_function_like<
              Type, Result, ::scl::detail::member_function_noexcept::disabled, Arguments...>
    {};

    // Entry: Signature with noexcept
    template <typename Type, typename Result, typename... Arguments>
    struct member_function_like<Type, Result(Arguments...) noexcept>
        : ::scl::detail::build_member_function_like<
              Type, Result, ::scl::detail::member_function_noexcept::enabled, Arguments...>
    {};

    // Fallback for non-function Signature
    template <typename Type, typename Signature>
    struct member_function_like
    {
        static_assert(
            ::std::is_function_v<Signature>,
            "::scl::member_function_like_t requires a function type like Result(Arguments...) or Result(Arguments...) noexcept");
    };

    template <typename Type, typename Member>
    struct member_object_like
    {
    private:
        using class_t = ::std::remove_cvref_t<Type>;

        static_assert(
            ::std::is_class_v<class_t> || ::std::is_union_v<class_t>,
            "member_object_like_t requires Type to be a class or union type");
        static_assert(
            !::std::is_function_v<Member>,
            "member_object_like_t expects a data member type; use member_function_like_t for member functions");

        using no_ref_t = ::std::remove_reference_t<Type>;
        using member_like_c_t =
            ::std::conditional_t<::std::is_const_v<no_ref_t>, ::std::add_const_t<Member>, Member>;
        using member_like_cv_t = ::std::conditional_t<
            ::std::is_volatile_v<no_ref_t>, ::std::add_volatile_t<member_like_c_t>, member_like_c_t>;

    public:
        using type = member_like_cv_t(class_t::*);
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
