#pragma once

/**
 * @file std_any.h
 * @brief Reaching the object a `std::any` holds through an ScL cast.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_cast.h>
#include <scl/utility/preprocessor/rtti.h>

#include <type_traits>

#if SCL_HAS_RTTI || defined(DOXYGEN)
#include <any>

namespace scl
{
    // Guarded: every specialisation implements the contract the primary template documents.
#ifndef DOXYGEN
    template <>
    struct any_cast_traits<::std::any>
    {
        struct movable_tag;

        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr auto access(Source * source)           /**/
            noexcept(noexcept(::std::any_cast<Target>(source))) /**/
            -> decltype(::std::any_cast<Target>(source))
            requires(::std::is_same_v<::std::remove_const_t<Source>, ::std::any>)
        {
            return ::std::any_cast<Target>(source);
        }
    };
#endif
} // namespace scl
#endif

// =============================================================================
// Documentation
// =============================================================================

/**
 * @file std_any.h
 *
 * A handle names the type it is bound to and nothing else, so @ref scl::any_cast
 * through @ref scl::any_view, @ref scl::any_arg or @ref scl::any_mutable_view bound
 * to a `std::any` answers the box itself. This header is what reaches inside one: it
 * specialises @ref scl::any_cast_traits for `std::any`, and that specialisation is
 * the only part of the interop costing anything at run time, being the only one that
 * compares `type_info`. A cast that never names a `std::any` never instantiates it.
 *
 * The box owns what it holds, so its own constness reaches the boxed object: a non-`const`
 * box answers `T *` and a `const` one `T const *`, and `nullptr` where the box is null,
 * holds nothing, or holds another type. The reference forms follow from the same request —
 * `any_cast<T>` copies the boxed object out of a named box and moves it out of an unnamed one,
 * `any_cast<T &>` writes into a non-`const` box, `any_cast<T const &>` binds without a copy,
 * and a request the box does not answer throws
 * @ref scl::bad_any_cast. A `volatile` box is refused at the call, since `std::any_cast`
 * takes no pointer to one.
 *
 * The whole header is empty without RTTI, where `std::any` cannot be named at all.
 */
