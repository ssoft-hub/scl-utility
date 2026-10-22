#include <gtest_utils.h>

#include <scl/utility/attribute/no_unique_address.h>

#include "attribute_strict_warnings.h"

namespace
{
    struct empty
    {};

    struct only_int
    {
        int value;
    };

    struct one_empty
    {
        SCL_NO_UNIQUE_ADDRESS
        empty policy;
        int value;
    };

    struct two_empty
    {
        SCL_NO_UNIQUE_ADDRESS
        empty first;
        SCL_NO_UNIQUE_ADDRESS
        empty second;
        int value;
    };
} // namespace

TEST(AttributeNoUniqueAddress, GivesAnEmptyMemberNoStorageOfItsOwn)
{
    STATIC_EXPECT_TRUE(sizeof(one_empty) == sizeof(only_int));
}

TEST(AttributeNoUniqueAddress, KeepsTwoMembersOfOneTypeAtDistinctAddresses)
{
    two_empty const object{};
    EXPECT_NE(static_cast<void const *>(&object.first), static_cast<void const *>(&object.second));
}
