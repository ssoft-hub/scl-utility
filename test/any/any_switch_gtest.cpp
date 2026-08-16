#include <gtest_utils.h>

#include <scl/utility/any.h>

#include <any>
#include <optional>
#include <stdexcept>
#include <string>

namespace
{
    struct counted
    {
        inline static int copies = 0;

        int id = 0;

        constexpr counted() = default;
        constexpr explicit counted(int value) noexcept
            : id{value}
        {}
        // Not constexpr: counting a copy touches a static, which GCC reports as a
        // constexpr function that can never be evaluated as one.
        counted(counted const & other) noexcept
            : id{other.id}
        {
            ++copies;
        }
        constexpr counted(counted &&) noexcept = default;
        constexpr counted & operator=(counted const &) = default;
        constexpr counted & operator=(counted &&) = default;
        constexpr ~counted() = default;
    };

    // A free function, so a handler that is a plain function pointer gets exercised rather
    // than only type-checked.
    int doubled(int value) { return value * 2; }

    // A chain holds no subject, so it is an ordinary constant: built once here, applied to
    // as many subjects as the tests need, at compile time and at run time alike.
    constexpr auto doubling_matcher =
        ::scl::any_switch<int>().in_case<int>([](int number) { return number * 2; }).or_else(0);

    template <typename Case>
    concept case_after_int =
        requires {
            ::scl::any_switch<>().in_case<int>([](int) {}).template in_case<Case>([](Case) {});
        };

    template <typename Case>
    concept case_after_mutable_int =
        requires {
            ::scl::any_switch<>().in_case<int &>([](int &) {}).template in_case<Case>([](Case) {});
        };

    template <typename Case>
    concept case_after_volatile_int =
        requires {
            ::scl::any_switch<>().in_case<int volatile &>([](int volatile &) {
            }).template in_case<Case>([](Case) {});
        };

    template <typename Case>
    concept case_selectable =
        requires { ::scl::any_switch<>().template in_case<Case>([](auto &&) {}); };

    template <typename Result>
    concept handlerless_case = requires { ::scl::any_switch<Result>().template in_case<int>(); };

    template <typename Handler>
    concept int_handler =
        requires(Handler handler) { ::scl::any_switch<int>().in_case<int>(handler); };

    template <typename Handler>
    concept void_handler = requires(Handler handler) { ::scl::any_switch<>().in_case<int>(handler); };

#if SCL_HAS_RTTI
    // A case naming std::any takes every subject the box itself is, so a case naming a type
    // may not follow one.
    template <typename Case>
    concept case_after_std_any =
        requires {
            ::scl::any_switch<>().in_case<::std::any &>([](::std::any &) {
            }).template in_case<Case>([](Case) {});
        };

    template <typename Case>
    concept case_after_const_std_any =
        requires {
            ::scl::any_switch<>().in_case<::std::any const &>([](::std::any const &) {
            }).template in_case<Case>([](Case) {});
        };

    template <typename Case>
    concept empty_case_after_std_any =
        requires {
            ::scl::any_switch<>().in_case<::std::any const &>([](::std::any const &) {
            }).template in_case<Case>([]() {});
        };

    template <typename Result>
    concept fallback_after_std_any =
        requires {
            ::scl::any_switch<Result>()
                .template in_case<::std::any const &>([](::std::any const &) {
            }).or_else([]() {});
        };
#endif

    template <typename Case>
    concept case_after_fallback =
        requires { ::scl::any_switch<>().or_else([]() {}).template in_case<Case>([]() {}); };

    template <typename Result>
    concept repeated_fallback =
        requires { ::scl::any_switch<Result>().or_else([]() {}).or_else([]() {}); };

    template <typename Case>
    concept repeated_case =
        requires {
            ::scl::any_switch<>().template in_case<Case>([]() {}).template in_case<Case>([]() {});
        };

    template <typename Chain>
    concept const_applicable = requires(Chain const chain, int value) { chain.apply(value); };

