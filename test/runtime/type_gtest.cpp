#include <gtest_utils.h>

#include <memory>
#include <string>

// Declares ::scl without RTTI, where the concepts below would otherwise name nothing.
#include <scl/utility/meta/type.h>
#include <scl/utility/runtime/type.h>

namespace
{
    template <typename T>
    concept runtime_type_name_declared = requires(T const & object) { ::scl::type_name(object); };

    template <typename T>
    concept runtime_type_short_name_declared =
        requires(T const & object) { ::scl::type_short_name(object); };
} // namespace

#if SCL_HAS_RTTI

/**
 * @test Verify that both functions are declared where RTTI is enabled.
 */
TEST(RuntimeTypeTest, DeclaredWithRtti)
{
    STATIC_EXPECT_TRUE(runtime_type_name_declared<int>);
    STATIC_EXPECT_TRUE(runtime_type_short_name_declared<int>);
}

struct SimpleStruct
{};

namespace ns
{
    struct NamespacedType
    {};

    template <typename T>
    struct TemplateType
    {};
} // namespace ns

struct PolymorphicBase
{
    virtual ~PolymorphicBase() = default;
};

struct PolymorphicDerived : PolymorphicBase
{};

struct FurtherDerived : PolymorphicDerived
{};

/**
 * @test Verify that type_name returns a name containing the expected identifier for fundamental types.
 */
TEST(TypeNameTest, FundamentalType)
{
    int i = 0;
    double d = 0.0;
    EXPECT_NE(::scl::type_name(i).find("int"), ::std::string::npos);
    EXPECT_NE(::scl::type_name(d).find("double"), ::std::string::npos);
}

/**
 * @test Verify that type_name returns a name containing the type identifier for user-defined types.
 */
TEST(TypeNameTest, UserDefinedType)
{
    SimpleStruct s;
    EXPECT_NE(::scl::type_name(s).find("SimpleStruct"), ::std::string::npos);
}

/**
 * @test Verify that type_name returns the dynamic type name when called through a base pointer.
 */
TEST(TypeNameTest, Polymorphism)
{
    ::std::unique_ptr<PolymorphicBase> p = ::std::make_unique<PolymorphicDerived>();
    EXPECT_NE(::scl::type_name(*p).find("PolymorphicDerived"), ::std::string::npos);
}

/**
 * @test Verify that type_name includes both the type name and template argument for template types.
 */
TEST(TypeNameTest, TemplateType)
{
    ns::TemplateType<int> t;
    EXPECT_NE(::scl::type_name(t).find("TemplateType"), ::std::string::npos);
    EXPECT_NE(::scl::type_name(t).find("int"), ::std::string::npos);
}

/**
 * @test Verify that type_short_name strips namespace qualifiers.
 */
TEST(TypeShortNameTest, Namespaced)
{
    ns::NamespacedType t;
    EXPECT_EQ(::scl::type_short_name(t), "NamespacedType");
}

/**
 * @test Verify that type_short_name strips both namespace qualifiers and template arguments.
 */
TEST(TypeShortNameTest, Template)
{
    ns::TemplateType<int> t;
    EXPECT_EQ(::scl::type_short_name(t), "TemplateType");
}

/**
 * @test Verify that type_short_name returns the dynamic short name when called through a base pointer.
 */
TEST(TypeShortNameTest, Polymorphism)
{
    ::std::unique_ptr<PolymorphicBase> p = ::std::make_unique<FurtherDerived>();
    EXPECT_EQ(::scl::type_short_name(*p), "FurtherDerived");
}

/**
 * @test Verify that type_short_name gives a closure type a non-empty ending of its full name with
 *       no scope qualifier.
 */
TEST(TypeShortNameTest, Closure)
{
    auto const closure = [](int number) { return number; };
    auto const name = ::scl::type_short_name(closure);
    EXPECT_FALSE(name.empty());
    EXPECT_TRUE(::scl::type_name(closure).ends_with(name));
    EXPECT_EQ(name.find("::"), ::std::string::npos);
}

