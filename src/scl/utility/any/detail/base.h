#pragma once

/**
 * @file base.h
 * @brief Shared storage and type-identity machinery for the ScL Any views.
 * @ingroup scl_utility_any
 */

#include <scl/utility/concepts/reference.h>
#include <scl/utility/meta/type_key.h>
#include <scl/utility/type_traits/forward_like.h>

#include <cstddef>
#include <memory>
#include <new>
#include <string_view>
#include <type_traits>
#include <utility>

/**
 * @internal
 * @def SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST
 * @brief Whether P2738 (C++26) is available, making the recovery of a typed
 *        pointer from `void const *` a constant expression.
 *
 * That recovery is what every workaround in this group exists to stand in for,
 * and several branches turn on it — spelled once here rather than comparing
 * `__cpp_constexpr` against a bare 202306L at each of them.
 */
#if defined(__cpp_constexpr) && __cpp_constexpr >= 202306L
#define SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST 1
#else
#define SCL_DETAIL_ANY_HAS_CONSTEXPR_VOID_CAST 0
#endif

namespace scl::detail
{
    using any_name = ::std::string_view;

    enum class any_qualifier : unsigned char
    {
        none = 0,
        const_qualified = 1U << 0U,
        volatile_qualified = 1U << 1U
    };

    [[nodiscard]]
    constexpr any_qualifier operator|(any_qualifier left, any_qualifier right) noexcept
    {
        return static_cast<any_qualifier>(static_cast<unsigned char>(left) |
            static_cast<unsigned char>(right));
    }

    [[nodiscard]]
    constexpr any_qualifier operator&(any_qualifier left, any_qualifier right) noexcept
    {
        return static_cast<any_qualifier>(static_cast<unsigned char>(left) &
            static_cast<unsigned char>(right));
    }

    [[nodiscard]]
    constexpr any_qualifier operator~(any_qualifier value) noexcept
    {
        return static_cast<any_qualifier>(~static_cast<unsigned char>(value));
    }

    // How a handle remembers its referent at compile time: a shared descriptor stands for the
    // address, an owner-made one for the holder, an anchor for the typed pointer it carries.
    enum class any_binding : unsigned char
    {
        object = 0,
        holder = 1,
        anchor = 2
    };

    template <::scl::concepts::reference Type>
    [[nodiscard]]
    constexpr any_qualifier any_qualifiers_of() noexcept
    {
        using referent = ::std::remove_reference_t<Type>;

        constexpr bool has_const = ::std::is_const_v<referent>;
        constexpr bool has_volatile = ::std::is_volatile_v<referent>;

        return (has_const ? any_qualifier::const_qualified : any_qualifier::none) |
            (has_volatile ? any_qualifier::volatile_qualified : any_qualifier::none);
    }

