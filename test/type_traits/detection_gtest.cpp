#include <gtest/gtest.h>

#include <scl/utility/type_traits/detection.h>

#include <string>
#include <vector>

// Test fixtures and helper types
namespace
{
    struct HasFoo
    {
        int foo() { return 42; }
    };

    struct NoFoo
    {
        void bar() {}
    };

    struct HasValueType
    {
        using value_type = int;
    };

    struct NoValueType
    {};

    struct ReturnsDouble
    {
        double compute() { return 3.14; }
    };

    // Detection trait templates
    template <typename T>
    using has_foo_t = decltype(::std::declval<T>().foo());

    template <typename T>
    using has_bar_t = decltype(::std::declval<T>().bar());

    template <typename T>
    using value_type_t = typename T::value_type;

    template <typename T>
    using compute_result_t = decltype(::std::declval<T>().compute());

    template <typename T>
    using has_begin_t = decltype(::std::declval<T>().begin());

    template <typename T>
    using has_size_t = decltype(::std::declval<T>().size());

    template <typename T>
    using iterator_t = typename T::iterator;

    template <typename T>
    using inner_type_t = typename T::Inner::type;

} // anonymous namespace

// Test is_detected
TEST(DetectionTest, IsDetected_MemberFunction)
{
    EXPECT_TRUE((::scl::is_detected_v<has_foo_t, HasFoo>));
    EXPECT_FALSE((::scl::is_detected_v<has_foo_t, NoFoo>));
    EXPECT_FALSE((::scl::is_detected_v<has_bar_t, HasFoo>));
    EXPECT_TRUE((::scl::is_detected_v<has_bar_t, NoFoo>));
}

TEST(DetectionTest, IsDetected_MemberType)
{
    EXPECT_TRUE((::scl::is_detected_v<value_type_t, HasValueType>));
    EXPECT_FALSE((::scl::is_detected_v<value_type_t, NoValueType>));
    EXPECT_TRUE((::scl::is_detected_v<value_type_t, ::std::vector<int>>));
    EXPECT_TRUE((::scl::is_detected_v<value_type_t, ::std::string>));
}

TEST(DetectionTest, IsDetected_STLContainers)
{
    EXPECT_TRUE((::scl::is_detected_v<has_begin_t, ::std::vector<int>>));
    EXPECT_TRUE((::scl::is_detected_v<has_size_t, ::std::vector<int>>));
    EXPECT_FALSE((::scl::is_detected_v<has_begin_t, int>));
    EXPECT_FALSE((::scl::is_detected_v<has_size_t, int>));
}

// Test detected_t
TEST(DetectionTest, DetectedT_ValidType)
{
    using detected = ::scl::detected_t<value_type_t, HasValueType>;
    EXPECT_TRUE((::std::is_same_v<detected, int>));
}

TEST(DetectionTest, DetectedT_InvalidType)
{
    using detected = ::scl::detected_t<value_type_t, NoValueType>;
    EXPECT_TRUE((::std::is_same_v<detected, void>));
}

TEST(DetectionTest, DetectedT_STLTypes)
{
    using vec_value = ::scl::detected_t<value_type_t, ::std::vector<double>>;
    EXPECT_TRUE((::std::is_same_v<vec_value, double>));

    using str_value = ::scl::detected_t<value_type_t, ::std::string>;
    EXPECT_TRUE((::std::is_same_v<str_value, char>));
}

// Test detected_or_t
TEST(DetectionTest, DetectedOrT_ValidType)
{
    using detected = ::scl::detected_or_t<float, value_type_t, HasValueType>;
    EXPECT_TRUE((::std::is_same_v<detected, int>));
}

TEST(DetectionTest, DetectedOrT_InvalidType_UsesDefault)
{
    using detected = ::scl::detected_or_t<float, value_type_t, NoValueType>;
    EXPECT_TRUE((::std::is_same_v<detected, float>));
}

TEST(DetectionTest, DetectedOrT_ComplexDefault)
{
    using detected = ::scl::detected_or_t<::std::string, value_type_t, NoValueType>;
    EXPECT_TRUE((::std::is_same_v<detected, ::std::string>));
}

// Test is_detected_exact
TEST(DetectionTest, IsDetectedExact_MatchingType)
{
    EXPECT_TRUE((::scl::is_detected_exact_v<int, value_type_t, HasValueType>));
    EXPECT_TRUE((::scl::is_detected_exact_v<double, value_type_t, ::std::vector<double>>));
}

TEST(DetectionTest, IsDetectedExact_NonMatchingType)
{
    EXPECT_FALSE((::scl::is_detected_exact_v<float, value_type_t, HasValueType>));
    EXPECT_FALSE((::scl::is_detected_exact_v<int, value_type_t, NoValueType>));
}

TEST(DetectionTest, IsDetectedExact_ReturnType)
{
    EXPECT_TRUE((::scl::is_detected_exact_v<double, compute_result_t, ReturnsDouble>));
    EXPECT_FALSE((::scl::is_detected_exact_v<float, compute_result_t, ReturnsDouble>));
}

// Test is_detected_convertible
TEST(DetectionTest, IsDetectedConvertible_SameType)
{
    EXPECT_TRUE((::scl::is_detected_convertible_v<int, value_type_t, HasValueType>));
}

TEST(DetectionTest, IsDetectedConvertible_ConvertibleType)
{
    EXPECT_TRUE((::scl::is_detected_convertible_v<double, value_type_t, HasValueType>));
    EXPECT_TRUE((::scl::is_detected_convertible_v<long, value_type_t, HasValueType>));
}

TEST(DetectionTest, IsDetectedConvertible_InvalidDetection)
{
    EXPECT_FALSE((::scl::is_detected_convertible_v<int, value_type_t, NoValueType>));
}

TEST(DetectionTest, IsDetectedConvertible_ReturnType)
{
    EXPECT_TRUE((::scl::is_detected_convertible_v<double, compute_result_t, ReturnsDouble>));
    EXPECT_TRUE((::scl::is_detected_convertible_v<float, compute_result_t, ReturnsDouble>));
    EXPECT_TRUE((::scl::is_detected_convertible_v<int, compute_result_t, ReturnsDouble>));
}

// Advanced use cases
TEST(DetectionTest, ComplexTraits_Iterators)
{
    EXPECT_TRUE((::scl::is_detected_v<iterator_t, ::std::vector<int>>));
    EXPECT_FALSE((::scl::is_detected_v<iterator_t, int>));
}

TEST(DetectionTest, ComplexTraits_NestedTypes)
{
    struct Nested
    {
        struct Inner
        {
            using type = double;
        };
    };

    EXPECT_TRUE((::scl::is_detected_v<inner_type_t, Nested>));
    EXPECT_TRUE((::scl::is_detected_exact_v<double, inner_type_t, Nested>));
}

TEST(DetectionTest, SFINAE_Integration)
{
    // Demonstrate use in constexpr if
    auto process = []<typename T>(T && value) {
        if constexpr (::scl::is_detected_v<has_size_t, T>)
            return value.size();
        else
            return 1ul;
    };

    ::std::vector<int> vec{1, 2, 3};
    EXPECT_EQ(process(vec), 3ul);
    EXPECT_EQ(process(42), 1ul);
}
