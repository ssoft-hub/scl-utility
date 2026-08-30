# Any arg

`scl::any_arg` refers to an existing object without naming its type, and is meant for a
method parameter. A function declares a parameter with it to read - and
sometimes to modify - an argument of an unknown type, copying nothing and allocating
nothing.

- Header: `#include <scl/utility/any/any_arg.h>`

## Overview

`scl::any_arg` complements [`scl::any_view`](any_view.md). Both refer to someone else's
object and cost two pointers, but they serve different places.

A view can be stored: put in a field, in a container, returned from a function. That is why
it accepts a named object only. Were it to bind a temporary, it would outlive that temporary
and become invalid.

An argument cannot be stored, and in exchange it binds both a named object and a temporary
one. A temporary passed into a call lives until that call ends, so a parameter uses it
safely.

### An argument arrives as a reference

`scl::any_arg` is not a class but a name for the reference `scl::any_argument const &`. The
class is called `scl::any_argument`, and a parameter is written with the short name:

```cpp
void foo(scl::any_arg value);
```

The reference is what turns the ban on storing into a rule of the language. A container of
references cannot be declared, so `std::vector<scl::any_arg>` and
`std::optional<scl::any_arg>` do not compile. The class itself forbids copying and moving,
so neither a data member of that type nor `auto copy = value;` compiles either. No copy is
needed: passing an argument on binds one more reference to the same object.

Narrowing the rights with `const` does not work. A qualifier written on a name that already
stands for a reference is dropped, so `scl::any_arg const` is the same `scl::any_arg`, and
Clang reports it as `-Wignored-reference-qualifiers`. A function that only reads declares
its parameter as [`scl::any_view`](any_view.md), and then the restriction is visible to the
caller, because the types differ.

Two openings the language still leaves: a reference data member,
`scl::any_argument const & kept;`, and a function returning `scl::any_arg`. Any C++
reference behaves that way. The parameters of the constructors that bind an object
directly carry a lifetime-binding attribute, so Clang warns when an argument leaves the
scope where the object it refers to lives. Only the constructor taking a view carries
none: the argument adopts that view's referent and a static descriptor, so the view may
die first and a temporary view is a valid source. To keep the value, copy it out.

`any_arg` refers to the same things a view does: to a typed object directly, without RTTI,
or to a `std::any` - the latter in builds with RTTI only. A view passed to it hands over its
object, so the argument refers to that object rather than to the view. Identity queries
behave identically on both types.

The access rights of an argument are wider. A view only reads. An argument always reads, and
writes when it was bound to an object carrying neither `const` nor `volatile`. Nothing else
affects the rights: an argument always arrives through a const reference, so its own
constness takes no part in the check.

## Features

- Binds both a named and a temporary object of any constness - safe for a parameter.
- Owns nothing and costs two pointers.
- Refuses to be stored: a container of references cannot be declared, and the class behind
  the short name forbids copying and moving.
- Refers to the same sources as a view and answers the same identity queries; they run
  during constant evaluation on the C++20 baseline.
- Hands out a pointer through `any_cast<T>(arg *)`; a type mismatch answers `nullptr` and
  throws nothing.
- Hands out a value or a reference through `any_cast<T>(arg)`; a type mismatch throws
  `scl::bad_any_cast`.
- Grants a write through `any_cast<T>(arg *)` or `any_cast<T &>` when the object carries no
  qualifiers.
- Folds casts at compile time on C++20 already when it stands in a parameter position, and
  in any position on a compiler that implements P2738 (C++26).
- Does not convert to `scl::any_view` at all: a view may be stored, an argument may not.

## API reference

### Construction

```cpp
void foo(scl::any_arg value);

std::string text{"Hello Any!"};
foo(text);                        // named object, referred to directly
foo(std::string{"temporary"});    // the temporary outlives the call, which is allowed
foo(std::any{text});              // a temporary std::any, builds with RTTI only

scl::any_view view{text};
foo(view);                        // takes the object of the view, not the view itself
```

An `any_arg` cannot be declared without an argument: there is no default constructor. There
is no assignment either, so an empty instance would stay empty forever - nothing could fill
it. The binding is fixed once, at construction, and never changes afterwards.

