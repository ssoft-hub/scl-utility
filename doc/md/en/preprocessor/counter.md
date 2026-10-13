# `SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT`

The macro `SCL_COUNTER_VALUE(Tag)` reads at compile time the counter of the tag `Tag`, and
the macro `SCL_COUNTER_NEXT(Tag)` adds one to that counter for the rest of the translation unit.

- Header: `#include <scl/utility/preprocessor/counter.h>`

```cpp
#define SCL_COUNTER_VALUE(Tag) /* constant expression of type std::uint_fast32_t */
#define SCL_COUNTER_NEXT(Tag)  /* declaration */
```

## Semantics

- **A counter per tag:** each type is the tag of a counter of its own, and two specializations
  of one class template are two tags. A class without a default constructor, an incomplete
  or abstract class, a function type, a reference and `void` are tags as well. A reference,
  a `const` type and an array are tags apart from, respectively, the type referred to, the type
  without `const` and a pointer to the element.
- **Value:** `SCL_COUNTER_VALUE(Tag)` is a constant expression of type `std::uint_fast32_t`. It
  reads `0` before the first `SCL_COUNTER_NEXT(Tag)` of the translation unit and one more after
  each of them.
- **Advance:** `SCL_COUNTER_NEXT(Tag)` expands to a declaration rather than a statement, so it
  stands at namespace or class scope, not in a function body.
- **Translation unit:** the count is kept in each translation unit apart. An inline function or
  a template that reads a counter in a header can read different values in different units, which
  breaks the one-definition rule.
- **Mechanism:** `SCL_COUNTER_NEXT` declares an overload of a helper function, and
  `SCL_COUNTER_VALUE` calls that function by its qualified name and assembles the count by
  overload resolution over the overloads declared above the call.

## Examples

The tags of the example:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp tags -->
```cpp
struct apples
{};

struct pears
{};
```

Two counters advanced apart:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp count -->
```cpp
static_assert(SCL_COUNTER_VALUE(apples) == 0);

SCL_COUNTER_NEXT(apples)
SCL_COUNTER_NEXT(apples)
SCL_COUNTER_NEXT(pears)

static_assert(SCL_COUNTER_VALUE(apples) == 2);
static_assert(SCL_COUNTER_VALUE(pears) == 1);
```

Two specializations of one class template as two tags:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp template_tag -->
```cpp
template <typename Type>
struct per_type
{};

SCL_COUNTER_NEXT(per_type<int>)

static_assert(SCL_COUNTER_VALUE(per_type<int>) == 1);
static_assert(SCL_COUNTER_VALUE(per_type<long>) == 0);
```

## See also

- [`example/preprocessor/counter/preprocessor_counter_example.cpp`](../../../../example/preprocessor/counter/preprocessor_counter_example.cpp)

---

Previous: [Preprocessor](index.md) | Next: [`SCL_FORWARD`](forward.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
