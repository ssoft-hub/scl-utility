// One include of <scl/utility/any.h> here makes every assertion below pass for the wrong reason.

#include <gtest_utils.h>

#include <scl/utility/any/any_arg.h>
#include <scl/utility/any/any_switch.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <type_traits>

namespace
{
    template <typename Type, typename = void>
    struct is_defined : ::std::false_type
    {};

    template <typename Type>
    struct is_defined<Type, ::std::void_t<decltype(sizeof(Type))>> : ::std::true_type
    {};

    template <typename Type>
    constexpr bool is_defined_v = is_defined<Type>::value;
} // namespace

TEST(AnyLayeringTest, AnArgumentDoesNotDefineTheViews)
{
    // An argument adopts a view's binding through a template, so a declaration is enough.
    STATIC_EXPECT_TRUE(is_defined_v<::scl::any_argument>);
    STATIC_EXPECT_FALSE(is_defined_v<::scl::any_view>);
    STATIC_EXPECT_FALSE(is_defined_v<::scl::any_mutable_view>);
}

TEST(AnyLayeringTest, AChainDefinesOnlyItsSubject)
{
    // The chain reads its subject as an argument, so the views are no part of what it needs.
    STATIC_EXPECT_TRUE(is_defined_v<::scl::any_switch<>>);
    STATIC_EXPECT_TRUE(is_defined_v<::scl::any_argument>);
    STATIC_EXPECT_FALSE(is_defined_v<::scl::any_view>);
}

#if SCL_HAS_RTTI
TEST(AnyLayeringTest, NothingHereSpecialisesTheTraitForABox)
{
    // Asked through a TU-local trait: asking through a cast would poison every later question.
    STATIC_EXPECT_FALSE(is_defined_v<::scl::any_cast_traits<::std::any>>);
    STATIC_EXPECT_TRUE(is_defined_v<::scl::any_cast_traits<::scl::any_argument>>);
}
#endif

TEST(AnyLayeringTest, AChainStillSelectsOnAPlainValue)
{
    int chosen = 0;

    ::scl::any_switch<>().in_case<int const &>([&chosen](int const &) {
        chosen = 1;
    }).or_else([&chosen]() {
        chosen = 2;
    }).apply(42);

    EXPECT_EQ(chosen, 1);
}
