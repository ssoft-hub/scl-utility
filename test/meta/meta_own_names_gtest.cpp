#include <gtest_utils.h>

// Each namespace declared here before the group is included takes the name of a namespace
// the headers use, so a name the headers write below it without the leading :: finds this
// namespace and breaks the build of this unit: std:: and scl:: anywhere under scl, detail::
// inside scl::detail and scl::concepts, and concepts:: inside scl::concepts and the namespaces
// scl::detail nests.
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

    // A class named as one of the interface, so a name the headers write in scl::detail without
    // scl:: finds this class.
    struct type_key
    {};

    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}

    namespace
    {
        namespace std
        {}

        namespace scl
        {}

        namespace detail
        {}

        namespace concepts
        {}
    } // namespace
} // namespace scl::detail

namespace scl::detail::concepts
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::detail::concepts

namespace scl::concepts
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::concepts

// A namespace named scl, nominated at global scope, makes a name written as scl:: outside
// the namespace scl ambiguous.
namespace own_names
{
    namespace scl
    {}
} // namespace own_names

using namespace own_names;

#include <scl/utility/meta.h>

namespace own_names_types
{
    enum class Shade : int
    {
        Dark = 1,
    };

    struct Probe
    {};

    inline void probe_function() {}
} // namespace own_names_types

namespace
{
    struct OwnNamesHidden
    {};
} // namespace

/**
 * @test Verify that type_name and type_short_name answer in constant evaluation beside
 *       the caller's own names.
 */
TEST(MetaOwnNamesTest, TypeName)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<::scl::detail::own_type>(), "own_type");
    STATIC_EXPECT_TRUE(::scl::type_name<::own_names_types::Probe>().ends_with("own_names_types::Probe"));
    STATIC_EXPECT_EQ(::scl::type_short_name<::own_names_types::Probe>(), "Probe");
}

/**
 * @test Verify that enum_name and enum_short_name answer in constant evaluation beside
 *       the caller's own names.
 */
TEST(MetaOwnNamesTest, EnumName)
{
    STATIC_EXPECT_TRUE(::scl::enum_name<::own_names_types::Shade::Dark>().ends_with("Shade::Dark"));
    STATIC_EXPECT_EQ(::scl::enum_short_name<::own_names_types::Shade::Dark>(), "Dark");
}

/**
 * @test Verify that symbol_name and symbol_short_name answer in constant evaluation beside
 *       the caller's own names.
 */
TEST(MetaOwnNamesTest, SymbolName)
{
    STATIC_EXPECT_TRUE(::scl::symbol_name<&::own_names_types::probe_function>().ends_with("probe_function"));
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&::own_names_types::probe_function>(), "probe_function");
}

/**
 * @test Verify that type_key_of and is_tu_local answer in constant evaluation beside the caller's
 *       own names.
 */
TEST(MetaOwnNamesTest, TypeKey)
{
    STATIC_EXPECT_EQ(
        ::scl::type_key_of<::own_names_types::Probe>().name(),
        ::scl::type_name<::own_names_types::Probe>());
    STATIC_EXPECT_NE(::scl::type_key_of<::own_names_types::Probe>(), ::scl::type_key_of<int>());
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<::own_names_types::Probe>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<OwnNamesHidden>);
}
