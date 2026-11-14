#include <gtest_utils.h>

#include <scl/utility/concepts/type_relation.h>

namespace
{
    struct base
    {};

    struct derived : base
    {};

    struct indirect_derived : derived
    {};

    struct private_derived : private base
    {};

    struct virtual_derived : virtual base
    {};

    struct private_virtual_derived : private virtual base
    {};

    struct indirect_virtual_derived : virtual_derived
    {};

    struct left : base
    {};

    struct right : base
    {};

    struct twice
        : left
        , right
    {};

    struct unrelated
    {};

    union plain_union
    {
        int integer;
    };

    template <::scl::concepts::class_type Type>
    constexpr int base_of_over_class_type_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<Type, derived>
    constexpr int base_of_over_class_type_of_base()
    {
        return 2;
    }

    template <::scl::concepts::class_type Type>
    constexpr int base_of_over_class_type_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<base, Type>
    constexpr int base_of_over_class_type_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int base_of_over_object_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<Type, derived>
    constexpr int base_of_over_object_of_base()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int base_of_over_object_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<base, Type>
    constexpr int base_of_over_object_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int base_of_over_compound_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<Type, derived>
    constexpr int base_of_over_compound_of_base()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int base_of_over_compound_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<base, Type>
    constexpr int base_of_over_compound_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int base_of_over_non_reference_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<Type, derived>
    constexpr int base_of_over_non_reference_of_base()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int base_of_over_non_reference_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::base_of<base, Type>
    constexpr int base_of_over_non_reference_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::base_of<derived> Base>
    constexpr bool base_of_derived()
    {
        return true;
    }

    template <typename Type>
    constexpr bool accepted_as_base_of_derived = requires { base_of_derived<Type>(); };
} // namespace

TEST(ConceptsBaseOf, HoldsForAPublicBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, derived>));
}

TEST(ConceptsBaseOf, HoldsForAClassAndItself)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, base>));
}

TEST(ConceptsBaseOf, HoldsForAPrivateBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, private_derived>));
}

TEST(ConceptsBaseOf, HoldsForAnIndirectBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, indirect_derived>));
}

TEST(ConceptsBaseOf, HoldsForAVirtualBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, virtual_derived>));
}

TEST(ConceptsBaseOf, HoldsForABaseClassHeldMoreThanOnce)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, twice>));
}

TEST(ConceptsBaseOf, HoldsForCvQualifiedTypesWhereTheUnqualifiedTypesSatisfyIt)
{
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base const, derived>));
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base, derived volatile>));
    STATIC_EXPECT_TRUE((::scl::concepts::base_of<base const volatile, derived const>));
}

TEST(ConceptsBaseOf, FailsForCvQualifiedTypesWhereTheUnqualifiedTypesDoNot)
{
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<derived const, base>));
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<derived, base volatile>));
}

TEST(ConceptsBaseOf, FailsForADerivedClassInPlaceOfTheBase)
{
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<derived, base>));
}

TEST(ConceptsBaseOf, FailsForUnrelatedClasses)
{
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<unrelated, derived>));
}

TEST(ConceptsBaseOf, FailsForAUnion)
{
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<plain_union, plain_union>));
}

TEST(ConceptsBaseOf, FailsForATypeThatIsNotAClass)
{
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<int, int>));
}

TEST(ConceptsBaseOf, FailsForAReferenceInEitherPosition)
{
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<base &, derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<base, derived &>));
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<base &&, derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::base_of<base, derived &&>));
}

TEST(ConceptsBaseOf, ConstrainsATemplateParameterInPlaceOfTheBase)
{
    STATIC_EXPECT_TRUE(accepted_as_base_of_derived<base>);
    STATIC_EXPECT_FALSE(accepted_as_base_of_derived<indirect_derived>);
}

TEST(ConceptsBaseOf, SubsumesClassTypeOfTheBase)
{
    STATIC_EXPECT_EQ(base_of_over_class_type_of_base<base>(), 2);
}

