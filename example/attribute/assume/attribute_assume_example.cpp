/**
 * @example attribute_assume_example.cpp
 * @brief A condition promised with SCL_ASSUME, and a point marked with SCL_UNREACHABLE.
 *
 * A count that is a multiple of four is promised with SCL_ASSUME in the function sum_of_quads.
 * The point after a switch that returns for every enumerator is marked with SCL_UNREACHABLE in
 * the function priority.
 */

#include <scl/utility/attribute/assume.h>

#include <iostream>

//! [assume]
int sum_of_quads(int const * values, int count)
{
    SCL_ASSUME(count % 4 == 0);
    int sum = 0;
    for (int index = 0; index < count; ++index)
        sum += values[index];
    return sum;
}
//! [assume]

//! [unreachable]
enum class severity
{
    low,
    high
};

int priority(severity level)
{
    switch (level)
    {
    case severity::low:
        return 1;
    case severity::high:
        return 2;
    }
    SCL_UNREACHABLE();
}
//! [unreachable]

int main()
{
    int const values[] = {1, 2, 3, 4, 5, 6, 7, 8};

    std::cout << sum_of_quads(values, 8) << ' ' << priority(severity::high) << '\n'; // 36 2
}
