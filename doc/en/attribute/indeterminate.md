# SCL_INDETERMINATE

Opt-in to indeterminate (uninitialised) storage.

- Header: `#include <scl/utility/attribute/indeterminate.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_INDETERMINATE

Marks a variable declaration as having an indeterminate initial value,
explicitly opting out of any zero- or default-initialisation the compiler
would otherwise insert. This allows the compiler to skip the initialisation
instruction entirely, which can matter in tight loops or hot paths where a
scratch variable is always written before it is read.

Corresponds to the C++26 `[[indeterminate]]` attribute. On compilers that do
not yet support it the macro expands to nothing — the variable is simply
initialised normally, with no undefined behaviour and no performance gain.

Place before the type in a local variable declaration.

> **Safety:** Reading the variable before writing to it is **undefined
> behaviour**. Use only when the variable is unconditionally assigned before
> its first read on every code path.

### Detection

| Condition | Expansion |
|-----------|-----------|
| `__has_cpp_attribute(indeterminate)` (C++26) | `[[indeterminate]]` |
| None of the above | *(empty — normal initialisation)* |

### Usage

```cpp
#include <scl/utility/attribute/indeterminate.h>

void encode(const float* src, std::uint8_t* dst, std::size_t n) {
    SCL_INDETERMINATE std::uint8_t byte;  // written on every iteration
    for (std::size_t i = 0; i < n; ++i) {
        byte = static_cast<std::uint8_t>(src[i] * 255.0f);
        dst[i] = byte;
    }
}
```

---

## Notes

- On compilers without `[[indeterminate]]` support the fallback is empty:
  the code compiles and runs correctly, just without the optimisation hint.
- Unlike `SCL_ASSUME`, there is no runtime expression involved — the attribute
  only affects whether initialisation code is emitted.
- The macro can be overridden before inclusion via `#define SCL_INDETERMINATE`.
