# Any view

`scl::any_view` refers to an existing object whose type is known only at run time, and lets
a caller read it. The view neither creates, copies nor destroys that object: it holds its
address and the description of its type.

- Header: `#include <scl/utility/any/any_view.h>`

## Overview

`scl::any_view` relates to `std::any` the way `std::string_view` relates to
`std::string`. A function that has to read a value of an unknown type takes a view and does
not force the caller to build an owning object. Nothing is copied and nothing is allocated.

The view costs two pointers and is trivially copyable.

It refers to an object in one of two ways. In the first, the view refers to a typed object
directly: the type is known at compile time and identified by
[`scl::type_key`](../meta/type_key.md), so no RTTI is involved, and `type_name()`,
`type_key()`, `has_value()` and building the view itself all run during constant evaluation.
In the second, the view refers to a `std::any`, and every cast is delegated to the standard
`std::any_cast`. Since `std::any` requires RTTI, that constructor disappears from the
library when the build sets `-fno-rtti`.

There is a subtlety here. Some standard libraries declare `std::any` even with RTTI turned
off. Such an object then binds to the ordinary constructor of the view, that is, it is read
as a plain typed object. `type_name()` still answers `"std::any"`, but `any_cast` can no
longer reach the object stored inside it.

A view reads and never writes. `scl::any_cast` answers either a copy of the value or a const
reference to it. Asking for a non-const reference, or for an rvalue reference, does not
compile, because the view grants no write access.

The view owns nothing and extends no lifetime. It stays valid exactly as long as the object
it refers to - the same caveat `std::string_view` carries.

## Features

- Refers to an object without copying it, and grants no write access.
- Costs two pointers and is trivially copyable.
- Refers to a typed object of any type and uses no RTTI.
- Refers to a `std::any` and delegates the cast to the standard `std::any_cast`; that
  constructor exists only in builds with RTTI.
- Answers `type_name()`, `type_key()` and `has_value()` during constant evaluation as well,
  on the C++20 baseline.
- Hands out a pointer through `any_cast<T>(view *)`; a type mismatch answers `nullptr` and
  throws nothing.
- Hands out a value or a const reference through `any_cast<T>(view &)`; a type mismatch
  throws `scl::bad_any_cast`.
- Converts from `std::any` implicitly, so one `scl::any_cast` reads both a view and a
  `std::any`.

## API reference

### Construction

```cpp
std::string text{"Hello Any!"};

scl::any_view empty{};      // refers to no object
scl::any_view raw{text};    // refers to the object directly
scl::any_view alias = raw;  // trivially copyable
```

The view accepts an lvalue only, that is, a named object. Binding to a temporary is rejected
by the compiler: such a view would become invalid as soon as the expression ends.

In a build with RTTI a `std::any` converts to a view implicitly:

```cpp
std::any boxed{text};
scl::any_view over_any{boxed};   // refers to the std::any
```

A temporary `std::any` is refused as well, so the view never outlives the object it refers
to.

### Observation

```cpp
std::string text{"Hello Any!"};
scl::any_view raw{text};

raw.has_value();   // true
raw.type_name();   // the name of std::string; the exact spelling comes from the compiler,
                   // for instance "std::basic_string<char>"
raw.type_key();    // the scl::type_key identifying std::string
```

`has_value()` answers `false` in two cases: the view is empty, or it refers to an empty
`std::any`.

`type_name()` answers the name of the type known at compile time. When the view refers to an
object directly, that is the name of the object's own type. When it refers to a `std::any`,
that is the name of `std::any`, not the name of the type stored inside it.

`type_key()` answers a [`scl::type_key`](../meta/type_key.md) by value. An empty view
identifies no type at all and answers `scl::type_key{}`, the key that names nothing. The
equality `type_key().name() == type_name()` always holds:

```cpp
raw.type_key() == scl::type_key_of<std::string>();      // true
scl::any_view{}.type_key() == scl::type_key{};          // true
```

To learn the type stored inside a `std::any`, call `any_cast<T>` with that very type `T`.
A call to `any_cast<std::any>` answers the box itself - the object the view refers to, and
the type `type_name()` and `type_key()` already report for this backing. That is the way to
take a `std::any` out of a view when what it holds does not matter. A `std::any` nested
inside another one is reached with `std::any_cast` on the box that was answered.

