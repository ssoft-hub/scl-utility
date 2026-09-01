#pragma once

/**
 * @file any_mutable_view.h
 * @brief Non-owning view granting write access over a typed lvalue.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_anchor.h>
#include <scl/utility/any/any_cast.h>
#include <scl/utility/any/any_view.h>
#include <scl/utility/attribute/lifetimebound.h>

#include <memory>
#include <type_traits>

#include "detail/base.h"

namespace scl
{
    class any_mutable_view;

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

        // An owner and an anchor have a constructor of their own here; every other ScL Any
        // handle is refused outright.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(Type & object SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(::std::is_object_v<Type>) && (!::std::is_const_v<Type>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            : base_type{::std::addressof(object), &detail::any_type_descriptor_of<Type &>}
        {}

        // The anchor is what lets a cast answer during constant evaluation before P2738.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(any_anchor<Type> const & bound SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(::std::is_object_v<Type>) && (!::std::is_const_v<Type>)
            : base_type{bound.bound_object(), bound.bound_descriptor()}
        {}

        template <typename AnyType>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_mutable_view(AnyType & owner SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(::std::is_base_of_v<detail::any_owner, AnyType>) &&
            (!::std::is_const_v<AnyType>) && (!::std::is_volatile_v<AnyType>)
            : base_type{owner.viewed_held(), owner.viewed_object(), owner.viewed_descriptor()}
        {}

        // A `const` object grants no write. A class derived from this one is excluded rather
        // than refused: an exact match here would beat the copy constructor it is meant to reach.
        template <typename Type>
        any_mutable_view(Type const &)
            requires(!::std::is_base_of_v<any_mutable_view, ::std::remove_cvref_t<Type>>)
        = delete;

        // Spelled for an rvalue too: otherwise an anchor temporary reaches the form above.
        template <typename Type>
        any_mutable_view(Type const &&)
            requires(!::std::is_base_of_v<any_mutable_view, ::std::remove_cvref_t<Type>>)
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
            return any_view{*this, detail::any_viewed_form_of(descriptor())};
        }

        // Takes the referent with the binding untouched, which is what keeps the write.
        friend class ::scl::any_argument;

        // An explicit object parameter deduces this type, which private inheritance refuses.
        friend class ::scl::detail::any_base;
        friend struct ::scl::detail::any_handle_access;
    };

    // No qualifier in the request is what makes it a write, so only a bare referent is covered.
    // Guarded: every specialisation implements the contract the primary template documents.
#ifndef DOXYGEN
    template <>
    struct any_cast_traits<any_mutable_view>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr Target * access(Source * view) noexcept
        {
            return detail::any_referent_of<Target>(*view);
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
 *        typed lvalue
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
 * `T const *`. Over an unqualified referent either is granted. The only qualifier a
 * referent here can carry is `volatile`, and a request has to repeat it to cover it,
 * so a `volatile` referent answers `any_cast<T volatile>` and refuses `any_cast<T>`.
 * The reference form spells the same choice with a reference type:
 * `any_cast<T &>` writes, `any_cast<T const &>` reads.
 *
 * @note The view's own qualifiers govern the view and take no part in a request:
 *       `any_mutable_view const` still hands out a `T *`, and a `volatile` view
 *       over an unqualified referent answers `any_cast<T>`. Only the qualifiers the
 *       referent was bound with are matched against the request. A handle that may
 *       not write is @ref scl::any_view.
 *
 * A view that is not `volatile` converts to @ref scl::any_view, which narrows it to
 * reading, and to @ref scl::any_arg, which keeps the write for the duration of a call.
 * Neither converts back: a handle that promises no write never acquires one. A `volatile`
 * view converts to neither and is read through @ref scl::any_cast alone, which is what a
 * callee taking one of those two has to be spared.
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
 * @fn scl::any_mutable_view::any_mutable_view(Type & object)
 * @brief Constructs a view over a typed lvalue without copying it.
 *
 * Participates only for a non-`const` lvalue — a `const` object has no write to
 * grant and an rvalue would dangle. An owning @ref scl::any and an
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
 *          reassigning it, swapping it or moving it leaves the view dangling.
 */

/**
 * @fn scl::any_mutable_view::operator any_view() const
 * @brief Narrows the view to reading.
 *
 * The result refers to the same object with the binding narrowed to `const`, so
 * no write survives the conversion. There is no conversion the other way.
 *
 * A view narrowed from an @ref scl::any_anchor keeps the anchor, so a cast through
 * the result still folds on the C++20 baseline. A reading view asks for `const` in
 * every request it makes, which is what refuses the write the description alone
 * would still have described.
 *
 * @return An @ref scl::any_view over this view's referent.
 */

/**
 * @fn scl::any_mutable_view::has_value() const
 * @brief Reports whether the view refers to anything at all.
 * @return `false` for an empty view; `true` otherwise, even where the object it
 *         refers to is itself empty.
 */

/**
 * @fn scl::any_mutable_view::type_name() const
 * @brief Returns the compile-time name of the viewed type.
 * @return `type_name<T>()` for the viewed type, which for a container is the
 *         container and not what it holds; an empty string for an empty view.
 */

/**
 * @fn scl::any_mutable_view::type_key() const
 * @brief Returns the identity key of the viewed type.
 * @return The key of the viewed type, answered by value; `scl::type_key{}` for
 *         an empty view. As with `type_name()`, a container identifies itself.
 */
