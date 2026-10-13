#include <gtest_utils.h>

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
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
    {
        struct Nested
        {};
    };

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
 * @test Verify that both function templates return a std::string_view and throw nothing.
 */
TEST(MetaTypeTest, Declarations)
{
    STATIC_EXPECT_TRUE(noexcept(::scl::type_name<int>()));
    STATIC_EXPECT_TRUE(noexcept(::scl::type_short_name<int>()));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::type_name<int>()), ::std::string_view>));
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::type_short_name<int>()), ::std::string_view>));
}

/**
 * @test Verify fundamental types extraction.
 */
TEST(MetaTypeTest, FundamentalTypes)
{
    STATIC_EXPECT_EQ(::scl::type_name<int>(), "int");
    STATIC_EXPECT_EQ(::scl::type_name<float>(), "float");
    STATIC_EXPECT_EQ(::scl::type_name<double>(), "double");
    STATIC_EXPECT_EQ(::scl::type_name<void>(), "void");
}

/**
 * @test Verify user-defined types (struct/class).
 * @note On MSVC, type names include struct/class keywords.
 */
TEST(MetaTypeTest, UserDefinedTypes)
{
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<SimpleStruct>(), "struct SimpleStruct");
    STATIC_EXPECT_EQ(::scl::type_name<SimpleClass>(), "class SimpleClass");
#else
    STATIC_EXPECT_EQ(::scl::type_name<SimpleStruct>(), "SimpleStruct");
    STATIC_EXPECT_EQ(::scl::type_name<SimpleClass>(), "SimpleClass");
#endif

    static constexpr auto simple_struct_name = ::scl::type_short_name<SimpleStruct>();
    static constexpr auto simple_class_name = ::scl::type_short_name<SimpleClass>();

    STATIC_EXPECT_EQ(simple_struct_name, "SimpleStruct");
    STATIC_EXPECT_EQ(simple_class_name, "SimpleClass");
}

/**
 * @test Verify types within custom namespaces.
 */
TEST(MetaTypeTest, NamespacedTypes)
{
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<Namespace::Struct>(), "struct Namespace::Struct");
#else
    STATIC_EXPECT_EQ(::scl::type_name<Namespace::Struct>(), "Namespace::Struct");
#endif

    STATIC_EXPECT_EQ(::scl::type_short_name<Namespace::Struct>(), "Struct");
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<Namespace::Struct::Nested>(), "struct Namespace::Struct::Nested");
#else
    STATIC_EXPECT_EQ(::scl::type_name<Namespace::Struct::Nested>(), "Namespace::Struct::Nested");
#endif
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
    STATIC_EXPECT_EQ(::scl::type_name<T>(), "struct Namespace::TemplateStruct<struct Namespace::Struct>");
    STATIC_EXPECT_EQ(::scl::type_name<TT>(),
        "class Namespace::TemplateClass<struct Namespace::TemplateStruct<struct Namespace::Struct> >");
#elif defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    STATIC_EXPECT_EQ(::scl::type_name<TT>(),
        "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct>>");
#else
    STATIC_EXPECT_EQ(::scl::type_name<T>(), "Namespace::TemplateStruct<Namespace::Struct>");
    STATIC_EXPECT_EQ(::scl::type_name<TT>(),
        "Namespace::TemplateClass<Namespace::TemplateStruct<Namespace::Struct> >");
#endif

    STATIC_EXPECT_EQ(::scl::type_short_name<T>(), "TemplateStruct");
    STATIC_EXPECT_EQ(::scl::type_short_name<TT>(), "TemplateClass");
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
 * @test Verify that type_short_name names a fundamental type as type_name does, without its
 *       namespace.
 */
TEST(MetaTypeTest, ShortNameOfFundamental)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<int>(), "int");
    STATIC_EXPECT_EQ(::scl::type_short_name<void>(), "void");
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_short_name<unsigned long long>(), "unsigned __int64");
    STATIC_EXPECT_EQ(::scl::type_short_name<::std::nullptr_t>(), "nullptr");
#else
    STATIC_EXPECT_EQ(::scl::type_short_name<::std::nullptr_t>(), "nullptr_t");
#endif
#if defined(__GNUC__) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_short_name<unsigned long long>(), "long long unsigned int");
#endif
}

/**
 * @test Verify that type_short_name gives a closure type a non-empty ending of its full name.
 */
