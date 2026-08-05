#pragma once

/**
 * @file any_view.h
 * @brief Non-owning, read-only view over a `std::any` or a typed lvalue.
 * @ingroup scl_utility_any
 */

#include <scl/utility/attribute/hotcold.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/likely.h>
#include <scl/utility/preprocessor/rtti.h>

#include <memory>
#include <type_traits>

#if SCL_HAS_RTTI || defined(DOXYGEN)
#include <any>
#endif
#include <typeinfo>

#include "detail/base.h"

namespace scl
{
    class any_view;

    // Never std::bad_any_cast under RTTI: a base type that depends on SCL_HAS_RTTI
    // is an ODR trap for a binary linking RTTI and -fno-rtti translation units.
    struct bad_any_cast : ::std::bad_cast
    {};

    class any_view : detail::any_base
    {
        using base_type = detail::any_base;

    public:
        using name = base_type::name;

    public:
        constexpr any_view() noexcept = default;

#if SCL_HAS_RTTI || defined(DOXYGEN)
        // cppcheck-suppress noExplicitConstructor
        constexpr any_view(::std::any const & value SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            : base_type{::std::addressof(value), &detail::any_view_descriptor_of<::std::any const &>}
        {}

        any_view(::std::any const &&) = delete;
#endif

        template <typename Type>
            requires(!detail::is_std_any_v<::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>)
        // cppcheck-suppress noExplicitConstructor
        constexpr any_view(Type & object SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*): implicit view by design
            : base_type{::std::addressof(object), &detail::any_view_descriptor_of<Type &>}
        {}

        template <typename Type>
            requires(!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>)
        any_view(Type const &&) = delete;

    public:
        using base_type::has_value;
        using base_type::type_key;
        using base_type::type_name;

    private:
        // any_arg reaches this to hand over a referent it already narrowed with
        // any_base::const_descriptor.
        constexpr explicit any_view(void const volatile * object, base_type::descriptor_type const * descriptor) noexcept
            : base_type{object, descriptor}
        {}

        friend class any_arg;

        // An explicit object parameter in the base deduces this type, not the base, and
        // private inheritance would otherwise refuse that conversion.
        friend class detail::any_base;

        template <typename Type, typename View>
            requires(::std::is_object_v<Type>) && (::std::same_as<::std::remove_cv_t<View>, any_view>)
        friend constexpr Type const * any_cast(View * view) noexcept;
    };

    // View is deduced so that the caller's own cv-qualification reaches accepts().
    template <typename Type, typename View>
        requires(::std::is_object_v<Type>) && (::std::same_as<::std::remove_cv_t<View>, any_view>)
    [[nodiscard]]
    SCL_HOT constexpr Type const * any_cast(View * view) noexcept
    {
        using bare = ::std::remove_cvref_t<Type>;

        auto const * descriptor = (view != nullptr) ? view->descriptor() : nullptr;
        if (descriptor == nullptr)
            SCL_UNLIKELY return nullptr;
        // Ahead of the std::any branch, which delegates to std::any_cast and so knows
        // nothing of this view's qualifiers.
        if (!view->accepts(detail::any_qualifiers_of<Type const &>()))
            return nullptr;
#if SCL_HAS_RTTI
        if (auto const * boxed = view->std_any())
            return ::std::any_cast<bare>(boxed);
#endif
        if (*descriptor->type == ::scl::type_key_of<bare>())
            SCL_LIKELY return detail::erased_cast<Type const>(view->object());
        return nullptr;
    }

