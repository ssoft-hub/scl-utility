// expect: unused-result|C4834
#include <scl/utility/type_traits/overload_cast.h>

#include "../attribute/attribute_strict_warnings.h"

int twice(int value);

void caller() { scl::overload_cast<int>(&twice); }
