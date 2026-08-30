#pragma once

/**
 * @file any.h
 * @brief Owning, allocator-aware holder of a single object of any type.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/bad_any_cast.h>
#include <scl/utility/attribute/likely.h>
#include <scl/utility/attribute/no_unique_address.h>
#include <scl/utility/meta/type_key.h>
#include <scl/utility/preprocessor/exceptions.h>
#include <scl/utility/type_traits/forward_like.h>

#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>

#include "detail/holder.h"

namespace scl
{
    class any_argument;
    class any_mutable_view;
    class any_view;

    template <typename ValueType, typename AnyType>
    [[nodiscard]]
    constexpr auto any_cast(AnyType * any) noexcept
        -> ::std::conditional_t<::std::is_const_v<AnyType>, ValueType const *, ValueType *>
        requires(::std::is_object_v<ValueType>) &&
        (::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cv_t<AnyType>>);

    template <typename Allocator = ::std::allocator<::std::byte>, ::std::size_t Capacity = sizeof(void *)>
    class basic_any : detail::any_owner_tag
    {
    public:
        using allocator_type = Allocator;
        using name = detail::any_name;

    private:
        using descriptor_type = detail::any_type_descriptor;

    public:
        static constexpr ::std::size_t capacity = Capacity;

    private:
        descriptor_type const * m_descriptor = nullptr;
        detail::any_storage<Capacity> m_storage;
        SCL_NO_UNIQUE_ADDRESS
        Allocator m_allocator;

        // The union gives the buffer a pointer's worth of room whatever Capacity asks for.
        static constexpr ::std::size_t buffer_capacity = detail::any_storage<Capacity>::buffer_size;

    public:
        // Not defaulted: a defaulted constructor is not user-provided, so `any const value;`
        // would then need an initialiser.
        // NOLINTNEXTLINE(modernize-use-equals-default): see above
        constexpr basic_any() noexcept {}

        constexpr basic_any(::std::allocator_arg_t /*tag*/, Allocator const & allocator) noexcept
            : m_allocator{allocator}
        {}

        template <typename ValueType>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(bugprone-forwarding-reference-overload): the constraint excludes an any
        constexpr basic_any(ValueType && value) // NOLINT(*-explicit-*): implicit by design
            requires(!::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<ValueType>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<ValueType>>) &&
            (!detail::any_construction_tag_v<::std::remove_cvref_t<ValueType>>) &&
            detail::any_alignment_supported_v<::std::decay_t<ValueType>> &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, ValueType>)
        {
            construct<::std::decay_t<ValueType>>(::std::forward<ValueType>(value));
        }

        template <typename HandleType>
        // cppcheck-suppress noExplicitConstructor
        constexpr basic_any(HandleType const & handle) // NOLINT(*-explicit-*): takes the value
            requires(::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<HandleType>>)
        {
            take_referent_from(handle);
        }

        basic_any(basic_any const &) = delete;
        basic_any & operator=(basic_any const &) = delete;

        // An allocator's move never throws ([allocator.requirements]).
        constexpr basic_any(basic_any && other) noexcept
            : m_allocator{::std::move(other.m_allocator)}
        {
            take(other);
        }

        constexpr basic_any & operator=(basic_any && other) noexcept
        {
            if (this == &other)
                return *this;

            destroy_held();
            adopt_allocator(other.m_allocator);
            take(other);
            return *this;
        }

        template <typename ValueType, typename... Arguments>
        constexpr explicit basic_any(::std::in_place_type_t<ValueType> /*type*/, Arguments &&... arguments)
            requires detail::any_alignment_supported_v<::std::decay_t<ValueType>> &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, Arguments...>)
        {
            construct<::std::decay_t<ValueType>>(::std::forward<Arguments>(arguments)...);
        }

        template <typename HandleType>
        constexpr basic_any & operator=(HandleType const & handle)
            requires(::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<HandleType>>)
        {
            if (::std::is_constant_evaluated())
            {
                // The paths below read an address and reuse storage, and neither is
                // available here.
                if (holds(detail::any_handle_access::held(handle)))
                    return *this;

                basic_any replacement{::std::allocator_arg, m_allocator};
                replacement.take_referent_from(handle);

                destroy_held();
                take(replacement);
                return *this;
            }

            auto const * const described = detail::any_handle_access::descriptor(handle);
            void const * const referent = detail::any_handle_access::referent(handle);

            // The same type at the same address is the value already held.
            if (m_descriptor != nullptr && described != nullptr && described->as_value == m_descriptor &&
                SCL_UNLIKELY_EXPR(overlaps_held_object(referent)))
                return *this;

            if (rebuilds_in_place(described))
            {
                detail::any_type_descriptor const * const stored = described->as_value;

                m_storage.adopt(described->rebuild({.held = held(), .end = m_descriptor->erase}, referent));
                m_descriptor = stored;
                return *this;
            }

            basic_any replacement{::std::allocator_arg, m_allocator};
            replacement.take_referent(described, referent);

            destroy_held();
            take(replacement);
            return *this;
        }

        constexpr ~basic_any() { destroy_held(); }

        // Built before anything is destroyed, so a self-assignment reads a live object;
        // `emplace` cannot, being defined to reset first.
        template <typename ValueType>
        constexpr basic_any & operator=(ValueType && value)
            requires(!::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<ValueType>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<ValueType>>) &&
            (!detail::any_construction_tag_v<::std::remove_cvref_t<ValueType>>) &&
            detail::any_alignment_supported_v<::std::decay_t<ValueType>> &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, ValueType>)
        {
            using bare = ::std::decay_t<ValueType>;

            // The operation answering where the object sits is a run-time one.
            if (!::std::is_constant_evaluated() && m_descriptor == &detail::any_type_descriptor_of<bare &> &&
                SCL_UNLIKELY_EXPR(overlaps_held(value)))
                return *this;

            if constexpr (::std::is_nothrow_move_constructible_v<bare> && sizeof(bare) <= detail::any_widest_reuse_copy)
            {
                if (reuses_block<bare>())
                {
                    // Taken first: the stored object may own the value, and no step after throws.
                    bare taken(::std::forward<ValueType>(value));
                    rebuild<bare>(::std::move(taken));
                    return *this;
                }
            }

            basic_any replacement{::std::allocator_arg, m_allocator};
            replacement.construct<bare>(::std::forward<ValueType>(value));

            destroy_held();
            take(replacement);
            return *this;
        }

    public:
        template <typename ValueType, typename... Arguments>
        constexpr ::std::decay_t<ValueType> & emplace(Arguments &&... arguments)
            requires detail::any_alignment_supported_v<::std::decay_t<ValueType>> &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, Arguments...>)
        {
            using bare = ::std::decay_t<ValueType>;

            if (reuses_block<bare>())
            {
                rebuild<bare>(::std::forward<Arguments>(arguments)...);
                return *detail::any_holder_object<bare>(held());
            }

            destroy_held();
            construct<bare>(::std::forward<Arguments>(arguments)...);
            return *detail::any_holder_object<bare>(held());
        }

        constexpr void reset() noexcept { destroy_held(); }

        // Three moves, not an exchange of storage: a buffer object is reached through its address.
        constexpr void swap(basic_any & other) noexcept
        {
            if (this == &other)
                return;

            basic_any held_by_other{::std::move(other)};
            other = ::std::move(*this);
            *this = ::std::move(held_by_other);
        }

    public:
        [[nodiscard]]
        constexpr bool has_value() const noexcept
        {
            return m_descriptor != nullptr;
        }

        [[nodiscard]]
        constexpr ::scl::type_key type_key() const noexcept
        {
            return (m_descriptor != nullptr) ? *m_descriptor->type : ::scl::type_key{};
        }

        [[nodiscard]]
        constexpr name type_name() const noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->type->name() : name{};
        }

        [[nodiscard]]
        constexpr allocator_type get_allocator() const noexcept
        {
            return m_allocator;
        }

        [[nodiscard]]
        constexpr bool copyable() const noexcept
        {
            return m_descriptor != nullptr && m_descriptor->place != nullptr;
        }

        [[nodiscard]]
        constexpr basic_any try_copy() const
        {
            using traits = ::std::allocator_traits<Allocator>;

            basic_any copy{::std::allocator_arg, traits::select_on_container_copy_construction(m_allocator)};
            if (!copyable())
                return copy;

            if (::std::is_constant_evaluated())
            {
                auto const * owned = static_cast<detail::any_descriptor<Allocator> const *>(m_descriptor);
                copy.m_storage.adopt(owned->duplicate(m_storage.allocated(), copy.m_allocator));
                copy.m_descriptor = m_descriptor;
            }
            else
            {
                copy.take_referent(m_descriptor, m_descriptor->object(held()));
            }
            return copy;
        }

    private:
        [[nodiscard]]
        constexpr bool stored_in_buffer() const noexcept
        {
            return m_descriptor != nullptr && !::std::is_constant_evaluated() &&
                detail::any_fits_in_buffer(*m_descriptor, buffer_capacity);
        }

        [[nodiscard]]
        constexpr detail::any_extent extent() const noexcept
        {
            return {.size = m_descriptor->size, .alignment = m_descriptor->alignment};
        }

        // Fitting by size says nothing about the relocation the buffer demands. During constant
        // evaluation an object lives in a typed allocation, not in a block of bytes.
        [[nodiscard]]
        constexpr bool reuses_block_for(detail::any_type_descriptor const & described) const noexcept
        {
            if (::std::is_constant_evaluated() || m_descriptor == nullptr || stored_in_buffer())
                return false;

            if (detail::any_fits_in_buffer(described, buffer_capacity))
                return false;

            return detail::any_same_block(extent(), {.size = described.size, .alignment = described.alignment});
        }

        template <typename ValueType>
        [[nodiscard]]
        constexpr bool reuses_block() const noexcept
        {
            return reuses_block_for(detail::any_type_descriptor_of<ValueType &>);
        }

        // The operation stands in for the type: absent, no copy can be held aside.
        [[nodiscard]]
        constexpr bool rebuilds_in_place(detail::any_type_descriptor const * described) const noexcept
        {
            return described != nullptr && described->rebuild != nullptr &&
                reuses_block_for(*described->as_value);
        }

        // Asked only of equal sizes, where an object inside the stored one starts where it does.
        [[nodiscard]]
        constexpr bool overlaps_held_object(void const * object) const noexcept
        {
            return m_descriptor->object(held()) == object;
        }

        template <typename ValueType>
        [[nodiscard]]
        constexpr bool overlaps_held(ValueType const & value) const noexcept
        {
            return overlaps_held_object(static_cast<void const *>(::std::addressof(value)));
        }

        template <typename ValueType, typename... Arguments>
        void rebuild(Arguments &&... arguments)
        {
#if SCL_HAS_EXCEPTIONS
            // Read before the descriptor goes, which is what a failed construction needs.
            detail::any_extent const room = extent();
#endif
            void * const block = m_storage.allocated();

            m_descriptor->erase(m_storage.allocated());
            m_descriptor = nullptr;
#if SCL_HAS_EXCEPTIONS
            try
            {
                m_storage.adopt(detail::any_make_holder_in_place<ValueType>(block,
                    ::std::forward<Arguments>(arguments)...));
            }
            catch (...)
            {
                detail::any_release(m_allocator, block, room);
                throw;
            }
#else
            m_storage.adopt(detail::any_make_holder_in_place<ValueType>(block,
                ::std::forward<Arguments>(arguments)...));
#endif
            m_descriptor = &detail::any_type_descriptor_of<ValueType &>;
        }

        // The descriptor is set last, so a constructor that throws leaves the any empty. Constant
        // evaluation takes the typed allocation, run time the buffer or bytes.
        template <typename ValueType, typename... Arguments>
        constexpr void construct(Arguments &&... arguments)
        {
            if (::std::is_constant_evaluated())
            {
                m_storage.adopt(detail::any_make_holder<ValueType>(m_allocator,
                    ::std::forward<Arguments>(arguments)...));
                m_descriptor = &detail::any_descriptor_of<ValueType, Allocator>;
                return;
            }

            auto const & described = detail::any_type_descriptor_of<ValueType &>;
            if (detail::any_fits_in_buffer(described, buffer_capacity))
                static_cast<void>(detail::any_make_holder_in_place<ValueType>(m_storage.buffer(),
                    ::std::forward<Arguments>(arguments)...));
            else
                m_storage.adopt(place_allocated<ValueType>(described,
                    ::std::forward<Arguments>(arguments)...));

            m_descriptor = &described;
        }

        template <typename ValueType, typename... Arguments>
        [[nodiscard]]
        detail::any_holder_base *
        place_allocated(detail::any_type_descriptor const & described, Arguments &&... arguments)
        {
            detail::any_extent const room{.size = described.size, .alignment = described.alignment};
            void * const storage = detail::any_acquire(m_allocator, room);
#if SCL_HAS_EXCEPTIONS
            try
            {
                return detail::any_make_holder_in_place<ValueType>(storage,
                    ::std::forward<Arguments>(arguments)...);
            }
            catch (...)
            {
                detail::any_release(m_allocator, storage, room);
                throw;
            }
#else
            return detail::any_make_holder_in_place<ValueType>(storage,
                ::std::forward<Arguments>(arguments)...);
#endif
        }

        // A referent's type is known only to the descriptor its handle carries.
        template <typename HandleType>
        constexpr void take_referent_from(HandleType const & handle)
        {
            auto const * const described = detail::any_handle_access::descriptor(handle);
            if (described == nullptr)
                return;

            if (::std::is_constant_evaluated())
            {
                take_held_referent(described, detail::any_handle_access::held(handle));
                return;
            }

            take_referent(described, detail::any_handle_access::referent(handle));
        }

        // Constant evaluation allocates through the typed allocator, which only an owner-made
        // descriptor names, so anything else stops on the cast below.
        constexpr void
        take_held_referent(detail::any_type_descriptor const * described, detail::any_holder_base const * held)
        {
            auto const * const stored =
                static_cast<detail::any_descriptor<Allocator> const *>(described->as_value);

            // Refused on the same terms as at run time.
            if (stored->duplicate == nullptr || stored->alignment > detail::any_widest_alignment)
                return;

            m_storage.adopt(stored->duplicate(held, m_allocator));
            m_descriptor = stored;
        }

        // What `overlaps_held_object` answers about an address at run time.
        [[nodiscard]]
        constexpr bool holds(detail::any_holder_base const * candidate) const noexcept
        {
            return m_descriptor != nullptr && candidate != nullptr && candidate == held();
        }

        constexpr void
        take_referent(detail::any_type_descriptor const * described, void const * referent)
        {
            if (described == nullptr || described->place == nullptr)
                return;

            // The decayed form is what lands here, so the room and the operations come from it.
            auto const * const stored = described->as_value;

            // No block carries a wider alignment, so such a referent leaves the any empty.
            if (stored->alignment > detail::any_widest_alignment)
                return;

            if (detail::any_fits_in_buffer(*stored, buffer_capacity))
            {
                static_cast<void>(described->place(m_storage.buffer(), referent));
            }
            else
            {
                detail::any_extent const room{.size = stored->size, .alignment = stored->alignment};
                void * const storage = detail::any_acquire(m_allocator, room);
#if SCL_HAS_EXCEPTIONS
                try
                {
                    m_storage.adopt(described->place(storage, referent));
                }
                catch (...)
                {
                    detail::any_release(m_allocator, storage, room);
                    throw;
                }
#else
                m_storage.adopt(described->place(storage, referent));
#endif
            }

            m_descriptor = stored;
        }

        // An allocator need not be assignable, so the member is replaced through its lifetime.
        constexpr void adopt_allocator(Allocator const & allocator) noexcept
        {
            if constexpr (::std::is_copy_assignable_v<Allocator>)
            {
                m_allocator = allocator;
            }
            else
            {
                ::std::destroy_at(::std::addressof(m_allocator));
                ::std::construct_at(::std::addressof(m_allocator), allocator);
            }
        }

        // The buffer path keeps no pointer: the object's address is the buffer's.
        constexpr void take(basic_any & other) noexcept
        {
            m_descriptor = other.m_descriptor;
            if (m_descriptor == nullptr)
                return;

            if (stored_in_buffer())
                static_cast<void>(m_descriptor->move(m_storage.buffer(), other.held()));
            else
                m_storage.adopt(other.m_storage.allocated());

            other.m_descriptor = nullptr;
        }

        // What the typed allocator gave, it takes back; only an owner-made descriptor is seen there.
        constexpr void destroy_held() noexcept
        {
            if (m_descriptor == nullptr)
                return;

            if (::std::is_constant_evaluated())
            {
                static_cast<detail::any_descriptor<Allocator> const *>(m_descriptor)
                    ->release(m_storage.allocated(), m_allocator);
            }
            else if (stored_in_buffer())
            {
                m_descriptor->erase(held());
            }
            else
            {
                detail::any_extent const room = extent();
                detail::any_holder_base * const object = m_storage.allocated();

                m_descriptor->erase(object);
                detail::any_release(m_allocator, object, room);
            }

            m_descriptor = nullptr;
        }

        [[nodiscard]]
        constexpr detail::any_holder_base * held() noexcept
        {
            if (stored_in_buffer())
                return m_descriptor->reach(m_storage.buffer());
            return (m_descriptor != nullptr) ? m_storage.allocated() : nullptr;
        }

        [[nodiscard]]
        constexpr detail::any_holder_base const * held() const noexcept
        {
            if (stored_in_buffer())
            {
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): reach only locates the holder
                return m_descriptor->reach(const_cast<void *>(m_storage.buffer()));
            }
            return (m_descriptor != nullptr) ? m_storage.allocated() : nullptr;
        }

        // A view refers to the content, so relocating or ending it invalidates the view.
        [[nodiscard]]
        constexpr void const * viewed_object() const noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->object(held()) : nullptr;
        }

        // At run time a handle keeps the address instead, which spares its cast an indirection.
        [[nodiscard]]
        constexpr detail::any_holder_base const * viewed_held() const noexcept
        {
            return ::std::is_constant_evaluated() ? held() : nullptr;
        }

        [[nodiscard]]
        constexpr detail::any_type_descriptor const * viewed_descriptor() const noexcept
        {
            return m_descriptor;
        }

        [[nodiscard]]
        constexpr detail::any_type_descriptor const * viewed_const_descriptor() const noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->as_const : nullptr;
        }

        friend class ::scl::any_argument;
        friend class ::scl::any_mutable_view;
        friend class ::scl::any_view;

        template <typename ValueType, typename AnyType>
        friend constexpr auto ::scl::any_cast(AnyType * any) noexcept
            -> ::std::conditional_t<::std::is_const_v<AnyType>, ValueType const *, ValueType *>
            requires(::std::is_object_v<ValueType>) &&
            (::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cv_t<AnyType>>);
    };

    using any = basic_any<>;

    template <typename ValueType, typename AnyType>
    [[nodiscard]]
    constexpr auto any_cast(AnyType * any) noexcept
        -> ::std::conditional_t<::std::is_const_v<AnyType>, ValueType const *, ValueType *>
        requires(::std::is_object_v<ValueType>) &&
        (::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cv_t<AnyType>>)
    {
        using bare = ::std::remove_cv_t<ValueType>;

        if (any == nullptr)
            return nullptr;

        if (any->type_key() != ::scl::type_key_of<bare>())
            return nullptr;

        return detail::any_holder_object<bare>(any->held());
    }

