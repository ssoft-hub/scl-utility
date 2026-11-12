#include <gtest_utils.h>

#include <scl/utility/attribute/no_unique_address.h>
#include <scl/utility/concepts/type_category.h>

#include <string>

namespace
{
    template <typename Type>
    using lvalue_reference_to = Type &;

    template <typename Type>
    using rvalue_reference_to = Type &&;

    struct trivial_class
    {
        int member;
    };

    struct string_holder
    {
        ::std::string text;
    };

    struct empty_class
    {};

    struct empty_derived : empty_class
    {};

    struct empty_member_class
    {
        SCL_NO_UNIQUE_ADDRESS
        empty_class tag;
    };

    struct non_empty_base_derived : trivial_class
    {};

    union empty_union
    {};

    union plain_union
    {
        int integer;
        float real;
    };

    struct virtual_function_class
    {
        virtual ~virtual_function_class() = default;
    };

    struct virtual_function_derived : virtual_function_class
    {};

    struct virtual_base_class : virtual empty_class
    {};

    struct indirect_virtual_base_class : virtual_base_class
    {};

    struct abstract_class
    {
        virtual ~abstract_class() = default;

        virtual void act() = 0;
    };

    struct abstract_derived : abstract_class
    {};

    struct concrete_derived : abstract_class
    {
        void act() override {}
    };

    struct final_class final
    {};

    union final_union final
    {
        int integer;
    };

    struct user_constructor
    {
        user_constructor();

        int member;
    };

    struct defaulted_constructor
    {
        defaulted_constructor() = default;

        int member;
    };

    struct constructed_base
    {
        explicit constructed_base(int) {}
    };

    struct inherited_constructor : constructed_base
    {
        using constructed_base::constructed_base;
    };

    class private_member_class
    {
    public:
        [[nodiscard]]
        int value() const
        {
            return member;
        }

    private:
        int member;
    };

    class protected_member_class
    {
    public:
        [[nodiscard]]
        int value() const
        {
            return member;
        }

    protected:
        int member;
    };

    struct private_base_class : private empty_class
    {};

    struct protected_base_class : protected empty_class
    {};

    struct user_destructor
    {
        int member;

        ~user_destructor() {}
    };

    struct trivial_constructor
    {
        trivial_constructor() = default;

        explicit trivial_constructor(int value)
            : member{value}
        {}

        int member;
    };

    struct user_copy_constructor
    {
        user_copy_constructor(user_copy_constructor const & other)
            : member{other.member}
        {}

        int member;
    };

    struct deleted_destructor
    {
        explicit deleted_destructor(int) {}

        ~deleted_destructor() = delete;
    };

    struct aggregate_deleted_destructor
    {
        int member;

        ~aggregate_deleted_destructor() = delete;
    };

