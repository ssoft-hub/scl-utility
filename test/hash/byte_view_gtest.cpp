#include <gtest_utils.h>

#include <scl/utility/hash/byte_view.h>
#include <scl/utility/hash/concepts.h>
#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <forward_list>
#include <ranges>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

namespace
{
    using ::scl::hash::concepts::hashable_range;

    /// Satisfied when @ref scl::hash::byte_view accepts @p Range.
    template <typename Range>
    concept spellable = requires(Range const & range) { ::scl::hash::byte_view(range); };

    enum class byte_enum : unsigned char
    {
        value = 7
    };

    /// Satisfied when @p Range reaches a hash function or @ref scl::hash::byte_view.
    template <typename Range>
    concept taken = hashable_range<Range> || spellable<Range>;

    /// Satisfied when @p Hasher accepts @p Range.
    template <typename Hasher, typename Range>
    concept hasher_takes = requires(Range & range) { Hasher{}(range); };

    /// A hash function written outside the group, stating its constraint with the concept.
    template <::scl::hash::concepts::hashable_range Range>
    constexpr ::std::uint64_t rolling(Range && range)
    {
        ::std::uint64_t h = 0;
        for (::std::ranges::range_value_t<Range> const c : ::std::forward<Range>(range))
            h = (h * 31ull) + static_cast<::std::uint8_t>(c);
        return h;
    }

    /// The same function under no constraint beyond being a range.
    template <::std::ranges::range Range>
    constexpr ::std::uint64_t unconstrained(Range && range)
    {
        ::std::uint64_t h = 0;
        for (::std::ranges::range_value_t<Range> const c : ::std::forward<Range>(range))
            h = (h * 31ull) + static_cast<::std::uint8_t>(c);
        return h;
    }

    /// Satisfied when @ref rolling accepts @p Range.
    template <typename Range>
    concept rolling_takes = requires(Range & range) { rolling(range); };

    /// Satisfied when @ref unconstrained accepts @p Range.
    template <typename Range>
    concept unconstrained_takes = requires(Range & range) { unconstrained(range); };

    struct two_bytes
    {
        char low;
        char high;
    };

    struct one_byte
    {
        char c;
    };

    struct empty
    {};
} // namespace

using namespace ::scl::hash;

// ============================================================================
// Accepted element types — one byte wide, and every bit of it is data
// ============================================================================

/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
TEST(HashElementTest, ByteSizedElementsAccepted)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::string_view>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<char>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<signed char>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<unsigned char>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::u8string_view>);
    STATIC_EXPECT_TRUE(hashable_range<::std::span<::std::byte const>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<byte_enum>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<one_byte>>);
}

/**
 * @test A `std::byte` range hashes as the byte values it holds.
 */
TEST(HashElementTest, ByteSpanMatchesItsCharacters)
{
    static constexpr ::std::byte bytes[]{::std::byte{'a'}, ::std::byte{'b'}, ::std::byte{'c'}};
    EXPECT_EQ(fnv1a(::std::span{bytes}), fnv1a(::std::string_view{"abc"}));
    EXPECT_EQ(siphash(::std::span{bytes}), siphash(::std::string_view{"abc"}));
}

/**
 * @test A range whose element is wider than a byte is rejected.
 */
TEST(HashElementTest, WiderThanByteElementsRejected)
{
    STATIC_EXPECT_FALSE(hashable_range<::std::wstring_view>);
    STATIC_EXPECT_FALSE(hashable_range<::std::u16string_view>);
    STATIC_EXPECT_FALSE(hashable_range<::std::u32string_view>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<int>>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<unsigned>>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<double>>);
}

/**
 * @test An element carrying no value of its own is rejected.
 */
TEST(HashElementTest, EmptyElementRejected)
{
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<empty>>);
}

/**
 * @test A wider range becomes hashable once spelled through byte_view.
 */
TEST(HashByteViewTest, MakesAWiderRangeHashable)
{
    STATIC_EXPECT_FALSE(hashable_range<::std::u16string_view>);
    STATIC_EXPECT_TRUE(hashable_range<decltype(byte_view(::std::u16string_view{}))>);
    STATIC_EXPECT_TRUE(hashable_range<decltype(byte_view(::std::declval<::std::vector<int> const &>()))>);
}

/**
 * @test A floating-point element is refused, and an integer of any width is spelled.
 */
TEST(HashByteViewTest, AFloatingPointElementIsRefused)
{
    STATIC_EXPECT_FALSE(spellable<::std::vector<double>>);
    STATIC_EXPECT_TRUE(spellable<::std::wstring_view>);
    STATIC_EXPECT_TRUE(spellable<::std::u16string_view>);
    STATIC_EXPECT_TRUE(spellable<::std::u32string_view>);
}

