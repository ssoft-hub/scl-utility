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
    EXPECT_EQ(scl::symbol_short_name<Namespace::initialize>(), "initialize");
    EXPECT_EQ(scl::symbol_short_name<&Namespace::Processor::execute>(), "execute");
    EXPECT_EQ(scl::symbol_short_name<Namespace::Processor::shutdown>(), "shutdown");
    EXPECT_EQ(scl::symbol_short_name<&Namespace::Processor::property>(), "property");
    EXPECT_EQ(scl::symbol_short_name<&Namespace::variable>(), "variable");
}

TEST(SymbolTraitsTest, QualifiedNames)
{
    EXPECT_EQ(scl::symbol_name<Namespace::initialize>(), "Namespace::initialize");
    EXPECT_EQ(scl::symbol_name<&Namespace::Processor::execute>(), "Namespace::Processor::execute");
    EXPECT_EQ(scl::symbol_name<Namespace::Processor::shutdown>(), "Namespace::Processor::shutdown");
    EXPECT_EQ(scl::symbol_name<&Namespace::Processor::property>(), "Namespace::Processor::property");
    EXPECT_EQ(scl::symbol_name<&Namespace::variable>(), "Namespace::variable");
}
