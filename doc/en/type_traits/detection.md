# Detection Idiom

A C++20 implementation of the Detection Idiom based on
[N4502](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf).

- Header: `#include <scl/utility/type_traits/detection.h>`

## Overview

The Detection Idiom provides a clean, standardized way to detect the validity
of expressions at compile-time using SFINAE (Substitution Failure Is Not An Error). This is particularly useful for:

- Detecting member functions, types, or variables
- Implementing concept-like constraints before C++20 concepts
- Writing generic code that adapts to type properties
- Trait detection and introspection

## Features

- Full N4502 compliance
- C++20 compatible
- Header-only library
- Zero runtime overhead

## API Reference

### Basic Detection

#### `is_detected<Operation, Arguments... >`

Checks if `Operation<Arguments...>` is a valid expression.

**Returns:** `std::true_type` or `std::false_type`

```cpp
template <typename T>
using has_size_t = decltype(std:: declval<T>().size());

static_assert(scl::is_detected<has_size_t, std::vector<int>>::value);
```

#### `is_detected_v<Operation, Arguments...>` (C++17)

Variable template version of `is_detected`.

**Returns:** `bool`

```cpp
if constexpr (scl::is_detected_v<has_size_t, MyType>) {
    // Type has size() member
}
```

### Type Extraction

#### `detected_t<Operation, Arguments...>`

Extracts the type of `Operation<Arguments...>`, or `void` if ill-formed.

```cpp
template <typename T>
using value_type_t = typename T:: value_type;

using VecValueType = scl::detected_t<value_type_t, std::vector<int>>;
// VecValueType is int
```

#### `detected_or_t<Default, Operation, Arguments...>`

Extracts the type of `Operation<Arguments...>`, or `Default` if ill-formed.

```cpp
using ValueType = scl::detected_or_t<void, value_type_t, MyType>;
// ValueType is MyType:: value_type if it exists, otherwise void
```

### Advanced Detection

#### `is_detected_exact<Expected, Operation, Arguments...>`

Checks if `Operation<Arguments...>` results in exactly the `Expected` type.

```cpp
static_assert(
    scl::is_detected_exact_v<int, value_type_t, std::vector<int>>
);
```

#### `is_detected_convertible<To, Operation, Arguments... >`

Checks if `Operation<Arguments...>` is convertible to `To`.

```cpp
static_assert(
    scl:: is_detected_convertible_v<double, value_type_t, std::vector<int>>
);
```

## Usage Examples

### Example 1: Detecting Member Functions

```cpp
template <typename T>
using has_serialize_t = decltype(std::declval<T>().serialize());

template <typename T>
void process(const T& obj) {
    if constexpr (scl::is_detected_v<has_serialize_t, T>) {
        obj.serialize();
    } else {
        // Fallback behavior
    }
}
```

### Example 2: Detecting Member Types

```cpp
template <typename T>
using value_type_t = typename T:: value_type;

template <typename Container>
auto sum(const Container& c) {
    using ValueType = scl::detected_or_t<int, value_type_t, Container>;
    ValueType result{};
    for (const auto& elem : c) {
        result += elem;
    }
    return result;
}
```

### Example 3: Type Traits

```cpp
template <typename T>
using has_begin_end_t = decltype(
    std::declval<T>().begin(),
    std::declval<T>().end()
);

template <typename T>
inline constexpr bool is_iterable_v = scl::is_detected_v<has_begin_end_t, T>;

template <typename T>
requires is_iterable_v<T>
void print(const T& container) {
    for (const auto& elem : container) {
        std::cout << elem << " ";
    }
}
```

### Example 4: Detecting Operators

```cpp
template <typename T, typename U>
using has_plus_t = decltype(std::declval<T>() + std::declval<U>());

template <typename T, typename U>
inline constexpr bool is_addable_v = scl::is_detected_v<has_plus_t, T, U>;

template <typename T, typename U>
auto safe_add(const T& a, const U& b) {
    if constexpr (is_addable_v<T, U>) {
        return a + b;
    } else {
        return T{};
    }
}
```

### Example 5: Concept-like Constraints (pre-C++20)

```cpp
template <typename T>
using is_container = std::conjunction<
    scl::is_detected<has_begin_end_t, T>,
    scl::is_detected<value_type_t, T>,
    scl::is_detected<has_size_t, T>
>;

template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

template <typename T, std::enable_if_t<is_container_v<T>, int> = 0>
void process_container(const T& container) {
    // Implementation
}
```

## Performance

The Detection Idiom is a **compile-time only** facility with: 
- Zero runtime overhead
- No code generation
- Resolved entirely during compilation

## Comparison with C++20 Concepts

While C++20 Concepts provide a more powerful and expressive way to constrain
templates, the Detection Idiom remains useful for:

- **Type introspection**:  Extracting types with `detected_t`
- **Backward compatibility**: Supporting pre-C++20 codebases
- **Metaprogramming**: Complex SFINAE patterns

```cpp
// Concept version (C++20)
template <typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

// Detection Idiom version
template <typename T>
using has_size_t = decltype(std::declval<T>().size());

template <typename T>
inline constexpr bool has_size_v = 
    scl::is_detected_convertible_v<std::size_t, has_size_t, T>;
```

## References

- [N4502 - The Detection Idiom](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf)
- [Library Fundamentals TS v2](https://en.cppreference.com/w/cpp/experimental/lib_extensions_2)
