#include <gtest_utils.h>

#include <scl/utility/attribute/nodiscard.h>

#include "attribute_strict_warnings.h"

namespace
{
    SCL_NODISCARD int result() { return 1; }

    SCL_NODISCARD_MSG("the status tells whether the call succeeded") int status() { return 2; }

    struct SCL_NODISCARD code
    {
        int value;
    };

    code make_code() { return code{3}; }

    enum class SCL_NODISCARD outcome
    {
        done
    };

    outcome finish() { return outcome::done; }
} // namespace

TEST(AttributeNodiscard, DrawsNoWarningWhereTheCallerReadsTheResult)
{
    int const first = result();
    int const second = status();
    code const third = make_code();
    outcome const fourth = finish();
    EXPECT_EQ(first + second + third.value, 6);
    EXPECT_EQ(fourth, outcome::done);
}
