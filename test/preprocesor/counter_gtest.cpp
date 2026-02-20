#include <gtest/gtest.h>

#include <scl/utility/preprocessor/counter.h>

// === Test with a simple struct tag ===

// Define a tag for the first counter
struct MyCounterTag1
{};

// The counter starts at 0 before any increments
constexpr auto val1_0 = SCL_COUNTER_VALUE(MyCounterTag1);

SCL_COUNTER_NEXT(MyCounterTag1)
constexpr auto val1_1 = SCL_COUNTER_VALUE(MyCounterTag1);

SCL_COUNTER_NEXT(MyCounterTag1)
constexpr auto val1_2 = SCL_COUNTER_VALUE(MyCounterTag1);

// === Test with another simple struct tag to ensure counters are independent ===

// Define a tag for the second counter
struct MyCounterTag2
{};

constexpr auto val2_0 = SCL_COUNTER_VALUE(MyCounterTag2);

SCL_COUNTER_NEXT(MyCounterTag2)
constexpr auto val2_1 = SCL_COUNTER_VALUE(MyCounterTag2);

// === Test with a template tag, which requires SCL_FORWARD to work correctly ===

// Define a template tag
template <typename T>
struct TemplateTag
{};

// The SCL_FORWARD macro is crucial here
constexpr auto tpl_val_0 = SCL_COUNTER_VALUE(TemplateTag<int>);

SCL_COUNTER_NEXT(TemplateTag<int>)
constexpr auto tpl_val_1 = SCL_COUNTER_VALUE(TemplateTag<int>);

// A different specialization of the same template tag should be a different counter
constexpr auto tpl_val_float_0 = SCL_COUNTER_VALUE(TemplateTag<float>);

TEST(CompileTimeCounter, HandlesSimpleTags)
{
    // Check initial and incremented values
    EXPECT_EQ(val1_0, 0);
    EXPECT_EQ(val1_1, 1);
    EXPECT_EQ(val1_2, 2);
}

TEST(CompileTimeCounter, CountersAreIndependent)
{
    // Check that the second counter was not affected by the first
    EXPECT_EQ(val2_0, 0);
    EXPECT_EQ(val2_1, 1);

    // Re-check the first counter's final value to ensure it's independent
    EXPECT_EQ(val1_2, 2);
}

TEST(CompileTimeCounter, HandlesTemplateTags)
{
    // Check values for the template tag with `int`
    EXPECT_EQ(tpl_val_0, 0);
    EXPECT_EQ(tpl_val_1, 1);
}

TEST(CompileTimeCounter, TemplateSpecializationsAreIndependent)
{
    // Check that the `float` specialization is a separate counter
    EXPECT_EQ(tpl_val_float_0, 0);

    // Re-check the `int` specialization's final value
    EXPECT_EQ(tpl_val_1, 1);
}
