#include <gtest_utils.h>

#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <cstdint>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>

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
 * @test key<Hasher> values match the corresponding free-function digest.
 */
TEST(KeyTypeTest, ValueMatchesHasherFreeFunction)
{
    STATIC_EXPECT_EQ(key<fnv1a_hasher>{"hello"}.value, fnv1a("hello"));
    STATIC_EXPECT_EQ(key<djb2_hasher>{"hello"}.value, djb2("hello"));
    STATIC_EXPECT_EQ(key<sdbm_hasher>{"hello"}.value, sdbm("hello"));
    STATIC_EXPECT_EQ(key<jenkins_ota_hasher>{"hello"}.value, jenkins_ota("hello"));
    STATIC_EXPECT_EQ(key<siphash_hasher<>>{"hello"}.value, siphash("hello"));
}

// ============================================================================
// key<> (default SipHash) — core behaviour
// ============================================================================

/**
 * @test Same input → equal keys.
 */
TEST(KeyDefaultTest, EqualityForSameInput) { STATIC_EXPECT_EQ(key<>{"same"}, key<>{"same"}); }

/**
 * @test Different inputs → unequal keys.
 */
TEST(KeyDefaultTest, InequalityForDifferentInputs)
{
    STATIC_EXPECT_NE(key<>{"alpha"}, key<>{"beta"});
}

/**
 * @test Comparison operators derive from value ordering.
 */
TEST(KeyDefaultTest, ComparisonOperators)
{
    constexpr key<> a{"alpha"};
    constexpr key<> b{"beta"};
    STATIC_EXPECT_TRUE((a < b) != (b < a)); // strict ordering
    STATIC_EXPECT_LE(a, a);
    STATIC_EXPECT_GE(a, a);
}

/**
 * @test Implicit conversion to value_type returns the raw digest.
 */
TEST(KeyDefaultTest, ImplicitConversionToValueType)
{
    constexpr key<> k{"convert"};
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
    key<> const k{"std_hash"};
    EXPECT_EQ(h(k), h(k));
}

/**
 * @test std::hash partial specialisation — distinct inputs hash differently.
 */
TEST(KeyDefaultTest, StdHashDistinct)
{
    ::std::hash<key<>> const h{};
    EXPECT_NE(h(key<>{"one"}), h(key<>{"two"}));
}

/**
 * @test key<> is usable as key in std::unordered_map.
 */
TEST(KeyDefaultTest, UnorderedMapKey)
{
    ::std::unordered_map<key<>, int> map;
    map[key<>{"key_a"}] = 1;
    map[key<>{"key_b"}] = 2;
    EXPECT_EQ(map.at(key<>{"key_a"}), 1);
    EXPECT_EQ(map.at(key<>{"key_b"}), 2);
    EXPECT_EQ(map.size(), 2u);
}

/**
 * @test key<> is usable as key in std::unordered_set (deduplication).
 */
TEST(KeyDefaultTest, UnorderedSetKey)
{
    ::std::unordered_set<key<>> set;
    set.insert(key<>{"alpha"});
    set.insert(key<>{"beta"});
    set.insert(key<>{"alpha"}); // duplicate
    EXPECT_EQ(set.size(), 2u);
    EXPECT_TRUE(set.contains(key<>{"alpha"}));
    EXPECT_FALSE(set.contains(key<>{"gamma"}));
}

/**
 * @test std::string and std::string_view with the same content produce the same key.
 */