    template <::scl::concepts::object Type>
    constexpr int trivially_copyable_over_object()
    {
        return 1;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int trivially_copyable_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int trivially_copyable_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int trivially_copyable_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int standard_layout_over_object()
    {
        return 1;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int standard_layout_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int standard_layout_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int standard_layout_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int aggregate_over_object()
    {
        return 1;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int aggregate_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int aggregate_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int aggregate_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int aggregate_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int aggregate_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::class_type Type>
    constexpr int empty_over_class_type()
    {
        return 1;
    }

    template <::scl::concepts::empty Type>
    constexpr int empty_over_class_type()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int empty_over_object()
    {
        return 1;
    }

    template <::scl::concepts::empty Type>
    constexpr int empty_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int empty_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::empty Type>
    constexpr int empty_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int empty_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::empty Type>
    constexpr int empty_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::class_type Type>
    constexpr int polymorphic_over_class_type()
    {
        return 1;
    }

    template <::scl::concepts::polymorphic Type>
    constexpr int polymorphic_over_class_type()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int polymorphic_over_object()
    {
        return 1;
    }

    template <::scl::concepts::polymorphic Type>
    constexpr int polymorphic_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int polymorphic_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::polymorphic Type>
    constexpr int polymorphic_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int polymorphic_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::polymorphic Type>
    constexpr int polymorphic_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::polymorphic Type>
    constexpr int abstract_over_polymorphic()
    {
        return 1;
    }

    template <::scl::concepts::abstract Type>
    constexpr int abstract_over_polymorphic()
    {
        return 2;
    }

    template <::scl::concepts::class_type Type>
    constexpr int abstract_over_class_type()
    {
        return 1;
    }

    template <::scl::concepts::abstract Type>
    constexpr int abstract_over_class_type()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int abstract_over_object()
    {
        return 1;
    }

    template <::scl::concepts::abstract Type>
    constexpr int abstract_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int abstract_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::abstract Type>
    constexpr int abstract_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int abstract_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::abstract Type>
    constexpr int abstract_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::object Type>
    constexpr int final_over_object()
    {
        return 1;
    }

    template <::scl::concepts::final Type>
    constexpr int final_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int final_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::final Type>
    constexpr int final_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int final_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::final Type>
    constexpr int final_over_compound()
    {
        return 2;
    }

    template <typename Type>
        requires(::scl::concepts::class_type<Type> || ::scl::concepts::union_type<Type>)
    constexpr int final_over_class_or_union()
    {
        return 1;
    }

    template <::scl::concepts::final Type>
    constexpr int final_over_class_or_union()
    {
        return 2;
    }

    template <::scl::concepts::trivially_copyable Type>
    constexpr int copyable_or_layout()
    {
        return 1;
    }

    template <::scl::concepts::standard_layout Type>
    constexpr int copyable_or_layout()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool copyable_or_layout_resolves = requires { copyable_or_layout<Type>(); };

    template <::scl::concepts::class_type Type>
    constexpr int class_or_aggregate()
    {
        return 1;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int class_or_aggregate()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool class_or_aggregate_resolves = requires { class_or_aggregate<Type>(); };

    template <::scl::concepts::union_type Type>
    constexpr int union_or_aggregate()
    {
        return 1;
    }

    template <::scl::concepts::aggregate Type>
    constexpr int union_or_aggregate()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool union_or_aggregate_resolves = requires { union_or_aggregate<Type>(); };

    template <::scl::concepts::class_type Type>
    constexpr int class_or_final()
    {
        return 1;
    }

    template <::scl::concepts::final Type>
    constexpr int class_or_final()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool class_or_final_resolves = requires { class_or_final<Type>(); };

    template <::scl::concepts::union_type Type>
    constexpr int union_or_final()
    {
        return 1;
    }

    template <::scl::concepts::final Type>
    constexpr int union_or_final()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool union_or_final_resolves = requires { union_or_final<Type>(); };
} // namespace

TEST(ConceptsTriviallyCopyable, HoldsForAScalarType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<int *>);
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<double>);
}

TEST(ConceptsTriviallyCopyable, HoldsForATriviallyCopyableClass)
{
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<trivial_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<empty_class>);
}

TEST(ConceptsTriviallyCopyable, HoldsForAnArrayOfSuchTypes)
{
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<trivial_class[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<int[2][3]>);
}

TEST(ConceptsTriviallyCopyable, FailsForAClassWithANonTrivialCopy)
{
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<string_holder>);
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<string_holder[2]>);
}

TEST(ConceptsTriviallyCopyable, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<trivial_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<int volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::trivially_copyable<int const volatile>);
}

TEST(ConceptsTriviallyCopyable, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<string_holder const>);
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<string_holder volatile>);
}

TEST(ConceptsTriviallyCopyable, IsUnorderedWithStandardLayout)
{
    STATIC_EXPECT_FALSE(copyable_or_layout_resolves<int>);
}

TEST(ConceptsTriviallyCopyable, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<lvalue_reference_to<trivial_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::trivially_copyable<rvalue_reference_to<trivial_class>>);
}

