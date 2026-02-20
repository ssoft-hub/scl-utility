# Symbol name utilities

Compile-time name extraction for functions, methods, and class data members.

- Header: `#include <scl/utility/meta/symbol.h>`

Contents:
- [`symbol_name<S>`](#symbol_name-s)
- [`symbol_short_name<S>`](#symbol_short_name-s)

---

## `symbol_name<S>`

Retrieves the qualified string name of a symbol (global function, member function, or data member).

This utility provides a unified way to extract identifiers for various C++ entities passed as
non-type template parameters.

- Header: `#include <scl/utility/meta/symbol.h>`
- Declaration: `template <auto S> constexpr std::string_view symbol_name() noexcept;`

### Semantics

- **Aggressive Cleanup:**
  The utility automatically removes compiler-specific "noise" such as:
  - Address-of operators (`&`).
  - Surrounding parentheses and extra whitespace (common in MSVC for global variables).
  - Type information injected into the signature by MSVC.
- **Member Support:**
  Works for both pointer-to-member-functions (`&Class::Method`) and
  pointer-to-data-members (`&Class::Field`).

### Examples

```cpp
#include <scl/utility/meta/symbol.h>

namespace sys {
    void initialize() {}
    struct Clock {
        int ticks;
        void reset() {}
    };
}

// Global/Namespaced function
static_assert(scl::symbol_name<sys::initialize>() == "sys::initialize");

// Member function
static_assert(scl::symbol_name<&sys::Clock::reset>() == "sys::Clock::reset");

// Data member (Property)
static_assert(scl::symbol_name<&sys::Clock::ticks>() == "sys::Clock::ticks");
```

### Typical Use Cases

Implementation of serialization frameworks, automatic command-line argument mapping, or debuggers
where the string name of a class field or function is required at compile-time.

---

## `symbol_short_name<S>`

Retrieves only the terminal identifier of the symbol `S`.

- Header: `#include <scl/utility/meta/symbol.h>`
- Declaration: `template <auto S> constexpr std::string_view symbol_short_name() noexcept;`

### Example

```cpp
#include <scl/utility/meta/symbol.h>

struct Processor {
    void execute() {}
};

int main() {
    // Returns "execute" instead of "Processor::execute"
    constexpr auto name = scl::symbol_short_name<&Processor::execute>();
}
```
