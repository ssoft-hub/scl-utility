/**
 * @example attribute_inline_example.cpp
 * @brief A function marked with SCL_FORCE_INLINE, and one marked with SCL_NOINLINE.
 *
 * The compiler is asked to inline the function clamp at every call, and never to inline
 * the function report_out_of_range.
 */

#include <scl/utility/attribute/inline.h>

#include <iostream>

//! [force_inline]
SCL_FORCE_INLINE
int clamp(int value, int low, int high)
{
    return value < low ? low : value > high ? high : value;
}
//! [force_inline]

//! [noinline]
SCL_NOINLINE
void report_out_of_range(int value)
{
    std::cerr << value << " is out of range\n";
}
//! [noinline]

int main()
{
    int const value = 42;

    if (clamp(value, 0, 10) != value)
        report_out_of_range(value);
    std::cout << clamp(value, 0, 10) << '\n'; // 10
}
