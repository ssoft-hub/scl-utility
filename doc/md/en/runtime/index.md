# Runtime

The runtime group holds function templates that read, while the program runs, the name of the
type an object of a polymorphic class was created as, and the number a value of an enumeration
type holds.

- Header: `#include <scl/utility/runtime.h>`

| Question | Group and function template | RTTI |
|---|---|---|
| What is the name of the type `T` written here? | meta: [`scl::type_name<T>()`](../meta/type_name.md) | not required |
| What is the name of the type this polymorphic object was created as? | runtime: [`scl::type_name(obj)`](type_name.md) | required |
| What is the name of the enumeration constant `V`? | meta: [`scl::enum_name<V>()`](../meta/enum_name.md) | not required |
| Which value does a variable of an enumeration type hold? | runtime: [`scl::enum_string(value)`](enum_string.md) | not required |

## Without RTTI

The function templates `type_name(obj)` and `type_short_name(obj)` are declared only where the
macro [`SCL_HAS_RTTI`](../preprocessor/rtti.md) is `1`. The function template `enum_string` is
declared with RTTI and without it. A caller whose code is built both ways should test the macro,
which is always defined, and take the name of the static type where RTTI is disabled:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp no_rtti -->
```cpp
template <typename T>
std::string name_of([[maybe_unused]] T const & object)
{
#if SCL_HAS_RTTI
    return scl::type_name(object); // the dynamic type of a polymorphic object
#else
    return std::string{scl::type_name<T>()}; // the static type only
#endif
}
```

## Performance

What one call costs, and which optimisation attribute the group carries:
[Runtime Benchmarks](benchmark.md).

---

Next: [`type_name(obj)`](type_name.md) | [Back to the overview](../Main.md)
