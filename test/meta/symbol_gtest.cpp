#include <gtest_utils.h>

#include <scl/utility/meta/symbol.h>

#include <cstddef>
#include <new>
#include <string_view>
#include <type_traits>

namespace Outside
{
    struct Item
    {};
} // namespace Outside

namespace Namespace
{
    void initialize() {}

    int variable;

    void quiet() noexcept {}

    template <typename T>
    void generic()
    {}

    struct Processor
    {
        static int count;
        static void (*callback)(int);

        int property;

        void execute() {}

        void read() const {}

        void bound() & {}

        void operator()() {}

        bool operator==(Processor const &) const { return true; }

        bool operator<(Processor const &) const { return false; }

        bool operator>(Processor const &) const { return false; }

        int operator[](int) const { return 0; }

        static void * operator new(::std::size_t size) { return ::operator new(size); }

        static void operator delete(void * pointer) { ::operator delete(pointer); }

        virtual void hook() {}

        explicit operator int() const { return 0; }

        static void shutdown() {}
    };

    int Processor::count = 0;
    void (*Processor::callback)(int) = nullptr;

    template <typename T>
    struct Pool
    {
        static int count;

        void act() {}
    };

    template <typename T>
    int Pool<T>::count = 0;

    template <auto F>
    struct Hook
    {
        void act() {}
    };

    struct Left
    {
        virtual void left() {}
    };

    struct Right
    {
        virtual void right() {}
    };

    struct Both
        : Left
        , Right
    {
        virtual void both() {}

        virtual void operator~() {}

        int operator,(int) const { return 0; }

        void plain() {}
    };

    struct Shared
    {
        virtual void shared() {}
    };

    struct Derived : virtual Shared
    {
        void plain() {}
    };

    constexpr auto to_int = &Processor::operator int;
    constexpr auto call = &Processor::operator();
    constexpr auto equals = &Processor::operator==;
    constexpr auto subscript = &Processor::operator[];
    constexpr auto allocate = &Processor::operator new;
    constexpr auto less = &Processor::operator<;
    constexpr auto greater = &Processor::operator>;
} // namespace Namespace

namespace
{
    void hidden() {}

    struct Hidden
    {
        static int count;
    };

    int Hidden::count = 0;

    constexpr bool balanced(::std::string_view name) noexcept
    {
        int depth = 0;
        for (char const character : name)
        {
            depth += character == '(' ? 1 : character == ')' ? -1 : 0;
            if (depth < 0)
                return false;
        }
        return depth == 0;
    }
} // namespace

/**
 * @test Verify that both function templates return a std::string_view and throw nothing.
 */
TEST(MetaSymbolTest, Declarations)
{
    STATIC_EXPECT_TRUE(noexcept(::scl::symbol_name<Namespace::initialize>()));
    STATIC_EXPECT_TRUE(noexcept(::scl::symbol_short_name<Namespace::initialize>()));
    STATIC_EXPECT_TRUE((
        ::std::is_same_v<decltype(::scl::symbol_name<Namespace::initialize>()), ::std::string_view>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::symbol_short_name<Namespace::initialize>()), ::std::string_view>));
}

/**
 * @test Verify that a function and a pointer to it get one name.
 */
TEST(MetaSymbolTest, FunctionAndPointerToFunction)
{
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::initialize>(), "Namespace::initialize");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::initialize>(), "Namespace::initialize");
}

/**
 * @test Verify that the full name of each kind of entity carries its namespaces and classes.
 */
TEST(MetaSymbolTest, QualifiedNames)
{
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::execute>(), "Namespace::Processor::execute");
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::Processor::shutdown>(), "Namespace::Processor::shutdown");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::property>(), "Namespace::Processor::property");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::count>(), "Namespace::Processor::count");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::variable>(), "Namespace::variable");
}

/**
 * @test Verify that the signature around a name is dropped: the qualifiers of a member function,
 *       noexcept, and the parameter list of a function pointed to by a static data member.
 */
TEST(MetaSymbolTest, SignatureDropped)
{
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::read>(), "Namespace::Processor::read");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::bound>(), "Namespace::Processor::bound");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::quiet>(), "Namespace::quiet");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::callback>(), "Namespace::Processor::callback");
}

/**
 * @test Verify that a template argument spelled with a space stays in the name.
 */
TEST(MetaSymbolTest, TemplateArgumentWithSpace)
{
    STATIC_EXPECT_EQ(
        ::scl::symbol_name<&Namespace::Pool<unsigned int>::count>(), "Namespace::Pool<unsigned int>::count");
    STATIC_EXPECT_EQ(
        ::scl::symbol_name<&Namespace::Pool<unsigned int>::act>(), "Namespace::Pool<unsigned int>::act");
    STATIC_EXPECT_TRUE(
        ::scl::symbol_name<&Namespace::generic<unsigned int>>().starts_with("Namespace::generic"));
}

