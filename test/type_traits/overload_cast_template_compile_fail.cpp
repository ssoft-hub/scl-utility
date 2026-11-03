// expect: no matching function for call|no match for call|no matching call operator|couldn.t deduce|C2672|C2783|C2893
#include <scl/utility/type_traits/overload_cast.h>

template <typename Type>
Type echo(Type value);
long echo(int value);

auto const pointer = scl::overload_cast<int>(&echo);
