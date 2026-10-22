// expect: return(ed)? value
#include <scl/utility/attribute/nodiscard.h>

#include "attribute_strict_warnings.h"

struct SCL_NODISCARD status
{
    int code;
};

status open_channel();

void caller() { open_channel(); }