    template <typename Target>
    concept chain_convertible_to =
        requires {
            static_cast<Target>(::scl::any_switch<int>().in_case<int>([](int) { return 0; }));
        };
} // namespace

TEST(AnySwitchTest, CompileTimeGuards)
{
    // A covered case is judged by what it matches, not by how it is spelled: a copy and a
    // `const` reference cover the same referents, and both of them cover a mutable
    // reference, which asks for less.
    STATIC_EXPECT_FALSE(case_after_int<int>);
    STATIC_EXPECT_FALSE(case_after_int<int const &>);
    STATIC_EXPECT_FALSE(case_after_int<int &>);
    STATIC_EXPECT_TRUE(case_after_int<double>);
    STATIC_EXPECT_TRUE(case_after_int<int volatile &>);
    STATIC_EXPECT_FALSE(repeated_case<void>);
    STATIC_EXPECT_FALSE(repeated_case<int>);

    // Order decides: the narrower case keeps its referent only by coming first.
    STATIC_EXPECT_TRUE(case_after_mutable_int<int>);
    STATIC_EXPECT_TRUE(case_after_mutable_int<int const &>);
    STATIC_EXPECT_TRUE(case_after_mutable_int<int volatile &>);
    STATIC_EXPECT_FALSE(case_after_mutable_int<int &>);

    // `volatile` covers the same way `const` does, and neither covers the other.
    STATIC_EXPECT_FALSE(case_after_volatile_int<int &>);
    STATIC_EXPECT_TRUE(case_after_volatile_int<int const &>);
    STATIC_EXPECT_TRUE(case_after_volatile_int<int>);

#if SCL_HAS_RTTI
    // A std::any case takes every boxed subject, and a chain cannot tell one from a
    // subject bound directly, so only a wider std::any case or the fallback may follow.
    STATIC_EXPECT_FALSE(case_after_std_any<int>);
    STATIC_EXPECT_FALSE(case_after_std_any<double>);
    STATIC_EXPECT_TRUE(case_after_std_any<::std::any const &>);
    STATIC_EXPECT_FALSE(case_after_const_std_any<::std::any>);
    STATIC_EXPECT_FALSE(empty_case_after_std_any<void>);
    STATIC_EXPECT_TRUE(fallback_after_std_any<void>);
#endif

    // The fallback catches everything, so nothing may follow it and nothing may repeat it.
    STATIC_EXPECT_FALSE(case_after_fallback<int>);
    STATIC_EXPECT_FALSE(case_after_fallback<void>);
    STATIC_EXPECT_FALSE(repeated_fallback<void>);
    STATIC_EXPECT_FALSE(repeated_fallback<::std::string>);

    // Doing nothing is a complete handler only where nothing is the result.
    STATIC_EXPECT_TRUE(handlerless_case<void>);
    STATIC_EXPECT_FALSE(handlerless_case<::std::string>);

    // A case names an object type, an lvalue reference to one, or `void`.
    STATIC_EXPECT_TRUE(case_selectable<int>);
    STATIC_EXPECT_TRUE(case_selectable<int const &>);
    STATIC_EXPECT_FALSE(case_selectable<int &&>);
    STATIC_EXPECT_FALSE(case_selectable<int()>);

    // A handler produces the result, or is that result; anything else is not a handler.
    STATIC_EXPECT_TRUE(int_handler<int (*)(int)>);
    STATIC_EXPECT_TRUE(int_handler<int>);
    STATIC_EXPECT_FALSE(int_handler<::std::string>);
    STATIC_EXPECT_FALSE(int_handler<void (*)(int)>);
    STATIC_EXPECT_FALSE(void_handler<int>);
    STATIC_EXPECT_FALSE(void_handler<void (*)(::std::string)>);

    // A `const` chain applies exactly as far as its handlers are callable on one.
    STATIC_EXPECT_TRUE(const_applicable<decltype(::scl::any_switch<>().in_case<int>([](int) {}))>);
    STATIC_EXPECT_FALSE(const_applicable<decltype(::scl::any_switch<>().in_case<int>([count = 0](int) mutable {
        ++count;
    }))>);

    // The chain is a description; reading it is a named call and nothing else.
    STATIC_EXPECT_FALSE(chain_convertible_to<bool>);
    STATIC_EXPECT_FALSE(chain_convertible_to<int>);
    STATIC_EXPECT_FALSE(chain_convertible_to<::std::optional<int>>);
}

