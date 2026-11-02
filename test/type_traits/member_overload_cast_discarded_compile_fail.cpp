// expect: unused-result|C4834
#include <scl/utility/type_traits/overload_cast.h>

#include "../attribute/attribute_strict_warnings.h"

struct gauge
{
    int read() const &;
};

void caller() { scl::member_overload_cast<gauge const &>(&gauge::read); }
