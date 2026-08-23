#pragma once

/**
 * @file any_mutable_view.h
 * @brief Non-owning view granting write access over a `std::any` or a typed lvalue.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_anchor.h>
#include <scl/utility/any/any_view.h>
#include <scl/utility/any/bad_any_cast.h>
#include <scl/utility/attribute/hotcold.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/concepts/reference.h>
#include <scl/utility/concepts/type_category.h>
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
    class any_mutable_view;

    template <::scl::concepts::object_type Type, typename MutableView>
    [[nodiscard]]
    SCL_HOT constexpr Type * any_cast(MutableView * view) noexcept
        requires(::std::same_as<::std::remove_cv_t<MutableView>, any_mutable_view>);

    class any_mutable_view : detail::any_base
    {
    public:
        using name = detail::any_base::name;

    private:
        using base_type = detail::any_base;

    public:
        constexpr any_mutable_view() noexcept = default;
        constexpr any_mutable_view(any_mutable_view const &) = default;
        constexpr any_mutable_view(any_mutable_view &&) = default;
        constexpr any_mutable_view & operator=(any_mutable_view const &) = default;
        constexpr any_mutable_view & operator=(any_mutable_view &&) = default;
        constexpr ~any_mutable_view() = default;

#if SCL_HAS_RTTI || defined(DOXYGEN)
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(::std::any & value SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            : base_type{::std::addressof(value), &detail::any_type_descriptor_of<::std::any &>}
        {}
#endif

        // Each excluded type is bound by a constructor of its own, over what it stands for
        // rather than over itself.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(Type & object SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(!::std::is_const_v<Type>) && (!detail::is_std_any_v<::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            : base_type{::std::addressof(object), &detail::any_type_descriptor_of<Type &>}
        {}

        // The anchor is what lets a cast answer during constant evaluation before P2738.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(any_anchor<Type> const & bound SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(!::std::is_const_v<Type>)
            : base_type{bound.bound_object(), bound.bound_descriptor()}
        {}

        template <typename AnyType>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(AnyType & owner SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(::std::is_base_of_v<detail::any_owner_tag, AnyType>) &&
            (!::std::is_const_v<AnyType>) && (!::std::is_volatile_v<AnyType>)
            : base_type{owner.viewed_held(), owner.viewed_object(), owner.viewed_descriptor()}
        {}

        // A `const` object grants no write.
        template <typename Type>
        any_mutable_view(Type const &)
            requires(!::std::same_as<::std::remove_cvref_t<Type>, any_mutable_view>)
        = delete;

        // Spelled for an rvalue reference as well, and not left to the overload above: an
        // rvalue binds a const lvalue reference too, so an anchor temporary would reach the
        // more specialized constructor taking one. Only the view's own copy escapes.
        template <typename Type>
        any_mutable_view(Type const &&)
            requires(!::std::same_as<::std::remove_cvref_t<Type>, any_mutable_view>)
        = delete;

    public:
        using base_type::has_value;
        using base_type::type_key;
        using base_type::type_name;

        // NOLINTNEXTLINE(*-explicit-*): narrowing to reading needs no ceremony
        constexpr operator any_view() const noexcept { return as_view(); }

    private:
        [[nodiscard]]
        constexpr any_view as_view() const noexcept
        {
            return any_view{*this, const_descriptor()};
        }

        [[nodiscard]]
        any_view as_view() const volatile noexcept
        {
            // No holder: one is reached during constant evaluation only, which no volatile
            // object takes part in.
            return any_view{nullptr, object(), const_descriptor()};
        }

        // Takes the referent with the binding untouched, which is what keeps the write.
        friend class ::scl::any_argument;

        // An explicit object parameter in the base deduces this type, not the base, and
        // private inheritance would otherwise refuse that conversion.
        friend class ::scl::detail::any_base;
        friend struct ::scl::detail::any_handle_access;

        template <::scl::concepts::object_type Type, typename MutableView>
        friend constexpr Type * scl::any_cast(MutableView * view) noexcept
            requires(::std::same_as<::std::remove_cv_t<MutableView>, any_mutable_view>);
    };

    // The view type is deduced so that a cv-qualified handle binds at all; its qualification governs
    // the handle, not the referent, so it takes no part in the request.
    template <::scl::concepts::object_type Type, typename MutableView>
    [[nodiscard]]
    SCL_HOT constexpr Type * any_cast(MutableView * view) noexcept
        requires(::std::same_as<::std::remove_cv_t<MutableView>, any_mutable_view>)
    {
        if (view == nullptr) [[unlikely]]
            return nullptr;
        if (!view->binding_accepts(detail::any_qualifiers_of<Type &>()))
            return nullptr;
        if (::std::is_constant_evaluated())
            if (auto * const reached = detail::any_constant_referent<Type>(*view))
                return reached;

        // Through the reading view, not object(): for the std::any backing the stored
        // address is the box, and a cast must reach what is inside it.
        any_view const reading = view->as_view();
        auto const * const reached = ::scl::any_cast<Type const>(&reading);
        if (reached == nullptr)
            return nullptr;
        return detail::erased_cast<Type>(reached);
    }

#if SCL_HAS_EXCEPTIONS || defined(DOXYGEN)
    // Pinned to this class by deduction: a plain parameter would admit the conversion from
    // a reading handle, and with it a write that handle never promised.
    template <::scl::concepts::lvalue_reference Type, typename WriteView>
    [[nodiscard]]
    constexpr Type any_cast(WriteView & view)
        requires(::std::same_as<::std::remove_cv_t<WriteView>, any_mutable_view>) &&
        (!::std::is_const_v<::std::remove_reference_t<Type>>)
    {
        auto * const reached = any_cast<::std::remove_reference_t<Type>>(&view);
        if (reached == nullptr)
            throw bad_any_cast{};
        return *reached;
    }

    // Not lifetime-bound: the result is a copy, and it outlives the view by design.
    template <::scl::concepts::object_type Type, typename ValueView>
    [[nodiscard]]
    constexpr Type any_cast(ValueView & view)
        requires(::std::same_as<::std::remove_cv_t<ValueView>, any_mutable_view>)
    {
        auto const * const reached = any_cast<Type const>(&view);
        if (reached == nullptr)
            throw bad_any_cast{};
        return *reached;
    }

    template <::scl::concepts::lvalue_reference Type, typename ReadView>
    [[nodiscard]]
    constexpr Type any_cast(ReadView & view)
        requires(::std::same_as<::std::remove_cv_t<ReadView>, any_mutable_view>) &&
        (::std::is_const_v<::std::remove_reference_t<Type>>)
    {
        // remove_reference_t keeps the request's cv: T const volatile & must reach a volatile referent.
        auto const * const reached = any_cast<::std::remove_reference_t<Type>>(&view);
        if (reached == nullptr)
            throw bad_any_cast{};
        return *reached;
    }
#endif
} // namespace scl

// =============================================================================
// Documentation-only declarations
// =============================================================================

#ifdef DOXYGEN
namespace scl
{
    class any_mutable_view
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
 * @class scl::any_mutable_view
 * @ingroup scl_utility_any
 * @brief Non-owning view granting write access over an object held in a
 *        `std::any` or a typed lvalue
 *
 * `any_mutable_view` is @ref scl::any_view granting write access. It is
 * storable — a field, a container element, a return value — and two pointers
 * wide and trivially copyable, exactly as @ref scl::any_view is. The pair states
 * the axis in the names: an unqualified view only reads, a `mutable` one
 * writes.
 *
 * In ordinary reference terms the group covers `T const &` with
 * @ref scl::any_view, `T &` with this type, and a parameter with
 * @ref scl::any_arg. Reach for this one where the write has to outlive the call
 * that set it up — a slot filled later, a binding that pushes a value back, an
 * object that observes now and assigns afterwards.
 *
 * The binding is narrower than a reading view's, because writing is: a
 * `const` object is refused outright, and so is a temporary of any constness,
 * which the view would outlive. A `volatile` referent binds and is reached
 * under the coverage rule below.
 *
 * Access takes the request as written, the way @ref scl::any_arg does:
 * `any_cast<T>(&view)` yields `T *` and `any_cast<T const>(&view)` yields
 * `T const *`. On a type match either is granted, since the only qualifier a
 * referent here can carry is `volatile`, which the request then has to repeat.
 * The reference form spells the same choice with a reference type:
 * `any_cast<T &>` writes, `any_cast<T const &>` reads.
 *
 * @note The view's own qualifiers govern the view and take no part in a request:
 *       `any_mutable_view const` still hands out a `T *`, and a `volatile` view
 *       over an unqualified referent answers `any_cast<T>`. Only the qualifiers the
 *       referent was bound with are matched against the request. A handle that may
 *       not write is @ref scl::any_view.
 *
 * A view converts to @ref scl::any_view, which narrows it to reading, and to
 * @ref scl::any_arg, which keeps the write for the duration of a call. Neither
 * converts back: a handle that promises no write never acquires one.
 *
 * @warning Migrating a parameter between the handles is a change of its type,
 *          not an overload beside the existing one: a typed lvalue converts
 *          implicitly to each of them, so a call passing one becomes ambiguous
 *          between two such overloads.
 *
 * Identity queries and constant evaluation behave as @ref scl::any_view's do. A
 * cast folds where the recovery of a typed pointer from `void const *` folds,
 * which is P2738 (C++26); before that it needs an @ref scl::any_anchor the
 * caller declares, or an object an @ref scl::any owns, which already sits in a
 * holder the cast comes back down to. The anchor an @ref scl::any_arg gets for
 * free is a caller's temporary and dies with the call, which a storable view
 * cannot use.
 *
 * @warning The view does not own or extend the lifetime of the referenced
 *          object, and a dangling one writes rather than merely reads.
 *          Constructor parameters are lifetime-bound (@ref SCL_LIFETIMEBOUND),
 *          so Clang reports a view escaping the scope of its referent.
 *
 * @par Example
 * @code
 * struct binder
 * {
 *     scl::any_mutable_view slot;                // stored, still writable
 *
 *     void fill(int value)
 *     {
 *         if (auto * target = scl::any_cast<int>(&slot))
 *             *target = value;
 *     }
 * };
 *
 * int jobs = 1;
 * binder bound{jobs};
 * bound.fill(8);                                 // writes the caller's object
 * @endcode
 *
 * @see scl::any_view — the same view, granting no write access
 * @see scl::any_arg — the parameter-position companion, which also binds temporaries
 * @see scl::any_switch — a branch chain reading a view without a cascade of casts
 */