`has_value()` answers `false` only when the argument refers to nothing at all - one adopted
from an empty view, for instance. An argument bound to a `std::any` answers `true` even
when that `std::any` holds nothing: the argument still refers to a value, the box itself.

### Observation

```cpp
void foo(scl::any_arg value)
{
    value.has_value();
    value.type_name();   // the name of the object's type; "std::any" for a std::any
    value.type_key();    // the scl::type_key identifying that type
}
```

Identity behaves exactly as it does for a view, including comparison across a module
boundary: see [any_view](any_view.md#identity-across-module-boundaries).

### Reading

The pointer form answers `Type *` when the requested type matches the type of the object,
and `nullptr` otherwise. The pointer leads to the caller's object. To read, request a type
carrying `const`: such a request also passes for a const object:

```cpp
if (auto const * s = scl::any_cast<std::string const>(&value))
    use(*s);   // no copy
```

The reference form answers a copy of the value or a const reference, and throws when the
type does not match:

```cpp
std::string copy = scl::any_cast<std::string>(value);                 // a copy handed out
std::string const & ref = scl::any_cast<std::string const &>(value);  // no copy
```

The reference and value forms are declared only where
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `1`. With exceptions turned off the
pointer form remains, and it already reports a mismatch.

To keep a value past the call, copy it out.

### Writing

An argument takes the request literally: the pointer form answers `Type *` with the
qualification that was asked for. The request passes only when it covers the qualifiers of
the object:

```cpp
int *       w = scl::any_cast<int>(&value);       // an object without qualifiers only
int const * r = scl::any_cast<int const>(&value); // a const object as well
```

This is where an argument differs from a view. A view only reads, so a request there may
name any qualification, but the result always comes back with `const`.

<!-- snippet: example/any/common/any_common_example.cpp write_through_argument -->
```cpp
static void bump(::scl::any_arg value)
{
    if (auto * number = ::scl::any_cast<int>(&value))
        ++*number;
    else
        ::std::cout << "  refused: the referent is not a writable int\n";
}
```

The reference form expresses the same choice through the reference type: `any_cast<int &>`
writes, `any_cast<int const &>` reads. Where the pointer form answers `nullptr`, the
reference form throws `scl::bad_any_cast`.

One rule holds for the whole group: a cast has to request every qualifier of the object. A
write requests none of them, which is why only an object without qualifiers grants one.

The same rule governs a reference to a `std::any`: the write reaches the object inside when
the `std::any` itself is non-const. A `volatile std::any` is not supported, because
`std::any` has no `volatile`-qualified members and `std::any_cast` does not accept a
pointer to `volatile`.

Coverage looks at the binding alone. An argument's own representation takes no part in it:
an argument always arrives through a const reference, and were that constness added to the
qualifiers of the object, no write would ever pass.

A view grants no write at all. An argument that took the object of an
[`any_view`](any_view.md) receives it narrowed to reading, so a view stays strictly
read-only.

### Constant evaluation

The casts of an argument run during constant evaluation on C++20 already, which a view
cannot do:

```cpp
constexpr int doubled(scl::any_arg value)
{
    auto const * number = scl::any_cast<int const>(&value);
    return (number != nullptr) ? *number * 2 : 0;
}

static_assert(doubled(21) == 42);
```

This works through a helper object the library calls an anchor. An anchor is a type
description that additionally carries a typed pointer to the object. Recovering a typed
pointer from `void const *` is not a constant expression before P2738 (C++26), while casting
a pointer down a class hierarchy is one today. The anchor is exactly such a derived class:
the constructor of the argument takes an anchor the caller creates per binding, and
`any_cast` reaches the object by casting the type description down. The width of two
pointers is preserved: the pointer the argument already spends on a type description serves
both roles.

An anchor is a temporary of the caller, so it lives exactly as long as the call it was made
for. A function parameter is precisely such an entity, which is why a view - storable for as
long as one likes - cannot work this way.

The anchor takes part in constant evaluation only. At run time the constructor records the
static type description, so a local variable of type `any_arg` behaves exactly as it would
without this machinery.

The limits follow from that.

- The argument has to be a **parameter** while the anchor is the default one. If an
  `any_arg` is declared as a local variable bound to an argument built on the spot, that
  anchor is already destroyed by the time of the cast. The cast - and with it the identity
  query - then stops with a compiler diagnostic rather than answering something wrong. Only
  Clang diagnoses this reliably: GCC and MSVC extend the lifetime of the temporary further
  and accept such code, so do not take their agreement as a guarantee. Declare the anchor
  yourself - [`scl::any_anchor`](any_anchor.md) - and the position stops mattering.
- A `constexpr` variable of type `any_arg` does not exist: its value would hold a pointer to
  a temporary, which a constant expression forbids. Write the work as a `constexpr`
  function instead.
- An argument over an [`scl::any`](any.md) reads during constant evaluation in any position
  and with no anchor: the owner keeps the object inside a holder of its own, and the cast
  comes back down to it.
- A `std::any` cannot be built during constant evaluation at all, so its casts run at run
  time. An object taken over from a view reads during constant evaluation exactly when the
  view itself can.
- None of this concerns run time. There every position works and every request answers, the
  width is the same two pointers, and the cost equals that of a view.

One compiler quirk is worth knowing, measured on GCC 13.1. It treats a comparison of the
address of a temporary against `nullptr` as non-constant when the enclosing constant
expression sits at namespace scope. During constant evaluation the type description is
exactly such a temporary, and every cast checks it, so GCC rejects the whole check. The same
code inside a function body it accepts. Clang and MSVC accept either placement:

```cpp
void check()
{
    static_assert(doubled(21) == 42);   // inside a function body every compiler accepts it
}
```

### Once the compiler folds the cast itself

Every limit above belongs to the anchor, not to the type. When `__cpp_constexpr` reports
support for P2738 (C++26), the header takes another branch: the anchor is not compiled at
all, the constructor records the same static type description a view does, and the cast
recovers the object from `void const *` directly.

Nothing changes at the call site, and the limits above are lifted entirely. The cast folds
for an `any_arg` in any position, for an object taken over from a view, and at namespace
scope on every compiler, GCC included. A view gains compile-time casts at the same moment
and for the same reason, without a single edit of its own code.

Neither the width nor the layout of `any_arg` depends on the standard: the branch changes
the declaration of one constructor parameter the caller never passes. As with the standard
library, build one binary throughout with a single `-std`.

The branch is covered by
`AnyArgTest.ConstexprCastReachesAnyPositionOnceTheVoidCastFolds`. On a C++20 build the test
is reported as skipped, and it turns into real checks as soon as the project is built as
C++26.

### Passing the value on

An argument is passed on as an argument: the next call binds one more reference to the same
object, and no copy is made.

```cpp
void inner(scl::any_arg value);

void foo(scl::any_arg value)
{
    inner(value);   // one more reference to the same object
}
```

An argument does not convert to an `any_view` - neither a named one nor a temporary:

```cpp
scl::any_view stored = value;   // does not compile
```

The two types differ in the right to be stored. A view may be stored, which is why it
refuses to bind a temporary object in the first place. An argument may not be stored: it is
valid only within the call it was passed to, and the object it refers to may be a temporary
the caller created. The conversion would hand out a storable view naming an object whose
lifetime ends with the call.

A function that only reads a value for the duration of a call therefore declares an
`any_arg` parameter. A function that may be trusted to store the handle declares an
`any_view`, and the caller builds one itself, out of an object it owns.

## See also

- [`example/any/common/any_common_example.cpp`](../../../../example/any/common/any_common_example.cpp) -
  a working example: the argument binds a temporary, grants the write a view does not, and
  folds its casts at compile time.
- [any_view](any_view.md) - the view that can be stored
- [any_mutable_view](any_mutable_view.md) - the storable view granting write access
- [any_anchor](any_anchor.md) - the anchor that frees an argument from the parameter position
- [any_switch](any_switch.md) - a chain of branches over the same value, one branch per type
- [any](any.md) - the owning type both handles read
- [Russian documentation](../../ru/any/any_arg.md)
