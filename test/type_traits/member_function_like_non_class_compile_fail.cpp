// expect: member_function_like_t requires Type to be a class or union type
#include <scl/utility/type_traits/member_like.h>

static_assert(sizeof(scl::member_function_like_t<int, void()>) != 0);