TEST(MetaTypeTest, ClosureShortName)
{
    auto closure = [](int number) { return number; };
    using closure_type = decltype(closure);
#if defined(__GNUC__) && !defined(__clang__)
    auto empty_closure = [] {};
    STATIC_EXPECT_EQ(::scl::type_short_name<decltype(empty_closure)>(), "<lambda()>");
#endif
    STATIC_EXPECT_FALSE(::scl::type_short_name<closure_type>().empty());
    STATIC_EXPECT_TRUE(::scl::type_name<closure_type>().ends_with(::scl::type_short_name<closure_type>()));
    STATIC_EXPECT_EQ(::scl::type_short_name<closure_type>().find("::"), ::std::string_view::npos);
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
    STATIC_EXPECT_FALSE(::scl::type_short_name<object_type>().empty());
    STATIC_EXPECT_TRUE(::scl::type_name<object_type>().ends_with(::scl::type_short_name<object_type>()));
    STATIC_EXPECT_EQ(::scl::type_short_name<object_type>().find("::"), ::std::string_view::npos);
    STATIC_EXPECT_FALSE(::scl::type_short_name<value_type>().empty());
    STATIC_EXPECT_TRUE(::scl::type_name<value_type>().ends_with(::scl::type_short_name<value_type>()));
    STATIC_EXPECT_EQ(::scl::type_short_name<value_type>().find("::"), ::std::string_view::npos);
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
    STATIC_EXPECT_EQ(::scl::type_short_name<CharArg<'('>>(), "CharArg");
    STATIC_EXPECT_EQ(::scl::type_short_name<CharArg<')'>>(), "CharArg");
}

/**
 * @test Verify that a bracket in a string of a template argument does not hide the last scope
 *       operator.
 */
TEST(MetaTypeTest, StringArgumentShortName)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<TextArg<"a(b">::Inner>(), "Inner");
    STATIC_EXPECT_EQ(::scl::type_short_name<TextArg<"a(b">>(), "TextArg");
}

namespace
{
    template <typename Nested>
    void check_nested_short_name()
    {
        STATIC_EXPECT_EQ(::scl::type_short_name<Nested>(), "Nested");
    }

    // GCC 16 writes a quote inside a string unescaped and MSVC writes the string as character codes,
    // so neither name holds an escaped quote.
    template <typename Nested>
    void check_escaped_quote_short_name()
    {
        if constexpr (::scl::type_name<Nested>().find("\\\"") == ::std::string_view::npos)
            GTEST_SKIP() << "The name holds no escaped quote";
        else
            check_nested_short_name<Nested>();
    }
} // namespace

/**
 * @test Verify that an escaped quote in a string of a template argument does not end the string.
 */
TEST(MetaTypeTest, EscapedQuoteArgumentShortName)
{
    check_escaped_quote_short_name<TextArg<"x">::Nested<"a\"(">>();
}

/**
 * @test Verify that the symbol of an enclosing operator does not hide the last scope operator.
 */
TEST(MetaTypeTest, TypeInOperatorShortName)
{
    using closure_type = decltype(Ordered{} < Ordered{});
    STATIC_EXPECT_FALSE(::scl::type_short_name<closure_type>().empty());
    STATIC_EXPECT_TRUE(::scl::type_name<closure_type>().ends_with(::scl::type_short_name<closure_type>()));
    STATIC_EXPECT_EQ(::scl::type_short_name<closure_type>().find("::"), ::std::string_view::npos);
    STATIC_EXPECT_EQ(::scl::type_short_name<decltype(Ordered{} <= Ordered{})>(), "Local");
}

/**
 * @test Verify that the symbol of an operator in a template argument does not hide the last scope
 *       operator.
 */
TEST(MetaTypeTest, OperatorInTemplateArgumentShortName)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<Holder<(&operator<)>>(), "Holder");
    STATIC_EXPECT_EQ(::scl::type_short_name<Holder<(&operator-)>>(), "Holder");
    STATIC_EXPECT_EQ(::scl::type_short_name<Holder<(&operator<=)>>(), "Holder");
    STATIC_EXPECT_EQ(::scl::type_short_name<Holder<(&operator>)>>(), "Holder");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<Holder<(&operator-)>>>(), "Box");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<decltype(Ordered{} < Ordered{})>>(), "Box");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<decltype(Ordered{} <= Ordered{})>>(), "Box");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<cooperator<int>>>(), "Box");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator-> >>(), "Member");
}

/**
 * @test Verify that the operators ->, ->* and - in template arguments do not move the short name to
 *       another scope.
 */
