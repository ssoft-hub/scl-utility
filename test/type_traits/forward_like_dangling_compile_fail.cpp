// expect: Wdangling
// requires: clang
#include <scl/utility/type_traits/forward_like.h>

#include "../attribute/attribute_strict_warnings.h"

int caller()
{
    int && value = scl::forward_like<int>(1);
    return value;
}
