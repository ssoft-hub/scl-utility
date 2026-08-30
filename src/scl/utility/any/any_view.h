#pragma once

/**
 * @file any_view.h
 * @brief Non-owning, read-only view over a typed lvalue.
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
    class any_argument;
    class any_mutable_view;
    class any_view;

    class any_view : detail::any_base
    {
    public:
        using name = detail::any_base::name;

    private:
        using base_type = detail::any_base;

    public:
        constexpr any_view() noexcept = default;
        constexpr any_view(any_view const &) = default;
        constexpr any_view(any_view &&) = default;
        constexpr any_view & operator=(any_view const &) = default;
        constexpr any_view & operator=(any_view &&) = default;
        constexpr ~any_view() = default;

        // Each excluded type has a constructor of its own, over what it stands for.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_view(Type & object SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*): implicit view by design
            requires(!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<Type>>) &&
            (!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<Type>>) &&
            (!detail::is_any_anchor_v<::std::remove_cvref_t<Type>>)
            : base_type{::std::addressof(object), &detail::any_type_descriptor_of<Type &>}
        {}

        // The anchor is what lets a cast answer at compile time for an object no any owns.
        template <typename Type>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_view(any_anchor<Type> const & bound SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            : base_type{bound.bound_object(), bound.bound_descriptor()}
        {}

        template <typename AnyType>
        // cppcheck-suppress noExplicitConstructor
        constexpr any_view(AnyType const & owner SCL_LIFETIMEBOUND) noexcept // NOLINT(*-explicit-*)
            requires(::std::is_base_of_v<detail::any_owner, AnyType>)
            : base_type{owner.viewed_held(), owner.viewed_object(), owner.viewed_const_descriptor()}
        {}

        // Only the view's own copy escapes the refusal.
        template <typename Type>
        any_view(Type const &&)
            requires(!::std::is_same_v<::std::remove_cvref_t<Type>, any_view>)
        = delete;

    public:
        using base_type::has_value;
        using base_type::type_key;
        using base_type::type_name;

    private:
        // The referent travels as the base holds it, so a remembered holder stays one.
        constexpr explicit any_view(base_type const & bound, base_type::descriptor_type const * descriptor) noexcept
            : base_type{bound, descriptor}
        {}

        // From the parts: a volatile handle is not copyable as a base.
        any_view(detail::any_holder_base const * held,
            void const volatile * object,
            base_type::descriptor_type const * descriptor) noexcept
            : base_type{held, object, descriptor}
        {}

        friend class ::scl::any_argument;
        friend class ::scl::any_mutable_view;

        // An explicit object parameter deduces this type, which private inheritance refuses.
        friend class ::scl::detail::any_base;
        friend struct ::scl::detail::any_handle_access;
    };

    // A view reads, so its request carries `const`; the view's own qualification takes no part.
    // Guarded: every specialisation implements the contract the primary template documents.
#ifndef DOXYGEN
    template <>
    struct any_cast_traits<any_view>
    {
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr Target const * access(Source * view) noexcept
        {
            return detail::any_referent_of<Target const>(*view);
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
    class any_view
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
 * @class scl::any_view
 * @ingroup scl_utility_any
 * @brief Non-owning, read-only view over a typed lvalue
 *
 * A view is to a value what `std::string_view` is to a string: it refers to an
 * existing one without copying it, so a function can accept a heterogeneous
 * argument for read access at no allocation cost. It is two
 * pointers wide and trivially copyable.
 *
 * A view is built from a typed lvalue, whatever that type is. It carries the
 * identity as a @ref scl::type_key rather than a `type_info`, so `type_name()`,
 * `type_key()` and `has_value()` are usable in constant evaluation and no build
 * setting takes the constructor away. A type that itself holds something is bound
 * as the container it is: the view refers to it, not to what it contains.
 *
 * Access is read-only: @ref scl::any_cast yields a value copy or a `const`
 * lvalue reference (`any_cast<T const &>` binds with no copy); a non-`const` or
 * rvalue reference cast is ill-formed.
 *
 * To also accept rvalue arguments "in place" — valid only for the duration of a
 * call — use the parameter-only companion @ref scl::any_arg.
 *
 * @note @ref scl::any_cast over a plain lvalue is a runtime operation on the C++20
 *       baseline; over an @ref scl::basic_any it folds, the owner's own holder being
 *       what the view carries, and the identity queries fold whatever it refers to. The
 *       limit is one of binding rather than of erasure: the view refers to an lvalue of
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
 * bar(text);                               // views the string, no copy
 * int number = 42;
 * bar(number);                             // a type mismatch, answered with nullptr
 * @endcode
 *
 * @see scl::any_mutable_view — the same view, granting write access
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
 * @fn scl::any_view::any_view(any_view const &)
 * @brief Copies the view; both then refer to the same object.
 */

