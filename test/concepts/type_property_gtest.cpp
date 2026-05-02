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

TEST(TypeProperty, trivial)
{
    STATIC_EXPECT_TRUE(trivial<int>);
    STATIC_EXPECT_TRUE(trivial<Trivial>);
    STATIC_EXPECT_FALSE(trivial<NonTrivial>);
    STATIC_EXPECT_FALSE(trivial<PolymorphicBase>);
}

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

TEST(TypeProperty, empty_type)
{
    STATIC_EXPECT_TRUE(empty_type<Empty>);
    STATIC_EXPECT_FALSE(empty_type<int>);
    STATIC_EXPECT_FALSE(empty_type<Trivial>);
}

TEST(TypeProperty, polymorphic)
{
    STATIC_EXPECT_TRUE(polymorphic<PolymorphicBase>);
    STATIC_EXPECT_FALSE(polymorphic<int>);
    STATIC_EXPECT_FALSE(polymorphic<Trivial>);
}

TEST(TypeProperty, abstract_type)
{
    STATIC_EXPECT_TRUE(abstract_type<Abstract>);
    STATIC_EXPECT_FALSE(abstract_type<PolymorphicBase>);
    STATIC_EXPECT_FALSE(abstract_type<int>);
}

TEST(TypeProperty, final_type)
{
    STATIC_EXPECT_TRUE(final_type<Final>);
    STATIC_EXPECT_FALSE(final_type<int>);
    STATIC_EXPECT_FALSE(final_type<Trivial>);
}

TEST(TypeProperty, aggregate)
{
    STATIC_EXPECT_TRUE(aggregate<Trivial>);
    STATIC_EXPECT_TRUE(aggregate<AggregateUnion>);
    STATIC_EXPECT_FALSE(aggregate<PolymorphicBase>);
    STATIC_EXPECT_FALSE(aggregate<Abstract>);
}
