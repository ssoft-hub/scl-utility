# Any anchor

`scl::any_anchor` is an object a caller declares beside a value so that
[`scl::any_view`](any_view.md) and [`scl::any_arg`](any_arg.md) can read that value during
constant evaluation.

- Header: `#include <scl/utility/any/any_anchor.h>`

## Overview

A cast through a handle answers with a typed pointer. Recovering one from `void const *` is
not a constant expression before P2738, that is, before C++26, while a downcast along a class
hierarchy is one on C++20 already. The second way needs an object of a class the compiler
knows to exist beside the value, and someone to keep that object alive for as long as the
handle.

An anchor is that object: it holds the type description of the value and a typed pointer to
the value itself. The caller declares it, so the lifetime question is answered where the
value's own lifetime is answered.

A value an [`scl::any`](any.md) owns needs no anchor: the owner keeps the object inside a
holder of its own, and the cast comes back down to it.

At run time an anchor changes nothing. A handle built through one records the same shared
type description an ordinary binding gives it, so neither the size of the handle nor the cost
of the cast moves.

## Features

- Lets a plain object be read through a view and an argument during constant evaluation, on
  the C++20 baseline.
- Frees a handle from the parameter position and from the lifetime of one expression: an
  anchor lives as long as the variable declaring it.
- Deduces the value's type, qualifiers included, so `scl::any_anchor bound{value};` needs no
  template argument.
- Owns nothing and extends no lifetime.
- Trivial: copying, moving and destroying an anchor are the compiler's own definitions, and
  each of them is usable in a constant expression.
- Costs run time nothing: neither the width of a handle nor the cost of a cast.

## API reference

### Construction

```cpp
int probe = 42;

scl::any_anchor bound{probe};       // scl::any_anchor<int>
scl::any_view const view{bound};    // the view refers to probe, not to the anchor
```

The value's type is deduced with its qualifiers: an anchor over `int const` differs from one
over `int`, and the write access an argument grants follows that difference exactly as it
does for an ordinary binding.

A view and an argument are built from an anchor implicitly, so the anchor is passed wherever
the value itself would have been.

### Reading during constant evaluation

Inside a `constexpr` function:

```cpp
constexpr int read()
{
    int const probe = 42;
    scl::any_anchor const bound{probe};
    scl::any_view const view{bound};

    return *scl::any_cast<int>(&view);
}

static_assert(read() == 42);
```

In a variable with static storage duration, an array of views included:

```cpp
static constexpr int one = 1;
static constexpr int two = 2;

static constexpr scl::any_anchor anchored_one{one};
static constexpr scl::any_anchor anchored_two{two};

constexpr std::array<scl::any_view, 2> views{anchored_one, anchored_two};

static_assert(*scl::any_cast<int>(&views[1]) == 2);
```

An argument that is not a parameter reads through an anchor as well - which is precisely what
the anchor a parameter gets by default cannot do:

```cpp
constexpr int doubled()
{
    int probe = 21;
    scl::any_anchor const bound{probe};
    scl::any_argument const subject{bound};

    *scl::any_cast<int>(&subject) *= 2;

    return probe;
}

static_assert(doubled() == 42);
```

### Lifetime

An anchor neither owns the value nor extends its lifetime. A handle built through an anchor
refers to the anchor in turn, so **both the value and the anchor have to outlive every handle
bound through it**. The rule is the one an ordinary binding already carries; there are simply
two objects to watch instead of one.

### Limits

Without an anchor, a cast to a plain object stops during constant evaluation with a compiler
diagnostic rather than answering wrongly.

An anchor lifts no other limit: a `std::any` backing cannot be built during constant
evaluation at all, and there is no `constexpr` variable of type `scl::any`, since a constant
allocation has to be released inside the same evaluation.

### C++26

From C++26 on no anchor is needed: recovering the pointer from `void const *` is a constant
expression by itself, and a cast folds for any object. The type stays in the public interface
so that code written against the C++20 baseline keeps compiling unchanged.

## See also

- [any_view](any_view.md) - the view an anchor teaches to read during constant evaluation
- [any_mutable_view](any_mutable_view.md) - the view an anchor teaches to write during
  constant evaluation
- [any_arg](any_arg.md) - the argument; in a parameter position it gets an anchor by default
- [any](any.md) - the owner whose value needs no anchor
- [Russian documentation](../../ru/any/any_anchor.md)
