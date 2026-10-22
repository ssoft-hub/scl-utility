#pragma once

#include <scl/utility/attribute/inline.h>

// Two translation units of the suite include this header, and the program links only where
// SCL_FORCE_INLINE carries inline.
SCL_FORCE_INLINE
int twice_in_every_unit(int value)
{
    return value * 2;
}

int twice_from_the_other_unit(int value);
