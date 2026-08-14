#include <gtest_utils.h>

#include <any/any_view_shared_type.h>

#include <scl/utility/any/any.h>

/**
 * @brief Same-named TU-local type as in any_cross_tu_provider_gtest.cpp: a
 *        distinct type, so an any holding that one must not cast to this one.
 */
namespace
{
    struct AnyCrossTuDuck
    {
        int id = 0;
    };
} // namespace

::scl::any any_cross_tu_provider_duck();
::scl::any any_cross_tu_provider_goose();

/**
 * @test Verify an any holding a TU-local type refuses an any_cast to the
 *       same-named TU-local type of another translation unit. Both render the
 *       same type_name, so only the key's per-TU discriminator can tell them
 *       apart.
 */
TEST(AnyCrossTuTest, SameNamedTuLocalTypesDoNotMatch)
{
    ::scl::any const value = any_cross_tu_provider_duck();

    EXPECT_EQ(value.type_name(), ::scl::type_name<AnyCrossTuDuck>());
    EXPECT_FALSE(value.type_key() == ::scl::type_key_of<AnyCrossTuDuck>());
    EXPECT_EQ(::scl::any_cast<AnyCrossTuDuck>(&value), nullptr);
}

/**
 * @test Verify an external type still matches across translation units — the
 *       discriminator is null on both sides, so identity is the name alone.
 */
TEST(AnyCrossTuTest, SharedExternalTypeMatches)
{
    ::scl::any const value = any_cross_tu_provider_goose();

    EXPECT_TRUE(value.type_key() == ::scl::type_key_of<AnyViewShared::Goose>());
    auto const * goose = ::scl::any_cast<AnyViewShared::Goose>(&value);
    ASSERT_NE(goose, nullptr);
    EXPECT_EQ(goose->id, 9);
}