namespace
{
    struct
    {
        int value;
    } const unnamed_object{};

    enum
    {
        unnamed_first
    } const unnamed_value{};
} // namespace

/**
 * @test Verify that type_short_name gives an unnamed class or enumeration a name with no scope
 *       qualifier.
 */
TEST(TypeShortNameTest, UnnamedType)
{
    auto const object_name = ::scl::type_short_name(unnamed_object);
    auto const value_name = ::scl::type_short_name(unnamed_value);
    EXPECT_FALSE(object_name.empty());
    EXPECT_TRUE(::scl::type_name(unnamed_object).ends_with(object_name));
    EXPECT_EQ(object_name.find("::"), ::std::string::npos);
    EXPECT_FALSE(value_name.empty());
    EXPECT_TRUE(::scl::type_name(unnamed_value).ends_with(value_name));
    EXPECT_EQ(value_name.find("::"), ::std::string::npos);
}

namespace
{
    struct Ordered
    {};

    auto operator<(Ordered, Ordered)
    {
        return [](int number) { return number; };
    }

    auto operator<=(Ordered, Ordered)
    {
        struct Local
        {};
        return Local{};
    }

    int operator-(Ordered, Ordered) { return 0; }

    int operator>(Ordered, Ordered) { return 0; }

    template <auto Function>
    struct Holder
    {};

    template <typename T>
    struct Box
    {};

    template <typename T>
    struct cooperator
    {};
} // namespace

/**
 * @test Verify that the symbol of an enclosing operator does not cut the short name of a closure
 *       or of a local class.
 */
TEST(TypeShortNameTest, TypeInOperator)
{
    auto const closure = Ordered{} < Ordered{};
    auto const closure_name = ::scl::type_short_name(closure);
    EXPECT_FALSE(closure_name.empty());
    EXPECT_TRUE(::scl::type_name(closure).ends_with(closure_name));
    EXPECT_EQ(closure_name.find("::"), ::std::string::npos);
    EXPECT_EQ(::scl::type_short_name(Ordered{} <= Ordered{}), "Local");
}

/**
 * @test Verify that the symbol of an operator in a template argument does not hide the last scope
 *       operator.
 */
TEST(TypeShortNameTest, OperatorInTemplateArgument)
{
    EXPECT_EQ(::scl::type_short_name(Holder<(&operator<)>{}), "Holder");
    EXPECT_EQ(::scl::type_short_name(Holder<(&operator-)>{}), "Holder");
    EXPECT_EQ(::scl::type_short_name(Holder<(&operator<=)>{}), "Holder");
    EXPECT_EQ(::scl::type_short_name(Holder<(&operator>)>{}), "Holder");
    EXPECT_EQ(::scl::type_short_name(Box<decltype(Ordered{} < Ordered{})>{}), "Box");
    EXPECT_EQ(::scl::type_short_name(Box<decltype(Ordered{} <= Ordered{})>{}), "Box");
    EXPECT_EQ(::scl::type_short_name(Box<cooperator<int>>{}), "Box");
}

/**
 * @test Verify that a qualified parameter type of a closure does not cut its short name.
 */
TEST(TypeShortNameTest, ClosureWithQualifiedParameter)
{
    auto const closure = [](::std::string const & text) { return text.size(); };
    auto const name = ::scl::type_short_name(closure);
    EXPECT_FALSE(name.empty());
    EXPECT_TRUE(::scl::type_name(closure).ends_with(name));
}

#else

/**
 * @test Verify that neither function is declared where RTTI is disabled.
 */
TEST(RuntimeTypeTest, AbsentWithoutRtti)
{
    STATIC_EXPECT_FALSE(runtime_type_name_declared<int>);
    STATIC_EXPECT_FALSE(runtime_type_short_name_declared<int>);
}

#endif // SCL_HAS_RTTI
