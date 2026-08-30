# Any mutable view

`scl::any_mutable_view` refers to an existing mutable object of any type. The type is not
part of the view's own type, so one function accepts a value of any type; the actual type is
reported by `type_name()`, `type_key()` and `scl::any_cast`. The view reads and writes the
object: it neither creates, copies nor destroys it, and holds its address together with the
description of its type.

- Header: `#include <scl/utility/any/any_mutable_view.h>`

## Overview

`scl::any_mutable_view` is [`scl::any_view`](any_view.md) granting write access. Both may be
stored: put in a field, in a container, returned from a function. Both cost two pointers and
are trivially copyable. The unqualified view only reads; the `mutable` one reads and writes.

The Any group tells its handles apart by two properties: whether a handle may be stored, and
whether it grants write access.

| handle | may be stored | write access | binds a temporary |
|---|---|---|---|
| [`scl::any_view`](any_view.md) | yes | no | no |
| [`scl::any_arg`](any_arg.md) | no | yes | yes |
| `scl::any_mutable_view` | yes | yes | no |

In ordinary reference terms these are `T const &`, a parameter, and `T &`.

`scl::any_mutable_view` is used where the write happens after the call that bound the view
to the object: the value arrives later - from command-line parsing, from configuration, from
the network - and is written through the stored view. [`scl::any_arg`](any_arg.md) does not
serve that, being valid only for the duration of a call.

The binding is narrower than [`scl::any_view`](any_view.md)'s. A `const` object does not
bind: it grants no write. A temporary does not bind whatever its constness: the view is
storable and would outlive it. A `volatile` object binds, and a cast to it obeys the
qualifier rule below.

Everything else matches [`scl::any_view`](any_view.md): the same bindings - a typed
object of any type, `std::any` included and named as the box it is - the same identity
queries, and the same limits on constant evaluation.

## Features

- Refers to an object without copying it, and grants write access.
- Is storable: it can be put in a field, in a container, returned from a function.
- Costs two pointers and is trivially copyable.
- Refuses a `const` object and a temporary of any constness.
- Refers to a typed object of any type and uses no RTTI.
- Refers to a `std::any` and writes the box itself, through the same constructor every other
  type takes.
- Refers to the content of an [`scl::any`](any.md) rather than to the any itself.
- Answers `type_name()`, `type_key()` and `has_value()` during constant evaluation as well,
  on the C++20 baseline.
- Hands out a pointer through `any_cast<T>(view *)`; a type mismatch answers `nullptr` and
  throws nothing.
- Hands out a value or a reference through `any_cast<T>(view &)`; a type mismatch throws
  `scl::bad_any_cast`.
- Grants a write through `any_cast<T>(view *)` or `any_cast<T &>`; over a `volatile` object
  the request has to repeat that qualifier.
- Converts to [`scl::any_view`](any_view.md) and to [`scl::any_arg`](any_arg.md); neither
  converts back.

## API reference

### Construction

```cpp
std::string text{"Hello Any!"};
std::string const frozen{"read only"};

scl::any_mutable_view empty{};      // refers to no object
scl::any_mutable_view raw{text};    // refers to the object directly
scl::any_mutable_view alias = raw;  // trivially copyable

scl::any_mutable_view refused{frozen};                 // does not compile: const object
scl::any_mutable_view also_refused{std::string{"x"}};  // does not compile: temporary
```

A `const` object is turned away by the type rather than by the cast, so the refusal is a
compile error at the point of binding, not a null pointer later. A caller who only reads
takes [`scl::any_view`](any_view.md), which binds both.

A non-`const` `std::any` converts to a view implicitly, as any other non-`const` lvalue does:

```cpp
std::any boxed{text};
scl::any_mutable_view over_any{boxed};   // can write the std::any itself
```

Such a view grants access to the box and to nothing else. `any_cast<std::any>` answers the
box, and a write through it replaces everything the box holds; `any_cast<T>` with the stored
type answers `nullptr`. To change the stored object in place, take the box and hand it to
`std::any_cast`:

```cpp
std::any_cast<std::string &>(*scl::any_cast<std::any>(&over_any)) += "!";
```

An [`scl::any`](any.md) is unwrapped to its content, exactly as it is for a reading view:

```cpp
scl::any owner{42};
scl::any_mutable_view over_owner{owner};   // refers to the int, not to the any

over_owner.type_name();   // "int"
```

### Observation

`has_value()`, `type_name()` and `type_key()` answer exactly what they answer for
[`scl::any_view`](any_view.md), including the way a `std::any` binding identifies the box
rather than the type stored inside it.

### Casting

The pointer form returns the requested type: `any_cast<T>` requests a write,
`any_cast<T const>` a read. On a type match the request is granted. The only qualifier that
can stand in the way is `volatile` on the referent, which the request has to repeat: `const`
never appears among the referent's qualifiers, because a `const` object does not bind.

```cpp
int number = 7;
scl::any_mutable_view const view{number};

int *       w = scl::any_cast<int>(&view);       // writes through to number
int const * r = scl::any_cast<int const>(&view); // reading asks for less
scl::any_cast<long>(&view);                      // nullptr: another type
```

