#include <gtest_utils.h>

#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <cstddef>
#include <cstdint>
#include <span>
#include <string_view>
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
