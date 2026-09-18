#include <gtest_utils.h>

#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <array>
#include <cstdint>
#include <span>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

using namespace ::std::string_view_literals;

namespace
{
    /// Satisfied when @ref scl::hash::key accepts @p Range.
    template <typename Range>
    concept key_takes = requires(Range const & range) { ::scl::hash::key<>{range}; };
} // namespace

using namespace ::scl::hash;

// ============================================================================
// value_type deduction from Hasher
// ============================================================================

/**
 * @test value_type is deduced from the hasher's return type.
 */
TEST(KeyTypeTest, ValueTypeDeduction)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<key<fnv1a_hasher>::value_type, ::std::uint64_t>));
    STATIC_EXPECT_TRUE((::std::is_same_v<key<djb2_hasher>::value_type, ::std::uint64_t>));
    STATIC_EXPECT_TRUE((::std::is_same_v<key<sdbm_hasher>::value_type, ::std::uint64_t>));
    STATIC_EXPECT_TRUE((::std::is_same_v<key<jenkins_ota_hasher>::value_type, ::std::uint32_t>));
    STATIC_EXPECT_TRUE((::std::is_same_v<key<siphash_hasher<>>::value_type, ::std::uint64_t>));
}

/**
 * @test Default template argument is siphash_hasher<>.
 */
TEST(KeyTypeTest, DefaultHasherIsSipHash)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<key<>::hasher_type, siphash_hasher<>>));
}

/**
 * @test key<Hasher> values match the corresponding free-function hash value.
 */
TEST(KeyTypeTest, ValueMatchesHasherFreeFunction)
{
    STATIC_EXPECT_EQ(key<fnv1a_hasher>{"hello"sv}.value, fnv1a("hello"sv));
    STATIC_EXPECT_EQ(key<djb2_hasher>{"hello"sv}.value, djb2("hello"sv));
    STATIC_EXPECT_EQ(key<sdbm_hasher>{"hello"sv}.value, sdbm("hello"sv));
    STATIC_EXPECT_EQ(key<jenkins_ota_hasher>{"hello"sv}.value, jenkins_ota("hello"sv));
    STATIC_EXPECT_EQ(key<siphash_hasher<>>{"hello"sv}.value, siphash("hello"sv));
}

// ============================================================================
// key<> (default SipHash) — core behaviour
// ============================================================================

/**
 * @test Same input → equal keys.
 */
TEST(KeyDefaultTest, EqualityForSameInput) { STATIC_EXPECT_EQ(key<>{"same"sv}, key<>{"same"sv}); }

/**
 * @test Different inputs → unequal keys.
 */
TEST(KeyDefaultTest, InequalityForDifferentInputs)
{
    STATIC_EXPECT_NE(key<>{"alpha"sv}, key<>{"beta"sv});
}

/**
 * @test Comparison operators derive from value ordering.
 */
TEST(KeyDefaultTest, ComparisonOperators)
{
    constexpr key<> a{"alpha"sv};
    constexpr key<> b{"beta"sv};
    STATIC_EXPECT_TRUE((a < b) != (b < a)); // strict ordering
    STATIC_EXPECT_LE(a, a);
    STATIC_EXPECT_GE(a, a);
}

/**
 * @test Implicit conversion to value_type returns the raw hash value.
 */
TEST(KeyDefaultTest, ImplicitConversionToValueType)
{
    constexpr key<> k{"convert"sv};
    constexpr key<>::value_type v = k;
    STATIC_EXPECT_EQ(v, k.value);
}

/**
 * @test std::hash partial specialisation — same instance hashes consistently.
 */
TEST(KeyDefaultTest, StdHashDeterministic)
{
    // std::hash<integral>::operator() is not required to be constexpr by the standard.
    ::std::hash<key<>> const h{};
    key<> const k{"std_hash"sv};
    EXPECT_EQ(h(k), h(k));
}

/**
 * @test std::hash partial specialisation — distinct inputs hash differently.
 */
TEST(KeyDefaultTest, StdHashDistinct)
{
    ::std::hash<key<>> const h{};
    EXPECT_NE(h(key<>{"one"sv}), h(key<>{"two"sv}));
}

/**
 * @test key<> is usable as key in std::unordered_map.
 */
TEST(KeyDefaultTest, UnorderedMapKey)
{
    ::std::unordered_map<key<>, int> map;
    map[key<>{"key_a"sv}] = 1;
    map[key<>{"key_b"sv}] = 2;
    EXPECT_EQ(map.at(key<>{"key_a"sv}), 1);
    EXPECT_EQ(map.at(key<>{"key_b"sv}), 2);
    EXPECT_EQ(map.size(), 2u);
}