    // Not deduced: this form must keep admitting an implicit conversion, which template
    // deduction does not consider. A volatile handle cannot bind here — use the pointer form.
    template <typename Type>
        requires(::std::is_object_v<Type> ||
            (::std::is_lvalue_reference_v<Type> && ::std::is_const_v<::std::remove_reference_t<Type>>))
    [[nodiscard]]
    constexpr Type any_cast(any_view const & view)
    {
        // remove_reference_t keeps the request's cv: T const volatile & must reach a volatile referent.
        auto const * pointer = any_cast<::std::remove_reference_t<Type>>(&view);
        if (pointer == nullptr)
            throw bad_any_cast{};
        return static_cast<Type>(*pointer);
    }
} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef DOXYGEN
namespace scl
{
    class any_view
    {
    public:
        constexpr bool has_value() const noexcept;
        constexpr ::scl::type_key const * type_key() const noexcept;
        constexpr name type_name() const noexcept;
    };
} // namespace scl
#endif

// =============================================================================
// Documentation
// =============================================================================

/**
 * @struct scl::bad_any_cast
 * @ingroup scl_utility_any
 * @brief Exception thrown by the throwing form of @ref scl::any_cast on a type
 *        mismatch
 *
 * Derives from `std::bad_cast` in every configuration — never from
 * `std::bad_any_cast`, so this class stays a single, RTTI-independent type and
 * a `catch (std::bad_cast const &)` handler catches it regardless of build.
 */

/**
 * @class scl::any_view
 * @ingroup scl_utility_any
 * @brief Non-owning, read-only view over an object held in a `std::any` or a
 *        typed lvalue
 *
 * `any_view` is to `std::any` what `std::string_view` is to `std::string`: it
 * refers to an existing value without copying it, so a function can accept a
 * heterogeneous argument for read access at no allocation cost. It is two
 * pointers wide and trivially copyable.
 *
 * A view is built either from a typed lvalue directly (the *raw* backing) or from
 * a `std::any` (the *std::any* backing). The raw backing never depends on RTTI and
 * carries its type identity as a @ref scl::type_key, so `type_name()`,
 * `type_key()` and `has_value()` are usable in constant evaluation.
 * The `std::any` backing exists only when RTTI is enabled (`std::any` is itself
 * RTTI-coupled) and delegates casts to `std::any_cast`.
 *
 * Access is read-only: @ref scl::any_cast yields a value copy or a `const`
 * lvalue reference (`any_cast<T const &>` binds with no copy); a non-`const` or
 * rvalue reference cast is ill-formed.
 *
 * To also accept rvalue arguments "in place" — valid only for the duration of a
 * call — use the parameter-only companion @ref scl::any_arg.
 *
 * @note @ref scl::any_cast is a runtime operation on the C++20 baseline, as with
 *       `std::any_cast`; the identity queries above remain `constexpr`. The limit
 *       is one of binding rather than of erasure: the view refers to an lvalue of
 *       any type, so the only thing it can erase that lvalue's address to is
 *       `void const *`, and recovering a typed pointer from `void` is
 *       constant-evaluable only on compilers implementing P2738 (C++26). Erasing to
 *       a common base class instead is constant-evaluable on C++20, but it needs an
 *       object of that class kept alive alongside the referent for as long as the
 *       handle - which a storable view cannot bound and @ref scl::any_arg, being a
 *       parameter, can.
 *
 * @warning The view does not own or extend the lifetime of the referenced object.
 *          It stays valid only while that object lives — the same caveat as
 *          `std::string_view`. Constructor parameters are lifetime-bound
 *          (@ref SCL_LIFETIMEBOUND), so Clang reports a view escaping the scope
 *          of its referent.
 *
 * @par Example
 * @code
 * void bar(scl::any_view value)
 * {
 *     if (auto const * text = scl::any_cast<std::string>(&value))
 *         use(*text);                      // no copy
 * }
 *
 * std::string text{"Hello Any!"};
 * bar(text);                               // raw backing, no copy
 * std::any boxed{text};
 * bar(boxed);                              // std::any backing (RTTI builds)
 * @endcode
 *
 * @see scl::any_arg — the parameter-only companion, which also grants write access
 * @see scl::any_switch — a branch chain reading a view without a cascade of casts
 */

/**
 * @typedef scl::any_view::name
 * @brief Type-name string produced by @ref scl::any_view::type_name.
 */

/**
 * @fn scl::any_view::any_view()
 * @brief Constructs an empty view referring to no object.
 */

/**
 * @fn scl::any_view::any_view(::std::any const & value)
 * @brief Constructs a view over the object held in @p value without copying it.
 * @param value  The `std::any` to view; its contained object is referenced in
 *               place. Only available when RTTI is enabled.
 *
 * @warning On a build where RTTI is disabled but the standard library still declares
 *          `std::any` (this varies by toolchain — some make `<any>` itself unavailable
 *          without RTTI, others do not), passing a `std::any` binds the raw-lvalue
 *          constructor below instead of this one: the view reports `type_name() ==
 *          "std::any"` either way, but `any_cast` never unwraps the boxed object.
 *          There is no portable way to detect this case from inside the library.
 */

/**
 * @fn scl::any_view::any_view(Type & object)
 * @brief Constructs a view over a typed lvalue without copying it.
 *
 * Participates only for lvalues — an rvalue of any constness is rejected, since
 * the view would dangle — and excludes `std::any` and the ScL Any views
 * themselves. The object is referenced in place; its type is captured for later
 * @ref scl::any_cast.
 *
 * @tparam Type  Deduced reference type of the viewed lvalue.
 * @param  object  The lvalue to view.
 */

/**
 * @fn scl::any_view::has_value() const
 * @brief Reports whether the view refers to a live value.
 * @return `false` for an empty view or a view over an empty `std::any`;
 *         `true` otherwise.
 */

/**
 * @fn scl::any_view::type_name() const
 * @brief Returns the compile-time name of the viewed type.
 * @return `type_name<T>()` for the raw backing; `type_name<std::any>()` for the
 *         std::any backing, which names the backing itself rather than the type
 *         boxed inside it; an empty string for an empty view.
 */

/**
 * @fn scl::any_view::type_key() const
 * @brief Returns the identity key of the viewed type.
 *
 * The result is a pointer, not a reference: a @ref scl::type_key is an identity
 * with no empty value, so an empty view answers `nullptr` rather than a key of
 * nothing. A non-null result is the very object @ref scl::type_key_of returns for
 * that type, so `*view.type_key() == scl::type_key_of<T>()` costs one pointer
 * comparison, and `view.type_key()->name() == view.type_name()` holds throughout.
 *
 * @return The key of the viewed type; `nullptr` for an empty view. As with
 *         `type_name()`, the std::any backing identifies itself and not the boxed
 *         type; probe that with @ref scl::any_cast.
 */

/**
 * @fn scl::any_cast(View * view)
 * @ingroup scl_utility_any
 * @brief Returns a pointer to the viewed object when its type matches, else null.
 *
 * A match needs more than the type: @p Type must also carry every cv-qualifier the
 * referent was bound with, so reaching it adds qualification rather than discarding
 * it. Reading supplies `const` on its own, so only `volatile` has to be asked for —
 * a view over a `volatile` object answers `any_cast<T volatile>` and refuses
 * `any_cast<T>`. The reverse always works: the request may add qualifiers the
 * referent lacks.
 *
 * `view` itself may be `volatile`-qualified, and that is a qualifier the request
 * must cover too: a `volatile any_view` answers `any_cast<T volatile>` and refuses
 * `any_cast<T>`, on top of whatever the referent's own qualifiers require.
 *
 * @tparam Type     The expected object type; a reference type is rejected.
 * @tparam View     Deduced `any_view`, possibly `volatile`-qualified — that
 *                  qualification is itself a requirement the request must cover.
 * @param  view  The view to read (may be null).
 * @return `const Type *` to the viewed object on a type match; `nullptr` on
 *         mismatch, a qualifier the request does not cover, a null pointer, or an
 *         empty view. Never throws.
 *
 * @warning A raw-backed match compares @ref scl::type_key values, which stays exact
 *          across module boundaries and tells same-named anonymous-namespace types
 *          from different translation units apart. Two limits of the key carry over
 *          unchanged: a type declared at block scope (a local class or a closure
 *          type) is outside its contract and may falsely match a same-named
 *          namespace-scope type, and a key must not outlive the module that produced
 *          it — drop views built inside a shared library before unloading it.
 */

/**
 * @fn scl::any_cast(any_view const & view)
 * @ingroup scl_utility_any
 * @brief Returns the viewed object by value or by `const` reference when its type
 *        matches, otherwise throws.
 *
 * `Type` may be an object type (returns a copy) or a `const` lvalue reference
 * (binds to the viewed object with no copy). A non-`const` or rvalue reference is
 * rejected: the view never hands out a reference that suggests its referent may
 * be moved from. A `std::any` argument converts implicitly to a view, so the
 * same call serves both backings.
 *
 * The qualifier rule of the pointer form applies here too: a `volatile` referent is
 * read as `any_cast<T const volatile &>`.
 *
 * @tparam Type  The requested result type (`T` or `T const &`).
 * @param  view  The view to read.
 * @return The viewed object as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match @p Type, or @p Type
 *                            drops a cv-qualifier the referent carries.
 */
