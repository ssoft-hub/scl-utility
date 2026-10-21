# `SCL_LIFETIMEBOUND`

The macro `SCL_LIFETIMEBOUND` is written on a parameter whose object the result of the function
refers to. Clang, and MSVC in code analysis, warn where the result outlives that object.

- Header: `#include <scl/utility/attribute/lifetimebound.h>`

```cpp
#define SCL_LIFETIMEBOUND /* see below */
```

## Semantics

- **What the compiler is told:** the reference, pointer or view the function returns refers to
  the object the parameter refers to. The same is stated of `*this` on a member function.
- **What Clang reports:** Clang reports the warning `-Wdangling` where the result outlives
  the object, such as a `std::string_view` initialised from
  `first_word(std::string{"hello world"})`, whose argument is destroyed at the end of the full
  expression.
- **What MSVC reports:** MSVC reports warning C26815 where the result outlives the object, in code
  analysis with `/analyze /analyze:plugin EspXEngine.dll`. MSVC reports nothing without that plugin.
- **GCC:** the macro expands to nothing on GCC. GCC issues its warning `-Wdangling-reference`, part
  of `-Wall`, by an analysis of its own, and that analysis is the same with the macro and without
  it.
- **Place:** the macro is written after the declarator of a parameter, or after the qualifiers of
  a member function.

| Condition, first match | Expansion |
|---|---|
| MSVC other than clang-cl, `__has_cpp_attribute(msvc::lifetimebound)`, neither `msvc` nor `lifetimebound` a macro | `[[msvc::lifetimebound]]` |
| `__has_cpp_attribute(_Clang::__lifetimebound__)` (Clang) | `[[_Clang::__lifetimebound__]]` |
| `__has_attribute(__lifetimebound__)` | `__attribute__((__lifetimebound__))` |
| none of the above | nothing |

The macro expands to `[[msvc::lifetimebound]]` on MSVC 19.44, to `[[_Clang::__lifetimebound__]]` on
Clang and to nothing on GCC.

## Definitions made before the header

A definition of a macro made before its header is kept only as one of the expansions below for
the compiler at hand. The build with any other text ends with an error, as described on the [group
page](index.md#replacing-a-macro).

| Macro | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_LIFETIMEBOUND` | empty | `[[_Clang::__lifetimebound__]]`, `__attribute__((__lifetimebound__))`, empty | `[[msvc::lifetimebound]]`, empty |

## Examples

A view into the string passed to the function:

<!-- snippet: example/attribute/lifetimebound/attribute_lifetimebound_example.cpp lifetimebound -->
```cpp
std::string_view first_word(std::string const & text SCL_LIFETIMEBOUND)
{
    return std::string_view{text}.substr(0, text.find(' '));
}
```

## See also

- [`example/attribute/lifetimebound/attribute_lifetimebound_example.cpp`](../../../../example/attribute/lifetimebound/attribute_lifetimebound_example.cpp)

---

Previous: [`SCL_INDETERMINATE`](indeterminate.md) | Next: [`SCL_LIKELY`](likely.md) |
[Back to the group](index.md) | [Back to the main page](../Main.md)
