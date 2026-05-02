#pragma once

/// @file
/// @brief Primary and composite type-category concepts.
/// @ingroup scl_utility_concepts

#include <type_traits>

namespace scl::concepts
{

    /// @brief Satisfied when T is an enumeration type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// enum Color { Red, Green, Blue };
    /// enum class Dir { North, South };
    ///
    /// static_assert( scl::concepts::enum_type<Color>);
    /// static_assert( scl::concepts::enum_type<Dir>);
    /// static_assert(!scl::concepts::enum_type<int>);
    ///
    /// template <scl::concepts::enum_type E>
    /// auto to_int(E e)
    /// {
    ///     return static_cast<std::underlying_type_t<E>>(e);
    /// }
    /// @endcode
    template <typename T>
    concept enum_type = ::std::is_enum_v<T>;

    /// @brief Satisfied when V is a value of enumeration type.
    /// @ingroup scl_utility_concepts
    /// @tparam V value to check
    /// @note For template parameter lists prefer `enum_type auto V`; use this
    ///       concept in explicit requires-expressions or composed constraints.
    /// @par Example
    /// @code{.cpp}
    /// enum Color { Red, Green, Blue };
    ///
    /// static_assert( scl::concepts::enum_value<Color::Red>);
    /// static_assert(!scl::concepts::enum_value<42>);
    ///
    /// // Abbreviated NTTP constraint via the type concept:
    /// template <scl::concepts::enum_type auto V>
    /// constexpr auto underlying()
    /// {
    ///     return static_cast<std::underlying_type_t<decltype(V)>>(V);
    /// }
    ///
    /// // Or via an explicit requires-expression:
    /// template <auto V>
    ///     requires scl::concepts::enum_value<V>
    /// constexpr auto underlying();
    /// @endcode
    template <auto V>
    concept enum_value = ::std::is_enum_v<decltype(V)>;

    /// @brief Satisfied when T is void.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::void_type<void>);
    /// static_assert( scl::concepts::void_type<void const>);
    /// static_assert(!scl::concepts::void_type<int>);
    /// static_assert(!scl::concepts::void_type<void*>);
    /// @endcode
    template <typename T>
    concept void_type = ::std::is_void_v<T>;

    /// @brief Satisfied when T is a (possibly cv-qualified) pointer to object or function.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::pointer<int*>);
    /// static_assert( scl::concepts::pointer<void (*)(int)>);
    /// static_assert(!scl::concepts::pointer<int&>);
    /// static_assert(!scl::concepts::pointer<std::nullptr_t>);
    ///
    /// template <scl::concepts::pointer P>
    /// auto deref(P p) -> decltype(*p) { return *p; }
    /// @endcode
    template <typename T>
    concept pointer = ::std::is_pointer_v<T>;

    /// @brief Satisfied when T is std::nullptr_t.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::null_pointer<std::nullptr_t>);
    /// static_assert(!scl::concepts::null_pointer<int*>);
    /// static_assert(!scl::concepts::null_pointer<void*>);
    ///
    /// template <scl::concepts::null_pointer T>
    /// void accept_null(T);            // overload for nullptr_t
    /// @endcode
    template <typename T>
    concept null_pointer = ::std::is_null_pointer_v<T>;

    /// @brief Satisfied when T is a pointer-to-member (object or function).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct S { int x; void f(); };
    ///
    /// static_assert( scl::concepts::member_pointer<int S::*>);
    /// static_assert( scl::concepts::member_pointer<void (S::*)()>);
    /// static_assert(!scl::concepts::member_pointer<int*>);
    /// @endcode
    template <typename T>
    concept member_pointer = ::std::is_member_pointer_v<T>;

    /// @brief Satisfied when T is a pointer-to-data-member.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct S { int x; void f(); };
    ///
    /// static_assert( scl::concepts::member_object_pointer<int S::*>);
    /// static_assert(!scl::concepts::member_object_pointer<void (S::*)()>);
    /// static_assert(!scl::concepts::member_object_pointer<int*>);
    ///
    /// template <typename C, scl::concepts::member_object_pointer auto M>
    /// auto get(C& obj) { return obj.*M; }
    /// @endcode
    template <typename T>
    concept member_object_pointer = ::std::is_member_object_pointer_v<T>;

    /// @brief Satisfied when T is a pointer-to-member-function.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct S { int x; void f(); };
    ///
    /// static_assert( scl::concepts::member_function_pointer<void (S::*)()>);
    /// static_assert(!scl::concepts::member_function_pointer<int S::*>);
    /// static_assert(!scl::concepts::member_function_pointer<void (*)()>);
    ///
    /// template <typename C, scl::concepts::member_function_pointer auto M>
    /// auto invoke(C& obj) { return (obj.*M)(); }
    /// @endcode
    template <typename T>
    concept member_function_pointer = ::std::is_member_function_pointer_v<T>;

    /// @brief Satisfied when T is an array type (bounded or unbounded).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::array_type<int[3]>);
    /// static_assert( scl::concepts::array_type<int[]>);
    /// static_assert(!scl::concepts::array_type<int*>);
    /// static_assert(!scl::concepts::array_type<int>);
    /// @endcode
    template <typename T>
    concept array_type = ::std::is_array_v<T>;

