#include <gtest_utils.h>

#include <scl/utility/type_traits/detection/method.h>

#include <type_traits>

namespace
{
    struct counter
    {
        void reset();
        int query(int value);
        long add(int value) &;
        long take(int value) &&;
        long plain_add(int value);
        long peek(int value) const;
        long look(int value) const &;
        long poke(int value) volatile;
        long calm(int value) noexcept;
        static long make(int value);
    };

    struct derived_counter : counter
    {};

    struct private_counter : private counter
    {
        using counter::add;
    };

    struct protected_counter : protected counter
    {
        using counter::add;
    };

    struct overloads
    {
        long choose(int value) &;
        char choose(int && value) &;

        template <typename Type>
        Type echo(Type value) const;
        long echo(int value) const;

        template <typename Type>
        Type gather(Type value) &;
        long gather(int value) &;
        char gather(int && value) &;

        static long shared(int value);
        static char shared(int && value);
    };

    class restricted
    {
    public:
        long retire(int value) & = delete;
        long retire(long value) &;

        long hide(long value) &;

    private:
        long hide(short value) &;
    };

    union variant
    {
        int number;

        int query(int value) &;
    };

    struct empty
    {};

    struct method_traits
    {
        SCL_METHOD_DETECTION(query)
    };

    SCL_METHOD_DETECTION(reset)
    SCL_METHOD_DETECTION(query)
    SCL_METHOD_DETECTION(add)
    SCL_METHOD_DETECTION(take)
    SCL_METHOD_DETECTION(plain_add)
    SCL_METHOD_DETECTION(peek)
    SCL_METHOD_DETECTION(look)
    SCL_METHOD_DETECTION(poke)
    SCL_METHOD_DETECTION(calm)
    SCL_METHOD_DETECTION(make)
    SCL_METHOD_DETECTION(choose)
    SCL_METHOD_DETECTION(echo)
    SCL_METHOD_DETECTION(gather)
    SCL_METHOD_DETECTION(shared)
    SCL_METHOD_DETECTION(retire)
    SCL_METHOD_DETECTION(hide)
} // namespace

TEST(TypeTraitsMethodDetection, CallsOnAnLvalueForAnLvalueReferenceType)
{
    STATIC_EXPECT_TRUE((has_add_method_v<counter &, int>));
    STATIC_EXPECT_FALSE((has_add_method_v<counter, int>));
    STATIC_EXPECT_FALSE((has_add_method_v<counter &&, int>));
}

TEST(TypeTraitsMethodDetection, CallsOnAnRvalueForAClassAndAnRvalueReferenceType)
{
    STATIC_EXPECT_TRUE((has_take_method_v<counter, int>));
    STATIC_EXPECT_TRUE((has_take_method_v<counter &&, int>));
    STATIC_EXPECT_FALSE((has_take_method_v<counter &, int>));
}

TEST(TypeTraitsMethodDetection, GivesTheObjectTheQualifiersOfTheType)
{
    STATIC_EXPECT_FALSE((has_plain_add_method_v<counter const, int>));
    STATIC_EXPECT_TRUE((has_peek_method_v<counter const, int>));
    STATIC_EXPECT_FALSE((has_peek_method_v<counter volatile, int>));
    STATIC_EXPECT_TRUE((has_poke_method_v<counter volatile, int>));
}

TEST(TypeTraitsMethodDetection, IsTrueWhereTheCallCompiles)
{
    STATIC_EXPECT_TRUE(has_reset_method_v<counter>);
    STATIC_EXPECT_TRUE((has_query_method_v<counter, int>));
}

TEST(TypeTraitsMethodDetection, ConvertsTheArgumentsImplicitly)
{
    STATIC_EXPECT_TRUE((has_add_method_v<counter &, short>));
}

TEST(TypeTraitsMethodDetection, IsFalseWhereTheCallDoesNotCompile)
{
    STATIC_EXPECT_FALSE((has_query_method_v<counter, char const *>));
    STATIC_EXPECT_FALSE(has_query_method_v<counter>);
}

TEST(TypeTraitsMethodDetection, CallsAConstMemberFunctionOnAnObjectWithoutConst)
{
    STATIC_EXPECT_TRUE((has_peek_method_v<counter, int>));
    STATIC_EXPECT_TRUE((has_look_method_v<counter &&, int>));
}

TEST(TypeTraitsMethodDetection, ResultIsTheTypeOfTheCall)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<query_method_t<counter, int>, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<reset_method_t<counter>, void>));
}

TEST(TypeTraitsMethodDetection, ResultIsNonesuchWhereTheCallDoesNotCompile)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<query_method_t<counter, char const *>, ::scl::nonesuch>));
}

