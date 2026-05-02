#include <gtest_utils.h>

#include <scl/utility/concepts/qualifier.h>

using namespace scl::concepts;

TEST(Qualifier, const_type)
{
    STATIC_EXPECT_TRUE(const_type<int const>);
    STATIC_EXPECT_TRUE(const_type<int const volatile>);
    STATIC_EXPECT_TRUE(const_type<void const>);
    STATIC_EXPECT_FALSE(const_type<int>);
    STATIC_EXPECT_FALSE(const_type<int volatile>);
}

TEST(Qualifier, volatile_type)
{
    STATIC_EXPECT_TRUE(volatile_type<int volatile>);
    STATIC_EXPECT_TRUE(volatile_type<int const volatile>);
    STATIC_EXPECT_TRUE(volatile_type<void volatile>);
    STATIC_EXPECT_FALSE(volatile_type<int>);
    STATIC_EXPECT_FALSE(volatile_type<int const>);
}

TEST(Qualifier, cv_type)
{
    STATIC_EXPECT_TRUE(cv_type<int const volatile>);
    STATIC_EXPECT_TRUE(cv_type<void const volatile>);
    STATIC_EXPECT_FALSE(cv_type<int const>);
    STATIC_EXPECT_FALSE(cv_type<int volatile>);
    STATIC_EXPECT_FALSE(cv_type<int>);
}

// Reference types are never top-level const/volatile-qualified.
// std::is_const_v / std::is_volatile_v check the type itself, not the referent.

TEST(Qualifier, const_type_reference)
{
    STATIC_EXPECT_FALSE(const_type<int &>);
    STATIC_EXPECT_FALSE(const_type<int const &>); // ref-to-const, not a const type
    STATIC_EXPECT_FALSE(const_type<int &&>);
    STATIC_EXPECT_FALSE(const_type<int const &&>);
}

TEST(Qualifier, volatile_type_reference)
{
    STATIC_EXPECT_FALSE(volatile_type<int &>);
    STATIC_EXPECT_FALSE(volatile_type<int volatile &>);
    STATIC_EXPECT_FALSE(volatile_type<int &&>);
    STATIC_EXPECT_FALSE(volatile_type<int volatile &&>);
}

TEST(Qualifier, cv_type_reference)
{
    STATIC_EXPECT_FALSE(cv_type<int &>);
    STATIC_EXPECT_FALSE(cv_type<int const volatile &>);
    STATIC_EXPECT_FALSE(cv_type<int const volatile &&>);
}
