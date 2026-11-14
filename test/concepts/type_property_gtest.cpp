#include <gtest_utils.h>

#include <scl/utility/concepts/type_category.h>

#include <string>

using namespace scl::concepts;

namespace
{
    struct Trivial
    {
        int x;
    };

    struct NonTrivial
    {
        ::std::string s;
    };

    struct Empty
    {};

    struct PolymorphicBase
    {
        virtual ~PolymorphicBase() = default;
    };

    struct Abstract
    {
        virtual void f() = 0;
    };

    struct Final final
    {};

    struct Standard
    {
        int x;
        float y;
    };

    union AggregateUnion
    {
        int i;
        float f;
    };

    union FinalUnion final
    {
        int i;
        float f;
    };

    struct UserDestructor
    {
        int x;

        ~UserDestructor() {}
    };
} // namespace

TEST(TypeProperty, trivially_copyable)
{
    STATIC_EXPECT_TRUE(trivially_copyable<int>);
    STATIC_EXPECT_TRUE(trivially_copyable<Trivial>);
    STATIC_EXPECT_FALSE(trivially_copyable<NonTrivial>);
}

TEST(TypeProperty, standard_layout)
{
    STATIC_EXPECT_TRUE(standard_layout<int>);
    STATIC_EXPECT_TRUE(standard_layout<Standard>);
    STATIC_EXPECT_FALSE(standard_layout<PolymorphicBase>);
}

TEST(TypeProperty, empty)
{
    STATIC_EXPECT_TRUE(empty<Empty>);
    STATIC_EXPECT_FALSE(empty<int>);
    STATIC_EXPECT_FALSE(empty<Trivial>);
}

TEST(TypeProperty, polymorphic)
{
    STATIC_EXPECT_TRUE(polymorphic<PolymorphicBase>);
    STATIC_EXPECT_FALSE(polymorphic<int>);
    STATIC_EXPECT_FALSE(polymorphic<Trivial>);
}

TEST(TypeProperty, abstract)
{
    STATIC_EXPECT_TRUE(abstract<Abstract>);
    STATIC_EXPECT_FALSE(abstract<PolymorphicBase>);
    STATIC_EXPECT_FALSE(abstract<int>);
}

TEST(TypeProperty, final)
{
    STATIC_EXPECT_TRUE(final<Final>);
    STATIC_EXPECT_TRUE(final<FinalUnion>);
    STATIC_EXPECT_FALSE(final<int>);
    STATIC_EXPECT_FALSE(final<Trivial>);
}

TEST(TypeProperty, aggregate)
{
    STATIC_EXPECT_TRUE(aggregate<Trivial>);
    STATIC_EXPECT_TRUE(aggregate<AggregateUnion>);
    STATIC_EXPECT_FALSE(aggregate<PolymorphicBase>);
    STATIC_EXPECT_FALSE(aggregate<Abstract>);
}

namespace
{
    template <object Type>
    constexpr int trivially_copyable_over_object()
    {
        return 1;
    }

    template <trivially_copyable Type>
    constexpr int trivially_copyable_over_object()
    {
        return 2;
    }

    template <object Type>
    constexpr int standard_layout_over_object()
    {
        return 1;
    }

    template <standard_layout Type>
    constexpr int standard_layout_over_object()
    {
        return 2;
    }

    template <class_type Type>
    constexpr int empty_over_class_type()
    {
        return 1;
    }

    template <empty Type>
    constexpr int empty_over_class_type()
    {
        return 2;
    }

    template <class_type Type>
    constexpr int polymorphic_over_class_type()
    {
        return 1;
    }

    template <polymorphic Type>
    constexpr int polymorphic_over_class_type()
    {
        return 2;
    }

    template <polymorphic Type>
    constexpr int abstract_over_polymorphic()
    {
        return 1;
    }

    template <abstract Type>
    constexpr int abstract_over_polymorphic()
    {
        return 2;
    }

    template <typename Type>
        requires(class_type<Type> || union_type<Type>)
    constexpr int final_over_class_or_union()
    {
        return 1;
    }

    template <final Type>
    constexpr int final_over_class_or_union()
    {
        return 2;
    }

} // namespace

TEST(TypeProperty, trivially_copyable_subsumes_object)
{
    STATIC_EXPECT_EQ(trivially_copyable_over_object<Trivial>(), 2);
    STATIC_EXPECT_EQ(trivially_copyable_over_object<NonTrivial>(), 1);
}

TEST(TypeProperty, standard_layout_subsumes_object)
{
    STATIC_EXPECT_EQ(standard_layout_over_object<Standard>(), 2);
    STATIC_EXPECT_EQ(standard_layout_over_object<PolymorphicBase>(), 1);
}

