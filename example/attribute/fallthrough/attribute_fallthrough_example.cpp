/**
 * @example attribute_fallthrough_example.cpp
 * @brief The cases of a switch marked with SCL_FALLTHROUGH as continuing into the next one.
 *
 * Every step from the given stage to the last one is run by the function steps_left, and no warning
 * is issued about the cases that end without break.
 */

#include <scl/utility/attribute/fallthrough.h>

#include <iostream>

//! [fallthrough]
int steps_left(int stage)
{
    int steps = 0;
    switch (stage)
    {
    case 0:
        ++steps;
        SCL_FALLTHROUGH;
    case 1:
        ++steps;
        SCL_FALLTHROUGH;
    case 2:
        ++steps;
        break;
    default:
        break;
    }
    return steps;
}
//! [fallthrough]

int main()
{
    std::cout << steps_left(0) << ' ' << steps_left(2) << '\n'; // 3 1
}
