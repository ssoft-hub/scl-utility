#include <gtest_utils.h>

// The group compiles beside names a caller declares in scl::detail and scl::meta before
// including it.
namespace scl::detail
{
    struct own_type
    {};

    namespace concepts
    {}
} // namespace scl::detail

namespace scl::meta
{
    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::meta

#include <scl/utility/runtime.h>

namespace
{
    enum class OwnNamesColor : int
    {
        Red = 1,
    };
} // namespace

/**
 * @test Verify that enum_string compiles and answers beside the caller's own names.
 */
TEST(RuntimeOwnNamesTest, EnumString)
{
    EXPECT_EQ(::scl::enum_string(OwnNamesColor::Red), "OwnNamesColor::1");
}

#if SCL_HAS_RTTI

/**
 * @test Verify that type_name and type_short_name compile and answer beside the caller's own names.
 */
TEST(RuntimeOwnNamesTest, TypeName)
{
    ::scl::detail::own_type const object{};
    EXPECT_EQ(::scl::type_short_name(object), "own_type");
}

#endif // SCL_HAS_RTTI
