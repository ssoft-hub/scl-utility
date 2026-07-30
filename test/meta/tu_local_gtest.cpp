#include <gtest_utils.h>

#include <vector>

#include <scl/utility/meta/tu_local.h>

/**
 * @brief TU-local type: same-named types in other TUs are distinct entities.
 */
namespace
{
    struct AnonDuck
    {};
} // namespace

/**
 * @brief External-linkage type at global namespace scope.
 */
struct GlobalDuck
{};

/**
 * @brief External-linkage type inside a named namespace.
 */
namespace TuLocalNs
{
    struct Duck
    {};

    template <typename T>
    struct Box
    {};

    /**
     * @brief Structural string for use as a non-type template parameter.
     */
    template <::std::size_t N>
    struct FixedString
    {
        char chars[N] = {}; // NOLINT(*-avoid-c-arrays): structural NTTP storage

        // NOLINTNEXTLINE(google-explicit-constructor): NTTP deduction from a literal
        constexpr FixedString(char const (&text)[N]) noexcept
        {
            for (::std::size_t i = 0; i < N; ++i)
                chars[i] = text[i];
        }
    };

    template <FixedString S>
    struct StringTag
    {};

    template <char C>
    struct CharTag
    {};

    template <typename T, FixedString S>
    struct MixedTag
    {};

} // namespace TuLocalNs

/**
 * @test Verify a type declared in an anonymous namespace is detected as TU-local.
 */
TEST(TuLocalTest, AnonymousNamespaceType)
{
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<AnonDuck>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local<AnonDuck>::value);
}

/**
 * @test Verify named-scope and fundamental types are not TU-local.
 */
TEST(TuLocalTest, ExternalTypes)
{
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<int>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<GlobalDuck>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<TuLocalNs::Duck>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<::std::vector<int>>);
}

/**
 * @test Verify compound types built from a TU-local type are TU-local too.
 */
TEST(TuLocalTest, CompoundTypes)
{
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<AnonDuck *>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<AnonDuck &>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<AnonDuck const>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<::std::vector<AnonDuck>>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<TuLocalNs::Box<AnonDuck>>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<AnonDuck (*)(int)>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<void (*)(AnonDuck)>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<AnonDuck[3]>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<int AnonDuck::*>);
}

/**
 * @test Verify compound types built solely from external types stay external.
 */
TEST(TuLocalTest, ExternalCompoundTypes)
{
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<GlobalDuck *>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<TuLocalNs::Box<GlobalDuck>>);
}

/**
 * @test Verify a string NTTP spelling an anonymous-namespace marker does not
 *       misclassify an external type: the value renders inside quotes (or as
 *       character codes on MSVC), and quoted contents are skipped.
 */
TEST(TuLocalTest, StringNttpDoesNotInjectMarker)
{
    using namespace TuLocalNs;
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<StringTag<FixedString{"{anonymous}"}>>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<StringTag<FixedString{"(anonymous namespace)"}>>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<StringTag<FixedString{"`anonymous-namespace'"}>>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<StringTag<FixedString{"`anonymous namespace'"}>>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<StringTag<FixedString{"say \"hi\" \\"}>>);
}

/**
 * @test Verify character NTTPs (rendered as quoted characters on Clang/GCC)
 *       neither inject a marker nor derail the literal-skipping scan.
 */
TEST(TuLocalTest, CharNttpDoesNotInjectMarker)
{
    using namespace TuLocalNs;
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<CharTag<'{'>>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<CharTag<'\''>>);
    STATIC_EXPECT_FALSE(::scl::is_tu_local_v<CharTag<'\\'>>);
}

/**
 * @test Verify a TU-local type stays TU-local when a marker-spelling string
 *       NTTP appears in the same template argument list.
 */
TEST(TuLocalTest, TuLocalTypeWithStringNttpStaysLocal)
{
    using namespace TuLocalNs;
    using MixedGccToken = MixedTag<AnonDuck, FixedString{"{anonymous}"}>;
    using MixedClangToken = MixedTag<AnonDuck, FixedString{"(anonymous namespace)"}>;
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<MixedGccToken>);
    STATIC_EXPECT_TRUE(::scl::is_tu_local_v<MixedClangToken>);
}