    /// @brief Satisfied when T is a bounded array type (e.g. int[3]).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::bounded_array<int[3]>);
    /// static_assert( scl::concepts::bounded_array<char[256]>);
    /// static_assert(!scl::concepts::bounded_array<int[]>);
    /// static_assert(!scl::concepts::bounded_array<int*>);
    ///
    /// template <scl::concepts::bounded_array T>
    /// constexpr std::size_t array_size() { return std::extent_v<T>; }
    /// @endcode
    template <typename T>
    concept bounded_array = ::std::is_bounded_array_v<T>;

    /// @brief Satisfied when T is an unbounded array type (e.g. int[]).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::unbounded_array<int[]>);
    /// static_assert(!scl::concepts::unbounded_array<int[3]>);
    /// static_assert(!scl::concepts::unbounded_array<int*>);
    /// @endcode
    template <typename T>
    concept unbounded_array = ::std::is_unbounded_array_v<T>;

    /// @brief Satisfied when T is a function type (not a pointer-to-function).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @note Pointer-to-function types such as `void(*)(int)` do **not** satisfy
    ///       this concept; only bare function types like `void(int)` do.
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::function_type<void(int)>);
    /// static_assert( scl::concepts::function_type<int(float, char)>);
    /// static_assert(!scl::concepts::function_type<void(*)(int)>);
    /// static_assert(!scl::concepts::function_type<int>);
    /// @endcode
    template <typename T>
    concept function_type = ::std::is_function_v<T>;

    /// @brief Satisfied when T is a (non-union) class type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct Point { int x, y; };
    /// union U { int i; float f; };
    ///
    /// static_assert( scl::concepts::class_type<Point>);
    /// static_assert(!scl::concepts::class_type<U>);
    /// static_assert(!scl::concepts::class_type<int>);
    ///
    /// template <scl::concepts::class_type T>
    /// void accept_class(const T&);
    /// @endcode
    template <typename T>
    concept class_type = ::std::is_class_v<T>;

    /// @brief Satisfied when T is a union type.
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// union Variant { int i; float f; };
    /// struct S { int x; };
    ///
    /// static_assert( scl::concepts::union_type<Variant>);
    /// static_assert(!scl::concepts::union_type<S>);
    /// static_assert(!scl::concepts::union_type<int>);
    /// @endcode
    template <typename T>
    concept union_type = ::std::is_union_v<T>;

    /// @brief Satisfied when T is a fundamental type (arithmetic, void, or std::nullptr_t).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::fundamental<int>);
    /// static_assert( scl::concepts::fundamental<double>);
    /// static_assert( scl::concepts::fundamental<void>);
    /// static_assert( scl::concepts::fundamental<std::nullptr_t>);
    /// static_assert(!scl::concepts::fundamental<int*>);
    /// static_assert(!scl::concepts::fundamental<std::string>);
    /// @endcode
    template <typename T>
    concept fundamental = ::std::is_fundamental_v<T>;

    /// @brief Satisfied when T is an arithmetic type (integral or floating-point).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// static_assert( scl::concepts::arithmetic<int>);
    /// static_assert( scl::concepts::arithmetic<double>);
    /// static_assert( scl::concepts::arithmetic<bool>);
    /// static_assert(!scl::concepts::arithmetic<void>);
    /// static_assert(!scl::concepts::arithmetic<int*>);
    ///
    /// template <scl::concepts::arithmetic T>
    /// T add(T a, T b) { return a + b; }
    /// @endcode
    template <typename T>
    concept arithmetic = ::std::is_arithmetic_v<T>;

    /// @brief Satisfied when T is a scalar type (arithmetic, pointer, enum, or std::nullptr_t).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// enum E { A };
    ///
    /// static_assert( scl::concepts::scalar<int>);
    /// static_assert( scl::concepts::scalar<int*>);
    /// static_assert( scl::concepts::scalar<E>);
    /// static_assert( scl::concepts::scalar<std::nullptr_t>);
    /// static_assert(!scl::concepts::scalar<int[3]>);
    /// static_assert(!scl::concepts::scalar<void>);
    /// @endcode
    template <typename T>
    concept scalar = ::std::is_scalar_v<T>;

    /// @brief Satisfied when T is an object type (not void, reference, or function).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct S {};
    ///
    /// static_assert( scl::concepts::object_type<int>);
    /// static_assert( scl::concepts::object_type<int*>);
    /// static_assert( scl::concepts::object_type<S>);
    /// static_assert(!scl::concepts::object_type<void>);
    /// static_assert(!scl::concepts::object_type<int&>);
    /// static_assert(!scl::concepts::object_type<void(int)>);
    /// @endcode
    template <typename T>
    concept object_type = ::std::is_object_v<T>;

    /// @brief Satisfied when T is a compound type (any non-fundamental type).
    /// @ingroup scl_utility_concepts
    /// @tparam T type to check
    /// @par Example
    /// @code{.cpp}
    /// struct S {};
    ///
    /// static_assert( scl::concepts::compound<int*>);
    /// static_assert( scl::concepts::compound<int&>);
    /// static_assert( scl::concepts::compound<S>);
    /// static_assert( scl::concepts::compound<void(int)>);
    /// static_assert(!scl::concepts::compound<int>);
    /// static_assert(!scl::concepts::compound<void>);
    /// @endcode
    template <typename T>
    concept compound = ::std::is_compound_v<T>;

} // namespace scl::concepts