/**
 * @test key<> is usable as key in std::unordered_set (deduplication).
 */
TEST(KeyDefaultTest, UnorderedSetKey)
{
    ::std::unordered_set<key<>> set;
    set.insert(key<>{"alpha"sv});
    set.insert(key<>{"beta"sv});
    set.insert(key<>{"alpha"sv}); // duplicate
    EXPECT_EQ(set.size(), 2u);
    EXPECT_TRUE(set.contains(key<>{"alpha"sv}));
    EXPECT_FALSE(set.contains(key<>{"gamma"sv}));
}

/**
 * @test std::string and std::string_view with the same content produce the same key.
 */
TEST(KeyDefaultTest, StringAndStringViewEquivalent)
{
    STATIC_EXPECT_EQ(key<>{::std::string_view{"equal"}}, key<>{::std::string_view{"equal"}});
    EXPECT_EQ(key<>{::std::string{"equal"}}, key<>{::std::string_view{"equal"}});
}

/**
 * @test One text produces one key, however it is spelled.
 */
TEST(KeyDefaultTest, OneTextMakesOneKeyHoweverSpelled)
{
    static constexpr ::std::array<char, 5> held{'s', 't', 'a', 'r', 't'};
    STATIC_EXPECT_EQ(key<>{"start"sv}, key<>{held});
    STATIC_EXPECT_EQ(key<>{"start"sv}, key<>{::std::span{held}});
    EXPECT_EQ(key<>{"start"sv}, key<>{::std::string{"start"}});
}

/**
 * @test A zero byte is data, so a longer input makes a different key.
 */
TEST(KeyDefaultTest, AZeroByteIsData)
{
    static constexpr ::std::string_view with_zero{"start", 6};
    STATIC_EXPECT_NE(key<>{with_zero}, key<>{"start"sv});
}

/**
 * @test An array is refused and a wider element with it, while a span over the array
 *       names the bytes the array holds.
 */
