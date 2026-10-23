/**
 * @example attribute_indeterminate_example.cpp
 * @brief A buffer declared with SCL_INDETERMINATE and written before every read.
 *
 * The buffer is filled digit by digit by the function reversed_decimal, and only the digits written
 * are read.
 */

#include <scl/utility/attribute/indeterminate.h>

#include <cstddef>
#include <iostream>
#include <limits>
#include <string>

//! [indeterminate]
std::string reversed_decimal(unsigned value)
{
    SCL_INDETERMINATE
    char digits[std::numeric_limits<unsigned>::digits10 + 1];
    std::size_t count = 0;
    do
    {
        digits[count++] = static_cast<char>('0' + value % 10);
        value /= 10;
    } while (value != 0);
    return std::string(digits, count);
}
//! [indeterminate]

int main()
{
    std::cout << reversed_decimal(1203) << '\n'; // 3021, the digits from the lowest
}
