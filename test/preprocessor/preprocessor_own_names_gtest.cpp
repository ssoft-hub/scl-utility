#include <gtest_utils.h>

// Each namespace declared here before the group is included takes the name of a namespace
// the headers use, so a name the headers write below it without the leading :: finds this
// namespace and breaks the build of this unit: std:: and scl:: anywhere under scl, detail::
// inside scl::detail and scl::preprocessor::detail, and preprocessor:: inside scl::preprocessor
// and scl::preprocessor::detail.
namespace scl
{
    namespace std
    {}

    namespace scl
    {}
} // namespace scl

namespace scl::detail
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}
} // namespace scl::detail

namespace scl::preprocessor
{
    namespace std
    {}

    namespace scl
    {}

    namespace preprocessor
    {}
} // namespace scl::preprocessor

namespace scl::preprocessor::detail
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace preprocessor
    {}
} // namespace scl::preprocessor::detail

// A namespace named scl, nominated at global scope, makes a name written as scl:: outside
// the namespace scl ambiguous.
namespace own_names
{
    namespace scl
    {}
} // namespace own_names

using namespace own_names;

#include <scl/utility/preprocessor.h>

namespace
{
    constexpr int add(int left, int right) noexcept { return left + right; }
} // namespace

struct OwnNamesTag
{};

SCL_COUNTER_NEXT(OwnNamesTag)
SCL_COUNTER_NEXT(OwnNamesTag)

/**
 * @test Verify that SCL_COUNTER_NEXT and SCL_COUNTER_VALUE count beside the caller's own names.
 */
TEST(PreprocessorOwnNamesTest, Counter) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(OwnNamesTag), 2U); }

/**
 * @test Verify that SCL_FORWARD passes its arguments beside the caller's own names.
 */
TEST(PreprocessorOwnNamesTest, Forward) { STATIC_EXPECT_EQ(add(SCL_FORWARD(1, 2)), 3); }
