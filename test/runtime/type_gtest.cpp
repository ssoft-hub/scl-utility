#include <gtest_utils.h>

#include <memory>
#include <string>
#include <typeinfo>

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

namespace ns
{
    struct NamespacedType
    {};

    struct Outer
    {
        struct Inner
        {};
    };

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

struct PlainBase
{};

struct PlainDerived : PlainBase
{};

/**
 * @test Verify that type_name returns the dynamic type name when called through a base pointer.
 */
TEST(TypeNameTest, Polymorphism)
{
    ::std::unique_ptr<PolymorphicBase> p = ::std::make_unique<PolymorphicDerived>();
    EXPECT_EQ(::scl::type_name(*p), ::scl::type_name(PolymorphicDerived{}));
}

/**
 * @test Verify that type_name names a base class with no virtual function, not the object's class.
 */
TEST(TypeNameTest, NonPolymorphicBase)
{
    PlainDerived const derived;
    PlainBase const & base = derived;
    EXPECT_EQ(::scl::type_name(base), ::scl::type_name(PlainBase{}));
}

/**
 * @test Verify that type_name demangles where <cxxabi.h> exists and returns typeid's name elsewhere.
 */
TEST(TypeNameTest, Spelling)
{
#if __has_include(<cxxabi.h>)
    EXPECT_EQ(::scl::type_name(0), "int");
    EXPECT_EQ(::scl::type_name(ns::NamespacedType{}), "ns::NamespacedType");
    EXPECT_EQ(::scl::type_name(ns::TemplateType<int>{}), "ns::TemplateType<int>");
#else
    EXPECT_EQ(::scl::type_name(0), typeid(int).name());
    EXPECT_EQ(::scl::type_name(ns::NamespacedType{}), typeid(ns::NamespacedType).name());
    EXPECT_EQ(::scl::type_name(ns::TemplateType<int>{}), typeid(ns::TemplateType<int>).name());
#endif
}

/**
 * @test Verify that scl::detail::demangle returns a name it cannot demangle unchanged.
 */
TEST(TypeNameTest, UndemangledNameUnchanged)
{
    EXPECT_EQ(::scl::detail::demangle("not a mangled name"), "not a mangled name");
}

/**
 * @test Verify that type_short_name strips the qualifiers of namespaces and classes.
 */
TEST(TypeShortNameTest, QualifiersStripped)
{
    EXPECT_EQ(::scl::type_short_name(ns::NamespacedType{}), "NamespacedType");
    EXPECT_EQ(::scl::type_short_name(ns::Outer::Inner{}), "Inner");
}

/**
 * @test Verify that type_short_name names a base class with no virtual function, not the object's class.
 */
TEST(TypeShortNameTest, NonPolymorphicBase)
{
    PlainDerived const derived;
    PlainBase const & base = derived;
    EXPECT_EQ(::scl::type_short_name(base), "PlainBase");
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
 * @test Verify that a qualified parameter type in the name of a closure stays in its short name.
 */
TEST(TypeShortNameTest, ClosureWithQualifiedParameter)
{
    auto const closure = [](::std::string const & text) { return text.size(); };
    auto const holds_parameter = [](::std::string const & name) {
        return name.find("basic_string") != ::std::string::npos;
    };
    auto const balanced = [](::std::string const & name) {
        int depth = 0;
        for (char const ch : name)
        {
            depth += (ch == '(' || ch == '<' || ch == '{') ? 1
                : (ch == ')' || ch == '>' || ch == '}')
                ? -1
                : 0;
            if (depth < 0)
                return false;
        }
        return depth == 0;
    };
    auto const name = ::scl::type_short_name(closure);
    EXPECT_EQ(holds_parameter(name), holds_parameter(::scl::type_name(closure)));
    EXPECT_TRUE(balanced(name));
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
