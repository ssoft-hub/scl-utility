#include <gtest_utils.h>

#include <any/any_view_shared_type.h>

#include <scl/utility/any/any_view.h>

/**
 * @brief Same-named TU-local type as in any_view_cross_tu_provider_gtest.cpp:
 *        a distinct type, so a view over that one must not cast to this one.
 */
namespace
{
    struct AnyCrossTuDuck
    {
        int id = 0;
    };
} // namespace

::scl::any_view any_view_cross_tu_provider_duck() noexcept;
::scl::any_view any_view_cross_tu_provider_goose() noexcept;

/**
 * @test Verify a view over a TU-local type refuses an any_cast to the same-named
 *       TU-local type of another translation unit. Both render the same
 *       type_name, so only the key's per-TU discriminator can tell them apart.
 */
TEST(AnyViewCrossTuTest, SameNamedTuLocalTypesDoNotMatch)
{
    ::scl::any_view const view = any_view_cross_tu_provider_duck();

    EXPECT_EQ(view.type_name(), ::scl::type_name<AnyCrossTuDuck>());
    EXPECT_NE(view.type_key(), &::scl::type_key_of<AnyCrossTuDuck>());
    EXPECT_EQ(::scl::any_cast<AnyCrossTuDuck>(&view), nullptr);
}

/**
 * @test Verify an external type still matches across translation units — the
 *       discriminator is null on both sides, so identity is the name alone.
 */
TEST(AnyViewCrossTuTest, SharedExternalTypeMatches)
{
    ::scl::any_view const view = any_view_cross_tu_provider_goose();

    EXPECT_EQ(view.type_key(), &::scl::type_key_of<AnyViewShared::Goose>());
    auto const * goose = ::scl::any_cast<AnyViewShared::Goose>(&view);
    ASSERT_NE(goose, nullptr);
    EXPECT_EQ(goose->id, 9);
}