#if SCL_HAS_EXCEPTIONS || defined(DOXYGEN)
    // Not deduced: this form must admit an implicit conversion, which deduction ignores.
    template <typename ValueType, typename AnyType>
    [[nodiscard]]
    // NOLINTNEXTLINE(cppcoreguidelines-missing-std-forward): the referent is forwarded, not the handle
    constexpr ValueType any_cast(AnyType && held)
        requires(::std::is_base_of_v<detail::any_owner_tag, ::std::remove_cvref_t<AnyType>>)
    {
        using bare = ::std::remove_cvref_t<ValueType>;

        auto * const pointer = ::scl::any_cast<bare>(::std::addressof(held));
        if (pointer == nullptr)
            throw bad_any_cast{};

        return static_cast<ValueType>(::scl::forward_like<AnyType>(*pointer));
    }
#endif

    template <typename Allocator, ::std::size_t Capacity>
    constexpr void
    swap(basic_any<Allocator, Capacity> & left, basic_any<Allocator, Capacity> & right) noexcept
    {
        left.swap(right);
    }

    template <typename ValueType, typename... Arguments>
    [[nodiscard]]
    constexpr any make_any(Arguments &&... arguments)
    {
        return any{::std::in_place_type<ValueType>, ::std::forward<Arguments>(arguments)...};
    }
} // namespace scl

