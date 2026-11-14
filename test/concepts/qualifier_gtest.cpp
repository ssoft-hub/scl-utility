#include <gtest_utils.h>

#include <scl/utility/concepts/qualifier.h>

using namespace scl::concepts;

namespace
{
    template <const_qualified Type>
    constexpr int const_rank()
    {
        return 1;
    }

    template <cv_qualified Type>
    constexpr int const_rank()
    {
        return 2;
    }

    template <volatile_qualified Type>
    constexpr int volatile_rank()
    {
        return 1;
    }

    template <cv_qualified Type>
    constexpr int volatile_rank()
    {
        return 2;
    }
} // namespace

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

TEST(Qualifier, cv_qualified_subsumes_const_qualified)
{
    STATIC_EXPECT_EQ(const_rank<int const volatile>(), 2);
    STATIC_EXPECT_EQ(const_rank<int const>(), 1);
}

TEST(Qualifier, cv_qualified_subsumes_volatile_qualified)
{
    STATIC_EXPECT_EQ(volatile_rank<int const volatile>(), 2);
    STATIC_EXPECT_EQ(volatile_rank<int volatile>(), 1);
}
