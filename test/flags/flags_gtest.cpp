#include <gtest_utils.h>

#include <scl/utility/flags.h>

#include <iterator>
#include <ranges>
#include <stdexcept>
#include <vector>

namespace
{
    enum class flag_number
    {
        One,
        Two,
        Three,
    };

    using flag_numbers = ::scl::flags<flag_number>;
} // namespace

TEST(FlagsTest, ConstructAllOfAndSubscript)
{
    STATIC_EXPECT_TRUE(flag_numbers{}.all_of()); // empty pack vacuously true
    STATIC_EXPECT_FALSE(flag_numbers{}.all_of(flag_number::One));

    constexpr flag_numbers f{flag_number::One, flag_number::Two};
    STATIC_EXPECT_TRUE(f.all_of(flag_number::One));
    STATIC_EXPECT_TRUE(f.all_of(flag_number::One, flag_number::Two));
    STATIC_EXPECT_FALSE(f.all_of(flag_number::Three));

    STATIC_EXPECT_TRUE(f[flag_number::One]);
    STATIC_EXPECT_FALSE(f[flag_number::Three]);

    STATIC_EXPECT_TRUE(f == flag_numbers(flag_number::Two, flag_number::One)); // order-independent
    STATIC_EXPECT_TRUE(f != flag_numbers(flag_number::One));
}

TEST(FlagsTest, OrdinalOutOfRangeThrows)
{
    using tiny = ::scl::flags<flag_number, 2>;                           // valid ordinals: 0,1
    EXPECT_THROW((void)tiny(flag_number::Three), ::std::out_of_range);   // ctor, ordinal 2
    EXPECT_THROW((void)tiny{}[flag_number::Three], ::std::out_of_range); // subscript
}

TEST(FlagsTest, BitwiseOperators)
{
    using enum flag_number;
    constexpr flag_numbers a{One, Two};
    constexpr flag_numbers b{Two, Three};

    STATIC_EXPECT_TRUE((a | b) == flag_numbers(One, Two, Three));
    STATIC_EXPECT_TRUE((a & b) == flag_numbers(Two));
    STATIC_EXPECT_TRUE((a ^ b) == flag_numbers(One, Three));

    STATIC_EXPECT_TRUE((a | Three) == flag_numbers(One, Two, Three));
    STATIC_EXPECT_TRUE((a & One) == flag_numbers(One));
    STATIC_EXPECT_TRUE((a ^ One) == flag_numbers(Two));

    using tiny = ::scl::flags<flag_number, 2>; // bits 0,1; ~ must not set padding
    STATIC_EXPECT_TRUE(~tiny{} == tiny(One, Two));
    STATIC_EXPECT_TRUE(~tiny(One) == tiny(Two));
}

TEST(FlagsTest, CompoundAssignment)
{
    using enum flag_number;

    auto const or_flags = [] {
        flag_numbers f{One};
        f |= flag_numbers{Two};
        f |= Three;
        return f;
    };
    STATIC_EXPECT_TRUE(or_flags() == flag_numbers(One, Two, Three));

    auto const and_flags = [] {
        flag_numbers f{One, Two, Three};
        f &= flag_numbers{Two, Three};
        f &= Two;
        return f;
    };
    STATIC_EXPECT_TRUE(and_flags() == flag_numbers(Two));

    auto const xor_flags = [] {
        flag_numbers f{One, Two};
        f ^= flag_numbers{Two, Three};
        f ^= One;
        return f;
    };
    STATIC_EXPECT_TRUE(xor_flags() == flag_numbers(Three));
}

TEST(FlagsTest, VariadicPredicates)
{
    using enum flag_number;
    constexpr flag_numbers f{One, Two};

    STATIC_EXPECT_TRUE(f.any_of(One, Three));
    STATIC_EXPECT_FALSE(f.any_of(Three));
    STATIC_EXPECT_FALSE(f.any_of()); // empty pack

    STATIC_EXPECT_TRUE(f.none_of(Three));
    STATIC_EXPECT_FALSE(f.none_of(One, Three));
    STATIC_EXPECT_TRUE(f.none_of()); // empty pack
}

