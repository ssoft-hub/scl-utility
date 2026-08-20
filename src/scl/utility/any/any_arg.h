#pragma once

/**
 * @file any_arg.h
 * @brief Parameter-only view over a `std::any` or a typed value.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_anchor.h>
#include <scl/utility/any/any_view.h>
#include <scl/utility/attribute/hotcold.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/likely.h>
#include <scl/utility/preprocessor/exceptions.h>
#include <scl/utility/preprocessor/rtti.h>

#include <concepts>
#include <memory>
#include <type_traits>

#if SCL_HAS_RTTI || defined(DOXYGEN)
#include <any>
#endif

#include "detail/base.h"

namespace scl
{
    class any_argument;

    template <typename Type>
    [[nodiscard]]
    SCL_HOT constexpr Type * any_cast(any_argument const * arg) noexcept
        requires(::std::is_object_v<Type>);

    class any_argument : detail::any_base
    {
    public:
        using name = detail::any_base::name;

    private:
        using base_type = detail::any_base;

    public:
        // Passing an argument on binds another reference, so the only use left for a copy
        // is storing one, which the contract forbids.
        any_argument() = delete;
        any_argument(any_argument const &) = delete;
        any_argument(any_argument &&) = delete;
        any_argument & operator=(any_argument const &) = delete;
        any_argument & operator=(any_argument &&) = delete;
        constexpr ~any_argument() = default;

#if SCL_HAS_RTTI || defined(DOXYGEN)
        // std::any has no volatile-qualified members, so a volatile std::any is excluded
        // rather than bound as one nothing could later reach.
        template <typename Any>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_argument(Any && value SCL_LIFETIMEBOUND) noexcept
            requires(detail::is_std_any_v<::std::remove_cvref_t<Any>>) &&
            (!::std::is_volatile_v<::std::remove_reference_t<Any>>)
            : base_type{::std::addressof(value), &detail::any_type_descriptor_of<::std::remove_reference_t<Any> &>}
        {}
#endif

        // A view promises read access only, so the referent is adopted narrowed to it.
        // Not lifetime-bound: what is adopted is the referent, so the view may die first.
        // cppcheck-suppress noExplicitConstructor
        constexpr any_argument(any_view const & view) noexcept // NOLINT(*-explicit-*): adopts the referent
            : base_type{view, view.const_descriptor()}
        {}

        // Unwrapped to its content, bound with the constness of the any it was taken from.
        template <typename AnyType>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): binds, never forwards
        constexpr any_argument(AnyType && owner SCL_LIFETIMEBOUND) noexcept
            requires(::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<AnyType>>)
            : base_type{owner.viewed_held(), owner.viewed_object(),
                  ::std::is_const_v<::std::remove_reference_t<AnyType>>
                      ? owner.viewed_const_descriptor()
                      : owner.viewed_descriptor()}
        {}

        // The anchor is what lets an argument that is not a parameter answer during
        // constant evaluation: the default one below dies with the call that made it.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_argument(any_anchor<Type> const & bound SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            : base_type{bound.bound_object(), bound.bound_descriptor()}
        {}

        // Spelled once per branch rather than as one declaration with the anchor
        // conditioned inside it: the parameter list is what differs, and a reader should
        // see each form whole.
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_argument(Type && object SCL_LIFETIMEBOUND) noexcept
            requires(!detail::is_std_any_v<::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            // With the recovery constant-evaluable on its own, the binding is the plain one
            // a view makes, and every restriction the anchor imposed on where an `any_arg`
            // may sit goes with it.
            : base_type{::std::addressof(object), &detail::any_type_descriptor_of<::std::remove_reference_t<Type> &>}
        {}
#else
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_argument(Type && object SCL_LIFETIMEBOUND
#ifndef DOXYGEN
            // The anchor is a caller's temporary, so it cannot be created inside this
            // constructor; a default argument is what puts it in the caller's frame. It is
            // an implementation detail of constant evaluation, never something to pass.
            ,
            // Taken by rvalue reference and filled rather than moved from: bound to a const
            // reference the temporary would be a const object, and writing through one is
            // not a constant expression.
            // NOLINTNEXTLINE(cppcoreguidelines-rvalue-reference-param-not-moved)
            detail::any_anchored_descriptor<::std::remove_cvref_t<Type>> && anchor = detail::any_anchored_descriptor<
                ::std::remove_cvref_t<Type>>{detail::any_type_descriptor_of<::std::remove_reference_t<Type> &>}
#endif
            ) noexcept
            requires(!detail::is_std_any_v<::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            // Only constant evaluation needs the anchor, and only constant evaluation takes
            // it. The anchor dies with the full expression that made it, which spans the
            // call it was made for and nothing beyond; at run time the descriptor is the
            // static one, so an `any_arg` outside a parameter stays as valid as any other.
            : base_type{::std::addressof(object),
                  ::std::is_constant_evaluated()
                      ? static_cast<base_type::descriptor_type const *>(::std::addressof(anchor))
                      : &detail::any_type_descriptor_of<::std::remove_reference_t<Type> &>}
        {
            // The qualifiers dropped here are restored by any_cast, which hands out a
            // pointer only once binding_accepts() has proved the request covers them.
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
            anchor.referent = const_cast<::std::remove_cvref_t<Type> *>(::std::addressof(object));
        }
#endif

    public:
        using base_type::has_value;
        using base_type::type_key;
        using base_type::type_name;

    private:
        // Named rather than a conversion, and private: a view may be stored and an
        // argument may not, so a caller never obtains one from the other.
        [[nodiscard]]
        constexpr any_view as_view() const noexcept
        {
            return any_view{*this, const_descriptor()};
        }

        // An explicit object parameter in the base deduces this type, not the base, and
        // private inheritance would otherwise refuse that conversion.
        friend class ::scl::detail::any_base;
        friend struct ::scl::detail::any_handle_access;

        template <typename Type>
        friend constexpr Type * scl::any_cast(any_argument const * arg) noexcept
            requires(::std::is_object_v<Type>);
    };

    using any_arg = any_argument const &;

    template <typename Type>
    [[nodiscard]]
    SCL_HOT constexpr Type * any_cast(any_argument const * arg) noexcept
        requires(::std::is_object_v<Type>)
    {
        if (arg == nullptr)
            SCL_UNLIKELY return nullptr;
        if (!arg->binding_accepts(detail::any_qualifiers_of<Type &>()))
            return nullptr;
        if (::std::is_constant_evaluated())
        {
            // Both shapes are typed, so either may only be reached once the descriptor has
            // said which type it holds. A plain binding falls through to the recovery from
            // `void const *`, which folds from C++26 on.
            using bare = ::std::remove_cv_t<Type>;

            if (*arg->descriptor()->type == ::scl::type_key_of<bare>())
            {
                if (arg->descriptor()->binding == detail::any_binding::anchor)
                    return static_cast<detail::any_anchored_descriptor<bare> const *>(arg->descriptor())
                        ->referent;

                if (arg->descriptor()->binding == detail::any_binding::holder)
                    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): binding_accepts() covered it
                    return const_cast<bare *>(detail::any_holder_object<bare>(arg->held()));
            }
        }
        // Through the view, not object(): for the std::any backing the stored address is
        // the box, and a cast must reach what is inside it.
        any_view const view = arg->as_view();
        auto const * reached = ::scl::any_cast<Type const>(&view);
        if (reached == nullptr)
            return nullptr;
        return detail::erased_cast<Type>(reached);
    }

#if SCL_HAS_EXCEPTIONS || defined(DOXYGEN)
    // Constrained to the argument itself, not to any_base: a view converts to one
    // implicitly, and admitting that conversion would hand it write access.
    template <typename Type, typename LValueArgument>
    [[nodiscard]]
    constexpr Type any_cast(LValueArgument & arg SCL_LIFETIMEBOUND)
        requires(::std::same_as<::std::remove_cv_t<LValueArgument>, any_argument>) &&
        (::std::is_lvalue_reference_v<Type>) && (!::std::is_const_v<::std::remove_reference_t<Type>>)
    {
        auto * pointer = any_cast<::std::remove_reference_t<Type>>(&arg);
        if (pointer == nullptr)
            throw bad_any_cast{};
        return *pointer;
    }

    // The reading forms are spelled for an argument too, rather than left to the any_view
    // conversion: before P2738 (C++26) that conversion drops the anchor, and with it the
    // ability to answer during constant evaluation. Reading supplies the `const` itself,
    // as the view does.
    // Not lifetime-bound: the result is a copy, and it outlives the argument by design.
    template <typename Type, typename ValueArgument>
    [[nodiscard]]
    constexpr Type any_cast(ValueArgument & arg)
        requires(::std::same_as<::std::remove_cv_t<ValueArgument>, any_argument>) && (::std::is_object_v<Type>)
    {
        auto const * pointer = any_cast<Type const>(&arg);
        if (pointer == nullptr)
            throw bad_any_cast{};
        return *pointer;
    }

    template <typename Type, typename ConstLValueArgument>
    [[nodiscard]]
    constexpr Type any_cast(ConstLValueArgument & arg SCL_LIFETIMEBOUND)
        requires(::std::same_as<::std::remove_cv_t<ConstLValueArgument>, any_argument>) &&
        (::std::is_lvalue_reference_v<Type>) && (::std::is_const_v<::std::remove_reference_t<Type>>)
    {
        // remove_reference_t keeps the request's cv: T const volatile & must reach a
        // volatile referent.
        auto const * pointer = any_cast<::std::remove_reference_t<Type>>(&arg);
        if (pointer == nullptr)
            throw bad_any_cast{};
        return *pointer;
    }
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
 * @brief Parameter-only view over an object held in a `std::any`, a typed value,
 *        or an existing @ref scl::any_view
 *
 * An argument is the parameter-position companion of @ref scl::any_view — it binds
 * lvalues **and** rvalues of any constness, and is valid only for the duration
 * of the call it is passed into. Write the parameter as @ref scl::any_arg, which
 * names `any_argument const &`, to accept a heterogeneous argument — including a
 * temporary — at no allocation cost. Like a view it is two pointers wide: the
 * referent and its descriptor.
 *
 * The backings and identity queries are those of @ref scl::any_view — a typed
 * value forms the RTTI-free raw backing, a `std::any` forms the RTTI-gated
 * std::any backing, and an `any_view` contributes its referent (the argument
 * refers to the same object). There is no conversion the other way: a view may be
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
 * for an empty `std::any` or a referent adopted from an empty `any_view`.
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
 *       either, and the std::any backing and a referent adopted from an `any_view`
 *       keep run-time casts, as the view's are.
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
 * foo(std::any{text});                      // temporary std::any (RTTI builds)
 * @endcode
 *
 * @see scl::any_view — the storable companion
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
 * @fn scl::any_argument::any_argument(Any && value)
 * @brief Constructs a view over the object held in @p value without copying it.
 *
 * Binds a `std::any` of any constness — a temporary included, since the argument
 * outlives the call — and records that constness, so a non-`const` `std::any`
 * answers write requests for its boxed object. A `volatile std::any` does not
 * participate: `std::any` has no volatile-qualified members, so nothing could
 * reach the object afterwards.
 *
 * @tparam Any    Deduced (forwarding) reference type of the `std::any`.
 * @param  value  The `std::any` to view. Only available when RTTI is enabled.
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
 * @fn scl::any_argument::any_argument(any_view const & view)
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
 * @note @ref scl::any_cast through this binding is a run-time operation: the
 *       content is reached as `void *`, which no anchor can make
 *       constant-evaluable, exactly as for the `std::any` backing.
 */

