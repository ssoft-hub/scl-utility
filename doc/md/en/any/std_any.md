# Std any

`<scl/utility/any/std_any.h>` reaches the object a `std::any` holds through the same
`scl::any_cast` spelling the rest of the group uses.

- Header: `#include <scl/utility/any/std_any.h>`

## Overview

A handle names the type it is bound to and nothing else. An [`any_view`](any_view.md), an
[`any_arg`](any_arg.md) or an [`any_mutable_view`](any_mutable_view.md) bound to a
`std::any` therefore refers to the box: `type_name()` answers `"std::any"`,
`any_cast<std::any>` answers the box itself, and a cast to any other type answers nothing.

This header is where the other direction lives, and it is the only header of the group that
names `std::any` at all. It reads what the box holds by specialising
[`any_cast_traits`](any_cast.md) for `std::any`, and a cast that never names a `std::any`
never instantiates that specialisation, so no cast pays for the ability. Including
`<scl/utility/any.h>` pulls the header in along with the rest of the group; what costs
nothing is the cast, not the include. The whole header is empty without RTTI, where
`std::any` cannot be named.

## Features

- Reaches the boxed object by pointer; a mismatch answers `nullptr` and throws nothing.
- Reaches it by copy, by `const` reference, or by reference for a non-`const` box; a mismatch
  throws `scl::bad_any_cast`. A box with no name at the call site hands its object over.
- Refuses a `volatile` box at the call, `std::any_cast` taking no pointer to one.
- Costs a cast that never names `std::any` nothing at all.
- Exists only in builds with RTTI; the reference form only where exceptions are available.

## API reference

### Casting

```cpp
std::any boxed{std::string{"Hello Any!"}};

scl::any_cast<std::string>(&boxed);        // std::string *, nullptr on a mismatch
scl::any_cast<std::string>(boxed);         // a copy of the stored string
scl::any_cast<std::string const &>(boxed); // the stored string, without a copy
scl::any_cast<int>(&boxed);                // nullptr
scl::any_cast<int>(boxed);                 // throws scl::bad_any_cast
```

The pointer form answers `nullptr` for a null pointer, an empty box and a box holding
another type alike. The box owns what it holds, so its own constness reaches the boxed
object: a non-`const` box answers `T *` and a `const` one `T const *`, exactly as
`std::any_cast` does. The request's own qualification takes no part in the match, so
`any_cast<T>` and `any_cast<T const>` reach the same object.

The copy form asks to read, so it is offered only where the type can be copied from a
`const` object. That is stricter than the letter of `std::any_cast<T>(std::any &)`, which
mandates construction from `U &` alone; libstdc++ requires the `const` form as well, and the
group's cast requires it for every source.

The reference form is declared only where
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `1`. With exceptions turned off the
pointer form remains, and it already reports a mismatch.

### Through a handle

```cpp
std::any boxed{std::string{"Hello Any!"}};
scl::any_view const view{boxed};

scl::any_cast<std::string>(&view);   // nullptr: the view names the box
scl::any_cast<std::any const>(&view); // the box itself
scl::any_cast<std::string>(&boxed);  // the string inside the box
```

Writing into the stored object goes through the box the handle hands out:

```cpp
scl::any_mutable_view const over_any{boxed};
std::any_cast<std::string &>(*scl::any_cast<std::any>(&over_any)) += "!";
```

An [`any_switch`](any_switch.md) chain takes `std::any` as a case type like any other: a
branch naming it matches the box, and none reads inside. Apply a chain to what a cast over
the box answers.

## See also

- [any_view](any_view.md), [any_arg](any_arg.md), [any_mutable_view](any_mutable_view.md) -
  the handles that bind a `std::any` as the box it is
- [any_switch](any_switch.md) - the chain that takes a box as a case type like any other
- [any](any.md) - the owner that needs none of this
- [any_cast](any_cast.md) - the cast itself and the trait behind it
- [Russian documentation](../../ru/any/std_any.md)
