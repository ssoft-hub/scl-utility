#pragma once

/// @file any_arg.h
/// @brief Parameter-only view over a `std::any` or a typed value.
/// @ingroup scl_utility_any

#include <scl/utility/any/any_view.h>
#include <scl/utility/attribute/hotcold.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/likely.h>
#include <scl/utility/preprocessor/rtti.h>

#include <concepts>
#include <memory>
#include <type_traits>

#if SCL_HAS_RTTI || defined(DOXYGEN)
#include <any>
#endif

#include "detail/base.h"

// P2738 (C++26) makes recovering a typed pointer from `void const *` a constant
// expression, which is the one thing the anchor below exists to work around.
// `__cpp_constexpr` reports it, spelled once here rather than compared against a bare
// 202306L at each of the branches that turn on it.
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202306L
#define SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST 1
#else
#define SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST 0
#endif

#if !SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
namespace scl::detail
{
    // Recovering a typed pointer from `void const *` is not a constant expression before
    // P2738 (C++26), while casting down to a derived class is, provided the object really
    // is that class. An anchor is that class: a descriptor that also carries the typed
    // pointer, created per binding at the call site, where it outlives the call it was
    // made for. Being a descriptor is what keeps the view two pointers wide - the one it
    // already spends on a descriptor does for both.
    template <typename Type>
    struct any_anchor : any_view_descriptor
    {
        // Spelled out rather than left to aggregate initialisation, which would also admit
        // a default-constructed anchor - one describing no type at all.
        constexpr explicit any_anchor(any_view_descriptor const & descriptor) noexcept
            : any_view_descriptor{descriptor}
        {}

        // Named for what it holds rather than `object`, which any_base already spells as
        // an accessor. A default argument cannot bind a non-const reference, so the
        // constructor reaches the anchor it was handed through `mutable`.
        mutable Type * referent = nullptr;
    };
} // namespace scl::detail
#endif

namespace scl
{
    class any_arg;

    // Wrapper is deduced so that the caller's own cv-qualification reaches accepts(): a
    // const handle cannot escalate to a write the handle itself does not carry.
    template <typename Type, typename Wrapper>
        requires ::std::is_object_v<Type> && ::std::same_as<::std::remove_cv_t<Wrapper>, any_arg>
    [[nodiscard]]
    SCL_HOT constexpr Type * any_cast(Wrapper * arg) noexcept;

    class any_arg : detail::any_base
    {
        using base_type = detail::any_base;

    public:
        using name = base_type::name;

        any_arg() = delete;
        constexpr any_arg(any_arg const &) noexcept = default;
        constexpr any_arg(any_arg &&) noexcept = default;
        any_arg & operator=(any_arg const &) = delete;
        any_arg & operator=(any_arg &&) = delete;
        ~any_arg() = default;

#if SCL_HAS_RTTI || defined(DOXYGEN)
        // std::any has no volatile-qualified members, so a volatile std::any is excluded
        // rather than bound as one nothing could later reach.
        template <typename Type>
            requires detail::is_std_any_v<::std::remove_cvref_t<Type>> &&
            (!::std::is_volatile_v<::std::remove_reference_t<Type>>)
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_arg(Type && value SCL_LIFETIMEBOUND) noexcept
            : base_type{::std::addressof(value), &detail::any_view_descriptor_of<::std::remove_reference_t<Type> &>}
        {}
#endif

        // A view promises read access only, so the referent is adopted narrowed to it.
        // cppcheck-suppress noExplicitConstructor
        constexpr any_arg(any_view const & view SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*): adopts the referent
            : base_type{view.object(), view.const_descriptor()}
        {}

