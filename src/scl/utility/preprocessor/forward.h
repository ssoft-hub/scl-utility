#pragma once

/**
 * @file
 * @brief A macro that passes a comma-separated list through one parameter of another macro.
 * @ingroup scl_utility_preprocessor
 */

/**
 * @def SCL_FORWARD(...)
 * @brief Expands to its arguments unchanged.
 * @ingroup scl_utility_preprocessor
 * @details
 * The preprocessor splits the arguments of a macro at every comma outside parentheses, so
 * `SCL_FORWARD(a, b)` written as an argument of another macro stays one argument, and
 * the parameter receives `a, b` once that argument is expanded. A parameter the other macro applies
 * `#` or `##` to receives the argument unexpanded, as `SCL_FORWARD(a, b)`. The macro adds no
 * parentheses around the list.
 *
 * @param ... The list to expand to.
 *
 * @snippet preprocessor/forward/preprocessor_forward_example.cpp apply
 */
#ifndef SCL_FORWARD
#define SCL_FORWARD(...) __VA_ARGS__
#endif