TEST(TypeTraitsMethodDetection, ExpandsInAClass)
{
    STATIC_EXPECT_TRUE((method_traits::has_query_method_v<counter, int>));
    STATIC_EXPECT_TRUE((method_traits::has_query_method_exact_v<counter, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<method_traits::query_method_t<counter, int>, int>));
}

TEST(TypeTraitsMethodDetection, IsFalseForEveryFormWithoutTheMember)
{
    STATIC_EXPECT_FALSE((has_query_method_v<empty, int>));
    STATIC_EXPECT_FALSE((has_query_method_exact_v<empty, int>));
    STATIC_EXPECT_FALSE((has_query_static_method_v<empty, int>));
    STATIC_EXPECT_FALSE((has_query_static_method_exact_v<empty, int>));
    STATIC_EXPECT_FALSE((has_query_explicit_method_exact_v<empty, int>));
}

TEST(TypeTraitsMethodDetectionExact, IsTrueForExactlyTheParameterTypes)
{
    STATIC_EXPECT_TRUE((has_query_method_exact_v<counter, int>));
    STATIC_EXPECT_TRUE(has_reset_method_exact_v<counter>);
}

TEST(TypeTraitsMethodDetectionExact, IsFalseWhereTheArgumentsOnlyConvert)
{
    STATIC_EXPECT_FALSE((has_add_method_exact_v<counter &, short>));
    STATIC_EXPECT_FALSE((has_reset_method_exact_v<counter, int>));
}

TEST(TypeTraitsMethodDetectionExact, IgnoresTheTopLevelQualifiersOfTheArguments)
{
    STATIC_EXPECT_TRUE((has_add_method_exact_v<counter &, int const>));
    STATIC_EXPECT_TRUE((has_add_method_exact_v<counter &, int volatile>));
    STATIC_EXPECT_TRUE((has_make_static_method_exact_v<counter, int volatile>));
}

TEST(TypeTraitsMethodDetectionExact, FindsAMemberFunctionOfAPublicBase)
{
    STATIC_EXPECT_TRUE((has_add_method_exact_v<derived_counter &, int>));
}

TEST(TypeTraitsMethodDetectionExact, FindsAMemberFunctionOfAUnion)
{
    STATIC_EXPECT_TRUE((has_query_method_exact_v<variant &, int>));
}

TEST(TypeTraitsMethodDetectionExact, IsFalseForAMemberFunctionOfAPrivateOrProtectedBase)
{
    STATIC_EXPECT_TRUE((has_add_method_v<private_counter &, int>));
    STATIC_EXPECT_FALSE((has_add_method_exact_v<private_counter &, int>));
    STATIC_EXPECT_TRUE((has_add_method_v<protected_counter &, int>));
    STATIC_EXPECT_FALSE((has_add_method_exact_v<protected_counter &, int>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsMethodDetectionExact, IsFalseForAPrivateMemberFunction)
{
    STATIC_EXPECT_FALSE((has_hide_method_exact_v<restricted &, short>));
    STATIC_EXPECT_TRUE((has_hide_method_exact_v<restricted &, long>));
}
#endif

TEST(TypeTraitsMethodDetectionExact, IsFalseForADeletedMemberFunction)
{
    STATIC_EXPECT_FALSE((has_retire_method_exact_v<restricted &, int>));
    STATIC_EXPECT_TRUE((has_retire_method_exact_v<restricted &, long>));
}

TEST(TypeTraitsMethodDetectionExact, IsFalseForAStaticMemberFunction)
{
    STATIC_EXPECT_TRUE((has_make_method_v<counter, int>));
    STATIC_EXPECT_FALSE((has_make_method_exact_v<counter, int>));
}

TEST(TypeTraitsMethodDetectionExact, TakesTheRefQualifierFromTheReferenceOfTheType)
{
    STATIC_EXPECT_TRUE((has_add_method_exact_v<counter &, int>));
    STATIC_EXPECT_FALSE((has_add_method_exact_v<counter &&, int>));
    STATIC_EXPECT_TRUE((has_take_method_exact_v<counter &&, int>));
    STATIC_EXPECT_FALSE((has_take_method_exact_v<counter, int>));
    STATIC_EXPECT_TRUE((has_plain_add_method_exact_v<counter, int>));
    STATIC_EXPECT_FALSE((has_plain_add_method_exact_v<counter &, int>));
}

TEST(TypeTraitsMethodDetectionExact, TakesTheCvQualifiersFromTheType)
{
    STATIC_EXPECT_TRUE((has_look_method_exact_v<counter const &, int>));
    STATIC_EXPECT_FALSE((has_look_method_exact_v<counter &, int>));
    STATIC_EXPECT_TRUE((has_poke_method_exact_v<counter volatile, int>));
    STATIC_EXPECT_FALSE((has_poke_method_exact_v<counter, int>));
}

TEST(TypeTraitsMethodDetectionExact, IsFalseForAConstMemberFunctionWithoutTheRefQualifierOfTheType)
{
    STATIC_EXPECT_TRUE((has_peek_method_v<counter const &, int>));
    STATIC_EXPECT_FALSE((has_peek_method_exact_v<counter const &, int>));
    STATIC_EXPECT_TRUE((has_peek_method_exact_v<counter const, int>));
}

TEST(TypeTraitsMethodDetectionExact, ResultIsTheResultTypeOfTheMemberFunctionFound)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<add_method_exact_t<counter &, int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<add_method_exact_t<counter &, short>, ::scl::nonesuch>));
}

