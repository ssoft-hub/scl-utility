#include <gtest_utils.h>

#include <scl/utility/preprocessor/counter.h>

struct WithoutDefaultConstructor
{
    explicit WithoutDefaultConstructor(int) {}
};

struct Incomplete;

struct Abstract
{
    virtual ~Abstract() = default;
    virtual void act() = 0;
};

using Reference = int &;

struct Plain
{};

using PlainConst = Plain const;
using Ints = int[3];
using IntPointer = int *;
using Function = void(int);

template <typename First, typename Second>
struct Pair
{};

namespace scl
{
    struct Probe
    {};
} // namespace scl

struct Probe
{};

namespace detail
{
    struct Id
    {};
} // namespace detail

struct counter
{};

struct Shadowed
{};

namespace caller
{
    struct Shadowed
    {};
} // namespace caller

SCL_COUNTER_NEXT(WithoutDefaultConstructor)
SCL_COUNTER_NEXT(Incomplete)
SCL_COUNTER_NEXT(Abstract)
SCL_COUNTER_NEXT(Reference)
SCL_COUNTER_NEXT(void)
SCL_COUNTER_NEXT(PlainConst)
SCL_COUNTER_NEXT(Ints)
SCL_COUNTER_NEXT(Function)
SCL_COUNTER_NEXT(unsigned int)
SCL_COUNTER_NEXT(Pair<int, char>)
SCL_COUNTER_NEXT(Pair<char, int>)
SCL_COUNTER_NEXT(::detail::Id)
SCL_COUNTER_NEXT(::counter)
SCL_COUNTER_NEXT(::Pair<::detail::Id, int>)
SCL_COUNTER_NEXT(::Probe)
SCL_COUNTER_NEXT(Shadowed)
SCL_COUNTER_NEXT(SCL_FORWARD(Pair<char, int>))

/**
 * @test Verify that a class without a default constructor is a tag of its own counter.
 */
TEST(CounterTag, ClassWithoutDefaultConstructor)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(WithoutDefaultConstructor), 1U);
}

/**
 * @test Verify that a class only declared so far is a tag of its own counter.
 */
TEST(CounterTag, IncompleteClass) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Incomplete), 1U); }

/**
 * @test Verify that an abstract class is a tag of its own counter.
 */
TEST(CounterTag, AbstractClass) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Abstract), 1U); }

/**
 * @test Verify that a reference is a tag of its own counter, apart from the type it refers to.
 */
TEST(CounterTag, Reference)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Reference), 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(int), 0U);
}

/**
 * @test Verify that void is a tag of its own counter.
 */
TEST(CounterTag, Void) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(void), 1U); }

/**
 * @test Verify that a const type is a tag of its own counter, apart from the type without const.
 */
TEST(CounterTag, ConstType)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(PlainConst), 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Plain), 0U);
}

/**
 * @test Verify that an array is a tag of its own counter, apart from a pointer to its element.
 */
TEST(CounterTag, Array)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Ints), 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(IntPointer), 0U);
}

/**
 * @test Verify that a function type is a tag of its own counter.
 */
TEST(CounterTag, FunctionType) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Function), 1U); }

/**
 * @test Verify that a type written in several words is a tag of its own counter.
 */
TEST(CounterTag, TypeOfSeveralWords) { STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(unsigned int), 1U); }

/**
 * @test Verify that a tag whose name holds a comma is written as is, apart from another
 *       specialization of the same template.
 */
TEST(CounterTag, NameWithComma)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Pair<int, char>), 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Pair<int, long>), 0U);
}

/**
 * @test Verify that a tag wrapped in SCL_FORWARD shares the counter of the same tag written as is.
 */
TEST(CounterTag, NameWrappedInForward)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(Pair<char, int>), 2U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(SCL_FORWARD(Pair<char, int>)), 2U);
}

/**
 * @test Verify that a tag whose name the library declares as well counts when written from ::.
 */
TEST(CounterTag, NameOfLibraryWrittenFromGlobal)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::detail::Id), 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::counter), 1U);
}

/**
 * @test Verify that a name of the library inside a template argument counts when written from ::.
 */
TEST(CounterTag, NameOfLibraryInTemplateArgument)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::Pair<::detail::Id, int>), 1U);
}

/**
 * @test Verify that a class named as a class of the namespace scl, written from ::, advances its
 *       own counter.
 */
TEST(CounterTag, NameOfNamespaceSclWrittenFromGlobal)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::Probe), 1U);
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::scl::Probe), 0U);
}

namespace caller
{
    inline constexpr auto own_count = SCL_COUNTER_VALUE(Shadowed);
    inline constexpr auto global_count = SCL_COUNTER_VALUE(::Shadowed);
} // namespace caller

/**
 * @test Verify that a tag written from :: inside a namespace that declares the same name reads
 *       the counter advanced at the global namespace scope.
 */
TEST(CounterTag, NameShadowedInNamespaceWrittenFromGlobal)
{
    STATIC_EXPECT_EQ(caller::global_count, 1U);
    STATIC_EXPECT_EQ(caller::own_count, 0U);
}

struct Holder
{
    struct Shadowed
    {};

    static constexpr auto own_count = SCL_COUNTER_VALUE(Shadowed);
    static constexpr auto global_count = SCL_COUNTER_VALUE(::Shadowed);
};

/**
 * @test Verify that a tag written from :: inside a class that declares the same name reads
 *       the counter advanced at the global namespace scope.
 */
TEST(CounterTag, NameShadowedInClassWrittenFromGlobal)
{
    STATIC_EXPECT_EQ(Holder::global_count, 1U);
    STATIC_EXPECT_EQ(Holder::own_count, 0U);
}

struct Ticket
{};

template <typename Tag>
constexpr auto count_of()
{
    return SCL_COUNTER_VALUE(Tag);
}

template <typename Unused>
constexpr auto count_of_ticket()
{
    return SCL_COUNTER_VALUE(::Ticket);
}

template <typename Tag>
struct Reader
{
    static auto count() { return SCL_COUNTER_VALUE(Tag); }
};

SCL_COUNTER_NEXT(::Ticket)

/**
 * @test Verify that a read inside a template answers the count at the definition of the template,
 *       for a tag that depends on a parameter of the template and for one that does not.
 */
TEST(CounterTag, ReadInsideTemplateAnswersCountAtDefinition)
{
    STATIC_EXPECT_EQ(SCL_COUNTER_VALUE(::Ticket), 1U);
    STATIC_EXPECT_EQ(count_of<::Ticket>(), 0U);
    STATIC_EXPECT_EQ(count_of_ticket<int>(), 0U);
    EXPECT_EQ(Reader<::Ticket>::count(), 0U);
}

template <typename Tag>
constexpr auto count_after()
{
    return SCL_COUNTER_VALUE(Tag);
}

/**
 * @test Verify that a template defined after an advance reads the advanced count.
 */
TEST(CounterTag, ReadInsideTemplateDefinedAfterAdvance)
{
    STATIC_EXPECT_EQ(count_after<::Ticket>(), 1U);
}
