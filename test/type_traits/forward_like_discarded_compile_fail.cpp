// expect: unused-result|C4834
#include <scl/utility/type_traits/forward_like.h>

#include "../attribute/attribute_strict_warnings.h"

void caller(int value) { scl::forward_like<int>(value); }
