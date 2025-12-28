# SCL_FORWARD

Identity variadic macro that forwards tokens unchanged and can be used to
encourage expansion in macro-chaining scenarios.

- Header: `#include <scl/utility/preprocessor/forward.h>`
- Declaration: `#define SCL_FORWARD(...) __VA_ARGS__`

## Purpose

- Forward comma-separated tokens into another macro, function call, or template
  parameter list without altering tokenization.
- Encourage expansion when a macro produces comma-separated tokens that need to
  be consumed by another macro.

This macro introduces no runtime overhead; it operates purely at preprocessing
time.

## Usage

```cpp
#include <scl/preprocessor.h>

// Forwarding function arguments
#define CALL(fn, ...) fn(__VA_ARGS__)
int add(int a, int b) { return a + b; }
int result = CALL(add, SCL_FORWARD(1, 2)); // -> add(1, 2)

// Forcing expansion of a macro that yields a type list
#define BASIC_TYPES int, long, double
template <typename... Ts>
struct type_list {};
using TL = type_list<SCL_FORWARD(BASIC_TYPES)>; // -> type_list<int, long, double>

// Chaining macros that produce comma-separated tokens
#define MAKE_PAIR(a, b) a, b
using P = std::pair<SCL_FORWARD(MAKE_PAIR(int, double))>; // -> std::pair<int, double>

// Forwarding arguments through an intermediate macro
#define FORWARD_TO_EMPLACE(obj, ...) obj.emplace_back(__VA_ARGS__)
std::vector<std::pair<int, std::string>> v;
FORWARD_TO_EMPLACE(v, SCL_FORWARD(42, "answer")); // -> emplace_back(42, "answer")
```

## Notes and pitfalls

- SCL_FORWARD does not add parentheses or change token grouping. If grouping is
  required, parenthesize at call sites.
- It does not turn multiple arguments into “one argument”; the preprocessor
  still splits by commas at the outer macro level.
- Useful in macro chains where an intermediate expansion is needed before the
  next macro consumes the tokens.
