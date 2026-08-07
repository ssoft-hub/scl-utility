# Any switch

A branch chain that runs one branch, chosen by the type an erased value holds.

- Header: `#include <scl/utility/any/any_switch.h>`

## Overview

Reading an erased value with [`scl::any_cast`](any_view.md#casting) alone is a
cascade of probes: one call per candidate type, each with its own `if`, and a
fallback spelled separately. The type is named twice on every branch — once in the
cast, once in the variable it fills — and nothing keeps the branches from
overlapping or from silently never matching.

`scl::any_switch` collapses that into one expression. Every branch names its type
once, the first match runs, and the fallback belongs to the same chain. It reads
every subject [`scl::any_arg`](any_arg.md) accepts: a typed lvalue or rvalue, an
[`scl::any_view`](any_view.md), or a `std::any` on an RTTI build.

The chain holds no subject. `in_case` and `or_else` describe the branches;
`apply` runs them over the subject it is given, and `has_case` asks whether there
is one to run. So one chain serves any number of subjects, none of them has to
outlive it, and a chain is an ordinary object: keep it in a variable, a member or
a container, return it from a function, apply it wherever the subject turns up.

```cpp
constexpr auto matcher = scl::any_switch<std::string>()
    .in_case<void>("has no value")
    .in_case<int>([](int number) { return std::to_string(number); })
    .in_case<std::string const &>([](std::string const & text) { return text; })
    .or_else("undefined");

std::optional<std::string> result = matcher.apply(value);   // no precondition
bool covered = matcher.has_case(value);                     // runs no branch
```

## Features

- One chain, applied to as many subjects as the caller has; no lifetime coupling
- Selection by the qualifier-coverage rule of `any_cast`, `volatile` included
- `in_case<void>` for the empty value; a `std::any` subject is unwrapped
- Branch: an invocable taking nothing or the matched value; a ready value for a named `Result`
- Optional `or_else` fallback, taking the subject as an `any_arg`
- `has_case` performs the selection only — nothing runs, no side effect, `noexcept`
- A case an earlier one covers, a second `or_else`, and an `in_case` after one are compile errors
- `constexpr` throughout, on the C++20 baseline

## API reference

### Starting a chain

```cpp
scl::any_switch<>();               // apply returns nothing
scl::any_switch<std::string>();    // apply returns std::optional<std::string>
```

`Result` is `void` by default. A named `Result` must be an object type, since
`apply` hands it back as a `std::optional<Result>` — empty where no branch matched,
and equally where the branch that did produced nothing.

### Cases

```cpp
auto matcher = scl::any_switch<>()
    .in_case<void>([]() { /* the subject holds nothing */ })
    .in_case<int>([](int number) { /* a copy */ })
    .in_case<std::string const &>([](std::string const & text) { /* no copy */ })
    .in_case<double &>([](double & ratio) { ratio *= 2; })
    .in_case<char>();              // matches and does nothing
```

A case names an object type, an lvalue reference to one, or `void`. An rvalue
reference is rejected — nothing here is movable-from.

`in_case<Type>()` with no argument is a branch that matches and does nothing. It
is how a type is claimed away from the fallback without writing an empty branch for
it, and it belongs to `Result = void`, where doing nothing is a complete branch; for
a named `Result` it is a compile error.

After its case, `in_case` takes an invocable taking no argument or one argument
constructible from the case type. A named `Result` also takes a ready value — a
`void` chain has nothing to convert one into, so there it is a compile error and
`in_case<Type>()` is what says "match and do nothing":

```cpp
auto named = scl::any_switch<std::string>()
    .in_case<int>("an int")                              // a value
    .in_case<double>([]() { return "a double"; })        // no argument
    .in_case<char>([](char symbol) { return std::string(1, symbol); });
```

An invocable is called with the matched value where it takes one, and called with
nothing where it takes none — `[](char symbol) { ... }` sees the value, `[]() { ... }`
does not. Where both calls compile, the matched value is the one passed.

What a branch produces may be a `Result` or the `std::optional<Result>` holding one —
either form of branch argument, an invocable and a ready value alike. An empty optional
means the branch ran and produced nothing:

```cpp
auto parsed = scl::any_switch<std::string>()
    .in_case<int>([](int value) -> std::optional<std::string> {
        return value > 0 ? std::optional{std::to_string(value)} : std::nullopt;
    })
    .in_case<char>(std::optional<std::string>{});   // a ready value, optional too
```

### Nesting one chain inside another

That second form is what makes chains compose. A chain is callable and answers
`std::optional<Result>`, so it qualifies as a branch of an outer chain — a case of
it, or its fallback:

```cpp
auto const inner = scl::any_switch<std::string>().in_case<double>("double");
auto const outer = scl::any_switch<std::string>().in_case<int>("int").or_else(inner);

outer.apply(number);   // "int"     - the outer case
outer.apply(ratio);    // "double"  - delegated to the inner chain
outer.apply(symbol);   // empty     - neither chain matched
```

A subject neither chain matches falls through as an empty result rather than a wrong
one. Note that `has_case` answers for the chain it is asked and does not look inside a
nested one: a fallback covers every subject, whatever the chain behind it then decides.

### Selection

A case selects by the qualifier-coverage rule of `any_cast`, mirrored in full:

| Case | Matches |
|---|---|
| `in_case<T>` | a `T` or a `T const` referent; the branch's own parameter decides whether that is a copy |
| `in_case<T const &>` | the same referents, bound without a copy |
| `in_case<T &>` | an unqualified referent only, reaching the caller's object |
| `in_case<T volatile &>` | an unqualified or a `volatile` referent; `volatile` covers the way `const` does |
| `in_case<void>` | an empty value |

`in_case<std::any>` does not match the box, exactly as `any_cast<std::any>` does
not: a `std::any` subject is unwrapped and the branches see the boxed type.

A case an earlier one already covers is a compile error rather than a branch that
never runs, and coverage follows what a case matches rather than how it is spelled.
`in_case<T>` and `in_case<T const &>` reach the same referents, so the second after
the first is an error; both reach everything `in_case<T &>` reaches, so it is an
error after either of them.

The order is what decides. Written first, `in_case<T &>` keeps the unqualified
referent for itself, and `in_case<T const &>` after it still takes the `const` ones:

```cpp
auto matcher = scl::any_switch<>()
    .in_case<int &>([](int & number) { number *= 2; })     // an unqualified referent
    .in_case<int const &>([](int const &) { /* ... */ });  // the const ones, still free
```

### Fallback

```cpp
auto matcher = scl::any_switch<std::string>()
    .in_case<int>("an int")
    .or_else([](scl::any_arg other) { return std::string{other.type_name()}; });
```

`or_else` is optional and catches everything no case caught — an empty value
included when the chain has no `in_case<void>`. It is an ordinary branch: it
returns the chain, not a result. Since it catches everything, nothing may follow
it and nothing may repeat it; both are compile errors.

### Applying

```cpp
std::optional<std::string> result = matcher.apply(value);
```

`apply` selects the first matching branch in the order the cases are written, runs
that one and no other, and returns `std::optional<Result>` — empty when nothing
matched. A `void` chain returns nothing at all, and an unmatched subject simply
runs nothing.

It may be called any number of times, over as many subjects as the caller has, and
does the whole thing again each time: the chain describes the branches and holds
no result, so a branch with a side effect fires once per call. It carries no
precondition — no branch matching is an ordinary outcome, reported by the result.

`apply` is `noexcept` when the branches it may run are and moving the result cannot
throw, and `const` wherever those branches are callable on a `const` chain; a chain
holding a `mutable` invocable applies through a non-`const` one.

`operator()` is the same thing by another spelling — same selection, same result,
same `const` and `noexcept` forms:

```cpp
matcher(value);                                  // identical to matcher.apply(value)
std::ranges::transform(values, out, matcher);    // a chain is a callable
```

It exists so a chain can be handed to anything that takes a callable without
wrapping it in a lambda that only calls `apply`. It is a call, not a conversion: a
chain still converts to nothing at all.

### Asking without running

```cpp
if (matcher.has_case(value))
    matcher.apply(value);          // the branch is expensive, or has a side effect
```

`has_case` reports whether some branch matches the subject. It performs the
selection and stops there — no branch is run, so no side effect fires and it is
`noexcept`. With `or_else` present it is always true.

| state | `has_case(subject)` | `apply(subject)`, named `Result` | `apply(subject)`, `Result = void` |
|---|---|---|---|
| no branch matches | `false` | empty | nothing runs |
| a branch matches | `true` | its value, empty where it produced none | that branch runs |

An empty result therefore does not tell the two rows apart — `has_case` is what
answers whether a branch matched at all.

There is no implicit conversion from the chain to its result, to `bool`, or to
anything else: the chain is a description, and reading it is a named call.

### What is deferred, and what is not

Building the chain runs nothing: no selection, no branch, no result. The one
thing no chain defers is a branch that is a ready *value* —
`or_else(compute())` evaluates `compute()` where it is written, since that is an
ordinary function argument. Pass an invocable where building the value is
expensive.

## Constant evaluation

The chain is a literal type wherever its branches are, and the subject reaches
`apply` as a parameter — the one position where an
[`any_arg`](any_arg.md#constant-evaluation) cast folds on the C++20 baseline. A
`constexpr` chain applied in a constant expression therefore selects and runs at
compile time, with none of the restrictions a stored `any_arg` carries:

```cpp
constexpr auto doubling = scl::any_switch<int>()
    .in_case<int>([](int number) { return number * 2; })
    .or_else(0);

static_assert(doubling.apply(21).value() == 42);
static_assert(doubling.has_case(21));
```

## Exceptions

The chain catches nothing and wraps nothing. A branch that throws throws out of
`apply`, in the caller's frame, caught by an ordinary `try` around that call. What
can still dangle is a branch capturing by reference, and it dangles the way any
captured reference does.

## See also

- [`example/any/any_example.cpp`](../../../../example/any/any_example.cpp) —
  runnable version: one chain replacing a cascade of casts, over both backings.
- [any_arg](any_arg.md) — the subject type every branch reads through
- [any_view](any_view.md) — the storable read-only view, also accepted as a subject
- [Russian documentation](../../ru/any/any_switch.md)
