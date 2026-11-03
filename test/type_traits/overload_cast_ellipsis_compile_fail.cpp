// expect: no matching function for call|no match for call|no matching call operator|C2672|C2893
#include <scl/utility/type_traits/overload_cast.h>

long spread(int value, ...);

auto const pointer = scl::overload_cast<int>(&spread);
