// expect: member pointer to a reference|pointer to member reference|pointer to reference member
#include <scl/utility/type_traits/member_like.h>

struct gauge
{};

static_assert(sizeof(scl::member_like_t<gauge, void (&)()>) != 0);
