#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/operator.h>

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
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithRvalueMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_v<NoOperators const &>); // built-in
    STATIC_EXPECT_FALSE(::scl::has_address_of_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, AddressOfMember)
{
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, AddressOfMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_address_of_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// INDIRECTION (*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Indirection)
{
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int *>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int *&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int *&&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int * const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int * const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<int * const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, IndirectionMember)
{
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, IndirectionMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_indirection_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// UNARY PLUS (+)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, UnaryPlus)
{
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryPlusMember)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryPlusMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_plus_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// UNARY MINUS (-)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, UnaryMinus)
{
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryMinusMember)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, UnaryMinusMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_unary_minus_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// PREFIX INCREMENT (++)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PrefixIncrement)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixIncrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixIncrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_increment_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// PREFIX DECREMENT (--)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PrefixDecrement)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixDecrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PrefixDecrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_prefix_decrement_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// BITWISE NOT (~)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseNot)
{
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, BitwiseNotMember)
{
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, BitwiseNotMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_bitwise_not_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// LOGICAL NOT (!)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalNot)
{
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, LogicalNotMember)
{
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, LogicalNotMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_logical_not_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// POSTFIX INCREMENT (++)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PostfixIncrement)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixIncrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixIncrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_increment_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// POSTFIX DECREMENT (--)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, PostfixDecrement)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixDecrementMember)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, PostfixDecrementMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_postfix_decrement_member_exact_v<NoOperators const &&>);
}

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

TEST(OperatorDetectionTest, BitwiseAnd)
{
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseAndMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_and_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE OR (|)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseOr)
{
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseOrMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_or_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY BITWISE XOR (^)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, BitwiseXor)
{
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorMember)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, BitwiseXorMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_bitwise_xor_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LOGICAL AND (&&)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalAnd)
{
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalAndMember)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalAndMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_and_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// BINARY LOGICAL OR (||)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, LogicalOr)
{
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalOrMember)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, LogicalOrMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_logical_or_member_exact_v<NoOperators const &&, int>));
}

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

TEST(OperatorDetectionTest, MultiplyAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, MultiplyAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_multiply_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// DIVIDE ASSIGN (/=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, DivideAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, DivideAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_divide_assign_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// MODULO ASSIGN (%=)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ModuloAssign)
{
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<int, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloAssignMember)
{
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ModuloAssignMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_modulo_assign_member_exact_v<NoOperators const &&, int>));
}

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

// ============================================================================
// TESTS - SPECIAL
// ============================================================================

// ----------------------------------------------------------------------------
// SUBSCRIPT ([])
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Subscript)
{
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, SubscriptMember)
{
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, SubscriptMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_subscript_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// FUNCTION CALL (())
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Call)
{
    STATIC_EXPECT_FALSE((::scl::has_call_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CallMember)
{
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CallMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_call_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_call_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// COMMA (,)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, Comma)
{
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int, int>)); // built-in
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CommaMember)
{
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, CommaMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_comma_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_comma_member_exact_v<NoOperators const &&, int>));
}

// ----------------------------------------------------------------------------
// ARROW (->)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ArrowMember)
{
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int *>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int *&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int *&&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int const *>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int const *&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<int const *&&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithMember>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithConstMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithRvalueMember const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_v<NoOperators const &&>);
}

TEST(OperatorDetectionTest, ArrowMemberExact)
{
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<int const &&>);

    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember &&>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithConstMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember &>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithRvalueMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithRvalueMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember const>);
    STATIC_EXPECT_TRUE(::scl::has_arrow_member_exact_v<WithLvalueConstMember const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithLvalueConstMember const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<WithGlobal const &&>);

    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators &&>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators const>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators const &>);
    STATIC_EXPECT_FALSE(::scl::has_arrow_member_exact_v<NoOperators const &&>);
}

// ----------------------------------------------------------------------------
// MEMBER POINTER ACCESS (->*)
// ----------------------------------------------------------------------------

TEST(OperatorDetectionTest, ArrowToPointerMember)
{
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithConstMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_v<NoOperators const &&, int>));
}

TEST(OperatorDetectionTest, ArrowToPointerMemberExact)
{
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<int const &&, int>));

    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember &&, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember &, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithRvalueMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const, int>));
    STATIC_EXPECT_TRUE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithLvalueConstMember const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<WithGlobal const &&, int>));

    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators &&, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const &, int>));
    STATIC_EXPECT_FALSE((::scl::has_arrow_to_pointer_member_exact_v<NoOperators const &&, int>));
}
