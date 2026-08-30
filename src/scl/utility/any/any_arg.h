#pragma once

/**
 * @file any_arg.h
 * @brief Parameter-only view over a typed value.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_anchor.h>
#include <scl/utility/any/any_cast.h>
#include <scl/utility/attribute/lifetimebound.h>

#include <memory>
#include <type_traits>

#include "detail/base.h"

namespace scl
{
    // Declared, not defined: a template body reaches a view only where a caller has one.
    class any_argument;
    class any_mutable_view;
    class any_view;

    class any_argument : detail::any_base
    {
    public:
        using name = detail::any_base::name;

    private:
        using base_type = detail::any_base;

    public:
        // Passing an argument on binds another reference; a copy would only be for storing it.
        any_argument() = delete;
        any_argument(any_argument const &) = delete;
        any_argument(any_argument &&) = delete;
        any_argument & operator=(any_argument const &) = delete;
        any_argument & operator=(any_argument &&) = delete;
        constexpr ~any_argument() = default;

        // One template per hierarchy: a writing view must not adopt through the reading form.

        // Not lifetime-bound: the referent is adopted, narrowed to reading, so the view may die.
        template <typename View>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_argument(View const & view) noexcept // NOLINT(*-explicit-*): adopts the referent
            requires(::std::is_base_of_v<any_view, View>) &&
            (::std::is_convertible_v<View const volatile *, any_view const volatile *>) &&
            (!::std::is_volatile_v<View>)
            : base_type{view, view.const_descriptor()}
        {}

        // The write survives because the binding is adopted as it stands.
        template <typename MutableView>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_argument(MutableView const & view) noexcept // NOLINT(*-explicit-*)
            requires(::std::is_base_of_v<any_mutable_view, MutableView>) &&
            (::std::is_convertible_v<MutableView const volatile *, any_mutable_view const volatile *>) &&
            (!::std::is_volatile_v<MutableView>)
            : base_type{view, view.descriptor()}
        {}

        // Unwrapped to its content, bound with the constness of the any it came from.
        template <typename AnyType>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): binds, never forwards
        constexpr any_argument(AnyType && owner SCL_LIFETIMEBOUND) noexcept
            requires(::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<AnyType>>)
            : base_type{owner.viewed_held(), owner.viewed_object(),
                  ::std::is_const_v<::std::remove_reference_t<AnyType>>
                      ? owner.viewed_const_descriptor()
                      : owner.viewed_descriptor()}
        {}

        // The anchor below is what lets an argument outside a parameter answer at compile time.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_argument(any_anchor<Type> const & bound SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            : base_type{bound.bound_object(), bound.bound_descriptor()}
        {}

        // Once per branch, not one declaration: the parameter list is what differs.
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_argument(Type && object SCL_LIFETIMEBOUND) noexcept
            requires(!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            // The recovery folds on its own here, so the anchor and its restrictions go.
            : base_type{::std::addressof(object), &detail::any_type_descriptor_of<::std::remove_reference_t<Type> &>}
        {}
#else
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_argument(Type && object SCL_LIFETIMEBOUND
#ifndef DOXYGEN
            // A default argument is what puts the anchor in the caller's frame; never passed.
            ,
            // Filled, not moved from: writing through a const temporary is no constant expression.
            // NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved)
            detail::any_anchored_descriptor<::std::remove_cvref_t<Type>> && anchor = detail::any_anchored_descriptor<
                ::std::remove_cvref_t<Type>>{detail::any_type_descriptor_of<::std::remove_reference_t<Type> &>}
#endif
            ) noexcept
            requires(!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            // The anchor dies with the full expression, which spans the call it was made for.
            : base_type{::std::addressof(object),
                  ::std::is_constant_evaluated()
                      ? static_cast<base_type::descriptor_type const *>(::std::addressof(anchor))
                      : &detail::any_type_descriptor_of<::std::remove_reference_t<Type> &>}
        {
            // The qualifiers dropped here are restored by any_cast, once it proves them covered.
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
            anchor.referent = const_cast<::std::remove_cvref_t<Type> *>(::std::addressof(object));
        }
#endif

    public:
        using base_type::has_value;
        using base_type::type_key;
        using base_type::type_name;

    private:
        // An explicit object parameter deduces this type, which private inheritance refuses.
        friend class ::scl::detail::any_base;
        friend struct ::scl::detail::any_handle_access;
    };

    using any_arg = any_argument const &;

    // The request carries what the caller spelled: bare `Type` writes, `Type const` reads.
    // Guarded: every specialisation implements the contract the primary template documents.
#ifndef DOXYGEN
    template <>
    struct any_cast_traits<any_argument>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr Target * access(Source * arg) noexcept
        {
            return detail::any_referent_of<Target>(*arg);
        }
    };
#endif

} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef DOXYGEN
namespace scl
{
    class any_argument
    {
    public:
        constexpr bool has_value() const noexcept;
        constexpr ::scl::type_key type_key() const noexcept;
        constexpr name type_name() const noexcept;
    };
} // namespace scl
#endif

// =============================================================================
// Documentation
// =============================================================================

/**
 * @class scl::any_argument
 * @ingroup scl_utility_any
 * @brief Parameter-only view over a typed value or an existing @ref scl::any_view
 *
 * An argument is the parameter-position companion of @ref scl::any_view — it binds
 * lvalues **and** rvalues of any constness, and is valid only for the duration
 * of the call it is passed into. Write the parameter as @ref scl::any_arg, which
 * names `any_argument const &`, to accept a heterogeneous argument — including a
 * temporary — at no allocation cost. Like a view it is two pointers wide: the
 * referent and its descriptor.
 *
 * What it binds and how it answers identity are those of @ref scl::any_view — a
 * typed value of any type, a container named as the container it is, while an
 * `any_view` contributes its referent (the argument refers to the same object).
 * There is no conversion the other way: a view may be
 * stored and an argument may not, so one is never obtained from the other. A callee
 * delegates by passing its own argument on, which binds another reference to it.
 *
 * @warning Migrate a parameter from `any_view` to `any_arg` by changing its type,
 *          not by adding an `any_arg` overload alongside the existing `any_view`
 *          one: both convert implicitly from the same typed lvalue, so a call
 *          passing one becomes ambiguous between the two overloads.
 *
 * Access is wider than a view's, and the difference is where the request's
 * qualification comes from. A view only reads, so its pointer cast always adds
 * `const` and yields `Type const *`. An argument takes the request as written:
 * `any_cast<T>(&arg)` yields `T *` and `any_cast<T const>(&arg)` yields `T const *`,
 * each granted only when the request covers the qualifiers the referent was bound
 * with — the same coverage rule, with writing simply carrying no qualifier at all.
 * The reference form spells the same choice with a reference type: `any_cast<T &>`
 * writes, `any_cast<T const &>` reads.
 *
 * The rights come from the binding and from nothing else. The handle is `const`
 * wherever an argument is reached, since the parameter is a reference, so its own
 * qualification decides nothing; only the qualifiers the referent was bound with
 * are matched against the request. Write access is therefore unreachable through
 * an @ref scl::any_view, whose referent is adopted narrowed to reading.
 *
 * @note Writing `scl::any_arg const` or `scl::any_arg volatile` narrows nothing:
 *       a cv-qualifier applied to a name that already stands for a reference is
 *       ignored, and Clang reports it as `-Wignored-reference-qualifiers`. A callee
 *       that means to read only takes an @ref scl::any_view.
 *
 * An argument cannot be stored, and the declaration is what refuses it: the class
 * has no default constructor, no assignment, and neither a copy nor a move
 * constructor, so `std::vector<any_arg>`, `std::optional<any_arg>` and
 * `auto copy = argument;` are each a compile error. Nothing needs the copy — passing
 * an argument on binds another reference to the same object. The deleted default
 * constructor is not an emptiness guarantee: `has_value()` still reports `false`
 * for a referent adopted from an empty `any_view`.
 *
 * Unlike a view's, an argument's casts are constant-evaluable on the C++20 baseline,
 * where a view's wait for P2738 (C++26). The cast reaches the referent through an
 * object the caller materialises per binding, so the capability is bounded by the
 * lifetime an argument already has, and it costs no width: that object is a
 * descriptor too, and rides in the pointer a view already spends on one.
 *
 * @note Constant evaluation therefore answers for an argument that is a *parameter*.
 *       For a local `scl::any_arg` bound to an argument built on the spot, a cast, and
 *       an identity query with it, stops during constant evaluation with a diagnostic
 *       rather than answering wrongly. There is no `constexpr` argument *variable*
 *       either, and a referent adopted from an `any_view` keeps run-time casts,
 *       as the view's are.
 *
 * @note None of that reaches run time, where the descriptor is the static one: every
 *       position works, every query answers, and the cost is a view's.
 *
 * @note Nor does any of it outlive the baseline that forced it. Where `__cpp_constexpr`
 *       reports P2738 (C++26), the header declares the plain binding a view makes, the
 *       mechanism is not compiled at all, and every restriction listed above lifts:
 *       a cast then folds for an `any_arg` in any position, over a referent adopted
 *       from an `any_view`, and at namespace scope on every compiler. Nothing has to
 *       change at the call site, and neither the width nor the layout of this type
 *       depends on the standard — only the declaration of a constructor parameter no
 *       caller ever passes, so a binary must still be built against one `-std`
 *       throughout, as one built against a standard library is.
 *
 * @warning An argument does not own or extend the lifetime of its referent. Bound
 *          to a temporary, the referent dies at the end of the full expression
 *          containing the call. Constructor parameters are lifetime-bound
 *          (@ref SCL_LIFETIMEBOUND), so Clang reports an argument escaping the
 *          scope of its referent directly — into a local variable, or through a
 *          return statement. Two shapes stay outside what the type can refuse: a
 *          reference data member, `any_argument const & kept;`, and a function
 *          returning `scl::any_arg`. Both are the hazards of any C++ reference. To
 *          keep the value, copy it out with @ref scl::any_cast.
 *
 * @par Example
 * @code
 * void foo(scl::any_arg value)
 * {
 *     if (auto const * text = scl::any_cast<std::string>(&value))
 *         use(*text);                       // no copy
 *     inner(value);                         // delegate to void inner(scl::any_arg)
 * }
 *
 * std::string text{"Hello Any!"};
 * foo(text);                                // lvalue
 * foo(std::string{"temporary"});            // rvalue — outlives the call, OK
 * foo(42);                                  // a temporary of any type at all
 * @endcode
 *
 * @see scl::any_view — the storable companion
 * @see scl::any_mutable_view — the storable companion that also writes
 * @see scl::any_switch — a branch chain over the same subject, one branch per type
 */

