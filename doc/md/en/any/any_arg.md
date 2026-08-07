# Any arg

A parameter-only view over a `std::any`, a typed value, or an existing
`any_view`.

- Header: `#include <scl/utility/any/any_arg.h>`

## Overview

`scl::any_arg` is the parameter-position companion of
[`scl::any_view`](any_view.md): it binds lvalues **and** rvalues of any
constness, and is valid only for the duration of the call it is passed into. A
function takes it as a by-value parameter to accept a heterogeneous argument —
including a temporary — at no allocation cost. Like a view, it is two pointers
wide and trivially copyable, and unlike a view its casts are constant-evaluable
(see [Constant evaluation](#constant-evaluation)).

The two types split the roles `std::string_view` tries to combine. `any_view`
is *storable*: it may be kept in a field or container, so it accepts only
lvalues — a bound temporary would dangle. `any_arg` is *parameter-only*: a
temporary argument outlives the call by construction, so rvalues are welcome,
and in exchange the object must not be kept beyond the call.

Access-wise the pair resembles `reference`/`const_reference`: a `const any_arg`
reads exactly like an `any_view` — it is literally the same qualifier-coverage
rule. Still, `any_view` is not "`const any_arg`", and constness cannot replace
it:

- Top-level `const` on a by-value parameter is not part of the function type —
  `void f(any_arg)` and `void f(any_arg const)` have the same signature, so a
  read-only contract spelled with `const` is invisible to the caller. The
  `f(any_view)`/`f(any_arg)` distinction is one the type system actually
  checks; `const` is not.
- `any_view`'s rvalue rejection is a compile error on every compiler. A
  `const any_arg` binds a temporary, and the dangling diagnostic
  (`SCL_LIFETIMEBOUND`) exists only on Clang — GCC and MSVC compile it
  silently.
- A storable handle needs assignment, which `const` forbids, while a non-`const`
  `any_arg` field would reopen write access.

The backings are those of `any_view`: a typed value forms the RTTI-free raw
backing, a `std::any` forms the std::any backing (RTTI builds only), and an
`any_view` contributes its referent — the `any_arg` refers to the same object,
not to the view. Identity queries behave identically.

Access is wider than a view's. `any_view` is read-only always; `any_arg` also
grants write access, but only when the argument was bound to an object carrying
no cv-qualifiers. That is the same qualifier-coverage rule `any_view` applies to
reads — a cast must carry every qualifier the referent has — and writing carries
none.

Storage is discouraged at the API level: `any_arg` has no default constructor
and no assignment. The copy constructor remains, since by-value parameter
passing requires it. Constructor parameters are lifetime-bound
(`SCL_LIFETIMEBOUND`), so Clang reports an `any_arg` escaping the scope of its
referent.

## Features

- Binds lvalues and rvalues of any constness — safe in parameter position
- Non-owning, two pointers wide and trivially copyable
- Same backings and identity queries as `any_view`; `constexpr` on C++20
- Pointer cast `any_cast<T>(arg*)` — `noexcept`, null on mismatch
- Reference / value cast `any_cast<T>(arg)` — throws `scl::bad_any_cast` on
  mismatch
- Casts are constant-evaluable on C++20 where the argument is a parameter, and in any
  position once the compiler implements P2738 (C++26)
- Write access through `any_cast<T>(arg*)` or `any_cast<T &>` on an unqualified
  referent
- Implicit conversion to `any_view` for delegation; no default state, no
  assignment

## API reference

### Construction

```cpp
void foo(scl::any_arg value);

std::string text{"Hello Any!"};
foo(text);                        // lvalue, raw backing
foo(std::string{"temporary"});    // rvalue — outlives the call, OK
foo(std::any{text});              // temporary std::any (RTTI builds only)

scl::any_view view{text};
foo(view);                        // adopts the referent of an existing view
```

An `any_arg` cannot be declared without an argument: there is no default
constructor, and since there is no assignment either, such an instance would
stay empty forever — nothing could ever fill it. Copying is available: the copy
constructor is what by-value parameter passing runs on. A binding is made once,
at construction, and does not change afterwards. `has_value()` can still
return `false`, though: an empty `std::any`, or an `any_arg` adopting the
referent of an empty `any_view`, reports the emptiness of the *contents* — an
argument was passed, it just refers to an empty box.

### Observation

```cpp
void foo(scl::any_arg value)
{
    value.has_value();
    value.type_name();   // the type's name for the raw backing, "std::any" for that one
    value.type_key();    // scl::type_key const * identifying that type
}
```

Identity works exactly as it does for a view, including across a module boundary:
see [any_view](any_view.md#identity-across-module-boundaries).

### Reading

The pointer form returns `Type *` on a type match and `nullptr` otherwise; the
returned pointer refers to the argument's object. Ask for `const` to read, and a
`const` referent answers too:

```cpp
if (auto const * s = scl::any_cast<std::string const>(&value))
    use(*s);   // no copy
```

The reference form returns a value copy or a `const` reference, throwing on a
mismatch:

```cpp
std::string copy = scl::any_cast<std::string>(value);                 // copy out
std::string const & ref = scl::any_cast<std::string const &>(value);  // zero copy
```

To keep the value beyond the call, copy it out — never keep the `any_arg`
itself or an `any_view` converted from it when the argument was a temporary.

### Writing

An argument takes the request as written. The pointer form returns `Type *`, so
the qualification you ask for is the qualification you get — and you get it only
when the request covers what the referent was bound with:

```cpp
int *       w = scl::any_cast<int>(&value);       // writable referent only
int const * r = scl::any_cast<int const>(&value); // also a const referent
```

That differs from `any_view`, which only ever reads: there the request may name
any qualification but the result always gains `const`.

```cpp
void bump(scl::any_arg value)
{
    if (auto * number = scl::any_cast<int>(&value))
        ++*number;                              // the write reaches the caller
}

int counter = 0;
bump(counter);            // counter == 1
int const frozen = 0;
bump(frozen);             // refused: nullptr, the referent is const
```

The reference form spells the same choice with a reference type — `any_cast<int &>`
writes, `any_cast<int const &>` reads — and throws `scl::bad_any_cast` where the
pointer form answers `nullptr`. It is the one qualifier-coverage rule throughout:
a cast must carry every qualifier the referent has, and writing carries none, so
only an unqualified referent grants it.

The std::any backing behaves the same way: a write reaches the boxed object when
the `std::any` itself was non-`const`. A `volatile std::any` is not supported —
`std::any` has no volatile-qualified members and `std::any_cast` takes no
volatile pointer.

The coverage rule applies to the `any_arg` itself too: its own `const`/`volatile`
are qualifiers the request must cover, same as the referent's. A `const any_arg`
therefore never writes, even over an unqualified referent:

```cpp
int value = 0;
scl::any_arg const arg{value};
scl::any_cast<int>(&arg);         // nullptr: the handle is const, request doesn't cover it
scl::any_cast<int const>(&arg);   // reads fine
```

A `volatile any_arg` symmetrically requires `volatile` in the request, regardless
of the referent's own qualifiers.

Write access is unreachable through an `any_view`. The implicit
`any_view` → `any_arg` conversion does not open it, so the view stays read-only
as documented.

### Constant evaluation

Unlike a view's, an argument's casts fold at compile time on the C++20 baseline:

```cpp
constexpr int doubled(scl::any_arg value)
{
    auto const * number = scl::any_cast<int const>(&value);
    return (number != nullptr) ? *number * 2 : 0;
}

static_assert(doubled(21) == 42);
```

What makes this work is an *anchor*: recovering a typed pointer from `void const *`
is not a constant expression before P2738 (C++26), while casting down to a derived
class is. An anchor is such a derived class — a descriptor that also carries the
typed pointer — and the constructor takes one the caller materialises per binding, so
`any_cast` reaches the referent by casting the descriptor down. Being a descriptor is
what keeps the type two pointers wide: the pointer a view already spends on a
descriptor serves both roles.

The anchor is a temporary of the caller, so it lives exactly as long as the call it was
made for — which is what an argument is, and why a view cannot do the same. **Only
constant evaluation takes it.** At run time the constructor stores the static descriptor
instead, so an `any_arg` in any position — a local variable, a member — behaves exactly
as it would have without the mechanism.

That bounds what compile-time evaluation can answer:

- The argument must be a **parameter**. For an `any_arg` that is a local variable, a
  member or an element of an aggregate, the anchor is already gone, so a cast — and an
  identity query with it — stops with a diagnostic instead of answering wrongly. Only
  Clang diagnoses this reliably; GCC and MSVC extend the temporary further and accept
  such code, so do not read their acceptance as a guarantee.
- There is no `constexpr any_arg` **variable**: its value would hold a pointer to a
  temporary, which no constant expression may do. Wrap the work in a `constexpr`
  function instead.
- The std::any backing and a referent adopted from an `any_view` never anchor, so casts
  on those stay run-time, exactly as the view's do.
- None of this reaches run time. There, every position works and every query answers,
  the width is the same two pointers, and the cost is the same as a view's.

One toolchain quirk is worth knowing, measured on GCC 13.1: it rejects comparing the
address of a temporary with `nullptr` as non-constant when the enclosing constant
expression sits at namespace scope. During constant evaluation the descriptor *is* such
a temporary, and every cast tests it, so the whole assertion is refused there. The same
code in block scope is accepted; Clang and MSVC accept either placement. Put such an
assertion inside a function:

```cpp
void check()
{
    static_assert(doubled(21) == 42);   // block scope: accepted everywhere
}
```

### Once the compiler folds the cast itself

Every bound above belongs to the anchor, not to the type. Where `__cpp_constexpr`
reports P2738 (C++26), the header takes the other branch: the anchor is not compiled at
all, the constructor stores the same static descriptor a view stores, and the cast
recovers the referent from `void const *` directly. Nothing changes at the call site,
and the list above lifts entirely — a cast then folds for an `any_arg` in any position,
over a referent adopted from an `any_view`, and at namespace scope on every compiler,
GCC's quirk included. `any_view` gains constant-evaluable casts at the same moment and
for the same reason, with no change to its own code.

Neither the width nor the layout of `any_arg` depends on the standard; what the branch
changes is the declaration of one constructor parameter that no caller ever passes. As
with the standard library itself, build one binary against one `-std` throughout.

The `AnyArgTest.ConstexprCastReachesAnyPositionOnceTheVoidCastFolds` case covers this
branch. It reports as *skipped* on a C++20 build and turns into real assertions the
moment the project is compiled against C++26.

### Delegation

```cpp
void inner(scl::any_view value);

void foo(scl::any_arg value)
{
    inner(value);   // implicit any_arg -> any_view conversion
}
```

## See also

- [any_view](any_view.md) — the storable companion
- [any_switch](any_switch.md) — a branch chain over the same subject, one branch per type
- [Russian documentation](../../ru/any/any_arg.md)
