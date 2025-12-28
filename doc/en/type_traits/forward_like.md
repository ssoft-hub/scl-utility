# Forward-like type traits

Utilities for forwarding types based on an object type’s cv/ref qualifiers.

- Header: `#include <scl/utility/type_traits/forward_like.h>`

Contents:
- [`forward_like_t<Base, Type>`](#forward_like-type)
- [`forward_like<Base, Type>`](#forward_like-method)

---

## forward_like_t<Base, Type>

Creates a type that mimics the cv-ref semantics of `std::forward_like`, combining qualifier overlay
with reference replacement.

This trait models the behavior of C++23 `std::forward_like`. It overlays cv-qualifiers from `Base`
onto `Type` (preserving `Type`'s own qualifiers) while completely replacing `Type`'s reference
category with that of `Base`.

- Header: `#include <scl/utility/type_traits.h>`
- Declaration: `template <typename Base, typename Type> using forward_like_t = ...;`

### Semantics

- **CV-Qualifier Application (Overlay):**
  The `const` and `volatile` qualifiers from `Base` are **added** to `Type`. Any existing
  cv-qualifiers on `Type` are preserved, resulting in a union.

- **Reference Application (Replacement):**
  The reference category (`&` or `&&`) is determined solely by `Base`. The original reference
  on `Type` is ignored, and reference collapsing does not apply.

- **Special Cases:**
  A reference is not applied if the underlying `Type` is `void`. For arrays, cv-qualifiers
  apply to the element type, and the reference applies to the array itself.

### Examples

```cpp
#include <scl/utility/type_traits/forward_like.h>

using ::scl::forward_like_t;

// CV-qualifiers are overlaid (union)
static_assert(std::is_same_v<forward_like_t<const int, volatile double>, const volatile double>);
static_assert(std::is_same_v<forward_like_t<int, const double>, const double>);

// Reference category is replaced (copied from Base)
static_assert(std::is_same_v<forward_like_t<int&, double&&>, double&>);      // & replaces &&
static_assert(std::is_same_v<forward_like_t<int&&, double&>, double&&>);      // && replaces &
static_assert(std::is_same_v<forward_like_t<int, double&>, double>);        // no-ref replaces &

// Hybrid Behavior
static_assert(std::is_same_v<forward_like_t<const int&, volatile double>, const volatile double&>);

// Special case for void
static_assert(std::is_same_v<forward_like_t<const int&, void>, const void>); // ref not applied

// Arrays
static_assert(std::is_same_v<forward_like_t<const int, int volatile[3]>, const volatile int[3]>);
static_assert(std::is_same_v<forward_like_t<int&&, int const(&)[3]>, int const(&&)[3]>);
```

### Typical Use Cases

Mimicking `std::forward_like` to provide consistent forwarding semantics, especially in code
aiming for compatibility with different C++ standard versions. It's useful when you need to
control the reference type explicitly while adding, but not removing, cv-qualifiers.

### See Also

- `forward_like` — The corresponding function that uses this trait for casting.

---

## forward_like<Base, Type>(t)

A utility function that forwards a value `t` as a reference with cv-ref qualifiers
determined by `forward_like_t`. It is a direct analog to C++23 `std::forward_like`.

- Header: `#include <scl/utility/type_traits.h>`
- Declaration: `template <typename Base, typename Type> constexpr decltype(auto) forward_like(Type&& t) noexcept;`

### Description

This function performs a `static_cast` on the input `t` to the type produced by
`forward_like_t<Base, Type&&>`. It perfectly forwards the value with the desired cv-ref
properties, making generic code cleaner and more expressive.

### Example

```cpp
#include <scl/utility/type_traits/firward_like.h>
#include <iostream>
#include <string>

// A function that processes a string, with behavior depending on qualifiers
void process(const std::string& str) { std::cout << "Processing const lvalue: " << str << std::endl; }
void process(std::string& str)       { std::cout << "Processing non-const lvalue: " << str << std::endl; }
void process(std::string&& str)      { std::cout << "Processing rvalue: " << str << std::endl; }

// A generic wrapper that forwards its argument `like` the template parameter `T`
template <typename T, typename U>
void wrapper(U&& arg)
{
    // Forward `arg` with qualifiers from T
    process(scl::forward_like<T>(arg));
}

int main()
{
    std::string s = "hello";
    const std::string cs = "world";

    wrapper<const int&>(s); // Forwards s as const lvalue ref
    wrapper<int&>(s);       // Forwards s as non-const lvalue ref
    wrapper<int&&>(std::move(s)); // Forwards a temporary as rvalue ref
    // wrapper<int&>(cs);      // Error: cannot cast away const.
}
```
The last line in `main` would fail to compile because `forward_like_t<int&, const std::string&>`
results in `const std::string&`. The `process` function does not have an overload that can bind a
`const std::string&` to a `std::string&` parameter. This demonstrates how `forward_like`
correctly preserves `const`-ness from the underlying value type.