// =============================================================================
// Documentation
// =============================================================================

/**
 * @class scl::basic_any
 * @ingroup scl_utility_any
 * @brief Owning holder of a single object of any type, usable in constant
 *        evaluation and free of RTTI
 * @tparam Allocator  Allocator used for an object the buffer cannot hold. An
 *                    over-aligned object lands on storage that suits it.
 * @tparam Capacity   Bytes of in-place storage, never effectively less than
 *                    `sizeof(void *)`: the buffer shares storage with the
 *                    allocation pointer, so that much room exists anyway. The
 *                    default keeps `sizeof(scl::any)` at two pointers, the
 *                    size `std::any` has on libstdc++.
 *
 * `basic_any` is to `std::any` what this library's @ref scl::any_view is to a
 * view: the same service, with the two properties `std::any` cannot offer. It
 * names types through @ref scl::type_key rather than `typeid`, so it compiles
 * with RTTI disabled, and its whole lifecycle - construction, `emplace`,
 * `any_cast`, destruction - runs during constant evaluation.
 *
 * An object of at most @p Capacity bytes that is nothrow-movable and no more
 * aligned than a pointer is stored inside the any itself; anything else is
 * allocated. The choice is made per type at compile time and costs no space:
 * the buffer shares its storage with the pointer that would otherwise hold the
 * allocation.
 *
 * The requirements on a stored type are destructibility without throwing,
 * constructibility from the arguments given, and an alignment of at most 64 bytes - the widest storage
 * the allocator is asked for; a stricter alignment is refused at compile time
 * rather than served under-aligned. An immovable type is admitted, since it is
 * allocated and an any moves by handing over the pointer.
 *
 * @note Copying is not a constructor. `basic_any` is move-only as a type, and a
 *       copy is requested explicitly, which is what lets a non-copyable type be
 *       stored at all. `std::vector<scl::any>` can therefore be moved but not
 *       copied.
 *
 * @note A `constexpr` object of this type at namespace scope can only be empty:
 *       an allocation made during constant evaluation must be released before
 *       that evaluation ends, and the buffer is unreachable there. Inside a
 *       `constexpr` function there is no such limit.
 *
 * @warning Move assignment and `swap` carry the allocator along with the object,
 *          ignoring `propagate_on_container_move_assignment` and
 *          `propagate_on_container_swap`. Storage acquired by one allocator can
 *          only be released by that same allocator, and the standard
 *          alternative - moving the object into the target's allocator - would
 *          require a move constructor from every stored type. For
 *          `std::pmr::polymorphic_allocator` this means move assignment carries
 *          the source's `memory_resource`.
 *
 * @par Example
 * @code
 * scl::any value{std::string{"Hello Any!"}};
 * if (auto * text = scl::any_cast<std::string>(&value))
 *     use(*text);
 * value.emplace<int>(42);
 * @endcode
 *
 * @see scl::any_view - the non-owning, read-only companion
 * @see scl::any_mutable_view - the non-owning companion that writes
 * @see scl::make_any - construction of an @ref scl::any in one call
 */