### Casting

The pointer form answers a const pointer when the requested type matches the type of the
object, and `nullptr` otherwise. It throws nothing:

```cpp
if (auto const * s = scl::any_cast<std::string>(&raw))
    use(*s);   // no copy
```

The reference form answers the object by value or by const reference, and throws
`scl::bad_any_cast` when the type does not match. A reference result binds to the object
without copying it:

```cpp
std::string copy = scl::any_cast<std::string>(raw);                 // a copy of the value
std::string const & ref = scl::any_cast<std::string const &>(raw);  // no copy
// scl::any_cast<std::string &>(raw);   // does not compile: a view never writes
```

The reference form is declared only where
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `1`. With exceptions turned off the
pointer form remains, and it already reports a mismatch.

An argument of type `std::any` converts to a view implicitly, so the same `scl::any_cast`
call reads both a view and a `std::any` directly:

```cpp
std::any boxed{text};
scl::any_cast<std::string>(boxed);   // through the implicit conversion to any_view
```

`scl::bad_any_cast` derives from `std::bad_cast` in every configuration and never from
`std::bad_any_cast`. A handler written as `catch (std::bad_cast const &)` therefore catches
it whatever the build settings are, and the type itself does not depend on RTTI.

### The const and volatile qualifiers

A view remembers the qualifiers of the object it was bound to. A cast has to request all of
them: adding a qualifier is allowed, dropping one is not. Reading adds `const` on its own,
so in practice only `volatile` has to be requested:

```cpp
int volatile sensor = 0;
scl::any_view const view{sensor};

scl::any_cast<int>(&view);                 // nullptr: the request would drop volatile
scl::any_cast<int volatile>(&view);        // int const volatile *
scl::any_cast<int const volatile &>(view); // reads the volatile object
```

The other direction is always allowed: a request carrying `volatile` also passes for an
object without qualifiers.

The same rule covers the view itself. When the variable is declared `any_view volatile`,
that qualifier has to be requested too, whatever the qualifiers of the object are:

```cpp
int number = 0;
scl::any_view volatile view{number};

scl::any_cast<int>(&view);          // nullptr: the view itself is volatile
scl::any_cast<int volatile>(&view); // reads normally
```

### Constant evaluation

Building a view and asking it for `type_name()`, `type_key()` and `has_value()` all run
during constant evaluation, on the C++20 baseline. `any_cast`, however, runs at run time
there - exactly as `std::any_cast` does.

The limit comes from the way a view binds, not from type erasure as such. A view refers to
an object of any type, so the only way it can remember the address is as `void const *`, and
recovering a typed pointer from `void const *` becomes a constant expression only on
compilers that implement P2738 (C++26).

The other way - erasing to a common base class - is a constant expression on C++20 already,
but it needs an instance of that class to exist beside the object, and someone has to keep
that instance alive for as long as the type description lives. For a method parameter
[`any_arg`](any_arg.md#constant-evaluation) does exactly that: the caller creates such an
instance per binding, and a call outlives its own arguments by construction. A view, on the
other hand, can be stored for as long as one likes, and no one is there to provide that
instance. For a stored view over someone else's object, C++26 therefore remains the earliest
level at which the cast folds at compile time.

### Identity across module boundaries

A cast through a direct reference to an object compares
[`scl::type_key`](../meta/type_key.md) values. Within one module that is a pointer
comparison. A view that arrived from another module is compared by the content of the key,
and that comparison is exact: two same-named types from anonymous namespaces of different
translation units differ by the discriminator the key carries.

The two limits of the key apply here as well:

- A type declared inside a block - a local class or a closure type - falls outside the
  contract of the key and may match a same-named type from a namespace by mistake. Declare
  such a type at namespace scope.
- The key must not outlive the module that created it. Destroy views created inside a shared
  library before unloading it.

## See also

- [`example/any/common/any_common_example.cpp`](../../../../example/any/common/any_common_example.cpp) -
  a working example: one function reads both a view and an argument, casts in both forms and
  identity queries.
- [any_arg](any_arg.md) - a view for a method parameter that also grants write access
- [any_switch](any_switch.md) - a chain of branches that reads a value without a run of casts
- [type_key](../meta/type_key.md) - the identity key `type_key()` answers with
- [Russian documentation](../../ru/any/any_view.md)
