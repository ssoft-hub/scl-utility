// expect: the macro SCL_LIKELY, defined before the header
// A definition that states the opposite promise.
#define SCL_LIKELY [[unlikely]]

#include <scl/utility/attribute/likely.h>
