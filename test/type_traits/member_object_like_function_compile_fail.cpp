// expect: member_object_like_t expects a data member type
#include <scl/utility/type_traits/member_like.h>

struct gauge
{};

static_assert(sizeof(scl::member_object_like_t<gauge, void()>) != 0);