/**
 * @test A source of wchar_t spells as many bytes per element as the platform gives it.
 */
TEST(HashByteViewTest, AWideCharacterSourceSpellsThePlatformWidth)
{
    static constexpr ::std::uint8_t expected[sizeof(wchar_t)]{0x41U};

    STATIC_EXPECT_TRUE(::std::ranges::equal(::scl::hash::byte_view(::std::wstring_view{L"A"}),
        ::std::span{expected}));
}

/**
 * @test Inputs differing above the low byte no longer collide.
 */
TEST(HashByteViewTest, WideInputsDifferingAboveTheLowByteDiffer)
{
    STATIC_EXPECT_NE(fnv1a(byte_view(::std::u16string_view{u"Ā"})),
        fnv1a(byte_view(::std::u16string_view{u"Ȁ"})));
    STATIC_EXPECT_NE(siphash(byte_view(::std::u16string_view{u"AB"})),
        siphash(byte_view(::std::u16string_view{u"Łł"})));
}

/**
 * @test A wide spelling of a text is a different input from its narrow one.
 */
TEST(HashByteViewTest, WideDiffersFromNarrow)
{
    STATIC_EXPECT_NE(fnv1a(byte_view(::std::u16string_view{u"AB"})), fnv1a(::std::string_view{"AB"}));
}

/**
 * @test Each element contributes its bytes in little-endian order, whatever the platform.
 *
 * The order is the order of the value, not the order of the element's storage. Reading the
 * storage would make two machines hash one input differently.
 */
TEST(HashByteViewTest, ElementsSpellTheirBytesLittleEndian)
{
    static constexpr ::std::uint32_t values[]{0x04030201U, 0x08070605U};
    static constexpr ::std::uint8_t expected[]{1, 2, 3, 4, 5, 6, 7, 8};
    STATIC_EXPECT_TRUE(::std::ranges::equal(byte_view(::std::span{values}), ::std::span{expected}));
}

/**
 * @test The byte order comes from the value, not from the element's storage.
 *
 * On a little-endian host the two orders coincide, so the assertion above passes for a
 * `bit_cast` implementation as well. Reading the element's own bytes is what the module
 * must not do — two machines would then hash one input differently — and this is where
 * such an implementation parts ways with the contract.
 */
TEST(HashByteViewTest, ByteOrderIsIndependentOfTheHost)
{
    static constexpr ::std::uint32_t value[]{0x04030201U};
    static constexpr auto storage = ::std::bit_cast<::std::array<::std::uint8_t, 4>>(value[0]);
    static constexpr bool storage_spells_the_value = ::std::endian::native == ::std::endian::little;

    STATIC_EXPECT_EQ(::std::ranges::equal(byte_view(::std::span{value}), storage), storage_spells_the_value);
}

/**
 * @test A byte-sized element passes through unchanged.
 */
TEST(HashByteViewTest, ByteElementIsUnchanged)
{
    STATIC_EXPECT_EQ(fnv1a(byte_view(::std::string_view{"hello"})), fnv1a(::std::string_view{"hello"}));
}

/**
 * @test hashable_range holds both rules at once: the range is not a bounded array, and its
 *       element is a single byte. A type that is no range at all fails on the first, and
 *       the second is never reached for it.
 */
TEST(HashElementTest, HashableRangeIsAByteRangeThatIsNotABoundedArray)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::string_view>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<::std::byte>>);
    STATIC_EXPECT_TRUE((hashable_range<::std::array<char, 4>>));
    STATIC_EXPECT_TRUE(hashable_range<::std::forward_list<char>>);
    STATIC_EXPECT_FALSE(hashable_range<char[4]>);
    STATIC_EXPECT_FALSE(hashable_range<::std::u16string_view>);
    STATIC_EXPECT_FALSE(hashable_range<int>);
}

/**
 * @test A hash function written outside the group refuses an array once it states the
 *       concept, and takes one without it: the refusal stands in each signature rather
 *       than in a mechanism the group shares.
 */