TEST(KeyDefaultTest, StringAndStringViewEquivalent)
{
    STATIC_EXPECT_EQ(key<>{::std::string_view{"equal"}}, key<>{::std::string_view{"equal"}});
    EXPECT_EQ(key<>{::std::string{"equal"}}, key<>{::std::string_view{"equal"}});
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
        case key<>{"start"}:
            return 1;
        case key<>{"stop"}:
            return 2;
        case key<>{"pause"}:
            return 3;
        case key<>{"resume"}:
            return 4;
        default:
            return 0;
        }
    };
    STATIC_EXPECT_EQ(dispatch(key<>{"start"}), 1);
    STATIC_EXPECT_EQ(dispatch(key<>{"stop"}), 2);
    STATIC_EXPECT_EQ(dispatch(key<>{"pause"}), 3);
    STATIC_EXPECT_EQ(dispatch(key<>{"resume"}), 4);
    STATIC_EXPECT_EQ(dispatch(key<>{"other"}), 0);
    STATIC_EXPECT_EQ(dispatch(key<>{""}), 0);
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
        case fnv_key{"red"}:
            return 1;
        case fnv_key{"green"}:
            return 2;
        case fnv_key{"blue"}:
            return 3;
        default:
            return 0;
        }
    };
    STATIC_EXPECT_EQ(dispatch(fnv_key{"red"}), 1);
    STATIC_EXPECT_EQ(dispatch(fnv_key{"green"}), 2);
    STATIC_EXPECT_EQ(dispatch(fnv_key{"blue"}), 3);
    STATIC_EXPECT_EQ(dispatch(fnv_key{"white"}), 0);
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
struct nttp_handler<key<>{"alpha"}>
{
    static constexpr int value = 1;
};
template <>
struct nttp_handler<key<>{"beta"}>
{
    static constexpr int value = 2;
};
template <>
struct nttp_handler<key<>{"gamma"}>
{
    static constexpr int value = 3;
};

/**
 * @test key<> is a structural type — different string keys yield distinct types.
 */
TEST(KeyNttpTest, DistinctTypesForDifferentKeys)
{
    STATIC_EXPECT_FALSE((::std::is_same_v<tagged<key<>{"hello"}>, tagged<key<>{"world"}>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<tagged<key<>{"hello"}>, tagged<key<>{"hello"}>>));
}

/**
 * @test The id member of a tagged type matches the key used to instantiate it.
 */
TEST(KeyNttpTest, EmbeddedIdMatchesKey)
{
    STATIC_EXPECT_EQ(tagged<key<>{"event"}>::id, key<>{"event"});
}

/**
 * @test Template specializations are selected by compile-time string key.
 */
TEST(KeyNttpTest, TemplateSpecializationByKey)
{
    STATIC_EXPECT_EQ(nttp_handler<key<>{"alpha"}>::value, 1);
    STATIC_EXPECT_EQ(nttp_handler<key<>{"beta"}>::value, 2);
    STATIC_EXPECT_EQ(nttp_handler<key<>{"gamma"}>::value, 3);
    STATIC_EXPECT_EQ(nttp_handler<key<>{"other"}>::value, 0);
}

/**
 * @test key with a non-default hasher is also usable as NTTP.
 */
TEST(KeyNttpTest, NonDefaultHasherAsNttp)
{
    using fnv_key = key<fnv1a_hasher>;
    STATIC_EXPECT_FALSE((::std::is_same_v<tagged<key<>{"x"}>, tagged<key<>{"y"}>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<tagged<key<>{"same"}>, tagged<key<>{"same"}>>));
    // fnv1a_hasher-based key is a distinct type from siphash-based key.
    STATIC_EXPECT_FALSE((::std::is_same_v<fnv_key, key<>>));
}

// ============================================================================
// Cross-algorithm: same input → distinct digests per hasher
// ============================================================================

/**
 * @test All 64-bit hashers produce distinct values for the same input.
 */
TEST(KeyCrossAlgorithmTest, AllHashersDistinct)
{
    constexpr auto h_fnv = key<fnv1a_hasher>{"hello"}.value;
    constexpr auto h_djb = key<djb2_hasher>{"hello"}.value;
    constexpr auto h_sdb = key<sdbm_hasher>{"hello"}.value;
    constexpr auto h_sip = key<siphash_hasher<>>{"hello"}.value;
    STATIC_EXPECT_NE(h_fnv, h_djb);
    STATIC_EXPECT_NE(h_fnv, h_sdb);
    STATIC_EXPECT_NE(h_fnv, h_sip);
    STATIC_EXPECT_NE(h_djb, h_sdb);
    STATIC_EXPECT_NE(h_djb, h_sip);
    STATIC_EXPECT_NE(h_sdb, h_sip);
}