/**
 * @typedef scl::any
 * @ingroup scl_utility_any
 * @brief @ref scl::basic_any with the default allocator and in-place capacity.
 */

/**
 * @typedef scl::basic_any::allocator_type
 * @brief Allocator the any acquires storage from.
 */

/**
 * @typedef scl::basic_any::name
 * @brief Type-name string produced by @ref scl::basic_any::type_name.
 */

/**
 * @var scl::basic_any::capacity
 * @brief Bytes of in-place storage, as requested by the @p Capacity parameter.
 */

/**
 * @fn scl::basic_any::basic_any()
 * @brief Constructs an any holding nothing.
 */

/**
 * @fn scl::basic_any::basic_any(::std::allocator_arg_t tag, Allocator const & allocator)
 * @brief Constructs an empty any that will acquire storage from @p allocator.
 * @param tag  Tag selecting this constructor.
 * @param allocator  Allocator the any keeps and hands back from
 *                   @ref scl::basic_any::get_allocator.
 */

/**
 * @fn scl::basic_any::basic_any(ValueType && value)
 * @brief Constructs an any holding a copy or a move of @p value.
 *
 * Participates for every type but an any itself, a handle, and a construction
 * tag (`std::allocator_arg_t`, a `std::in_place_type_t`), each of which selects
 * its own constructor rather than being stored.
 *
 * @tparam ValueType  Deduced type of the stored object; the decayed form is
 *                    what gets stored.
 * @param  value  The object to store.
 */

