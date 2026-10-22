// expect: old_value[^ ]* (is|was declared) deprecated
#include <scl/utility/attribute/deprecated.h>

#include "attribute_strict_warnings.h"

SCL_DEPRECATED
int old_value();

int caller() { return old_value(); }
