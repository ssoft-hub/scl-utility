// expect: no matching function for call|no match for call|no matching call operator|C2672|C2893
#include <scl/utility/type_traits/overload_cast.h>

int twice(int value);
double twice(double value);

auto const pointer = scl::overload_cast<long>(&twice);
