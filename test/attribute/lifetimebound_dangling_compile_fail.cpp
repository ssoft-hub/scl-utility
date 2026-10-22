// expect: Wdangling
// requires: clang
#include <scl/utility/attribute/lifetimebound.h>

#include <string>
#include <string_view>

#include "attribute_strict_warnings.h"

std::string_view first_word(std::string const & text SCL_LIFETIMEBOUND);

std::size_t caller()
{
    std::string_view const word = first_word(std::string{"hello world"});
    return word.size();
}
