/**
 * @example preprocessor_exceptions_example.cpp
 * @brief Builds the same function with exceptions and without them.
 *
 * text_of branches on SCL_HAS_EXCEPTIONS and takes one path in a build with exceptions and
 * another in a build without them.
 */

#include <scl/utility/any/any_view.h>
#include <scl/utility/preprocessor/exceptions.h>

#include <iostream>
#include <string>

//! [text_of]
std::string text_of(scl::any_view const & view)
{
#if SCL_HAS_EXCEPTIONS
    try
    {
        return scl::any_cast<std::string>(view);
    }
    catch (scl::bad_any_cast const &)
    {
        return "<not a string>";
    }
#else
    auto const * text = scl::any_cast<std::string>(&view);
    return text != nullptr ? *text : "<not a string>";
#endif
}
//! [text_of]

int main()
{
    std::string const word = "word";
    int const number = 7;

    std::cout << text_of(word) << '\n';
    std::cout << text_of(number) << '\n';
}
