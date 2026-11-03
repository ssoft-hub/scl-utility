#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - ASSIGNMENT
// ============================================================================

// ----------------------------------------------------------------------------
// ASSIGNMENT (=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Assign)
{
    STATIC_EXPECT_FALSE((::scl::has_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, AssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, AssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// PLUS ASSIGN (+=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PlusAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MINUS ASSIGN (-=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, MinusAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MULTIPLY ASSIGN (*=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, MultipliesAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultipliesAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultipliesAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiplies_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiplies_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// DIVIDE ASSIGN (/=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, DividesAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DividesAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DividesAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divides_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divides_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MODULO ASSIGN (%=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ModulusAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModulusAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModulusAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulus_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulus_assign_member_exact_v<NoOperators const &&, int>));
}
