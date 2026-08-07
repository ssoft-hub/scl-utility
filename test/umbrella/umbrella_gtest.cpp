#include <gtest_utils.h>

#include <scl/utility.h>

#include <string>

namespace
{
    enum class color : int
    {
        red = 1,
        green = 2,
    };

    SCL_NODISCARD constexpr int answer() noexcept { return 42; }
} // namespace

/**
 * @test The umbrella header carries the any views.
 */
TEST(UmbrellaHeaderTest, ExposesAny)
{
    int const value = 7;
    ::scl::any_view const view{value};
    EXPECT_TRUE(view.has_value());
}

/**
 * @test The umbrella header carries the attribute macros.
 */
TEST(UmbrellaHeaderTest, ExposesAttribute) { STATIC_EXPECT_EQ(answer(), 42); }

/**
 * @test The umbrella header carries the concepts.
 */
TEST(UmbrellaHeaderTest, ExposesConcepts) { STATIC_EXPECT_TRUE(::scl::concepts::enum_type<color>); }

/**
 * @test The umbrella header carries the flags.
 */
TEST(UmbrellaHeaderTest, ExposesFlags)
{
    ::scl::flags<color> const granted{color::red};
    EXPECT_TRUE(granted[color::red]);
}

/**
 * @test The umbrella header carries the hash utilities.
 */
TEST(UmbrellaHeaderTest, ExposesHash)
{
    STATIC_EXPECT_EQ(::scl::hash::key<>{"same"}, ::scl::hash::key<>{"same"});
}

/**
 * @test The umbrella header carries the hierarchy.
 */
TEST(UmbrellaHeaderTest, ExposesHierarchy)
{
    ::scl::hierarchy::node<int> const root{7};
    EXPECT_EQ(root.get(), 7);
}

/**
 * @test The umbrella header carries the compile-time meta utilities.
 */
TEST(UmbrellaHeaderTest, ExposesMeta) { STATIC_EXPECT_EQ(::scl::type_name<int>(), "int"); }

/**
 * @test The umbrella header carries the preprocessor utilities.
 */
TEST(UmbrellaHeaderTest, ExposesPreprocessor) { STATIC_EXPECT_EQ(SCL_FORWARD(answer)(), 42); }

/**
 * @test The umbrella header carries the runtime utilities.
 */
TEST(UmbrellaHeaderTest, ExposesRuntime) { EXPECT_EQ(::scl::enum_value(color::green), "color::2"); }

/**
 * @test The umbrella header carries the type traits.
 */
TEST(UmbrellaHeaderTest, ExposesTypeTraits)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::forward_like_t<int const &, double>, double const &>));
}
