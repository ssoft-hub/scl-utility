# Any arg

`scl::any_arg` refers to an existing object whose type is known only at run time, and is
meant for a method parameter. A function takes it by value to read - and sometimes to
modify - an argument of an unknown type, copying nothing and allocating nothing.

- Header: `#include <scl/utility/any/any_arg.h>`

## Overview

`scl::any_arg` complements [`scl::any_view`](any_view.md). Both refer to someone else's
object, cost two pointers and are trivially copyable, but they serve different places.

A view can be stored: put in a field, in a container, returned from a function. That is why
it accepts a named object only. Were it to bind a temporary, it would outlive that temporary
and become invalid.

An argument cannot be stored, and in exchange it binds both a named object and a temporary
one. A temporary passed into a call lives until that call ends, so a parameter uses it
safely.

Hence the question: could one type serve both roles, with the difference expressed by
`const`, as `any_arg const`? No, for three reasons.

First, `const` on a by-value parameter is not part of the type of the function. The
declarations `void f(any_arg)` and `void f(any_arg const)` describe the same function, so
the signature does not tell the caller whether the function reads the argument or writes to
it. The type system checks the difference between `f(any_view)` and `f(any_arg)`; it does
not check a difference in `const`.

Second, `any_view` refuses a temporary on every compiler, while `const any_arg` binds one.
The warning that the reference will dangle comes from Clang alone; GCC and MSVC compile such
code silently.

Third, a `const any_arg` cannot be put in a field: a field needs assignment, and `const`
forbids it. A non-const `any_arg` in a field would open write access again.

`any_arg` refers to the same things a view does: to a typed object directly, without RTTI,
or to a `std::any` - the latter in builds with RTTI only. A view passed to it hands over its
object, so the argument refers to that object rather than to the view. Identity queries
behave identically on both types.

The access rights of an argument are wider. A view only reads. An argument always reads, and
writes when it was bound to an object carrying neither `const` nor `volatile`.

The interface itself discourages storing an argument: `scl::any_arg` has neither a default
constructor nor assignment. The copy constructor remains, because passing a parameter by
value needs it. The constructor parameters carry a lifetime-binding attribute, so Clang
warns when an argument leaves the scope where the object it refers to lives.

## Features

- Binds both a named and a temporary object of any constness - safe for a parameter.
- Owns nothing, costs two pointers and is trivially copyable.
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
it. Copying is available, since the copy constructor is what passes a parameter by value.
The binding is fixed once, at construction, and never changes afterwards.

`has_value()` may still answer `false`. That is what an argument bound to an empty
`std::any`, or one that took the object of an empty view, answers: the argument was passed,
but its source holds no content.

### Observation

```cpp
void foo(scl::any_arg value)
{
    value.has_value();
    value.type_name();   // the name of the object's type; "std::any" for a std::any
    value.type_key();    // scl::type_key const * identifying that type
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

To keep a value past the call, copy it out. Do not hold the `any_arg` itself, or a view
obtained from it, when the argument was bound to a temporary.

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

```cpp
void bump(scl::any_arg value)
{
    if (auto * number = scl::any_cast<int>(&value))
        ++*number;                              // the write is visible to the caller
}

int counter = 0;
bump(counter);            // counter becomes 1
int const frozen = 0;
bump(frozen);             // no write happens: the object is const, the cast answers nullptr
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

Coverage applies to the argument itself, not only to the object. The `const` and `volatile`
qualifiers of an `any_arg` variable have to be requested as well. A `const any_arg`
therefore grants no write, even when the object inside carries no qualifiers at all:

```cpp
int value = 0;
scl::any_arg const arg{value};

scl::any_cast<int>(&arg);         // nullptr: the argument is const, the request misses it
scl::any_cast<int const>(&arg);   // reads normally
```

A `volatile any_arg` symmetrically demands `volatile` in the request, whatever the
qualifiers of the object are.

A view grants no write at all. The implicit conversion from `any_arg` to `any_view` does not
open one, so a view stays strictly read-only.

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
static type description, so an `any_arg` in any position - a local variable, a field -
behaves exactly as it would without this machinery.

The limits follow from that.

- The argument has to be a **parameter**. If an `any_arg` is declared as a local variable, a
  field or a member of an aggregate, the anchor is already destroyed by the time of the
  cast. The cast - and with it the identity query - then stops with a compiler diagnostic
  rather than answering something wrong. Only Clang diagnoses this reliably: GCC and MSVC
  extend the lifetime of the temporary further and accept such code, so do not take their
  agreement as a guarantee.
- A `constexpr` variable of type `any_arg` does not exist: its value would hold a pointer to
  a temporary, which a constant expression forbids. Write the work as a `constexpr`
  function instead.
- A `std::any`, and an object taken over from a view, carry no anchor, so their casts run at
  run time - exactly as a view's do.
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

An argument is passed on as an argument: it is copyable, and the copy refers to the same
object.

```cpp
void inner(scl::any_arg value);

void foo(scl::any_arg value)
{
    inner(value);   // a copy of the argument, the same object
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
- [any_switch](any_switch.md) - a chain of branches over the same value, one branch per type
- [Russian documentation](../../ru/any/any_arg.md)
