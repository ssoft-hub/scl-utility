# Any switch

`scl::any_switch` describes a set of branches, each answering for its own type, and runs the
one that matches the actual type of the value it is given.

- Header: `#include <scl/utility/any/any_switch.h>`

The header brings in [`scl::any_arg`](any_arg.md), [`scl::any_anchor`](any_anchor.md) and the
cast, so a plain object and an argument need nothing else. A subject spelled as
[`scl::any_view`](any_view.md), [`scl::any_mutable_view`](any_mutable_view.md) or
[`scl::any`](any.md) needs that type's own header, and reading the object inside a `std::any`
needs [`<scl/utility/any/std_any.h>`](std_any.md).

## Overview

Reading a value whose type is erased used to mean a run of [`scl::any_cast`](any_cast.md)
attempts: one call per candidate type, an `if` around each, and the "none of them matched"
case written out separately. The type appears twice in every branch - in the cast and in
the variable the cast fills - and nothing stops two branches from covering each other or
from never running at all.

`scl::any_switch` reduces that to one expression. A branch names its type once, the first
matching branch runs, and the fallback stands in the same chain. The chain reads whatever
[`scl::any_arg`](any_arg.md) accepts: a named or temporary object of a known type, an
[`scl::any_view`](any_view.md), an [`scl::any`](any.md), and a `std::any` as well.

The chain does not store the value. `in_case` and `or_else` only describe branches. `apply`
runs them over the value it is handed, and `has_case` answers whether there is anything to
run. One chain therefore serves any number of values, none of them has to outlive it, and the
chain itself behaves as an ordinary object: it can be put in a variable, a field or a
container, returned from a function, and applied wherever the value appears.

<!-- snippet: example/any/common/any_common_example.cpp switch -->
```cpp
static auto const describe_switch =
    ::scl::any_switch<::std::string>()
        .in_case<void>("nothing at all")
        .in_case<int>([](int number) { return "int " + ::std::to_string(number); })
        .in_case<::std::string const &>([](::std::string const & text) {
    return "string \"" + text + "\"";
}).or_else([](::scl::any_arg other) {
    return "something else (" + ::std::string{other.type_name()} + ")";
});
```

## Features

- Serves any number of values and is tied by lifetime to none of them.
- Selects a branch by the same qualifier coverage rule `any_cast` uses, `volatile` included.
- Accepts an `in_case<void>` branch for an empty value, and takes `std::any` as a case type
  like any other.
- Lets a branch be a callable - taking no argument or taking the value found - and, when the
  result type is named, a ready value as well.
- Accepts an optional `or_else` fallback, which receives the value as an `any_arg`.
- Answers `has_case` without running a branch: no side effect happens, and the method is
  `noexcept`.
- Rejects at compile time a branch fully covered by an earlier one, a second `or_else`, and
  any branch after one.
- Works during constant evaluation, on the C++20 baseline.

## API reference

### Starting a chain

```cpp
scl::any_switch<>();               // apply answers nothing
scl::any_switch<std::string>();    // apply answers std::optional<std::string>
```

The `Result` parameter defaults to `void`. Named explicitly, it has to be an object type:
`apply` then answers `std::optional<Result>`. The result is empty when no branch matched, and
equally empty when the matching branch produced no value.

### Branches

```cpp
auto matcher = scl::any_switch<>()
    .in_case<void>([]() { /* no value */ })
    .in_case<int>([](int number) { /* a copy of the value */ })
    .in_case<std::string const &>([](std::string const & text) { /* no copy */ })
    .in_case<double &>([](double & ratio) { ratio *= 2; })
    .in_case<char>();              // the branch matches and does nothing
```

A branch is named by a type. That is an object type, an lvalue reference to one, or `void`.
An rvalue reference is refused for the same reason a view refuses one: nothing here should
look movable.

Calling `in_case<Type>()` with no argument describes a branch that matches and does nothing.
That is how a type is taken away from the fallback without inventing an empty lambda for it.
The form makes sense only when `Result` is `void`, where "nothing" already is the finished
result; with a named result it does not compile.

