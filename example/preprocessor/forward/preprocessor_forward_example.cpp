/**
 * @example preprocessor_forward_example.cpp
 * @brief Passes a comma-separated list through one parameter of another macro.
 *
 * The preprocessor splits the arguments of a macro at every comma outside parentheses.
 * SCL_FORWARD keeps such a list in one argument and expands to the list itself.
 */

#include <scl/utility/preprocessor/forward.h>

#include <iostream>
#include <map>
#include <string>

//! [apply]
#define APPLY(function, arguments) function(arguments)

constexpr int add(int left, int right) noexcept { return left + right; }

static_assert(APPLY(add, SCL_FORWARD(1, 2)) == 3);
//! [apply]

//! [type_argument]
#define DECLARE_ALIAS(name, type) using name = type;

DECLARE_ALIAS(phone_book, SCL_FORWARD(std::map<std::string, int>))
//! [type_argument]

int main()
{
    phone_book const book{
        {"Alice", 2},
        {  "Bob", 3}
    };

    std::cout << "add  : " << APPLY(add, SCL_FORWARD(1, 2)) << '\n';
    std::cout << "Alice: " << book.at("Alice") << '\n';
}
