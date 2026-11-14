#include <gtest_utils.h>

#include <scl/utility/concepts/reference.h>

using namespace scl::concepts;

TEST(Reference, reference)
{
    STATIC_EXPECT_TRUE(reference<int &>);
    STATIC_EXPECT_TRUE(reference<int &&>);
    STATIC_EXPECT_TRUE(reference<int const &>);
    STATIC_EXPECT_TRUE(reference<int volatile &&>);
    STATIC_EXPECT_FALSE(reference<int>);
    STATIC_EXPECT_FALSE(reference<int *>);
    STATIC_EXPECT_FALSE(reference<void>);
}

TEST(Reference, lvalue_reference)
{
    STATIC_EXPECT_TRUE(lvalue_reference<int &>);
    STATIC_EXPECT_TRUE(lvalue_reference<int const &>);
    STATIC_EXPECT_TRUE(lvalue_reference<int volatile &>);
    STATIC_EXPECT_FALSE(lvalue_reference<int &&>);
    STATIC_EXPECT_FALSE(lvalue_reference<int>);
}

TEST(Reference, rvalue_reference)
{
    STATIC_EXPECT_TRUE(rvalue_reference<int &&>);
    STATIC_EXPECT_TRUE(rvalue_reference<int const &&>);
    STATIC_EXPECT_TRUE(rvalue_reference<int volatile &&>);
    STATIC_EXPECT_FALSE(rvalue_reference<int &>);
    STATIC_EXPECT_FALSE(rvalue_reference<int>);
}

TEST(Reference, non_reference)
{
    STATIC_EXPECT_TRUE(non_reference<int>);
    STATIC_EXPECT_TRUE(non_reference<int *>);
    STATIC_EXPECT_TRUE(non_reference<void>);
    STATIC_EXPECT_TRUE(non_reference<int const>);
    STATIC_EXPECT_TRUE(non_reference<int volatile>);
    STATIC_EXPECT_FALSE(non_reference<int &>);
    STATIC_EXPECT_FALSE(non_reference<int const &>);
    STATIC_EXPECT_FALSE(non_reference<int &&>);
    STATIC_EXPECT_FALSE(non_reference<int volatile &&>);
}

namespace
{
    template <compound Type>
    constexpr int reference_over_compound()
    {
        return 1;
    }

    template <reference Type>
    constexpr int reference_over_compound()
    {
        return 2;
    }

    template <reference Type>
    constexpr int lvalue_reference_over_reference()
    {
        return 1;
    }

    template <lvalue_reference Type>
    constexpr int lvalue_reference_over_reference()
    {
        return 2;
    }

    template <reference Type>
    constexpr int rvalue_reference_over_reference()
    {
        return 1;
    }

    template <rvalue_reference Type>
    constexpr int rvalue_reference_over_reference()
    {
        return 2;
    }
} // namespace

TEST(Reference, reference_subsumes_compound)
{
    STATIC_EXPECT_EQ(reference_over_compound<int &>(), 2);
    STATIC_EXPECT_EQ(reference_over_compound<int *>(), 1);
}

TEST(Reference, lvalue_reference_subsumes_reference)
{
    STATIC_EXPECT_EQ(lvalue_reference_over_reference<int &>(), 2);
    STATIC_EXPECT_EQ(lvalue_reference_over_reference<int &&>(), 1);
}

TEST(Reference, rvalue_reference_subsumes_reference)
{
    STATIC_EXPECT_EQ(rvalue_reference_over_reference<int &&>(), 2);
    STATIC_EXPECT_EQ(rvalue_reference_over_reference<int &>(), 1);
}

namespace
{
    template <compound Type>
    constexpr int lvalue_reference_over_compound()
    {
        return 1;
    }

    template <lvalue_reference Type>
    constexpr int lvalue_reference_over_compound()
    {
        return 2;
    }

    template <compound Type>
    constexpr int rvalue_reference_over_compound()
    {
        return 1;
    }

    template <rvalue_reference Type>
    constexpr int rvalue_reference_over_compound()
    {
        return 2;
    }
} // namespace

TEST(Reference, lvalue_reference_subsumes_compound)
{
    STATIC_EXPECT_EQ(lvalue_reference_over_compound<int &>(), 2);
    STATIC_EXPECT_EQ(lvalue_reference_over_compound<int &&>(), 1);
}

TEST(Reference, rvalue_reference_subsumes_compound)
{
    STATIC_EXPECT_EQ(rvalue_reference_over_compound<int &&>(), 2);
    STATIC_EXPECT_EQ(rvalue_reference_over_compound<int &>(), 1);
}
