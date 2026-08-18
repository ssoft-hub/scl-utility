#include <gtest_utils.h>

#include <type_traits>
#include <vector>

#include <scl/utility/meta/type_key.h>

/**
 * @brief TU-local type: its key must carry a per-TU discriminator.
 */
namespace
{
    struct KeyAnonDuck
    {};
} // namespace

/**
 * @brief External-linkage type at global namespace scope.
 */
struct KeyGlobalDuck
{};

/**
 * @brief External-linkage type inside a named namespace.
 */
namespace TypeKeyNs
{
    struct Duck
    {};

} // namespace TypeKeyNs

/**
 * @test Verify a key is an ordinary value: default-built, copied, moved and
 *       assigned, so a handle can answer one without handing out a pointer.
 */
TEST(TypeKeyTest, TypeProperties)
{
    STATIC_EXPECT_TRUE(::std::is_default_constructible_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_copy_constructible_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_move_constructible_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_copy_assignable_v<::scl::type_key>);
    STATIC_EXPECT_TRUE(::std::is_move_assignable_v<::scl::type_key>);
}

/**
 * @test Verify the empty key names no type and matches a default-built one.
 */
TEST(TypeKeyTest, EmptyKeyNamesNothing)
{
    STATIC_EXPECT_TRUE(::scl::type_key{} == ::scl::type_key{});
    STATIC_EXPECT_TRUE(::scl::type_key{}.name().empty());
    STATIC_EXPECT_FALSE(::scl::type_key_of<int>() == ::scl::type_key{});
}

/**
 * @test Verify a copy of a key keeps naming the same type as its source.
 */
TEST(TypeKeyTest, ACopyNamesTheSameType)
{
    ::scl::type_key copy = ::scl::type_key_of<int>();

    EXPECT_TRUE(copy == ::scl::type_key_of<int>());
    EXPECT_FALSE(copy == ::scl::type_key_of<double>());

    copy = ::scl::type_key_of<double>();
    EXPECT_TRUE(copy == ::scl::type_key_of<double>());
}

/**
 * @test Verify the name accessor exposes the type_name rendering.
 */
TEST(TypeKeyTest, Name)
{
    STATIC_EXPECT_EQ(::scl::type_key_of<KeyGlobalDuck>().name(), ::scl::type_name<KeyGlobalDuck>());
    STATIC_EXPECT_EQ(::scl::type_key_of<KeyAnonDuck>().name(), ::scl::type_name<KeyAnonDuck>());
}

/**
 * @test Verify type_key_of returns a reference to one per-type object.
 */
TEST(TypeKeyTest, PerTypeAddressIdentity)
{
    STATIC_EXPECT_TRUE(&::scl::type_key_of<int>() == &::scl::type_key_of<int>());
    STATIC_EXPECT_TRUE(&::scl::type_key_of<KeyAnonDuck>() == &::scl::type_key_of<KeyAnonDuck>());
    STATIC_EXPECT_TRUE(&::scl::type_key_of<int>() != &::scl::type_key_of<long>());
}

/**
 * @test Verify key equality and inequality in constant expressions.
 */
TEST(TypeKeyTest, Equality)
{
    STATIC_EXPECT_EQ(::scl::type_key_of<int>(), ::scl::type_key_of<int>());
    STATIC_EXPECT_NE(::scl::type_key_of<int>(), ::scl::type_key_of<long>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyGlobalDuck>(), ::scl::type_key_of<TypeKeyNs::Duck>());

    STATIC_EXPECT_EQ(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<KeyAnonDuck>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<KeyGlobalDuck>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<KeyAnonDuck *>());
    STATIC_EXPECT_NE(::scl::type_key_of<KeyAnonDuck>(), ::scl::type_key_of<::std::vector<KeyAnonDuck>>());
}
