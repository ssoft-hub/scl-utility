// expect: the macro SCL_FORCE_INLINE, defined before the header
// A space between two identifiers counts, so this is not the last row `inline`.
#define SCL_FORCE_INLINE in line

#include <scl/utility/attribute/inline.h>