/**
 * @fn scl::any_view::any_view(any_view &&)
 * @brief Moves the view, which copies it: a view owns nothing to hand over.
 */

/**
 * @fn scl::any_view::operator=(any_view const &)
 * @brief Rebinds this view to what the assigned view refers to.
 * @return This view.
 */

/**
 * @fn scl::any_view::operator=(any_view &&)
 * @brief Rebinds this view, which copies: a view owns nothing to hand over.
 * @return This view.
 */

/**
 * @fn scl::any_view::~any_view()
 * @brief Trivial: the view owns neither the referent nor the descriptor.
 */

/**
 * @fn scl::any_view::any_view(Type const &&)
 * @brief Deleted: an rvalue of any constness would dangle the moment the full
 *        expression ends.
 */

/**
 * @fn scl::any_view::any_view(Type & object)
 * @brief Constructs a view over a typed lvalue without copying it.
 *
 * Participates only for lvalues — an rvalue of any constness is rejected, since
 * the view would dangle — and excludes the ScL Any handles and owners themselves,
 * each of which has a constructor of its own over what it stands for. The object is
 * referenced in place; its type is captured for later @ref scl::any_cast.
 *
 * @tparam Type  Deduced reference type of the viewed lvalue.
 * @param  object  The lvalue to view.
 */

/**
 * @fn scl::any_view::any_view(any_anchor<Type> const & bound)
 * @brief Constructs a view over the object @p bound stands for.
 *
 * The referent is the anchored object, never the anchor: `type_name()` and
 * `type_key()` answer for the object, and @ref scl::any_cast reaches it. What the
 * anchor adds is constant evaluation — see @ref scl::any_anchor for why a plain
 * lvalue needs one on the C++20 baseline and why C++26 does not.
 *
 * @tparam Type  The anchored object's type, qualifiers included.
 * @param  bound  The anchor standing for the object to view.
 */

/**
 * @fn scl::any_view::any_view(AnyType const & owner)
 * @brief Constructs a view over the object an owning any holds, rather than over
 *        the any itself.
 *
 * An owning any is the one referent a view sees through rather than binds:
 * `type_name()` answers the stored type, and a cast costs the same one key
 * comparison it costs over a plain lvalue.
 *
 * @tparam AnyType  Deduced @ref scl::basic_any specialization.
 * @param  owner  The any whose content is viewed.
 *
 * @warning The view refers into the any's own storage, so emptying it,
 *          reassigning it or moving it leaves the view dangling - the caveat
 *          `std::string_view` carries over `std::string`.
 */

/**
 * @fn scl::any_view::has_value() const
 * @brief Reports whether the view refers to anything at all.
 * @return `false` for an empty view; `true` otherwise, even where the object it
 *         refers to is itself empty.
 */

/**
 * @fn scl::any_view::type_name() const
 * @brief Returns the compile-time name of the viewed type.
 * @return `type_name<T>()` for the viewed type, which for a container is the
 *         container and not what it holds; an empty string for an empty view.
 */

/**
 * @fn scl::any_view::type_key() const
 * @brief Returns the identity key of the viewed type.
 *
 * The key is answered by value, and an empty view answers an empty key, which
 * names no type at all. `view.type_key() == scl::type_key_of<T>()` compares
 * against the per-type constant, and `view.type_key().name() == view.type_name()`
 * holds throughout.
 *
 * @return The key of the viewed type; `scl::type_key{}` for an empty view. As
 *         with `type_name()`, a view over a container identifies the container.
 */
