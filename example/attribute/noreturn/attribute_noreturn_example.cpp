/**
 * @example attribute_noreturn_example.cpp
 * @brief A function marked with SCL_NORETURN, which never returns to its caller.
 *
 * The function divide calls the function fail for a zero divisor, and the compiler takes the code
 * after that call as never reached.
 */

#include <scl/utility/attribute/noreturn.h>

#include <cstdlib>
#include <iostream>

//! [noreturn]
SCL_NORETURN
void fail(char const * message)
{
    std::cerr << "fatal: " << message << '\n';
    std::exit(EXIT_FAILURE);
}

int divide(int dividend, int divisor)
{
    if (divisor == 0)
        fail("division by zero");
    return dividend / divisor;
}
//! [noreturn]

int main()
{
    std::cout << divide(42, 6) << '\n'; // 7
}