TEST(ConceptsTriviallyCopyable, SubsumesObject)
{
    STATIC_EXPECT_EQ(trivially_copyable_over_object<trivial_class>(), 2);
}

TEST(ConceptsTriviallyCopyable, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(trivially_copyable_over_non_reference<trivial_class>(), 2);
}

TEST(ConceptsStandardLayout, HoldsForAScalarType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<int *>);
}

TEST(ConceptsStandardLayout, HoldsForAStandardLayoutClass)
{
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<trivial_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<empty_class>);
}

TEST(ConceptsStandardLayout, HoldsForAnArrayOfSuchTypes)
{
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<trivial_class[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<int[]>);
}

TEST(ConceptsStandardLayout, FailsForAClassWithAVirtualFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<virtual_function_class>);
}

TEST(ConceptsStandardLayout, FailsForAClassWithAVirtualBaseClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<virtual_base_class>);
}

TEST(ConceptsStandardLayout, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<trivial_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<trivial_class volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::standard_layout<int const volatile>);
}

TEST(ConceptsStandardLayout, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<virtual_function_class const>);
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<virtual_base_class volatile>);
}

TEST(ConceptsStandardLayout, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<lvalue_reference_to<trivial_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::standard_layout<rvalue_reference_to<trivial_class>>);
}

TEST(ConceptsStandardLayout, SubsumesObject)
{
    STATIC_EXPECT_EQ(standard_layout_over_object<trivial_class>(), 2);
}

TEST(ConceptsStandardLayout, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(standard_layout_over_non_reference<trivial_class>(), 2);
}

TEST(ConceptsEmpty, HoldsForAClassWithNoDataMember)
{
    STATIC_EXPECT_TRUE(::scl::concepts::empty<empty_class>);
}

TEST(ConceptsEmpty, HoldsForAClassWithAnEmptyBaseClass)
{
    STATIC_EXPECT_TRUE(::scl::concepts::empty<empty_derived>);
}

TEST(ConceptsEmpty, HoldsForAClassWithAnEmptyMemberDeclaredSclNoUniqueAddress)
{
    STATIC_EXPECT_TRUE(::scl::concepts::empty<empty_member_class>);
}

TEST(ConceptsEmpty, FailsForAUnion) { STATIC_EXPECT_FALSE(::scl::concepts::empty<empty_union>); }

TEST(ConceptsEmpty, FailsForAClassWithADataMember)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<trivial_class>);
}

TEST(ConceptsEmpty, FailsForAClassWithAVirtualFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<virtual_function_class>);
}

TEST(ConceptsEmpty, FailsForAClassWithAVirtualBaseClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<virtual_base_class>);
}

TEST(ConceptsEmpty, FailsForAClassWithABaseClassThatIsNotEmpty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<non_empty_base_derived>);
}

TEST(ConceptsEmpty, FailsForATypeThatIsNotAClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<int>);
}

TEST(ConceptsEmpty, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::empty<empty_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::empty<empty_class volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::empty<empty_class const volatile>);
}

TEST(ConceptsEmpty, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<trivial_class const>);
    STATIC_EXPECT_FALSE(::scl::concepts::empty<trivial_class volatile>);
}

TEST(ConceptsEmpty, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::empty<lvalue_reference_to<empty_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::empty<rvalue_reference_to<empty_class>>);
}

TEST(ConceptsEmpty, SubsumesClassType)
{
    STATIC_EXPECT_EQ(empty_over_class_type<empty_class>(), 2);
}

TEST(ConceptsEmpty, SubsumesObject) { STATIC_EXPECT_EQ(empty_over_object<empty_class>(), 2); }

TEST(ConceptsEmpty, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(empty_over_non_reference<empty_class>(), 2);
}

TEST(ConceptsEmpty, SubsumesCompound) { STATIC_EXPECT_EQ(empty_over_compound<empty_class>(), 2); }

