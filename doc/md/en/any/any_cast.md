# Any cast

`scl::any_cast` is the one cast the Any group offers. What differs between the types it
serves is stated by `scl::any_cast_traits`, which a type outside the library may specialise
to join.

- Header: `#include <scl/utility/any/any_cast.h>`

## Overview

Reading an erased value takes two decisions, and they are made in different places.

The **result type** follows from the spelling asked for and the source's access rule, and
nothing else. A source that reads adds `const` to every request, so
[`any_view`](any_view.md) answers `T const *` whatever the caller spelled. A source that
writes adds nothing, so [`any_mutable_view`](any_mutable_view.md) and
[`any_arg`](any_arg.md) answer `T *`. An [`any`](any.md) is neither: it keeps the object as
a subobject of its own, so its own constness reaches the object and a `const` owner answers
`T const *`.

**Whether anything comes back** follows from the binding. A request answers only where it
covers every qualifier the object was bound with, so a request naming neither `const` nor
`volatile` asks to write and reaches only an object bound without either. The two decisions
are independent: a request may name a type the source can spell and still answer `nullptr`
because the binding carries a qualifier the request left out.

The trait states the first decision. The second is one rule and applies to every source.

## Features

- One cast for every source: a handle, an owner, a `std::any`, or a type outside the library.
- The request spelling decides the result type; the binding decides whether it is answered.
- Hands out a pointer without throwing, binds a reference without copying, or copies out.
- Turns away a type that specialises nothing at the call, not inside the cast.
- Opens to a new type through one specialisation, with no overload added to `namespace scl`.

## API reference

### The shared entry points

```cpp
int value = 42;
scl::any_mutable_view const source{value};

scl::any_cast<int>(&source);        // int *, nullptr on a mismatch, never throws
scl::any_cast<int &>(source);       // binds the object, no copy
scl::any_cast<int>(source);         // a copy of the object
```