/**
 * @typedef scl::any_mutable_view::name
 * @brief Type-name string produced by @ref scl::any_mutable_view::type_name.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view()
 * @brief Constructs an empty view referring to no object.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(any_mutable_view const &)
 * @brief Copies the view; both then refer to the same object and may write it.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(any_mutable_view &&)
 * @brief Moves the view, which copies it: a view owns nothing to hand over.
 */

/**
 * @fn scl::any_mutable_view::operator=(any_mutable_view const &)
 * @brief Rebinds this view to what the assigned view refers to.
 * @return This view.
 */

/**
 * @fn scl::any_mutable_view::operator=(any_mutable_view &&)
 * @brief Rebinds this view, which copies: a view owns nothing to hand over.
 * @return This view.
 */

/**
 * @fn scl::any_mutable_view::~any_mutable_view()
 * @brief Trivial: the view owns neither the referent nor the descriptor.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(::std::any & value)
 * @brief Constructs a view over the object held in @p value without copying it.
 *
 * The `std::any` is bound for writing, so a request naming the boxed type
 * reaches it, and a request naming `std::any` hands out the box itself and lets
 * a callee replace what it holds. A `const std::any` is refused, as is a
 * temporary one.
 *
 * @param value  The `std::any` to view; its contained object is referenced in
 *               place. Only available when RTTI is enabled.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(Type & object)
 * @brief Constructs a view over a typed lvalue without copying it.
 *
 * Participates only for a non-`const` lvalue — a `const` object has no write to
 * grant and an rvalue would dangle. `std::any`, an owning @ref scl::any and an
 * @ref scl::any_anchor are excluded here and bound by a constructor of their own;
 * the other ScL Any handles are excluded and refused outright. The object is
 * referenced in place; its type and qualifiers are captured for later
 * @ref scl::any_cast.
 *
 * @tparam Type  Deduced type of the viewed lvalue, `volatile` included.
 * @param  object  The lvalue to view.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(Type const &)
 * @brief Deleted: a `const` object has no write to grant.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(Type const &&)
 * @brief Deleted: an rvalue of any constness would dangle the moment the full
 *        expression ends, a temporary @ref scl::any_anchor included.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(any_anchor<Type> const & bound)
 * @brief Constructs a view over the object @p bound stands for.
 *
 * The referent is the anchored object, never the anchor: `type_name()` and
 * `type_key()` answer for the object, and @ref scl::any_cast reaches it. An
 * anchor over a `const` object is refused, as a `const` lvalue is. What the
 * anchor adds is constant evaluation — see @ref scl::any_anchor for why a plain
 * lvalue needs one on the C++20 baseline and why C++26 does not.
 *
 * @tparam Type  The anchored object's type, `volatile` included.
 * @param  bound  The anchor standing for the object to view.
 */