TEST(TypeTraitsMethodDetectionExact, FindsANoexceptMemberFunction)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<calm_method_exact_t<counter, int>, long>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsMethodDetectionExact, FindsEachOverloadBesideAnAmbiguousPlainCall)
{
    STATIC_EXPECT_FALSE((has_choose_method_v<overloads &, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<choose_method_exact_t<overloads &, int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<choose_method_exact_t<overloads &, int &&>, char>));
}
#else
TEST(TypeTraitsMethodDetectionExact, IsFalseBesideAnAmbiguousPlainCallOnMsvc)
{
    STATIC_EXPECT_FALSE((has_choose_method_v<overloads &, int>));
    STATIC_EXPECT_FALSE((has_choose_method_exact_v<overloads &, int>));
}
#endif

TEST(TypeTraitsMethodDetectionExact, TakesTheOverloadOfThePlainCallBesideAMemberFunctionTemplate)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<echo_method_exact_t<overloads const, int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<echo_method_exact_t<overloads const, double>, double>));
}

TEST(TypeTraitsMethodDetectionExact, IsFalseBesideAMemberFunctionTemplateWhereThePlainCallIsAmbiguous)
{
    STATIC_EXPECT_FALSE((has_gather_method_v<overloads &, int>));
    STATIC_EXPECT_FALSE((has_gather_method_exact_v<overloads &, int>));
}

TEST(TypeTraitsMethodDetectionStatic, IsTrueWhereTheCallThroughTheClassNameCompiles)
{
    STATIC_EXPECT_TRUE((has_make_static_method_v<counter, int>));
    STATIC_EXPECT_TRUE((has_make_static_method_v<counter, short>));
    STATIC_EXPECT_TRUE((::std::is_same_v<make_static_method_t<counter, int>, long>));
}

TEST(TypeTraitsMethodDetectionStatic, IsFalseForANonStaticMemberFunction)
{
    STATIC_EXPECT_FALSE((has_query_static_method_v<counter, int>));
    STATIC_EXPECT_FALSE((has_query_static_method_exact_v<counter, int>));
}

TEST(TypeTraitsMethodDetectionStatic, ExactIsTrueForExactlyTheParameterTypes)
{
    STATIC_EXPECT_TRUE((has_make_static_method_exact_v<counter, int>));
    STATIC_EXPECT_FALSE((has_make_static_method_exact_v<counter, short>));
    STATIC_EXPECT_TRUE((::std::is_same_v<make_static_method_exact_t<counter, int>, long>));
}

TEST(TypeTraitsMethodDetectionStatic, IgnoresTheQualifiersOfTheType)
{
    STATIC_EXPECT_TRUE((has_make_static_method_v<counter const &, int>));
    STATIC_EXPECT_TRUE((has_make_static_method_exact_v<counter volatile &&, int>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsMethodDetectionStatic, ExactFindsEachOverloadBesideAnAmbiguousCall)
{
    STATIC_EXPECT_FALSE((has_shared_static_method_v<overloads, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<shared_static_method_exact_t<overloads, int>, long>));
    STATIC_EXPECT_TRUE((::std::is_same_v<shared_static_method_exact_t<overloads, int &&>, char>));
}
#else
TEST(TypeTraitsMethodDetectionStatic, ExactIsFalseBesideAnAmbiguousCallOnMsvc)
{
    STATIC_EXPECT_FALSE((has_shared_static_method_v<overloads, int>));
    STATIC_EXPECT_FALSE((has_shared_static_method_exact_v<overloads, int>));
}
#endif

TEST(TypeTraitsMethodDetectionExplicit, IsFalseForAMemberFunctionWithAnImplicitObjectParameter)
{
    STATIC_EXPECT_TRUE((has_look_method_exact_v<counter const &, int>));
    STATIC_EXPECT_FALSE((has_look_explicit_method_exact_v<counter const &, int>));
}

TEST(TypeTraitsMethodDetectionExplicit, IsFalseForAStaticMemberFunction)
{
    STATIC_EXPECT_FALSE((has_make_explicit_method_exact_v<counter, int>));
}

namespace
{
    struct static_pick
    {
        static int pick(static_pick const & self, long value);
    };

    struct static_beside_implicit_pick
    {
        static int pick(static_beside_implicit_pick const & self, long value);
        int pick(long value) const;
    };

    struct ambiguous_static_pick
    {
        static int pick(ambiguous_static_pick self, long value);
        static int pick(ambiguous_static_pick const & self, long value);
    };

    SCL_METHOD_DETECTION(pick)
} // namespace

TEST(TypeTraitsMethodDetectionExplicit, IsFalseBesideAStaticMemberFunctionTakingTheObject)
{
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<static_pick const &, long>));
    STATIC_EXPECT_TRUE((has_pick_method_v<static_beside_implicit_pick const &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<static_beside_implicit_pick const &, long>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsMethodDetectionExplicit, IsTrueBesideStaticMemberFunctionsWhoseCallIsAmbiguous)
{
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<ambiguous_static_pick const &, long>));
}
#else
TEST(TypeTraitsMethodDetectionExplicit, IsFalseBesideStaticMemberFunctionsWhoseCallIsAmbiguousOnMsvc)
{
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<ambiguous_static_pick const &, long>));
}
#endif

