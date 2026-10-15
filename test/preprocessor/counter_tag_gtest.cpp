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

SCL_COUNTER_NEXT(WithoutDefaultConstructor)
SCL_COUNTER_NEXT(Incomplete)
SCL_COUNTER_NEXT(Abstract)
SCL_COUNTER_NEXT(Reference)
SCL_COUNTER_NEXT(void)
SCL_COUNTER_NEXT(PlainConst)
SCL_COUNTER_NEXT(Ints)
SCL_COUNTER_NEXT(Function)
SCL_COUNTER_NEXT(unsigned int)

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
