/**
 * @example attribute_unsequenced_example.cpp
 * @brief A function marked with SCL_UNSEQUENCED, and one marked with SCL_REPRODUCIBLE.
 *
 * The function square reads nothing but its argument, and the function count_zeros reads the array
 * its argument points to. Neither changes anything a caller observes.
 */

#include <scl/utility/attribute/unsequenced.h>

#include <cstddef>
#include <iostream>

//! [unsequenced]
SCL_UNSEQUENCED(int square(int value)) { return value * value; }

SCL_REPRODUCIBLE(std::size_t count_zeros(int const * values, std::size_t size))
{
    std::size_t zeros = 0;
    for (std::size_t index = 0; index < size; ++index)
        if (values[index] == 0)
            ++zeros;
    return zeros;
}
//! [unsequenced]

int main()
{
    int const values[] = {0, 3, 0, 5};

    std::cout << square(7) << ' ' << count_zeros(values, 4) << '\n'; // 49 2
}
