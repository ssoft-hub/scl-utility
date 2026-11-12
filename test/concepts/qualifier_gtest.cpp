#include <gtest_utils.h>

#include <scl/utility/concepts/qualifier.h>

namespace
{
    template <::scl::concepts::const_qualified Type>
    constexpr int cv_qualified_over_const_qualified()
    {
        return 1;
    }

    template <::scl::concepts::cv_qualified Type>
    constexpr int cv_qualified_over_const_qualified()
    {
        return 2;
    }

    template <::scl::concepts::volatile_qualified Type>
    constexpr int cv_qualified_over_volatile_qualified()
    {
        return 1;
    }

    template <::scl::concepts::cv_qualified Type>
    constexpr int cv_qualified_over_volatile_qualified()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int const_qualified_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::const_qualified Type>
    constexpr int const_qualified_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int volatile_qualified_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::volatile_qualified Type>
    constexpr int volatile_qualified_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::non_reference Type>
    constexpr int cv_qualified_over_non_reference()
    {
        return 1;
    }

    template <::scl::concepts::cv_qualified Type>
    constexpr int cv_qualified_over_non_reference()
    {
        return 2;
    }

    template <::scl::concepts::const_qualified Type>
    constexpr int const_or_volatile_qualified()
    {
        return 1;
    }

    template <::scl::concepts::volatile_qualified Type>
    constexpr int const_or_volatile_qualified()
    {
        return 2;
    }

    template <typename Type>
    constexpr bool const_or_volatile_qualified_resolves =
        requires { const_or_volatile_qualified<Type>(); };
} // namespace

TEST(ConceptsConstQualified, HoldsForATypeDeclaredConst)
{
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<int const>);
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<int * const>);
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<void const>);
}

TEST(ConceptsConstQualified, HoldsForATypeDeclaredConstAndVolatile)
{
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<int const volatile>);
}

TEST(ConceptsConstQualified, HoldsForAnArrayOfConstElements)
{
    STATIC_EXPECT_TRUE(::scl::concepts::const_qualified<int const[3]>);
}

TEST(ConceptsConstQualified, FailsForATypeWithoutConst)
{
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<int volatile>);
}

TEST(ConceptsConstQualified, FailsForAPointerToConst)
{
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<int const *>);
}

TEST(ConceptsConstQualified, FailsForAReferenceToConst)
{
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<int const &>);
    STATIC_EXPECT_FALSE(::scl::concepts::const_qualified<int const &&>);
}

TEST(ConceptsVolatileQualified, HoldsForATypeDeclaredVolatile)
{
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<int volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<int * volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<void volatile>);
}

TEST(ConceptsVolatileQualified, HoldsForATypeDeclaredConstAndVolatile)
{
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<int const volatile>);
}

TEST(ConceptsVolatileQualified, HoldsForAnArrayOfVolatileElements)
{
    STATIC_EXPECT_TRUE(::scl::concepts::volatile_qualified<int volatile[3]>);
}

TEST(ConceptsVolatileQualified, FailsForATypeWithoutVolatile)
{
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<int>);
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<int const>);
}

TEST(ConceptsVolatileQualified, FailsForAPointerToVolatile)
{
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<int volatile *>);
}

TEST(ConceptsVolatileQualified, FailsForAReferenceToVolatile)
{
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<int volatile &>);
    STATIC_EXPECT_FALSE(::scl::concepts::volatile_qualified<int volatile &&>);
}

TEST(ConceptsCvQualified, HoldsForATypeDeclaredConstAndVolatile)
{
    STATIC_EXPECT_TRUE(::scl::concepts::cv_qualified<int const volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::cv_qualified<void const volatile>);
    STATIC_EXPECT_TRUE(::scl::concepts::cv_qualified<int const volatile[3]>);
}

TEST(ConceptsCvQualified, FailsForATypeWithOneQualifier)
{
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<int const>);
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<int volatile>);
}

TEST(ConceptsCvQualified, FailsForATypeWithoutQualifiers)
{
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<int>);
}

TEST(ConceptsCvQualified, FailsForAReferenceToAConstVolatileType)
{
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<int const volatile &>);
    STATIC_EXPECT_FALSE(::scl::concepts::cv_qualified<int const volatile &&>);
}

TEST(ConceptsCvQualified, SubsumesConstQualified)
{
    STATIC_EXPECT_EQ(cv_qualified_over_const_qualified<int const volatile>(), 2);
}

TEST(ConceptsCvQualified, SubsumesVolatileQualified)
{
    STATIC_EXPECT_EQ(cv_qualified_over_volatile_qualified<int const volatile>(), 2);
}

TEST(ConceptsConstQualified, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(const_qualified_over_non_reference<int const>(), 2);
}

TEST(ConceptsVolatileQualified, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(volatile_qualified_over_non_reference<int volatile>(), 2);
}

TEST(ConceptsCvQualified, SubsumesNonReference)
{
    STATIC_EXPECT_EQ(cv_qualified_over_non_reference<int const volatile>(), 2);
}

TEST(ConceptsConstQualified, IsUnorderedWithVolatileQualified)
{
    STATIC_EXPECT_FALSE(const_or_volatile_qualified_resolves<int const volatile>);
}
