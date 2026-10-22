/**
 * @example attribute_hotcold_example.cpp
 * @brief A function marked with SCL_HOT as called often, and one with SCL_COLD as called rarely.
 *
 * The function checksum is called for every block of data, and the function report_corruption only
 * for a damaged block.
 */

#include <scl/utility/attribute/hotcold.h>

#include <cstddef>
#include <iostream>

//! [hotcold]
SCL_HOT unsigned checksum(unsigned char const * bytes, std::size_t size)
{
    unsigned sum = 0;
    for (std::size_t index = 0; index < size; ++index)
        sum = sum * 31 + bytes[index];
    return sum;
}

SCL_COLD void report_corruption(std::size_t block)
{
    std::cerr << "block " << block << " is damaged\n";
}
//! [hotcold]

int main()
{
    unsigned char const block[] = {1, 2, 3};

    if (checksum(block, sizeof block) != 1026)
        report_corruption(0);
    std::cout << checksum(block, sizeof block) << '\n'; // 1026
}
