# SCL_MAYBE_UNUSED

Suppress unused-entity warnings.

- Header: `#include <scl/utility/attribute/maybe_unused.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_MAYBE_UNUSED

Suppresses compiler warnings about unused variables, function parameters,
functions, and types. Use it for entities that are intentionally unused in
some build configurations (e.g., debug-only variables, platform-specific
parameters, `assert`-only values).

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(maybe_unused)` (C++17) | `[[maybe_unused]]` |
| `__has_attribute(unused)` (GCC, Clang) | `__attribute__((unused))` |
| None of the above | *(empty — warning may still fire)* |

### Usage

```cpp
#include <scl/utility/attribute/maybe_unused.h>

// Parameter unused in release builds (only referenced inside assert)
void set_size(std::size_t n, SCL_MAYBE_UNUSED std::size_t capacity) {
    assert(n <= capacity);
    storage_.resize(n);
}

// Function compiled away in release builds
SCL_MAYBE_UNUSED static void dump_state() { ... }

// Variable only read in debug mode
SCL_MAYBE_UNUSED bool ok = init();
assert(ok);
```

---

## Notes

- `SCL_MAYBE_UNUSED` suppresses the warning unconditionally; prefer it over
  `(void)var` casts when the entity may be used conditionally across builds.
- Applying it to a type suppresses warnings about all variables of that type.
- The macro can be overridden before inclusion via `#define SCL_MAYBE_UNUSED`.
