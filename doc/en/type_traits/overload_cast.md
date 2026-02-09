# overload_cast

Utility for disambiguating overloaded functions and member functions by argument types.

- Header: `#include <scl/utility/type_traits/overload_cast.h>`

Contents:
- [`overload_cast<Args...>`](#overload_cast-args)

---

## `overload_cast<Args...>`

Selects a specific overload of a function or member function pointer based on argument types.

- Header: `#include <scl/utility/type_traits/overload_cast.h>`
- Declaration: `template <class... Args> inline constexpr detail::overload_cast<Args...> overload_cast{};`

### Semantics

- **Callable object:** `overload_cast<Args...>` is a `constexpr` variable template whose `operator()` accepts a function pointer or pointer-to-member and returns it unchanged, with the compiler selecting the correct overload.
- **Qualifier support:** Handles all member function qualifier combinations:
  - cv-qualifiers: `const`, `volatile`, `const volatile`
  - ref-qualifiers: `&`, `&&`
  - `noexcept` specifier
- **Free functions:** Also works with free (non-member) function pointers.
- **Zero overhead:** The call operator is `constexpr` and `noexcept`; it simply returns the pointer.

### Motivation

Taking the address of an overloaded function normally requires a verbose `static_cast` to the exact function pointer type. `overload_cast` provides a concise, readable alternative:

```cpp
// Without overload_cast — verbose and error-prone
auto p = static_cast<void (Widget::*)(int)>(&Widget::update);

// With overload_cast — concise and clear
auto p = scl::overload_cast<int>(&Widget::update);
```

### Examples

```cpp
#include <scl/utility/type_traits/overload_cast.h>

struct Widget {
    void update(int);
    void update(double) const;
    void update(int, int) noexcept;
};

void process(int);
void process(double) noexcept;

// Member function overloads
auto p1 = scl::overload_cast<int>(&Widget::update);          // void(int)
auto p2 = scl::overload_cast<double>(&Widget::update);       // void(double) const
auto p3 = scl::overload_cast<int, int>(&Widget::update);     // void(int, int) noexcept

// Free function overloads
auto f1 = scl::overload_cast<int>(&process);                 // void(int)
auto f2 = scl::overload_cast<double>(&process);              // void(double) noexcept
```

### Typical Use Cases

- Passing overloaded member functions to `std::bind`, `std::invoke`, or signal/slot systems where the compiler cannot deduce which overload is intended.
- Connecting Qt signals to overloaded slots.
- Registering overloaded callbacks in reflection or scripting bindings.
