// expect: ambiguous|C2668
#include <scl/utility/type_traits/overload_cast.h>

struct gauge
{
    int read(int value);
    int read(int value) const;
};

auto const pointer = scl::overload_cast<int>(&gauge::read);