TEST(HashElementTest, AnOwnHashFunctionTakesTheRefusalFromTheConcept)
{
    STATIC_EXPECT_TRUE(rolling_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(rolling_takes<char[4]>);

    STATIC_EXPECT_TRUE(unconstrained_takes<::std::string_view>);
    STATIC_EXPECT_TRUE(unconstrained_takes<char[4]>);

    STATIC_EXPECT_EQ(rolling(::std::string_view{"abc"}), unconstrained(::std::string_view{"abc"}));
}

/**
 * @test An array is refused, whatever its element type: nothing about a bound tells the
 *       storage from the content.
 */
TEST(HashByteViewTest, ArrayIsRefused)
{
    STATIC_EXPECT_FALSE(taken<char[4]>);
    STATIC_EXPECT_FALSE(taken<char8_t[4]>);
    STATIC_EXPECT_FALSE(taken<char16_t[4]>);
    STATIC_EXPECT_FALSE(taken<char32_t[4]>);
    STATIC_EXPECT_FALSE(taken<::std::uint32_t[4]>);
    STATIC_EXPECT_FALSE(taken<::std::byte[4]>);
    STATIC_EXPECT_FALSE(taken<unsigned char[4]>);
    STATIC_EXPECT_FALSE(taken<signed char[4]>);
    STATIC_EXPECT_FALSE(taken<::std::uint16_t[4]>);
    STATIC_EXPECT_FALSE(taken<wchar_t[4]>);
    STATIC_EXPECT_FALSE(taken<::std::uint8_t[4]>);
}

/**
 * @test The refusal stops at the array itself: a view over one is taken, and answers for
 *       the whole bound.
 */
TEST(HashByteViewTest, AViewOverAnArrayIsTakenAndAnswersForItsWholeBound)
{
    static constexpr char raw[4]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(taken<decltype(::std::views::all(raw))>);
    STATIC_EXPECT_EQ(fnv1a(::std::views::all(raw)), fnv1a(::std::string_view{"abc", 4}));
}

/**
 * @test A partially filled std::array is taken whole, which is where the rule stops: its
 *       bound is four, and four bytes are hashed.
 */
TEST(HashByteViewTest, APartiallyFilledArrayObjectIsTakenWhole)
{
    static constexpr ::std::array<char, 4> held{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE((taken<::std::array<char, 4>>));
    STATIC_EXPECT_EQ(fnv1a(held), fnv1a(::std::string_view{"abc", 4}));
}

/**
 * @test An element one byte past the rule is refused, whatever spells it.
 */
TEST(HashElementTest, AnElementJustPastTheByteRuleIsRefused)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<one_byte>>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<two_bytes>>);
}

/**
 * @test Every hasher takes and refuses exactly what the free function it wraps does.
 */
TEST(HashElementTest, EveryHasherRefusesAnArray)
{
    STATIC_EXPECT_TRUE((hasher_takes<fnv1a_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<djb2_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<sdbm_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<jenkins_ota_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<siphash_hasher<>, ::std::string_view>));

    STATIC_EXPECT_FALSE((hasher_takes<fnv1a_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<djb2_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<sdbm_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<jenkins_ota_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<siphash_hasher<>, char[4]>));

    STATIC_EXPECT_FALSE((hasher_takes<fnv1a_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<djb2_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<sdbm_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<jenkins_ota_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<siphash_hasher<>, ::std::u16string_view>));
}

/**
 * @test Every range the element rule admits is taken unless it is a bounded array, a
 *       fixed-extent std::array and std::span included.
 */
TEST(HashByteViewTest, EveryRangeButTheBoundedArrayIsTaken)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::string_view>);
    STATIC_EXPECT_TRUE((hashable_range<::std::array<char, 4>>));
    STATIC_EXPECT_TRUE((hashable_range<::std::span<char const, 4>>));
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<char>>);
    STATIC_EXPECT_TRUE(spellable<::std::u16string_view>);
    STATIC_EXPECT_TRUE((spellable<::std::array<char16_t, 4>>));
    STATIC_EXPECT_TRUE((spellable<::std::span<char16_t const, 4>>));
}

/**
 * @test A range whose end of traversal is of its own type is hashed by every function of
 *       the group, which a fold over two iterators of one type could not take.
 */
TEST(HashElementTest, ARangeWhoseSentinelIsOfItsOwnTypeIsHashed)
{
    ::std::string_view const text{"abcx"};
    auto taken = ::std::views::take_while(text, [](char const c) { return c != 'x'; });

    STATIC_EXPECT_TRUE(hashable_range<decltype(taken)>);
    EXPECT_EQ(::scl::hash::djb2(taken), ::scl::hash::djb2(::std::string_view{"abc"}));
    EXPECT_EQ(::scl::hash::sdbm(taken), ::scl::hash::sdbm(::std::string_view{"abc"}));
    EXPECT_EQ(::scl::hash::fnv1a(taken), ::scl::hash::fnv1a(::std::string_view{"abc"}));
}
