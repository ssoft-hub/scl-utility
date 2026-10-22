/**
 * @example attribute_lifetimebound_example.cpp
 * @brief A view whose lifetime is bound to the argument of its function by SCL_LIFETIMEBOUND.
 *
 * The function first_word returns a view into the string it is given. Clang warns where the view
 * outlives a temporary string, and no warning is issued for the named string passed here.
 */

#include <scl/utility/attribute/lifetimebound.h>

#include <iostream>
#include <string>
#include <string_view>

//! [lifetimebound]
std::string_view first_word(std::string const & text SCL_LIFETIMEBOUND)
{
    return std::string_view{text}.substr(0, text.find(' '));
}
//! [lifetimebound]

int main()
{
    std::string const greeting = "hello world";

    std::cout << first_word(greeting) << '\n'; // hello
}