/**
 * @fn scl::any_argument::any_argument(Type && object)
 * @brief Constructs a view over a typed value without copying it.
 *
 * Binds lvalues and rvalues of any constness — the parameter-only contract makes
 * a temporary safe, as it outlives the call — and excludes `std::any` and the
 * ScL Any views themselves. The object is referenced in place; its type is
 * captured for later @ref scl::any_cast.
 *
 * @tparam Type  Deduced (forwarding) reference type of the viewed value.
 * @param  object  The value to view.
 */

/**
 * @fn scl::any_argument::has_value() const
 * @brief Reports whether the argument refers to anything at all.
 * @return `false` for an argument that refers to nothing, one adopted from an
 *         empty `any_view` included; `true` otherwise, an argument over a
 *         `std::any` included regardless of what that `std::any` holds.
 */

/**
 * @fn scl::any_argument::type_name() const
 * @brief Returns the compile-time name of the viewed type.
 * @return `type_name<T>()` for the raw backing; `type_name<std::any>()` for the
 *         std::any backing, which names the backing rather than the boxed type.
 */

/**
 * @fn scl::any_argument::type_key() const
 * @brief Returns the identity key of the viewed type.
 * @return The @ref scl::type_key of the viewed type, answered by value;
 *         `scl::type_key{}` for an argument that adopted the referent of an
 *         empty @ref scl::any_view, exactly as @ref scl::any_view::type_key.
 */