/**
 * @fn scl::basic_any::basic_any(::std::in_place_type_t<ValueType> type, Arguments &&... arguments)
 * @brief Constructs the stored object in place from @p arguments, with no
 *        temporary of the stored type.
 * @tparam ValueType  Type to construct.
 * @tparam Arguments  Types of its constructor arguments.
 * @param  type  Tag naming the type to construct.
 * @param  arguments  Arguments forwarded to the constructor of @p ValueType.
 */

/**
 * @fn scl::basic_any::basic_any(HandleType const & handle)
 * @brief Constructs an any holding a copy of the object @p handle refers to.
 *
 * A handle stands for a value it does not own, so what is stored is that value
 * rather than the handle - the mistake `std::any` cannot refuse to make, since
 * its own constructor is an exact match for a view and cannot be excluded. The
 * referent's type is not named here, so the copy is made through the handle
 * rather than by naming one.
 *
 * The referent decays on the way in: an array is taken as the pointer it decays to.
 * A handle reads its referent as `const`, so only an array of `const` elements
 * yields a pointer the stored type can be built from; any other leaves the any
 * empty, as a referent that cannot be copied does.
 *
 * @tparam HandleType  Deduced @ref scl::any_view or @ref scl::any_argument.
 * @param  handle  The handle whose referent is copied.
 *
 * @note A referent that cannot be copied - a `std::unique_ptr`, or anything else
 *       without a copy constructor - leaves the any empty, as does one aligned
 *       more strictly than 64 bytes. Whether a handle's referent can be taken is
 *       not knowable at compile time, so this is a run-time outcome rather than
 *       a rejected call.
 *
 * @note During constant evaluation a referent is reachable only where a handle
 *       carries an owner's own description of it and the allocator is the one that
 *       owner holds. A handle over a plain lvalue or an anchor is not reachable
 *       there, and neither is an owner with another allocator, so the constructor
 *       is no constant expression in those cases.
 *
 * @note Storing the handle itself stays available, and is now the explicit
 *       spelling: `any{std::in_place_type<any_view>, view}`.
 */

