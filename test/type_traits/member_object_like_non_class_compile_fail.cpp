// expect: member_object_like_t requires Type to be a class or union type
#include <scl/utility/type_traits/member_like.h>

static_assert(sizeof(scl::member_object_like_t<int, int>) != 0);
