#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - COMPARISON
// ============================================================================

// ----------------------------------------------------------------------------
// BINARY EQUAL TO (==)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, EqualTo)
{
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, EqualToMember)
{
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, EqualToMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY NOT EQUAL (!=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, NotEqualTo)
{
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, NotEqualToMember)
{
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, NotEqualToMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LESS (<)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Less)
{
    STATIC_EXPECT_TRUE((::scl::has_less_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessMember)
{
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LESS EQUAL (<=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LessEqual)
{
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessEqualMember)
{
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessEqualMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY GREATER (>)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Greater)
{
    STATIC_EXPECT_TRUE((::scl::has_greater_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterMember)
{
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY GREATER EQUAL (>=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, GreaterEqual)
{
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterEqualMember)
{
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterEqualMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators const &&, int>));
}