After the type, `in_case` takes a callable - with no argument, or with one that is
constructible from the branch type. With a named result it also takes a ready value. In a
chain with `Result = void` there is nothing to convert such a value into, so it does not
compile there, and "match and do nothing" is written as `in_case<Type>()`:

```cpp
auto named = scl::any_switch<std::string>()
    .in_case<int>("integral")                             // a ready value
    .in_case<double>([]() { return "floating"; })         // a callable taking no argument
    .in_case<char>([](char symbol) { return std::string(1, symbol); });
```

The callable receives the value found when it takes an argument, and is called with no
arguments when it does not. The lambda `[](char symbol) { ... }` receives the value, the
lambda `[]() { ... }` does not. When both calls compile, the chain passes the value found.

A branch may produce either `Result` itself or a `std::optional<Result>`. That holds for both
forms, the callable and the ready value. An empty `optional` means the branch ran and
produced no value:

```cpp
auto parsed = scl::any_switch<std::string>()
    .in_case<int>([](int value) -> std::optional<std::string> {
        return value > 0 ? std::optional{std::to_string(value)} : std::nullopt;
    })
    .in_case<char>(std::optional<std::string>{});   // a ready value, an optional as well
```

### Nesting one chain inside another

The second form is what lets chains be built from each other. A chain is itself a callable
answering `std::optional<Result>`, so it serves as a branch of another chain, the fallback
included:

```cpp
auto const inner = scl::any_switch<std::string>().in_case<double>("floating");
auto const outer = scl::any_switch<std::string>().in_case<int>("integral").or_else(inner);

outer.apply(number);   // "integral": a branch of the outer chain ran
outer.apply(ratio);    // "floating":  control went into the nested chain
outer.apply(symbol);   // empty:       nothing matched in either
```

A value that matches no branch produces an empty result rather than a wrong one. Note that
`has_case` answers for the chain it is asked and does not look inside a nested one: a
fallback covers every value, whatever the chain behind it decides.

### How a branch is selected

The qualifier coverage rule comes from `any_cast` and is reproduced in full:

| Branch | When it matches |
|---|---|
| `in_case<T>` | an object of type `T` or `T const`; whether a copy is made is decided by the branch's own parameter |
| `in_case<T const &>` | the same objects, without a copy |
| `in_case<T &>` | an object without qualifiers only, with access to the caller's object |
| `in_case<T volatile &>` | an object without qualifiers or with `volatile`; that qualifier covers the way `const` does |
| `in_case<void>` | an empty value |

`std::any` is a case type like any other. An `in_case<std::any>` branch matches a subject
bound to a `std::any`, exactly as `any_cast<std::any>` answers the box, and no branch of any
chain reads what that box holds. A branch may stand before or after one naming `std::any`
and is judged by coverage like any other. A chain does not branch on the type inside a
`std::any` at all: the type to reach for where that is wanted is [`scl::any`](any.md), which a
chain does read through.

A branch fully covered by an earlier one does not compile. The chain therefore does not let a
branch be written that would never run. Coverage is counted by which objects a branch catches,
not by how it is spelled. `in_case<T>` and `in_case<T const &>` catch the same objects, so the
second after the first is refused. Both also catch everything `in_case<T &>` catches, so it is
refused after either of them.

The order decides. Written first, `in_case<T &>` keeps the object without qualifiers for
itself, and the `in_case<T const &>` that follows still takes objects carrying `const`:

```cpp
auto matcher = scl::any_switch<>()
    .in_case<int &>([](int & number) { number *= 2; })     // objects without qualifiers
    .in_case<int const &>([](int const &) { /* ... */ });  // const objects, still free
```

### The fallback

```cpp
auto matcher = scl::any_switch<std::string>()
    .in_case<int>("integral")
    .or_else([](scl::any_arg other) { return std::string{other.type_name()}; });
```

`or_else` is optional and accepts everything no branch matched, including an empty value when
the chain has no `in_case<void>`. It is a branch like the others: it answers a chain, not a
result. Since it accepts everything, nothing may follow it and it may not be repeated -
either one fails to compile.

