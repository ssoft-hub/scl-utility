#pragma once

#include <type_traits>

/**
 * @file
 * @brief Traits to build pointers to class members (C++20).
 * @ingroup scl_utility_type_traits
 * @details
 * - scl::member_function_like_t<Type, Signature>:
 *     Builds a pointer-to-member-function type:
 *       Result (::std::remove_cvref_t<Type>::*)(Arguments...) [cv] [ref] [noexcept]
 *     where [cv]/[ref] are mirrored from Type and [noexcept] is mirrored from Signature.
 * - scl::member_property_like_t<Type, Member>:
 *     Builds a pointer-to-data-member type:
 *       Member (::std::remove_cvref_t<Type>::*)
 *     Object cv/ref qualifiers do not affect the pointer type for data members.
 */

namespace scl::detail
{
    template <class Type, class Signature>
    struct member_function_like;

    enum class member_function_noexcept : bool
    {
        disabled = false,
        enabled = true
    };

    // Common builder parametrized by Noex (controls noexcept on the member function)
    template <class Type, class Result, member_function_noexcept Noexcept, class... Arguments>
    struct build_member_function_like
    {
    private:
        static_assert(::std::is_class_v<::std::remove_cvref_t<Type>>,
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
        using type = ::std::conditional_t<is_l,
            // lvalue-qualified member functions
            ::std::conditional_t<(is_c && is_v), cv_l, ::std::conditional_t<is_c, c_l, ::std::conditional_t<is_v, v_l, un_l>>>,
            ::std::conditional_t<is_r,
                // rvalue-qualified member functions
                ::std::conditional_t<(is_c && is_v), cv_r, ::std::conditional_t<is_c, c_r, ::std::conditional_t<is_v, v_r, un_r>>>,
                // no ref-qualifier
                ::std::conditional_t<(is_c && is_v), cv, ::std::conditional_t<is_c, c, ::std::conditional_t<is_v, v, un>>>>>;
    };

    template <class Type, class Result, class... Arguments>
    struct build_member_function_like<Type, Result, member_function_noexcept::enabled, Arguments...>
    {
    private:
        static_assert(::std::is_class_v<::std::remove_cvref_t<Type>>,
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
        using type = ::std::conditional_t<is_l,
            // lvalue-qualified member functions
            ::std::conditional_t<(is_c && is_v), cv_l, ::std::conditional_t<is_c, c_l, ::std::conditional_t<is_v, v_l, un_l>>>,
            ::std::conditional_t<is_r,
                // rvalue-qualified member functions
                ::std::conditional_t<(is_c && is_v), cv_r, ::std::conditional_t<is_c, c_r, ::std::conditional_t<is_v, v_r, un_r>>>,
                // no ref-qualifier
                ::std::conditional_t<(is_c && is_v), cv, ::std::conditional_t<is_c, c, ::std::conditional_t<is_v, v, un>>>>>;
    };

    // Entry: Signature without noexcept
    template <class Type, class Result, class... Arguments>
    struct member_function_like<Type, Result(Arguments...)>
        : build_member_function_like<Type, Result, member_function_noexcept::disabled, Arguments...>
    {};

    // Entry: Signature with noexcept
    template <class Type, class Result, class... Arguments>
    struct member_function_like<Type, Result(Arguments...) noexcept>
        : build_member_function_like<Type, Result, member_function_noexcept::enabled, Arguments...>
    {};

    // Fallback for non-function Signature
    template <class Type, class Signature>
    struct member_function_like
    {
        static_assert(::std::is_function_v<Signature>,
            "::scl::member_function_like_t requires a function type like Result(Arguments...) or Result(Arguments...) noexcept");
    };

    template <class Type, class Member>
    struct member_property_like
    {
    private:
        using class_t = ::std::remove_cvref_t<Type>;

        static_assert(::std::is_class_v<class_t> || ::std::is_union_v<class_t>,
            "member_property_like_t requires Type to be a class or union type");
        static_assert(!::std::is_function_v<Member>,
            "member_property_like_t expects a data member type; use member_function_like_t for member functions");

        using no_ref_t = ::std::remove_reference_t<Type>;
        using member_like_c_t =
            ::std::conditional_t<::std::is_const_v<no_ref_t>, ::std::add_const_t<Member>, Member>;
        using member_like_cv_t =
            ::std::conditional_t<::std::is_volatile_v<no_ref_t>, ::std::add_volatile_t<member_like_c_t>, member_like_c_t>;

