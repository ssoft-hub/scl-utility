#include <gtest_utils.h>

#include <scl/utility/attribute/inline.h>
#include <scl/utility/attribute/noreturn.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <cstdlib>

#include "attribute_strict_warnings.h"
#include "noreturn_function.h"

namespace
{
    SCL_NORETURN SCL_NOINLINE
    void fail_out_of_line()
    {
        std::abort();
    }

    SCL_NORETURN SCL_FORCE_INLINE
    void fail_in_line()
    {
        std::abort();
    }

    int checked(int value)
    {
        if (value >= 0)
            return value;
        stop_elsewhere();
    }

    int checked_by_inline_macros(int value)
    {
        if (value >= 0)
            return value;
        if (value == -1)
            fail_out_of_line();
        fail_in_line();
    }

#if SCL_HAS_EXCEPTIONS
    SCL_NORETURN
    void throw_code(int code)
    {
        throw code;
    }
#endif
} // namespace

TEST(AttributeNoReturn, EndsAFunctionThatReturnsAValueWithNoWarning) { EXPECT_EQ(checked(4), 4); }

TEST(AttributeNoReturn, StandsBeforeTheInlineMacrosWithNoWarning)
{
    EXPECT_EQ(checked_by_inline_macros(4), 4);
}

#if SCL_HAS_EXCEPTIONS
TEST(AttributeNoReturn, LetsAFunctionLeaveByAnException) { EXPECT_THROW(throw_code(7), int); }
#endif
