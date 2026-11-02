#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/function.h>

#include <type_traits>

namespace plain
{
    int multiply(int value, int factor);
    double multiply(double value, double factor);
    void reset();
    void bind(int & value);
    void scan(int const * values);
    long calm(int value) noexcept;

    long retire(int value) = delete;
    long retire(long value);

    inline auto deduced(int value) { return value * 2L; }

    template <typename Type>
    Type single(Type value);

    SCL_FUNCTION_DETECTION(multiply)
    SCL_FUNCTION_DETECTION(reset)
    SCL_FUNCTION_DETECTION(bind)
    SCL_FUNCTION_DETECTION(scan)
    SCL_FUNCTION_DETECTION(calm)
    SCL_FUNCTION_DETECTION(retire)
    SCL_FUNCTION_DETECTION(deduced)
    SCL_FUNCTION_DETECTION(single)
} // namespace plain

namespace overloads
{
    long pick(int value);
    char pick(int && value);

    template <typename Type>
    Type echo(Type value);
    long echo(int value);

    template <typename Type>
    Type gather(Type value);
    long gather(int value);
    char gather(int && value);

    long settle(int value) noexcept;
    char settle(int && value);

    SCL_FUNCTION_DETECTION(pick)
    SCL_FUNCTION_DETECTION(echo)
    SCL_FUNCTION_DETECTION(gather)
    SCL_FUNCTION_DETECTION(settle)
} // namespace overloads

namespace shapes
{
    struct square
    {};

    struct circle
    {
        friend double visit(circle, double factor) { return factor; }
    };
} // namespace shapes

namespace later
{
    void visit();

    SCL_FUNCTION_DETECTION(visit)
} // namespace later

namespace shapes
{
    long visit(square shape);
} // namespace shapes

namespace callables
{
    struct doubler
    {
        int operator()(int value) const;
    };

    inline constexpr doubler twice{};
    inline constexpr int (*halve)(int) = nullptr;

    SCL_FUNCTION_DETECTION(twice)
    SCL_FUNCTION_DETECTION(halve)
} // namespace callables

namespace scoped
{
    int multiply(int value, int factor);

    struct class_traits
    {
        SCL_FUNCTION_DETECTION(multiply)
    };

    double multiply(double value, double factor);
} // namespace scoped

TEST(TypeTraitsFunctionDetection, IsTrueWhereTheCallCompiles)
{
    STATIC_EXPECT_TRUE((plain::has_multiply_function_v<int, int>));
    STATIC_EXPECT_TRUE(plain::has_reset_function_v<>);
}

TEST(TypeTraitsFunctionDetection, ConvertsTheArgumentsImplicitly)
{
    STATIC_EXPECT_TRUE((plain::has_multiply_function_v<int, long>));
}

TEST(TypeTraitsFunctionDetection, IsFalseWhereTheCallDoesNotCompile)
{
    STATIC_EXPECT_FALSE(plain::has_multiply_function_v<int>);
    STATIC_EXPECT_FALSE((plain::has_multiply_function_v<int, char const *>));
    STATIC_EXPECT_FALSE(plain::has_reset_function_v<int>);
}

TEST(TypeTraitsFunctionDetection, ResultIsTheTypeOfTheCall)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::multiply_function_t<double, double>, double>));
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::reset_function_t<>, void>));
}

TEST(TypeTraitsFunctionDetection, ResultIsNonesuchWhereTheCallDoesNotCompile)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::multiply_function_t<int>, ::scl::nonesuch>));
}

TEST(TypeTraitsFunctionDetection, FindsThroughTheArgumentsAFunctionDeclaredLater)
{
    STATIC_EXPECT_TRUE(later::has_visit_function_v<shapes::square>);
    STATIC_EXPECT_TRUE((::std::is_same_v<later::visit_function_t<shapes::square>, long>));
}

TEST(TypeTraitsFunctionDetection, FindsAFriendDefinedInAClass)
{
    STATIC_EXPECT_TRUE((later::has_visit_function_v<shapes::circle, double>));
}

TEST(TypeTraitsFunctionDetection, CallsAVariableOfACallableType)
{
    STATIC_EXPECT_TRUE(callables::has_twice_function_v<int>);
    STATIC_EXPECT_TRUE(callables::has_halve_function_v<int>);
}

TEST(TypeTraitsFunctionDetection, ExpandsInAClass)
{
    STATIC_EXPECT_TRUE((scoped::class_traits::has_multiply_function_v<int, int>));
    STATIC_EXPECT_TRUE((scoped::class_traits::has_multiply_function_exact_v<int, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<scoped::class_traits::multiply_function_t<int, int>, int>));
}

