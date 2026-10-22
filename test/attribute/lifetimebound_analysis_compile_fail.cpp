// expect: C26815
// requires: msvc
// options: /WX /external:anglebrackets /external:W0 /analyze:external- /analyze /analyze:pluginEspXEngine.dll
#include <scl/utility/attribute/lifetimebound.h>

#include <string>
#include <string_view>

std::string_view first_word(std::string const & text SCL_LIFETIMEBOUND);

std::size_t caller()
{
    std::string_view const word = first_word(std::string{"hello world"});
    return word.size();
}
