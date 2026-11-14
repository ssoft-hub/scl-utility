#pragma once

/**
 * @file
 * @brief Concepts of the properties of a type, such as trivial copying and standard layout.
 * @ingroup scl_utility_concepts
 */

#include <type_traits>

namespace scl::concepts
{

    /**
     * @brief Satisfied by a type whose objects can be copied as bytes.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if its objects can be copied as bytes, that is, if
     * the type is a scalar type, a trivially copyable class or an array of such types. The object
     * representation of such an object can be copied with `std::memcpy` into another object of
     * the same type, and after the copy the second object holds the same value. Each of the two
     * objects must be neither a base class subobject nor a member declared with the attribute
     * `[[no_unique_address]]`, since such a subobject can occupy bytes of another member of
     * the enclosing object, and the copy overwrites that member. The result of the check equals
     * the value of the trait `std::is_trivially_copyable_v<T>`. For a type with cv-qualifiers
     * the concept gives the same result as for the type without them, and a reference type does not
     * satisfy the concept.
     *
     * The type `std::remove_all_extents_t<T>` must be a complete type or `void`, as
     * `std::is_trivially_copyable` requires. The concept is declared through no other concept, so
     * its constraint is not stricter than the constraint of any other concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp trivially_copyable
     * @snippet concepts/type_property/concepts_type_property_example.cpp copy_bytes
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept trivially_copyable = ::std::is_trivially_copyable_v<T>;

    /**
     * @brief Satisfied by a standard-layout type.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it has standard layout, that is, if it is a scalar
     * type, a standard-layout class or an array of such types. A class with a virtual function or
     * a virtual base class has no standard layout. The result of the check equals the value of
     * the trait `std::is_standard_layout_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * The type `std::remove_all_extents_t<T>` must be a complete type or `void`, as
     * `std::is_standard_layout` requires. The concept is declared through no other concept, so its
     * constraint is not stricter than the constraint of any other concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp standard_layout
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept standard_layout = ::std::is_standard_layout_v<T>;

    /**
     * @brief Satisfied by a class other than a union that holds no data and nothing virtual.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an empty class, that is, a class other than
     * a union that has no non-static data member other than a subobject of zero size, no virtual
     * function, no virtual base class and no base class that is not empty. The result of the check
     * equals the value of the trait `std::is_empty_v<T>`. For a type with cv-qualifiers the concept
     * gives the same result as for the type without them, and a reference type does not satisfy
     * the concept. An empty member with the attribute `[[no_unique_address]]` is a subobject of
     * zero size only under the Itanium ABI, which GCC and Clang follow on every platform other than
     * the MSVC one. Under the MSVC ABI, which MSVC follows and Clang follows when compiling for
     * the MSVC platform, the attribute has no effect, and a class with such a member is not empty.
     * A class with an empty member declared with the macro ::SCL_NO_UNIQUE_ADDRESS is empty under
     * both ABIs if the macro expands to the attribute `[[no_unique_address]]` under the Itanium ABI
     * and to the attribute `[[msvc::no_unique_address]]` under the MSVC ABI.
     *
     * A class that is not a union must be complete, as `std::is_empty` requires. The concept is
     * declared through no other concept, so its constraint is not stricter than the constraint of
     * any other concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp empty
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept empty = ::std::is_empty_v<T>;

    /**
     * @brief Satisfied by a class that declares or inherits a virtual function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class that declares or inherits a virtual
     * function. The result of the check equals the value of the trait `std::is_polymorphic_v<T>`.
     * For a type with cv-qualifiers the concept gives the same result as for the type without them,
     * and a reference type does not satisfy the concept.
     *
     * A class that is not a union must be complete, as `std::is_polymorphic` requires. The concept
     * is declared through no other concept, so its constraint is not stricter than the constraint
     * of any other concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp polymorphic
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept polymorphic = ::std::is_polymorphic_v<T>;

    /**
     * @brief Satisfied by a class with at least one pure virtual function.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class with at least one pure virtual
     * function, declared in the class itself or inherited and not overridden in it. An object of
     * such a class exists only as a subobject of a derived class. The result of the check equals
     * the value of the trait `std::is_abstract_v<T>`. For a type with cv-qualifiers the concept
     * gives the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * A class that is not a union must be complete, as `std::is_abstract` requires. The concept is
     * declared through no other concept, so its constraint is not stricter than the constraint of
     * any other concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp abstract
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept abstract = ::std::is_abstract_v<T>;

    /**
     * @brief Satisfied by a class or a union declared with the specifier `final`.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is a class or a union declared with
     * the specifier `final`. No class can derive from such a type. The result of the check equals
     * the value of the trait `std::is_final_v<T>`. For a type with cv-qualifiers the concept gives
     * the same result as for the type without them, and a reference type does not satisfy
     * the concept.
     *
     * A class or a union must be complete, as `std::is_final` requires. The concept is declared
     * through no other concept, so its constraint is not stricter than the constraint of any other
     * concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp final
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept final = ::std::is_final_v<T>;

    /**
     * @brief Satisfied by an aggregate, that is, an array or a restricted class or union.
     * @ingroup scl_utility_concepts
     *
     * The checked type satisfies the concept if it is an aggregate, that is, an array or a class or
     * a union with no user-declared or inherited constructor, no virtual function, no private or
     * protected direct non-static data member, no virtual base class, direct or indirect, and no
     * private or protected direct base class. The result of the check equals the value of the trait
     * `std::is_aggregate_v<T>`. For a type with cv-qualifiers the concept gives the same result as
     * for the type without them, and a reference type does not satisfy the concept.
     *
     * The type must be an array type, a complete type or `void`, as `std::is_aggregate` requires.
     * The concept is declared through no other concept, so its constraint is not stricter than
     * the constraint of any other concept of the group.
     *
     * @snippet concepts/type_property/concepts_type_property_example.cpp aggregate
     *
     * @tparam T Type to check.
     */
    template <typename T>
    concept aggregate = ::std::is_aggregate_v<T>;

} // namespace scl::concepts