TEST(AnySwitchTest, MatchingCaseRunsItsHandler)
{
    int value = 7;
    int seen = 0;

    ::scl::any_switch<>().in_case<int>([&seen](int number) { seen = number; }).apply(value);

    EXPECT_EQ(seen, 7);
}

TEST(AnySwitchTest, FirstMatchingCaseWins)
{
    int value = 7;
    int chosen = 0;

    ::scl::any_switch<>().in_case<double>([&chosen](double) {
        chosen = 1;
    }).in_case<int &>([&chosen](int &) {
        chosen = 2;
    }).in_case<int const &>([&chosen](int const &) {
        chosen = 3;
    }).apply(value);

    EXPECT_EQ(chosen, 2);
}

TEST(AnySwitchTest, VoidCaseMatchesEmptySubject)
{
    ::scl::any_view const empty;
    bool seen = false;

    ::scl::any_switch<>().in_case<int>([](int) {}).in_case<void>([&seen]() {
        seen = true;
    }).apply(empty);

    EXPECT_TRUE(seen);
}

TEST(AnySwitchTest, VoidCaseDoesNotMatchLiveSubject)
{
    int value = 7;
    bool empty_seen = false;
    bool int_seen = false;

    ::scl::any_switch<>().in_case<void>([&empty_seen]() {
        empty_seen = true;
    }).in_case<int>([&int_seen](int) {
        int_seen = true;
    }).apply(value);

    EXPECT_FALSE(empty_seen);
    EXPECT_TRUE(int_seen);
}

TEST(AnySwitchTest, CaseWithoutHandlerMatchesAndDoesNothing)
{
    char value = 'x';
    bool fallback_fired = false;

    ::scl::any_switch<>().in_case<char>().or_else([&fallback_fired]() {
        fallback_fired = true;
    }).apply(value);

    EXPECT_FALSE(fallback_fired);
}

TEST(AnySwitchTest, ValueBranchNeedsNoInvocable)
{
    int value = 7;

    auto const result =
        ::scl::any_switch<::std::string>()
            .in_case<double>("double")
            .in_case<int>("int")
            .or_else("other")
            .apply(value);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "int");
}

TEST(AnySwitchTest, HasCaseReportsWhetherABranchMatches)
{
    int matching = 7;
    double unmatched = 1.5;

    auto const matcher = ::scl::any_switch<>().in_case<int>([](int) {});

    EXPECT_TRUE(matcher.has_case(matching));
    EXPECT_FALSE(matcher.has_case(unmatched));
}

TEST(AnySwitchTest, HasCaseIsAlwaysTrueWithAFallback)
{
    double unmatched = 1.5;
    ::scl::any_view const empty;

    auto const matcher = ::scl::any_switch<>().in_case<int>([](int) {}).or_else([]() {});

    EXPECT_TRUE(matcher.has_case(unmatched));
    EXPECT_TRUE(matcher.has_case(empty));
}

TEST(AnySwitchTest, HasCaseRunsNoHandler)
{
    int value = 7;
    bool fired = false;

    auto const matcher = ::scl::any_switch<>().in_case<int>([&fired](int) { fired = true; });

    EXPECT_TRUE(matcher.has_case(value));
    EXPECT_FALSE(fired);
}

TEST(AnySwitchTest, NamedResultReturnsWhatTheHandlerProduced)
{
    int value = 7;

    ::std::optional<::std::string> const result =
        ::scl::any_switch<::std::string>().in_case<int>([](int) { return "int"; }).apply(value);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "int");
}

