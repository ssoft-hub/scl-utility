#include <gtest_utils.h>

#include <meta/type_key_shared_type.h>

#include <scl/utility/meta/type_key.h>

#include <vector>

/**
 * @brief Same-named TU-local types as in type_key_cross_tu_provider_gtest.cpp:
 *        distinct types, expected to produce different keys.
 */
namespace
{
    struct CrossTuDuck
    {};

    struct CrossTuPointing
    {
        CrossTuPointing const * operator->() const { return this; }

        int operator-(CrossTuPointing) const { return 0; }
    };

    using cross_tu_arrow = TypeKeyShared::Wrap<TypeKeyShared::Holder<&CrossTuPointing::operator- >>;

    constexpr ::scl::type_key arrow_copy = ::scl::type_key_of<TypeKeyShared::arrow_type>();
} // namespace

::scl::type_key const & cross_tu_provider_duck_key() noexcept;
::scl::type_key const & cross_tu_provider_goose_key() noexcept;
::scl::type_key const & cross_tu_provider_arrow_copy() noexcept;
::scl::type_key const & cross_tu_provider_arrow_key() noexcept;
::scl::type_key const & cross_tu_provider_nested_duck_key() noexcept;

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

/**
 * @test Verify that copies of the key of a shared type whose name holds `operator->`, each made in
 *       its own translation unit, compare equal.
 */
TEST(TypeKeyCrossTuTest, CopiesOfKeyOfOneSpellingMatch)
{
    EXPECT_EQ(arrow_copy, cross_tu_provider_arrow_copy());
}

/**
 * @test Verify same-named anonymous-namespace types whose name holds `operator->` with GCC and
 *       Clang, one from each of two translation units, produce unequal keys.
 */
TEST(TypeKeyCrossTuTest, SameNamedTuLocalTypesOfOneSpellingDiffer)
{
    auto const & local_key = ::scl::type_key_of<cross_tu_arrow>();
    auto const & provider_key = cross_tu_provider_arrow_key();

#if !defined(_MSC_VER) || defined(__clang__)
    // MSVC spells an unnamed namespace in a pointer to a member with a hash of its translation unit.
    EXPECT_EQ(local_key.name(), provider_key.name());
#endif
    EXPECT_NE(local_key, provider_key);
}

/**
 * @test Verify a TU-local type reached only through a template argument still
 *       separates the keys of two translation units, where a compiler renders
 *       the component without its anonymous-namespace qualification.
 */
TEST(TypeKeyCrossTuTest, NestedTuLocalTypesDiffer)
{
    auto const & local_key = ::scl::type_key_of<::std::vector<int CrossTuDuck::*>>();
    auto const & provider_key = cross_tu_provider_nested_duck_key();

    EXPECT_EQ(local_key.name(), provider_key.name());
    EXPECT_NE(local_key, provider_key);
}