TEST(ConceptsPolymorphic, HoldsForAClassDeclaringAVirtualFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<virtual_function_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<abstract_class>);
}

TEST(ConceptsPolymorphic, HoldsForAClassInheritingAVirtualFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<virtual_function_derived>);
}

TEST(ConceptsPolymorphic, FailsForAClassWithoutAVirtualFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<trivial_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<virtual_base_class>);
}

TEST(ConceptsPolymorphic, FailsForATypeThatIsNotAClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<plain_union>);
}

TEST(ConceptsPolymorphic, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<virtual_function_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::polymorphic<virtual_function_class volatile>);
}

TEST(ConceptsPolymorphic, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<trivial_class const>);
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<trivial_class volatile>);
}

TEST(ConceptsPolymorphic, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<lvalue_reference_to<virtual_function_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::polymorphic<rvalue_reference_to<virtual_function_class>>);
}

TEST(ConceptsPolymorphic, SubsumesClassType)
{
    STATIC_EXPECT_EQ(polymorphic_over_class_type<virtual_function_class>(), 2);
}

TEST(ConceptsPolymorphic, SubsumesObject)
{
    STATIC_EXPECT_EQ(polymorphic_over_object<virtual_function_class>(), 2);
}

TEST(ConceptsPolymorphic, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(polymorphic_over_non_reference<virtual_function_class>(), 2);
}

TEST(ConceptsPolymorphic, SubsumesCompound)
{
    STATIC_EXPECT_EQ(polymorphic_over_compound<virtual_function_class>(), 2);
}

TEST(ConceptsAbstract, HoldsForAClassDeclaringAPureVirtualFunction)
{
    STATIC_EXPECT_TRUE(::scl::concepts::abstract<abstract_class>);
}

TEST(ConceptsAbstract, HoldsForAClassInheritingAPureVirtualFunctionItDoesNotOverride)
{
    STATIC_EXPECT_TRUE(::scl::concepts::abstract<abstract_derived>);
}

TEST(ConceptsAbstract, FailsForAClassOverridingThePureVirtualFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<concrete_derived>);
}

TEST(ConceptsAbstract, FailsForAPolymorphicClassWithoutAPureVirtualFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<virtual_function_class>);
}

TEST(ConceptsAbstract, FailsForATypeThatIsNotAClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<int>);
}

TEST(ConceptsAbstract, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::abstract<abstract_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::abstract<abstract_class volatile>);
}

TEST(ConceptsAbstract, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<concrete_derived const>);
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<concrete_derived volatile>);
}

TEST(ConceptsAbstract, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<lvalue_reference_to<abstract_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::abstract<rvalue_reference_to<abstract_class>>);
}

TEST(ConceptsAbstract, SubsumesPolymorphic)
{
    STATIC_EXPECT_EQ(abstract_over_polymorphic<abstract_class>(), 2);
}

TEST(ConceptsAbstract, SubsumesClassType)
{
    STATIC_EXPECT_EQ(abstract_over_class_type<abstract_class>(), 2);
}

TEST(ConceptsAbstract, SubsumesObject)
{
    STATIC_EXPECT_EQ(abstract_over_object<abstract_class>(), 2);
}

TEST(ConceptsAbstract, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(abstract_over_non_reference<abstract_class>(), 2);
}

TEST(ConceptsAbstract, SubsumesCompound)
{
    STATIC_EXPECT_EQ(abstract_over_compound<abstract_class>(), 2);
}

TEST(ConceptsFinal, HoldsForAClassDeclaredFinal)
{
    STATIC_EXPECT_TRUE(::scl::concepts::final<final_class>);
}

TEST(ConceptsFinal, HoldsForAUnionDeclaredFinal)
{
    STATIC_EXPECT_TRUE(::scl::concepts::final<final_union>);
}

TEST(ConceptsFinal, FailsForAClassOrAUnionNotDeclaredFinal)
{
    STATIC_EXPECT_FALSE(::scl::concepts::final<empty_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::final<plain_union>);
}