### Applying

```cpp
std::optional<std::string> result = matcher.apply(value);
```

`apply` takes the first matching branch in the order written, runs that branch alone, and
answers `std::optional<Result>`. The result is empty when no branch matched. A chain with
`Result = void` answers nothing and, with no matching branch, simply does nothing.

`apply` may be called any number of times over any number of values: the chain describes
branches and stores no result, so a branch with a side effect runs on every call. No check is
needed beforehand, since finding no matching branch is an ordinary outcome the result reports.

`apply` is `noexcept` when every branch it might run is `noexcept` and moving the result does
not throw. The const form is available where those branches are callable on a const chain; a
chain holding mutable state inside a branch is applied through the non-const form.

`operator()` does exactly the same: the same selection, the same result, the same `const` and
`noexcept` forms:

```cpp
matcher(value);                                  // the same as matcher.apply(value)
std::ranges::transform(values, out, matcher);    // the chain is itself a callable
```

It removes the wrapping lambda where a callable is required. This is a call, not a
conversion: the chain still converts to nothing.

### Asking without running

```cpp
if (matcher.has_case(value))
    matcher.apply(value);          // the branch is expensive or has a side effect
```

`has_case` reports whether any branch matches the value. It goes as far as selecting the
branch and stops there: no branch runs, no side effect happens, and the method is `noexcept`.
When the chain has a fallback, the answer is always positive.

| State | `has_case(value)` | `apply(value)` with a named `Result` | `apply(value)` with `Result = void` |
|---|---|---|---|
| no branch matched | `false` | empty | nothing runs |
| a branch matched | `true` | its value; empty if it produced none | that branch runs |

An empty result does not tell the two rows apart, so the question "did any branch match" is
answered by `has_case`.

The chain converts implicitly to nothing - not to the result, not to `bool`: it describes
branches, and obtaining a result is written as an explicit call.

### What is deferred, and what is not

Building a chain runs nothing: no selection, no branch, no computation of a result. The one
thing that cannot be deferred is a branch given as a ready **value**. Written as
`or_else(compute())`, it evaluates `compute()` right there, since that is an ordinary function
argument. When building the value is expensive, pass a callable instead.

## Constant evaluation

A chain stays a literal type as long as its branches are literal. The value reaches `apply` as
a parameter, that is, where the cast of [`any_arg`](any_arg.md#constant-evaluation) already
folds at compile time on C++20. A chain declared `constexpr` and applied in a constant
expression therefore selects and runs a branch at compile time, free of the limits an
`any_arg` in a local variable carries:

```cpp
constexpr auto doubling = scl::any_switch<int>()
    .in_case<int>([](int number) { return number * 2; })
    .or_else(0);

void check()
{
    static_assert(doubling.apply(21).value() == 42);
    static_assert(doubling.has_case(21));
}
```

Both checks sit in a function body. The subject reaches `apply` as an
[`any_arg`](any_arg.md#constant-evaluation), and GCC 13.1 refuses a cast of one at namespace
scope for the reason that page gives; Clang and MSVC accept either placement.

## Exceptions

The chain catches nothing and wraps nothing. An exception thrown by a branch leaves `apply` in
the caller's frame and is caught by an ordinary `try` around that call. Only a branch that
captured something by reference can go stale, and it goes stale the way any captured reference
does.

## See also

- [`example/any/common/any_common_example.cpp`](../../../../example/any/common/any_common_example.cpp) -
  a working example: one chain instead of a run of casts, over a view and over an argument.
- [any_arg](any_arg.md) - the type every branch reads the value through
- [any_view](any_view.md) - the view the chain also accepts as a value
- [any_mutable_view](any_mutable_view.md) - the view granting write access, accepted as a value
- [any](any.md) - the owning type whose value the chain reads
- [std_any](std_any.md) - reading the object a `std::any` holds
- [any_cast](any_cast.md) - the cast itself and the trait behind it
- [Russian documentation](../../ru/any/any_switch.md)