    template <typename Target>
    [[nodiscard]]
    constexpr Target * erased_cast(void const volatile * object) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): guarded by binding_accepts()
        return static_cast<Target *>(const_cast<void *>(object));
    }

    // Empty on purpose: a vtable would cost a pointer and stop the downcast folding.
    struct any_holder_base
    {};

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
// A type a view may not destroy is described rather than refused.
#pragma warning(disable: 4624)
#endif

    template <typename Type>
    struct any_holder : any_holder_base
    {
        Type value;

        template <typename... Arguments>
        constexpr explicit any_holder(Arguments &&... arguments)
            : value(::std::forward<Arguments>(arguments)...)
        {}
    };

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif

    template <typename Type, typename Held>
    [[nodiscard]]
    constexpr auto * any_holder_object(Held * held) noexcept
    {
        using holder = ::std::remove_reference_t<::scl::forward_like_t<Held, any_holder<Type>>>;

        return ::std::addressof(static_cast<holder *>(held)->value);
    }

    // Rebuilding in place costs a stack copy; past this width fresh storage is cheaper.
    inline constexpr ::std::size_t any_widest_reuse_copy = 256U;

    // Every operation takes the storage it works in, so a view can carry them for its referent.
    using any_place_function = any_holder_base * (*)(void * storage, void const * source);
    using any_erase_function = void (*)(any_holder_base * held) noexcept;
    struct any_rebuild_target
    {
        any_holder_base * held;
        any_erase_function end;
    };

    using any_rebuild_function = any_holder_base * (*)(any_rebuild_target target, void const * source);
    using any_move_function = any_holder_base * (*)(void * storage, any_holder_base * source) noexcept;
    using any_reach_function = any_holder_base * (*)(void * storage) noexcept;
    using any_object_function = void const * (*)(any_holder_base const * held) noexcept;

    // The referent decays on the way in: an array cannot be copied at all.
    template <typename Type>
    [[nodiscard]]
    any_holder_base * any_place_copy(void * storage, void const * source)
        requires ::std::is_constructible_v<::std::decay_t<Type>, ::std::remove_reference_t<Type> const &> &&
        ::std::is_nothrow_destructible_v<::std::decay_t<Type>>
    {
        using referent = ::std::remove_reference_t<Type>;
        using holder = any_holder<::std::decay_t<Type>>;

        return ::std::construct_at(static_cast<holder *>(storage), *static_cast<referent const *>(source));
    }

    template <typename Type>
    void any_erase_at(any_holder_base * held) noexcept
    {
        ::std::destroy_at(static_cast<any_holder<Type> *>(held));
    }

    // The copy stands aside while the target ends, so the source may be what it owns.
    template <typename Type>
    [[nodiscard]]
    any_holder_base * any_rebuild_from(any_rebuild_target target, void const * source)
        requires ::std::constructible_from<::std::decay_t<Type>, ::std::remove_reference_t<Type> const &> &&
        ::std::is_nothrow_move_constructible_v<::std::decay_t<Type>>
    {
        using referent = ::std::remove_reference_t<Type>;
        using holder = any_holder<::std::decay_t<Type>>;

        void * const storage = target.held;
        ::std::decay_t<Type> taken(*static_cast<referent const *>(source));

        target.end(target.held);
        return ::std::construct_at(static_cast<holder *>(storage), ::std::move(taken));
    }

    template <typename Type>
    [[nodiscard]]
    any_holder_base * any_move_to(void * storage, any_holder_base * source) noexcept
        requires ::std::is_nothrow_move_constructible_v<Type>
    {
        auto * const moved = ::std::construct_at(static_cast<any_holder<Type> *>(storage),
            ::std::move(*any_holder_object<Type>(source)));
        any_erase_at<Type>(source);
        return moved;
    }

    // Laundered: the bytes behind the pointer were reused by placement new.
    template <typename Type>
    [[nodiscard]]
    any_holder_base * any_reach_at(void * storage) noexcept
    {
        return ::std::launder(static_cast<any_holder<Type> *>(storage));
    }

    template <typename Type>
    [[nodiscard]]
    constexpr void const * any_object_at(any_holder_base const * held) noexcept
    {
        return static_cast<void const *>(any_holder_object<Type>(held));
    }

    template <typename Type>
    [[nodiscard]]
    constexpr any_place_function any_place_operation_of() noexcept
    {
        if constexpr (::std::is_constructible_v<::std::decay_t<Type>, ::std::remove_reference_t<Type> const &> &&
            ::std::is_nothrow_destructible_v<::std::decay_t<Type>>)
            return &any_place_copy<Type>;
        else
            return nullptr;
    }

    template <typename Type>
    [[nodiscard]]
    constexpr any_move_function any_move_operation_of() noexcept
    {
        if constexpr (::std::is_nothrow_move_constructible_v<Type>)
            return &any_move_to<Type>;
        else
            return nullptr;
    }

    template <typename Type>
    [[nodiscard]]
    constexpr any_erase_function any_erase_operation_of() noexcept
    {
        if constexpr (::std::is_nothrow_destructible_v<Type>)
            return &any_erase_at<Type>;
        else
            return nullptr;
    }

    template <typename Type>
    [[nodiscard]]
    constexpr any_rebuild_function any_rebuild_operation_of() noexcept
    {
        using bare = ::std::decay_t<Type>;

        if constexpr (::std::is_constructible_v<bare, ::std::remove_reference_t<Type> const &> &&
            ::std::is_nothrow_destructible_v<bare> &&
            ::std::is_nothrow_move_constructible_v<bare> && sizeof(bare) <= any_widest_reuse_copy)
            return &any_rebuild_from<Type>;
        else
            return nullptr;
    }

    // Per cv-ref form of a referent; how a value was bound never changes its type.
    struct any_type_descriptor
    {
        ::scl::type_key const * type;
        any_qualifier qualifiers;
        any_binding binding;
        any_type_descriptor const * as_const;
        any_type_descriptor const * as_value;

        // Nothing an owner supplies itself: the storage comes from it, so no allocator here.
        ::std::size_t size;
        ::std::size_t alignment;
        any_place_function place;
        any_rebuild_function rebuild;
        any_erase_function erase;
        any_move_function move;
        any_reach_function reach;
        any_object_function object;
    };

    template <typename Type>
    inline constexpr any_type_descriptor any_type_descriptor_of{
        .type = &::scl::type_key_of<::std::remove_cvref_t<Type>>(),
        .qualifiers = any_qualifiers_of<Type>(),
        .binding = any_binding::object,
        .as_const = &any_type_descriptor_of<::std::remove_reference_t<Type> const &>,
        .as_value = &any_type_descriptor_of<::std::decay_t<Type> &>,
        .size = sizeof(any_holder<::std::remove_cvref_t<Type>>),
        .alignment = alignof(any_holder<::std::remove_cvref_t<Type>>),
        .place = any_place_operation_of<Type>(),
        .rebuild = any_rebuild_operation_of<Type>(),
        .erase = any_erase_operation_of<::std::remove_cvref_t<Type>>(),
        .move = any_move_operation_of<::std::remove_cvref_t<Type>>(),
        .reach = &any_reach_at<::std::remove_cvref_t<Type>>,
        .object = &any_object_at<::std::remove_cvref_t<Type>>};

    // A base, not a trait: no include order can leave one translation unit disagreeing.
    struct any_owner
    {};

    template <typename Type>
    inline constexpr bool is_any_construction_tag_v = ::std::is_same_v<Type, ::std::allocator_arg_t>;

    template <typename Type>
    inline constexpr bool is_any_construction_tag_v<::std::in_place_type_t<Type>> = true;

    [[nodiscard]]
    constexpr any_type_descriptor any_anchored_form(any_type_descriptor described) noexcept
    {
        described.binding = any_binding::anchor;
        return described;
    }

    template <typename Type>
    struct any_anchored_descriptor : any_type_descriptor
    {
        constexpr explicit any_anchored_descriptor(any_type_descriptor const & descriptor,
            Type * referent = nullptr) noexcept
            : any_type_descriptor{any_anchored_form(descriptor)}
            , referent{referent}
        {}

        Type * referent;
    };

    // The one place an owner reads a handle, befriended instead of the owner template.
    struct any_handle_access
    {
        template <typename Handle>
        [[nodiscard]]
        static constexpr any_type_descriptor const * descriptor(Handle const & handle) noexcept
        {
            return handle.descriptor();
        }

        template <typename Handle>
        [[nodiscard]]
        static constexpr any_holder_base const * held(Handle const & handle) noexcept
        {
            return handle.held();
        }

        template <typename Handle>
        [[nodiscard]]
        static void const * referent(Handle const & handle) noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): read through a const handle
            return const_cast<void const *>(handle.object());
        }

        template <typename Handle>
        [[nodiscard]]
        static constexpr void const volatile * object(Handle const & handle) noexcept
        {
            return handle.object();
        }

        template <typename Handle>
        [[nodiscard]]
        static constexpr bool binding_accepts(Handle const & handle, any_qualifier requested) noexcept
        {
            return handle.binding_accepts(requested);
        }
    };

    // One place answers what a binding reaches. Request carries the qualifiers the caller must
    // cover: a read spells `Type const`, a write spells `Type`.
    template <typename Request, typename Handle>
    [[nodiscard]]
    constexpr Request * any_referent_of(Handle const & handle) noexcept
    {
        using bare = ::std::remove_cvref_t<Request>;

        auto const * const described = any_handle_access::descriptor(handle);
        if (described == nullptr) [[unlikely]]
            return nullptr;
        if (!any_handle_access::binding_accepts(handle, any_qualifiers_of<Request &>()))
            return nullptr;
        // A handle names the type it is bound to and nothing that type may itself contain.
        if (*described->type != ::scl::type_key_of<bare>())
            return nullptr;

        if (::std::is_constant_evaluated())
        {
            // Recovering a typed pointer from `void const *` is no constant expression before
            // P2738, while a downcast to what the object really is - an anchor, or a holder - is one.
            if (described->binding == any_binding::anchor)
                return static_cast<any_anchored_descriptor<bare> const *>(described)->referent;

            // Only this branch may read the holder: elsewhere it is the union's inactive member.
            if (described->binding == any_binding::holder)
                // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): covered above
                return const_cast<bare *>(any_holder_object<bare>(any_handle_access::held(handle)));
        }

        [[likely]] return erased_cast<Request>(any_handle_access::object(handle));
    }

    class any_base
    {
    public:
        using descriptor_type = any_type_descriptor;
        using name = any_name;

    private:
        // Only constant evaluation activates the holder, and its allocation dies inside it.
        union
        {
            void const volatile * m_object = nullptr;
            any_holder_base const * m_held;
        };
        descriptor_type const * m_descriptor = nullptr;

    public:
        // A lone `const volatile` accessor would cost constexpr on a non-volatile object.
#ifdef __cpp_explicit_this_parameter
        template <typename Self>
        [[nodiscard]]
        constexpr bool has_value(this Self && self) noexcept
        {
            return self.m_descriptor != nullptr;
        }

        template <typename Self>
        [[nodiscard]]
        constexpr name type_name(this Self && self) noexcept
        {
            return (self.m_descriptor != nullptr) ? self.m_descriptor->type->name() : name{};
        }

        template <typename Self>
        [[nodiscard]]
        constexpr ::scl::type_key type_key(this Self && self) noexcept
        {
            return self.m_descriptor != nullptr ? *self.m_descriptor->type : ::scl::type_key{};
        }
#else
        [[nodiscard]]
        constexpr bool has_value() const noexcept
        {
            return m_descriptor != nullptr;
        }

        [[nodiscard]]
        bool has_value() const volatile noexcept
        {
            return m_descriptor != nullptr;
        }

        [[nodiscard]]
        constexpr name type_name() const noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->type->name() : name{};
        }

        [[nodiscard]]
        name type_name() const volatile noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->type->name() : name{};
        }

        [[nodiscard]]
        constexpr ::scl::type_key type_key() const noexcept
        {
            return m_descriptor != nullptr ? *m_descriptor->type : ::scl::type_key{};
        }

        [[nodiscard]]
        ::scl::type_key type_key() const volatile noexcept
        {
            return m_descriptor != nullptr ? *m_descriptor->type : ::scl::type_key{};
        }