        // Spelled once per branch rather than as one declaration with the anchor
        // conditioned inside it: the parameter list is what differs, and a reader should
        // see each form whole.
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
        template <typename Type>
            requires(!detail::is_std_any_v<::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>)
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_arg(Type && object SCL_LIFETIMEBOUND) noexcept
            // With the recovery constant-evaluable on its own, the binding is the plain one
            // a view makes, and every restriction the anchor imposed on where an `any_arg`
            // may sit goes with it.
            : base_type{::std::addressof(object), &detail::any_view_descriptor_of<::std::remove_reference_t<Type> &>}
        {}
#else
        template <typename Type>
            requires(!detail::is_std_any_v<::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>)
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(*-explicit-*,*-missing-std-forward): implicit view by design; binds, never forwards
        constexpr any_arg(Type && object SCL_LIFETIMEBOUND
#ifndef DOXYGEN
            // The anchor is a caller's temporary, so it cannot be created inside this
            // constructor; a default argument is what puts it in the caller's frame. It is
            // an implementation detail of constant evaluation, never something to pass.
            ,
            detail::any_anchor<::std::remove_cvref_t<Type>> const & anchor = detail::any_anchor<
                ::std::remove_cvref_t<Type>>{detail::any_view_descriptor_of<::std::remove_reference_t<Type> &>}
#endif
            ) noexcept
            // Only constant evaluation needs the anchor, and only constant evaluation takes
            // it. The anchor dies with the full expression that made it, which spans the
            // call it was made for and nothing beyond; at run time the descriptor is the
            // static one, so an `any_arg` outside a parameter stays as valid as any other.
            : base_type{::std::addressof(object),
                  ::std::is_constant_evaluated()
                      ? static_cast<base_type::descriptor_type const *>(::std::addressof(anchor))
                      : &detail::any_view_descriptor_of<::std::remove_reference_t<Type> &>}
        {
            // The qualifiers dropped here are restored by any_cast, which hands out a
            // pointer only once accepts() has proved the request covers them.
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
            anchor.referent = const_cast<::std::remove_cvref_t<Type> *>(::std::addressof(object));
        }
#endif

    public:
        // Spelled per handle qualification for the same reason as any_base's accessors: a
        // bare `const volatile` conversion would cost every caller constexpr-capability.
#ifdef __cpp_explicit_this_parameter
        template <typename Self>
        constexpr operator any_view(this Self && self SCL_LIFETIMEBOUND) noexcept
        {
            return any_view{self.object(), self.const_descriptor()};
        }
#else
        constexpr operator any_view() const noexcept SCL_LIFETIMEBOUND
        {
            return any_view{object(), const_descriptor()};
        }

        operator any_view() const volatile noexcept SCL_LIFETIMEBOUND
        {
            return any_view{object(), const_descriptor()};
        }
#endif

        using base_type::has_value;
        using base_type::type_key;
        using base_type::type_name;

    private:
        // An explicit object parameter in the base deduces this type, not the base, and
        // private inheritance would otherwise refuse that conversion.
        friend class detail::any_base;

        template <typename Type, typename Wrapper>
            requires ::std::is_object_v<Type> && ::std::same_as<::std::remove_cv_t<Wrapper>, any_arg>
        friend constexpr Type * any_cast(Wrapper * arg) noexcept;
    };

    template <typename Type, typename Wrapper>
        requires ::std::is_object_v<Type> && ::std::same_as<::std::remove_cv_t<Wrapper>, any_arg>
    [[nodiscard]]
    SCL_HOT constexpr Type * any_cast(Wrapper * arg) noexcept
    {
        if (arg == nullptr)
            SCL_UNLIKELY return nullptr;
        if (!arg->accepts(detail::any_qualifiers_of<Type &>()))
            return nullptr;
#if !SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
        if (::std::is_constant_evaluated())
        {
            // The anchor is typed, so it may only be reached once the descriptor has said
            // which type it holds. A type mismatch leaves the cast below to answer null;
            // a binding whose descriptor is not an anchor - a std::any backing, or a
            // referent adopted from an any_view - stops the evaluation on the downcast,
            // which is the honest outcome, since answering null would report a type
            // mismatch that did not happen.
            using bare = ::std::remove_cv_t<Type>;

            if (*arg->descriptor()->type == ::scl::type_key_of<bare>())
                return static_cast<detail::any_anchor<bare> const *>(arg->descriptor())->referent;
        }
#endif
        // Through the view, not object(): for the std::any backing the stored address is
        // the box, and a cast must reach what is inside it.
        any_view const view = *arg;
        auto const * reached = ::scl::any_cast<Type const>(&view);
        if (reached == nullptr)
            return nullptr;
        return detail::erased_cast<Type>(reached);
    }

