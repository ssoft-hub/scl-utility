#pragma once

/**
 * @file base.h
 * @brief Shared storage and type-identity machinery for the ScL Any views.
 */

#include <scl/utility/meta/type_key.h>
#include <scl/utility/preprocessor/rtti.h>

#include <concepts>
#include <cstddef>
#include <memory>
#include <new>
#include <string_view>
#include <type_traits>
#include <utility>

#if SCL_HAS_RTTI
#include <any>
#endif

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

    // How a handle remembers its referent during constant evaluation: a shared descriptor
    // stands for the object's address, an owner-made one for the holder, and an anchor for
    // the typed pointer it carries itself.
    enum class any_binding : unsigned char
    {
        object = 0,
        holder = 1,
        anchor = 2
    };

    template <typename Type>
    [[nodiscard]]
    constexpr any_qualifier any_qualifiers_of() noexcept
        requires ::std::is_reference_v<Type>
    {
        using referent = ::std::remove_reference_t<Type>;

        // Named rather than tested in place: a variable template in the condition of a
        // conditional expression defeats static analysers that parse templates heuristically.
        constexpr bool has_const = ::std::is_const_v<referent>;
        constexpr bool has_volatile = ::std::is_volatile_v<referent>;

        return (has_const ? any_qualifier::const_qualified : any_qualifier::none) |
            (has_volatile ? any_qualifier::volatile_qualified : any_qualifier::none);
    }

    // Sound only after any_base::binding_accepts proved Target covers the referent's qualifiers.
    template <typename Target>
    [[nodiscard]]
    constexpr Target * erased_cast(void const volatile * object) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): guarded by binding_accepts()
        return static_cast<Target *>(const_cast<void *>(object));
    }

    // Empty on purpose: a vtable would cost the buffer a pointer, and the downcast below
    // would stop being a constant expression.
    struct any_holder_base
    {};

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(push)
// A type a view may not destroy is described rather than refused, so its holder never is.
#pragma warning(disable: 4624)
#endif

    template <typename Type>
    struct any_holder : any_holder_base
    {
        Type value;

        // Parenthesised: braces would read (3U, 'x') as a narrowing list, not a constructor call.
        template <typename... Arguments>
        constexpr explicit any_holder(Arguments &&... arguments)
            : value(::std::forward<Arguments>(arguments)...)
        {}
    };