/**
 * @test Verify that the name of an operator keeps its symbol, after a space with MSVC and without
 *       one with GCC and Clang.
 */
TEST(MetaSymbolTest, Operators)
{
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::call>(), "Namespace::Processor::operator ()");
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::equals>(), "Namespace::Processor::operator ==");
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::subscript>(), "Namespace::Processor::operator []");
#else
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::call>(), "Namespace::Processor::operator()");
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::equals>(), "Namespace::Processor::operator==");
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::subscript>(), "Namespace::Processor::operator[]");
#endif
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::allocate>(), "Namespace::Processor::operator new");
}

/**
 * @test Verify that an operator in a template argument leaves the name of the template whole.
 */
TEST(MetaSymbolTest, OperatorInTemplateArgument)
{
    STATIC_EXPECT_TRUE(
        ::scl::symbol_name<&Namespace::Hook<Namespace::equals>::act>().starts_with("Namespace::Hook<"));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&Namespace::Hook<Namespace::equals>::act>().ends_with(">::act"));
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Hook<Namespace::equals>::act>(), "act");
    STATIC_EXPECT_TRUE(
        ::scl::symbol_name<&Namespace::Hook<Namespace::less>::act>().starts_with("Namespace::Hook<"));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&Namespace::Hook<Namespace::less>::act>().ends_with(">::act"));
    STATIC_EXPECT_TRUE(
        ::scl::symbol_name<&Namespace::Hook<Namespace::greater>::act>().starts_with("Namespace::Hook<"));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&Namespace::Hook<Namespace::greater>::act>().ends_with(">::act"));
}

/**
 * @test Verify the names the documentation states for the entities a compiler spells with a name of
 *       its own.
 */
TEST(MetaSymbolTest, CompilerOwnNames)
{
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::hook>(), "Namespace::Processor::`vcall'{0}'");
#else
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::hook>(), "Namespace::Processor::hook");
#endif
#if defined(__GNUC__) && !defined(__clang__)
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::to_int>(), "Namespace::Processor::__conv_op");
#else
    STATIC_EXPECT_EQ(::scl::symbol_name<Namespace::to_int>(), "Namespace::Processor::operator int");
#endif
}

/**
 * @test Verify that a member function of a class with several bases or a virtual base gets its
 *       full name and its short name.
 */
TEST(MetaSymbolTest, MemberOfClassWithSeveralBases)
{
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Both::plain>(), "Namespace::Both::plain");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Both::plain>(), "plain");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Derived::plain>(), "Namespace::Derived::plain");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Derived::plain>(), "plain");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Both::operator~ >(), "operator~");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Both::operator~ >(), "Namespace::Both::operator~");
#if defined(_MSC_VER) && !defined(__clang__)
    STATIC_EXPECT_EQ((::scl::symbol_name<&Namespace::Both::operator, >()), "Namespace::Both::operator ,");
    STATIC_EXPECT_TRUE(
        ::scl::symbol_name<&Namespace::Both::both>().starts_with("Namespace::Both::`vcall'{"));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&Namespace::Both::both>().ends_with(",{flat}}'"));
    STATIC_EXPECT_TRUE(::scl::symbol_short_name<&Namespace::Both::both>().starts_with("`vcall'{"));
#else
    STATIC_EXPECT_EQ((::scl::symbol_name<&Namespace::Both::operator, >()), "Namespace::Both::operator,");
    STATIC_EXPECT_EQ(::scl::symbol_name<&Namespace::Both::both>(), "Namespace::Both::both");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Both::both>(), "both");
#endif
}

/**
 * @test Verify that the short name of each kind of entity is its name alone.
 */
TEST(MetaSymbolTest, ShortNames)
{
    STATIC_EXPECT_EQ(::scl::symbol_short_name<Namespace::initialize>(), "initialize");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Processor::execute>(), "execute");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<Namespace::Processor::shutdown>(), "shutdown");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Processor::property>(), "property");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::Processor::count>(), "count");
    STATIC_EXPECT_EQ(::scl::symbol_short_name<&Namespace::variable>(), "variable");
    STATIC_EXPECT_TRUE(
        ::scl::symbol_short_name<&Namespace::generic<Outside::Item>>().starts_with("generic"));
}

/**
 * @test Verify that the name of an entity of an unnamed namespace keeps that namespace whole.
 */
TEST(MetaSymbolTest, UnnamedNamespace)
{
    STATIC_EXPECT_TRUE(::scl::symbol_name<&hidden>().ends_with("::hidden"));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&hidden>().find("anonymous") != ::std::string_view::npos);
    STATIC_EXPECT_TRUE(balanced(::scl::symbol_name<&hidden>()));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&Hidden::count>().ends_with("::Hidden::count"));
    STATIC_EXPECT_TRUE(::scl::symbol_name<&Hidden::count>().find("anonymous") != ::std::string_view::npos);
    STATIC_EXPECT_TRUE(balanced(::scl::symbol_name<&Hidden::count>()));
}
