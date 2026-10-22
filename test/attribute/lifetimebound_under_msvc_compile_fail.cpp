// expect: No spelling of SCL_LIFETIMEBOUND remains for MSVC
// requires: msvc
#define msvc msvc_is_a_macro_of_the_caller

#include <scl/utility/attribute/lifetimebound.h>