TEST(ConceptsFinal, FailsForATypeThatIsNotAClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::final<int>);
}

TEST(ConceptsFinal, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::final<final_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::final<final_union volatile>);
}

TEST(ConceptsFinal, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::final<empty_class const>);
    STATIC_EXPECT_FALSE(::scl::concepts::final<plain_union volatile>);
}

TEST(ConceptsFinal, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::final<lvalue_reference_to<final_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::final<rvalue_reference_to<final_class>>);
}

TEST(ConceptsFinal, SubsumesTheDisjunctionOfClassTypeAndUnionType)
{
    STATIC_EXPECT_EQ(final_over_class_or_union<final_class>(), 2);
    STATIC_EXPECT_EQ(final_over_class_or_union<final_union>(), 2);
}

TEST(ConceptsFinal, IsUnorderedWithClassType)
{
    STATIC_EXPECT_FALSE(class_or_final_resolves<final_class>);
}

TEST(ConceptsFinal, IsUnorderedWithUnionType)
{
    STATIC_EXPECT_FALSE(union_or_final_resolves<final_union>);
}

TEST(ConceptsFinal, SubsumesObject) { STATIC_EXPECT_EQ(final_over_object<final_class>(), 2); }

TEST(ConceptsFinal, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(final_over_non_reference<final_class>(), 2);
}

TEST(ConceptsFinal, SubsumesCompound) { STATIC_EXPECT_EQ(final_over_compound<final_class>(), 2); }

TEST(ConceptsAggregate, HoldsForAnArrayType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<int[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<int[]>);
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<virtual_function_class[2]>);
}

TEST(ConceptsAggregate, HoldsForAClassOfTheRestrictedForm)
{
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<trivial_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<string_holder>);
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<empty_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<empty_derived>);
}

TEST(ConceptsAggregate, HoldsForAUnionOfTheRestrictedForm)
{
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<plain_union>);
}

TEST(ConceptsAggregate, FailsForAClassWithAUserDeclaredConstructor)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<user_constructor>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<defaulted_constructor>);
}

TEST(ConceptsAggregate, FailsForAClassWithAnInheritedConstructor)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<inherited_constructor>);
}

TEST(ConceptsAggregate, FailsForAClassWithAVirtualFunction)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<virtual_function_class>);
}

TEST(ConceptsAggregate, FailsForAClassWithAPrivateOrProtectedDataMember)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<private_member_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<protected_member_class>);
}

TEST(ConceptsAggregate, FailsForAClassWithAVirtualBaseClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<virtual_base_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<indirect_virtual_base_class>);
}

TEST(ConceptsAggregate, FailsForAClassWithAPrivateOrProtectedBaseClass)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<private_base_class>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<protected_base_class>);
}

TEST(ConceptsAggregate, FailsForAScalarType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<int>);
}

TEST(ConceptsAggregate, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<trivial_class const>);
    STATIC_EXPECT_TRUE(::scl::concepts::aggregate<int volatile[3]>);
}

TEST(ConceptsAggregate, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<user_constructor const>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<virtual_function_class volatile>);
}

TEST(ConceptsAggregate, IsUnorderedWithClassType)
{
    STATIC_EXPECT_FALSE(class_or_aggregate_resolves<trivial_class>);
}

TEST(ConceptsAggregate, IsUnorderedWithUnionType)
{
    STATIC_EXPECT_FALSE(union_or_aggregate_resolves<plain_union>);
}

TEST(ConceptsAggregate, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<lvalue_reference_to<trivial_class>>);
    STATIC_EXPECT_FALSE(::scl::concepts::aggregate<rvalue_reference_to<trivial_class>>);
}

TEST(ConceptsAggregate, SubsumesObject)
{
    STATIC_EXPECT_EQ(aggregate_over_object<trivial_class>(), 2);
}

TEST(ConceptsAggregate, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(aggregate_over_non_reference<trivial_class>(), 2);
}