TEST(AnySwitchTest, NamedResultWithoutAMatchIsEmpty)
{
    double value = 1.5;

    auto const result = ::scl::any_switch<::std::string>().in_case<int>([](int) {
        return "int";
    }).apply(value);

    EXPECT_FALSE(result.has_value());
}

TEST(AnySwitchTest, NamedResultTakesTheFallbackWhenNoCaseMatches)
{
    double value = 1.5;

    auto const result = ::scl::any_switch<::std::string>().in_case<int>([](int) {
        return "int";
    }).or_else([](::scl::any_arg) {
        return "other";
    }).apply(value);

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "other");
}

TEST(AnySwitchTest, FallbackCatchesUnmatchedSubject)
{
    double value = 1.5;
    bool seen = false;

    ::scl::any_switch<>().in_case<int>([](int) {}).or_else([&seen](::scl::any_arg) {
        seen = true;
    }).apply(value);

    EXPECT_TRUE(seen);
}

TEST(AnySwitchTest, FallbackReceivesTheSubject)
{
    double value = 1.5;
    double seen = 0.0;

    ::scl::any_switch<>()
        .or_else([&seen](::scl::any_arg argument) {
        seen = ::scl::any_cast<double>(argument);
    }).apply(value);

    EXPECT_EQ(seen, 1.5);
}

TEST(AnySwitchTest, FallbackCatchesEmptySubjectWithoutVoidCase)
{
    ::scl::any_view const empty;
    bool seen = false;

    ::scl::any_switch<>().in_case<int>([](int) {}).or_else([&seen]() { seen = true; }).apply(empty);

    EXPECT_TRUE(seen);
}

TEST(AnySwitchTest, MatchedCaseWinsOverFallback)
{
    int value = 7;
    int chosen = 0;

    ::scl::any_switch<>().in_case<int>([&chosen](int) {
        chosen = 1;
    }).or_else([&chosen]() {
        chosen = 2;
    }).apply(value);

    EXPECT_EQ(chosen, 1);
}

TEST(AnySwitchTest, ZeroArgumentHandlerRuns)
{
    int value = 7;
    bool seen = false;

    ::scl::any_switch<>().in_case<int>([&seen]() { seen = true; }).apply(value);

    EXPECT_TRUE(seen);
}

TEST(AnySwitchTest, HandlerCallableBothWaysReceivesTheMatchedValue)
{
    int const value = 7;

    // A default argument leaves both calls compiling, and the matched value is the one
    // that reaches the handler - the default would be a silently wrong answer.
    auto const defaulted = ::scl::any_switch<int>().in_case<int>([](int number = 42) {
        return number;
    });

    EXPECT_EQ(defaulted.apply(value), 7);

    // A templated call operator takes either arity, so it reports the one it was given.
    auto const generic = ::scl::any_switch<int>().in_case<int>([](auto &&... argument) {
        return static_cast<int>(sizeof...(argument));
    });

    EXPECT_EQ(generic.apply(value), 1);
}

TEST(AnySwitchTest, ChainFoldsDuringConstantEvaluation)
{
    STATIC_EXPECT_TRUE(doubling_matcher.has_case(21));
    STATIC_EXPECT_EQ(doubling_matcher.apply(21).value(), 42);
    STATIC_EXPECT_EQ(doubling_matcher.apply(1.5).value(), 0);
    STATIC_EXPECT_EQ(doubling_matcher(21).value(), 42);
}

TEST(AnySwitchTest, ChainWithNoBranchMatchesNothing)
{
    int value = 7;

    auto const named = ::scl::any_switch<int>();

    EXPECT_FALSE(named.has_case(value));
    EXPECT_FALSE(named.apply(value).has_value());

    ::scl::any_switch<>().apply(value); // a void chain with no branch simply does nothing
}

TEST(AnySwitchTest, FunctionPointerHandlerRuns)
{
    int value = 21;

    auto const matcher = ::scl::any_switch<int>().in_case<int>(&doubled);

    EXPECT_EQ(*matcher.apply(value), 42);
}