/**
 * @typedef scl::any_arg
 * @ingroup scl_utility_any
 * @brief What a parameter taking @ref scl::any_argument is written as:
 *        `any_argument const &`.
 *
 * A reference is what turns the parameter-only contract into a rule of the
 * language. A container of references is ill-formed, so `std::vector<any_arg>`
 * and `std::optional<any_arg>` do not compile, and the class it names refuses the
 * copy that would otherwise reach a data member or an `auto` variable.
 *
 * @note A cv-qualifier written on this name is ignored, as on any name standing
 *       for a reference: `scl::any_arg const` is `scl::any_arg`.
 */

/**
 * @typedef scl::any_argument::name
 * @brief Type-name string produced by @ref scl::any_argument::type_name.
 */

/**
 * @fn scl::any_argument::any_argument()
 * @brief Deleted: an argument always refers to something, so there is no empty
 *        state to default-construct into.
 */

/**
 * @fn scl::any_argument::any_argument(any_argument const &)
 * @brief Deleted: a copy is what would let an argument into a container, a data
 *        member or an `auto` variable, and passing one on needs no copy — it binds
 *        another reference to the same object.
 */

/**
 * @fn scl::any_argument::any_argument(any_argument &&)
 * @brief Deleted, for the same reason as the copy constructor.
 */