    public:
        using type = member_like_cv_t(class_t::*);
    };

    template <class Type, class T, bool IsFunction = ::std::is_function_v<T>>
    struct member_like;

    template <class Type, class Signature>
    struct member_like<Type, Signature, true> : member_function_like<Type, Signature>
    {};

    template <class Type, class Member>
    struct member_like<Type, Member, false> : member_property_like<Type, Member>
    {};

} // namespace scl::detail

namespace scl
{
    /**
     * @brief Pointer-to-member-function type for class Type and function Signature,
     *        where the member function's cv/ref qualifiers are copied from Type.
     * @ingroup scl_utility_type_traits
     *
     * @tparam Type      Object type whose cv/ref qualifiers will be mirrored onto
     *                   the member function. The class type itself is decayed with
     *                   ::::std::remove_cvref_t before forming the pointer-to-member.
     * @tparam Signature Non-static member function signature as a function type,
     *                   e.g. `Result(Arguments...)` or `Result(Arguments...) noexcept`.
     *
     * @par Examples
     * @code{.cpp}
     *     struct Foo {};
     *     using P0 = scl::member_function_like_t<Foo, void(int)>;           // void (Foo::*)(int)
     *     using P1 = scl::member_function_like_t<Foo&, void(int)>;          // void (Foo::*)(int) &
     *     using P2 = scl::member_function_like_t<Foo const, void(int)>;     // void (Foo::*)(int) const
     *     using P3 = scl::member_function_like_t<Foo const&&, void() noexcept>; // void (Foo::*)() const && noexcept
     * @endcode
     */
    template <class Type, class Signature>
    using member_function_like_t = typename ::scl::detail::member_function_like<Type, Signature>::type;

    /**
     * @brief Pointer-to-data-member type for class Type and member type Member.
     * @ingroup scl_utility_type_traits
     *
     * @details
     * Forms the type `Member (::std::remove_cvref_t<Type>::*)`. Object cv/ref qualifiers
     * do not change the pointer type for data members; they only affect access expressions.
     *
     * @par Examples
     * @code{.cpp}
     *   using P0 = scl::member_property_like_t<Foo, int>;         // int Foo::*
     *   using P1 = scl::member_property_like_t<Foo const&, int>;  // int const & Foo::*
     *   using P2 = scl::member_property_like_t<Foo, int const>;   // int Foo::*
     * @endcode
     */
    template <class Type, class Member>
    using member_property_like_t = typename ::scl::detail::member_property_like<Type, Member>::type;

    /**
     * @brief Generic pointer-to-member type selector for class Type and Member.
     * @ingroup scl_utility_type_traits
     *
     * @details
     * Selects the appropriate pointer-to-member type based on whether Member is a function type.
     * - If Member is a function type like `Result(Arguments...)` or `Result(Arguments...) noexcept`,
     *   this aliases to scl::member_function_like_t<Type, Member>, producing:
     *     `Result (::std::remove_cvref_t<Type>::*)(Arguments...) [cv] [ref] [noexcept]`
     *   where:
     *   - [cv]/[ref] are mirrored from Type (const/volatile and &/&&),
     *   - [noexcept] is mirrored from T.
     * - Otherwise, it aliases to scl::member_property_like_t<Type, Member>, producing:
     *     `Member (::std::remove_cvref_t<Type>::*)`
     *   (see member_property_like_t for details on how object qualifiers may affect Member).
     *
     * @tparam Type   Class (object) type whose cv/ref qualifiers are mirrored for member functions.
     *                The class itself is decayed with ::std::remove_cvref_t before forming the pointer type.
     * @tparam Member Either a function type (for member functions) or a non-function type (for data members).
     *
     * @par Examples
     * @code{.cpp}
     *     struct Foo {};
     *
     *     // Function members
     *     using M0 = scl::member_like_t<Foo, void(int)>;              // void (Foo::*)(int)
     *     using M1 = scl::member_like_t<Foo const&, void() noexcept>; // void (Foo::*)() const & noexcept
     *
     *     // Data members
     *     using M2 = scl::member_like_t<Foo, int>;        // int Foo::*
     *     using M3 = scl::member_like_t<Foo const, int>;  // int const Foo::*
     * @endcode
     */
    template <class Type, class Member>
    using member_like_t = typename ::scl::detail::member_like<Type, Member>::type;
} // namespace scl
