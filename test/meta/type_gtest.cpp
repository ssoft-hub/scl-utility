#include <gtest_utils.h>

#include <cstddef>
#include <string_view>
#include <vector>

#include <scl/utility/meta/type.h>

/**
 * @brief Simple structure for testing type extraction.
 */
struct SimpleStruct
{};

/**
 * @brief Simple class for testing type extraction and prefix cleanup.
 */
class SimpleClass
{};

/**
 * @brief Namespace for testing nested type names.
 */
namespace Namespace
{
    struct Struct
    {};

    template <typename T>
    struct TemplateStruct
    {};

    template <typename T>
    class TemplateClass
    {};

} // namespace Namespace

/**
 * @brief TU-local type for testing the anonymous-namespace rendering.
 */
namespace
{
    struct AnonStruct
    {};
} // namespace

/**
 * @brief Enumeration for testing the MSVC 'enum ' prefix.
 */
enum class Color
{
    red,
};

/**
 * @brief Union for testing the MSVC 'union ' prefix.
 */
union Number
{
    int whole;
    float fraction;
};

namespace
{
    template <typename T>
    concept meta_type_short_name_declared = requires { ::scl::type_short_name<T>(); };
} // namespace

/**
 * @test Verify fundamental types extraction.
 */
TEST(MetaTypeTest, FundamentalTypes)
{
    EXPECT_EQ(::scl::type_name<int>(), "int");
    EXPECT_EQ(::scl::type_name<float>(), "float");
    EXPECT_EQ(::scl::type_name<double>(), "double");
    EXPECT_EQ(::scl::type_name<void>(), "void");
}

/**
 * @test Verify user-defined types (struct/class).
 * @note On MSVC, type names include struct/class keywords.
 */
TEST(MetaTypeTest, UserDefinedTypes)
{
#if defined(_MSC_VER) && !defined(__clang__)
    EXPECT_EQ(::scl::type_name<SimpleStruct>(), "struct SimpleStruct");
    EXPECT_EQ(::scl::type_name<SimpleClass>(), "class SimpleClass");
#else
    EXPECT_EQ(::scl::type_name<SimpleStruct>(), "SimpleStruct");
    EXPECT_EQ(::scl::type_name<SimpleClass>(), "SimpleClass");
#endif

    static constexpr auto simple_struct_name = ::scl::type_short_name<SimpleStruct>();
    static constexpr auto simple_class_name = ::scl::type_short_name<SimpleClass>();

    EXPECT_EQ(simple_struct_name, "SimpleStruct");
    EXPECT_EQ(simple_class_name, "SimpleClass");
}

/**
 * @test Verify types within custom namespaces.
 */
TEST(MetaTypeTest, NamespacedTypes)
{
#if defined(_MSC_VER) && !defined(__clang__)
    EXPECT_EQ(::scl::type_name<Namespace::Struct>(), "struct Namespace::Struct");
#else
    EXPECT_EQ(::scl::type_name<Namespace::Struct>(), "Namespace::Struct");
#endif

    EXPECT_EQ(::scl::type_short_name<Namespace::Struct>(), "Struct");
}

/**
 * @test Verify template types extraction.
 * @note On MSVC, template arguments include struct/class keywords.
 */
TEST(MetaTypeTest, TemplateTypes)
{
    using T = Namespace::TemplateStruct<Namespace::Struct>;
    using TT = Namespace::TemplateClass<T>;

#if defined(_MSC_VER) && !defined(__clang__)
    EXPECT_EQ(::scl::type_name<T>(), "struct Namespace::TemplateStruct<struct Namespace::Struct>");
    // MSVC adds space before > in nested templates: "> >" instead of ">>"
    EXPECT_EQ(::scl::type_name<TT>(),
        "class Namespace::TemplateClass<struct Namespace::TemplateStruct<struct Namespace::Struct> >");
#elif defined(__clang__)
    EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    EXPECT_EQ(::scl::type_name<TT>(), "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct>>");
#else
    // GCC adds space before > in nested templates
    EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    EXPECT_EQ(::scl::type_name<TT>(), "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct> >");
#endif

    EXPECT_EQ(::scl::type_short_name<T>(), "TemplateStruct");
    EXPECT_EQ(::scl::type_short_name<TT>(), "TemplateClass");
}

/**
 * @test Verify that type_short_name takes a class, a union, an enumeration or a fundamental type,
 *       qualified or referred to.
 */
