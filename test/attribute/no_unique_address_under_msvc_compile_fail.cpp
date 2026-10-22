// expect: No spelling of SCL_NO_UNIQUE_ADDRESS remains for the MSVC ABI
// requires: msvc_abi
#define msvc msvc_is_a_macro_of_the_caller

#include <scl/utility/attribute/no_unique_address.h>