/**
 * @fn scl::any_argument::operator=(any_argument const &)
 * @brief Deleted: rebinding would outlive the call the original binding was
 *        made for, which is the one thing the parameter-only contract forbids.
 */

/**
 * @fn scl::any_argument::operator=(any_argument &&)
 * @brief Deleted, for the same reason as copy assignment.
 */

/**
 * @fn scl::any_argument::~any_argument()
 * @brief Trivial: the view owns neither the referent nor the descriptor.
 */

/**
 * @fn scl::any_argument::any_argument(any_anchor<Type> const & bound)
 * @brief Binds the argument to the object @p bound stands for.
 *
 * The referent is the anchored object, never the anchor, and the write access an
 * argument grants follows the object's own qualifiers. An anchor is what lets an
 * argument answer during constant evaluation where it is not a parameter — see
 * @ref scl::any_anchor.
 *
 * @tparam Type  The anchored object's type, qualifiers included.
 * @param  bound  The anchor standing for the object to bind.
 */

/**
 * @fn scl::any_argument::any_argument(View const & view)
 * @tparam View  Deduced @ref scl::any_view or a class publicly derived from it.
 * @brief Adopts the referent of an existing view.
 *
 * A view promises read access only, so the referent is adopted narrowed to it:
 * write requests are refused however the object was originally bound.
 *
 * The parameter is the one constructor parameter here that is not lifetime-bound
 * (@ref SCL_LIFETIMEBOUND). What is adopted is the referent and a static
 * descriptor, so the argument may outlive the view it was built from; a temporary
 * view is a valid source, and what has to stay alive is the object that view
 * refers to.
 *
 * @param view  The view whose referent to adopt; the argument refers to the
 *              same object, not to the view.
 */