TEST(MetaTypeTest, ShortNameTakesClassUnionEnumerationOrFundamental)
{
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<SimpleClass>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<Number>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<Color>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<int>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<void>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<::std::nullptr_t>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<SimpleClass const volatile>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<SimpleClass &>);
    STATIC_EXPECT_TRUE(meta_type_short_name_declared<SimpleClass const &&>);
}

/**
 * @test Verify that type_short_name refuses a type spelled with a declarator around a name.
 */
TEST(MetaTypeTest, ShortNameRefusesDeclarator)
{
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass *>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass * const>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass *&>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass[2]>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass[]>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass(SimpleClass)>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<SimpleClass (*)(SimpleClass)>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<int SimpleClass::*>);
    STATIC_EXPECT_FALSE(meta_type_short_name_declared<void (SimpleClass::*)()>);
}

/**
 * @test Verify that type_short_name names a union by its identifier.
 */
TEST(MetaTypeTest, ShortNameOfUnion)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<Number>(), "Number");
}

/**
 * @test Verify that type_short_name drops const, volatile and a reference from the type it names.
 */
TEST(MetaTypeTest, ShortNameDropsQualifiersAndReference)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<SimpleClass const>(), "SimpleClass");
    STATIC_EXPECT_EQ(::scl::type_short_name<SimpleClass volatile>(), "SimpleClass");
    STATIC_EXPECT_EQ(::scl::type_short_name<SimpleClass &>(), "SimpleClass");
    STATIC_EXPECT_EQ(::scl::type_short_name<SimpleClass const &&>(), "SimpleClass");
    STATIC_EXPECT_EQ(::scl::type_short_name<Color const>(), "Color");
    STATIC_EXPECT_EQ(::scl::type_short_name<int const>(), "int");
}

/**
 * @test Verify that type_short_name names a fundamental type as type_name does.
 */
TEST(MetaTypeTest, ShortNameOfFundamental)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<int>(), ::scl::type_name<int>());
    STATIC_EXPECT_EQ(::scl::type_short_name<unsigned long>(), ::scl::type_name<unsigned long>());
    STATIC_EXPECT_EQ(::scl::type_short_name<void>(), "void");
}

/**
 * @test Verify that type_short_name gives a closure type a non-empty ending of its full name.
 */
TEST(MetaTypeTest, ClosureShortName)
{
    auto closure = [](int number) { return number; };
    using closure_type = decltype(closure);
    EXPECT_FALSE(::scl::type_short_name<closure_type>().empty());
    EXPECT_TRUE(::scl::type_name<closure_type>().ends_with(::scl::type_short_name<closure_type>()));
    EXPECT_EQ(::scl::type_short_name<closure_type>().find("::"), ::std::string_view::npos);
}

namespace
{
    [[maybe_unused]] struct
    {
        int value;
    } const unnamed_object{};

    [[maybe_unused]] enum { unnamed_first } const unnamed_value{};
} // namespace

/**
 * @test Verify that type_short_name gives an unnamed class or enumeration a non-empty ending of its
 *       full name.
 */
TEST(MetaTypeTest, UnnamedTypeShortName)
{
    using object_type = decltype(unnamed_object);
    using value_type = decltype(unnamed_value);
    EXPECT_FALSE(::scl::type_short_name<object_type>().empty());
    EXPECT_TRUE(::scl::type_name<object_type>().ends_with(::scl::type_short_name<object_type>()));
    EXPECT_EQ(::scl::type_short_name<object_type>().find("::"), ::std::string_view::npos);
    EXPECT_FALSE(::scl::type_short_name<value_type>().empty());
    EXPECT_TRUE(::scl::type_name<value_type>().ends_with(::scl::type_short_name<value_type>()));
    EXPECT_EQ(::scl::type_short_name<value_type>().find("::"), ::std::string_view::npos);
}

namespace
{
    template <char Character>
    struct CharArg
    {};

    template <::std::size_t Size>
    struct Text
    {
        char value[Size]{};

        constexpr Text(char const (&text)[Size]) // NOLINT(google-explicit-constructor)
        {
            for (::std::size_t i = 0; i < Size; ++i)
                value[i] = text[i];
        }
    };

    template <Text Value>
    struct TextArg
    {
        struct Inner
        {};

        template <Text Other>
        struct Nested
        {};
    };

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
    {
        template <auto Other>
        struct Inner
        {};
    };

