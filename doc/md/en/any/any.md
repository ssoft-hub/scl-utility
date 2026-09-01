# Any

`scl::any` stores one object of any type and owns it: it creates the object, holds it and
destroys it together with itself. The type of the stored object is not part of the type of
`scl::any`, so one variable holds objects of different types at different times, and a
function accepts and returns such a value without knowing its type in advance.

- Header: `#include <scl/utility/any/any.h>`

## Overview

The standard `std::any` solves the same task, but relies on two things that are not
available in every build.

First, `std::any` identifies the stored type through `typeid`, which requires run-time type
information (RTTI). A project compiled with `-fno-rtti` cannot use it.

Second, none of the operations of `std::any` runs in a constant expression. A value whose
type is already known at compile time has to be served by other means.

`scl::any` removes both limits. It tells types apart through
[`scl::type_key`](../meta/type_key.md) - one object the library creates per type and settles
at compile time - rather than through `typeid`, so the header compiles with RTTI turned
off. Construction, `emplace`, `any_cast`, `reset` and destruction all run during constant
evaluation as well.

`sizeof(scl::any)` is two pointers. `std::any` costs the same on libstdc++, twice as much on
libc++ and four times as much on the MSVC standard library. With any of those standard
libraries, replacing `std::any` with `scl::any` therefore never grows the object, and with
libc++ and with the MSVC implementation it shrinks it.

A small object is stored inside `scl::any` itself, with no call to the allocator. An object
is stored that way when it fits the in-place buffer, needs no alignment stricter than a
pointer, and moves without throwing. Every other object goes into memory obtained from the
allocator. The choice between the two is made per type at compile time and costs nothing:
the in-place buffer occupies the same room as the pointer to an allocated object.

The stored type has to satisfy two requirements: the object can be destroyed without
throwing, and the object can be constructed from the arguments given. Nothing else is
required.

An object that cannot be moved is stored as well: it goes into allocated memory, and moving
the `scl::any` hands over the pointer while the object itself stays where it is. An object
that cannot be copied is stored too, because copying here is performed by neither a
constructor nor an assignment. That is the one visible difference from `std::any`: `scl::any`
itself cannot be copied, only moved, and a copy of the stored object is asked for by naming
it with `scl::any::try_copy`.

The allocator and the size of the in-place buffer are template parameters of
`scl::basic_any<Allocator, Capacity>`. The name `scl::any` stands for `basic_any<>`, that is
`std::allocator<std::byte>` and a buffer the size of a pointer.

## Features

- Stores an object of any type destructible without throwing and uses no RTTI.
- Costs two pointers - the size `std::any` has on libstdc++.
- Runs the whole lifecycle of the object during constant evaluation, on the C++20 baseline.
- Stores a small object in place, without calling the allocator.
- Takes the allocator as a template parameter, including a stateful one such as
  `std::pmr::polymorphic_allocator`.
- Moves but does not copy; `scl::any::try_copy` names the copy of the stored object of
  another container, or of the object a handle refers to, and `is_copyable()` reports in
  advance whether one is possible.
- Takes no array, in a constructor or in an assignment, apart from an array of `const`
  characters - the type of a string literal.
- Declares moving, `swap()` and `reset()` as `noexcept`.
- Hands out a pointer to the stored object through `any_cast<T>(any *)`; a type mismatch
  answers `nullptr` and throws nothing. Through a non-const `scl::any` that pointer grants
  write access.
- Hands out a value or a reference through `any_cast<T>(any &&)`; a type mismatch throws
  `scl::bad_any_cast`.
- Converts to the views [`any_view`](any_view.md) and
  [`any_mutable_view`](any_mutable_view.md) and to the argument [`any_arg`](any_arg.md), all
  of which refer to the stored object rather than to the container around it.
- May be the argument of the `apply` method of an [`any_switch`](any_switch.md).

## API reference

### Construction

