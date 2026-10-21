/**
 * @example attribute_maybe_unused_example.cpp
 * @brief A parameter read only in a build without NDEBUG, marked with SCL_MAYBE_UNUSED.
 *
 * The capacity is checked by an assertion in the function resize. No warning is issued about
 * the parameter left unread in a build with NDEBUG, where the assertion is removed.
 */

#include <scl/utility/attribute/maybe_unused.h>

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

//! [maybe_unused]
void resize(std::vector<int> & storage, std::size_t size, SCL_MAYBE_UNUSED std::size_t capacity)
{
    assert(size <= capacity);
    storage.resize(size);
}
//! [maybe_unused]

int main()
{
    std::vector<int> storage;

    resize(storage, 3, 8);
    std::cout << storage.size() << '\n'; // 3
}
