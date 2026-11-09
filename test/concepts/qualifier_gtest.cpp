#include <gtest_utils.h>

#include <scl/utility/concepts/qualifier.h>

using namespace scl::concepts;

TEST(Qualifier, const_qualified)
{
    STATIC_EXPECT_TRUE(const_qualified<int const>);
    STATIC_EXPECT_TRUE(const_qualified<int const volatile>);
    STATIC_EXPECT_TRUE(const_qualified<void const>);
    STATIC_EXPECT_FALSE(const_qualified<int>);
    STATIC_EXPECT_FALSE(const_qualified<int volatile>);
}

TEST(Qualifier, volatile_qualified)
{
    STATIC_EXPECT_TRUE(volatile_qualified<int volatile>);
    STATIC_EXPECT_TRUE(volatile_qualified<int const volatile>);
    STATIC_EXPECT_TRUE(volatile_qualified<void volatile>);
    STATIC_EXPECT_FALSE(volatile_qualified<int>);
    STATIC_EXPECT_FALSE(volatile_qualified<int const>);
}

TEST(Qualifier, cv_qualified)
{
    STATIC_EXPECT_TRUE(cv_qualified<int const volatile>);
    STATIC_EXPECT_TRUE(cv_qualified<void const volatile>);
    STATIC_EXPECT_FALSE(cv_qualified<int const>);
    STATIC_EXPECT_FALSE(cv_qualified<int volatile>);
    STATIC_EXPECT_FALSE(cv_qualified<int>);
}

// Reference types are never top-level const/volatile-qualified.
// std::is_const_v / std::is_volatile_v check the type itself, not the referent.

TEST(Qualifier, const_qualified_reference)
{
    STATIC_EXPECT_FALSE(const_qualified<int &>);
    STATIC_EXPECT_FALSE(const_qualified<int const &>); // ref-to-const, not a const type
    STATIC_EXPECT_FALSE(const_qualified<int &&>);
    STATIC_EXPECT_FALSE(const_qualified<int const &&>);
}

TEST(Qualifier, volatile_qualified_reference)
{
    STATIC_EXPECT_FALSE(volatile_qualified<int &>);
    STATIC_EXPECT_FALSE(volatile_qualified<int volatile &>);
    STATIC_EXPECT_FALSE(volatile_qualified<int &&>);
    STATIC_EXPECT_FALSE(volatile_qualified<int volatile &&>);
}

TEST(Qualifier, cv_qualified_reference)
{
    STATIC_EXPECT_FALSE(cv_qualified<int &>);
    STATIC_EXPECT_FALSE(cv_qualified<int const volatile &>);
    STATIC_EXPECT_FALSE(cv_qualified<int const volatile &&>);
}
