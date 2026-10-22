#include <gtest_utils.h>

#include <scl/utility/attribute/lifetimebound.h>

#include <string>
#include <string_view>

#include "attribute_strict_warnings.h"

namespace
{
    std::string_view first_word(std::string const & text SCL_LIFETIMEBOUND)
    {
        return std::string_view{text}.substr(0, text.find(' '));
    }

    class holder
    {
    public:
        explicit holder(std::string text)
            : m_text{std::move(text)}
        {}

        std::string_view view() const SCL_LIFETIMEBOUND { return m_text; }

    private:
        std::string m_text;
    };
} // namespace

TEST(AttributeLifetimebound, StandsAfterAParameterWithNoWarning)
{
    std::string const text = "hello world";
    EXPECT_EQ(first_word(text), "hello");
}

TEST(AttributeLifetimebound, StandsAfterTheQualifiersOfAMemberFunctionWithNoWarning)
{
    holder const kept{"kept"};
    EXPECT_EQ(kept.view(), "kept");
}
