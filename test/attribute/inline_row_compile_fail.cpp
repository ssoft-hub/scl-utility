// expect: the macro SCL_FORCE_INLINE, defined before the header
// A definition as a row whose condition does not hold on the compiler at hand.
#if defined(_MSC_VER) && !defined(__clang__)
#define SCL_FORCE_INLINE [[__gnu__::__always_inline__]] inline
#else
#define SCL_FORCE_INLINE __forceinline
#endif

#include <scl/utility/attribute/inline.h>