TEST(ConceptsBaseOf, SubsumesClassTypeOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(base_of_over_class_type_of_derived<derived>(), 2);
}

TEST(ConceptsBaseOf, SubsumesObjectOfTheBase)
{
    STATIC_EXPECT_EQ(base_of_over_object_of_base<base>(), 2);
}

TEST(ConceptsBaseOf, SubsumesObjectOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(base_of_over_object_of_derived<derived>(), 2);
}

TEST(ConceptsBaseOf, SubsumesCompoundOfTheBase)
{
    STATIC_EXPECT_EQ(base_of_over_compound_of_base<base>(), 2);
}

TEST(ConceptsBaseOf, SubsumesCompoundOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(base_of_over_compound_of_derived<derived>(), 2);
}

TEST(ConceptsBaseOf, SubsumesNonReferenceOfTheBase)
{
    STATIC_EXPECT_EQ(base_of_over_non_reference_of_base<base>(), 2);
}

TEST(ConceptsBaseOf, SubsumesNonReferenceOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(base_of_over_non_reference_of_derived<derived>(), 2);
}

#ifndef SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
#error "SCL_HAS_VIRTUAL_BASE_OF_CONCEPT must always be defined, so that #if catches a misspelling"
#endif

#if defined(__cpp_lib_is_virtual_base_of) && __cpp_lib_is_virtual_base_of >= 202406L
TEST(ConceptsSclHasVirtualBaseOfConcept, ExpandsToOneWhereTheLibraryDeclaresTheTraitOrTheCompilerHasTheBuiltIn)
{
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 1);
}
#elif defined(__has_builtin)
#if __has_builtin(__builtin_is_virtual_base_of)
TEST(ConceptsSclHasVirtualBaseOfConcept, ExpandsToOneWhereTheLibraryDeclaresTheTraitOrTheCompilerHasTheBuiltIn)
{
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 1);
}
#else
TEST(ConceptsSclHasVirtualBaseOfConcept, ExpandsToZeroElsewhere)
{
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 0);
}
#endif
#else
TEST(ConceptsSclHasVirtualBaseOfConcept, ExpandsToZeroElsewhere)
{
    STATIC_EXPECT_EQ(SCL_HAS_VIRTUAL_BASE_OF_CONCEPT, 0);
}
#endif

#if !SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
namespace scl::concepts
{
    template <typename Base, typename Derived>
    concept virtual_base_of = false;
} // namespace scl::concepts

TEST(ConceptsSclHasVirtualBaseOfConcept, LeavesTheConceptUndeclaredWhereItExpandsToZero)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base, virtual_derived>));
}
#endif

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
namespace
{
    struct mixed
        : virtual_derived
        , derived
    {};

    template <typename Type>
        requires ::scl::concepts::base_of<base, Type>
    constexpr int virtual_base_of_over_base_of()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<base, Type>
    constexpr int virtual_base_of_over_base_of()
    {
        return 2;
    }

    template <::scl::concepts::class_type Type>
    constexpr int virtual_base_of_over_class_type_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<Type, virtual_derived>
    constexpr int virtual_base_of_over_class_type_of_base()
    {
        return 2;
    }

    template <::scl::concepts::class_type Type>
    constexpr int virtual_base_of_over_class_type_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<base, Type>
    constexpr int virtual_base_of_over_class_type_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int virtual_base_of_over_object_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<Type, virtual_derived>
    constexpr int virtual_base_of_over_object_of_base()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int virtual_base_of_over_object_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<base, Type>
    constexpr int virtual_base_of_over_object_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int virtual_base_of_over_compound_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<Type, virtual_derived>
    constexpr int virtual_base_of_over_compound_of_base()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int virtual_base_of_over_compound_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<base, Type>
    constexpr int virtual_base_of_over_compound_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int virtual_base_of_over_non_reference_of_base()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<Type, virtual_derived>
    constexpr int virtual_base_of_over_non_reference_of_base()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int virtual_base_of_over_non_reference_of_derived()
    {
        return 1;
    }

