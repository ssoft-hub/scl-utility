#include <gtest_utils.h>

#include <scl/utility/flags.h>

#include <array>
#include <cstddef>
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

    // Ordinals on the byte boundaries: the last bit of a byte, the first of the next one,
    // the last valid bit of a 12-bit mask, and a bit two bytes from the first.
    enum class wide_flag
    {
        First = 0,
        ByteEnd = 7,
        ByteStart = 8,
        PartialEnd = 11,
        Far = 23,
    };

    using wide_flags = ::scl::flags<wide_flag, 24>;
    using partial_flags = ::scl::flags<wide_flag, 12>;

    template <typename Flags>
    constexpr Flags every_ordinal_of()
    {
        Flags result;
        for (::std::size_t ordinal = 0; ordinal < Flags::capacity; ++ordinal)
            result |= static_cast<Flags::enum_type>(ordinal);
        return result;
    }
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
}

TEST(FlagsTest, Difference)
{
    using enum flag_number;
    constexpr flag_numbers a{One, Two};

    STATIC_EXPECT_TRUE((a - flag_numbers{Two, Three}) == flag_numbers(One)); // overlapping
    STATIC_EXPECT_TRUE((a - flag_numbers{Three}) == a);                      // disjoint
    STATIC_EXPECT_TRUE((a - a) == flag_numbers{});                           // self
    STATIC_EXPECT_TRUE((a - flag_numbers{}) == a);                           // empty subtrahend
    STATIC_EXPECT_TRUE((flag_numbers{} - a) == flag_numbers{});              // empty minuend
    STATIC_EXPECT_TRUE((a - Two) == flag_numbers(One));
}

TEST(FlagsTest, ComplementAgainstNamedUniverse)
{
    using enum flag_number;
    constexpr flag_numbers universe{One, Two, Three};
    constexpr flag_numbers f{Two};

    STATIC_EXPECT_TRUE((universe - f) == flag_numbers(One, Three));
    STATIC_EXPECT_TRUE((universe ^ f) == flag_numbers(One, Three));
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

    auto const minus_flags = [] {
        flag_numbers f{One, Two, Three};
        f -= flag_numbers{Two};
        f -= Three;
        return f;
    };
    STATIC_EXPECT_TRUE(minus_flags() == flag_numbers(One));
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
    constexpr tiny universe{One, Two};

    STATIC_EXPECT_TRUE(tiny{}.none());
    STATIC_EXPECT_FALSE(tiny{}.any());
    STATIC_EXPECT_FALSE(tiny{}.all_of(universe));

    STATIC_EXPECT_TRUE(tiny(One).any());
    STATIC_EXPECT_FALSE(tiny(One).none());
    STATIC_EXPECT_FALSE(tiny(One).all_of(universe));

    STATIC_EXPECT_TRUE(universe.all_of(universe));

    STATIC_EXPECT_FALSE(static_cast<bool>(tiny{}));
    STATIC_EXPECT_TRUE(static_cast<bool>(tiny(One)));
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

TEST(FlagsTest, ForwardIterationCrossesByteBoundary)
{
    using enum wide_flag;
    wide_flags const f{ByteEnd, ByteStart};

    ::std::vector<wide_flag> seen;
    for (wide_flag const flag : f)
        seen.push_back(flag);

    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], ByteEnd);
    EXPECT_EQ(seen[1], ByteStart);
}

TEST(FlagsTest, ReverseIterationCrossesByteBoundary)
{
    using enum wide_flag;
    wide_flags const f{ByteEnd, ByteStart};

    ::std::vector<wide_flag> seen;
    for (auto it = f.rbegin(); it != f.rend(); ++it)
        seen.push_back(*it);

    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], ByteStart);
    EXPECT_EQ(seen[1], ByteEnd);
}

TEST(FlagsTest, IterationSkipsBytesWithoutSetBits)
{
    using enum wide_flag;
    wide_flags const f{First, Far};

    ::std::vector<wide_flag> seen;
    for (wide_flag const flag : f)
        seen.push_back(flag);

    ASSERT_EQ(seen.size(), 2u);
    EXPECT_EQ(seen[0], First);
    EXPECT_EQ(seen[1], Far);
}

TEST(FlagsTest, SingleBitOnByteBoundaryIsReachedFromBothEnds)
{
    using enum wide_flag;

    wide_flags const at_byte_end{ByteEnd};
    EXPECT_EQ(*at_byte_end.begin(), ByteEnd);
    EXPECT_EQ(*at_byte_end.rbegin(), ByteEnd);
    EXPECT_EQ(::std::next(at_byte_end.begin()), at_byte_end.end());

    wide_flags const at_byte_start{ByteStart};
    EXPECT_EQ(*at_byte_start.begin(), ByteStart);
    EXPECT_EQ(*at_byte_start.rbegin(), ByteStart);
    EXPECT_EQ(::std::next(at_byte_start.begin()), at_byte_start.end());
}

TEST(FlagsTest, EmptyMultiByteMaskYieldsEmptyRange)
{
    wide_flags const f{};

    STATIC_EXPECT_EQ(wide_flags{}.size(), 0u);
    EXPECT_EQ(f.begin(), f.end());
    EXPECT_EQ(f.rbegin(), f.rend());
}

TEST(FlagsTest, FullMaskWithPartialLastByteStopsAtCapacity)
{
    using enum wide_flag;
    constexpr partial_flags full = every_ordinal_of<partial_flags>();

    STATIC_EXPECT_EQ(full.size(), 12u);
    EXPECT_EQ(::std::ranges::distance(full), 12);
    EXPECT_EQ(*full.begin(), First);
    EXPECT_EQ(*::std::prev(full.end()), PartialEnd);
}

TEST(FlagsTest, SizeCountsSetBitsInEveryByte)
{
    using enum wide_flag;

    constexpr wide_flags spread{First, ByteEnd, ByteStart, Far};
    STATIC_EXPECT_EQ(spread.size(), 4u);
    STATIC_EXPECT_EQ(every_ordinal_of<wide_flags>().size(), 24u);
    STATIC_EXPECT_EQ(wide_flags{Far}.size(), 1u);
}

TEST(FlagsTest, IterationRunsInConstantEvaluation)
{
    using enum wide_flag;

    auto const walk = [] {
        constexpr wide_flags f{ByteEnd, ByteStart, Far};
        ::std::array<wide_flag, 3> seen{};
        ::std::size_t index = 0;
        for (wide_flag const flag : f)
            seen[index++] = flag;
        return seen;
    };

    constexpr ::std::array expected{ByteEnd, ByteStart, Far};
    STATIC_EXPECT_TRUE(walk() == expected);
}

TEST(FlagsTest, ReverseIterationRunsInConstantEvaluation)
{
    using enum wide_flag;

    auto const walk_back = [] {
        constexpr partial_flags f{First, ByteEnd, ByteStart, PartialEnd};
        ::std::array<wide_flag, 4> seen{};
        ::std::size_t index = 0;
        for (auto it = f.rbegin(); it != f.rend(); ++it)
            seen[index++] = *it;
        return seen;
    };

    constexpr ::std::array expected{PartialEnd, ByteStart, ByteEnd, First};
    STATIC_EXPECT_TRUE(walk_back() == expected);
}