    template <typename T>
    struct Box
    {};

    template <typename T>
    struct cooperator
    {};

    struct Pointing
    {
        Pointing const * operator->() const { return this; }

        int operator-(Pointing) const { return 0; }

        int operator->*(int) const { return 0; }
    };

    template <auto Function>
    struct Enclosing
    {
        template <auto Other>
        struct Member
        {};
    };

    template <typename T>
    struct Wrap
    {
        template <auto Other>
        struct Inner
        {};
    };
} // namespace

/**
 * @test Verify that a bracket in a character literal of a template argument does not hide the last
 *       scope operator.
 */
TEST(MetaTypeTest, CharacterLiteralArgumentShortName)
{
    EXPECT_EQ(::scl::type_short_name<CharArg<'('>>(), "CharArg");
    EXPECT_EQ(::scl::type_short_name<CharArg<')'>>(), "CharArg");
}

/**
 * @test Verify that a bracket in a string of a template argument does not hide the last scope
 *       operator.
 */
TEST(MetaTypeTest, StringArgumentShortName)
{
    EXPECT_EQ(::scl::type_short_name<TextArg<"a(b">::Inner>(), "Inner");
    EXPECT_EQ(::scl::type_short_name<TextArg<"a(b">>(), "TextArg");
}

/**
 * @test Verify that an escaped quote in a string of a template argument does not end the string.
 */
TEST(MetaTypeTest, EscapedQuoteArgumentShortName)
{
    using nested_type = TextArg<"x">::Nested<"a\"(">;
    // GCC 16 writes a quote inside a string unescaped, which no scan can tell from the closing one.
    if (::scl::type_name<nested_type>().find("\\\"") == ::std::string_view::npos)
        GTEST_SKIP() << "The name holds no escaped quote";
    EXPECT_EQ(::scl::type_short_name<nested_type>(), "Nested");
}

/**
 * @test Verify that the symbol of an enclosing operator does not hide the last scope operator.
 */
TEST(MetaTypeTest, TypeInOperatorShortName)
{
    using closure_type = decltype(Ordered{} < Ordered{});
    EXPECT_FALSE(::scl::type_short_name<closure_type>().empty());
    EXPECT_TRUE(::scl::type_name<closure_type>().ends_with(::scl::type_short_name<closure_type>()));
    EXPECT_EQ(::scl::type_short_name<closure_type>().find("::"), ::std::string_view::npos);
    EXPECT_EQ(::scl::type_short_name<decltype(Ordered{} <= Ordered{})>(), "Local");
}

/**
 * @test Verify that the symbol of an operator in a template argument does not hide the last scope
 *       operator.
 */
TEST(MetaTypeTest, OperatorInTemplateArgumentShortName)
{
    EXPECT_EQ(::scl::type_short_name<Holder<(&operator<)>>(), "Holder");
    EXPECT_EQ(::scl::type_short_name<Holder<(&operator-)>>(), "Holder");
    EXPECT_EQ(::scl::type_short_name<Holder<(&operator<=)>>(), "Holder");
    EXPECT_EQ(::scl::type_short_name<Holder<(&operator>)>>(), "Holder");
    EXPECT_EQ(::scl::type_short_name<Box<Holder<(&operator-)>>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<decltype(Ordered{} < Ordered{})>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<decltype(Ordered{} <= Ordered{})>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<cooperator<int>>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator-> >>(),
        "Member");
}

/**
 * @test Verify that the operators ->, ->* and - in template arguments do not move the short name to
 *       another scope.
 */
TEST(MetaTypeTest, ArrowAndMinusInTemplateArgumentShortName)
{
    EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator-> >>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator- >>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator- > *>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator->* >>>(), "Box");
    EXPECT_EQ(
        ::scl::type_short_name<Box<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator-> >>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator- >>>(),
        "Box");
    EXPECT_EQ(
        ::scl::type_short_name<Box<Enclosing<&Pointing::operator- >::Member<&Pointing::operator-> >>>(), "Box");
    EXPECT_EQ(::scl::type_short_name<Box<Enclosing<&Pointing::operator- >::Member<&Pointing::operator- >>>(),
        "Box");
    EXPECT_EQ(::scl::type_short_name<Enclosing<&Pointing::operator- >::Member<&Pointing::operator-> >>(), "Member");
    EXPECT_EQ(::scl::type_short_name<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator- >>(), "Member");
    EXPECT_EQ(::scl::type_short_name<Enclosing<&Pointing::operator- >::Member<&Pointing::operator- >>(), "Member");
}