    template <typename Type>
        requires ::scl::concepts::virtual_base_of<base, Type>
    constexpr int virtual_base_of_over_non_reference_of_derived()
    {
        return 2;
    }

    template <::scl::concepts::virtual_base_of<virtual_derived> Base>
    constexpr bool virtual_base_of_virtual_derived()
    {
        return true;
    }

    template <typename Type>
    constexpr bool accepted_as_virtual_base_of_virtual_derived =
        requires { virtual_base_of_virtual_derived<Type>(); };
} // namespace

TEST(ConceptsVirtualBaseOf, HoldsForAVirtualBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base, virtual_derived>));
}

TEST(ConceptsVirtualBaseOf, HoldsForAPrivateVirtualBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base, private_virtual_derived>));
}

TEST(ConceptsVirtualBaseOf, HoldsForAnIndirectVirtualBaseClass)
{
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base, indirect_virtual_derived>));
}

TEST(ConceptsVirtualBaseOf, HoldsBesideABaseClassOfTheSameTypeThatIsNotVirtual)
{
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base, mixed>));
}

TEST(ConceptsVirtualBaseOf, HoldsForCvQualifiedTypesWhereTheUnqualifiedTypesSatisfyIt)
{
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base const, virtual_derived>));
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base, virtual_derived volatile>));
    STATIC_EXPECT_TRUE((::scl::concepts::virtual_base_of<base const volatile, virtual_derived const>));
}

TEST(ConceptsVirtualBaseOf, FailsForCvQualifiedTypesWhereTheUnqualifiedTypesDoNot)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<virtual_derived const, base>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<virtual_derived, base volatile>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base const, derived>));
}

TEST(ConceptsVirtualBaseOf, FailsForABaseClassThatIsNotVirtual)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base, derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base, private_derived>));
}

TEST(ConceptsVirtualBaseOf, FailsForAClassAndItself)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base, base>));
}

TEST(ConceptsVirtualBaseOf, FailsForADerivedClassInPlaceOfTheBase)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<virtual_derived, base>));
}

TEST(ConceptsVirtualBaseOf, FailsForUnrelatedClasses)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<unrelated, virtual_derived>));
}

TEST(ConceptsVirtualBaseOf, FailsForAUnion)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<plain_union, plain_union>));
}

TEST(ConceptsVirtualBaseOf, FailsForATypeThatIsNotAClass)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<int, int>));
}

TEST(ConceptsVirtualBaseOf, FailsForAReferenceInEitherPosition)
{
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base &, virtual_derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base, virtual_derived &>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base &&, virtual_derived>));
    STATIC_EXPECT_FALSE((::scl::concepts::virtual_base_of<base, virtual_derived &&>));
}

TEST(ConceptsVirtualBaseOf, ConstrainsATemplateParameterInPlaceOfTheBase)
{
    STATIC_EXPECT_TRUE(accepted_as_virtual_base_of_virtual_derived<base>);
    STATIC_EXPECT_FALSE(accepted_as_virtual_base_of_virtual_derived<indirect_virtual_derived>);
}

TEST(ConceptsVirtualBaseOf, SubsumesBaseOf)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_base_of<virtual_derived>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesClassTypeOfTheBase)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_class_type_of_base<base>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesClassTypeOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_class_type_of_derived<virtual_derived>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesObjectOfTheBase)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_object_of_base<base>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesObjectOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_object_of_derived<virtual_derived>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesCompoundOfTheBase)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_compound_of_base<base>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesCompoundOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_compound_of_derived<virtual_derived>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesNonReferenceOfTheBase)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_non_reference_of_base<base>(), 2);
}

TEST(ConceptsVirtualBaseOf, SubsumesNonReferenceOfTheDerivedClass)
{
    STATIC_EXPECT_EQ(virtual_base_of_over_non_reference_of_derived<virtual_derived>(), 2);
}
#endif
