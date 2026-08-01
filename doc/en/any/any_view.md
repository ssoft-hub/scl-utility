# Any view

A non-owning, read-only view over a `std::any` or a typed lvalue.

- Header: `#include <scl/utility/any/any_view.h>`

## Overview

`scl::any_view` is to `std::any` what `std::string_view` is to `std::string`: it
refers to an existing value without copying it, so a function can accept a
heterogeneous argument for read access at no allocation cost. The view is two
pointers wide and trivially copyable.

A view is built over one of two backings. The *raw* backing wraps a typed lvalue
directly: it never depends on RTTI and carries its type identity through
a [`scl::type_key`](../meta/type_key.md), so `type_name()`, `type_key()`,
`has_value()` and construction are usable in constant evaluation. The *std::any*
backing wraps a `std::any` and delegates every cast to `std::any_cast`; because
`std::any` is itself RTTI-coupled, this backing exists only when RTTI is enabled.
Under `-fno-rtti` only the raw backing is compiled, and the `std::any`
constructor disappears with it. This only removes the library's own overload —
some standard libraries still declare `std::any` without RTTI. Passing one to a
view built this way binds the raw backing instead: `type_name()` still reports
`"std::any"`, but `any_cast` never unwraps the boxed object.

Access is read-only. `scl::any_cast` yields either a value copy or a `const`
lvalue reference; a non-`const` or rvalue reference cast is ill-formed, because the view never
grants write access to the object it observes.

The view does not own or extend the lifetime of the referenced object. It stays
valid only while that object lives — the same caveat as `std::string_view`.

## Features

- Non-owning, read-only, two pointers wide and trivially copyable
- Raw backing over any typed lvalue, with no RTTI dependency
- `std::any` backing that delegates to `std::any_cast` (RTTI builds only)
- Identity queries `type_name` / `type_key` / `has_value` `constexpr` on C++20
- Pointer cast `any_cast<T>(view*)` — `noexcept`, null on mismatch
- Reference / value cast `any_cast<T>(view&)` — throws `scl::bad_any_cast` on mismatch
- Implicit conversion from `std::any`, so one `any_cast` serves both backings

## API reference

### Construction

```cpp
std::string text{"Hello Any!"};

scl::any_view empty{};      // refers to no object
scl::any_view raw{text};    // raw backing: views the lvalue in place
scl::any_view alias = raw;  // trivially copyable
```

Only lvalues are accepted; binding to an rvalue would dangle and is rejected.
When RTTI is enabled a `std::any` converts implicitly, selecting the `std::any`
backing:

```cpp
std::any boxed{text};
scl::any_view over_any{boxed};   // std::any backing (RTTI builds only)
```

An rvalue `std::any` is likewise rejected, so the view never outlives its source.

### Observation

```cpp
std::string text{"Hello Any!"};
scl::any_view raw{text};

raw.has_value();   // true
raw.type_name();   // std::string_view naming std::string (raw backing); the exact
                   // spelling is the compiler's, e.g. "std::basic_string<char>"
raw.type_key();    // scl::type_key const * identifying std::string
```

`has_value()` reports `false` for an empty view and for a view over an empty
`std::any`. `type_name()` returns a `std::string_view` over the compile-time type
name: the type's own name for the raw backing, and `type_name<std::any>()` for the
`std::any` backing — that is, the name of the backing rather than of the type boxed
inside it.

`type_key()` returns a pointer rather than a reference, because a
[`scl::type_key`](../meta/type_key.md) is an identity with no empty value: an
empty view answers `nullptr`, which keeps all three states distinguishable. A
non-null result is the very object `scl::type_key_of<T>()` returns, so comparing
against it costs one pointer comparison, and `type_key()->name() == type_name()`
holds throughout:

```cpp
raw.type_key() == &scl::type_key_of<std::string>();   // true
scl::any_view{}.type_key();                           // nullptr
```

Probe the type inside a `std::any` with `any_cast<T>` naming
that type — not `any_cast<std::any>`, which asks "is there another `std::any`
boxed inside?" and normally answers `nullptr`.

### Casting

The pointer form returns a `const` pointer on a type match and `nullptr`
otherwise. It never throws:

```cpp
if (auto const * s = scl::any_cast<std::string>(&raw))
    use(*s);   // no copy
```

The reference form returns the object by value or by `const` reference, and
throws `scl::bad_any_cast` on a mismatch. A `const`-reference result binds with
no copy; a non-`const` or rvalue reference is ill-formed:

```cpp
std::string copy = scl::any_cast<std::string>(raw);                 // value copy
std::string const & ref = scl::any_cast<std::string const &>(raw);  // zero copy
// scl::any_cast<std::string &>(raw);   // ill-formed: the view is read-only
```

A `std::any` argument converts implicitly, so the same `scl::any_cast` reads both
a view and a bare `std::any`:

```cpp
std::any boxed{text};
scl::any_cast<std::string>(boxed);   // via implicit any_view conversion
```

`scl::bad_any_cast` derives from `std::bad_cast` in every configuration — never
from `std::bad_any_cast`, so a `catch (std::bad_cast const &)` handler catches
it regardless of build, and the type itself never depends on RTTI.

### Qualifiers

A view records the cv-qualification of what it was bound to, and a cast must carry
every qualifier the referent has — reaching it may add qualification, never discard
it. Reading supplies `const` on its own, so in practice only `volatile` has to be
asked for:

```cpp
int volatile sensor = 0;
scl::any_view const view{sensor};

scl::any_cast<int>(&view);                // nullptr: would discard volatile
scl::any_cast<int volatile>(&view);       // int const volatile *
scl::any_cast<int const volatile &>(view);// reads the volatile object
```

The reverse direction is always allowed, so a plain referent answers a `volatile`
request as well.

The same coverage rule applies to the view itself: a `volatile any_view` is a
qualifier the request must cover too, independent of the referent's own:

```cpp
int number = 0;
scl::any_view volatile view{number};
scl::any_cast<int>(&view);          // nullptr: the view itself is volatile
scl::any_cast<int volatile>(&view); // reads fine
```

### Constant evaluation

Identity queries — `type_name`, `type_key`, `has_value` and construction — are
`constexpr` and evaluate at compile time on the C++20 baseline. `any_cast`,
however, is a runtime operation there, exactly like `std::any_cast`.

That limit belongs to the binding rather than to type erasure. The view refers to
an lvalue of any type, so the only thing it can erase that lvalue's address to is
`void const *`, and recovering a typed pointer from `void` is constant-evaluable
only on compilers implementing P2738 (C++26). Erasing to a common base class
instead stays constant-evaluable on C++20 — but that requires choosing the layout
of what is stored, which only an owning backing can do. For a view over an
object it did not create, C++26 is the earliest the cast can fold.

### Identity across module boundaries

A raw-backed cast compares [`scl::type_key`](../meta/type_key.md) values. Within
one module that is a pointer comparison; a view that arrived from another module
falls back to comparing the key's contents, which stays exact — including for
two same-named types declared in an anonymous namespace of different translation
units, which the key's per-TU discriminator tells apart.

Two limits of the key carry over unchanged:

- A type declared at block scope — a local class or a closure type — is outside
  the key's contract and may falsely match a same-named namespace-scope type.
  Move such a type to namespace scope.
- A key must not outlive the module that produced it. Drop views built inside a
  shared library before unloading it.

## See also

- [type_key](../meta/type_key.md) — the identity key behind `type_key()`
- [Russian documentation](../../ru/any/any_view.md)
