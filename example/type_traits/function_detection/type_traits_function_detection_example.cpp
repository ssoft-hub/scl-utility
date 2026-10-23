/**
 * @example type_traits_function_detection_example.cpp
 * @brief Tells whether a function of a given name accepts given arguments, exactly or not.
 *
 * SCL_FUNCTION_DETECTION(foo) declares, where it is expanded, the traits has_foo_function_v
 * and has_foo_function_exact_v with their operations and result types.
 */

#include <scl/utility/type_traits/detection/function.h>

#include <iostream>
#include <type_traits>
#include <utility>

//! [declare]
int foo(int value, int factor);
double foo(double value, double factor);

namespace foo_traits
{
    SCL_FUNCTION_DETECTION(foo)
} // namespace foo_traits
//! [declare]

//! [call]
static_assert(foo_traits::has_foo_function_v<int, int>);
static_assert(foo_traits::has_foo_function_v<int, long>);
static_assert(!foo_traits::has_foo_function_v<int>);
static_assert(std::is_same_v<foo_traits::foo_function_t<double, double>, double>);
//! [call]

//! [exact]
static_assert(foo_traits::has_foo_function_exact_v<int, int>);
static_assert(!foo_traits::has_foo_function_exact_v<int, long>);
//! [exact]

//! [properties]
static_assert(std::is_same_v<foo_traits::foo_function_exact_t<double, double>, double>);
static_assert(!noexcept(foo(std::declval<int>(), std::declval<int>())));
//! [properties]

//! [adl]
namespace shapes
{
    struct circle
    {
        double radius;

        friend double foo(circle shape, double factor) { return shape.radius * factor; }
    };
} // namespace shapes

static_assert(foo_traits::has_foo_function_v<shapes::circle, double>);
static_assert(!foo_traits::has_foo_function_exact_v<shapes::circle, double>);
//! [adl]

int foo(int value, int factor) { return value * factor; }

double foo(double value, double factor) { return value * factor; }

int main()
{
    std::cout << foo(3, 4) << '\n';                     // 12
    std::cout << foo(shapes::circle{1.5}, 2.0) << '\n'; // 3
}
