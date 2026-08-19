#pragma once

/**
 * @file holder.h
 * @brief Storage an owning ScL Any acquires for the object it holds.
 * @ingroup scl_utility_any
 */

#include <scl/utility/preprocessor/exceptions.h>

#include <array>
#include <concepts>
#include <cstddef>
#include <memory>
#include <utility>

#include "base.h"

namespace scl::detail
{
    // A buffer object is relocated by moving it, and a move that throws would leave the
    // owner with neither the old object nor the new one - hence the demand for `move`.
    [[nodiscard]]
    constexpr bool
    any_fits_in_buffer(any_type_descriptor const & descriptor, ::std::size_t capacity) noexcept
    {
        return descriptor.size <= capacity && descriptor.alignment <= alignof(void *) && descriptor.move != nullptr;
    }

    // Typed rather than byte-sized: this is what aligns an over-aligned object and what
    // makes the allocation a constant expression.
    template <typename Type, typename Allocator>
    using any_holder_allocator =
        typename ::std::allocator_traits<Allocator>::template rebind_alloc<any_holder<Type>>;

    template <typename Type, typename Allocator, typename... Arguments>
    [[nodiscard]]
    constexpr any_holder_base * any_make_holder(Allocator & allocator, Arguments &&... arguments)
    {
        using traits = ::std::allocator_traits<any_holder_allocator<Type, Allocator>>;

        any_holder_allocator<Type, Allocator> holder_allocator{allocator};
        any_holder<Type> * const created = traits::allocate(holder_allocator, 1);
#if SCL_HAS_EXCEPTIONS
        try
        {
            traits::construct(holder_allocator, created, ::std::forward<Arguments>(arguments)...);
        }
        catch (...)
        {
            traits::deallocate(holder_allocator, created, 1);
            throw;
        }
#else
        traits::construct(holder_allocator, created, ::std::forward<Arguments>(arguments)...);
#endif
        return created;
    }

    template <typename Type, typename Allocator>
    constexpr void any_destroy_holder(any_holder_base * held, Allocator & allocator) noexcept
    {
        using traits = ::std::allocator_traits<any_holder_allocator<Type, Allocator>>;

        any_holder_allocator<Type, Allocator> holder_allocator{allocator};
        auto * const typed = static_cast<any_holder<Type> *>(held);
        traits::destroy(holder_allocator, typed);
        traits::deallocate(holder_allocator, typed, 1);
    }

    // Not constexpr, and cannot be: raw bytes hold no object during constant evaluation.
    template <typename Type, typename... Arguments>
    [[nodiscard]]
    any_holder_base * any_make_holder_in_place(void * storage, Arguments &&... arguments)
    {
        return ::std::construct_at(static_cast<any_holder<Type> *>(storage),
            ::std::forward<Arguments>(arguments)...);
    }

    // Storage an owner hands to an operation that only knows the referent's size and
    // alignment. A block type carries the alignment, since allocator_traits has no way to
    // ask for one; the set is bounded, and an object aligned beyond it is refused rather
    // than under-aligned.
    template <::std::size_t Alignment>
    struct alignas(Alignment) any_block
    {
        ::std::array<::std::byte, Alignment> data;
    };

    struct any_extent
    {
        ::std::size_t size;
        ::std::size_t alignment;
    };

    inline constexpr ::std::size_t any_widest_alignment = 64U;

    // The bound the refusal enforces: an owner admits a type only while a block can carry
    // its alignment.
    template <typename Type>
    inline constexpr bool any_alignment_supported_v = alignof(any_holder<Type>) <= any_widest_alignment;

    // Exact, not roomy: the extent is what releasing is told, so it must describe the taker.
    [[nodiscard]]
    constexpr bool any_same_block(any_extent left, any_extent right) noexcept
    {
        return left.alignment == right.alignment && left.size == right.size;
    }

    template <::std::size_t Alignment, typename Allocator>
    [[nodiscard]]
    void * any_acquire_block(Allocator & allocator, ::std::size_t size)
    {
        using block = any_block<Alignment>;
        using traits = ::std::allocator_traits<
            typename ::std::allocator_traits<Allocator>::template rebind_alloc<block>>;

        typename traits::allocator_type block_allocator{allocator};
        return traits::allocate(block_allocator, (size + Alignment - 1U) / Alignment);
    }

    template <::std::size_t Alignment, typename Allocator>
    void any_release_block(Allocator & allocator, void * storage, ::std::size_t size) noexcept
    {
        using block = any_block<Alignment>;
        using traits = ::std::allocator_traits<
            typename ::std::allocator_traits<Allocator>::template rebind_alloc<block>>;

        typename traits::allocator_type block_allocator{allocator};
        traits::deallocate(block_allocator, static_cast<block *>(storage), (size + Alignment - 1U) / Alignment);
    }

    template <typename Allocator>
    [[nodiscard]]
    void * any_acquire(Allocator & allocator, any_extent extent)
    {
        switch (extent.alignment)
        {
        case 1U:
            return any_acquire_block<1U>(allocator, extent.size);
        case 2U:
            return any_acquire_block<2U>(allocator, extent.size);
        case 4U:
            return any_acquire_block<4U>(allocator, extent.size);
        case 8U:
            return any_acquire_block<8U>(allocator, extent.size);
        case 16U:
            return any_acquire_block<16U>(allocator, extent.size);
        case 32U:
            return any_acquire_block<32U>(allocator, extent.size);
        default:
            return any_acquire_block<any_widest_alignment>(allocator, extent.size);
        }
    }