```cpp
scl::any empty{};                                 // stores nothing
scl::any number{42};                              // stored inside the any itself
scl::any text{std::string{"Hello Any!"}};         // stored in allocated memory
scl::any built{std::in_place_type<std::string>, 3U, 'x'};  // "xxx", no temporary object
scl::any made = scl::make_any<std::string>(3U, 'x');       // the same as an expression
```

The constructor that takes a value applies `decay` to the type of the argument, as
`std::any` does. A string literal is therefore stored as `char const *`, and the name of a
function as a pointer to function.

An array is not taken. `decay` would turn it into a pointer to its first element, and a
pointer owns nothing: the array itself would stay outside the container, whose lifetime the
container does not extend. A one-dimensional array of `const` characters is the exception,
being the type of a string literal: a literal has static storage duration, so a pointer to
its first element stays valid for as long as the program runs. Every other array, an array of
arrays included, is refused at compile time - by the constructor, by assignment, by the
`std::in_place_type` form and by `emplace` alike - and through a handle it leaves the
container empty. To own the elements, pass the array as a `std::array`.

The type of a string literal is indistinguishable from the type of any other array of `const`
characters, so a `char const [3]` with automatic storage duration is taken as well and stored
as a pointer to its first element. That pointer outlives its array, and watching for it is
the caller's part.

The `std::in_place_type` form builds the object in the storage directly from the arguments
given and creates no temporary. It is also what stores a type that cannot be moved.

The allocator is passed as an argument, together with the `std::allocator_arg` tag:

<!-- snippet: example/any/owning/any_owning_example.cpp allocator -->
```cpp
static void show_allocator()
{
    ::std::pmr::monotonic_buffer_resource resource;
    using pmr_any = ::scl::basic_any<::std::pmr::polymorphic_allocator<::std::byte>>;

    pmr_any value{::std::allocator_arg, ::std::pmr::polymorphic_allocator<::std::byte>{&resource}};
    value.emplace<::std::string>(64U, 'x');

    ::std::cout
        << "  allocated from the given resource? " << (value.get_allocator().resource() == &resource) << '\n'; // 1

    // The allocator travels with the object: what one allocator acquired, only
    // that allocator may release.
    pmr_any const taken{::std::move(value)};
    ::std::cout << "  and travels with it? " << (taken.get_allocator().resource() == &resource) << '\n'; // 1
}
```

### Reading and writing

```cpp
scl::any value{42};

if (auto * number = scl::any_cast<int>(&value))   // int *: the write reaches the object
    *number = 7;

scl::any const frozen{42};
scl::any_cast<int>(&frozen);                      // int const *: reading only
scl::any_cast<double>(&value);                    // nullptr: the type does not match
scl::any_cast<int>(value);                        // 7, a copy of the value
scl::any_cast<double>(value);                     // throws scl::bad_any_cast
```

