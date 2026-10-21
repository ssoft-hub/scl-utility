# `SCL_NO_UNIQUE_ADDRESS`

An empty member marked with `SCL_NO_UNIQUE_ADDRESS` may share its address with another member, and
then occupies no storage of its own.

- Header: `#include <scl/utility/attribute/no_unique_address.h>`

```cpp
#define SCL_NO_UNIQUE_ADDRESS /* see below */
```

## Semantics

- **What the compiler is told:** the member needs no address distinct from the other members.
  The compiler may place an empty member at the address of another, and then no byte of the class is
  given to it.
- **The spelling by target:** `__has_cpp_attribute(no_unique_address)` is `0` on MSVC, and on Clang
  22, clang-cl included, for the MSVC ABI. The layout changes there only under
  `[[msvc::no_unique_address]]`. GCC and Clang take the standard spelling for the Itanium ABI.
- **Two members of one type:** two members of one type keep distinct addresses under the macro, yet
  either may still share its address with a member of another type.
- **Place:** the macro is written before the declaration of a non-static data member.

| Condition, first match | Expansion |
|---|---|
| `__has_cpp_attribute(no_unique_address)` | `[[no_unique_address]]` |
| `__has_cpp_attribute(msvc::no_unique_address)`, `msvc` not a macro | `[[msvc::no_unique_address]]` |
| none of the above | nothing, and the empty member takes a byte |

The macro expands to `[[no_unique_address]]` on GCC and to `[[msvc::no_unique_address]]` on MSVC. It
expands to `[[no_unique_address]]` on Clang 22 for the Itanium ABI, and to
`[[msvc::no_unique_address]]` for the MSVC ABI.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_NO_UNIQUE_ADDRESS` | `[[no_unique_address]]`, empty | `[[no_unique_address]]`, empty; for the MSVC ABI `[[msvc::no_unique_address]]`, empty | `[[msvc::no_unique_address]]`, empty |

## Examples

A class whose empty member occupies no storage of its own:

<!-- snippet: example/attribute/no_unique_address/attribute_no_unique_address_example.cpp no_unique_address -->
```cpp
struct no_policy
{};

struct counter
{
    SCL_NO_UNIQUE_ADDRESS no_policy policy;
    int value;
};

static_assert(sizeof(counter) == sizeof(int));
```

## See also

- [`example/attribute/no_unique_address/attribute_no_unique_address_example.cpp`](../../../../example/attribute/no_unique_address/attribute_no_unique_address_example.cpp)

---

Previous: [`SCL_NORETURN`](noreturn.md) | Next: [`SCL_UNSEQUENCED`](unsequenced.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
