# SCL_UNSEQUENCED / SCL_REPRODUCIBLE

Pure-function annotation macros.

- Header: `#include <scl/utility/attribute/unsequenced.h>`
- Aggregator: `#include <scl/utility/attribute.h>`

---

## SCL_UNSEQUENCED

Marks a function as having **no observable side effects** and reading **no
mutable global or pointed-to state**. Its return value depends solely on its
arguments. The compiler may freely deduplicate, hoist, reorder, or eliminate
calls.

Corresponds to the C++26 `[[unsequenced]]` attribute and the GCC/Clang
`[[gnu::const]]` / `__attribute__((const))` extensions.

**Form — wrapping macro:** pass the entire return-type and declarator as the
argument.

```cpp
SCL_UNSEQUENCED(int square(int x)) { return x * x; }
SCL_UNSEQUENCED(std::size_t popcount(std::uint64_t v));
```

The function body `{ ... }` is **not** part of the macro argument, so the
macro can correctly place the attribute in the position each form requires:

| Compiler / standard | Expansion |
|---------------------|-----------|
| C++26 | `int square(int x) [[unsequenced]] { ... }` |
| GCC / Clang | `[[gnu::const]] int square(int x) { ... }` |
| Older GCC / Clang | `__attribute__((const)) int square(int x) { ... }` |
| MSVC / unknown | `int square(int x) { ... }` (no annotation) |

### Detection

| Condition | Form | Attribute position |
|-----------|------|--------------------|
| `__has_cpp_attribute(unsequenced)` (C++26) | `__VA_ARGS__ [[unsequenced]]` | after parameter list |
| `__has_cpp_attribute(gnu::const)` (GCC, Clang) | `[[gnu::const]] __VA_ARGS__` | before return type |
| `__has_attribute(const)` (GCC, Clang older) | `__attribute__((const)) __VA_ARGS__` | before return type |
| None of the above | `__VA_ARGS__` | *(no annotation)* |

### Constraints

A `SCL_UNSEQUENCED` function must:
- Not modify any state observable outside the function.
- Not read any mutable global or thread-local variable.
- Not read memory through pointer arguments (use `SCL_REPRODUCIBLE` instead).
- Not call non-`SCL_UNSEQUENCED` functions (in general).

### Usage

```cpp
#include <scl/utility/attribute/unsequenced.h>

SCL_UNSEQUENCED(int square(int x)) { return x * x; }

SCL_UNSEQUENCED(std::size_t popcount(std::uint64_t v));

// Declaration and definition may both carry the annotation
SCL_UNSEQUENCED(double fast_rsqrt(float x));
SCL_UNSEQUENCED(double fast_rsqrt(float x)) { /* ... */ }
```

---

## SCL_REPRODUCIBLE

Marks a function as having **no observable side effects** but which **may read
mutable global or pointer-reachable memory**. Calls with identical visible state
may be deduplicated, but the compiler cannot hoist them past writes to memory
the function might read.

Corresponds to the C++26 `[[reproducible]]` attribute and the GCC/Clang
`[[gnu::pure]]` / `__attribute__((pure))` extensions. Weaker than
`SCL_UNSEQUENCED`.

**Form — wrapping macro:** same as `SCL_UNSEQUENCED`.

### Detection

| Condition | Form | Attribute position |
|-----------|------|--------------------|
| `__has_cpp_attribute(reproducible)` (C++26) | `__VA_ARGS__ [[reproducible]]` | after parameter list |
| `__has_cpp_attribute(gnu::pure)` (GCC, Clang) | `[[gnu::pure]] __VA_ARGS__` | before return type |
| `__has_attribute(pure)` (GCC, Clang older) | `__attribute__((pure)) __VA_ARGS__` | before return type |
| None of the above | `__VA_ARGS__` | *(no annotation)* |

### Constraints

A `SCL_REPRODUCIBLE` function must:
- Not modify any state observable outside the function.
- May read global variables, `errno`, or memory through pointer arguments.
- Not call functions with side effects.

### Usage

```cpp
#include <scl/utility/attribute/unsequenced.h>

SCL_REPRODUCIBLE(std::size_t strlen_safe(const char* s));

SCL_REPRODUCIBLE(bool is_sorted(const int* data, std::size_t n));
```

---

## Comparison

| Macro | Reads global/ptr memory | Reorderable | C++26 standard |
|-------|------------------------|-------------|----------------|
| `SCL_UNSEQUENCED` | No | Yes | `[[unsequenced]]` |
| `SCL_REPRODUCIBLE` | Yes (read-only) | Limited | `[[reproducible]]` |

---

## Notes

- Both macros imply **no writes** to externally observable state; violations
  are undefined behaviour.
- Do not pass arguments with side effects: `f(++i)` may have the increment
  elided if `f` is annotated.
- Each macro can be overridden before inclusion:
  `#define SCL_UNSEQUENCED(...) __VA_ARGS__` or
  `#define SCL_REPRODUCIBLE(...) __VA_ARGS__`.
