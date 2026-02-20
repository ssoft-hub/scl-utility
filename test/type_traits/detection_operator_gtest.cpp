#include <gtest/gtest.h>

#include <scl/utility/type_traits/detection/operator.h>

// ============================================================================
// MACROS
// ============================================================================

#define TEST_EXPECT_TRUE(X) \
    static_assert(X, #X);   \
    EXPECT_TRUE(X);

#define TEST_EXPECT_FALSE(X) \
    static_assert(!(X), #X);   \
    EXPECT_FALSE(X);

namespace test
{
    // ========================================================================
    // HELPER CLASSES (Declarations Only)
    // ========================================================================

    struct WithMember
    {
        int * operator&();
        int operator*();
        int operator+();
        int operator-();
        WithMember & operator++();
        WithMember & operator--();
        int operator~();
        bool operator!();

        WithMember operator++(int);
        WithMember operator--(int);

        int operator+(int);
        int operator-(int);
        int operator*(int);
        int operator/(int);
        int operator%(int);

        bool operator==(int);
        bool operator!=(int);
        bool operator<(int);
        bool operator>(int);
        bool operator<=(int);
        bool operator>=(int);

        int operator<<(int);
        int operator>>(int);
        int operator&(int);
        int operator|(int);
        int operator^(int);

        bool operator&&(int);
        bool operator||(int);

        WithMember & operator=(int);
        WithMember & operator+=(int);
        WithMember & operator-=(int);
        WithMember & operator*=(int);
        WithMember & operator/=(int);
        WithMember & operator%=(int);
        WithMember & operator<<=(int);
        WithMember & operator>>=(int);
        WithMember & operator&=(int);
        WithMember & operator|=(int);
        WithMember & operator^=(int);

        int operator[](int);
        void operator()(int);
        int * operator->();
        int operator,(int);
        int operator->*(int);
    };

    struct WithConstMember
    {
        int * operator&() const;
        int operator*() const;
        int operator+() const;
        int operator-() const;
        WithConstMember & operator++() const;
        WithConstMember & operator--() const;
        int operator~() const;
        bool operator!() const;

        WithConstMember operator++(int) const;
        WithConstMember operator--(int) const;

        int operator+(int) const;
        int operator-(int) const;
        int operator*(int) const;
        int operator/(int) const;
        int operator%(int) const;

        bool operator==(int) const;
        bool operator!=(int) const;
        bool operator<(int) const;
        bool operator>(int) const;
        bool operator<=(int) const;
        bool operator>=(int) const;

        int operator<<(int) const;
        int operator>>(int) const;
        int operator&(int) const;
        int operator|(int) const;
        int operator^(int) const;

        bool operator&&(int) const;
        bool operator||(int) const;

        void operator=(int) const;
        void operator+=(int) const;
        void operator-=(int) const;
        void operator*=(int) const;
        void operator/=(int) const;
        void operator%=(int) const;
        void operator<<=(int) const;
        void operator>>=(int) const;
        void operator&=(int) const;
        void operator|=(int) const;
        void operator^=(int) const;

        int operator[](int) const;
        void operator()(int) const;
        int const * operator->() const;
        int operator,(int) const;
        int operator->*(int) const;
    };

    struct WithRvalueMember
    {
        int * operator&() &&;
        int operator*() &&;
        int operator+() &&;
        int operator-() &&;
        WithRvalueMember & operator++() &&;
        WithRvalueMember & operator--() &&;
        int operator~() &&;
        bool operator!() &&;

        WithRvalueMember operator++(int) &&;
        WithRvalueMember operator--(int) &&;

        int operator+(int) &&;
        int operator-(int) &&;
        int operator*(int) &&;
        int operator/(int) &&;
        int operator%(int) &&;

        bool operator==(int) &&;
        bool operator!=(int) &&;
        bool operator<(int) &&;
        bool operator>(int) &&;
        bool operator<=(int) &&;
        bool operator>=(int) &&;

        int operator<<(int) &&;
        int operator>>(int) &&;
        int operator&(int) &&;
        int operator|(int) &&;
        int operator^(int) &&;

        bool operator&&(int) &&;
        bool operator||(int) &&;

        void operator=(int) &&;
        void operator+=(int) &&;
        void operator-=(int) &&;
        void operator*=(int) &&;
        void operator/=(int) &&;
        void operator%=(int) &&;
        void operator<<=(int) &&;
        void operator>>=(int) &&;
        void operator&=(int) &&;
        void operator|=(int) &&;
        void operator^=(int) &&;

        int operator[](int) &&;
        void operator()(int) &&;
        int * operator->() &&;
        int operator,(int) &&;
        int operator->*(int) &&;
    };

    struct WithLvalueConstMember
    {
        int * operator&() const &;
        int operator*() const &;
        int operator+() const &;
        int operator-() const &;
        WithLvalueConstMember & operator++() const &;
        WithLvalueConstMember & operator--() const &;
        int operator~() const &;
        bool operator!() const &;

        WithLvalueConstMember operator++(int) const &;
        WithLvalueConstMember operator--(int) const &;

        int operator+(int) const &;
        int operator-(int) const &;
        int operator*(int) const &;
        int operator/(int) const &;
        int operator%(int) const &;

        bool operator==(int) const &;
        bool operator!=(int) const &;
        bool operator<(int) const &;
        bool operator>(int) const &;
        bool operator<=(int) const &;
        bool operator>=(int) const &;

        int operator<<(int) const &;
        int operator>>(int) const &;
        int operator&(int) const &;
        int operator|(int) const &;
        int operator^(int) const &;

        bool operator&&(int) const &;
        bool operator||(int) const &;

        void operator=(int) const &;
        void operator+=(int) const &;
        void operator-=(int) const &;
        void operator*=(int) const &;
        void operator/=(int) const &;
        void operator%=(int) const &;
        void operator<<=(int) const &;
        void operator>>=(int) const &;
        void operator&=(int) const &;
        void operator|=(int) const &;
        void operator^=(int) const &;

        int operator[](int) const &;
        void operator()(int) const &;
        int const * operator->() const &;
        int operator,(int) const &;
        int operator->*(int) const &;
    };

    struct WithGlobal
    {};

    // Forward declarations
    int * operator&(WithGlobal &);
    int operator*(WithGlobal);
    int operator+(WithGlobal);
    int operator-(WithGlobal);
    WithGlobal & operator++(WithGlobal &);
    WithGlobal & operator--(WithGlobal &);
    int operator~(WithGlobal);
    bool operator!(WithGlobal);
    WithGlobal operator++(WithGlobal &, int);
    WithGlobal operator--(WithGlobal &, int);
    int operator+(WithGlobal, int);
    int operator-(WithGlobal, int);
    int operator*(WithGlobal, int);
    int operator/(WithGlobal, int);
    int operator%(WithGlobal, int);
    bool operator==(WithGlobal, int);
    bool operator!=(WithGlobal, int);
    bool operator<(WithGlobal, int);
    bool operator>(WithGlobal, int);
    bool operator<=(WithGlobal, int);
    bool operator>=(WithGlobal, int);
    int operator<<(WithGlobal, int);
    int operator>>(WithGlobal, int);
    int operator&(WithGlobal, int);
    int operator|(WithGlobal, int);
    int operator^(WithGlobal, int);
    bool operator&&(WithGlobal, int);
    bool operator||(WithGlobal, int);
    int operator,(WithGlobal, int);
    int operator->*(WithGlobal, int);

    struct NoOperators
    {
        int * operator&() = delete;
        int operator*() = delete;
        int operator+() = delete;
        int operator-() = delete;
        void operator++() = delete;
        void operator--() = delete;
        int operator~() = delete;
        bool operator!() = delete;
        void operator++(int) = delete;
        void operator--(int) = delete;
        int operator+(int) = delete;
        int operator-(int) = delete;
        int operator*(int) = delete;
        int operator/(int) = delete;
        int operator%(int) = delete;
        bool operator==(int) = delete;
        bool operator!=(int) = delete;
        bool operator<(int) = delete;
        bool operator>(int) = delete;
        bool operator<=(int) = delete;
        bool operator>=(int) = delete;
        int operator<<(int) = delete;
        int operator>>(int) = delete;
        int operator&(int) = delete;
        int operator|(int) = delete;
        int operator^(int) = delete;
        bool operator&&(int) = delete;
        bool operator||(int) = delete;
        void operator=(int) = delete;
        void operator+=(int) = delete;
        void operator-=(int) = delete;
        void operator*=(int) = delete;
        void operator/=(int) = delete;
        void operator%=(int) = delete;
        void operator<<=(int) = delete;
        void operator>>=(int) = delete;
        void operator&=(int) = delete;
        void operator|=(int) = delete;
        void operator^=(int) = delete;
        int operator[](int) = delete;
        void operator()(int) = delete;
        int * operator->() = delete;
        int operator,(int) const = delete;
        int operator->*(int) = delete;
    };
} // namespace test

using namespace test;

// ============================================================================
// TESTS - UNARY
// ============================================================================

// ----------------------------------------------------------------------------
// ADDRESS OF (&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, AddressOf)
{
    TEST_EXPECT_FALSE(::scl::has_address_of_v<int>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<int const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithRvalueMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_v<NoOperators const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_v<NoOperators const &>); // built-in
    TEST_EXPECT_FALSE(::scl::has_address_of_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, AddressOfMember)
{
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, AddressOfMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// INDIRECTION (*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Indirection)
{
    TEST_EXPECT_TRUE(::scl::has_indirection_v<int *>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<int *&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<int *&&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<int * const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<int * const &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<int * const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal &&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, IndirectionMember)
{
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, IndirectionMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// UNARY PLUS (+)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, UnaryPlus)
{
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<int>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<int &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<int &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<int const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<int const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryPlusMember)
{
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryPlusMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// UNARY MINUS (-)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, UnaryMinus)
{
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<int>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<int &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<int &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<int const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<int const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryMinusMember)
{
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryMinusMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// PREFIX INCREMENT (++)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PrefixIncrement)
{
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<int>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixIncrementMember)
{
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixIncrementMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// PREFIX DECREMENT (--)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PrefixDecrement)
{
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<int>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixDecrementMember)
{
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixDecrementMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// BITWISE NOT (~)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseNot)
{
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<int>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<int &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<int &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<int const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<int const &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, BitwiseNotMember)
{
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, BitwiseNotMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// LOGICAL NOT (!)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalNot)
{
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<int>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<int &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<int &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<int const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<int const &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, LogicalNotMember)
{
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, LogicalNotMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// POSTFIX INCREMENT (++)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PostfixIncrement)
{
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<int>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixIncrementMember)
{
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixIncrementMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// POSTFIX DECREMENT (--)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PostfixDecrement)
{
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<int>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixDecrementMember)
{
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixDecrementMemberExact)
{
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const &&>);

    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember &&>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember &>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const>);
    TEST_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const &&>);

    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators &&>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const &>);
    TEST_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// BINARY PLUS (+)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Plus)
{
    TEST_EXPECT_TRUE((::scl::has_plus_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusMember)
{
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MINUS (-)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Minus)
{
    TEST_EXPECT_TRUE((::scl::has_minus_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusMember)
{
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MULTIPLY (*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Multiply)
{
    TEST_EXPECT_TRUE((::scl::has_multiply_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyMember)
{
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY DIVIDE (/)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Divide)
{
    TEST_EXPECT_TRUE((::scl::has_divide_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideMember)
{
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY MODULO (%)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Modulo)
{
    TEST_EXPECT_TRUE((::scl::has_modulo_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloMember)
{
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY EQUAL TO (==)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, EqualTo)
{
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, EqualToMember)
{
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, EqualToMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_equal_to_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY NOT EQUAL (!=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, NotEqualTo)
{
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, NotEqualToMember)
{
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, NotEqualToMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_not_equal_to_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LESS (<)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Less)
{
    TEST_EXPECT_TRUE((::scl::has_less_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessMember)
{
    TEST_EXPECT_FALSE((::scl::has_less_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LESS EQUAL (<=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LessEqual)
{
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessEqualMember)
{
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LessEqualMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_less_equal_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY GREATER (>)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Greater)
{
    TEST_EXPECT_TRUE((::scl::has_greater_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterMember)
{
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY GREATER EQUAL (>=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, GreaterEqual)
{
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterEqualMember)
{
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, GreaterEqualMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_greater_equal_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LEFT SHIFT (<<)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LeftShift)
{
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftMember)
{
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY RIGHT SHIFT (>>)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, RightShift)
{
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, RightShiftMember)
{
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, RightShiftMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE AND (&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseAnd)
{
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndMember)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE OR (|)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseOr)
{
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrMember)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE XOR (^)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseXor)
{
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorMember)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LOGICAL AND (&&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalAnd)
{
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalAndMember)
{
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalAndMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LOGICAL OR (||)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalOr)
{
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<int &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<int &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<int const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalOrMember)
{
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalOrMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// ASSIGNMENT (=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Assign)
{
    TEST_EXPECT_FALSE((::scl::has_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, AssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, AssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_assign_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// PLUS ASSIGN (+=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PlusAssign)
{
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, PlusAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_plus_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MINUS ASSIGN (-=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, MinusAssign)
{
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MinusAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_minus_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MULTIPLY ASSIGN (*=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, MultiplyAssign)
{
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// DIVIDE ASSIGN (/=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, DivideAssign)
{
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MODULO ASSIGN (%=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ModuloAssign)
{
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// LEFT SHIFT ASSIGN (<<=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LeftShiftAssign)
{
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_v<WithLvalueConstMember const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<WithGlobal const &&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LeftShiftAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<int const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember const &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithConstMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithRvalueMember &&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember const &, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithRvalueMember const &&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember &, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember &&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, LeftShiftAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_left_shift_assign_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// RIGHT SHIFT ASSIGN (>>=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, RightShiftAssign)
{
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, RightShiftAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, RightShiftAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_right_shift_assign_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// BITWISE AND ASSIGN (&=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseAndAssign)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_and_assign_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// BITWISE OR ASSIGN (|=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseOrAssign)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_or_assign_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// BITWISE XOR ASSIGN (^=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseXorAssign)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorAssignMember)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorAssignMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_bitwise_xor_assign_member_exact_v<NoOperators const&&, int>));
}

// ============================================================================
// TESTS - SPECIAL
// ============================================================================

// ----------------------------------------------------------------------------
// SUBSCRIPT ([])
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Subscript)
{
    TEST_EXPECT_FALSE((::scl::has_subscript_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, SubscriptMember)
{
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, SubscriptMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// FUNCTION CALL (())
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Call)
{
    TEST_EXPECT_FALSE((::scl::has_call_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, CallMember)
{
    TEST_EXPECT_FALSE((::scl::has_call_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, CallMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_call_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_call_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// COMMA (,)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Comma)
{
    TEST_EXPECT_TRUE((::scl::has_comma_v<int, int>)); // built-in
    TEST_EXPECT_TRUE((::scl::has_comma_v<int&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<int&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<int const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<int const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_v<WithGlobal, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithGlobal&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithGlobal&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, CommaMember)
{
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, CommaMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const&&, int>));
}

// ----------------------------------------------------------------------------
// ARROW (->)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ArrowMember)
{
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<int*>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<int*&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<int*&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<int const*>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<int const*&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<int const*&&>));

    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithMember>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithMember&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithMember&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithMember const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithMember const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithMember const&&>));

    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithConstMember>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithConstMember&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithConstMember&&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithConstMember const>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithConstMember const&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithConstMember const&&>));

    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithRvalueMember>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithRvalueMember&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithRvalueMember&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithRvalueMember const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithRvalueMember const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithRvalueMember const&&>));

    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithLvalueConstMember>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithLvalueConstMember&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithLvalueConstMember&&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithLvalueConstMember const>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithLvalueConstMember const&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_v<WithLvalueConstMember const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithGlobal>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithGlobal&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithGlobal&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithGlobal const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithGlobal const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<WithGlobal const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<NoOperators>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<NoOperators&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<NoOperators&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<NoOperators const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<NoOperators const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_v<NoOperators const&&>));
}

TEST(OperatorDetectionTest, ArrowMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<int&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<int&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<int const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<int const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<int const&&>));

    TEST_EXPECT_TRUE((::scl::has_arrow_member_exact_v<WithMember>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithMember&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithMember&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithMember const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithMember const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithMember const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithConstMember>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithConstMember&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithConstMember&&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_exact_v<WithConstMember const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithConstMember const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithConstMember const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithRvalueMember>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithRvalueMember&>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_exact_v<WithRvalueMember&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithRvalueMember const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithRvalueMember const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithRvalueMember const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithLvalueConstMember>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithLvalueConstMember&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithLvalueConstMember&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithLvalueConstMember const>));
    TEST_EXPECT_TRUE((::scl::has_arrow_member_exact_v<WithLvalueConstMember const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithLvalueConstMember const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithGlobal>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithGlobal&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithGlobal&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithGlobal const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithGlobal const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<WithGlobal const&&>));

    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<NoOperators>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<NoOperators&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<NoOperators&&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<NoOperators const>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<NoOperators const&>));
    TEST_EXPECT_FALSE((::scl::has_arrow_member_exact_v<NoOperators const&&>));
}

// ----------------------------------------------------------------------------
// MEMBER POINTER ACCESS (->*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ArrowToPointerMember)
{
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const&&, int>));
}

TEST(OperatorDetectionTest, ArrowToPointerMemberExact)
{
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const&&, int>));

    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithMember, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember&&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember&, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const, int>));
    TEST_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const&&, int>));

    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators&&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const&, int>));
    TEST_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const&&, int>));
}