    template <typename Allocator>
    void any_release(Allocator & allocator, void * storage, any_extent extent) noexcept
    {
        switch (extent.alignment)
        {
        case 1U:
            any_release_block<1U>(allocator, storage, extent.size);
            return;
        case 2U:
            any_release_block<2U>(allocator, storage, extent.size);
            return;
        case 4U:
            any_release_block<4U>(allocator, storage, extent.size);
            return;
        case 8U:
            any_release_block<8U>(allocator, storage, extent.size);
            return;
        case 16U:
            any_release_block<16U>(allocator, storage, extent.size);
            return;
        case 32U:
            any_release_block<32U>(allocator, storage, extent.size);
            return;
        default:
            any_release_block<any_widest_alignment>(allocator, storage, extent.size);
            return;
        }
    }

    // Buffer and pointer share storage, so holding a pointer-sized object in place is free.
    template <::std::size_t Capacity>
    class any_storage
    {
    public:
        static constexpr ::std::size_t buffer_size = (Capacity < sizeof(void *)) ? sizeof(void *) : Capacity;

    private:
        union
        {
            any_holder_base * m_allocated = nullptr;
            alignas(alignof(void *))::std::array<::std::byte, buffer_size> m_buffer;
        };

    public:
        [[nodiscard]]
        constexpr any_holder_base * allocated() const noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the storage
            return m_allocated;
        }

        constexpr void adopt(any_holder_base * held) noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): see above
            m_allocated = held;
        }

        [[nodiscard]]
        void * buffer() noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): see above
            return m_buffer.data();
        }

        [[nodiscard]]
        void const * buffer() const noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): see above
            return m_buffer.data();
        }
    };

    // Reached only during constant evaluation, where an object is allocated through the
    // typed allocator rather than placed in raw bytes, and so has to be released the same
    // way. At run time the storage-taking operations of the base descriptor do the work.
    // Constrained as `place` is, so a type copyable at run time is copyable here too: an
    // explicit copy constructor, or one beside a deleted move, satisfies one and not the other.
    template <typename Type, typename Allocator>
    [[nodiscard]]
    constexpr any_holder_base *
    any_duplicate_holder(any_holder_base const * held, Allocator & allocator)
        requires ::std::constructible_from<Type, Type const &>
    {
        return any_make_holder<Type>(allocator, *any_holder_object<Type>(held));
    }

    template <typename Allocator>
    using any_duplicate_function = any_holder_base * (*)(any_holder_base const * held, Allocator & allocator);

    template <typename Type, typename Allocator>
    [[nodiscard]]
    constexpr any_duplicate_function<Allocator> any_duplicate_operation_of() noexcept
    {
        if constexpr (::std::constructible_from<Type, Type const &>)
            return &any_duplicate_holder<Type, Allocator>;
        else
            return nullptr;
    }

    template <typename Allocator>
    struct any_descriptor : any_type_descriptor
    {
        void (*release)(any_holder_base * held, Allocator & allocator) noexcept;
        any_duplicate_function<Allocator> duplicate;
    };

    // The two cv-forms live in one object so that each can name the other: a handle
    // narrowed to const still reaches the value form, and both carry what only an owner has.
    template <typename Allocator>
    struct any_owned_forms
    {
        any_descriptor<Allocator> value;
        any_descriptor<Allocator> constant;
    };

    // Named rather than two pointers in a row, which read alike and swap unnoticed.
    struct any_owned_links
    {
        any_type_descriptor const * as_const;
        any_type_descriptor const * as_value;
    };

    [[nodiscard]]
    constexpr any_type_descriptor
    any_owned_form(any_type_descriptor described, any_owned_links links) noexcept
    {
        // What tells a cast that the handle remembers a holder; the shared forms describe
        // a plain binding.
        described.binding = any_binding::holder;
        described.as_const = links.as_const;
        described.as_value = links.as_value;
        return described;
    }

    template <typename Type, typename Allocator>
    inline constexpr any_owned_forms<Allocator> any_owned_forms_of{
        .value = {any_owned_form(any_type_descriptor_of<Type &>,
                  {.as_const = &any_owned_forms_of<Type, Allocator>.constant,
                  .as_value = &any_owned_forms_of<Type, Allocator>.value}),
                  &any_destroy_holder<Type, Allocator>, any_duplicate_operation_of<Type, Allocator>()},
        .constant = {
                  any_owned_form(any_type_descriptor_of<Type const &>,
                  {.as_const = &any_owned_forms_of<Type, Allocator>.constant,
                  .as_value = &any_owned_forms_of<Type, Allocator>.value}),
                  &any_destroy_holder<Type, Allocator>, any_duplicate_operation_of<Type, Allocator>()}
    };

    template <typename Type, typename Allocator>
    inline constexpr any_descriptor<Allocator> const & any_descriptor_of =
        any_owned_forms_of<Type, Allocator>.value;

} // namespace scl::detail
