/**
 * @example attribute_deprecated_example.cpp
 * @brief A function marked with SCL_DEPRECATED_MSG and a type marked with SCL_DEPRECATED.
 *
 * Only the replacements are used, so the program compiles with no warning. A warning is issued at
 * a call of the old function and at a use of the old type.
 */

#include <scl/utility/attribute/deprecated.h>

#include <iostream>

//! [deprecated]
struct endpoint
{
    char const * host;
    int port;
};

bool connect(endpoint const & target) { return target.port > 0; }

SCL_DEPRECATED_MSG("use connect(endpoint) instead")
bool connect(char const * host, int port);

struct SCL_DEPRECATED legacy_handle
{
    int descriptor;
};
//! [deprecated]

int main()
{
    std::cout << std::boolalpha << connect(endpoint{"localhost", 8080}) << '\n'; // true
}