The reference form spells the same choice with a reference type - `any_cast<T &>` writes,
`any_cast<T const &>` reads, `any_cast<T>` copies the value out - and throws
`scl::bad_any_cast` when the type does not match. It is declared only where
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `1`.

### The view's own constness

A `const` view forbids rebinding, not writing:

<!-- snippet: example/any/mutable_view/any_mutable_view_example.cpp const_view_writes -->
```cpp
static void bump(::scl::any_mutable_view const & view)
{
    if (auto * const number = ::scl::any_cast<int>(&view))
        ++*number;
    else
        ::std::cout << "  refused: the referent is not a writable int\n";
}
```

The rule holds across the group: a view's own qualifiers govern the view, and only the
qualifiers of the object are matched against the request. A `volatile` view therefore writes
an unqualified object and asks nothing of the request. A function that does not need the
write declares its parameter as `scl::any_view`.

### The const and volatile qualifiers of the object

A view remembers the qualifiers of the object it was bound to, and a cast has to request all
of them. `const` cannot appear among them, because a `const` object does not bind, so only
`volatile` has to be requested:

```cpp
int volatile sensor = 0;
scl::any_mutable_view const view{sensor};

scl::any_cast<int>(&view);           // nullptr: the request would drop volatile
scl::any_cast<int volatile>(&view);  // int volatile *, and it writes
```

### Conversions

The view converts to both reading handles of the group; there is no conversion back:

```cpp
int number = 7;
scl::any_mutable_view const view{number};

scl::any_view reading = view;   // narrowed to reading, same referent
takes_an_argument(view);        // scl::any_arg: keeps the write for the call
```

Narrowing to [`scl::any_view`](any_view.md) drops write access, and nothing recovers it from
the result. Converting to [`scl::any_arg`](any_arg.md) keeps it: an argument states the same
contract for the duration of a call. Neither converts back - a handle that promises no write
does not acquire one.

A [`scl::any_switch`](any_switch.md) chain accepts the view as a value, and a branch written
as `in_case<T &>` reaches the caller's object:

```cpp
int number = 41;
scl::any_mutable_view const view{number};

scl::any_switch<>().in_case<int &>([](int & reached) { ++reached; }).apply(view);
// number == 42
```

An [`scl::any`](any.md) built or assigned from the view stores a copy of the referent, not
the view itself.

### Storing a view

A view can be stored and used later:

<!-- snippet: example/any/mutable_view/any_mutable_view_example.cpp stored_write -->
```cpp
class binder
{
public:
    template <typename Type>
    void bind(::std::string name, Type & slot)
    {
        m_slots.insert_or_assign(::std::move(name), ::scl::any_mutable_view{slot});
    }

    template <typename Type>
    bool fill(::std::string const & name, Type value) const
    {
        auto const found = m_slots.find(name);
        if (found == m_slots.end())
            return false;

        auto * const target = ::scl::any_cast<Type>(&found->second);
        if (target == nullptr)
            return false;

        *target = ::std::move(value);
        return true;
    }

private:
    ::std::unordered_map<::std::string, ::scl::any_mutable_view> m_slots;
};
```

The view owns nothing and extends no lifetime, so every bound object has to outlive whatever
stores the view. A write through an invalid view corrupts storage the object no longer
occupies, where a read only returns garbage from it.

### Constant evaluation

The rules are those of [`scl::any_view`](any_view.md). Identity queries fold on the C++20
baseline. A cast folds where the library can obtain a typed pointer: over an object an
[`scl::any`](any.md) owns, which sits inside the holder that any keeps, or through an
[`scl::any_anchor`](any_anchor.md) declared beside a plain object:

```cpp
constexpr int incremented()
{
    int value = 41;
    scl::any_anchor const anchor{value};
    scl::any_mutable_view const view{anchor};

    ++*scl::any_cast<int>(&view);
    return value;
}

static_assert(incremented() == 42);
```

The anchor that [`scl::any_arg`](any_arg.md) creates on its own is a caller's temporary that
exists for the duration of the call, so a storable view cannot use it. From C++26 on no
anchor is needed: recovering a typed pointer from `void const *` is a constant expression
there, and a cast folds over any plain object. It never folds over a `std::any`, under any
standard: `std::any` itself has no constant-evaluable operations.

## See also

- [`example/any/mutable_view/any_mutable_view_example.cpp`](../../../../example/any/mutable_view/any_mutable_view_example.cpp) -
  a working example: a stored write, a branch chain that writes, and writing during constant
  evaluation.
- [any_view](any_view.md) - the same view without write access
- [any_arg](any_arg.md) - a view for a method parameter that also binds temporaries
- [any_anchor](any_anchor.md) - the anchor that folds a cast to a plain object on C++20
- [any_switch](any_switch.md) - a chain of branches that reads a value without a run of casts
- [any](any.md) - the owning type these views read and write
- [type_key](../meta/type_key.md) - the identity key `type_key()` answers with
- [std_any](std_any.md) - reading the object a `std::any` holds
- [any_cast](any_cast.md) - the cast itself and the trait behind it
- [Russian documentation](../../ru/any/any_mutable_view.md)