/**
 * @fn scl::basic_any::operator=(HandleType const & handle)
 * @brief Destroys what this any holds and stores a copy of the object @p handle
 *        refers to.
 * @tparam HandleType  Deduced @ref scl::any_view or @ref scl::any_argument.
 * @param  handle  The handle whose referent is copied.
 * @return This any.
 *
 * @note A referent that cannot be taken - no copy constructor, or an alignment
 *       past 64 bytes - still ends what the any held: assigning it leaves this
 *       any empty rather than keeping the old object.
 *
 * @note The copy is taken before what this any holds is destroyed, as in
 *       @ref scl::basic_any::operator=(ValueType &&), so a referent the stored
 *       object owns is still alive when it is read. An allocated object replaced
 *       by a referent of the same size and alignment, allocated as well, keeps
 *       the storage it already has on the same terms: the referent's type moves
 *       without throwing and is no wider than 256 bytes.
 *
 * @note A handle standing for the stored object itself asks for the value
 *       already held, so `value = scl::any_view{value}` does nothing - which is
 *       what keeps a value whose type has no copy constructor at all. A handle
 *       on a member of the stored object names another type and replaces it.
 *
 * @note During constant evaluation a referent is reachable on the terms
 *       @ref scl::basic_any::basic_any(HandleType const &) states.
 */