#ifdef __cpp_explicit_this_parameter
namespace
{
    struct explicit_pick
    {
        int pick(this explicit_pick const & self, long value);
    };

    struct derived_explicit_pick : explicit_pick
    {};

    struct explicit_value_pick
    {
        int pick(this explicit_value_pick const self, long value);
    };

    struct explicit_template_pick
    {
        template <typename Self>
        int pick(this Self && self, long value);
    };

    struct derived_template_pick : explicit_template_pick
    {};

    struct explicit_overloaded_pick
    {
        int pick(this explicit_overloaded_pick const & self, long value);
        double pick(this explicit_overloaded_pick & self, long value);
    };

    struct deleted_explicit_pick
    {
        int pick(this deleted_explicit_pick const & self, long value) = delete;
    };

} // namespace

TEST(TypeTraitsMethodDetectionExplicit, IsTrueForExactlyTheTypeOfTheObjectParameter)
{
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_pick const &, long>));
    STATIC_EXPECT_TRUE((has_pick_method_v<explicit_pick &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<explicit_pick &, long>));
}

TEST(TypeTraitsMethodDetectionExplicit, IsFalseWhereTheArgumentsOnlyConvert)
{
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<explicit_pick const &, int>));
}

TEST(TypeTraitsMethodDetectionExplicit, ResultIsTheResultTypeOfTheMemberFunctionFound)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<pick_explicit_method_exact_t<explicit_pick const &, long>, int>));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<pick_explicit_method_exact_t<explicit_overloaded_pick const &, long>, int>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<pick_explicit_method_exact_t<explicit_overloaded_pick &, long>, double>));
}

TEST(TypeTraitsMethodDetectionExplicit, IsFalseForTheObjectParameterOfABase)
{
    STATIC_EXPECT_TRUE((has_pick_method_v<derived_explicit_pick const &, long>));
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<derived_explicit_pick const &, long>));
}

TEST(TypeTraitsMethodDetectionExplicit, DeducesTheObjectParameterOfATemplate)
{
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_template_pick &, long>));
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_template_pick const &&, long>));
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<derived_template_pick const &, long>));
}

TEST(TypeTraitsMethodDetectionExplicit, IgnoresTheTopLevelQualifiersOfAnObjectParameterByValue)
{
    STATIC_EXPECT_TRUE((has_pick_explicit_method_exact_v<explicit_value_pick, long>));
}

TEST(TypeTraitsMethodDetectionExplicit, IsFalseForADeletedMemberFunction)
{
    STATIC_EXPECT_FALSE((has_pick_explicit_method_exact_v<deleted_explicit_pick const &, long>));
}

TEST(TypeTraitsMethodDetectionExact, IsFalseForAMemberFunctionWithAnExplicitObjectParameter)
{
    STATIC_EXPECT_FALSE((has_pick_method_exact_v<explicit_pick const &, long>));
}

#if !defined(_MSC_VER) || defined(__clang__)
TEST(TypeTraitsMethodDetectionStatic, ExactIsTrueForAnExplicitObjectParameterAmongTheArguments)
{
    STATIC_EXPECT_FALSE((has_pick_static_method_v<explicit_pick, explicit_pick const &, long>));
    STATIC_EXPECT_TRUE((has_pick_static_method_exact_v<explicit_pick, explicit_pick const &, long>));
}
#else
TEST(TypeTraitsMethodDetectionStatic, ExactIsFalseForAnExplicitObjectParameterOnMsvc)
{
    STATIC_EXPECT_FALSE((has_pick_static_method_exact_v<explicit_pick, explicit_pick const &, long>));
}
#endif
#endif