#endif

    protected:
        constexpr any_base() noexcept = default;

        constexpr any_base(void const volatile * object, descriptor_type const * descriptor) noexcept
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            : m_object{object}
            , m_descriptor{descriptor}
        {}

        // The body picks the live shape: a mem-initializer cannot choose a union member.
        constexpr any_base(
            any_holder_base const * held, void const volatile * object, descriptor_type const * descriptor) noexcept
            : m_descriptor{descriptor}
        {
            // NOLINTBEGIN(cppcoreguidelines-pro-type-union-access): the union is the referent
            if (::std::is_constant_evaluated())
                m_held = held;
            else
                m_object = object;
            // NOLINTEND(cppcoreguidelines-pro-type-union-access)
        }

        // Hands the referent on as held: an address would fix one shape and lose the other.
        constexpr any_base(any_base const & bound, descriptor_type const * descriptor) noexcept
            : any_base{bound}
        {
            m_descriptor = descriptor;
        }

        // Outside constant evaluation over an owner this reads the union's other member.
        [[nodiscard]]
        constexpr any_holder_base const * held() const noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            return m_held;
        }

        // Declared for a volatile handle: it compiles the compile-time branch without reaching it.
        [[nodiscard]]
        any_holder_base const * held() const volatile noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            return m_held;
        }

