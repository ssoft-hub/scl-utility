# Thread support

Reports whether the standard library supports threads, and gates the ScL code that takes a
lock.

- Header: `#include <scl/utility/preprocessor/threads.h>`

Contents:
- [`SCL_HAS_THREADS`](#scl_has_threads)

---

## `SCL_HAS_THREADS`

Expands to `1` when the standard library supports threads, `0` otherwise.

- Header: `#include <scl/utility/preprocessor/threads.h>`
- Declaration: `#define SCL_HAS_THREADS /* 1 or 0 */`

### Semantics

- **Always defined:** interrogate it with `#if`, never `#ifdef`. A misspelled name is then
  reported by `-Wundef` instead of quietly reading as false.
- **Derived per library:** `_GLIBCXX_HAS_GTHREADS` (libstdc++), `_LIBCPP_HAS_NO_THREADS` or
  `_LIBCPP_HAS_THREADS` (libc++); every other library counts as supporting threads. A library
  built without threads, such as that of `arm-none-eabi`, declares no `std::mutex`, and its
  `std::thread` cannot start a thread. The standard macro `__STDCPP_THREADS__` reports the target
  rather than the library, and Clang defines it for `arm-none-eabi` as well, which is why this
  macro exists.
- **A report, not a switch:** defining it by hand does not add threads or take them away. It
  states how the standard library was built.
- **What it gates in ScL:** the cache of demangled names behind `type_name(obj)` and
  `type_short_name(obj)` takes a `std::mutex` only where it is `1`, and takes no lock where it is
  `0` - see [`type_name`](../runtime/type_name.md).

### Examples

```cpp
#include <scl/utility/preprocessor/threads.h>

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

## Notes

- [`SCL_HAS_RTTI`](rtti.md) and [`SCL_HAS_EXCEPTIONS`](exceptions.md) report on the translation
  unit, which the command line of the compiler decides. This macro reports on the standard
  library, which the toolchain fixes.
