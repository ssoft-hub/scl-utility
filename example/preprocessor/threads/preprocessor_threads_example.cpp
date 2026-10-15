/**
 * @example preprocessor_threads_example.cpp
 * @brief Takes a lock only where the standard library supports threads.
 *
 * A standard library built without threads declares no std::mutex, so code built both for
 * a library with threads and for one without them branches on SCL_HAS_THREADS.
 */

#include <scl/utility/preprocessor/threads.h>

#include <iostream>

//! [next_ticket]
#if SCL_HAS_THREADS
#include <mutex>
#endif

int next_ticket()
{
    static int ticket = 0;
#if SCL_HAS_THREADS
    static std::mutex mutex;
    std::lock_guard const lock{mutex};
#endif
    return ++ticket;
}
//! [next_ticket]

int main()
{
    std::cout << next_ticket() << '\n';
    std::cout << next_ticket() << '\n';
}