#ifdef __cpp_explicit_this_parameter
        template <typename Self>
        [[nodiscard]]
        constexpr void const volatile * object(this Self && self) noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            return self.m_object;
        }

        template <typename Self>
        [[nodiscard]]
        constexpr descriptor_type const * descriptor(this Self && self) noexcept
        {
            return self.m_descriptor;
        }

        // The single place a binding is narrowed to read-only.
        template <typename Self>
        [[nodiscard]]
        constexpr descriptor_type const * const_descriptor(this Self && self) noexcept
        {
            return (self.m_descriptor != nullptr) ? self.m_descriptor->as_const : nullptr;
        }
#else
        [[nodiscard]]
        constexpr void const volatile * object() const noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            return m_object;
        }

        [[nodiscard]]
        void const volatile * object() const volatile noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            return m_object;
        }

        [[nodiscard]]
        constexpr descriptor_type const * descriptor() const noexcept
        {
            return m_descriptor;
        }

        [[nodiscard]]
        descriptor_type const * descriptor() const volatile noexcept
        {
            return m_descriptor;
        }

        // The single place a binding is narrowed to read-only.
        [[nodiscard]]
        constexpr descriptor_type const * const_descriptor() const noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->as_const : nullptr;
        }

        [[nodiscard]]
        descriptor_type const * const_descriptor() const volatile noexcept
        {
            return (m_descriptor != nullptr) ? m_descriptor->as_const : nullptr;
        }
#endif

        // A handle's own cv-qualification governs the handle, so rights come from the binding.
        [[nodiscard]]
        constexpr bool binding_accepts(any_qualifier requested) const noexcept
        {
            return m_descriptor != nullptr && (m_descriptor->qualifiers & ~requested) == any_qualifier::none;
        }

        [[nodiscard]]
        bool binding_accepts(any_qualifier requested) const volatile noexcept
        {
            return m_descriptor != nullptr && (m_descriptor->qualifiers & ~requested) == any_qualifier::none;
        }
    };
} // namespace scl::detail