/**
 * @fn scl::basic_any::basic_any(basic_any const &)
 * @brief Deleted: an any holds objects that need not be copyable, so copying is asked for
 *        rather than performed implicitly.
 *
 * A constructor has no way to report that the stored type has no copy constructor, and
 * refusing the whole any for such a type would cost more than it buys. Copying therefore
 * goes through @ref scl::basic_any::try_copy, which answers an empty any instead, with
 * @ref scl::basic_any::copyable telling the two outcomes apart beforehand.
 */

/**
 * @fn scl::basic_any::operator=(basic_any const &)
 * @brief Deleted for the same reason as the copy constructor: a copy is asked for through
 *        @ref scl::basic_any::try_copy and assigned from there.
 */

/**
 * @fn scl::basic_any::basic_any(basic_any && other)
 * @brief Takes over what @p other holds, leaving it empty.
 *
 * An allocated object is taken over by its pointer; an in-place one is
 * relocated, which only a nothrow-movable type ever is. Never throws.
 *
 * @param other  The any to take from.
 */

/**
 * @fn scl::basic_any::operator=(basic_any && other)
 * @brief Destroys what this any holds and takes over what @p other holds.
 * @param  other  The any to take from.
 * @return This any.
 */

/**
 * @fn scl::basic_any::~basic_any()
 * @brief Destroys the stored object and releases the storage it was given.
 *
 * An in-place object is destroyed where it sits; an allocated one is destroyed and its
 * block returned to the allocator this any holds. An empty any has nothing to do.
 */

/**
 * @fn scl::basic_any::operator=(ValueType && value)
 * @brief Destroys what this any holds and stores @p value instead.
 *
 * The stored object is always a new one - the assignment operator of the stored
 * type is never called, as with `std::any`. The value reaches this any before
 * what it holds is destroyed, so a throwing constructor leaves the any as it
 * was, and a value read out of the stored object is read while that object is
 * alive. @ref scl::basic_any::emplace gives the weaker guarantee `std::any`
 * gives.
 *
 * An allocated object replaced by one of the same size and alignment, allocated
 * as well, keeps the storage it already has, so the allocator is not asked
 * again. That holds while the stored type moves without throwing and is no wider
 * than 256 bytes, which is what a value taken aside for the rebuild costs;
 * beyond it a fresh allocation is asked for.
 *
 * @tparam ValueType  Deduced type of the stored object.
 * @param  value  The object to store.
 * @return This any.
 *
 * @note Assigning the stored object to its own any does nothing at all: the
 *       value is already there. A member of it names another type and is an
 *       ordinary replacement.
 *
 * @note Pointers into the stored object are invalidated, since the object they
 *       addressed is destroyed. Assigning the stored object to its own any, as
 *       above, is the one case that keeps them, since nothing is replaced.
 */

/**
 * @fn scl::basic_any::emplace(Arguments &&... arguments)
 * @brief Destroys what this any holds and constructs @p ValueType in its place.
 *
 * A constructor that throws leaves the any empty, as with `std::any`, since what
 * was held is destroyed first. @ref scl::basic_any::operator=(ValueType &&) gives
 * the stronger guarantee.
 *
 * An allocated object whose replacement is allocated as well, with the same size
 * and alignment, keeps the storage it already has, so neither the allocator nor
 * the address changes. Destroying first is what lets that happen for any type,
 * where assigning a value reaches it only for a type it can take aside.
 *
 * @warning An argument that refers into the stored object - the object itself, a
 *          member of it, or anything it owns - is read after that object is
 *          destroyed. Pass a copy, or assign the value instead, which destroys
 *          nothing until the replacement stands.
 *
 * @tparam ValueType  Type to construct; the decayed form is what gets stored.
 * @tparam Arguments  Types of its constructor arguments.
 * @param  arguments  Arguments forwarded to the constructor of @p ValueType.
 * @return Reference to the constructed object.
 */

/**
 * @fn scl::basic_any::reset()
 * @brief Destroys what this any holds and leaves it empty. Never throws.
 */