/**
 * @fn scl::any_mutable_view::any_mutable_view(AnyType & owner)
 * @brief Constructs a view over the object an owning any holds, rather than over
 *        the any itself.
 *
 * The content is bound for writing, so `type_name()` answers the stored type and
 * a cast reaches the object in place. A `const` any is refused: its content is
 * no more writable than the any is.
 *
 * @tparam AnyType  Deduced @ref scl::basic_any specialization.
 * @param  owner  The any whose content is viewed.
 *
 * @warning The view refers into the any's own storage, so emptying it,
 *          reassigning it or moving it leaves the view dangling.
 */

/**
 * @fn scl::any_mutable_view::operator any_view() const
 * @brief Narrows the view to reading.
 *
 * The result refers to the same object with the binding narrowed to `const`, so
 * no write survives the conversion. There is no conversion the other way.
 *
 * @return An @ref scl::any_view over this view's referent.
 */

/**
 * @fn scl::any_mutable_view::has_value() const
 * @brief Reports whether the view refers to anything at all.
 * @return `false` for an empty view; `true` otherwise, a view over a `std::any`
 *         included regardless of what that `std::any` holds.
 */

/**
 * @fn scl::any_mutable_view::type_name() const
 * @brief Returns the compile-time name of the viewed type.
 * @return `type_name<T>()` for the raw backing; `type_name<std::any>()` for the
 *         std::any backing, which names the backing itself rather than the type
 *         boxed inside it; an empty string for an empty view.
 */