#if defined(_MSC_VER) && !defined(__clang__)
#pragma warning(pop)
#endif

    template <typename Type>
    [[nodiscard]]
    constexpr Type * any_holder_object(any_holder_base * held) noexcept
    {
        return ::std::addressof(static_cast<any_holder<Type> *>(held)->value);
    }

    template <typename Type>
    [[nodiscard]]
    constexpr Type const * any_holder_object(any_holder_base const * held) noexcept
    {
        return ::std::addressof(static_cast<any_holder<Type> const *>(held)->value);
    }

    // Rebuilding in place costs a copy on the stack; past this width fresh storage is the
    // cheaper answer.
    inline constexpr ::std::size_t any_widest_reuse_copy = 256U;

    // Every operation below takes the storage it works in rather than acquiring it, so none
    // of them needs an allocator - which is what lets a view carry them for its referent.
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

    // The referent decays on the way in, the way a value does when it reaches an any: an
    // array is taken as the pointer it decays to, since an array cannot be copied at all.
    template <typename Type>
    [[nodiscard]]
    any_holder_base * any_place_copy(void * storage, void const * source)
        requires ::std::constructible_from<::std::decay_t<Type>, ::std::remove_reference_t<Type> const &>
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

    // The copy stands aside while the target ends, so the source may be what that object
    // owns; the type is known here, which is what makes room for the copy on the stack.
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

    // Only the type says where inside the holder its object sits, so a view asks for it.
    // Constexpr, unlike the storage-taking operations beside it: the address comes from a
    // holder reached by a downcast, which a constant expression allows.
    template <typename Type>
    [[nodiscard]]
    constexpr void const * any_object_at(any_holder_base const * held) noexcept
    {
        return static_cast<void const *>(any_holder_object<Type>(held));
    }

    // A type that cannot be copied carries no copy operation at all, rather than one that
    // reports failure: a reader answers whether a copy is possible by reading this.
    template <typename Type>
    [[nodiscard]]
    constexpr any_place_function any_place_operation_of() noexcept
    {
        // A volatile referent is why the source form appears here rather than just the
        // decayed one: a copy constructor that takes `const &` cannot read it.
        if constexpr (::std::constructible_from<::std::decay_t<Type>, ::std::remove_reference_t<Type> const &>)
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

    // A view destroys nothing, so a referent whose destructor it may not call is described
    // without the operation rather than refused.
    template <typename Type>
    [[nodiscard]]
    constexpr any_erase_function any_erase_operation_of() noexcept
    {
        if constexpr (::std::destructible<Type>)
            return &any_erase_at<Type>;
        else
            return nullptr;
    }

    // Absent for a type the copy cannot stand aside for: a throwing move would end with
    // neither object, and a wide one would spend the stack an owner cannot bound.
    template <typename Type>
    [[nodiscard]]
    constexpr any_rebuild_function any_rebuild_operation_of() noexcept
    {
        using bare = ::std::decay_t<Type>;

        if constexpr (::std::constructible_from<bare, ::std::remove_reference_t<Type> const &> &&
            ::std::is_nothrow_move_constructible_v<bare> && sizeof(bare) <= any_widest_reuse_copy)
            return &any_rebuild_from<Type>;
        else
            return nullptr;
    }

    // Per cv-ref form of a referent; how a value was bound never changes its type.
    struct any_type_descriptor
    {
        // One key object per bare type, so identity is a pointer comparison within a module.
        ::scl::type_key const * type;
        any_qualifier qualifiers;
        // Beside the qualifiers: the alignment after them already holds this byte, so the
        // descriptor does not grow by carrying it.
        any_binding binding;
        // Already-const forms point at themselves, which terminates the chain.
        any_type_descriptor const * as_const;
        // What `place` builds: the referent's decayed form, which is what an owner ends up
        // describing. A plain type points at its own unqualified form.
        any_type_descriptor const * as_value;

        // What an owner needs to take the referent in, and nothing an owner supplies itself:
        // the storage comes from the owner, so the allocator stays out of here.
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

#if SCL_HAS_RTTI
    template <typename Type>
    inline constexpr bool is_std_any_v = ::std::same_as<Type, ::std::any>;
#else
    // Without RTTI this header cannot name std::any, so on a toolchain that still declares
    // it a caller passing one binds the raw constructor instead of being unwrapped.
    template <typename Type>
    inline constexpr bool is_std_any_v = false;
#endif

    // A base, not a trait: no include order can leave one translation unit seeing the
    // exclusion and another not.
    struct any_owner_tag
    {};

    template <typename Type>
    inline constexpr bool any_construction_tag_v = ::std::same_as<Type, ::std::allocator_arg_t>;

    template <typename Type>
    inline constexpr bool any_construction_tag_v<::std::in_place_type_t<Type>> = true;

    // The one place an owner reads a handle. Both views befriend it rather than the owner,
    // which is a template and would otherwise have to be befriended per instantiation.
    struct any_handle_access
    {
        template <typename Handle>
        [[nodiscard]]
        static constexpr any_type_descriptor const * descriptor(Handle const & handle) noexcept
        {
            return handle.descriptor();
        }

        // A taker needs the holder rather than the address, since the copy it makes is
        // typed and an address is not.
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
    };

    class any_base
    {
    public:
        using descriptor_type = any_type_descriptor;
        using name = any_name;

    private:
        // Only constant evaluation activates the holder, and it never reaches run time: an
        // owning any allocates there, and such an allocation dies inside the evaluation.
        union
        {
            void const volatile * m_object = nullptr;
            any_holder_base const * m_held;
        };
        descriptor_type const * m_descriptor = nullptr;

    public:
        // A lone `const volatile` accessor would count every read as a volatile access
        // during constant evaluation, costing constexpr where the object is not volatile.
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

        // Both shapes arrive and the body picks the live one: a mem-initializer cannot
        // choose a union member on a condition.
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

        // Hands the referent on as it is held: passing the address would fix one shape and
        // lose the other.
        constexpr any_base(any_base const & bound, descriptor_type const * descriptor) noexcept
            : any_base{bound}
        {
            m_descriptor = descriptor;
        }

        // Anywhere but constant evaluation over an owner this reads the union's other
        // member, which constant evaluation diagnoses.
        [[nodiscard]]
        constexpr any_holder_base const * held() const noexcept
        {
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-union-access): the union is the referent
            return m_held;
        }

        // Declared for a volatile handle, which compiles the constant-evaluation branch of
        // a cast without ever reaching it: no volatile object exists in a constant expression.
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

        // A handle's own cv-qualification governs the handle and not what it refers to, so
        // the rights come from the binding alone.
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

#if SCL_HAS_RTTI
        // std::any is an external type, so its key compares equal across module boundaries.
#ifdef __cpp_explicit_this_parameter
        template <typename Self>
        [[nodiscard]]
        constexpr ::std::any const * std_any(this Self && self) noexcept
        {
            if (self.m_descriptor == nullptr)
                return nullptr;
            if (*self.m_descriptor->type != ::scl::type_key_of<::std::any>())
                return nullptr;
            // No constructor binds a volatile std::any, so dropping the qualifier is safe.
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
            return static_cast<::std::any const *>(const_cast<void const *>(self.m_object));
        }
#else
        [[nodiscard]]
        constexpr ::std::any const * std_any() const noexcept
        {
            if (m_descriptor == nullptr)
                return nullptr;
            if (*m_descriptor->type != ::scl::type_key_of<::std::any>())
                return nullptr;
            // No constructor binds a volatile std::any, so dropping the qualifier is safe.
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast,cppcoreguidelines-pro-type-union-access)
            return static_cast<::std::any const *>(const_cast<void const *>(m_object));
        }

        [[nodiscard]]
        ::std::any const * std_any() const volatile noexcept
        {
            if (m_descriptor == nullptr)
                return nullptr;
            if (*m_descriptor->type != ::scl::type_key_of<::std::any>())
                return nullptr;
            // No constructor binds a volatile std::any, so dropping the qualifier is safe.
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast,cppcoreguidelines-pro-type-union-access)
            return static_cast<::std::any const *>(const_cast<void const *>(m_object));
        }
#endif
#endif
    };
} // namespace scl::detail
