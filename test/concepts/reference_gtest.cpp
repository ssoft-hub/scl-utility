#include <gtest_utils.h>

#include <scl/utility/concepts/reference.h>

namespace
{
    template <::scl::concepts::compound Type>
    constexpr int reference_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::reference Type>
    constexpr int reference_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::reference Type>
    constexpr int lvalue_reference_over_reference()
    {
        return 1;
    }

    template <::scl::concepts::lvalue_reference Type>
    constexpr int lvalue_reference_over_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int lvalue_reference_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::lvalue_reference Type>
    constexpr int lvalue_reference_over_compound()
    {
        return 2;
    }

    template <::scl::concepts::reference Type>
    constexpr int rvalue_reference_over_reference()
    {
        return 1;
    }

    template <::scl::concepts::rvalue_reference Type>
    constexpr int rvalue_reference_over_reference()
    {
        return 2;
    }

    template <::scl::concepts::compound Type>
    constexpr int rvalue_reference_over_compound()
    {
        return 1;
    }

    template <::scl::concepts::rvalue_reference Type>
    constexpr int rvalue_reference_over_compound()
    {
        return 2;
    }

    template <typename Argument>
    constexpr bool deduced_as_lvalue_reference(Argument &&)
    {
        return ::scl::concepts::lvalue_reference<Argument>;
    }

    template <typename Argument>
    constexpr bool deduced_as_rvalue_reference(Argument &&)
    {
        return ::scl::concepts::rvalue_reference<Argument>;
    }
} // namespace

TEST(ConceptsReference, HoldsForAnLvalueReference)
{
    STATIC_EXPECT_TRUE(::scl::concepts::reference<int &>);
    STATIC_EXPECT_TRUE(::scl::concepts::reference<int const &>);
    STATIC_EXPECT_TRUE(::scl::concepts::reference<void (&)()>);
}

TEST(ConceptsReference, HoldsForAnRvalueReference)
{
    STATIC_EXPECT_TRUE(::scl::concepts::reference<int &&>);
    STATIC_EXPECT_TRUE(::scl::concepts::reference<int volatile &&>);
    STATIC_EXPECT_TRUE(::scl::concepts::reference<void (&&)()>);
}

TEST(ConceptsReference, FailsForAPointer)
{
    STATIC_EXPECT_FALSE(::scl::concepts::reference<int *>);
}

TEST(ConceptsReference, FailsForATypeThatIsNotAReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::reference<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::reference<void>);
    STATIC_EXPECT_FALSE(::scl::concepts::reference<void()>);
}

TEST(ConceptsReference, SubsumesCompound) { STATIC_EXPECT_EQ(reference_over_compound<int &>(), 2); }

TEST(ConceptsLvalueReference, HoldsForAnLvalueReference)
{
    STATIC_EXPECT_TRUE(::scl::concepts::lvalue_reference<int &>);
    STATIC_EXPECT_TRUE(::scl::concepts::lvalue_reference<int const &>);
    STATIC_EXPECT_TRUE(::scl::concepts::lvalue_reference<int volatile &>);
}

TEST(ConceptsLvalueReference, FailsForAnRvalueReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::lvalue_reference<int &&>);
    STATIC_EXPECT_FALSE(::scl::concepts::lvalue_reference<int const &&>);
}

TEST(ConceptsLvalueReference, FailsForATypeThatIsNotAReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::lvalue_reference<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::lvalue_reference<int *>);
}

TEST(ConceptsLvalueReference, HoldsForTheTypeDeducedFromAnLvalueThroughAForwardingReference)
{
    int const value = 0;
    STATIC_EXPECT_TRUE(deduced_as_lvalue_reference(value));
}

TEST(ConceptsLvalueReference, FailsForTheTypeDeducedFromAnRvalueThroughAForwardingReference)
{
    STATIC_EXPECT_FALSE(deduced_as_lvalue_reference(0));
}

TEST(ConceptsLvalueReference, SubsumesReference)
{
    STATIC_EXPECT_EQ(lvalue_reference_over_reference<int &>(), 2);
}

TEST(ConceptsLvalueReference, SubsumesCompound)
{
    STATIC_EXPECT_EQ(lvalue_reference_over_compound<int &>(), 2);
}

TEST(ConceptsRvalueReference, HoldsForAnRvalueReference)
{
    STATIC_EXPECT_TRUE(::scl::concepts::rvalue_reference<int &&>);
    STATIC_EXPECT_TRUE(::scl::concepts::rvalue_reference<int const &&>);
    STATIC_EXPECT_TRUE(::scl::concepts::rvalue_reference<int volatile &&>);
}

TEST(ConceptsRvalueReference, FailsForAnLvalueReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::rvalue_reference<int &>);
    STATIC_EXPECT_FALSE(::scl::concepts::rvalue_reference<int const &>);
}

TEST(ConceptsRvalueReference, FailsForATypeThatIsNotAReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::rvalue_reference<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::rvalue_reference<int *>);
}

TEST(ConceptsRvalueReference, FailsForTheTypeDeducedFromAnRvalueThroughAForwardingReference)
{
    STATIC_EXPECT_FALSE(deduced_as_rvalue_reference(0));
}

TEST(ConceptsRvalueReference, FailsForTheTypeDeducedFromAnLvalueThroughAForwardingReference)
{
    int const value = 0;
    STATIC_EXPECT_FALSE(deduced_as_rvalue_reference(value));
}

TEST(ConceptsRvalueReference, SubsumesReference)
{
    STATIC_EXPECT_EQ(rvalue_reference_over_reference<int &&>(), 2);
}

TEST(ConceptsRvalueReference, SubsumesCompound)
{
    STATIC_EXPECT_EQ(rvalue_reference_over_compound<int &&>(), 2);
}

TEST(ConceptsNonReference, HoldsForAnObjectType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<int>);
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<int *>);
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<int const>);
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<int volatile>);
}

TEST(ConceptsNonReference, HoldsForAFunctionType)
{
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<void(int)>);
}

TEST(ConceptsNonReference, HoldsForVoid)
{
    STATIC_EXPECT_TRUE(::scl::concepts::non_reference<void>);
}

TEST(ConceptsNonReference, FailsForAnLvalueReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::non_reference<int &>);
    STATIC_EXPECT_FALSE(::scl::concepts::non_reference<int const &>);
}

TEST(ConceptsNonReference, FailsForAnRvalueReference)
{
    STATIC_EXPECT_FALSE(::scl::concepts::non_reference<int &&>);
    STATIC_EXPECT_FALSE(::scl::concepts::non_reference<int volatile &&>);
}
