# ScL Utility

[![GitLab Lint](https://img.shields.io/gitlab/pipeline-status/ssoft-scl%2Fscl-utility?branch=dev&label=GitLab%20Lint)](https://gitlab.com/ssoft-scl/scl-utility/-/pipelines)
[![GitHub Lint](https://img.shields.io/github/actions/workflow/status/ssoft-hub/scl-utility/lint.yml?branch=dev&label=GitHub%20Lint)](https://github.com/ssoft-hub/scl-utility/actions/workflows/lint.yml)
[![GitLab Pages](https://img.shields.io/badge/pages-gitlab-orange)](https://ssoft-scl.gitlab.io/scl-utility/)
[![GitHub Pages](https://img.shields.io/badge/pages-github-blue)](https://ssoft-hub.github.io/scl-utility/)

Common C++ utilities not available in the standard library.

ScL Utility is a header-only module of the ScL Toolkit. It provides erased views over any
value, portable attribute macros, type-classifying concepts, a bitmask over a scoped enum,
non-cryptographic hashes, parent-child tree hierarchies, compile-time and runtime name
extraction, preprocessor utilities and extended type traits. Licensed under
[The Unlicense](LICENSE.md).

The module is built and tested from the toolkit super-project
[scl-kit](https://gitlab.com/ssoft-scl/scl-kit); the pipelines above are its own lint and
documentation runs.

## Features

Every group has its own umbrella header; `#include <scl/utility.h>` pulls in all ten.

- **Any** — erased views over a value (`#include <scl/utility/any.h>`):
  - `any_view` — storable read-only view over a typed lvalue or a `std::any`; two pointers
    wide, no allocation, no RTTI required
  - `any_arg` — parameter-position companion that also binds temporaries and grants write
    access when the argument was an unqualified object
  - `any_switch` — a branch chain running the one branch whose type the erased value holds
  - `any_cast` — recovers the value by pointer (`nullptr` on mismatch), by copy, or by
    const reference
- **Attribute** — portable compiler attribute macros (`#include <scl/utility/attribute.h>`):
  - `SCL_NO_UNIQUE_ADDRESS` — portable `[[no_unique_address]]` (MSVC/GCC/Clang)
  - `SCL_ASSUME(expr)` — optimizer hint: expression is always true
  - `SCL_UNREACHABLE()` — mark unreachable code path
  - `SCL_FORCE_INLINE` / `SCL_NOINLINE` — inlining control
  - `SCL_NORETURN` — mark functions that never return
  - `SCL_DEPRECATED` / `SCL_DEPRECATED_MSG(msg)` — deprecation warnings
  - `SCL_FALLTHROUGH` — suppress implicit-fallthrough warning in `switch`
  - `SCL_NODISCARD` / `SCL_NODISCARD_MSG(msg)` — warn on discarded return value
  - `SCL_MAYBE_UNUSED` — suppress unused-entity warnings
  - `SCL_LIKELY` / `SCL_UNLIKELY` — C++20 statement branch hints
  - `SCL_LIKELY_EXPR(expr)` / `SCL_UNLIKELY_EXPR(expr)` — expression branch hints via `__builtin_expect`
  - `SCL_HOT` / `SCL_COLD` — function frequency hints
  - `SCL_LIFETIMEBOUND` — dangling-reference detection (`[[clang::lifetimebound]]`)
  - `SCL_UNSEQUENCED` / `SCL_REPRODUCIBLE` — purity annotations (C++26 semantics, GCC/Clang)
  - `SCL_INDETERMINATE` — C++26 opt-in to indeterminate storage
- **Concepts** — concept form of the standard type traits that have no `std` concept
  (`#include <scl/utility/concepts.h>`):
  - reference and qualifier classifiers (`reference`, `lvalue_reference`, `const_type`,
    `cv_type`, …)
  - type-category and type-property classifiers (`enum_type`, …)
- **Meta** — compile-time name extraction without RTTI (`#include <scl/utility/meta.h>`):
  - `type_name<T>` / `type_short_name<T>` — human-readable type names
  - `enum_name<V>` / `enum_short_name<V>` — enum member names
  - `symbol_name<S>` / `symbol_short_name<S>` — function and data member names
  - `type_key` / `type_key_of<T>` — RTTI-free identity key that tells TU-local types apart
- **Runtime** — the runtime counterparts of the meta utilities
  (`#include <scl/utility/runtime.h>`):
  - `type_name(obj)` / `type_short_name(obj)` — the dynamic type's name, via RTTI
  - `enum_value(v)` — any enum value as `"TypeName::N"`, named or not, without RTTI
- **Preprocessor** — macro utilities:
  - `SCL_FORWARD` — identity macro for token forwarding in macro chains
  - `SCL_COUNTER_VALUE` / `SCL_COUNTER_NEXT` — compile-time counter via ADL
- **Type traits** — extended helpers:
  - Detection idiom (`is_detected`, `detected_t`, `detected_or`)
  - `forward_like` — forward with value category of another type
  - `member_like` — member pointer trait decomposition
  - `overload_cast<Args...>` — overloaded function pointer disambiguation
- **Hierarchy** — parent-child tree structures (`#include <scl/utility/hierarchy.h>`):
  - `node<Payload, Allocator>` — tree node owning children in `std::list`
  - `tree<Payload, Observer, Allocator>` — root-level node list with observer notifications
  - `observer_tuple<Tree, Observers...>` — fan-out adapter for N observers via `std::apply`
  - Free algorithm functions: `has_parent`, `parent_of`, `is_parent_of`, `is_ancestor_of`,
    `are_sibling`, `are_identical` — adapt to any type via ADL (`adl_parent`,
    `adl_has_parent`, `adl_identity`)
- **Hash** — non-cryptographic compile-time hash utilities (`#include <scl/utility/hash.h>`):
  - `fnv1a`, `djb2`, `sdbm` — 64-bit hashes over any byte range; chainable
  - `jenkins_ota` — 32-bit Jenkins one-at-a-time hash
  - `siphash` — SipHash-2-4 64-bit keyed hash (hash-flooding resistant)
  - `key<Hasher>` — strongly-typed hash value; usable as `switch`/`case` label,
    STL unordered container key, and non-type template parameter (C++20 NTTP)
- **Flags** — type-safe bitmask over a scoped enum (`#include <scl/utility/flags.h>`):
  - `flags<Enum, bit_count>` — one bit per enumerator ordinal; `constexpr`-capable
  - bitwise `~ | & ^` and compound `|= &= ^=` (flags and single-flag forms),
    `operator[]` membership test
  - `all_of`/`any_of`/`none_of` predicates (variadic flag packs and whole-mask
    subset/intersection/disjoint forms), whole-mask `any`/`none`/`all`
  - a bidirectional range over the set flags (`begin`/`end`/`rbegin`/`rend`, `size`)

## Requirements

- C++20 compiler (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.23+, and only to build the tests, the examples, or an installed package —
  using the headers needs no build step

## Installation

The module is header-only: putting its `src` on the include path is enough.

```cmake
target_include_directories(your_target PRIVATE path/to/scl-utility/src)
```

To get the `scl::utility` target instead, take it from an installed toolkit:

```cmake
find_package(scl REQUIRED COMPONENTS utility)
target_link_libraries(your_target PRIVATE scl::utility)
```

The same target comes from the super-project built in tree, which composes the module
with its siblings:

```cmake
add_subdirectory(path/to/scl-kit)             # or FetchContent_MakeAvailable(scl-kit)
target_link_libraries(your_target PRIVATE scl::utility)
```

`module/utility` carries no `CMakeLists.txt` of its own — its CMake entry point is
`project/cmake/`, which the super-project adds per module.

Then include the umbrella header or a single component:

```cpp
#include <scl/utility.h>            // every group
#include <scl/utility/meta.h>       // one group
#include <scl/utility/meta/type.h>  // one header
```

## Quick start

Six programs, one per topic that reads in a few lines. Each is compiled as an example, so
what follows is the code that builds, not a paraphrase of it.

Names of types and enumerators at compile time, without RTTI
([`example/quick_start/meta`](example/quick_start/meta/meta.cpp)):

<!-- snippet: example/quick_start/meta/meta.cpp -->
```cpp
#include <scl/utility/meta/enum.h>
#include <scl/utility/meta/type.h>

#include <iostream>

enum class color
{
    red,
    green,
};

int main()
{
    static_assert(::scl::type_name<int>() == "int");
    static_assert(::scl::enum_short_name<color::green>() == "green");

    ::std::cout << ::scl::type_name<color>() << '\n';        // color
    ::std::cout << ::scl::enum_name<color::green>() << '\n'; // color::green
}
```

A string as a `switch` label and as a template parameter
([`example/quick_start/hash`](example/quick_start/hash/hash.cpp)):

<!-- snippet: example/quick_start/hash/hash.cpp -->
```cpp
#include <scl/utility/hash/key.h>

#include <iostream>
#include <string_view>

using ::scl::hash::key;
using namespace ::std::literals;

// A key hashes the range it is given, and a string literal carries its
// terminating zero: keep every key in the comparison built from a view.
int code_of(::std::string_view command)
{
    switch (key<>{command})
    {
    case key<>{"start"sv}:
        return 1;
    case key<>{"stop"sv}:
        return 2;
    default:
        return 0;
    }
}

template <key<> Command>
constexpr bool is_start = (Command == key<>{"start"sv});

static_assert(is_start<key<>{"start"sv}>);

int main()
{
    ::std::cout << code_of("start") << code_of("stop") << code_of("pause") << '\n'; // 120
}
```

A type-safe bitmask over a scoped enum
([`example/quick_start/flags`](example/quick_start/flags/flags.cpp)):

<!-- snippet: example/quick_start/flags/flags.cpp -->
```cpp
#include <scl/utility/flags.h>

#include <iostream>

enum class permission
{
    read,
    write,
    execute,
};

using permissions = ::scl::flags<permission>;

constexpr permissions read_write{permission::read, permission::write};

static_assert(read_write.all_of(permission::read, permission::write));
static_assert(read_write.none_of(permission::execute));

int main()
{
    permissions granted = read_write;
    granted |= permission::execute;

    ::std::cout
        << granted[permission::read]    // 1
        << granted[permission::execute] // 1
        << granted.size() << '\n';      // 3
}
```

One function accepting any argument type, and one chain choosing the branch
([`example/quick_start/any`](example/quick_start/any/any.cpp)):

<!-- snippet: example/quick_start/any/any.cpp -->
```cpp
#include <scl/utility/any.h>

#include <iostream>
#include <string>

using ::std::string;

string from_int(int number) { return "int " + ::std::to_string(number); }
string from_string(string const & text) { return "string " + text; }
string from_other(::scl::any_arg value) { return "other " + string{value.type_name()}; }

// The chain holds no subject: build it once, apply it to whatever turns up.
auto const describe =
    ::scl::any_switch<string>() //
        .in_case<int>(from_int)
        .in_case<string const &>(from_string)
        .or_else(from_other);

void print(::scl::any_arg value) { ::std::cout << *describe.apply(value) << '\n'; }

int main()
{
    print(string{"text"}); // string text
    print(42);             // int 42
    print(2.5);            // other double
}
```

A parent-child tree whose relationships stay queryable and whose changes reach an
observer
([`example/quick_start/hierarchy`](example/quick_start/hierarchy/hierarchy.cpp)):

<!-- snippet: example/quick_start/hierarchy/hierarchy.cpp -->
```cpp
#include <scl/utility/hierarchy.h>

#include <iostream>
#include <string>

using person = ::scl::hierarchy::node<::std::string>;

// Every insertion, erasure and payload change reaches the observer the tree carries.
template <typename Tree>
struct headcount
{
    int size = 0;

    void on_insert(Tree::iterator) { ++size; }
    void on_erase(Tree::const_iterator) { --size; }
    void on_clear() { size = 0; }
    void on_change(Tree::const_payload_reference, Tree::const_payload_reference) {}
};

using company = ::scl::hierarchy::tree<::std::string, headcount>;

int main()
{
    person lead{"lead"};
    person & senior = *lead.emplace_back("senior");
    senior.emplace_back("junior");

    person const & junior = senior.front();
    ::std::cout
        << ::std::boolalpha                                     //
        << ::scl::hierarchy::is_parent_of(senior, junior)       // true
        << ::scl::hierarchy::is_ancestor_of(lead, junior)       // true
        << ::scl::hierarchy::is_parent_of(lead, junior) << '\n' // false
        ;

    company staff;
    company::reference root = *staff.push_back("lead");
    root.push_back("senior");
    ::std::cout << staff.get_observer().size << '\n'; // 2
}
```

Picking an overload and carrying a value category over
([`example/quick_start/type_traits`](example/quick_start/type_traits/type_traits.cpp)):

<!-- snippet: example/quick_start/type_traits/type_traits.cpp -->
```cpp
#include <scl/utility/type_traits/forward_like.h>
#include <scl/utility/type_traits/overload_cast.h>

#include <iostream>
#include <type_traits>

struct widget
{
    void update(int) { ::std::cout << "update(int)\n"; }
    void update(double) { ::std::cout << "update(double)\n"; }
};

// The value category and cv-qualification of the owner, applied to a member type.
static_assert(::std::is_same_v<::scl::forward_like_t<widget const &, int>, int const &>);
static_assert(::std::is_same_v<::scl::forward_like_t<widget &&, int>, int &&>);

int main()
{
    auto const update_int = ::scl::overload_cast<int>(&widget::update);

    widget instance;
    (instance.*update_int)(1); // update(int)
}
```

## Examples

Each directory under `example/` builds into one runnable program, named `utility_` plus
the directory path (`example/hierarchy_node` becomes `utility_hierarchy_node`). The same
sources appear on the Examples page of the generated reference.

- [`example/any`](example/any/any_example.cpp) — `any_view` over a typed lvalue and over a
  `std::any`, `any_arg` in parameter position, and an `any_switch` chain.
- [`example/flags`](example/flags/flags_example.cpp) — combining and querying flags, set
  algebra between two masks, and iterating the set flags.
- [`example/hash`](example/hash/key_nttp.cpp) — `hash::key` as a non-type template
  parameter: string-tagged types, specialisation by string value, compile-time dispatch.
- [`example/hierarchy_node`](example/hierarchy_node/node_example.cpp) — an organization
  chart built from `hierarchy::node`, reorganized with `take()` and `transfer()`.
- [`example/hierarchy_tree`](example/hierarchy_tree/tree_example.cpp) — a task board on
  `hierarchy::tree` with two observers composed through `observer_tuple`.
- [`example/meta`](example/meta/type_key_example.cpp) — a hashable handle over `type_key`,
  used as an unordered-container key.
- [`example/runtime_enum`](example/runtime_enum/enum_value.cpp) — `enum_value` against its
  compile-time counterpart, including values outside the enumerator set.
- [`example/runtime_type`](example/runtime_type/type_name.cpp) — compile-time and runtime
  type names side by side under polymorphism.

The programs under `example/quick_start/` are the shortest form of each topic, and are the
source of the snippets in the Quick start above:

- [`example/quick_start/any`](example/quick_start/any/any.cpp)
- [`example/quick_start/flags`](example/quick_start/flags/flags.cpp)
- [`example/quick_start/hash`](example/quick_start/hash/hash.cpp)
- [`example/quick_start/hierarchy`](example/quick_start/hierarchy/hierarchy.cpp)
- [`example/quick_start/meta`](example/quick_start/meta/meta.cpp)
- [`example/quick_start/type_traits`](example/quick_start/type_traits/type_traits.cpp)

## Doxygen

- [GitLab Pages](https://ssoft-scl.gitlab.io/scl-utility/dev/)
- [GitHub Pages](https://ssoft-hub.github.io/scl-utility/dev/)

## Documentation

- [English](doc/md/en/Main.md)
- [Russian](doc/md/ru/Main.md)

## See also

- [CHANGELOG](CHANGELOG.md)
- [CONTRIBUTING](CONTRIBUTING.md)