TEST(TypeProperty, empty_subsumes_class_type)
{
    STATIC_EXPECT_EQ(empty_over_class_type<Empty>(), 2);
    STATIC_EXPECT_EQ(empty_over_class_type<Trivial>(), 1);
}

TEST(TypeProperty, polymorphic_subsumes_class_type)
{
    STATIC_EXPECT_EQ(polymorphic_over_class_type<PolymorphicBase>(), 2);
    STATIC_EXPECT_EQ(polymorphic_over_class_type<Trivial>(), 1);
}

TEST(TypeProperty, abstract_subsumes_polymorphic)
{
    STATIC_EXPECT_EQ(abstract_over_polymorphic<Abstract>(), 2);
    STATIC_EXPECT_EQ(abstract_over_polymorphic<PolymorphicBase>(), 1);
}

TEST(TypeProperty, final_subsumes_class_or_union)
{
    STATIC_EXPECT_EQ(final_over_class_or_union<Final>(), 2);
    STATIC_EXPECT_EQ(final_over_class_or_union<FinalUnion>(), 2);
    STATIC_EXPECT_EQ(final_over_class_or_union<AggregateUnion>(), 1);
    STATIC_EXPECT_EQ(final_over_class_or_union<Trivial>(), 1);
}

namespace
{
    template <non_reference Type>
    constexpr int trivially_copyable_over_non_reference()
    {
        return 1;
    }

    template <trivially_copyable Type>
    constexpr int trivially_copyable_over_non_reference()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int standard_layout_over_non_reference()
    {
        return 1;
    }

    template <standard_layout Type>
    constexpr int standard_layout_over_non_reference()
    {
        return 2;
    }

    template <object Type>
    constexpr int empty_over_object()
    {
        return 1;
    }

    template <empty Type>
    constexpr int empty_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int empty_over_non_reference()
    {
        return 1;
    }

    template <empty Type>
    constexpr int empty_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int empty_over_compound()
    {
        return 1;
    }

    template <empty Type>
    constexpr int empty_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int polymorphic_over_object()
    {
        return 1;
    }

    template <polymorphic Type>
    constexpr int polymorphic_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int polymorphic_over_non_reference()
    {
        return 1;
    }

    template <polymorphic Type>
    constexpr int polymorphic_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int polymorphic_over_compound()
    {
        return 1;
    }

    template <polymorphic Type>
    constexpr int polymorphic_over_compound()
    {
        return 2;
    }

    template <class_type Type>
    constexpr int abstract_over_class_type()
    {
        return 1;
    }

    template <abstract Type>
    constexpr int abstract_over_class_type()
    {
        return 2;
    }

    template <object Type>
    constexpr int abstract_over_object()
    {
        return 1;
    }

    template <abstract Type>
    constexpr int abstract_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int abstract_over_non_reference()
    {
        return 1;
    }

    template <abstract Type>
    constexpr int abstract_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int abstract_over_compound()
    {
        return 1;
    }

    template <abstract Type>
    constexpr int abstract_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int final_over_object()
    {
        return 1;
    }

    template <final Type>
    constexpr int final_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int final_over_non_reference()
    {
        return 1;
    }

    template <final Type>
    constexpr int final_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int final_over_compound()
    {
        return 1;
    }

    template <final Type>
    constexpr int final_over_compound()
    {
        return 2;
    }

    template <object Type>
    constexpr int aggregate_over_object()
    {
        return 1;
    }

    template <aggregate Type>
    constexpr int aggregate_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int aggregate_over_non_reference()
    {
        return 1;
    }

    template <aggregate Type>
    constexpr int aggregate_over_non_reference()
    {
        return 2;
    }

    template <compound Type>
    constexpr int aggregate_over_compound()
    {
        return 1;
    }

    template <aggregate Type>
    constexpr int aggregate_over_compound()
    {
        return 2;
    }
} // namespace

TEST(TypeProperty, trivially_copyable_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(trivially_copyable_over_non_reference<int>(), 2);
    STATIC_EXPECT_EQ(trivially_copyable_over_non_reference<void>(), 1);
}

TEST(TypeProperty, standard_layout_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(standard_layout_over_non_reference<int>(), 2);
    STATIC_EXPECT_EQ(standard_layout_over_non_reference<void>(), 1);
}

TEST(TypeProperty, empty_subsumes_object)
{
    STATIC_EXPECT_EQ(empty_over_object<Empty>(), 2);
    STATIC_EXPECT_EQ(empty_over_object<int>(), 1);
}

TEST(TypeProperty, empty_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(empty_over_non_reference<Empty>(), 2);
    STATIC_EXPECT_EQ(empty_over_non_reference<int>(), 1);
}

