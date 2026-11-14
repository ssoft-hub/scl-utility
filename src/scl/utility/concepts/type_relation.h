#pragma once

/**
 * @file
 * @brief Concepts of the relations between two types, such as a base class and a derived class.
 * @ingroup scl_utility_concepts
 */

#include <scl/utility/concepts/type_category.h>

#include <type_traits>

#if (defined(__cpp_lib_is_virtual_base_of) && __cpp_lib_is_virtual_base_of >= 202406L) || defined(SCL_DOXYGEN)
#define SCL_HAS_VIRTUAL_BASE_OF_CONCEPT 1
#elif defined(__has_builtin)
#if __has_builtin(__builtin_is_virtual_base_of)
#define SCL_HAS_VIRTUAL_BASE_OF_CONCEPT 1
#else
#define SCL_HAS_VIRTUAL_BASE_OF_CONCEPT 0
#endif
#else
#define SCL_HAS_VIRTUAL_BASE_OF_CONCEPT 0
#endif

namespace scl::concepts
{

    /**
     * @brief Satisfied where the class `Base` is the class `Derived` or a base class of it.
     * @ingroup scl_utility_concepts
     *
     * The pair of checked types satisfies the concept if both types are classes other than unions
     * and the class `Base` is the class `Derived` or a base class of it, direct or indirect.
     * The access to the base class does not count, and the class `Base` may occur in the class
     * `Derived` as a base class more than once. This is where the concept differs from the concept
     * `std::derived_from`, which requires a public and unambiguous base class. The result of
     * the check equals the value of the trait `std::is_base_of_v<Base, Derived>`.
     *
     * The cv-qualifiers of both types do not count. If at least one of the types is a reference,
     * the pair does not satisfy the concept. The concept can be given in the declaration of
     * a template parameter, such as `template <::scl::concepts::base_of<Derived> Base>`, and
     * the template parameter is then checked as the type `Base`. If both types are classes other
     * than unions and differ, the type `Derived` must be complete, as `std::is_base_of` requires.
     *
     * The concept is declared through `::scl::concepts::class_type<Base>` and
     * `::scl::concepts::class_type<Derived>`, so its constraint is stricter than the constraints of
     * ::scl::concepts::class_type, ::scl::concepts::object, ::scl::concepts::compound and
     * ::scl::concepts::non_reference of each type, and the compiler chooses a function template
     * constrained by the concept over one constrained by any of them.
     *
     * @snippet concepts/type_relation/concepts_type_relation_example.cpp base_of
     *
     * @tparam Base Type checked as the base class.
     * @tparam Derived Type checked as the derived class.
     */
    template <typename Base, typename Derived>
    concept base_of = ::scl::concepts::class_type<Base> && ::scl::concepts::class_type<Derived> &&
        ::std::is_base_of_v<Base, Derived>;

    /**
     * @brief Satisfied where the class `Base` is a virtual base class of the class `Derived`.
     * @ingroup scl_utility_concepts
     *
     * The concept is declared only if the macro ::SCL_HAS_VIRTUAL_BASE_OF_CONCEPT expands to `1`.
     * The pair of checked types satisfies the concept if the class `Base` is a virtual base class
     * of the class `Derived`, direct or indirect. The access to the base class does not count.
     * The class `Derived` may hold the class `Base` both as a virtual and as a non-virtual base
     * class. A class is not a virtual base class of itself. The result of the check equals
     * the value of the trait `std::is_virtual_base_of_v<Base, Derived>` of C++26.
     *
     * The cv-qualifiers of both types do not count. If at least one of the types is a reference,
     * the pair does not satisfy the concept. The concept can be given in the declaration of
     * a template parameter, such as `template <::scl::concepts::virtual_base_of<Derived> Base>`,
     * and the template parameter is then checked as the type `Base`. If both types are classes
     * other than unions and differ, the type `Derived` must be complete, as
     * `std::is_virtual_base_of` requires.
     *
     * The concept is declared through ::scl::concepts::base_of, so its constraint is stricter than
     * the constraints of ::scl::concepts::base_of and of every concept it is declared through, and
     * the compiler chooses a function template constrained by the concept over one constrained by
     * any of them.
     *
     * @snippet concepts/type_relation/concepts_type_relation_example.cpp virtual_base_of
     *
     * @tparam Base Type checked as the base class.
     * @tparam Derived Type checked as the derived class.
     */
#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
#if (defined(__cpp_lib_is_virtual_base_of) && __cpp_lib_is_virtual_base_of >= 202406L) || defined(SCL_DOXYGEN)
    template <typename Base, typename Derived>
    concept virtual_base_of = ::scl::concepts::base_of<Base, Derived> &&
        ::std::is_virtual_base_of_v<Base, Derived>;
#else
    template <typename Base, typename Derived>
    concept virtual_base_of = ::scl::concepts::base_of<Base, Derived> &&
        __builtin_is_virtual_base_of(Base, Derived);
#endif
#endif

} // namespace scl::concepts

// -----------------------------------------------------------------------------
// Documentation
// -----------------------------------------------------------------------------

/**
 * @def SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
 * @brief Whether the header declares the concept ::scl::concepts::virtual_base_of.
 * @ingroup scl_utility_concepts
 *
 * The macro tells whether the concept ::scl::concepts::virtual_base_of is declared. The macro
 * expands to `1` if the standard library provides the trait `std::is_virtual_base_of` (the macro
 * `__cpp_lib_is_virtual_base_of` is at least `202406L`) or the compiler provides the built-in
 * function `__builtin_is_virtual_base_of`. Otherwise the macro expands to `0`, and the concept
 * ::scl::concepts::virtual_base_of is not declared.
 *
 * The macro is always defined, so a caller should test it with `#if`, not with `#ifdef`. Under
 * `#if` a misspelled name of the macro reads as `0` and draws the warning `-Wundef` where that
 * option of GCC or Clang is given, while under `#ifdef` it reads as false with no warning.
 */
