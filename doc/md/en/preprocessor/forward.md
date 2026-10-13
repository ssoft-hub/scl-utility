# `SCL_FORWARD`

The macro `SCL_FORWARD(...)` expands to its arguments unchanged, so a comma-separated list passes
through one parameter of another macro.

- Header: `#include <scl/utility/preprocessor/forward.h>`

```cpp
#define SCL_FORWARD(...) __VA_ARGS__
```

## Semantics

- **Expansion:** `SCL_FORWARD(a, b)` expands to `a, b`, with no parentheses added around
  the list.
- **An argument of another macro:** the preprocessor splits the arguments of a macro at every
  comma outside parentheses, so `SCL_FORWARD(a, b)` written as an argument of another macro stays
  one argument, and the parameter receives `a, b` once that argument is expanded.
- **`#` and `##`:** a parameter that the other macro applies `#` or `##` to receives
  the argument unexpanded, as `SCL_FORWARD(a, b)`.

## Examples

A list of arguments through one parameter:

<!-- snippet: example/preprocessor/forward/preprocessor_forward_example.cpp apply -->
```cpp
#define APPLY(function, arguments) function(arguments)

constexpr int add(int left, int right) noexcept { return left + right; }

static_assert(APPLY(add, SCL_FORWARD(1, 2)) == 3);
```

A type whose name holds a comma through one parameter:

<!-- snippet: example/preprocessor/forward/preprocessor_forward_example.cpp type_argument -->
```cpp
#define DECLARE_ALIAS(name, type) using name = type;

DECLARE_ALIAS(phone_book, SCL_FORWARD(std::map<std::string, int>))
```

## See also

- [`SCL_COUNTER_VALUE`](counter.md), which takes a tag through one parameter
- [`example/preprocessor/forward/preprocessor_forward_example.cpp`](../../../../example/preprocessor/forward/preprocessor_forward_example.cpp)

---

Previous: [`SCL_COUNTER_VALUE`](counter.md) | Next: [`SCL_HAS_EXCEPTIONS`](exceptions.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