TEST(AnySwitchTest, ResultThatIsItselfAnOptionalKeepsItsMeaning)
{
    int value = 7;
    double other = 1.5;

    auto const matcher = ::scl::any_switch<::std::optional<int>>().in_case<int>([](int number) {
        return ::std::optional<int>{number};
    });

    // The outer optional says a branch ran, the inner one is the result it produced.
    auto const matched = matcher.apply(value);
    ASSERT_TRUE(matched.has_value());
    EXPECT_EQ(matched->value(), 7);

    EXPECT_FALSE(matcher.apply(other).has_value());
}

TEST(AnySwitchTest, ChainsNestMoreThanOneLevelDeep)
{
    int number = 21;
    double ratio = 1.5;
    char symbol = 'x';

    auto const innermost = ::scl::any_switch<int>().in_case<int>(&doubled);
    auto const middle = ::scl::any_switch<int>().in_case<double>(1).or_else(innermost);
    auto const outermost = ::scl::any_switch<int>().in_case<char>(2).or_else(middle);

    EXPECT_EQ(*outermost.apply(number), 42);
    EXPECT_EQ(*outermost.apply(ratio), 1);
    EXPECT_EQ(*outermost.apply(symbol), 2);
}

TEST(AnySwitchTest, HandlerMayProduceTheOptionalItself)
{
    int number = 7;
    double other = 1.5;

    auto const matcher =
        ::scl::any_switch<::std::string>()
            .in_case<int>([](int value) {
        return ::std::optional<::std::string>{::std::to_string(value)};
    }).in_case<double>([](double) { return ::std::optional<::std::string>{}; });

    EXPECT_EQ(*matcher.apply(number), "7");

    // The branch ran and produced nothing, which is the empty optional it returned.
    EXPECT_FALSE(matcher.apply(other).has_value());
}

TEST(AnySwitchTest, ValueBranchMayBeTheOptionalItself)
{
    int number = 7;
    char symbol = 'x';
    double other = 1.5;

    auto const matcher =
        ::scl::any_switch<::std::string>()
            .in_case<int>(::std::optional<::std::string>{"int"})
            .in_case<char>(::std::optional<::std::string>{})
            .or_else(::std::optional<::std::string>{"other"});

    EXPECT_EQ(*matcher.apply(number), "int");
    EXPECT_FALSE(matcher.apply(symbol).has_value()); // the branch ran and produced nothing
    EXPECT_EQ(*matcher.apply(other), "other");
}

TEST(AnySwitchTest, ChainNestsAsTheFallbackOfAnotherChain)
{
    int number = 7;
    double ratio = 1.5;
    char symbol = 'x';

    auto const inner = ::scl::any_switch<::std::string>().in_case<double>("double");
    auto const outer = ::scl::any_switch<::std::string>().in_case<int>("int").or_else(inner);

    EXPECT_EQ(*outer.apply(number), "int");
    EXPECT_EQ(*outer.apply(ratio), "double");

    // Nothing in either chain matches, and the outer result is empty rather than wrong.
    EXPECT_FALSE(outer.apply(symbol).has_value());

    // has_case answers for this chain alone: the fallback covers everything, whatever
    // the chain behind it then decides.
    EXPECT_TRUE(outer.has_case(symbol));
}

TEST(AnySwitchTest, ChainNestsAsTheHandlerOfACase)
{
    int number = 7;

    auto const inner = ::scl::any_switch<::std::string>().in_case<int>("inner int");
    auto const outer = ::scl::any_switch<::std::string>().in_case<int>(inner).or_else("other");

    EXPECT_EQ(*outer.apply(number), "inner int");
}

TEST(AnySwitchTest, CallOperatorRunsTheSameBranchAsApply)
{
    int number = 7;
    double other = 1.5;

    auto const matcher = ::scl::any_switch<int>().in_case<int>([](int value) { return value * 2; });

    EXPECT_EQ(matcher(number), matcher.apply(number));
    EXPECT_EQ(*matcher(number), 14);
    EXPECT_FALSE(matcher(other).has_value());
}