    // Argument is deduced so `&arg` carries the handle's qualification into the pointer
    // form. Constrained to any_arg itself, not any_base: an any_view converts to any_arg
    // implicitly, and admitting that conversion here would hand write access to a view
    // that promises none.
    template <typename Type, typename Argument>
        requires ::std::same_as<::std::remove_cv_t<Argument>, any_arg> &&
        ::std::is_lvalue_reference_v<Type> && (!::std::is_const_v<::std::remove_reference_t<Type>>)
    [[nodiscard]]
    constexpr Type any_cast(Argument & arg SCL_LIFETIMEBOUND)
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
    template <typename Type, typename Argument>
        requires ::std::same_as<::std::remove_cv_t<Argument>, any_arg> && ::std::is_object_v<Type>
    [[nodiscard]]
    constexpr Type any_cast(Argument & arg)
    {
        auto const * pointer = any_cast<Type const>(&arg);
        if (pointer == nullptr)
            throw bad_any_cast{};
        return *pointer;
    }

    template <typename Type, typename Argument>
        requires ::std::same_as<::std::remove_cv_t<Argument>, any_arg> &&
        ::std::is_lvalue_reference_v<Type> && ::std::is_const_v<::std::remove_reference_t<Type>>
    [[nodiscard]]
    constexpr Type any_cast(Argument & arg SCL_LIFETIMEBOUND)
    {
        // remove_reference_t keeps the request's cv: T const volatile & must reach a
        // volatile referent.
        auto const * pointer = any_cast<::std::remove_reference_t<Type>>(&arg);
        if (pointer == nullptr)
            throw bad_any_cast{};
        return *pointer;
    }
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/// @class scl::any_arg
/// @ingroup scl_utility_any
/// @brief Parameter-only view over an object held in a `std::any`, a typed value,
///        or an existing @ref scl::any_view
///
/// `any_arg` is the parameter-position companion of @ref scl::any_view — it binds
/// lvalues **and** rvalues of any constness, and is valid only for the duration
/// of the call it is passed into. Use it as a by-value function parameter to
/// accept a heterogeneous argument — including a temporary — at no allocation
/// cost. Like a view it is two pointers wide — the referent and its descriptor —
/// and trivially copyable.
///
/// The backings and identity queries are those of @ref scl::any_view — a typed
/// value forms the RTTI-free raw backing, a `std::any` forms the RTTI-gated
/// std::any backing, and an `any_view` contributes its referent (the new `any_arg`
/// refers to the same object). An implicit conversion to `any_view` lets a callee
/// delegate onward.
///
/// @warning Migrate a parameter from `any_view` to `any_arg` by changing its type,
///          not by adding an `any_arg` overload alongside the existing `any_view`
///          one: both convert implicitly from the same typed lvalue, so a call
///          passing one becomes ambiguous between the two overloads.
///
/// Access is wider than a view's, and the difference is where the request's
/// qualification comes from. A view only reads, so its pointer cast always adds
/// `const` and yields `Type const *`. An argument takes the request as written:
/// `any_cast<T>(&arg)` yields `T *` and `any_cast<T const>(&arg)` yields `T const *`,
/// each granted only when the request covers the qualifiers the referent was bound
/// with — the same coverage rule, with writing simply carrying no qualifier at all.
/// The reference form spells the same choice with a reference type: `any_cast<T &>`
/// writes, `any_cast<T const &>` reads.
///
/// The coverage rule reaches the handle itself, not just the referent: `arg`'s own
/// `const` and/or `volatile` are qualifiers the request must also cover. A
/// `const any_arg` therefore cannot escalate to a write regardless of how unqualified
/// its referent is, and a `volatile any_arg` requires `volatile` in the request the
/// same way a `volatile` referent does.
///
/// A `const any_arg` thus reads exactly like an @ref scl::any_view, but it is not a
/// replacement for one: top-level `const` on a by-value parameter is not part of the
/// function type, so a read-only contract spelled that way is invisible to the
/// caller, and a `const any_arg` still binds a temporary where the view's deleted
/// rvalue constructors are a compile error on every compiler.
///
/// Write access is unreachable through an `any_view` — the implicit `any_view` to
/// `any_arg` conversion does not open it, so the view stays read-only.
///
/// Storage is discouraged at the API level: there is no default constructor and
/// no assignment. The copy constructor remains, as by-value parameter passing
/// requires it. The deleted default constructor is not an emptiness guarantee —
/// `has_value()` still reports `false` for an empty `std::any` or a referent
/// adopted from an empty `any_view` — it rules out the one value nothing could
/// ever fill: with no assignment, a default-constructed `any_arg` would stay
/// empty forever.
///
/// Unlike a view's, an argument's casts are constant-evaluable on the C++20 baseline,
/// where a view's wait for P2738 (C++26). The cast reaches the referent through an
/// object the caller materialises per binding, so the capability is bounded by the
/// lifetime an argument already has, and it costs no width: that object is a
/// descriptor too, and rides in the pointer a view already spends on one.
///
/// @note Constant evaluation therefore answers for an argument that is a *parameter*.
///       For an `any_arg` that is anything else — a local variable, a member, an
///       element of an aggregate — a cast, and an identity query with it, stops during
///       constant evaluation with a diagnostic rather than answering wrongly. There is
///       no `constexpr any_arg` *variable* either, and the std::any backing and a
///       referent adopted from an `any_view` keep run-time casts, as the view's are.
///
/// @note None of that reaches run time, where the descriptor is the static one: every
///       position works, every query answers, and the cost is a view's.
///
/// @note Nor does any of it outlive the baseline that forced it. Where `__cpp_constexpr`
///       reports P2738 (C++26), the header declares the plain binding a view makes, the
///       mechanism is not compiled at all, and every restriction listed above lifts:
///       a cast then folds for an `any_arg` in any position, over a referent adopted
///       from an `any_view`, and at namespace scope on every compiler. Nothing has to
///       change at the call site, and neither the width nor the layout of this type
///       depends on the standard — only the declaration of a constructor parameter no
///       caller ever passes, so a binary must still be built against one `-std`
///       throughout, as one built against a standard library is.
///
/// @warning An `any_arg` does not own or extend the lifetime of its referent.
///          Bound to a temporary, the referent dies at the end of the full
///          expression containing the call — keeping the `any_arg` (or an
///          `any_view` converted from it) beyond the call dangles. Constructor
///          parameters are lifetime-bound (@ref SCL_LIFETIMEBOUND), so Clang
///          reports an `any_arg` escaping the scope of its referent directly
///          (a local variable, a return statement). That diagnostic does not
///          reach through an intervening container — `std::vector<any_arg>`,
///          `std::optional<any_arg>` — so storing one there compiles silently
///          and dangles the moment the original argument's storage is reused.
///          To keep the value, copy it out with @ref scl::any_cast.
///
/// @par Example
/// @code
/// void foo(scl::any_arg value)
/// {
///     if (auto const * text = scl::any_cast<std::string>(&value))
///         use(*text);                       // no copy
///     inner(value);                         // delegate to void inner(scl::any_view)
/// }
///
/// std::string text{"Hello Any!"};
/// foo(text);                                // lvalue
/// foo(std::string{"temporary"});            // rvalue — outlives the call, OK
/// foo(std::any{text});                      // temporary std::any (RTTI builds)
/// @endcode

/// @typedef scl::any_arg::name
/// @brief Type-name string produced by @ref scl::any_arg::type_name.

/// @fn scl::any_arg::any_arg(Type && value)
/// @brief Constructs a view over the object held in @p value without copying it.
///
/// Binds a `std::any` of any constness — a temporary included, since the argument
/// outlives the call — and records that constness, so a non-`const` `std::any`
/// answers write requests for its boxed object. A `volatile std::any` does not
/// participate: `std::any` has no volatile-qualified members, so nothing could
/// reach the object afterwards.
///
/// @tparam Type  Deduced (forwarding) reference type of the `std::any`.
/// @param  value  The `std::any` to view. Only available when RTTI is enabled.

/// @fn scl::any_arg::any_arg(any_view const & view)
/// @brief Adopts the referent of an existing view.
///
/// A view promises read access only, so the referent is adopted narrowed to it:
/// write requests are refused however the object was originally bound.
///
/// @param view  The view whose referent to adopt; the `any_arg` refers to the
///              same object, not to the view.

/// @fn scl::any_arg::any_arg(Type && object)
/// @brief Constructs a view over a typed value without copying it.
///
/// Binds lvalues and rvalues of any constness — the parameter-only contract makes
/// a temporary safe, as it outlives the call — and excludes `std::any` and the
/// ScL Any views themselves. The object is referenced in place; its type is
/// captured for later @ref scl::any_cast.
///
/// @tparam Type  Deduced (forwarding) reference type of the viewed value.
/// @param  object  The value to view.

/// @fn scl::any_arg::operator any_view() const
/// @brief Converts to an @ref scl::any_view over the same referent, so a callee
///        can delegate onward.
///
/// The view receives the `const`-qualified form of the binding, so write access
/// does not survive the round trip through a view.

/// @fn scl::any_arg::has_value() const
/// @brief Reports whether the view refers to a live value.
/// @return `false` for a view over an empty `std::any` or an argument adopted
///         from an empty `any_view`; `true` otherwise.

/// @fn scl::any_arg::type_name() const
/// @brief Returns the compile-time name of the viewed type.
/// @return `type_name<T>()` for the raw backing; `type_name<std::any>()` for the
///         std::any backing, which names the backing rather than the boxed type.

/// @fn scl::any_arg::type_key() const
/// @brief Returns the identity key of the viewed type.
/// @return The @ref scl::type_key of the viewed type — the very object
///         @ref scl::type_key_of returns for it, so comparison is a pointer
///         comparison; `nullptr` for an argument that adopted the referent of an
///         empty @ref scl::any_view, exactly as @ref scl::any_view::type_key.

/// @fn scl::any_cast(Wrapper * arg)
/// @ingroup scl_utility_any
/// @brief Returns a `Type *` to the viewed object when the request is granted, else
///        null.
///
/// Unlike @ref scl::any_view, an argument takes the request literally: `Type` states
/// the qualification wanted and the result is `Type *`. So `any_cast<T>` asks to
/// write and `any_cast<T const>` asks to read, and either is granted only when the
/// request carries every cv-qualifier the referent was bound with — *and* every
/// cv-qualifier of `arg` itself. A `const any_arg` cannot escalate to a write the
/// handle does not carry, regardless of how unqualified the referent is; a
/// `volatile any_arg` likewise requires `Type` to carry `volatile`.
///
/// @code
/// int *       w = scl::any_cast<int>(&arg);       // writable referent, non-const handle
/// int const * r = scl::any_cast<int const>(&arg); // also a const referent or handle
/// @endcode
///
/// The std::any backing follows the same rule: a write reaches the boxed object when
/// the `std::any` itself was bound non-`const`.
///
/// @tparam Type     The requested object type with its qualification; a reference
///                  type is rejected.
/// @tparam Wrapper  Deduced `any_arg`, possibly `const`- and/or `volatile`-qualified —
///                  that qualification is itself a requirement the request must cover.
/// @param  arg  The argument view to read (may be null).
/// @return `Type *` to the viewed object on a type match whose qualifiers the request
///         covers; `nullptr` otherwise, or on a null pointer. Never throws. The
///         pointer refers to the viewed object, so it stays valid while that object
///         lives.

/// @fn scl::any_cast(Argument & arg)
/// @ingroup scl_utility_any
/// @brief Returns the viewed object by value, by `const` reference, or by mutable
///        reference, or throws.
///
/// Three forms share this shape, told apart by @p Type: an object type copies the
/// value out, a `const` lvalue reference binds it without copying, and a non-`const`
/// lvalue reference writes through it. Reading supplies the `const` itself, exactly
/// as @ref scl::any_view does; writing is granted under the coverage rule stated on
/// the pointer form.
///
/// The argument type is deduced and constrained to `any_arg` itself: an `any_view`
/// converts to an `any_arg` implicitly, and admitting that conversion here would
/// hand write access to a view that promises none. Spelling the reading forms for
/// `any_arg` rather than leaving them to that conversion is also what keeps them
/// usable in constant evaluation, which the conversion to a view gives up.
///
/// @tparam Type      The requested result type — an object type, a `const` lvalue
///                   reference, or a non-`const` lvalue reference.
/// @tparam Argument  Deduced; must be `scl::any_arg`.
/// @param  arg  The argument view to read or write through.
/// @return The viewed object as @p Type.
/// @throws scl::bad_any_cast  If the viewed type does not match, or the request does
///                            not cover the qualifiers of the referent or the handle.
