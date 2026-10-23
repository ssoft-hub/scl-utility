// expect: no matching function for call|no match for call|no matching call operator
#include <scl/utility/type_traits/overload_cast.h>

struct only_const
{
    int value() const;
};

// The object type asks for the ref-qualifier &, which no overload of value carries.
auto const pointer = scl::member_overload_cast<only_const const &>(&only_const::value);