TEST(AnySwitchTest, CallOperatorOnAVoidChainRunsTheHandler)
{
    int value = 7;
    int seen = 0;

    ::scl::any_switch<>().in_case<int>([&seen](int number) { seen = number; })(value);

    EXPECT_EQ(seen, 7);
}

TEST(AnySwitchTest, CallOperatorReachesAMutableHandler)
{
    int value = 7;

    auto matcher = ::scl::any_switch<int>().in_case<int>([count = 0](int) mutable { return ++count; });

    EXPECT_EQ(*matcher(value), 1);
    EXPECT_EQ(*matcher(value), 2);
}

TEST(AnySwitchTest, CallOperatorCarriesTheNoexceptOfApply)
{
    int value = 7;

    auto const nothrow_matcher = ::scl::any_switch<>().in_case<int>([](int) noexcept {});
    auto const throwing_matcher = ::scl::any_switch<>().in_case<int>([](int) {});

    STATIC_EXPECT_TRUE(noexcept(nothrow_matcher(value)));
    STATIC_EXPECT_FALSE(noexcept(throwing_matcher(value)));
}

TEST(AnySwitchTest, NoexceptFollowsTheBranches)
{
    int value = 7;

    auto const nothrow_matcher = ::scl::any_switch<>().in_case<int>([](int) noexcept {});
    auto const throwing_matcher = ::scl::any_switch<>().in_case<int>([](int) {});

    STATIC_EXPECT_TRUE(noexcept(nothrow_matcher.has_case(value)));
    STATIC_EXPECT_TRUE(noexcept(throwing_matcher.has_case(value)));
    STATIC_EXPECT_TRUE(noexcept(nothrow_matcher.apply(value)));
    STATIC_EXPECT_FALSE(noexcept(throwing_matcher.apply(value)));
}

TEST(AnySwitchTest, MutableHandlerRunsOnANonConstChain)
{
    int value = 7;

    auto matcher = ::scl::any_switch<int>().in_case<int>([count = 0](int) mutable { return ++count; });

    EXPECT_EQ(*matcher.apply(value), 1);
    EXPECT_EQ(*matcher.apply(value), 2);
}

TEST(AnySwitchTest, MutableReferenceCaseWritesThrough)
{
    int value = 7;

    ::scl::any_switch<>().in_case<int &>([](int & number) { number = 9; }).apply(value);

    EXPECT_EQ(value, 9);
}

TEST(AnySwitchTest, ConstReferentRefusesAMutableReferenceCase)
{
    int const value = 7;
    int chosen = 0;

    ::scl::any_switch<>().in_case<int &>([&chosen](int &) {
        chosen = 1;
    }).in_case<int const &>([&chosen](int const &) {
        chosen = 2;
    }).apply(value);

    EXPECT_EQ(chosen, 2);
}

TEST(AnySwitchTest, VolatileReferentNeedsAVolatileCase)
{
    int volatile value = 7;
    int chosen = 0;

    ::scl::any_switch<>().in_case<int const &>([&chosen](int const &) {
        chosen = 1;
    }).in_case<int const volatile &>([&chosen](int const volatile &) {
        chosen = 2;
    }).apply(value);

    EXPECT_EQ(chosen, 2);
}

TEST(AnySwitchTest, ConstReferenceCaseCopiesNothing)
{
    counted value{7};
    counted const * seen = nullptr;
    counted::copies = 0;

    ::scl::any_switch<>()
        .in_case<counted const &>([&seen](counted const & object) {
        seen = &object;
    }).apply(value);

    EXPECT_EQ(seen, &value);
    EXPECT_EQ(counted::copies, 0);
}

TEST(AnySwitchTest, ObjectCaseCopiesOnceIntoAByValueHandler)
{
    counted value{7};
    int seen = 0;
    counted::copies = 0;

    ::scl::any_switch<>().in_case<counted>([&seen](counted object) { seen = object.id; }).apply(value);

    EXPECT_EQ(seen, 7);
    EXPECT_EQ(counted::copies, 1);
}