TEST(FlagsTest, FlagsFormPredicates)
{
    using enum flag_number;
    constexpr flag_numbers f{One, Two};

    STATIC_EXPECT_TRUE(f.all_of(flag_numbers{One, Two})); // subset (equal)
    STATIC_EXPECT_TRUE(f.all_of(flag_numbers{One}));      // subset (proper)
    STATIC_EXPECT_FALSE(f.all_of(flag_numbers{One, Three}));

    STATIC_EXPECT_TRUE(f.any_of(flag_numbers{Two, Three})); // intersection
    STATIC_EXPECT_FALSE(f.any_of(flag_numbers{Three}));

    STATIC_EXPECT_TRUE(f.none_of(flag_numbers{Three})); // disjoint
    STATIC_EXPECT_FALSE(f.none_of(flag_numbers{Two, Three}));
}

TEST(FlagsTest, WholeMaskQueries)
{
    using enum flag_number;
    using tiny = ::scl::flags<flag_number, 2>; // bits 0,1

    STATIC_EXPECT_TRUE(tiny{}.none());
    STATIC_EXPECT_FALSE(tiny{}.any());
    STATIC_EXPECT_FALSE(tiny{}.all());

    STATIC_EXPECT_TRUE(tiny(One).any());
    STATIC_EXPECT_FALSE(tiny(One).none());
    STATIC_EXPECT_FALSE(tiny(One).all());

    STATIC_EXPECT_TRUE(tiny(One, Two).all());
}

TEST(FlagsTest, RangeConceptsAndSize)
{
    static_assert(::std::bidirectional_iterator<flag_numbers::const_iterator>);
    static_assert(::std::ranges::bidirectional_range<flag_numbers>);
    static_assert(::std::ranges::sized_range<flag_numbers>);

    using enum flag_number;
    STATIC_EXPECT_TRUE(flag_numbers{}.size() == 0u);
    STATIC_EXPECT_TRUE(flag_numbers(One, Three).size() == 2u);
}

TEST(FlagsTest, ForwardIteration)
{
    using enum flag_number;
    flag_numbers const f{One, Three};

    ::std::vector<flag_number> seen;
    for (flag_number const flag : f)
        seen.push_back(flag);

    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], One); // ascending ordinal order
    EXPECT_EQ(seen[1], Three);
}

TEST(FlagsTest, ReverseIteration)
{
    using enum flag_number;
    flag_numbers const f{One, Three};

    ::std::vector<flag_number> seen;
    for (auto it = f.rbegin(); it != f.rend(); ++it)
        seen.push_back(*it);

    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], Three);
    EXPECT_EQ(seen[1], One);
}

TEST(FlagsTest, IteratorBoundaries)
{
    using enum flag_number;

    flag_numbers const empty{};
    EXPECT_EQ(empty.begin(), empty.end()); // empty range
    EXPECT_EQ(empty.rbegin(), empty.rend());

    flag_numbers const single{Two};
    ASSERT_EQ(single.size(), 1u);
    EXPECT_EQ(*single.begin(), Two);
    EXPECT_EQ(::std::next(single.begin()), single.end());
    EXPECT_EQ(*single.rbegin(), Two); // single-element reverse
    EXPECT_EQ(::std::next(single.rbegin()), single.rend());
}

TEST(FlagsTest, BidirectionalIterator)
{
    using enum flag_number;
    flag_numbers const f{One, Two, Three};

    EXPECT_EQ(*::std::next(f.begin(), 2), Three);
    EXPECT_EQ(*::std::prev(f.end()), Three);
    EXPECT_EQ(::std::ranges::distance(f), 3);

    auto it = f.begin();
    ++it;
    EXPECT_EQ(*it, Two);
    --it;
    EXPECT_EQ(*it, One);
}