/**
 * @fn scl::any_cast(any_argument const * arg)
 * @ingroup scl_utility_any
 * @brief Returns a `Type *` to the viewed object when the request is granted, else
 *        null.
 *
 * Unlike @ref scl::any_view, an argument takes the request literally: `Type` states
 * the qualification wanted and the result is `Type *`. So `any_cast<T>` asks to
 * write and `any_cast<T const>` asks to read, and either is granted only when the
 * request carries every cv-qualifier the referent was bound with. The handle adds
 * none of its own: it is `const` wherever an argument is reached, so folding it in
 * would refuse every write.
 *
 * @code
 * int *       w = scl::any_cast<int>(&arg);       // referent bound non-const
 * int const * r = scl::any_cast<int const>(&arg); // also a referent bound const
 * @endcode
 *
 * The std::any backing follows the same rule: a write reaches the boxed object when
 * the `std::any` itself was bound non-`const`. Naming `std::any` in the request
 * answers the box rather than what it holds, so `any_cast<std::any>(&arg)` over a
 * non-`const` binding hands out a `std::any *` and lets a callee replace the whole
 * box.
 *
 * @tparam Type  The requested object type with its qualification; a reference type
 *               is rejected.
 * @param  arg  The argument to read (may be null).
 * @return `Type *` to the viewed object on a type match whose qualifiers the request
 *         covers; `nullptr` otherwise, or on a null pointer. Never throws. The
 *         pointer refers to the viewed object, so it stays valid while that object
 *         lives.
 */

