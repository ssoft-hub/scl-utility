#pragma once

/// @file
/// @brief Type-property concepts.
/// @ingroup scl_utility_concepts

#include <type_traits>

namespace scl::concepts
{

    /// @brief Satisfied when T is a trivial type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Pod { int x; };
    /// struct WithCtor { WithCtor() {} };
    ///
    /// static_assert( scl::concepts::trivial<int>);
    /// static_assert( scl::concepts::trivial<Pod>);
    /// static_assert(!scl::concepts::trivial<WithCtor>);
    /// static_assert(!scl::concepts::trivial<std::string>);
    /// @endcode
    template <typename T>
    concept trivial = ::std::is_trivial_v<T>;

    /// @brief Satisfied when T is trivially copyable.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Pod { int x; };
    ///
    /// static_assert( scl::concepts::trivially_copyable<int>);
    /// static_assert( scl::concepts::trivially_copyable<Pod>);
    /// static_assert(!scl::concepts::trivially_copyable<std::string>);
    ///
    /// // Safe to use std::memcpy for any trivially copyable type:
    /// template <scl::concepts::trivially_copyable T>
    /// void raw_copy(T* dst, const T* src, std::size_t n)
    /// {
    ///     std::memcpy(dst, src, n * sizeof(T));
    /// }
    /// @endcode
    template <typename T>
    concept trivially_copyable = ::std::is_trivially_copyable_v<T>;

    /// @brief Satisfied when T has standard layout.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Flat { int x; float y; };
    /// struct WithVirt { virtual void f(); };
    ///
    /// static_assert( scl::concepts::standard_layout<int>);
    /// static_assert( scl::concepts::standard_layout<Flat>);
    /// static_assert(!scl::concepts::standard_layout<WithVirt>);
    ///
    /// // Standard-layout types are safe to pass across FFI boundaries:
    /// template <scl::concepts::standard_layout T>
    /// void ffi_send(const T& value);
    /// @endcode
    template <typename T>
    concept standard_layout = ::std::is_standard_layout_v<T>;

    /// @brief Satisfied when T is an empty class (no non-static data members, no padding).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Tag {};
    /// struct WithData { int x; };
    ///
    /// static_assert( scl::concepts::empty_type<Tag>);
    /// static_assert(!scl::concepts::empty_type<WithData>);
    /// static_assert(!scl::concepts::empty_type<int>);
    ///
    /// // EBO: empty base adds zero overhead when it satisfies empty_type:
    /// template <scl::concepts::empty_type Policy>
    /// struct container : Policy {};
    /// @endcode
    template <typename T>
    concept empty_type = ::std::is_empty_v<T>;

    /// @brief Satisfied when T is a polymorphic class (has at least one virtual function).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Base    { virtual ~Base() = default; };
    /// struct Derived : Base {};
    /// struct Plain   { int x; };
    ///
    /// static_assert( scl::concepts::polymorphic<Base>);
    /// static_assert( scl::concepts::polymorphic<Derived>);
    /// static_assert(!scl::concepts::polymorphic<Plain>);
    ///
    /// // dynamic_cast is only valid for polymorphic types:
    /// template <scl::concepts::polymorphic T>
    /// T* safe_downcast(Base* p) { return dynamic_cast<T*>(p); }
    /// @endcode
    template <typename T>
    concept polymorphic = ::std::is_polymorphic_v<T>;

    /// @brief Satisfied when T is an abstract class (has at least one pure virtual function).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Interface { virtual void f() = 0; };
    /// struct Concrete  : Interface { void f() override {} };
    ///
    /// static_assert( scl::concepts::abstract_type<Interface>);
    /// static_assert(!scl::concepts::abstract_type<Concrete>);
    /// static_assert(!scl::concepts::abstract_type<int>);
    ///
    /// // Reject abstract types that cannot be instantiated:
    /// template <typename T>
    ///     requires (!scl::concepts::abstract_type<T>)
    /// T make_default() { return T{}; }
    /// @endcode
    template <typename T>
    concept abstract_type = ::std::is_abstract_v<T>;

    /// @brief Satisfied when T is declared final.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Open  {};
    /// struct Closed final {};
    ///
    /// static_assert( scl::concepts::final_type<Closed>);
    /// static_assert(!scl::concepts::final_type<Open>);
    /// static_assert(!scl::concepts::final_type<int>);
    ///
    /// // final types can be stored without vtable overhead:
    /// template <scl::concepts::final_type T>
    /// struct inline_storage { T value; };
    /// @endcode
    template <typename T>
    concept final_type = ::std::is_final_v<T>;

    /// @brief Satisfied when T is an aggregate type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Point { int x; int y; };
    /// union Raw  { int i; float f; };
    /// struct WithCtor { WithCtor(int) {} };
    ///
    /// static_assert( scl::concepts::aggregate<Point>);
    /// static_assert( scl::concepts::aggregate<Raw>);
    /// static_assert(!scl::concepts::aggregate<WithCtor>);
    ///
    /// // Aggregate initialization works for any aggregate type:
    /// template <scl::concepts::aggregate T>
    /// T make_zeroed() { return T{}; }
    /// @endcode
    template <typename T>
    concept aggregate = ::std::is_aggregate_v<T>;

} // namespace scl::concepts