TEST(TypeProperty, empty_subsumes_compound)
{
    STATIC_EXPECT_EQ(empty_over_compound<Empty>(), 2);
    STATIC_EXPECT_EQ(empty_over_compound<int *>(), 1);
}

TEST(TypeProperty, polymorphic_subsumes_object)
{
    STATIC_EXPECT_EQ(polymorphic_over_object<PolymorphicBase>(), 2);
    STATIC_EXPECT_EQ(polymorphic_over_object<int>(), 1);
}

TEST(TypeProperty, polymorphic_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(polymorphic_over_non_reference<PolymorphicBase>(), 2);
    STATIC_EXPECT_EQ(polymorphic_over_non_reference<int>(), 1);
}

TEST(TypeProperty, polymorphic_subsumes_compound)
{
    STATIC_EXPECT_EQ(polymorphic_over_compound<PolymorphicBase>(), 2);
    STATIC_EXPECT_EQ(polymorphic_over_compound<int *>(), 1);
}

TEST(TypeProperty, abstract_subsumes_class_type)
{
    STATIC_EXPECT_EQ(abstract_over_class_type<Abstract>(), 2);
    STATIC_EXPECT_EQ(abstract_over_class_type<Trivial>(), 1);
}

TEST(TypeProperty, abstract_subsumes_object)
{
    STATIC_EXPECT_EQ(abstract_over_object<Abstract>(), 2);
    STATIC_EXPECT_EQ(abstract_over_object<int>(), 1);
}

TEST(TypeProperty, abstract_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(abstract_over_non_reference<Abstract>(), 2);
    STATIC_EXPECT_EQ(abstract_over_non_reference<int>(), 1);
}

TEST(TypeProperty, abstract_subsumes_compound)
{
    STATIC_EXPECT_EQ(abstract_over_compound<Abstract>(), 2);
    STATIC_EXPECT_EQ(abstract_over_compound<int *>(), 1);
}

TEST(TypeProperty, final_subsumes_object)
{
    STATIC_EXPECT_EQ(final_over_object<Final>(), 2);
    STATIC_EXPECT_EQ(final_over_object<int>(), 1);
}

TEST(TypeProperty, final_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(final_over_non_reference<Final>(), 2);
    STATIC_EXPECT_EQ(final_over_non_reference<int>(), 1);
}

TEST(TypeProperty, final_subsumes_compound)
{
    STATIC_EXPECT_EQ(final_over_compound<Final>(), 2);
    STATIC_EXPECT_EQ(final_over_compound<int *>(), 1);
}

TEST(TypeProperty, aggregate_subsumes_object)
{
    STATIC_EXPECT_EQ(aggregate_over_object<Trivial>(), 2);
    STATIC_EXPECT_EQ(aggregate_over_object<int>(), 1);
}

TEST(TypeProperty, aggregate_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(aggregate_over_non_reference<Trivial>(), 2);
    STATIC_EXPECT_EQ(aggregate_over_non_reference<int>(), 1);
}

TEST(TypeProperty, aggregate_subsumes_compound)
{
    STATIC_EXPECT_EQ(aggregate_over_compound<Trivial>(), 2);
    STATIC_EXPECT_EQ(aggregate_over_compound<int *>(), 1);
}

namespace
{
    template <trivially_copyable Type>
    constexpr int copyable_or_layout()
    {
        return 1;
    }

    template <standard_layout Type>
    constexpr int copyable_or_layout()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool copyable_or_layout_resolves = requires { copyable_or_layout<Type>(); };

    template <class_type Type>
    constexpr int class_or_final()
    {
        return 1;
    }

    template <final Type>
    constexpr int class_or_final()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool class_or_final_resolves = requires { class_or_final<Type>(); };

    template <union_type Type>
    constexpr int union_or_final()
    {
        return 1;
    }

    template <final Type>
    constexpr int union_or_final()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool union_or_final_resolves = requires { union_or_final<Type>(); };

    template <class_type Type>
    constexpr int class_or_aggregate()
    {
        return 1;
    }

    template <aggregate Type>
    constexpr int class_or_aggregate()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool class_or_aggregate_resolves = requires { class_or_aggregate<Type>(); };

    template <union_type Type>
    constexpr int union_or_aggregate()
    {
        return 1;
    }

    template <aggregate Type>
    constexpr int union_or_aggregate()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool union_or_aggregate_resolves = requires { union_or_aggregate<Type>(); };
} // namespace

TEST(TypeProperty, trivially_copyable_and_standard_layout_are_unordered)
{
    STATIC_EXPECT_FALSE(copyable_or_layout_resolves<int>);
    STATIC_EXPECT_TRUE(copyable_or_layout_resolves<UserDestructor>);
}