TEST(KeyDefaultTest, ArrayIsRefusedAndASpanNamesItsBytes)
{
    static constexpr char raw[3]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(key_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(key_takes<char[3]>);
    STATIC_EXPECT_FALSE(key_takes<::std::u16string_view>);
    STATIC_EXPECT_EQ(key<>{::std::span{raw}}, key<>{::std::string_view{"abc"}});
}

/**
 * @test Every hasher agrees on two spellings of one text.
 */
TEST(KeyCrossAlgorithmTest, EveryHasherAgreesOnTwoSpellings)
{
    static constexpr ::std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
    STATIC_EXPECT_EQ(key<fnv1a_hasher>{"hello"sv}, key<fnv1a_hasher>{held});
    STATIC_EXPECT_EQ(key<djb2_hasher>{"hello"sv}, key<djb2_hasher>{held});
    STATIC_EXPECT_EQ(key<sdbm_hasher>{"hello"sv}, key<sdbm_hasher>{held});
    STATIC_EXPECT_EQ(key<jenkins_ota_hasher>{"hello"sv}, key<jenkins_ota_hasher>{held});
    STATIC_EXPECT_EQ(key<siphash_hasher<>>{"hello"sv}, key<siphash_hasher<>>{held});
}

// ============================================================================
// switch/case with key<Hasher>
// ============================================================================

/**
 * @test key<> (SipHash) usable as switch/case label.
 */
TEST(KeySwitchTest, DefaultHasherSwitchCase)
{
    auto dispatch = [](key<> cmd) constexpr -> int {
        switch (cmd)
        {
        case key<>{"start"sv}:
            return 1;
        case key<>{"stop"sv}:
            return 2;
        case key<>{"pause"sv}:
            return 3;
        case key<>{"resume"sv}:
            return 4;
        default:
            return 0;
        }
    };
    STATIC_EXPECT_EQ(dispatch(key<>{"start"sv}), 1);
    STATIC_EXPECT_EQ(dispatch(key<>{"stop"sv}), 2);
    STATIC_EXPECT_EQ(dispatch(key<>{"pause"sv}), 3);
    STATIC_EXPECT_EQ(dispatch(key<>{"resume"sv}), 4);
    STATIC_EXPECT_EQ(dispatch(key<>{"other"sv}), 0);
    STATIC_EXPECT_EQ(dispatch(key<>{""sv}), 0);
}

/**
 * @test key<fnv1a_hasher> usable as switch/case label.
 */
TEST(KeySwitchTest, Fnv1aSwitchCase)
{
    using fnv_key = key<fnv1a_hasher>;
    auto dispatch = [](fnv_key cmd) constexpr -> int {
        switch (cmd)
        {
        case fnv_key{"red"sv}:
            return 1;
        case fnv_key{"green"sv}:
            return 2;
        case fnv_key{"blue"sv}:
            return 3;
        default:
            return 0;
        }
    };
    STATIC_EXPECT_EQ(dispatch(fnv_key{"red"sv}), 1);
    STATIC_EXPECT_EQ(dispatch(fnv_key{"green"sv}), 2);
    STATIC_EXPECT_EQ(dispatch(fnv_key{"blue"sv}), 3);
    STATIC_EXPECT_EQ(dispatch(fnv_key{"white"sv}), 0);
}

// ============================================================================
// key<Hasher> as a non-type template parameter (NTTP)
// ============================================================================

/// Tag type parameterized by a compile-time string key.
template <key<> K>
struct tagged
{
    static constexpr key<> id = K;
};

/// Template specialization selected by a compile-time string key.
template <key<> Cmd>
struct nttp_handler
{
    static constexpr int value = 0;
};

template <>
struct nttp_handler<key<>{"alpha"sv}>
{
    static constexpr int value = 1;
};

template <>
struct nttp_handler<key<>{"beta"sv}>
{
    static constexpr int value = 2;
};

template <>
struct nttp_handler<key<>{"gamma"sv}>
{
    static constexpr int value = 3;
};

/**
 * @test key<> is a structural type — different string keys yield distinct types.
 */
TEST(KeyNttpTest, DistinctTypesForDifferentKeys)
{
    STATIC_EXPECT_FALSE((::std::is_same_v<tagged<key<>{"hello"sv}>, tagged<key<>{"world"sv}>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<tagged<key<>{"hello"sv}>, tagged<key<>{"hello"sv}>>));
}

/**
 * @test The id member of a tagged type matches the key used to instantiate it.
 */
TEST(KeyNttpTest, EmbeddedIdMatchesKey)
{
    STATIC_EXPECT_EQ(tagged<key<>{"event"sv}>::id, key<>{"event"sv});
}

/**
 * @test Template specializations are selected by compile-time string key.
 */
TEST(KeyNttpTest, TemplateSpecializationByKey)
{
    STATIC_EXPECT_EQ(nttp_handler<key<>{"alpha"sv}>::value, 1);
    STATIC_EXPECT_EQ(nttp_handler<key<>{"beta"sv}>::value, 2);
    STATIC_EXPECT_EQ(nttp_handler<key<>{"gamma"sv}>::value, 3);
    STATIC_EXPECT_EQ(nttp_handler<key<>{"other"sv}>::value, 0);
}

/**
 * @test key with a non-default hasher is also usable as NTTP.
 */
TEST(KeyNttpTest, NonDefaultHasherAsNttp)
{
    using fnv_key = key<fnv1a_hasher>;
    STATIC_EXPECT_FALSE((::std::is_same_v<tagged<key<>{"x"sv}>, tagged<key<>{"y"sv}>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<tagged<key<>{"same"sv}>, tagged<key<>{"same"sv}>>));
    // fnv1a_hasher-based key is a distinct type from siphash-based key.
    STATIC_EXPECT_FALSE((::std::is_same_v<fnv_key, key<>>));
}

// ============================================================================
// Cross-algorithm: same input → distinct hash values per hasher
// ============================================================================

/**
 * @test All 64-bit hashers produce distinct values for the same input.
 */
TEST(KeyCrossAlgorithmTest, AllHashersDistinct)
{
    constexpr auto h_fnv = key<fnv1a_hasher>{"hello"sv}.value;
    constexpr auto h_djb = key<djb2_hasher>{"hello"sv}.value;
    constexpr auto h_sdb = key<sdbm_hasher>{"hello"sv}.value;
    constexpr auto h_sip = key<siphash_hasher<>>{"hello"sv}.value;
    STATIC_EXPECT_NE(h_fnv, h_djb);
    STATIC_EXPECT_NE(h_fnv, h_sdb);
    STATIC_EXPECT_NE(h_fnv, h_sip);
    STATIC_EXPECT_NE(h_djb, h_sdb);
    STATIC_EXPECT_NE(h_djb, h_sip);
    STATIC_EXPECT_NE(h_sdb, h_sip);
}
