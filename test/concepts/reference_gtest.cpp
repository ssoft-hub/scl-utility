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