TEST(TypeTraitsFunctionDetectionExact, IsTrueForExactlyTheParameterTypes)
{
    STATIC_EXPECT_TRUE((plain::has_multiply_function_exact_v<int, int>));
    STATIC_EXPECT_TRUE((plain::has_multiply_function_exact_v<double, double>));
    STATIC_EXPECT_TRUE(plain::has_reset_function_exact_v<>);
}

TEST(TypeTraitsFunctionDetectionExact, IsFalseWhereTheArgumentsOnlyConvert)
{
    STATIC_EXPECT_FALSE((plain::has_multiply_function_exact_v<int, long>));
    STATIC_EXPECT_FALSE(plain::has_reset_function_exact_v<int>);
}

TEST(TypeTraitsFunctionDetectionExact, IgnoresTheTopLevelQualifiersOfTheArguments)
{
    STATIC_EXPECT_TRUE((plain::has_multiply_function_exact_v<int const, int volatile>));
    STATIC_EXPECT_TRUE((plain::has_multiply_function_exact_v<int const volatile, int>));
}

TEST(TypeTraitsFunctionDetectionExact, CountsTheQualifiersBelowTheTopLevel)
{
    STATIC_EXPECT_TRUE(plain::has_bind_function_exact_v<int &>);
    STATIC_EXPECT_FALSE(plain::has_bind_function_exact_v<int const &>);
    STATIC_EXPECT_TRUE(plain::has_scan_function_exact_v<int const[3]>);
    STATIC_EXPECT_FALSE(plain::has_scan_function_exact_v<int[3]>);
}

TEST(TypeTraitsFunctionDetectionExact, IsFalseForADeletedFunction)
{
    STATIC_EXPECT_FALSE(plain::has_retire_function_exact_v<int>);
    STATIC_EXPECT_TRUE(plain::has_retire_function_exact_v<long>);
}

TEST(TypeTraitsFunctionDetectionExact, IsFalseForAFunctionFoundOnlyThroughTheArguments)
{
    STATIC_EXPECT_FALSE(later::has_visit_function_exact_v<shapes::square>);
    STATIC_EXPECT_FALSE((later::has_visit_function_exact_v<shapes::circle, double>));
}

TEST(TypeTraitsFunctionDetectionExact, IsFalseForAFunctionDeclaredAfterTheExpansion)
{
    STATIC_EXPECT_FALSE((scoped::class_traits::has_multiply_function_exact_v<double, double>));
}

TEST(TypeTraitsFunctionDetectionExact, IsFalseForAVariableOfACallableType)
{
    STATIC_EXPECT_FALSE(callables::has_twice_function_exact_v<int>);
    STATIC_EXPECT_FALSE(callables::has_halve_function_exact_v<int>);
}

TEST(TypeTraitsFunctionDetectionExact, ResultIsTheResultTypeOfTheFunctionFound)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::multiply_function_exact_t<double, double>, double>));
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::multiply_function_exact_t<int, long>, ::scl::nonesuch>));
}

TEST(TypeTraitsFunctionDetectionExact, FindsAFunctionWithADeducedResultType)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::deduced_function_exact_t<int>, long>));
}

TEST(TypeTraitsFunctionDetectionExact, FindsASpecializationOfAFunctionTemplate)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::single_function_exact_t<double>, double>));
}

TEST(TypeTraitsFunctionDetectionExact, FindsANoexceptFunction)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<plain::calm_function_exact_t<int>, long>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsFunctionDetectionExact, FindsEachOverloadBesideAnAmbiguousPlainCall)
{
    STATIC_EXPECT_FALSE(overloads::has_pick_function_v<int>);
    STATIC_EXPECT_TRUE((::std::is_same_v<overloads::pick_function_exact_t<int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<overloads::pick_function_exact_t<int &&>, char>));
    STATIC_EXPECT_FALSE(overloads::has_pick_function_exact_v<int &>);
    STATIC_EXPECT_TRUE((::std::is_same_v<overloads::settle_function_exact_t<int>, long>));
}
#else
TEST(TypeTraitsFunctionDetectionExact, IsFalseBesideAnAmbiguousPlainCallOnMsvc)
{
    STATIC_EXPECT_FALSE(overloads::has_pick_function_v<int>);
    STATIC_EXPECT_FALSE(overloads::has_pick_function_exact_v<int>);
    STATIC_EXPECT_FALSE(overloads::has_settle_function_exact_v<int>);
}
#endif

TEST(TypeTraitsFunctionDetectionExact, TakesTheOverloadOfThePlainCallBesideAFunctionTemplate)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<overloads::echo_function_exact_t<int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<overloads::echo_function_exact_t<double>, double>));
}

TEST(TypeTraitsFunctionDetectionExact, IsFalseBesideAFunctionTemplateWhereThePlainCallIsAmbiguous)
{
    STATIC_EXPECT_FALSE(overloads::has_gather_function_v<int>);
    STATIC_EXPECT_FALSE(overloads::has_gather_function_exact_v<int>);
}