/**
 * @fn scl::any_mutable_view::type_key() const
 * @brief Returns the identity key of the viewed type.
 * @return The key of the viewed type, answered by value; `scl::type_key{}` for
 *         an empty view. As with `type_name()`, the std::any backing identifies
 *         itself and not the boxed type; probe that with @ref scl::any_cast.
 */

/**
 * @fn scl::any_cast(MutableView * view)
 * @ingroup scl_utility_any
 * @brief Returns a `Type *` to the viewed object when the request is granted,
 *        else null.
 *
 * The request is taken literally: @p Type states the qualification wanted and the
 * result is `Type *`. So `any_cast<T>` asks to write and `any_cast<T const>` asks
 * to read, and either is granted only when the request carries every cv-qualifier
 * the referent was bound with. The view's own constness adds none: it forbids
 * rebinding, not writing, so folding it in would refuse every write made through
 * a `const` view.
 *
 * @code
 * int *       w = scl::any_cast<int>(&view);       // the referent is writable
 * int const * r = scl::any_cast<int const>(&view); // reading asks for less
 * @endcode
 *
 * A request naming `std::any` is answered by the box itself, which is what the
 * identity queries already report for that backing; a view over anything else
 * refuses the request, as it refuses any other type it does not refer to.
 *
 * @tparam Type         The requested object type with its qualification; a reference
 *                      type is rejected.
 * @tparam MutableView  Deduced `any_mutable_view`, possibly cv-qualified; that
 *                      qualification governs the view alone.
 * @param  view  The view to reach through (may be null).
 * @return `Type *` to the viewed object on a type match whose qualifiers the
 *         request covers; `nullptr` otherwise, or on a null pointer. Never
 *         throws. The pointer refers to the viewed object, so it stays valid
 *         while that object lives.
 */

