// expect: nowhere.*(undeclared identifier|was not declared)|undeclared identifier.*nowhere
#include <scl/utility/type_traits/detection/function.h>

namespace traits
{
    SCL_FUNCTION_DETECTION(nowhere)
} // namespace traits
