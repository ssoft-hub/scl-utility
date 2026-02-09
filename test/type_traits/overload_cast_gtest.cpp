#include <gtest/gtest.h>

#include <scl/utility/type_traits/overload_cast.h>

using namespace scl;

// --- Global Functions ---
void foo(float const *) {}
void foo(long *) noexcept {}

// --- Test Class ---
struct S
{
    // Normal / Noexcept
    void foo(int) {}
    void foo(double) noexcept {}

    // L-ref / L-ref Noexcept
    void foo(float) & {}
    void foo(long) & noexcept {}

    // R-ref / R-ref Noexcept
    void foo(char) && {}
    void foo(short) && noexcept {}

    // Const / Const Noexcept
    void foo(unsigned int) const {}
    void foo(bool) const noexcept {}

    // Const L-ref / Const L-ref Noexcept
    void foo(long double) const & {}
    void foo(signed char) const & noexcept {}

    // Const R-ref / Const R-ref Noexcept
    void foo(unsigned char) const && {}
    void foo(unsigned short) const && noexcept {}

    // Volatile / Volatile Noexcept
    void foo(unsigned long) volatile {}
    void foo(long long) volatile noexcept {}

    // Volatile L-ref / Volatile L-ref Noexcept
    void foo(unsigned long long) volatile & {}
    void foo(void *) volatile & noexcept {}

    // Volatile R-ref / Volatile R-ref Noexcept
    void foo(void const *) volatile && {}
    void foo(char *) volatile && noexcept {}

    // Const Volatile / Const Volatile Noexcept
    void foo(char const *) const volatile {}
    void foo(int *) const volatile noexcept {}

    // Const Volatile L-ref / Const Volatile L-ref Noexcept
    void foo(int const *) const volatile & {}
    void foo(double *) const volatile & noexcept {}

    // Const Volatile R-ref / Const Volatile R-ref Noexcept
    void foo(double const *) const volatile && {}
    void foo(float *) const volatile && noexcept {}
};

TEST(OverloadCastTest, FullCoverage)
{
    // Free functions
    EXPECT_NE(overload_cast<float const *>(&foo), nullptr);
    EXPECT_NE(overload_cast<long *>(&foo), nullptr);

    // Standard members
    EXPECT_NE(overload_cast<int>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<double>(&S::foo), nullptr);

    // Ref-qualified members
    EXPECT_NE(overload_cast<float>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<long>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<char>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<short>(&S::foo), nullptr);

    // Const members
    EXPECT_NE(overload_cast<unsigned int>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<bool>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<long double>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<signed char>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<unsigned char>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<unsigned short>(&S::foo), nullptr);

    // Volatile members
    EXPECT_NE(overload_cast<unsigned long>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<long long>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<unsigned long long>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<void *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<void const *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<char *>(&S::foo), nullptr);

    // Const Volatile members
    EXPECT_NE(overload_cast<char const *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<int *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<int const *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<double *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<double const *>(&S::foo), nullptr);
    EXPECT_NE(overload_cast<float *>(&S::foo), nullptr);
}
