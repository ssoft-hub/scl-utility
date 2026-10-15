# `SCL_HAS_THREADS`

The macro `SCL_HAS_THREADS` expands to `1` where the standard library supports threads and to `0`
where it is built without them.

- Header: `#include <scl/utility/preprocessor/threads.h>`

```cpp
#define SCL_HAS_THREADS /* 1 or 0 */
```

## Semantics

- **Always defined:** a caller tests the macro with `#if` rather than `#ifdef`. Under `#if`
  a misspelled name reads as `0` and draws a `-Wundef` warning; under `#ifdef` it reads as false
  with no warning.
- **Source:** the value of the macro follows the configuration of the library:
  `_GLIBCXX_HAS_GTHREADS` (libstdc++), `_LIBCPP_HAS_NO_THREADS` or `_LIBCPP_HAS_THREADS` (libc++).
  Every other library counts as supporting threads.
- **A library without threads:** a library built without threads, such as the library of
  the `arm-none-eabi` target, declares no `std::mutex`, and its `std::thread` cannot start
  a thread. The value of the standard macro `__STDCPP_THREADS__` follows the target rather than
  the library, and Clang defines it for `arm-none-eabi` as well.
- **Not a switch:** the value of the macro follows how the standard library was built, and
  threads are not added or removed by defining the macro by hand. No option of the compiler
  listed on the [group page](index.md#compiler-options) changes it.

## Examples

A function that takes a lock only where the library supports threads:

<!-- snippet: example/preprocessor/threads/preprocessor_threads_example.cpp next_ticket -->
```cpp
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
```

## See also

- [`SCL_HAS_RTTI`](rtti.md) and [`SCL_HAS_EXCEPTIONS`](exceptions.md), whose values
  follow the translation unit rather than the library
- [`example/preprocessor/threads/preprocessor_threads_example.cpp`](../../../../example/preprocessor/threads/preprocessor_threads_example.cpp)

---

Previous: [`SCL_HAS_RTTI`](rtti.md) | [Back to the group](index.md) | [Back to the main page](../Main.md)
