#include <gtest_utils.h>

#include <scl/utility/attribute/maybe_unused.h>

#include "attribute_strict_warnings.h"

namespace
{
    SCL_MAYBE_UNUSED int never_called() { return 0; }

    int first(int kept, SCL_MAYBE_UNUSED int unread) { return kept; }

    // No compiler of the basis warns about an unused type.
    struct SCL_MAYBE_UNUSED never_used
    {};

    enum class SCL_MAYBE_UNUSED never_named
    {
        value
    };
} // namespace

TEST(AttributeMaybeUnused, LeavesAnUnusedEntityWithNoWarning)
{
    SCL_MAYBE_UNUSED int const unread = 1;
    EXPECT_EQ(first(2, 3), 2);
}
