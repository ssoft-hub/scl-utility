#pragma once

/**
 * @file holder.h
 * @brief Storage an owning ScL Any acquires for the object it holds.
 */

#include <scl/utility/preprocessor/exceptions.h>

#include <array>
#include <cstddef>
#include <memory>
#include <utility>

#include "base.h"

namespace scl::detail
{
    // A buffer object is relocated by moving it, so a throwing move would leave neither object.
    [[nodiscard]]
    constexpr bool
    any_fits_in_buffer(any_type_descriptor const & descriptor, ::std::size_t capacity) noexcept
    {
        return descriptor.size <= capacity && descriptor.alignment <= alignof(void *) && descriptor.move != nullptr;
    }

    // Typed: constant evaluation allocates a holder as an object, raw bytes holding none there.
    template <typename Type, typename Allocator>
    using any_holder_allocator = ::std::allocator_traits<Allocator>::template rebind_alloc<any_holder<Type>>;

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

    // Cannot be constexpr: raw bytes hold no object during constant evaluation.
    template <typename Type, typename... Arguments>
    [[nodiscard]]
    any_holder_base * any_make_holder_in_place(void * storage, Arguments &&... arguments)
    {
        return ::std::construct_at(static_cast<any_holder<Type> *>(storage),
            ::std::forward<Arguments>(arguments)...);
    }

    template <typename Allocator>
    [[nodiscard]]
    void * any_acquire(Allocator & allocator, ::std::size_t size, ::std::size_t alignment)
    {
        using traits = ::std::allocator_traits<any_byte_allocator<Allocator>>;

        ::std::size_t const capacity = any_block_capacity(size, alignment);
        any_byte_allocator<Allocator> block_allocator{allocator};
        return any_lay_out_block(traits::allocate(block_allocator, capacity), capacity, size, alignment);
    }

    template <typename Allocator>
    void any_release(Allocator & allocator, void * object) noexcept
    {
        using traits = ::std::allocator_traits<any_byte_allocator<Allocator>>;

        any_block_header const header = any_block_header_of(object);
        any_byte_allocator<Allocator> block_allocator{allocator};
        traits::deallocate(block_allocator, any_block_base_of(object, header.offset), header.capacity);
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

    // Constant evaluation only, where an object comes from the typed allocator and goes back the
    // same way. Constrained as `place` is, so a type copyable at run time stays so here.
    template <typename Type, typename Allocator>
    [[nodiscard]]
    constexpr any_holder_base *
    any_duplicate_holder(any_holder_base const * held, Allocator & allocator)
        requires ::std::is_constructible_v<Type, Type const &> && ::std::is_nothrow_destructible_v<Type>
    {
        return any_make_holder<Type>(allocator, *any_holder_object<Type>(held));
    }

    template <typename Allocator>
    using any_duplicate_function = any_holder_base * (*)(any_holder_base const * held, Allocator & allocator);

    template <typename Type, typename Allocator>
    [[nodiscard]]
    constexpr any_duplicate_function<Allocator> any_duplicate_operation_of() noexcept
    {
        if constexpr (::std::is_constructible_v<Type, Type const &> && ::std::is_nothrow_destructible_v<Type>)
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

    // One object holds both cv-forms so each can name the other.
    template <typename Allocator>
    struct any_owned_forms
    {
        any_descriptor<Allocator> value;
        any_descriptor<Allocator> constant;
    };

    // One address per allocator, and the only thing a description made with it carries of it.
    template <typename Allocator>
    inline constexpr char any_owner_tag_of = 0;

    // Named rather than two pointers in a row, which read alike and swap unnoticed.
    struct any_owned_links
    {
        any_type_descriptor const * as_const;
        any_type_descriptor const * as_value;
    };

    template <typename Allocator>
    [[nodiscard]]
    constexpr any_type_descriptor
    any_owned_form(any_type_descriptor described, any_owned_links links) noexcept
    {
        // What tells a cast the handle remembers a holder rather than a plain binding.
        described.binding = any_binding::holder;
        described.as_const = links.as_const;
        described.as_value = links.as_value;
        described.owner = &any_owner_tag_of<Allocator>;
        return described;
    }

    template <typename Type, typename Allocator>
    inline constexpr any_owned_forms<Allocator> any_owned_forms_of{
        .value = {any_owned_form<Allocator>(any_type_descriptor_of<Type &>,
                  {.as_const = &any_owned_forms_of<Type, Allocator>.constant,
                  .as_value = &any_owned_forms_of<Type, Allocator>.value}),
                  &any_destroy_holder<Type, Allocator>, any_duplicate_operation_of<Type, Allocator>()},
        .constant = {
                  any_owned_form<Allocator>(any_type_descriptor_of<Type const &>,
                  {.as_const = &any_owned_forms_of<Type, Allocator>.constant,
                  .as_value = &any_owned_forms_of<Type, Allocator>.value}),
                  &any_destroy_holder<Type, Allocator>, any_duplicate_operation_of<Type, Allocator>()}
    };

    template <typename Type, typename Allocator>
    inline constexpr any_descriptor<Allocator> const & any_descriptor_of =
        any_owned_forms_of<Type, Allocator>.value;

} // namespace scl::detail