TEST(TypeProperty, final_does_not_subsume_class_type_or_union_type)
{
    STATIC_EXPECT_FALSE(class_or_final_resolves<Final>);
    STATIC_EXPECT_TRUE(class_or_final_resolves<Trivial>);
    STATIC_EXPECT_FALSE(union_or_final_resolves<FinalUnion>);
    STATIC_EXPECT_TRUE(union_or_final_resolves<AggregateUnion>);
}

TEST(TypeProperty, aggregate_and_class_or_union_are_unordered)
{
    STATIC_EXPECT_FALSE(class_or_aggregate_resolves<Trivial>);
    STATIC_EXPECT_TRUE(class_or_aggregate_resolves<PolymorphicBase>);
    STATIC_EXPECT_FALSE(union_or_aggregate_resolves<AggregateUnion>);
}

TEST(TypeProperty, scl_has_implicit_lifetime_concept)
{
#if defined(__cpp_lib_is_implicit_lifetime) && __cpp_lib_is_implicit_lifetime >= 202302L
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 1);
#elif defined(__has_builtin)
#if __has_builtin(__builtin_is_implicit_lifetime)
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 1);
#else
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 0);
#endif
#else
    STATIC_EXPECT_EQ(SCL_HAS_IMPLICIT_LIFETIME_CONCEPT, 0);
#endif
}

#if SCL_HAS_IMPLICIT_LIFETIME_CONCEPT
namespace
{
    template <object Type>
    constexpr int implicit_lifetime_over_object()
    {
        return 1;
    }

    template <implicit_lifetime Type>
    constexpr int implicit_lifetime_over_object()
    {
        return 2;
    }

    template <non_reference Type>
    constexpr int implicit_lifetime_over_non_reference()
    {
        return 1;
    }

    template <implicit_lifetime Type>
    constexpr int implicit_lifetime_over_non_reference()
    {
        return 2;
    }

    struct TrivialConstructor
    {
        TrivialConstructor() = default;

        explicit TrivialConstructor(int value)
            : x{value}
        {}

        int x;
    };

    struct DeletedDestructor
    {
        explicit DeletedDestructor(int) {}

        ~DeletedDestructor() = delete;
    };

    struct AggregateDeletedDestructor
    {
        int x;

        ~AggregateDeletedDestructor() = delete;
    };
} // namespace

TEST(TypeProperty, implicit_lifetime)
{
    STATIC_EXPECT_TRUE(implicit_lifetime<int>);
    STATIC_EXPECT_TRUE(implicit_lifetime<int const>);
    STATIC_EXPECT_TRUE(implicit_lifetime<int *>);
    STATIC_EXPECT_TRUE(implicit_lifetime<int[3]>);
    STATIC_EXPECT_TRUE(implicit_lifetime<int[]>);
    STATIC_EXPECT_TRUE(implicit_lifetime<Trivial>);
    STATIC_EXPECT_TRUE(implicit_lifetime<NonTrivial>);
    STATIC_EXPECT_TRUE(implicit_lifetime<NonTrivial[2]>);
    STATIC_EXPECT_TRUE(implicit_lifetime<UserDestructor[2]>);
    STATIC_EXPECT_TRUE(implicit_lifetime<PolymorphicBase[]>);
    STATIC_EXPECT_TRUE(implicit_lifetime<TrivialConstructor const volatile>);
    STATIC_EXPECT_FALSE(implicit_lifetime<UserDestructor>);
    STATIC_EXPECT_FALSE(implicit_lifetime<PolymorphicBase>);
    STATIC_EXPECT_FALSE(implicit_lifetime<void>);
    STATIC_EXPECT_FALSE(implicit_lifetime<int &>);
    STATIC_EXPECT_FALSE(implicit_lifetime<void()>);
    STATIC_EXPECT_FALSE(aggregate<TrivialConstructor>);
    STATIC_EXPECT_TRUE(implicit_lifetime<TrivialConstructor>);
    STATIC_EXPECT_FALSE(implicit_lifetime<DeletedDestructor>);
    STATIC_EXPECT_TRUE(aggregate<AggregateDeletedDestructor>);
    STATIC_EXPECT_TRUE(implicit_lifetime<AggregateDeletedDestructor>);
}

TEST(TypeProperty, implicit_lifetime_subsumes_object)
{
    STATIC_EXPECT_EQ(implicit_lifetime_over_object<Trivial>(), 2);
    STATIC_EXPECT_EQ(implicit_lifetime_over_object<UserDestructor>(), 1);
}

TEST(TypeProperty, implicit_lifetime_subsumes_non_reference)
{
    STATIC_EXPECT_EQ(implicit_lifetime_over_non_reference<Trivial>(), 2);
    STATIC_EXPECT_EQ(implicit_lifetime_over_non_reference<void>(), 1);
}
#endif
