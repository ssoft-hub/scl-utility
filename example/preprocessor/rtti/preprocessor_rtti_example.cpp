/**
 * @example preprocessor_rtti_example.cpp
 * @brief Builds the same function with RTTI and without it.
 *
 * name_of branches on SCL_HAS_RTTI and takes one path in a build with RTTI and another in a build
 * without it.
 */

#include <scl/utility/meta/type.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <scl/utility/runtime/type.h>
#endif

#include <iostream>
#include <string>

struct shape
{
    virtual ~shape() = default;
};

struct circle : shape
{};

//! [name_of]
template <typename Type>
std::string name_of([[maybe_unused]] Type const & object)
{
#if SCL_HAS_RTTI
    return scl::type_short_name(object); // the dynamic type of a polymorphic object
#else
    return std::string{scl::type_short_name<Type>()}; // the static type of the argument
#endif
}
//! [name_of]

int main()
{
    circle const round;
    shape const & figure = round;

    std::cout << name_of(figure) << '\n'; // circle with RTTI, shape without it
}
