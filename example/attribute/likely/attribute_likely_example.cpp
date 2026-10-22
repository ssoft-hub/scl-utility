/**
 * @example attribute_likely_example.cpp
 * @brief The usual branch of a program, marked on a statement and inside an expression.
 *
 * The branch of a digit is marked with SCL_LIKELY and the other branch with SCL_UNLIKELY in
 * the function parse_digit. The condition of a nonzero code is marked with SCL_UNLIKELY_EXPR in
 * the function count_errors, and the usual result with SCL_LIKELY_EXPR in the function all_valid.
 */

#include <scl/utility/attribute/likely.h>

#include <cstddef>
#include <iostream>

//! [likely]
int parse_digit(char symbol)
{
    if (symbol >= '0' && symbol <= '9')
        SCL_LIKELY { return symbol - '0'; }
    else
        SCL_UNLIKELY { return -1; }
}
//! [likely]

//! [likely_expr]
int count_errors(int const * codes, std::size_t size)
{
    int errors = 0;
    for (std::size_t index = 0; index < size; ++index)
        if (SCL_UNLIKELY_EXPR(codes[index] != 0))
            ++errors;
    return errors;
}

bool all_valid(int const * codes, std::size_t size)
{
    return SCL_LIKELY_EXPR(count_errors(codes, size) == 0);
}
//! [likely_expr]

int main()
{
    int const codes[] = {0, 0, 7, 0};

    int const digit = parse_digit('7');
    int const errors = count_errors(codes, 4);
    bool const valid = all_valid(codes, 4);
    std::cout << std::boolalpha << digit << ' ' << errors << ' ' << valid << '\n'; // 7 1 false
}