/**
 * @fn scl::basic_any::swap(basic_any & other)
 * @brief Exchanges what the two anys hold, allocators included. Never throws.
 * @param other  The any to exchange with.
 */

/**
 * @fn scl::basic_any::has_value() const
 * @brief Reports whether this any holds an object.
 * @return `true` while an object is held, a `std::any` included regardless of what it
 *         holds; `false` for an empty any.
 */

/**
 * @fn scl::basic_any::type_name() const
 * @brief Returns the compile-time name of the stored type.
 * @return `type_name<T>()` for the stored type; an empty string for an empty any.
 */

/**
 * @fn scl::basic_any::type_key() const
 * @brief Returns the identity key of the stored type.
 *
 * The key is answered by value, as it is by every handle here. An empty any
 * stores no type and answers an empty key, `scl::type_key{}`.
 *
 * @return The key of the stored type; `scl::type_key{}` for an empty any.
 */

/**
 * @fn scl::basic_any::get_allocator() const
 * @brief Returns a copy of the allocator this any acquires storage from.
 * @return The allocator.
 */

/**
 * @fn scl::basic_any::copyable() const
 * @brief Reports whether the stored type can be copied.
 *
 * A question about the type, not about the value, so an empty any answers
 * `false`. Reading it ahead of @ref scl::basic_any::try_copy is what tells an
 * empty result apart from a copy of nothing.
 *
 * @return `true` when @ref scl::basic_any::try_copy would reproduce the object.
 */

/**
 * @fn scl::basic_any::try_copy() const
 * @brief Returns an any holding a copy of the stored object, or an empty any
 *        when the stored type cannot be copied.
 *
 * Copying is not a constructor here, so a non-copyable type can be stored at
 * all; this is where a copy is asked for instead. The copy's allocator comes
 * from `allocator_traits::select_on_container_copy_construction`, since the copy
 * is a fresh object rather than the same one changing hands.
 *
 * @return An any holding the copy; an empty any when
 *         @ref scl::basic_any::copyable is `false`.
 * @throws Whatever the copy constructor of the stored type or the allocator
 *         throws.
 */

/**
 * @fn scl::any_cast(AnyType * any)
 * @ingroup scl_utility_any
 * @brief Returns a pointer to the stored object when its type matches, else null.
 *
 * Access follows the handle: a `const` any answers a pointer to `const`, a
 * non-`const` one grants write access, which is what separates an owner from a
 * view.
 *
 * @tparam ValueType  The expected object type; a reference type is rejected.
 * @tparam AnyType    Deduced any, possibly `const`-qualified.
 * @param  any  The any to read (may be null).
 * @return Pointer to the stored object on a type match; `nullptr` on mismatch or
 *         for a null pointer or an empty any. Never throws.
 *
 * @warning A match compares @ref scl::type_key values, which stays exact across
 *          module boundaries and tells same-named anonymous-namespace types from
 *          different translation units apart. Two limits of the key carry over: a
 *          type declared at block scope is outside its contract, and a key must
 *          not outlive the module that produced it.
 */

/**
 * @fn scl::any_cast(AnyType && held)
 * @ingroup scl_utility_any
 * @brief Returns the stored object by value or by reference when its type
 *        matches, otherwise throws.
 *
 * The value form takes the object from an rvalue any by move rather than by copy.
 *
 * @note Declared only where @ref SCL_HAS_EXCEPTIONS is `1`. A translation unit
 *       compiled without exceptions keeps the pointer form, which answers a
 *       failed request with `nullptr`.
 *
 * @tparam ValueType  The requested result type (`T`, `T &` or `T const &`).
 * @tparam AnyType    Deduced any, with its own cv-ref qualification.
 * @param  held  The any to read.
 * @return The stored object as @p ValueType.
 * @throws scl::bad_any_cast  If the stored type does not match @p ValueType.
 */

/**
 * @fn scl::swap(basic_any<Allocator, Capacity> & left, basic_any<Allocator, Capacity> & right)
 * @ingroup scl_utility_any
 * @brief Exchanges what the two anys hold. Never throws.
 * @tparam Allocator  Allocator of both anys.
 * @tparam Capacity   In-place capacity of both anys.
 * @param  left   One any.
 * @param  right  The other any.
 */

/**
 * @fn scl::make_any(Arguments &&... arguments)
 * @ingroup scl_utility_any
 * @brief Constructs an @ref scl::any holding a @p ValueType built from
 *        @p arguments.
 * @tparam ValueType  Type to construct.
 * @tparam Arguments  Types of its constructor arguments.
 * @param  arguments  Arguments forwarded to the constructor of @p ValueType.
 * @return An any holding the constructed object.
 */
