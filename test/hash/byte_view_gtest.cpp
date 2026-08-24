#include <gtest_utils.h>

#include <scl/utility/hash/byte_view.h>
#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <algorithm>
#include <array>
#include <bit>
#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

using namespace ::scl::hash;

namespace
{
    /// Satisfied when every hash function of the module accepts @p Range.
    // clang-format off
    template <typename Range>
    concept hashable = requires(Range const & range) {
        fnv1a(range);
        djb2(range);
        sdbm(range);
        jenkins_ota(range);
        siphash(range);
        key<>{range};
    };
    // clang-format on

    /// Satisfied when @ref scl::hash::byte_view accepts @p Range.
    template <typename Range>
    concept spellable = requires(Range const & range) { byte_view(range); };

    enum class byte_enum : unsigned char
    {
        value = 7
    };

    struct one_byte
    {
        char c;
    };

    struct empty
    {};
} // namespace

// ============================================================================
// Accepted element types — one byte wide, and every bit of it is data
// ============================================================================

/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
// A key standing as a template argument is what proves the value is a constant, not merely
// foldable: a template argument admits nothing else.
template <key<> Id>
struct wide_tag
{
    static constexpr int value = 1;
};

TEST(HashElementTest, ByteSizedElementsAccepted)
{
    STATIC_EXPECT_TRUE(hashable<::std::string_view>);
    STATIC_EXPECT_TRUE(hashable<::std::vector<char>>);
    STATIC_EXPECT_TRUE(hashable<::std::vector<signed char>>);
    STATIC_EXPECT_TRUE(hashable<::std::vector<unsigned char>>);
    STATIC_EXPECT_TRUE(hashable<::std::u8string_view>);
    STATIC_EXPECT_TRUE(hashable<::std::span<::std::byte const>>);
    STATIC_EXPECT_TRUE(hashable<::std::vector<byte_enum>>);
    STATIC_EXPECT_TRUE(hashable<::std::vector<one_byte>>);
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

// ============================================================================
// Rejected element types — wider than a byte, so most of each element would
// never reach the hash value
// ============================================================================

/**
 * @test A range whose element is wider than a byte is rejected.
 */
TEST(HashElementTest, WiderThanByteElementsRejected)
{
    STATIC_EXPECT_FALSE(hashable<::std::wstring_view>);
    STATIC_EXPECT_FALSE(hashable<::std::u16string_view>);
    STATIC_EXPECT_FALSE(hashable<::std::u32string_view>);
    STATIC_EXPECT_FALSE(hashable<::std::vector<int>>);
    STATIC_EXPECT_FALSE(hashable<::std::vector<unsigned>>);
    STATIC_EXPECT_FALSE(hashable<::std::vector<double>>);
}

/**
 * @test An element carrying no value of its own is rejected.
 */
TEST(HashElementTest, EmptyElementRejected) { STATIC_EXPECT_FALSE(hashable<::std::vector<empty>>); }

// ============================================================================
// byte_view — the explicit conversion a wider element needs
// ============================================================================

/**
 * @test A wider range becomes hashable once spelled through byte_view.
 */
TEST(HashByteViewTest, MakesAWiderRangeHashable)
{
    STATIC_EXPECT_FALSE(hashable<::std::u16string_view>);
    STATIC_EXPECT_TRUE(hashable<decltype(byte_view(::std::u16string_view{}))>);
    STATIC_EXPECT_TRUE(hashable<decltype(byte_view(::std::declval<::std::vector<int> const &>()))>);
}

/**
 * @test A range byte_view cannot spell alike on every platform is rejected.
 */
TEST(HashByteViewTest, NonPortableElementsRejected)
{
    STATIC_EXPECT_FALSE(spellable<::std::wstring_view>);
    STATIC_EXPECT_FALSE(spellable<::std::vector<double>>);
    STATIC_EXPECT_TRUE(spellable<::std::u16string_view>);
    STATIC_EXPECT_TRUE(spellable<::std::u32string_view>);
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
 */
TEST(HashByteViewTest, ElementsSpellTheirBytesLittleEndian)
{
    static constexpr ::std::uint32_t values[]{0x04030201U, 0x08070605U};
    static constexpr ::std::uint8_t expected[]{1, 2, 3, 4, 5, 6, 7, 8};
    STATIC_EXPECT_TRUE(::std::ranges::equal(byte_view(values), expected));
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

    STATIC_EXPECT_EQ(::std::ranges::equal(byte_view(value), storage), storage_spells_the_value);
}

/**
 * @test A byte-sized element passes through unchanged.
 */
TEST(HashByteViewTest, ByteElementIsUnchanged)
{
    STATIC_EXPECT_EQ(fnv1a(byte_view(::std::string_view{"hello"})), fnv1a(::std::string_view{"hello"}));
}

/**
 * @test A wide literal is hashed as its text, without the terminating element.
 */
TEST(HashByteViewTest, WideLiteralDropsItsTerminator)
{
    STATIC_EXPECT_EQ(fnv1a(byte_view(u"AB")), fnv1a(byte_view(::std::u16string_view{u"AB"})));
    STATIC_EXPECT_EQ(fnv1a(byte_view(U"AB")), fnv1a(byte_view(::std::u32string_view{U"AB"})));
    STATIC_EXPECT_EQ(fnv1a(byte_view(u8"AB")), fnv1a(byte_view(::std::u8string_view{u8"AB"})));
}

/**
 * @test Every hasher agrees on a wide literal and its view spelling.
 */
TEST(HashByteViewTest, WideLiteralMatchesItsViewForEveryHasher)
{
    STATIC_EXPECT_EQ(djb2(byte_view(u"start")), djb2(byte_view(::std::u16string_view{u"start"})));
    STATIC_EXPECT_EQ(sdbm(byte_view(u"start")), sdbm(byte_view(::std::u16string_view{u"start"})));
    STATIC_EXPECT_EQ(jenkins_ota(byte_view(u"start")),
        jenkins_ota(byte_view(::std::u16string_view{u"start"})));
    STATIC_EXPECT_EQ(siphash(byte_view(u"start")), siphash(byte_view(::std::u16string_view{u"start"})));

    static constexpr key<> from_literal{byte_view(u"start")};
    static constexpr key<> from_view{byte_view(::std::u16string_view{u"start"})};
    STATIC_EXPECT_EQ(from_literal, from_view);
}

/**
 * @test A wide array that does not end in zero is hashed whole, the way a narrow one is.
 */
TEST(HashByteViewTest, WideArrayWithoutTerminatingZeroHashedWhole)
{
    static constexpr char16_t raw[3]{u'A', u'B', u'C'};
    STATIC_EXPECT_EQ(fnv1a(byte_view(raw)), fnv1a(byte_view(::std::u16string_view{u"ABC"})));
}

/**
 * @test An array of a non-character element keeps its trailing zero: there it is data.
 */
TEST(HashByteViewTest, NonCharacterArrayKeepsItsTrailingZero)
{
    static constexpr ::std::uint16_t with_zero[3]{1, 2, 0};
    static constexpr ::std::uint16_t without[2]{1, 2};
    STATIC_EXPECT_NE(fnv1a(byte_view(with_zero)), fnv1a(byte_view(without)));
}

/**
 * @test A narrow literal reaches the same value through the adapter as without it.
 */
TEST(HashByteViewTest, NarrowLiteralThroughTheAdapterMatchesTheDirectPath)
{
    STATIC_EXPECT_EQ(fnv1a(byte_view("hello")), fnv1a("hello"));
    STATIC_EXPECT_EQ(fnv1a(byte_view(u8"hello")), fnv1a(u8"hello"));
}

/**
 * @test A bounded array answers its bytes as a value, without its terminating element.
 */
TEST(HashByteViewTest, BoundedArrayAnswersConstantBytes)
{
    static constexpr auto wide = byte_view(u"AB");
    STATIC_EXPECT_EQ(wide.count, 4U);
    STATIC_EXPECT_EQ(sizeof(wide.bytes), 6U);

    static constexpr auto narrow = byte_view("hello");
    STATIC_EXPECT_EQ(narrow.count, 5U);

    static constexpr ::std::uint16_t data[3]{1, 2, 0};
    static constexpr auto whole = byte_view(data);
    STATIC_EXPECT_EQ(whole.count, 6U);
}

/**
 * @test A key over a wide literal is a constant, so it stands as a template argument.
 */
TEST(HashByteViewTest, KeyFromWideLiteralIsAConstant)
{
    static constexpr key<> id{byte_view(u"event.started")};
    STATIC_EXPECT_EQ(wide_tag<id>::value, 1);
    STATIC_EXPECT_NE(id, key<>{byte_view(u"event.stopped")});
}
