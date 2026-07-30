#include <gtest_utils.h>

#include <meta/type_key_shared_type.h>

#include <scl/utility/meta/type_key.h>

/**
 * @brief Same-named TU-local type as in type_key_cross_tu_provider_gtest.cpp:
 *        a distinct type, expected to produce a different key.
 */
namespace
{
    struct CrossTuDuck
    {};
} // namespace

::scl::type_key const & cross_tu_provider_duck_key() noexcept;
::scl::type_key const & cross_tu_provider_goose_key() noexcept;

/**
 * @test Verify same-named anonymous-namespace types from two translation
 *       units produce distinct key objects with unequal content despite
 *       identical name strings.
 */
TEST(TypeKeyCrossTuTest, SameNamedTuLocalTypesDiffer)
{
    auto const & local_key = ::scl::type_key_of<CrossTuDuck>();
    auto const & provider_key = cross_tu_provider_duck_key();

    EXPECT_EQ(local_key.name(), provider_key.name());
    EXPECT_NE(&local_key, &provider_key);
    EXPECT_NE(local_key, provider_key);
}

/**
 * @test Verify a shared named type resolves to one key object within the
 *       module (merged inline variable) and compares equal.
 */
TEST(TypeKeyCrossTuTest, SharedExternalTypeMatches)
{
    auto const & local_key = ::scl::type_key_of<TypeKeyShared::Goose>();
    auto const & provider_key = cross_tu_provider_goose_key();

    EXPECT_EQ(&local_key, &provider_key);
    EXPECT_EQ(local_key, provider_key);
}
