// expect: use current_value instead
#include <scl/utility/attribute/deprecated.h>

#include "attribute_strict_warnings.h"

// The literal is split, so only a message the compiler puts together matches.
SCL_DEPRECATED_MSG(
    "use current"
    "_value instead")
int old_value();

int caller() { return old_value(); }
