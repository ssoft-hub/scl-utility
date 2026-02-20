#include <gtest/gtest.h>

#include <scl/utility/meta/symbol.h>

namespace Namespace
{
    void initialize() {}

    int variable;

    struct Processor
    {
        void execute() {}
        static void shutdown() {}

        int property;
    };
} // namespace Namespace

TEST(SymbolTraitsTest, ShortNames)
{
    EXPECT_EQ(::scl::symbol_short_name<Namespace::initialize>(), "initialize");
    EXPECT_EQ(::scl::symbol_short_name<&Namespace::Processor::execute>(), "execute");
    EXPECT_EQ(::scl::symbol_short_name<Namespace::Processor::shutdown>(), "shutdown");
#ifndef _MSC_VER
    // MSVC limitation: pointer-to-member data cannot be extracted by __FUNCSIG__
    EXPECT_EQ(::scl::symbol_short_name<&Namespace::Processor::property>(), "property");
#endif
    EXPECT_EQ(::scl::symbol_short_name<&Namespace::variable>(), "variable");
}

TEST(SymbolTraitsTest, QualifiedNames)
{
    EXPECT_EQ(::scl::symbol_name<Namespace::initialize>(), "Namespace::initialize");
    EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::execute>(), "Namespace::Processor::execute");
    EXPECT_EQ(::scl::symbol_name<Namespace::Processor::shutdown>(), "Namespace::Processor::shutdown");
#ifndef _MSC_VER
    // MSVC limitation: pointer-to-member data cannot be extracted by __FUNCSIG__
    EXPECT_EQ(::scl::symbol_name<&Namespace::Processor::property>(), "Namespace::Processor::property");
#endif
    EXPECT_EQ(::scl::symbol_name<&Namespace::variable>(), "Namespace::variable");
}