The pointer form answers `nullptr` when the requested type differs from the stored one, and
throws nothing. The reference form throws `scl::bad_any_cast` in that case; it is declared
only where [`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) is `1`.

Access rights follow the way the `scl::any` itself is reached. A const `scl::any` answers a
pointer to const, a non-const one grants write access. This is what separates the owning
type from [`any_view`](any_view.md), which never grants a write.

### Replacing and clearing

```cpp
scl::any value{42};

value = std::string{"Hello Any!"};      // destroys the int, stores the string
value.emplace<std::string>(3U, 'x');    // builds the string in place from arguments
value.reset();                          // destroys the object, leaves the any empty
```

The assignment operator of the stored type is never called, exactly as with `std::any`: the
old object is destroyed and a new one is built from scratch.

The two ways of replacing - assigning a value and `emplace` - differ in the order of these
steps, and that order decides what happens when a constructor throws.

Assigning a value obtains that value first and destroys the old object only afterwards. A
constructor that throws therefore leaves the old object untouched. `emplace` destroys the
old object first, so a constructor that throws leaves the `scl::any` empty. That is the
guarantee `std::any::emplace` gives.

The order also decides what an argument may be. `emplace` reads its arguments after the old
object is gone. An argument referring into that object - the object itself, a member of it,
or anything it owns - is therefore read after its destruction. Assigning a value reads such
an argument while the old object is still alive:

```cpp
scl::any value{owner{std::make_unique<payload>()}};

value = *scl::any_cast<owner>(&value)->held;   // the source lives inside the stored object
```

Assigning the container its own value does nothing at all. That holds both for assigning a
value and for assigning a copy asked for through an [`any_view`](any_view.md) referring to
the object that very container holds:

```cpp
scl::any value{std::make_unique<int>(42)};

value = scl::any::try_copy(scl::any_view{value});   // nothing happens
```

That is what keeps a value whose type cannot be copied: the copy is impossible, and a
replacement would leave the `scl::any` empty. A view or a value naming a **member** of the
stored object is an ordinary replacement instead.

Pointers into the stored object must not be used after a replacement or after `reset()`: the
object they addressed has been destroyed.

### Reusing the memory

A replacement keeps the memory already allocated on two conditions: the block still holds the
new object, and that object does not belong in the in-place buffer. The allocator is then
called neither to release nor to allocate: the new object is built in the same block, laid out
for its own size and alignment, so its address need not be the one the old object had.

```cpp
pmr_any value{std::allocator_arg, pmr_allocator{&resource}};
value = three_doubles{1.0, 2.0, 3.0};                        // one allocation

for (int round = 0; round < 16; ++round)
    value = three_doubles{double(round), 0.0, 0.0};           // no further allocation
```

This matters most where releasing memory gives nothing back.
`std::pmr::monotonic_buffer_resource` never hands out released memory again, so without this
the loop would consume its arena on every round.

Another type takes over that memory too, as long as the block holds it, so a narrower type
reuses a block taken for a wider one. A new type that belongs in the in-place buffer needs no
block: it goes to the buffer, and the block returns to the allocator. Matching by size alone
is not enough for the buffer, an object there being relocated by moving it, so a type whose
move constructor may throw is allocated even when it fits by size.

Assigning a value and `emplace` reach this optimisation differently, and here is why.
Building an object where the old one stands requires destroying the old one first. `emplace`
destroys first by definition, so it keeps the memory for every type. Assigning a value
cannot: the new value is sometimes part of the old object, and destroying the old one first
would read from what no longer exists. So a temporary copy of the new value is built first,
while the old object is still alive - throwing there is safe and leaves the any untouched -
and only then is the old object destroyed and the copy moved into its place. That move is
the step that must not throw: between the destruction and the move the memory holds no
object, and there is nothing to put back. Hence the condition for reuse: the type has a move
constructor that does not throw, and an object of it is no wider than 256 bytes. That second
condition bounds the temporary copy rather than any storage: the copy is declared as an
ordinary variable of the type, so the compiler aligns it, and 256 bytes is the width past
which such a copy costs more than a fresh call to the allocator. Anything else is given fresh
memory, and the old object is destroyed only once the replacement stands.

A value arriving through [`any_view`](any_view.md), [`any_mutable_view`](any_mutable_view.md)
or [`any_arg`](any_arg.md) is taken on the same terms. The type of such a value is not named
at the call, so the temporary copy is built by an operation from the type description the
handle carries.

The block can also be asked for ahead of time and given back on demand:

```cpp
scl::any value;

value.reserve_space_for<three_doubles>();      // one allocation, the any is still empty
value.has_space_for<three_doubles>();          // true
value.emplace<three_doubles>(1.0, 2.0, 3.0);   // no further allocation

value.emplace<narrower>();                     // the block still holds it, no allocation
value.shrink_to_fit();                         // gives back what the narrower object left
```

`reserve_space_for<T>()` and `shrink_to_fit()` are requests rather than guarantees. A type
that fits the in-place buffer needs no block, an object already in the buffer keeps none,
since its placement follows from its type, and an object whose move may throw cannot be
relocated; in each of those cases the call does nothing. `has_space_for<T>()` answers whether
the storage already held has the room for a `T`, and covers both cases, the buffer and the
block.

An object already held is moved into the block acquired, so that block takes the wider of the
two shapes rather than the reserved one alone. `emplace` then asks the allocator for nothing.
Assignment is narrower: a value reaches the block only on the terms assignment states, and a
value through a handle only where the block already holds an object, so either may still
allocate. All three calls are run-time ones:
a block is raw bytes, which hold no object during constant evaluation.

A block asked for and never filled is given back by `reset()`, by `shrink_to_fit()` and by
the destructor, and travels to the target on move and on swap.

### Copying

`scl::any` has neither a copy constructor nor a copy assignment. That is what lets it store
a non-copyable object - and what makes it move-only itself: `std::vector<scl::any>` can be
moved but not copied, and generic code demanding `std::copy_constructible` rejects the type.

A copy is requested explicitly:

<!-- snippet: example/any/owning/any_owning_example.cpp copying -->
```cpp
static void show_copying()
{
    ::scl::any const text{::std::string{"copy me"}};

    // is_copyable() asks about the stored type, so an empty result of the copy is
    // never ambiguous.
    ::scl::any const copy = ::scl::any::try_copy(text);

    // A move-only type is storable precisely because copying is not a constructor.
    ::scl::any const owned{::std::make_unique<int>(42)};
    ::scl::any const refused = ::scl::any::try_copy(owned);

    ::std::cout
        << "  is_copyable=" << text.is_copyable() << " copy=\""
        << *::scl::any_cast<::std::string>(&copy) << "\"\n";
    ::std::cout
        << "  unique_ptr is_copyable=" << owned.is_copyable()
        << " copy has_value=" << refused.has_value() << '\n'; // 0 0
}
```

`is_copyable()` answers about the stored **type**, so an empty result of a copy is
unambiguous. It can be empty in two cases only: there was no object to copy at all, or its
type has no copy constructor. An exception thrown by the copy constructor or by the allocator
is not caught and reaches the caller.

The allocator of the copy comes from the source's own allocator through
`select_on_container_copy_construction`. Where the copy is of what a handle refers to, a
handle carrying no allocator, the allocator is default-constructed. Naming one explicitly
takes the `std::allocator_arg` tag:

```cpp
scl::any const copy{std::allocator_arg, allocator, scl::any::try_copy(source)};
```

### Moving and swapping

<!-- snippet: example/any/owning/any_owning_example.cpp moving -->
```cpp
static void show_moving()
{
    // An allocated object moves by handing over its pointer, an in-place one by
    // relocation - which only a nothrow-movable type ever undergoes.
    ::scl::any source{::std::string{"moved"}};
    ::scl::any const target{::std::move(source)};

    // NOLINTNEXTLINE(bugprone-use-after-move): emptiness after a move is the contract
    ::std::cout << "  after the move, source has_value=" << source.has_value() << '\n'; // 0
    ::std::cout << "  target holds \"" << *::scl::any_cast<::std::string>(&target) << "\"\n";
}
```

The move constructor, move assignment and `swap()` are unconditionally `noexcept`. An
allocated object is handed over together with the pointer to it and is not moved itself. An
object stored inside the `scl::any` is relocated by moving it, but only types whose move
does not throw are stored there.

### Identity queries

```cpp
scl::any const value{42};

value.has_value();                              // true
value.type_name();                              // "int"
value.type_key() == scl::type_key_of<int>();    // true
scl::any{}.type_key() == scl::type_key{};       // true: an empty any stores no type
```

`type_key()` answers a [`scl::type_key`](../meta/type_key.md) by value, as every handle in
this group does. An empty any stores no type and answers an empty key. The equality
`type_key().name() == type_name()` always holds.

### Where the object is stored

The way an object is stored follows from its type and from the buffer capacity, and is
decided at compile time:

| Condition | Where the object is stored |
|---|---|
| `sizeof(T) <= buffer_capacity`, `alignof(T) <= alignof(void *)`, the move constructor does not throw | inside the `scl::any` itself |
| none of the above holds | in memory obtained from the allocator with the alignment of the type |
| during constant evaluation | always in allocated memory |

`buffer_capacity` is the in-place capacity the type actually has: at least `sizeof(void *)`,
so a smaller `Capacity` parameter is rounded up to it.

An over-aligned type never lands in the in-place buffer: it is stored in memory obtained with
the alignment it asks for, whatever that alignment is. The block carries the room to align
the object inside itself, so no alignment is refused.

The template deliberately has no alignment parameter, since one would cost every user size.
An alignment of `alignof(std::max_align_t)` would grow `scl::any` to four pointers, and a
cache-line alignment to sixteen.

A wider buffer is asked for through the second template parameter:

```cpp
using roomy_any = scl::basic_any<std::allocator<std::byte>, 48>;   // 56 bytes
```

Each pair of parameters is a type of its own, and two such types do not travel into one
another: there is no move, no `swap()` and no assignment from one specialization to the
other. Otherwise the object would have to be relocated between buffers of different widths
and between different allocators, which calls the move constructor of the stored type - and
moving an `scl::any` would stop being `noexcept`.

A value travels through a handle instead, as a copy:

```cpp
roomy_any source{std::string{"Hello Any!"}};

scl::any taken = scl::any::try_copy(scl::any_view{source});   // a copy of the string
```

The copy is made by the copy constructor of the stored type, so a value that cannot be
copied does not travel this way - the target stays empty. An `scl::any` goes inside another
one only when that is spelled out: `scl::any{std::in_place_type<roomy_any>, ...}`.

### The allocator travels with the object

Move assignment and `swap()` carry the allocator together with the object.
`propagate_on_container_move_assignment` and `propagate_on_container_swap` are not consulted:

```cpp
pmr_any source{std::allocator_arg, pmr_allocator{&first}};
source.emplace<std::string>(64U, 'x');
pmr_any target{std::allocator_arg, pmr_allocator{&second}};

target = std::move(source);
target.get_allocator().resource() == &first;   // true: the allocator came along
```

The reason is that memory obtained from one allocator can be released only by that same one.
The rule containers follow - move the object into the allocator of the target - does not
apply here: moving would demand a move constructor from every stored type, which is exactly
the requirement this design exists to avoid.

A copy is not governed by that rule. It builds a new object, so it asks for the
allocator of the copy the usual way, through
`select_on_container_copy_construction`.

### Constant evaluation

Everything except storing the object inside the `scl::any` itself is available during
constant evaluation:

<!-- snippet: example/any/owning/any_owning_example.cpp constant_evaluation -->
```cpp
static constexpr ::std::size_t stored_length()
{
    ::scl::any value{::std::string{"Hello Any!"}};
    ::std::size_t const length = ::scl::any_cast<::std::string>(&value)->size();

    value.reset();
    return length;
}
```

A container declared inside a `constexpr` function is available there in construction,
reading, replacement and clearing. The three block operations — `reserve_space_for`,
`shrink_to_fit` and `has_space_for` — are run-time ones. A `constexpr` function calling any
of them compiles and works when called at run time, and is never a constant expression: a
`static_assert` over it is what the compiler rejects. A `constexpr scl::any` variable at namespace scope can only be
empty, and here is why. Memory allocated during
constant evaluation has to be released before that evaluation ends, and the in-place buffer
is unavailable there: no object exists in raw bytes at compile time. C++26 does not lift
this restriction.

Views and arguments over an `scl::any` read during constant evaluation as the owner itself
does, and they need no anchor for it: the object sits inside a holder the owner creates
anyway, and the cast comes back down to it. The other direction - taking the value out of a
handle - runs there as well:

```cpp
constexpr int taken()
{
    scl::any const source{42};
    scl::any_view const view{source};
    scl::any const copy = scl::any::try_copy(view);

    return *scl::any_cast<int>(&copy);
}

static_assert(taken() == 42);
```

Taking a value during constant evaluation asks that both sides hold the same allocator, and
that the allocator itself work during constant evaluation. The copy is made through the
owner's own description of the object, which names the allocator it was created for; a
description made for another allocator names another one, and the two do not meet. Reading
through a handle works either way.

### Reading through the views

The views [`any_view`](any_view.md) and [`any_mutable_view`](any_mutable_view.md), the
argument [`any_arg`](any_arg.md) and the chain [`any_switch`](any_switch.md) accept an
`scl::any` and read the stored object rather than the container around it:

```cpp
scl::any value{42};

void report(scl::any_view view);   // view.type_name() answers "int", not "scl::any"
void bump(scl::any_arg arg);       // any_cast<int>(&arg) writes into the stored object
```

A view takes a `std::any` for an ordinary value and does not reach the object inside it.
`type_name()` of such a view answers `"std::any"`, and a cast answers that `std::any` itself.
What it holds is read through [`<scl/utility/any/std_any.h>`](std_any.md). This is where the
two containers differ: the object stored in an `scl::any` is what a view hands out, and the
object stored in a `std::any` is not.

Views refer to the object inside the container. Clearing, reassigning or moving the
`scl::any` therefore invalidates such a view, the same way a `std::string_view` is invalidated
once the string it refers to changes.

### A copy of the object a view refers to

The view [`any_view`](any_view.md), the view [`any_mutable_view`](any_mutable_view.md) and
the argument [`any_arg`](any_arg.md) do not own the object they refer to. A container takes a
copy of that object and owns it. The copy is named by the same `scl::any::try_copy` that
copies from another container, described under Copying above:

```cpp
std::string text{"Hello Any!"};
scl::any_view const view{text};

scl::any value = scl::any::try_copy(view);        // stores a std::string of its own
value = scl::any::try_copy(scl::any_arg{text});   // the same by assignment
```

The type of the object the handle refers to is not named at the call, so the copy is built by
an operation from the type description the handle carries. Whether that description holds such
an operation is not something the compiler can check, and the answer arrives while the program
runs. Hence the three cases that leave the container empty: the handle refers to nothing, the
type of the object has no copy constructor, or the object is an array. An array of `const`
characters, the type of a string literal, is the same exception it is for a value, and is
copied as a pointer to its first element:

```cpp
scl::any taken = scl::any::try_copy(scl::any_arg{"abc"});   // stores char const *
```

The result is therefore worth checking:

```cpp
scl::any copy = scl::any::try_copy(view);
if (!copy.has_value())
{
    // the object the handle refers to could not be copied
}
```

A handle carries no allocator, so a construction default-constructs the allocator of the copy,
and an assignment puts the copy in memory the receiving container's allocator gives.

`std::any` makes no such copy: its own constructor accepts a view by reference as an ordinary
value and stores the view itself. An `scl::any` can store the view too, by naming its type:

```cpp
scl::any stored{std::in_place_type<scl::any_view>, view};   // stores the view
```

### Identity across module boundaries

A cast compares [`scl::type_key`](../meta/type_key.md) values and obeys the same two limits
as a view does. A type declared inside a block is outside the contract of the key. The key
itself must not outlive the module that created it.

## See also

- [`example/any/owning/any_owning_example.cpp`](../../../../example/any/owning/any_owning_example.cpp) -
  a working example: storage, constant evaluation, copying on request, a stateful allocator
  and views over an `scl::any`.
- [any_view](any_view.md) - a read-only view that owns nothing
- [any_mutable_view](any_mutable_view.md) - a storable view that grants write access
- [any_arg](any_arg.md) - a view for a method parameter that also grants write access
- [any_switch](any_switch.md) - a chain of branches that selects one by the type of a value
- [type_key](../meta/type_key.md) - the identity key `type_key()` answers with
- [any_cast](any_cast.md) - the cast itself and the trait behind it
- [Russian documentation](../../ru/any/any.md)
