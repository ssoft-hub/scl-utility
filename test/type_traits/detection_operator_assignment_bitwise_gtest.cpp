#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - BITWISE ASSIGNMENT (SHIFT, AND, OR, XOR)
// ============================================================================

// ----------------------------------------------------------------------------
// LEFT SHIFT ASSIGN (<<=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LeftShiftAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// RIGHT SHIFT ASSIGN (>>=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, RightShiftAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, RightShiftAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, RightShiftAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BITWISE AND ASSIGN (&=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseAndAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BITWISE OR ASSIGN (|=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseOrAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BITWISE XOR ASSIGN (^=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseXorAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators const &&, int>));
}