/**
 * @fn scl::any_argument::any_argument(MutableView const & view)
 * @tparam MutableView  Deduced @ref scl::any_mutable_view or a class publicly derived
 *                      from it.
 * @brief Adopts the referent of @ref scl::any_mutable_view, with its binding untouched.
 *
 * That view promises the write, so unlike the @ref scl::any_view form this
 * one narrows nothing: a request to write is granted exactly where the view
 * would grant it.
 *
 * The parameter is not lifetime-bound (@ref SCL_LIFETIMEBOUND) for the reason the
 * `any_view` form is not: what is adopted is the referent and a static
 * descriptor, so the argument may outlive the view it was built from, and what
 * has to stay alive is the object that view refers to.
 *
 * @param view  The view whose referent to adopt; the argument refers to the
 *              same object, not to the view.
 */

/**
 * @fn scl::any_argument::any_argument(AnyType && owner)
 * @brief Binds the object an owning any holds, rather than the any itself.
 *
 * The binding records the constness of the any it was made from: a non-`const`
 * any is bound for writing, a `const` one for reading only, the same rule every
 * other binding here obeys.
 *
 * @tparam AnyType  Deduced @ref scl::basic_any specialization, with its own
 *                  cv-ref qualification.
 * @param  owner  The any whose content is bound.
 *
 * @note A binding made from an owner carries that owner's own holder, which a cast
 *       reaches by a downcast, so it folds during constant evaluation without an
 *       anchor. A binding over a plain lvalue reaches its object as `void *`, which
 *       needs the anchor and P2738 (C++26).
 */

/**
 * @fn scl::any_argument::any_argument(Type && object)
 * @brief Constructs a view over a typed value without copying it.
 *
 * Binds lvalues and rvalues of any constness — the parameter-only contract makes
 * a temporary safe, as it outlives the call — and excludes the ScL Any handles
 * and owners, each of which has a constructor of its own. The object is referenced in place; its type is
 * captured for later @ref scl::any_cast.
 *
 * @tparam Type  Deduced (forwarding) reference type of the viewed value.
 * @param  object  The value to view.
 */

/**
 * @fn scl::any_argument::has_value() const
 * @brief Reports whether the argument refers to anything at all.
 * @return `false` for an argument that refers to nothing, one adopted from an
 *         empty `any_view` included; `true` otherwise, even where the object it
 *         refers to is itself empty.
 */

/**
 * @fn scl::any_argument::type_name() const
 * @brief Returns the compile-time name of the viewed type.
 * @return `type_name<T>()` for the bound type, which for a container is the
 *         container and not what it holds.
 */

/**
 * @fn scl::any_argument::type_key() const
 * @brief Returns the identity key of the viewed type.
 * @return The @ref scl::type_key of the viewed type, answered by value;
 *         `scl::type_key{}` for an argument that adopted the referent of an
 *         empty @ref scl::any_view, exactly as @ref scl::any_view::type_key.
 */
