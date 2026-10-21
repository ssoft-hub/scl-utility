/**
 * @example attribute_nodiscard_example.cpp
 * @brief A function marked with SCL_NODISCARD_MSG and a type marked with SCL_NODISCARD.
 *
 * Every result is read, so the program compiles with no warning. A warning is issued where
 * the result of store or of open_channel is dropped.
 */

#include <scl/utility/attribute/nodiscard.h>

#include <iostream>
#include <vector>

//! [nodiscard]
SCL_NODISCARD_MSG("the result tells whether the value was stored")
bool store(std::vector<int> & storage, int value)
{
    if (storage.size() == storage.capacity())
        return false;
    storage.push_back(value);
    return true;
}

struct SCL_NODISCARD status
{
    int code;
};

status open_channel(int id) { return status{id < 0 ? 1 : 0}; }
//! [nodiscard]

int main()
{
    std::vector<int> storage;
    storage.reserve(1);

    bool const stored = store(storage, 7);
    status const opened = open_channel(3);
    std::cout << std::boolalpha << stored << ' ' << opened.code << '\n'; // true 0
}
