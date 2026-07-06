#pragma once

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