/**
 * @fn scl::any_cast(LValueArgument & arg)
 * @ingroup scl_utility_any
 * @brief Writes through the viewed object, or throws.
 *
 * Three forms share this shape, told apart by @p Type: this one takes a non-`const`
 * lvalue reference and writes through it, @ref scl::any_cast(ValueArgument & arg)
 * copies the value out, and @ref scl::any_cast(ConstLValueArgument & arg) binds it as a
 * `const` reference without copying. Reading supplies the `const` itself, exactly
 * as @ref scl::any_view does; writing is granted under the coverage rule stated on
 * the pointer form.
 *
 * The argument type is deduced and constrained to the argument itself: an `any_view`
 * converts to an argument implicitly, and admitting that conversion here would
 * hand write access to a view that promises none. Spelling the reading forms for
 * an argument rather than leaving them to that conversion is also what keeps them
 * usable in constant evaluation, which the conversion to a view gives up.
 *
 * @note All three are declared only where @ref SCL_HAS_EXCEPTIONS is `1`. A translation
 *       unit compiled without exceptions keeps the pointer form, which answers a failed
 *       request with `nullptr`.
 *
 * @tparam Type      The requested result type — a non-`const` lvalue reference.
 * @tparam Argument  Deduced; must be `scl::any_argument`.
 * @param  arg  The argument view to write through.
 * @return The viewed object as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match, or the request does
 *                            not cover the qualifiers of the referent or the handle.
 */

/**
 * @fn scl::any_cast(ValueArgument & arg)
 * @ingroup scl_utility_any
 * @brief Copies the viewed object out, or throws.
 *
 * The reading form for an object type. It supplies the `const` itself, exactly as
 * @ref scl::any_view does, and is spelled for an argument rather than left to the
 * conversion to a view — which before P2738 (C++26) drops the anchor and with it the
 * ability to answer during constant evaluation. The result is a copy and outlives
 * @p arg, so it is not lifetime-bound.
 *
 * @note Declared only where @ref SCL_HAS_EXCEPTIONS is `1`.
 *
 * @tparam Type      The requested result type — an object type.
 * @tparam Argument  Deduced; must be `scl::any_argument`.
 * @param  arg  The argument view to copy the value out of.
 * @return A copy of the viewed object, as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match, or the request does
 *                            not cover the qualifiers of the referent or the handle.
 */

/**
 * @fn scl::any_cast(ConstLValueArgument & arg)
 * @ingroup scl_utility_any
 * @brief Binds the viewed object as a `const` reference without copying, or throws.
 *
 * The reading form for a `const` lvalue reference, on the same terms as
 * @ref scl::any_cast(ValueArgument & arg). The reference refers to the viewed object,
 * so it stays valid while that object lives.
 *
 * @note Declared only where @ref SCL_HAS_EXCEPTIONS is `1`.
 *
 * @tparam Type      The requested result type — a `const` lvalue reference.
 * @tparam Argument  Deduced; must be `scl::any_argument`.
 * @param  arg  The argument view to bind to.
 * @return The viewed object as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match, or the request does
 *                            not cover the qualifiers of the referent or the handle.
 */