TEST(MetaTypeTest, ArrowAndMinusInTemplateArgumentShortName)
{
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator-> >>>(), "Box");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator- >>>(), "Box");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator- > *>>(), "Box");
    STATIC_EXPECT_EQ(::scl::type_short_name<Box<Holder<&Pointing::operator->* >>>(), "Box");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Box<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator-> >>>(), "Box");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Box<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator- >>>(), "Box");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Box<Enclosing<&Pointing::operator- >::Member<&Pointing::operator-> >>>(), "Box");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Box<Enclosing<&Pointing::operator- >::Member<&Pointing::operator- >>>(), "Box");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Enclosing<&Pointing::operator- >::Member<&Pointing::operator-> >>(), "Member");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Enclosing<&Pointing::operator-> >::Member<&Pointing::operator- >>(), "Member");
    STATIC_EXPECT_EQ(
        ::scl::type_short_name<Enclosing<&Pointing::operator- >::Member<&Pointing::operator- >>(), "Member");
}

/**
 * @test Verify the short names the documentation states for two types GCC and Clang give one name.
 */
TEST(MetaTypeTest, SharedNameShortName)
{
    using first_type = Wrap<Holder<&Pointing::operator- >>::Inner<&Pointing::operator-> >;
    using second_type = Wrap<Holder<&Pointing::operator-> >::Inner<&Pointing::operator- >>;
    STATIC_EXPECT_EQ(::scl::type_short_name<second_type>(), "Wrap");
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_NE(::scl::type_name<first_type>(), ::scl::type_name<second_type>());
    STATIC_EXPECT_EQ(::scl::type_short_name<first_type>(), "Inner");
#else
    STATIC_EXPECT_EQ(::scl::type_name<first_type>(), ::scl::type_name<second_type>());
    STATIC_EXPECT_EQ(::scl::type_short_name<first_type>(), "Wrap");
#endif
#if defined(__clang__)
    STATIC_EXPECT_TRUE(::scl::type_name<first_type>().ends_with(
        "Wrap<(anonymous namespace)::Holder<&(anonymous namespace)::Pointing::operator->>::Inner<"
        "&(anonymous namespace)::Pointing::operator->>"));
#endif
}

/**
 * @test Verify that MSVC writes the key enum and union before such a type, where GCC and Clang
 *       write none, and that the short name of the enumeration drops the key.
 */
TEST(MetaTypeTest, KeyOfEnumerationAndUnion)
{
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<Color>(), "enum Color");
    STATIC_EXPECT_EQ(::scl::type_name<Number>(), "union Number");
#else
    STATIC_EXPECT_EQ(::scl::type_name<Color>(), "Color");
    STATIC_EXPECT_EQ(::scl::type_name<Number>(), "Number");
#endif
    STATIC_EXPECT_EQ(::scl::type_short_name<Color>(), "Color");
}

/**
 * @test Verify that a type of an unnamed namespace keeps its identifier, and the short name drops
 *       the namespace whatever the compiler spells for it.
 */
TEST(MetaTypeTest, AnonymousNamespaceType)
{
    STATIC_EXPECT_TRUE(::scl::type_name<AnonStruct>().ends_with("::AnonStruct"));
    STATIC_EXPECT_EQ(::scl::type_short_name<AnonStruct>(), "AnonStruct");
}

/**
 * @test Verify that a type of the standard library is spelled as the documentation states for
 *       std::string on each measured toolchain.
 */
TEST(MetaTypeTest, StandardLibraryTypes)
{
    STATIC_EXPECT_TRUE(::scl::type_name<::std::vector<int>>().find("vector<int") != ::std::string_view::npos);
    STATIC_EXPECT_EQ(::scl::type_short_name<::std::vector<int>>(), "vector");
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<::std::string>(),
        "class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >");
#elif defined(__clang__) && (defined(_MSVC_STL_VERSION) || defined(__GLIBCXX__))
    STATIC_EXPECT_EQ(::scl::type_name<::std::string>(), "std::basic_string<char>");
#elif defined(__GNUC__) && !defined(__clang__) && _GLIBCXX_USE_CXX11_ABI
    STATIC_EXPECT_EQ(::scl::type_name<::std::string>(), "std::__cxx11::basic_string<char>");
#endif
}

/**
 * @test Verify that the full name keeps const, volatile and the reference of the type.
 */
TEST(MetaTypeTest, Qualifiers)
{
    constexpr auto npos = ::std::string_view::npos;
    constexpr auto name = ::scl::type_name<SimpleClass const volatile &>();
    STATIC_EXPECT_NE(name.find("SimpleClass"), npos);
    STATIC_EXPECT_NE(name.find("const"), npos);
    STATIC_EXPECT_NE(name.find("volatile"), npos);
    STATIC_EXPECT_TRUE(name.ends_with("&"));
    STATIC_EXPECT_TRUE(::scl::type_name<SimpleClass &&>().ends_with("&&"));
#if defined(__clang__)
    STATIC_EXPECT_EQ(::scl::type_name<SimpleClass const &>(), "const SimpleClass &");
#endif
}
