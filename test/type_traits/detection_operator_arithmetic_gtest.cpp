#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

#include "detection_operator_helpers.h"

using namespace test;

// ============================================================================
// TESTS - ARITHMETIC
// ============================================================================

// ----------------------------------------------------------------------------
// BINARY PLUS (+)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Plus)
{
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusMember)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MINUS (-)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Minus)
{
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusMember)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MULTIPLY (*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Multiply)
{
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyMember)
{
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY DIVIDE (/)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Divide)
{
    STATIC_EXPECT_TRUE((::scl::has_divide_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideMember)
{
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MODULO (%)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Modulo)
{
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloMember)
{
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators const &&, int>));
}
