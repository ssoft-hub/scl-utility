#pragma once

/**
 * @file any_anchor.h
 * @brief Companion object that lets a handle read a plain lvalue at compile time.
 * @ingroup scl_utility_any
 */

#include <scl/utility/attribute/lifetimebound.h>

#include <memory>
#include <type_traits>

#include "detail/base.h"

namespace scl::detail
{
    // A cast reads this to tell an anchor from the form an owner's holder carries.
    [[nodiscard]]
    constexpr any_type_descriptor any_anchored_form(any_type_descriptor described) noexcept
    {
        described.binding = any_binding::anchor;
        return described;
    }

    // A descriptor that also carries the typed pointer, so a handle stays two pointers
    // wide: the one it already spends on a descriptor does for both.
    template <typename Type>
    struct any_anchored_descriptor : any_type_descriptor
    {
        // Spelled out rather than left to aggregate initialisation, which would also admit
        // a default-constructed one - describing no type at all.
        constexpr explicit any_anchored_descriptor(any_type_descriptor const & descriptor,
            Type * referent = nullptr) noexcept
            : any_type_descriptor{any_anchored_form(descriptor)}
            , referent{referent}
        {}

        // Not mutable: reading a mutable member of a constexpr object is not a constant
        // expression, and an anchor a caller declares is exactly such an object.
        Type * referent;
    };

    // Sound only once the caller has proved the request covers the referent's qualifiers.
    template <typename Type, typename Handle>
    [[nodiscard]]
    constexpr Type * any_constant_referent(Handle const & handle) noexcept
    {
        using bare = ::std::remove_cv_t<Type>;

        auto const * const described = any_handle_access::descriptor(handle);
        if (*described->type != ::scl::type_key_of<bare>())
            return nullptr;

        if (described->binding == any_binding::anchor)
            return static_cast<any_anchored_descriptor<bare> const *>(described)->referent;

        // Only this branch may read the holder: elsewhere it is the union's inactive member.
        if (described->binding == any_binding::holder)
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): the caller covered it
            return const_cast<bare *>(any_holder_object<bare>(any_handle_access::held(handle)));

        return nullptr;
    }
} // namespace scl::detail

namespace scl
{
    class any_argument;
    class any_mutable_view;
    class any_view;

    template <typename Type>
    class any_anchor;
} // namespace scl

namespace scl::detail
{
    // A handle bound to an anchor refers to what the anchor stands for, so the generic
    // constructors exclude one.
    template <typename Type>
    inline constexpr bool is_any_anchor_v = false;

    template <typename Type>
    inline constexpr bool is_any_anchor_v<::scl::any_anchor<Type>> = true;
} // namespace scl::detail

namespace scl
{
    template <typename Type>
    class any_anchor
    {
    private:
        // Bare: the qualifiers come from the descriptor beside the pointer.
        detail::any_anchored_descriptor<::std::remove_cv_t<Type>> m_anchor;

    public:
        // The qualifiers dropped here are restored by any_cast, which hands out a pointer
        // only once the descriptor has proved the request covers them.
        constexpr explicit any_anchor(Type & object SCL_LIFETIMEBOUND) noexcept
            : m_anchor{detail::any_type_descriptor_of<Type &>,
                  // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
                  const_cast<::std::remove_cv_t<Type> *>(::std::addressof(object))}
        {}

    private:
        [[nodiscard]]
        constexpr void const volatile * bound_object() const noexcept
        {
            return m_anchor.referent;
        }

        // Run time takes the descriptor shared by every binding of this type, which is what
        // keeps a handle comparable by address.
        [[nodiscard]]
        constexpr detail::any_type_descriptor const * bound_descriptor() const noexcept
        {
#if SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
            return &detail::any_type_descriptor_of<Type &>;
#else
            if (::std::is_constant_evaluated())
                return ::std::addressof(m_anchor);
            return &detail::any_type_descriptor_of<Type &>;
#endif
        }

        friend class ::scl::any_argument;
        friend class ::scl::any_mutable_view;
        friend class ::scl::any_view;
    };

#ifndef DOXYGEN
    template <typename Type>
    any_anchor(Type & object) -> any_anchor<Type>;
#endif
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/**
 * @class scl::any_anchor
 * @ingroup scl_utility_any
 * @brief Companion object that lets @ref scl::any_view and @ref scl::any_arg
 *        cast to a plain lvalue during constant evaluation
 *
 * @tparam Type  The referent's type, qualifiers included.
 *
 * A handle refers to an object of any type, so the only thing it can erase that
 * object's address to is `void const *` — and recovering a typed pointer from
 * `void` is a constant expression only on compilers implementing P2738 (C++26).
 * Casting down to a derived class is one on the C++20 baseline, but it needs an
 * object of that class to exist beside the referent, and someone has to keep it
 * alive for as long as the handle. An anchor is that object, and the caller
 * declaring it is that someone:
 *
 * @code
 * static constexpr int probe = 42;
 * static constexpr scl::any_anchor anchor{probe};
 * constexpr scl::any_view view{anchor};
 *
 * static_assert(*scl::any_cast<int>(&view) == 42);
 * @endcode
 *
 * An object an @ref scl::any owns needs no anchor: it already sits in a holder
 * the cast comes back down to, so a handle over an owning any answers during
 * constant evaluation on its own.
 *
 * The type is trivial. Copying, moving and destroying an anchor are the
 * compiler's own definitions, and each of them — the copy and move constructors,
 * both assignments and the destructor — is `constexpr`, so an anchor is as
 * usable during constant evaluation as the handles built through it.
 *
 * @note On C++26 no anchor is needed at all — a handle bound to a plain lvalue
 *       answers during constant evaluation by itself. The class stays part of
 *       the interface so that code written against the C++20 baseline keeps
 *       compiling, and it costs a run-time binding nothing: a handle built from
 *       an anchor keeps the descriptor shared by every binding of that type, and
 *       only constant evaluation reads the anchor.
 *
 * @warning The anchor neither owns the referent nor extends its lifetime, and a
 *          handle built from it refers to the anchor in turn. Both the referent
 *          and the anchor have to outlive every handle bound through it.
 */

/**
 * @fn scl::any_anchor::any_anchor(Type & object)
 * @brief Anchors @p object, recording its type and qualifiers.
 * @param object  The lvalue a handle built from this anchor refers to.
 */