TEST(AnySwitchTest, BuildingTheChainRunsNothing)
{
    bool fired = false;

    auto const matcher = ::scl::any_switch<>().in_case<int>([&fired](int) {
        fired = true;
    }).or_else([&fired]() { fired = true; });

    EXPECT_FALSE(fired);
    static_cast<void>(matcher);
}

TEST(AnySwitchTest, OneChainServesSeveralSubjects)
{
    int number = 7;
    ::std::string text{"hello"};
    double other = 1.5;

    auto const matcher =
        ::scl::any_switch<::std::string>()
            .in_case<int>("int")
            .in_case<::std::string const &>([](::std::string const & value) {
        return value;
    }).or_else("other");

    EXPECT_EQ(*matcher.apply(number), "int");
    EXPECT_EQ(*matcher.apply(text), "hello");
    EXPECT_EQ(*matcher.apply(other), "other");
}

TEST(AnySwitchTest, ApplyRunsTheHandlerOncePerCall)
{
    int value = 7;
    int runs = 0;

    auto const matcher = ::scl::any_switch<>().in_case<int>([&runs](int) { ++runs; });

    matcher.apply(value);
    matcher.apply(value);

    EXPECT_EQ(runs, 2);
}

TEST(AnySwitchTest, HandlerExceptionEscapesApply)
{
    int value = 7;

    auto const matcher = ::scl::any_switch<>().in_case<int>([](int) {
        throw ::std::runtime_error{"boom"};
    });

    EXPECT_THROW(matcher.apply(value), ::std::runtime_error);
}

TEST(AnySwitchTest, SubjectMayBeATemporary)
{
    int seen = 0;

    ::scl::any_switch<>().in_case<int>([&seen](int number) { seen = number; }).apply(7);

    EXPECT_EQ(seen, 7);
}

TEST(AnySwitchTest, SubjectMayBeAView)
{
    ::std::string text{"hello"};
    ::scl::any_view const view{text};
    ::std::string seen;

    ::scl::any_switch<>()
        .in_case<::std::string const &>([&seen](::std::string const & value) {
        seen = value;
    }).apply(view);

    EXPECT_EQ(seen, "hello");
}

TEST(AnySwitchTest, ViewSubjectRefusesAMutableReferenceCase)
{
    int value = 7;
    ::scl::any_view const view{value};
    int chosen = 0;

    ::scl::any_switch<>().in_case<int &>([&chosen](int &) {
        chosen = 1;
    }).in_case<int const &>([&chosen](int const &) {
        chosen = 2;
    }).apply(view);

    EXPECT_EQ(chosen, 2);
}

#if SCL_HAS_RTTI
TEST(AnySwitchTest, StdAnySubjectIsUnwrapped)
{
    ::std::any boxed{::std::string{"hello"}};
    ::std::string seen;

    ::scl::any_switch<>()
        .in_case<::std::string const &>([&seen](::std::string const & value) {
        seen = value;
    }).apply(boxed);

    EXPECT_EQ(seen, "hello");
}

TEST(AnySwitchTest, StdAnySubjectMatchesTheStdAnyCase)
{
    ::std::any boxed{::std::string{"hello"}};
    bool matched = false;

    ::scl::any_switch<>()
        .in_case<::std::any const &>([&matched](::std::any const & box) {
        matched = ::std::any_cast<::std::string>(&box) != nullptr;
    }).or_else([]() {
        FAIL() << "the case on the box took nothing";
    }).apply(boxed);

    EXPECT_TRUE(matched);
}

TEST(AnySwitchTest, EmptyStdAnyMatchesTheVoidCase)
{
    ::std::any const empty;
    bool seen = false;

    ::scl::any_switch<>().in_case<void>([&seen]() { seen = true; }).apply(empty);

    EXPECT_TRUE(seen);
}
#endif
