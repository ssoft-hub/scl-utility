# SCL_NO_UNIQUE_ADDRESS

Portable `[[no_unique_address]]` attribute for all supported compilers.

- Header: `#include <scl/utility/attribute/no_unique_address.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

## Purpose

`[[no_unique_address]]` (C++20) permits the compiler to give an empty member
sub-object zero storage by sharing its address with another member. Without it,
every member occupies at least one byte even if its type is empty, which breaks
the zero-overhead abstraction principle for policy- and executor-based designs.

The target ABI decides which spelling is available, not the compiler brand. Targeting
the MSVC ABI, only `[[msvc::no_unique_address]]` changes the layout; targeting the
Itanium ABI, only the standard spelling does. `SCL_NO_UNIQUE_ADDRESS` selects the form
the target accepts.

## Detection

The macro is selected via `__has_cpp_attribute`:

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(no_unique_address)` | `[[no_unique_address]]` |
| `__has_cpp_attribute(msvc::no_unique_address)` | `[[msvc::no_unique_address]]` |
| Neither | *(empty; the empty member is not collapsed)* |

Targeting the MSVC ABI, on MSVC, on clang-cl and on Clang alike, the first check
returns 0 and the vendor form is picked up by the second. Targeting the Itanium ABI,
GCC and Clang answer the first check and the vendor form is never reached.

## Usage

```cpp
#include <scl/utility/attribute/no_unique_address.h>

struct EmptyPolicy {};

struct Buffer {
    SCL_NO_UNIQUE_ADDRESS EmptyPolicy policy;
    int size;
    char data[64];
};

// The empty policy occupies no extra space
static_assert(sizeof(Buffer) == sizeof(int) + 64);
```

## Notes

- Applies to non-static data members only.
- Two members of the same type cannot share an address even with this attribute —
  the compiler will give each its own byte.
- The macro can be overridden before inclusion via `#define SCL_NO_UNIQUE_ADDRESS`.
