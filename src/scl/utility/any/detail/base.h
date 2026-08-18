#pragma once

/**
 * @file base.h
 * @brief Shared storage and type-identity machinery for the ScL Any views.
 * @ingroup scl_utility_any
 */

#include <scl/utility/meta/type_key.h>
#include <scl/utility/preprocessor/rtti.h>

#include <concepts>
#include <string_view>
#include <type_traits>

#if SCL_HAS_RTTI
#include <any>
#endif

namespace scl::detail
{
    using any_view_name = ::std::string_view;

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

    // Sound only after any_base::accepts proved Target covers the referent's qualifiers.
    template <typename Target>
    [[nodiscard]]
    constexpr Target * erased_cast(void const volatile * object) noexcept
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): guarded by accepts()
        return static_cast<Target *>(const_cast<void *>(object));
    }

    // Per cv-ref form of a referent; how a value was bound never changes its type.
    struct any_view_descriptor
    {
        // One key object per bare type, so identity is a pointer comparison within a module.
        ::scl::type_key const * type;
        any_qualifier qualifiers;
        // Already-const forms point at themselves, which terminates the chain.
        any_view_descriptor const * as_const;
    };

    template <typename Type>
    inline constexpr any_view_descriptor any_view_descriptor_of{
        .type = &::scl::type_key_of<::std::remove_cvref_t<Type>>(),
        .qualifiers = any_qualifiers_of<Type>(),
        .as_const = &any_view_descriptor_of<::std::remove_reference_t<Type> const &>};

#if SCL_HAS_RTTI
    template <typename Type>
    inline constexpr bool is_std_any_v = ::std::same_as<Type, ::std::any>;
#else
    // Without RTTI this header cannot name std::any, so on a toolchain that still declares
    // it a caller passing one binds the raw constructor instead of being unwrapped.
    template <typename Type>
    inline constexpr bool is_std_any_v = false;
#endif

    class any_base
    {
    public:
        using descriptor_type = any_view_descriptor;
        using name = any_view_name;

    private:
        void const volatile * m_object = nullptr;
        descriptor_type const * m_descriptor = nullptr;

    public:
        // A lone `const volatile` accessor would count every read as a volatile access
        // during constant evaluation, costing constexpr where the object is not volatile.
#ifdef __cpp_explicit_this_parameter
        template <typename Self>
        [[nodiscard]]
        constexpr bool has_value(this Self && self) noexcept
        {
#if SCL_HAS_RTTI
            if (auto const * boxed = self.std_any())
                return boxed->has_value();
#endif
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
#if SCL_HAS_RTTI
            if (auto const * boxed = std_any())
                return boxed->has_value();
#endif
            return m_descriptor != nullptr;
        }

        [[nodiscard]]
        bool has_value() const volatile noexcept
        {
#if SCL_HAS_RTTI
            if (auto const * boxed = std_any())
                return boxed->has_value();
#endif
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
            : m_object{object}
            , m_descriptor{descriptor}
        {}

        // The request must cover every cv-qualifier of the referent and of the handle it
        // is reached through; the latter is static, so it comes from the object parameter.
#ifdef __cpp_explicit_this_parameter
        template <typename Self>
        [[nodiscard]]
        constexpr void const volatile * object(this Self && self) noexcept
        {
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

        template <typename Self>
        [[nodiscard]]
        constexpr bool accepts(this Self && self, any_qualifier requested) noexcept
        {
            return self.m_descriptor != nullptr &&
                ((self.m_descriptor->qualifiers | any_qualifiers_of<::std::remove_reference_t<Self> &>()) &
                    ~requested) == any_qualifier::none;
        }
#else
        [[nodiscard]]
        constexpr void const volatile * object() const noexcept
        {
            return m_object;
        }

        [[nodiscard]]
        void const volatile * object() const volatile noexcept
        {
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

        [[nodiscard]]
        constexpr bool accepts(any_qualifier requested) noexcept
        {
            return m_descriptor != nullptr && (m_descriptor->qualifiers & ~requested) == any_qualifier::none;
        }

        [[nodiscard]]
        constexpr bool accepts(any_qualifier requested) const noexcept
        {
            return m_descriptor != nullptr &&
                ((m_descriptor->qualifiers | any_qualifier::const_qualified) & ~requested) == any_qualifier::none;
        }

        [[nodiscard]]
        bool accepts(any_qualifier requested) volatile noexcept
        {
            return m_descriptor != nullptr &&
                ((m_descriptor->qualifiers | any_qualifier::volatile_qualified) & ~requested) ==
                any_qualifier::none;
        }

        [[nodiscard]]
        bool accepts(any_qualifier requested) const volatile noexcept
        {
            return m_descriptor != nullptr &&
                ((m_descriptor->qualifiers | any_qualifier::const_qualified | any_qualifier::volatile_qualified) &
                    ~requested) == any_qualifier::none;
        }
#endif

        // For a handle whose own cv-qualification is fixed by its declaration and so states
        // nothing, the rights come from the binding alone.
        [[nodiscard]]
        constexpr bool binding_accepts(any_qualifier requested) const noexcept
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
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
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
            // NOLINTNEXTLINE(cppcoreguidelines-pro-type-const-cast): see above
            return static_cast<::std::any const *>(const_cast<void const *>(m_object));
        }
#endif
#endif
    };
} // namespace scl::detail