/**
 * @test Verify the short names the documentation states for two types GCC and Clang give one name.
 */
TEST(MetaTypeTest, SharedNameShortName)
{
    using first_type = Wrap<Holder<&Pointing::operator- >>::Inner<&Pointing::operator-> >;
    using second_type = Wrap<Holder<&Pointing::operator-> >::Inner<&Pointing::operator- >>;
    auto const shared = ::scl::type_name<first_type>() == ::scl::type_name<second_type>();
    EXPECT_EQ(::scl::type_short_name<first_type>(), shared ? "Wrap" : "Inner");
    EXPECT_EQ(::scl::type_short_name<second_type>(), "Wrap");
}

/**
 * @test Verify the rendering contract the documentation states.
 * @note The MSVC prefix reaches template arguments as well, so a rendered template name has
 * no spelling common to all compilers; the documented examples match on a substring.
 */
TEST(MetaTypeTest, DocumentedRenderingContract)
{
    constexpr auto npos = ::std::string_view::npos;

    static constexpr auto vector_name = ::scl::type_name<::std::vector<SimpleStruct>>();
    static constexpr auto enum_vector_name = ::scl::type_name<::std::vector<Color>>();
    static constexpr auto nested_name = ::scl::type_name<Namespace::Struct>();
    static constexpr auto anon_name = ::scl::type_name<AnonStruct>();

    STATIC_EXPECT_NE(vector_name.find("SimpleStruct"), npos);
    STATIC_EXPECT_NE(enum_vector_name.find("Color"), npos);
    STATIC_EXPECT_NE(nested_name.find("Namespace::Struct"), npos);

    // The anonymous-namespace marker is spelled differently by each compiler, so only the
    // identifier and the MSVC prefix are portable claims about the rendering.
    STATIC_EXPECT_NE(anon_name.find("AnonStruct"), npos);

    STATIC_EXPECT_EQ(::scl::type_short_name<Color>(), "Color");
    STATIC_EXPECT_EQ(::scl::type_short_name<AnonStruct>(), "AnonStruct");
    STATIC_EXPECT_EQ(::scl::type_short_name<::std::vector<SimpleStruct>>(), "vector");

#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<SimpleStruct>(), "struct SimpleStruct");
    STATIC_EXPECT_EQ(::scl::type_name<Color>(), "enum Color");
    STATIC_EXPECT_NE(vector_name.find("struct SimpleStruct"), npos);
    STATIC_EXPECT_NE(enum_vector_name.find("enum Color"), npos);
    STATIC_EXPECT_TRUE(nested_name.starts_with("struct "));
    STATIC_EXPECT_TRUE(anon_name.starts_with("struct "));
#else
    STATIC_EXPECT_EQ(::scl::type_name<SimpleStruct>(), "SimpleStruct");
    STATIC_EXPECT_EQ(::scl::type_name<Color>(), "Color");
    STATIC_EXPECT_EQ(vector_name.find("struct "), npos);
    STATIC_EXPECT_EQ(enum_vector_name.find("enum "), npos);
    STATIC_EXPECT_EQ(anon_name.find("struct "), npos);
#endif
}

/**
 * @test Verify standard library types.
 * @note STL implementations may vary in how they display default allocators.
 */
TEST(MetaTypeTest, StandardLibraryTypes)
{
    static constexpr ::std::string_view name = ::scl::type_name<::std::vector<int>>();
    // We check for substring because different compilers/STL versions
    // may include or omit default allocator details.
    EXPECT_TRUE(name.find("vector<int") != ::std::string_view::npos);
    EXPECT_EQ(::scl::type_short_name<::std::vector<int>>(), "vector");
}

/**
 * @test Verify that qualifiers like const and references are preserved or handled.
 */
TEST(MetaTypeTest, Qualifiers)
{
    // Current implementation preserves qualifiers as they are part of type T.
    // We use find/contains logic because exact formatting of 'const' (before or after type)
    // might differ between compilers.
    auto const_name = ::scl::type_name<int const>();
    EXPECT_TRUE(const_name.find("int") != ::std::string_view::npos);
    EXPECT_TRUE(const_name.find("const") != ::std::string_view::npos);
}
