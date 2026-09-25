# Runtime

Names read from a value while the program runs: the type an object of a polymorphic class
was created as, and the number an enumeration value holds. The names a type has at compile
time are the [meta](../meta/type_name.md) group's.

- Header: `#include <scl/utility/runtime.h>`

| Page | Answers | RTTI |
|---|---|---|
| [`type_name(obj)`](type_name.md) | which class is this object, behind a pointer or a reference to a polymorphic base too | required |
| [`enum_string(value)`](enum_string.md) | which number does this value hold, named or not | not required |

| Question | Group |
|---|---|
| What is the name of the type `T` written here? | meta: `scl::type_name<T>()` |
| What is the name of the type this polymorphic object was created as? | runtime: `scl::type_name(obj)` |
| What is the name of the enumerator `V`? | meta: `scl::enum_name<V>()` |
| What number does this value hold? | runtime: `scl::enum_string(value)` |

## Without RTTI

`type_name(obj)` and `type_short_name(obj)` are declared only where `SCL_HAS_RTTI` is `1`,
and `enum_string` is declared in either build. Code that builds both ways branches on the
macro, which is always defined, and falls back to the name of the static type:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp no_rtti -->
```cpp
template <typename T>
::std::string name_of(T const & object)
{
#if SCL_HAS_RTTI
    return ::scl::type_name(object); // the dynamic type of a polymorphic object
#else
    static_cast<void>(object);
    return ::std::string{::scl::type_name<T>()}; // the static type only
#endif
}
```

[Back to the overview](../Main.md)
