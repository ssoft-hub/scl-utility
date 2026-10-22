#pragma once

#include <scl/utility/attribute/noreturn.h>

// Defined in another translation unit, so no compiler sees that the function never returns.
SCL_NORETURN
void stop_elsewhere();