/**
 * @fn scl::any_cast(WriteView & view)
 * @ingroup scl_utility_any
 * @brief Writes through the viewed object, or throws.
 *
 * Three forms share this shape, told apart by @p Type: this one takes a
 * non-`const` lvalue reference and writes through it,
 * @ref scl::any_cast(ValueView & view) copies the value out, and
 * @ref scl::any_cast(ReadView & view) binds it as a `const` reference
 * without copying. Writing is granted under the coverage rule stated on the
 * pointer form.
 *
 * The view type is deduced and pinned to this class, because a plain parameter
 * would admit the implicit conversion from @ref scl::any_view, and with it a
 * write that handle never promised.
 *
 * @note All three are declared only where @ref SCL_HAS_EXCEPTIONS is `1`. A
 *       translation unit compiled without exceptions keeps the pointer form,
 *       which answers a failed request with `nullptr`.
 *
 * @tparam Type       The requested result type — a non-`const` lvalue reference.
 * @tparam WriteView  Deduced; must be `scl::any_mutable_view`.
 * @param  view  The view to write through.
 * @return The viewed object as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match, or the request
 *                            does not cover the qualifiers of the referent.
 */

/**
 * @fn scl::any_cast(ValueView & view)
 * @ingroup scl_utility_any
 * @brief Copies the viewed object out, or throws.
 *
 * The reading form for an object type. It supplies the `const` itself, exactly as
 * @ref scl::any_view does. The result is a copy and outlives @p view, so it is
 * not lifetime-bound.
 *
 * @note Declared only where @ref SCL_HAS_EXCEPTIONS is `1`.
 *
 * @tparam Type       The requested result type — an object type.
 * @tparam ValueView  Deduced; must be `scl::any_mutable_view`.
 * @param  view  The view to copy the value out of.
 * @return A copy of the viewed object, as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match, or the request
 *                            does not cover the qualifiers of the referent.
 */

/**
 * @fn scl::any_cast(ReadView & view)
 * @ingroup scl_utility_any
 * @brief Binds the viewed object as a `const` reference without copying, or
 *        throws.
 *
 * The reading form for a `const` lvalue reference, on the same terms as
 * @ref scl::any_cast(ValueView & view). The reference refers to the viewed
 * object, so it stays valid while that object lives.
 *
 * @note Declared only where @ref SCL_HAS_EXCEPTIONS is `1`.
 *
 * @tparam Type      The requested result type — a `const` lvalue reference.
 * @tparam ReadView  Deduced; must be `scl::any_mutable_view`.
 * @param  view  The view to bind to.
 * @return The viewed object as @p Type.
 * @throws scl::bad_any_cast  If the viewed type does not match, or the request
 *                            does not cover the qualifiers of the referent.
 */
