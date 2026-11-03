// expect: member_function_like_t requires a function type
#include <scl/utility/type_traits/member_like.h>

struct gauge
{};

static_assert(sizeof(scl::member_function_like_t<gauge, int>) != 0);
