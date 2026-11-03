#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - BITWISE (SHIFT, AND, OR, XOR)
// ============================================================================

// ----------------------------------------------------------------------------
// BINARY LEFT SHIFT (<<)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LeftShift)
{
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftMember)
{
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY RIGHT SHIFT (>>)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, RightShift)
{
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, RightShiftMember)
{
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, RightShiftMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE AND (&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitAnd)
{
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitAndMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitAndMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_and_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_and_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE OR (|)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitOr)
{
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitOrMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitOrMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_or_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_or_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE XOR (^)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitXor)
{
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitXorMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitXorMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bit_xor_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bit_xor_member_exact_v<NoOperators const &&, int>));
}
