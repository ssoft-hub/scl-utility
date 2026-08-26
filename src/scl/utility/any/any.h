#pragma once

/**
 * @file any.h
 * @brief Owning, allocator-aware holder of a single object of any type.
 * @ingroup scl_utility_any
 */

#include <scl/utility/any/any_cast.h>
#include <scl/utility/attribute/lifetimebound.h>
#include <scl/utility/attribute/no_unique_address.h>
#include <scl/utility/meta/type_key.h>
#include <scl/utility/preprocessor/exceptions.h>

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

    template <typename Allocator = ::std::allocator<::std::byte>, ::std::size_t Capacity = sizeof(void *)>
    class basic_any : detail::any_owner
    {
    public:
        using allocator_type = Allocator;
        using name = detail::any_name;

    private:
        using descriptor = detail::any_type_descriptor;
        using storage = detail::any_storage<Capacity>;

    public:
        // The union gives the buffer a pointer's worth of room whatever Capacity asks for.
        static constexpr ::std::size_t buffer_capacity = storage::buffer_size;

    private:
        descriptor const * m_descriptor = nullptr;
        storage m_storage;
        SCL_NO_UNIQUE_ADDRESS
        Allocator m_allocator;

    public:
        // GCC does not take the union initialiser inside the storage template.
        // NOLINTNEXTLINE(modernize-use-equals-default): see above
        constexpr basic_any() noexcept {}

        constexpr basic_any(::std::allocator_arg_t /*tag*/, Allocator const & allocator) noexcept
            : m_allocator{allocator}
        {}

        template <typename ValueType>
        // cppcheck-suppress noExplicitConstructor
        // NOLINTNEXTLINE(bugprone-forwarding-reference-overload): the constraint excludes an any
        constexpr basic_any(ValueType && value) // NOLINT(*-explicit-*): implicit by design
            requires(!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<ValueType>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<ValueType>>) &&
            (!detail::is_any_referent_copy_v<::std::remove_cvref_t<ValueType>>) &&
            (!detail::is_any_construction_tag_v<::std::remove_cvref_t<ValueType>>) &&
            (detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, ValueType>)
        {
            construct<::std::decay_t<ValueType>>(::std::forward<ValueType>(value));
        }

        template <typename ValueType>
        constexpr basic_any(::std::allocator_arg_t /*tag*/, Allocator const & allocator, ValueType && value)
            requires(!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<ValueType>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<ValueType>>) &&
            (!detail::is_any_referent_copy_v<::std::remove_cvref_t<ValueType>>) &&
            (!detail::is_any_construction_tag_v<::std::remove_cvref_t<ValueType>>) &&
            (detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, ValueType>)
            : m_allocator{allocator}
        {
            construct<::std::decay_t<ValueType>>(::std::forward<ValueType>(value));
        }

        // Takes what @ref scl::basic_any::try_copy answers, and nothing a caller writes by hand:
        // neither a handle nor another container converts to one, since the object copied may
        // have no copy constructor and the result would then be silently empty.
        template <typename Source>
        // cppcheck-suppress noExplicitConstructor
        constexpr basic_any(detail::any_referent_copy<Source> taken) // NOLINT(*-explicit-*)
            requires(::std::is_base_of_v<detail::any_base, Source>) || (::std::is_same_v<Source, basic_any>)
            : m_allocator{copy_allocator_of(taken.source)}
        {
            copy_from(taken.source);
        }

        template <typename Source>
        constexpr basic_any(::std::allocator_arg_t /*tag*/,
            Allocator const & allocator,
            detail::any_referent_copy<Source> taken)
            requires(::std::is_base_of_v<detail::any_base, Source>) || (::std::is_same_v<Source, basic_any>)
            : m_allocator{allocator}
        {
            copy_from(taken.source);
        }

        basic_any(basic_any const &) = delete;
        basic_any & operator=(basic_any const &) = delete;

        constexpr basic_any(basic_any && other) noexcept
            : m_allocator{::std::move(other.m_allocator)}
        {
            take_from_any(other);
        }

        constexpr basic_any & operator=(basic_any && other) noexcept
        {
            if (this == &other)
                return *this;

            destroy_held();
            adopt_allocator(other.m_allocator);
            take_from_any(other);
            return *this;
        }

        template <typename ValueType, typename... Arguments>
        constexpr explicit basic_any(::std::in_place_type_t<ValueType> /*type*/, Arguments &&... arguments)
            requires(detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, Arguments...>)
        {
            construct<::std::decay_t<ValueType>>(::std::forward<Arguments>(arguments)...);
        }

        template <typename ValueType, typename... Arguments>
        constexpr explicit basic_any(::std::allocator_arg_t /*tag*/,
            Allocator const & allocator,
            ::std::in_place_type_t<ValueType> /*type*/,
            Arguments &&... arguments)
            requires(detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, Arguments...>)
            : m_allocator{allocator}
        {
            construct<::std::decay_t<ValueType>>(::std::forward<Arguments>(arguments)...);
        }

        template <typename Source>
        constexpr basic_any & operator=(detail::any_referent_copy<Source> taken)
            requires(::std::is_base_of_v<detail::any_base, Source>) || (::std::is_same_v<Source, basic_any>)
        {
            if constexpr (::std::is_same_v<Source, basic_any>)
            {
                if (this == ::std::addressof(taken.source))
                    return *this;

                // The allocator of the container assigned to stays: only it may release what
                // it acquired, and the copy is built through it.
                basic_any replacement{::std::allocator_arg, m_allocator};
                replacement.copy_from(taken.source);

                destroy_held();
                take_from_any(replacement);
                return *this;
            }
            else
            {
                Source const & handle = taken.source;

                if (::std::is_constant_evaluated())
                {
                    // The paths below read an address and reuse storage, and neither is available
                    // here. Only a holder binding answers where the object stands, and a handle
                    // carrying any other reaches for the union member that is not the active one.
                    auto const * const bound = detail::any_handle_access::descriptor(handle);
                    if (bound != nullptr && bound->binding == detail::any_binding::holder &&
                        holds(detail::any_handle_access::held(handle)))
                        return *this;

                    basic_any replacement{::std::allocator_arg, m_allocator};
                    replacement.copy_from_handle(handle);

                    destroy_held();
                    take_from_any(replacement);
                    return *this;
                }

                auto const * const described = detail::any_handle_access::descriptor(handle);
                void const * const referent = detail::any_handle_access::referent(handle);

                // The same type at the same address is the value already held.
                if (m_descriptor != nullptr && described != nullptr &&
                    described->as_value == m_descriptor && overlaps_held_object(referent))
                    return *this;

                if (rebuilds_in_place(described))
                {
                    detail::any_type_descriptor const * const stored = described->as_value;

                    m_storage.adopt(described->rebuild({.held = held(), .end = m_descriptor->erase}, referent));
                    m_descriptor = stored;
                    return *this;
                }

                basic_any replacement{::std::allocator_arg, m_allocator};
                replacement.copy_from_referent(described, referent);

                destroy_held();
                take_from_any(replacement);
                return *this;
            }
        }

        constexpr ~basic_any() { destroy_held(); }

        // Built before anything is destroyed, so a self-assignment reads a live object;
        // `emplace` cannot, being defined to reset first.
        template <typename ValueType>
        constexpr basic_any & operator=(ValueType && value)
            requires(!::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<ValueType>>) &&
            (!::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<ValueType>>) &&
            (!detail::is_any_referent_copy_v<::std::remove_cvref_t<ValueType>>) &&
            (!detail::is_any_construction_tag_v<::std::remove_cvref_t<ValueType>>) &&
            (detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
            (::std::is_constructible_v<::std::decay_t<ValueType>, ValueType>)
        {
            using bare = ::std::decay_t<ValueType>;

            // The same type at the same address is the value already held.
            if (m_descriptor == stored_form_of<bare>() && overlaps_held(value))
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
            take_from_any(replacement);
            return *this;
        }

    public:
        template <typename ValueType, typename... Arguments>
        constexpr ::std::decay_t<ValueType> & emplace(Arguments &&... arguments)
            requires(detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
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

        template <typename ValueType>
        void reserve_space_for()
            requires(detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (detail::is_any_holdable_v<::std::decay_t<ValueType>>)
        {
            using bare = ::std::decay_t<ValueType>;

            if (has_space_for<bare>())
                return;

            if (m_descriptor != nullptr && (is_stored_in_buffer() || m_descriptor->move == nullptr))
                return;

            auto const & described = detail::any_type_descriptor_of<bare &>;
            ::std::size_t size = described.size;
            ::std::size_t alignment = described.alignment;

            // Adopting a block moves the object already held into it, so it takes both shapes.
            if (m_descriptor != nullptr)
            {
                size = (m_descriptor->size > size) ? m_descriptor->size : size;
                alignment = (m_descriptor->alignment > alignment) ? m_descriptor->alignment : alignment;
            }

            adopt_block(detail::any_acquire(m_allocator, size, alignment));
        }

        void shrink_to_fit()
        {
            if (m_descriptor == nullptr)
            {
                release_block();
                return;
            }

            if (is_stored_in_buffer() || m_descriptor->move == nullptr)
                return;

            if (allocated_capacity() <= detail::any_block_capacity(m_descriptor->size, m_descriptor->alignment))
                return;

            adopt_block(detail::any_acquire(m_allocator, m_descriptor->size, m_descriptor->alignment));
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

        template <typename ValueType>
        [[nodiscard]]
        bool has_space_for() const noexcept
            requires(detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
            (detail::is_any_holdable_v<::std::decay_t<ValueType>>)
        {
            auto const & described = detail::any_type_descriptor_of<::std::decay_t<ValueType> &>;

            if (detail::any_fits_in_buffer(described, buffer_capacity))
                return true;

            return has_block() &&
                detail::any_block_fits(allocated_capacity(), described.size, described.alignment);
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

        // A container is the only consumer of the result, so a call outside a construction or
        // an assignment reaches nothing. Every specialisation takes what any of them answers.
        template <typename SourceType>
        [[nodiscard]]
        static constexpr auto try_copy(SourceType const & source SCL_LIFETIMEBOUND) noexcept
            requires(::std::is_base_of_v<detail::any_base, ::std::remove_cvref_t<SourceType>>) ||
            (::std::is_base_of_v<detail::any_owner, ::std::remove_cvref_t<SourceType>>)
        {
            return detail::any_referent_copy<SourceType>{source};
        }

        [[nodiscard]]
        constexpr bool is_copyable() const noexcept
        {
            return m_descriptor != nullptr && m_descriptor->place != nullptr;
        }

    private:
        // A handle takes the default allocator; a container hands over what its own allocator
        // answers for a copy.
        template <typename Source>
        [[nodiscard]]
        static constexpr Allocator copy_allocator_of(Source const & source)
        {
            if constexpr (::std::is_same_v<Source, basic_any>)
                return ::std::allocator_traits<Allocator>::select_on_container_copy_construction(
                    source.m_allocator);
            else
                return Allocator{};
        }

        template <typename Source>
        constexpr void copy_from(Source const & source)
        {
            if constexpr (::std::is_same_v<Source, basic_any>)
            {
                if (!source.is_copyable())
                    return;

                if (::std::is_constant_evaluated())
                {
                    auto const * const owned =
                        static_cast<detail::any_descriptor<Allocator> const *>(source.m_descriptor);
                    m_storage.adopt(owned->duplicate(source.m_storage.allocated(), m_allocator));
                    m_descriptor = source.m_descriptor;
                    return;
                }

                copy_from_referent(source.m_descriptor, source.m_descriptor->object(source.held()));
                return;
            }
            else
            {
                copy_from_handle(source);
            }
        }

        // The description a container built for this type carries, which says where the object
        // stands: a holder the allocator made during constant evaluation, the buffer or a block
        // at run time.
        template <typename ValueType>
        [[nodiscard]]
        static constexpr descriptor const * stored_form_of() noexcept
        {
            if (::std::is_constant_evaluated())
                return &detail::any_descriptor_of<ValueType, Allocator>;

            return &detail::any_type_descriptor_of<ValueType &>;
        }

        [[nodiscard]]
        constexpr bool is_stored_in_buffer() const noexcept
        {
            return m_descriptor != nullptr && !::std::is_constant_evaluated() &&
                detail::any_fits_in_buffer(*m_descriptor, buffer_capacity);
        }

        [[nodiscard]]
        ::std::size_t allocated_capacity() const noexcept
        {
            return detail::any_block_header_of(m_storage.allocated()).capacity;
        }

        [[nodiscard]]
        bool has_block() const noexcept
        {
            return !is_stored_in_buffer() && m_storage.allocated() != nullptr;
        }

        void release_block() noexcept
        {
            if (has_block())
                detail::any_release(m_allocator, m_storage.allocated());
            m_storage.adopt(nullptr);
        }

        void adopt_block(void * block) noexcept
        {
            if (m_descriptor == nullptr)
            {
                release_block();
                m_storage.adopt(static_cast<detail::any_holder_base *>(block));
                return;
            }

            detail::any_holder_base * const source = m_storage.allocated();
            detail::any_holder_base * const moved = m_descriptor->move(block, source);

            detail::any_release(m_allocator, source);
            m_storage.adopt(moved);
        }

        // Fitting by size says nothing about the relocation the buffer demands. During constant
        // evaluation an object lives in a typed allocation, not in a block of bytes.
        [[nodiscard]]
        constexpr bool reuses_block_for(detail::any_type_descriptor const & described) const noexcept
        {
            if (::std::is_constant_evaluated() || !has_block())
                return false;

            if (detail::any_fits_in_buffer(described, buffer_capacity))
                return false;

            return detail::any_block_fits(allocated_capacity(), described.size, described.alignment);
        }

        template <typename ValueType>
        [[nodiscard]]
        constexpr bool reuses_block() const noexcept
        {
            return reuses_block_for(detail::any_type_descriptor_of<ValueType &>);
        }

        // A reserved block holds no object to rebuild from, and no operation means no copy.
        [[nodiscard]]
        constexpr bool rebuilds_in_place(detail::any_type_descriptor const * described) const noexcept
        {
            return m_descriptor != nullptr && described != nullptr &&
                described->rebuild != nullptr && reuses_block_for(*described->as_value);
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
            using holder = detail::any_holder<ValueType>;

            // Read before the object goes, which is what placing the next one needs.
            detail::any_block_header const held_in = detail::any_block_header_of(m_storage.allocated());
            auto * const base = detail::any_block_base_of(m_storage.allocated(), held_in.offset);

            if (m_descriptor != nullptr)
                m_descriptor->erase(m_storage.allocated());
            m_descriptor = nullptr;

            void * const block = detail::any_lay_out_block(base, held_in.capacity, sizeof(holder),
                alignof(holder));
#if SCL_HAS_EXCEPTIONS
            try
            {
                m_storage.adopt(detail::any_make_holder_in_place<ValueType>(block,
                    ::std::forward<Arguments>(arguments)...));
            }
            catch (...)
            {
                detail::any_release(m_allocator, block);

                // The pointer would otherwise read as a block still held.
                m_storage.adopt(nullptr);
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
                place_in_buffer<ValueType>(::std::forward<Arguments>(arguments)...);
            else
                m_storage.adopt(place_allocated<ValueType>(described,
                    ::std::forward<Arguments>(arguments)...));

            m_descriptor = &described;
        }

        // The bytes of an object that never finished would read as a block still held.
        template <typename ValueType, typename... Arguments>
        void place_in_buffer(Arguments &&... arguments)
        {
#if SCL_HAS_EXCEPTIONS
            try
            {
                static_cast<void>(detail::any_make_holder_in_place<ValueType>(m_storage.buffer(),
                    ::std::forward<Arguments>(arguments)...));
            }
            catch (...)
            {
                m_storage.adopt(nullptr);
                throw;
            }
#else
            static_cast<void>(detail::any_make_holder_in_place<ValueType>(m_storage.buffer(),
                ::std::forward<Arguments>(arguments)...));
#endif
        }

        template <typename ValueType, typename... Arguments>
        [[nodiscard]]
        detail::any_holder_base * place_allocated(/**/
            detail::any_type_descriptor const & described,
            Arguments &&... arguments)
        {
            void * const storage = detail::any_acquire(m_allocator, described.size, described.alignment);
#if SCL_HAS_EXCEPTIONS
            try
            {
                return detail::any_make_holder_in_place<ValueType>(storage,
                    ::std::forward<Arguments>(arguments)...);
            }
            catch (...)
            {
                detail::any_release(m_allocator, storage);
                throw;
            }
#else
            return detail::any_make_holder_in_place<ValueType>(storage,
                ::std::forward<Arguments>(arguments)...);
#endif
        }

        // A referent's type is known only to the descriptor its handle carries.
        template <typename HandleType>
        constexpr void copy_from_handle(HandleType const & handle)
        {
            auto const * const described = detail::any_handle_access::descriptor(handle);
            if (described == nullptr)
                return;

            if (::std::is_constant_evaluated())
            {
                // Only a holder binding has the holder as the active union member, and only a
                // description this container's own allocator made names a duplicate it may run.
                // Anything else answers an empty any, as an object that cannot be copied does.
                if (described->binding != detail::any_binding::holder || described->as_value == nullptr ||
                    described->as_value->owner != &detail::any_owner_tag_of<Allocator>)
                    return;

                copy_from_holder(described, detail::any_handle_access::held(handle));
                return;
            }

            copy_from_referent(described, detail::any_handle_access::referent(handle));
        }

        // Reached only for a description this container's own allocator made, which is what
        // makes the cast below the one the object was described with.
        constexpr void copy_from_holder(/**/
            detail::any_type_descriptor const * described,
            detail::any_holder_base const * held)
        {
            auto const * const stored =
                static_cast<detail::any_descriptor<Allocator> const *>(described->as_value);

            // Refused on the same terms as at run time.
            if (stored->duplicate == nullptr)
                return;

            m_storage.adopt(stored->duplicate(held, m_allocator));
            m_descriptor = stored;
        }

        // What `overlaps_held_object` answers about an address at run time.
        [[nodiscard]]
        constexpr bool holds(/**/
            detail::any_holder_base const * candidate) const noexcept
        {
            return m_descriptor != nullptr && candidate != nullptr && candidate == held();
        }

        constexpr void copy_from_referent(/**/
            detail::any_type_descriptor const * described,
            void const * referent)
        {
            if (described == nullptr || described->place == nullptr)
                return;

            // The decayed form is what lands here, so the room and the operations come from it.
            auto const * const stored = described->as_value;

            if (detail::any_fits_in_buffer(*stored, buffer_capacity))
            {
#if SCL_HAS_EXCEPTIONS
                try
                {
                    static_cast<void>(described->place(m_storage.buffer(), referent));
                }
                catch (...)
                {
                    // The bytes written would otherwise read as a block still held.
                    m_storage.adopt(nullptr);
                    throw;
                }
#else
                static_cast<void>(described->place(m_storage.buffer(), referent));
#endif
            }
            else
            {
                void * const storage = detail::any_acquire(m_allocator, stored->size, stored->alignment);
#if SCL_HAS_EXCEPTIONS
                try
                {
                    m_storage.adopt(described->place(storage, referent));
                }
                catch (...)
                {
                    detail::any_release(m_allocator, storage);
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
        constexpr void take_from_any(basic_any & other) noexcept
        {
            m_descriptor = other.m_descriptor;

            if (m_descriptor != nullptr && is_stored_in_buffer())
                static_cast<void>(m_descriptor->move(m_storage.buffer(), other.held()));
            else
                m_storage.adopt(other.m_storage.allocated());

            other.m_descriptor = nullptr;
            other.m_storage.adopt(nullptr);
        }

        // What the typed allocator gave, it takes back; only an owner-made descriptor is seen there.
        constexpr void destroy_held() noexcept
        {
            if (m_descriptor == nullptr)
            {
                if (!::std::is_constant_evaluated())
                    release_block();
                return;
            }

            if (::std::is_constant_evaluated())
            {
                static_cast<detail::any_descriptor<Allocator> const *>(m_descriptor)
                    ->release(m_storage.allocated(), m_allocator);
            }
            else if (is_stored_in_buffer())
            {
                m_descriptor->erase(held());
            }
            else
            {
                detail::any_holder_base * const object = m_storage.allocated();

                m_descriptor->erase(object);
                detail::any_release(m_allocator, object);
            }

            m_descriptor = nullptr;

            // Tells a reserved block from bytes the object left behind.
            m_storage.adopt(nullptr);
        }

        [[nodiscard]]
        constexpr detail::any_holder_base * held() noexcept
        {
            if (is_stored_in_buffer())
                return m_descriptor->reach(m_storage.buffer());
            return (m_descriptor != nullptr) ? m_storage.allocated() : nullptr;
        }

        [[nodiscard]]
        constexpr detail::any_holder_base const * held() const noexcept
        {
            if (is_stored_in_buffer())
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): reach only locates the holder
                return m_descriptor->reach(const_cast<void *>(m_storage.buffer()));
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
        friend struct ::scl::detail::any_owner_access;
    };

    using any = basic_any<>;

#ifndef DOXYGEN
    template <typename Allocator, ::std::size_t Capacity>
    struct any_cast_traits<basic_any<Allocator, Capacity>>
    {
        struct movable_tag;

        // The owner names the type rather than covering it, which is what any_stored_object_of
        // answers; the cast forms follow from the pointer it hands back.
        template <typename Target, typename Source>
        [[nodiscard]]
        static constexpr auto access(Source * owner) noexcept /**/
            -> decltype(detail::any_stored_object_of<Target>(*owner))
        {
            return detail::any_stored_object_of<Target>(*owner);
        }
    };
#endif

    template <typename Allocator, ::std::size_t Capacity>
    constexpr void swap(/**/
        basic_any<Allocator, Capacity> & left,
        basic_any<Allocator, Capacity> & right) noexcept
    {
        left.swap(right);
    }

    template <typename ValueType, typename... Arguments>
    [[nodiscard]]
    constexpr any make_any(Arguments &&... arguments)
        requires(detail::is_any_storable_v<::std::remove_reference_t<ValueType>>) &&
        (::std::is_nothrow_destructible_v<::std::decay_t<ValueType>>) &&
        (::std::is_constructible_v<::std::decay_t<ValueType>, Arguments...>)
    {
        return any{::std::in_place_type<ValueType>, ::std::forward<Arguments>(arguments)...};
    }
} // namespace scl

// The uses-allocator protocol hands an element the allocator of whatever holds it and expects
// the element to use that one. This container cannot promise it: a block goes back only to the
// allocator that gave it, and a stored object need not be movable into storage another
// allocator gives. Written qualified rather than by reopening the namespace.
// Guarded: the specialisation states what the primary template would otherwise read off
// `allocator_type` alone.
#ifndef DOXYGEN
template <typename Allocator, ::std::size_t Capacity, typename Other>
// NOLINTNEXTLINE(bugprone-std-namespace-modification)
struct std::uses_allocator<::scl::basic_any<Allocator, Capacity>, Other> : ::std::false_type
{};
#endif

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
 * The requirements on a stored type are destructibility without throwing and
 * constructibility from the arguments given. Whatever alignment a type asks for
 * is served: a block carries the room to align the object inside itself. An
 * immovable type is admitted, since it is allocated and an any moves by handing
 * over the pointer. A copy constructor that is declared but ill-formed once
 * instantiated - the one `std::vector<std::unique_ptr<int>>` has - is a compile
 * error rather than an any left empty: what the constraint reads is the
 * declaration, and the copy operation is built from it.
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
 * @warning Move construction, move assignment and `swap` carry the allocator
 *          along with the object, ignoring
 *          `propagate_on_container_move_assignment` and
 *          `propagate_on_container_swap`. Storage acquired by one allocator can
 *          only be released by that same allocator, and the standard
 *          alternative - moving the object into the target's allocator - would
 *          require a move constructor from every stored type. For
 *          `std::pmr::polymorphic_allocator` this means the source's
 *          `memory_resource` travels with the object.
 *
 * @note For the same reason `std::uses_allocator<basic_any<Allocator, Capacity>, A>`
 *       is `false`: the uses-allocator protocol hands an element the allocator of
 *       whatever holds it and expects the element to use that one, which this type
 *       cannot promise. An element of a `std::pmr` container therefore takes a
 *       default-constructed allocator unless the allocator is named beside the
 *       value, which the constructors taking `std::allocator_arg_t` are for.
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
 * @var scl::basic_any::buffer_capacity
 * @brief Bytes of in-place storage. At least `sizeof(void *)`, so a smaller @p Capacity
 *        is rounded up to it.
 */

/**
 * @fn scl::basic_any::reserve_space_for()
 * @brief Acquires storage able to hold a @p ValueType, so storing one asks the
 *        allocator for nothing.
 * @tparam ValueType  The type the storage is meant for; decayed.
 *
 * @note A request, not a guarantee. A type that fits the in-place buffer needs no
 *       storage at all, an object already in the buffer keeps none, since its
 *       placement follows from its type, and an object whose move may throw cannot
 *       be relocated into wider storage. In each of those cases the call does
 *       nothing.
 *
 * @note An object already held is moved into the storage acquired, which therefore
 *       takes the wider of the two shapes. @ref scl::basic_any::emplace then asks the
 *       allocator for nothing. Assigning a value reaches the reserved block only on the
 *       terms @ref scl::basic_any::operator=(ValueType &&) states, and a handle
 *       assignment only where the block already holds an object, so either may still
 *       allocate.
 *
 * @note Run time only: the storage is raw bytes, which hold no object during constant
 *       evaluation. The same holds for @ref scl::basic_any::shrink_to_fit and
 *       @ref scl::basic_any::has_space_for.
 *
 * @note Storage acquired and never filled is given back by
 *       @ref scl::basic_any::reset, @ref scl::basic_any::shrink_to_fit and the
 *       destructor, and travels to the target on move and on swap.
 *
 * @throws Whatever the allocator throws.
 */

/**
 * @fn scl::basic_any::shrink_to_fit()
 * @brief Gives back storage wider than the object in it needs.
 *
 * @note A request, not a guarantee, on the terms
 *       @ref scl::basic_any::reserve_space_for states: an object in the buffer holds
 *       no block, and one whose move may throw cannot be relocated.
 *
 * @throws Whatever the allocator throws.
 */

/**
 * @fn scl::basic_any::has_space_for()
 * @brief Answers whether the storage already held has the room for a @p ValueType.
 * @tparam ValueType  The type asked about; decayed.
 * @return `true` where the type fits the in-place buffer, or the block already held
 *         has the space for it. Whether an operation then takes that room is the
 *         operation's own rule: @ref scl::basic_any::emplace always does, assigning
 *         a value does on the terms its own reference states.
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
 * Participates for every type but an any itself and a construction tag
 * (`std::allocator_arg_t`, a `std::in_place_type_t`), each of which selects its own
 * constructor rather than being stored. A handle is refused outright and stays storable
 * by name, as `any{std::in_place_type<any_view>, view}`; so is every array but one of
 * `const` characters, whose elements a value keeps by their address.
 *
 * @tparam ValueType  Deduced type of the stored object; the decayed form is
 *                    what gets stored.
 * @param  value  The object to store.
 */

/**
 * @fn scl::basic_any::basic_any(::std::allocator_arg_t tag, Allocator const & allocator, ValueType && value)
 * @brief Constructs an any acquiring its storage from @p allocator and holding a
 *        copy or a move of @p value.
 *
 * The allocator-extended form of the constructor above. `std::uses_allocator` is `false`
 * for this type, so nothing reaches this constructor but a caller naming the allocator
 * beside the value.
 *
 * @tparam ValueType  Deduced type of the stored object; the decayed form is
 *                    what gets stored.
 * @param  tag  Tag selecting this constructor.
 * @param  allocator  Allocator the any keeps.
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
 * @fn scl::basic_any::basic_any(::std::allocator_arg_t tag, Allocator const & allocator, ::std::in_place_type_t<ValueType> type, Arguments &&... arguments)
 * @brief Constructs the stored object in place from @p arguments, in storage
 *        acquired from @p allocator.
 * @tparam ValueType  Type to construct.
 * @tparam Arguments  Types of its constructor arguments.
 * @param  tag  Tag selecting this constructor.
 * @param  allocator  Allocator the any keeps.
 * @param  type  Tag naming the type to construct.
 * @param  arguments  Arguments forwarded to the constructor of @p ValueType.
 */

/**
 * @fn scl::basic_any::try_copy(SourceType const & source)
 * @brief Names the copy a container is to take, of the object @p source stands for.
 *
 * A handle stands for a value it does not own, and a container owns the value it
 * holds; either way the copy is of that value and never of @p source itself - the
 * mistake `std::any` cannot refuse to make, since its own constructor is an exact
 * match for a view and cannot be excluded.
 *
 * The result is consumed by a constructor or an assignment and reaches nothing on its
 * own. What consumes it is any specialisation of @ref scl::basic_any where @p source is
 * a handle, and the specialisation @p source itself is where it is a container: an
 * object is copied between two containers of one type, never between two of different
 * ones. It refers to @p source, so it outlives no full expression.
 *
 * Named rather than a conversion: whether the object can be copied is a run-time
 * answer, and an empty container is asked for rather than arrived at silently.
 *
 * @tparam SourceType  Deduced handle - @ref scl::any_view, @ref scl::any_mutable_view
 *         or @ref scl::any_argument - or a container.
 * @param  source  What the object to copy is reached through. Must outlive the call.
 * @return What a container takes to make the copy.
 */

/**
 * @fn scl::basic_any::basic_any(detail::any_referent_copy<Source> taken)
 * @brief Constructs an any holding the copy @p taken names, or an empty one where
 *        that copy cannot be made.
 *
 * An array leaves this any empty. A value naming one is refused where it is written; the
 * type an object reached through a handle has is not named at the call, so the refusal is a
 * run-time answer instead.
 * One array is the exception, a single extent of `const` characters, and what a value
 * keeps of it is the address of its first element. The elements outlive that address only
 * as far as the array itself does, which for a string literal is the whole program and for
 * a block-scope array is that block.
 *
 * The allocator comes from `allocator_traits::select_on_container_copy_construction`
 * where the copy is of another container, and is default-constructed where it is of a
 * handle's referent, a handle carrying no allocator of its own.
 *
 * @tparam Source  Deduced handle or container, as @ref scl::basic_any::try_copy took it.
 * @param  taken  What @ref scl::basic_any::try_copy answered.
 *
 * @note An object that cannot be copied - a `std::unique_ptr`, or anything else
 *       without a copy constructor - leaves this any empty. Whether it can be copied
 *       is not knowable at compile time, so this is a run-time outcome rather than a
 *       rejected call, and @ref scl::basic_any::has_value tells the two apart.
 *
 * @note During constant evaluation the object is reachable only where the source
 *       carries a container's own description of it and the allocator is the one that
 *       container holds. A handle over a plain lvalue or an anchor is not reachable
 *       there, and neither is a container with another allocator.
 *
 * @note Storing the handle itself stays available, and is spelled
 *       `any{std::in_place_type<any_view>, view}`.
 */

/**
 * @fn scl::basic_any::basic_any(::std::allocator_arg_t, Allocator const & allocator, detail::any_referent_copy<Source> taken)
 * @brief Constructs an any acquiring its storage from @p allocator and holding the
 *        copy @p taken names.
 *
 * Answers on the terms
 * @ref scl::basic_any::basic_any(detail::any_referent_copy<Source>) states; only the
 * allocator differs, being named here rather than derived from the source.
 *
 * @tparam Source  Deduced handle or container, as @ref scl::basic_any::try_copy took it.
 * @param  allocator  The allocator the result acquires storage from.
 * @param  taken  What @ref scl::basic_any::try_copy answered.
 */

/**
 * @fn scl::basic_any::operator=(detail::any_referent_copy<Source> taken)
 * @brief Destroys what this any holds and stores the copy @p taken names.
 * @tparam Source  Deduced handle or container, as @ref scl::basic_any::try_copy took it.
 * @param  taken  What @ref scl::basic_any::try_copy answered.
 * @return This any.
 *
 * @note An object that cannot be taken, having no copy constructor, still ends what
 *       the any held: assigning it leaves this any empty rather than keeping the old
 *       object.
 *
 * @note The copy is taken before what this any holds is destroyed, as in
 *       @ref scl::basic_any::operator=(ValueType &&), so an object the stored one owns
 *       is still alive when it is read. An allocated object replaced by one the block
 *       still holds, allocated as well, keeps the storage it already has on the same
 *       terms: the type moves without throwing and is no wider than 256 bytes.
 *
 * @note A source standing for the stored object itself asks for the value already
 *       held, so `value = scl::any::try_copy(scl::any_view{value})` does nothing - which
 *       is what keeps a value whose type has no copy constructor at all. A handle on a
 *       member of the stored object names another type and replaces it at run time.
 *
 * @note During constant evaluation the object is reachable only where the source carries a
 *       container's own description of it and the allocator is the one that container
 *       holds, exactly as in @ref scl::basic_any::basic_any(detail::any_referent_copy<Source>).
 *       A handle over a plain lvalue, over a member of the stored object or over an anchor
 *       empties this any there rather than replacing what it holds.
 *
 * @note The allocator of this any stays: only it may release what it acquired.
 */

/**
 * @fn scl::basic_any::basic_any(basic_any const &)
 * @brief Deleted: an any holds objects that need not be copyable, so copying is asked for
 *        rather than performed implicitly.
 *
 * A constructor has no way to report that the stored type has no copy constructor, and
 * refusing the whole any for such a type would cost more than it buys. Copying therefore
 * goes through @ref scl::basic_any::try_copy, which answers an empty any instead, with
 * @ref scl::basic_any::is_copyable telling the two outcomes apart beforehand.
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
 * An allocated object replaced by one the block still holds, allocated as well,
 * keeps the storage it already has, so the allocator is not asked again. The
 * block admits a narrower type too, since it carries the room it was taken with.
 * That holds while the stored type moves without throwing and is no wider than
 * 256 bytes, which is what a value taken aside for the rebuild costs; beyond it
 * a fresh allocation is asked for.
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
 * An allocated object whose replacement the block still holds keeps the storage it
 * already has, so the allocator is not asked again. The block is laid out for the
 * type going into it, so the object's address may move inside it. Destroying first
 * is what lets that happen for any type, where assigning a value reaches it only
 * for a type it can take aside.
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
 * @return `true` while an object is held, even one that is itself empty; `false`
 *         for an empty any.
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
 * @fn scl::basic_any::is_copyable() const
 * @brief Reports whether the stored type can be copied.
 *
 * A question about the type, not about the value, so an empty any answers
 * `false`. Reading it ahead of a copy is what tells an empty result apart from a
 * copy of nothing.
 *
 * @return `true` when a copy through @ref scl::basic_any::try_copy would reproduce the
 *         object.
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
