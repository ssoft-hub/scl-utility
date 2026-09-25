#include <gtest_utils.h>

// Each namespace declared here before the group is included takes the name of a namespace the
// headers use, so a name the headers write below it without the leading :: finds this namespace
// and breaks the build of this unit: std:: and scl:: anywhere under scl, and detail::, concepts::
// and meta:: inside scl::detail.
namespace scl
{
    namespace std
    {}

    namespace scl
    {}
} // namespace scl

namespace scl::detail
{
    struct own_type
    {};

    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}

    namespace meta
    {}
} // namespace scl::detail

namespace scl::meta
{
    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::meta

// A namespace named scl, nominated at global scope, makes a name written as scl:: outside the
// namespace scl ambiguous.
namespace own_names
{
    namespace scl
    {}
} // namespace own_names

using namespace own_names;

#include <scl/utility/runtime.h>

#include <string>

// Types of the caller's own with functions named as those of the group, taking the types by an
// exact match, so a call the headers would write with no qualifier finds them by argument.
namespace own_names_adl
{
    enum class Shade : int
    {
        Dark = 1,
    };

    struct Probe
    {};

    template <typename Format>
    ::std::string enum_string(Shade, Format &&)
    {
        return "caller's";
    }

    inline ::std::string type_name(Probe const &) { return "caller's"; }

    inline ::std::string type_short_name(Probe const &) { return "caller's"; }
} // namespace own_names_adl

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

/**
 * @test Verify that enum_string answers the same beside a function of the caller named as the
 *       group's.
 */
TEST(RuntimeOwnNamesTest, EnumStringIgnoresCallerFunctions)
{
    EXPECT_EQ(::scl::enum_string(::own_names_adl::Shade::Dark), "Shade::1");
}

#if SCL_HAS_RTTI

/**
 * @test Verify that type_name and type_short_name compile and answer beside the caller's own
 *       names.
 */
TEST(RuntimeOwnNamesTest, TypeName)
{
    ::scl::detail::own_type const object{};
    EXPECT_EQ(::scl::type_short_name(object), "own_type");
}

/**
 * @test Verify that type_name and type_short_name answer the same beside functions of the caller
 *       named as the group's.
 */
TEST(RuntimeOwnNamesTest, TypeNameIgnoresCallerFunctions)
{
    ::own_names_adl::Probe const object{};
    EXPECT_TRUE(::scl::type_name(object).ends_with("own_names_adl::Probe"));
    EXPECT_EQ(::scl::type_short_name(object), "Probe");
}

#endif // SCL_HAS_RTTI
