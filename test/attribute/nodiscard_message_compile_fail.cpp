// expect: the status tells whether the call succeeded
#include <scl/utility/attribute/nodiscard.h>

#include "attribute_strict_warnings.h"

// The literal is split, so only a message the compiler puts together matches.
SCL_NODISCARD_MSG(
    "the status tells"
    " whether the call succeeded")
int store();

void caller() { store(); }