The pointer form answers `nullptr` for a null pointer, a type that does not match, and a
qualifier the request does not cover alike. The two reference forms throw
`scl::bad_any_cast` where the pointer form would answer `nullptr`, and are declared only
where [`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `1`.

The binding form is lifetime-bound: the result refers into the object, which the source
alone keeps reachable. The copy form is not, since the result is an object of its own. It
asks to read, so it is offered where the type can be copied from a `const` object, or where
the source hands the object over instead: a copy constructor taking a non-`const` reference
would otherwise modify what it copies from, through a trait that hands out a write. Asking a
reading source to bind a mutable reference does not compile - the source's rule adds `const`,
and the refusal is stated in the constraint rather than met at the conversion.

A source with no name at the call site is read by moving the object out, where its trait
declares `movable_tag` and the spelling asks for an object rather than a reference:

```cpp
scl::any_cast<std::string>(scl::any{text});          // moved out of the any
scl::any_cast<std::string>(scl::any_view{text});     // copied, the view owns nothing
scl::any_cast<std::string>(stored);                  // copied, a name means it is still held
```

Every other source is read by copying, a named one whatever its trait declares. Moving out is
therefore stated by the source rather than asked for at the call, needs no overload of its
own, and is the only way a type that moves and does not copy is read at all.

### What each source states

| Source | Request for the spelling `T` | Refuses |
|---|---|---|
| [`any_view`](any_view.md) | `T const` - reads | a mutable reference request |
| [`any_mutable_view`](any_mutable_view.md) | `T` - writes | a `const` object at binding |
| [`any_arg`](any_arg.md) | `T` - the caller spells the access | nothing |
| [`any`](any.md) | `T const` for a `const` owner, `T` otherwise | a `volatile` owner |
| `std::any` | `T const` for a `const` box, `T` otherwise, see [std_any](std_any.md) | a `volatile` box |

A `volatile` owner is refused at the call rather than met as an error inside the library:
the owner keeps the object as a subobject, so its volatility reaches the object, and the
owner has no `volatile` accessors to read it through. A `volatile` box is refused the same
way, `std::any_cast` taking no pointer to one. A handle is the other way round - its own
qualification governs the handle alone and takes no part in the request, so a `volatile`
handle over an unqualified object answers `any_cast<T>`.

### Joining the cast

A type joins by specialising `scl::any_cast_traits` in namespace `scl`. The specialisation
carries one name:

```cpp
struct boxed_int
{
    int value;
};

namespace scl
{
    template <>
    struct any_cast_traits<boxed_int>
    {
        template <typename Target, typename Source>
        static constexpr auto access(Source * box) noexcept
            -> decltype(std::addressof(box->value))
        {
            return std::addressof(box->value);
        }
    };
} // namespace scl
```

`access<Target, Source>` answers the address of the object, or `nullptr` where the request is
not answered. `Source` is never null there: `scl::any_cast` checks that once and calls nothing
otherwise, so a specialisation states no null case of its own. A specialisation forwards the
work to the source itself and holds no algorithm of its own.

The result type of `access` is the result type of the cast, and the access rule is stated by
it: a source that hands out no write answers `Target const *`, one that does answers
`Target *`, and one whose own constness reaches the object carries that constness into the
answer. What the answer points at is compared with the spelling up to cv-qualification, so a
specialisation answering a pointer to a base class of what its source holds leaves the
constraint unsatisfied, and the cast is not offered for that spelling.

A specialisation may declare one name more, the type `movable_tag`, which needs no
definition. Declaring it states that the source owns what it hands out, so such a source with
no name at the call site is read by moving the object out; a specialisation that leaves the
name out is read by copying.

```cpp
namespace scl
{
    template <>
    struct any_cast_traits<boxed_int>
    {
        struct movable_tag;

        template <typename Target, typename Source>
        static constexpr auto access(Source * box) noexcept
            -> decltype(std::addressof(box->value))
        {
            return std::addressof(box->value);
        }
    };
} // namespace scl
```

`scl::any_cast` is offered exactly where the call it forwards compiles and answers such a
pointer, so a source that cannot answer needs no condition written out for it. A `Source`
whose methods `access` cannot call, or a `Target` it cannot produce, leaves the constraint
unsatisfied, and the caller reads that there is no such cast. Whether `access` throws decides
the exception specification of the pointer form.

Open `namespace scl` to declare it. A qualified name at an enclosing scope
(`template <> struct scl::any_cast_traits<boxed_int>`) is well-formed since C++17 and both
compilers accept it, but writing the leading `::` (`struct ::scl::any_cast_traits<...>`) is
rejected by GCC.

`access` is what `scl::any_cast` calls, not an interface for a caller to use: called directly
it skips the null check. Specialising the trait for a type the library already serves replaces
the library's own answer for that type and is not supported.

A type that specialises nothing has no cast at all, and the caller is told so at the call
rather than meeting an error inside one. Cv-qualification of the source takes no part: a
`const` or `volatile` handle is castable exactly when its unqualified self is, because a
handle's own qualification governs the handle and not what it refers to.

Whether a cast is offered is a constraint, and a compiler remembers the answer per source,
so a specialisation has to be visible before the first question about its type. Including
the header that defines a source before asking is enough; asking first and including after
is ill-formed and a compiler need not say so. Including `<scl/utility/any.h>` names every
source the library serves at once.

## See also

- [any_view](any_view.md), [any_mutable_view](any_mutable_view.md), [any_arg](any_arg.md) -
  the handles the cast serves
- [any](any.md) - the owner, whose own constness reaches the stored object
- [std_any](std_any.md) - the `std::any` specialisation
- [any_view, Casting](any_view.md#casting) - `scl::bad_any_cast`, what the throwing forms
  throw, and its `std::bad_cast` base
- [Russian documentation](../../ru/any/any_cast.md)
