# `SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT`

The macro `SCL_COUNTER_VALUE(Tag)` reads the counter of the tag `Tag` at compile time, and
the macro `SCL_COUNTER_NEXT(Tag)` adds one to it for the rest of the translation unit.

- Header: `#include <scl/utility/preprocessor/counter.h>`

```cpp
#define SCL_COUNTER_VALUE(...) /* constant expression of type std::uint_fast32_t */
#define SCL_COUNTER_NEXT(...)  /* declaration */
```

## Semantics

- **Value:** `SCL_COUNTER_VALUE(Tag)` is a constant expression of type `std::uint_fast32_t`. It is
  `0` before the first `SCL_COUNTER_NEXT(Tag)` of the translation unit and one more after each
  of them.
- **Placement:** `SCL_COUNTER_NEXT(Tag)` appears at the global namespace scope only, never
  in a function body, a class or a namespace the caller declares.
- **Inside a template:** `SCL_COUNTER_VALUE(Tag)` in a template gives the count where the macro
  is written, not where the template is instantiated. This holds for any tag, a tag that depends
  on a template parameter included.
- **What a tag is:** every type is the tag of a counter of its own, a class without a default
  constructor, an incomplete or abstract class, a function type, a reference and `void` included.
- **Tags kept apart:** each pair below names two counters:
  - two specializations of one class template, such as `per_type<int>` and `per_type<long>`;
  - a reference and the type it refers to, such as `int &` and `int`;
  - a `const` type and the type without `const`, such as `int const` and `int`;
  - an array and a pointer to its element, such as `int[3]` and `int *`.
- **Spelling of the tag:** the tag is the whole argument list of the macro, so a name with commas,
  such as `std::pair<int, char>`, is written as is.
- **Full name of the tag:** writing every name in the tag fully qualified from `::`, such as
  `::std::pair<::lamp, int>`, is recommended. Where a name of the tag conflicts with
  a name declared in another scope, the program fails to compile, or the counter of another tag
  is read or advanced with no diagnostic.
- **Translation unit:** each translation unit keeps its own count. An inline function or a template
  in a header that reads a counter can get different values in different translation units, which
  breaks the one-definition rule. Reading and advancing a counter within one source file is
  recommended.

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

A tag written fully qualified from `::`, read in a namespace that declares the same name:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp full_name -->
```cpp
struct lamp
{};

namespace shop
{
    struct lamp
    {};
} // namespace shop

SCL_COUNTER_NEXT(::lamp)

namespace shop
{
    static_assert(SCL_COUNTER_VALUE(::lamp) == 1);
    static_assert(SCL_COUNTER_VALUE(lamp) == 0); // shop::lamp, another tag
} // namespace shop
```

## See also

- [`example/preprocessor/counter/preprocessor_counter_example.cpp`](../../../../example/preprocessor/counter/preprocessor_counter_example.cpp)

---

Previous: [Preprocessor](index.md) | Next: [`SCL_FORWARD`](forward.md) | [Back to the group](index.md) |
[Back to the main page](../Main.md)
