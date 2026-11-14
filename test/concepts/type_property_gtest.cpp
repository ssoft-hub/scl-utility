#include <gtest_utils.h>

#include <scl/utility/concepts/type_property.h>

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