TEST(ConceptsAggregate, SubsumesCompound)
{
    STATIC_EXPECT_EQ(aggregate_over_compound<trivial_class>(), 2);
}

#ifndef SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
#error "SCL_HAS_IMPLICIT_LIFETIME_CONCEPT must always be defined, so that #if catches a misspelling"
#endif

#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
TEST(ConceptsSclHasImplicitLifetimeConcept, ExpandsToOneWhereTheLibraryDeclaresTheTraitOrTheCompilerHasTheBuiltIn)
{
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 1);
}
#elif defined(__has_builtin)
#if __has_builtin(__builtin_is_implicit_lifetime)
TEST(ConceptsSclHasImplicitLifetimeConcept, ExpandsToOneWhereTheLibraryDeclaresTheTraitOrTheCompilerHasTheBuiltIn)
{
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 1);
}
#else
TEST(ConceptsSclHasImplicitLifetimeConcept, ExpandsToZeroElsewhere)
{
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 0);
}
#endif
#else
TEST(ConceptsSclHasImplicitLifetimeConcept, ExpandsToZeroElsewhere)
{
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 0);
}
#endif

#if !SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
namespace scl::concepts
{
    template <typename Type>
    concept implicit_lifetime = false;
} // namespace scl::concepts

TEST(ConceptsSclHasImplicitLifetimeConcept, LeavesTheConceptUndeclaredWhereItExpandsToZero)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<int>);
}
#endif

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
namespace
{
    template <::scl::concepts::object Type>
    constexpr int implicit_lifetime_over_object()
    {
        return 1;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int implicit_lifetime_over_object()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int implicit_lifetime_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::implicit_lifetime Type>
    constexpr int implicit_lifetime_over_non_reference()
    {
        return 2;
    }
} // namespace

TEST(ConceptsImplicitLifetime, HoldsForAScalarType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<int *>);
}

TEST(ConceptsImplicitLifetime, HoldsForAnArrayType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<int[3]>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<int[]>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<user_destructor[2]>);
}

TEST(ConceptsImplicitLifetime, HoldsForAnAggregateWhoseDestructorIsNotUserProvided)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<trivial_class>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<string_holder>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<aggregate_deleted_destructor>);
}

TEST(ConceptsImplicitLifetime, HoldsForAClassWithATrivialEligibleConstructorAndATrivialDestructor)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<trivial_constructor>);
}

TEST(ConceptsImplicitLifetime, FailsForAnAggregateWithAUserProvidedDestructor)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<user_destructor>);
}

TEST(ConceptsImplicitLifetime, FailsForAClassWithADeletedDestructorThatIsNotAnAggregate)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<deleted_destructor>);
}

TEST(ConceptsImplicitLifetime, FailsForAClassWithoutATrivialConstructorThatIsNotAnAggregate)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<user_copy_constructor>);
}

TEST(ConceptsImplicitLifetime, FailsForVoidAndAFunctionType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<void>);
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<void()>);
}

TEST(ConceptsImplicitLifetime, HoldsForACvQualifiedTypeWithTheProperty)
{
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<int const>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<trivial_constructor volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::implicit_lifetime<trivial_constructor const volatile>);
}

TEST(ConceptsImplicitLifetime, FailsForACvQualifiedTypeWithoutTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<user_destructor const>);
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<user_copy_constructor volatile>);
}

TEST(ConceptsImplicitLifetime, FailsForAReferenceToATypeWithTheProperty)
{
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<lvalue_reference_to<trivial_constructor>>);
    STATIC_EXPECT_FALSE(::scl::concepts::implicit_lifetime<rvalue_reference_to<trivial_constructor>>);
}

TEST(ConceptsImplicitLifetime, SubsumesObject)
{
    STATIC_EXPECT_EQ(implicit_lifetime_over_object<trivial_class>(), 2);
}

TEST(ConceptsImplicitLifetime, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(implicit_lifetime_over_non_reference<trivial_class>(), 2);
}
#endif
