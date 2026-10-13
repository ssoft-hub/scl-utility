# `is_tu_local<T>`

The class template `is_tu_local<T>` tells at compile time whether the type `T` is local to its
translation unit, and the variable template `is_tu_local_v<T>` holds its answer.

- Header: `#include <scl/utility/meta/tu_local.h>`

```cpp
namespace scl
{
    template <typename T>
    struct is_tu_local;

    template <typename T>
    inline constexpr bool is_tu_local_v = is_tu_local<T>::value;
}
```

## Semantics

- **Value:** the class template derives from `std::true_type` where `T`, with `const` and `volatile`
  dropped, is declared in an unnamed namespace or is built of such a type, and from
  `std::false_type` otherwise. A type declared in an unnamed namespace has internal linkage, and
  a declaration of the same name in another translation unit declares another type.
- **Types built of a local type:** a pointer, a reference and an array of a local type are local,
  and so are a function type and a pointer to a member whose types include one, and
  a specialization of a class template with type parameters over one.
- **Types classified by their name:** a specialization of a class template with a parameter that
  is not a type, and a function type with a cv-qualifier, a ref-qualifier or an ellipsis, are
  classified by the name the compiler spells for them. A string or a character that a template
  argument holds is not read as part of that name.
- **Precondition:** `T` is neither a local class nor a closure type, nor a type built of
  one. Compilers spell those without a reliable mark of locality.

## Examples

The types of the example:

<!-- snippet: example/meta/is_tu_local/meta_is_tu_local_example.cpp types -->
```cpp
namespace
{
    struct duck
    {};
} // namespace

struct goose
{};
```

The answers:

<!-- snippet: example/meta/is_tu_local/meta_is_tu_local_example.cpp is_tu_local -->
```cpp
static_assert(scl::is_tu_local_v<duck>);
static_assert(scl::is_tu_local_v<duck const *>);
static_assert(scl::is_tu_local_v<std::vector<duck>>);
static_assert(!scl::is_tu_local_v<goose>);
static_assert(!scl::is_tu_local_v<int>);
```

## See also

- [`scl::type_key_of<T>()`](type_key.md), whose key tells local types of one name apart
- [`example/meta/is_tu_local/meta_is_tu_local_example.cpp`](../../../../example/meta/is_tu_local/meta_is_tu_local_example.cpp)

---

Previous: [`type_name<T>()`](type_name.md) | Next: [`type_key`](type_key.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
