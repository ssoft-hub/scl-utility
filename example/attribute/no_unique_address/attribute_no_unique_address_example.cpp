/**
 * @example attribute_no_unique_address_example.cpp
 * @brief An empty member marked with SCL_NO_UNIQUE_ADDRESS, which takes no storage of its own.
 *
 * The class counter holds an empty policy beside an int, and is no larger than an int.
 */

#include <scl/utility/attribute/no_unique_address.h>

#include <iostream>

//! [no_unique_address]
struct no_policy
{};

struct counter
{
    SCL_NO_UNIQUE_ADDRESS no_policy policy;
    int value;
};

static_assert(sizeof(counter) == sizeof(int));
//! [no_unique_address]

int main()
{
    std::cout << sizeof(counter) << '\n'; // the size of int
}
