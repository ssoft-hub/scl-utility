# ScL Utility

Common C++20 utilities not available in the standard library.

ScL Utility is a header-only module of the ScL Toolkit. It provides erased views over any
value, portable attribute macros, type-classifying concepts, a bitmask over a scoped enum,
non-cryptographic hashes, parent-child tree hierarchies, compile-time and runtime name
extraction, preprocessor utilities and extended type traits.

## Requirements

- C++20 compatible compiler (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.23+, and only to build the tests, the examples, or an installed package — using
  the headers needs no build step

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

Then include the umbrella header or individual component headers:

```cpp
#include <scl/utility.h>           // every group
#include <scl/utility/meta.h>      // meta only
```

## Components

### Any — non-owning read-only views over `std::any`

| Component | Header | Description |
|-----------|--------|-------------|
| [any_view](any/any_view.md) | `<scl/utility/any/any_view.h>` | Storable read-only view over a `std::any` or a typed lvalue; accepts lvalues only |
| [any_arg](any/any_arg.md) | `<scl/utility/any/any_arg.h>` | Parameter-only view that also binds rvalues; valid for the duration of the call |
| [any_switch](any/any_switch.md) | `<scl/utility/any/any_switch.h>` | Branch chain running one branch, chosen by the type an erased value holds; holds no subject |

### Attribute — portable compiler attribute macros

| Component | Header | Description |
|-----------|--------|-------------|
| [SCL_NO_UNIQUE_ADDRESS](attribute/no_unique_address.md) | `<scl/utility/attribute/no_unique_address.h>` | Portable `[[no_unique_address]]` for MSVC/GCC/Clang |
| [SCL_ASSUME / SCL_UNREACHABLE](attribute/assume.md) | `<scl/utility/attribute/assume.h>` | Optimizer hints: assumption and unreachable marker |
| [SCL_FORCE_INLINE / SCL_NOINLINE](attribute/inline.md) | `<scl/utility/attribute/inline.h>` | Inlining control |
| [SCL_NORETURN](attribute/noreturn.md) | `<scl/utility/attribute/noreturn.h>` | Mark a function that never returns |
| [SCL_DEPRECATED / SCL_DEPRECATED_MSG](attribute/deprecated.md) | `<scl/utility/attribute/deprecated.h>` | Deprecation annotations |
| [SCL_FALLTHROUGH](attribute/fallthrough.md) | `<scl/utility/attribute/fallthrough.h>` | Intentional switch fall-through marker |
| [SCL_NODISCARD / SCL_NODISCARD_MSG](attribute/nodiscard.md) | `<scl/utility/attribute/nodiscard.h>` | Warn on discarded return value |
| [SCL_MAYBE_UNUSED](attribute/maybe_unused.md) | `<scl/utility/attribute/maybe_unused.h>` | Suppress unused-entity warnings |
| [SCL_LIKELY / SCL_UNLIKELY / SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR](attribute/likely.md) | `<scl/utility/attribute/likely.h>` | Branch-prediction hints |
| [SCL_UNSEQUENCED / SCL_REPRODUCIBLE](attribute/unsequenced.md) | `<scl/utility/attribute/unsequenced.h>` | Pure-function annotations (C++26 / gnu::const / gnu::pure) |
| [SCL_INDETERMINATE](attribute/indeterminate.md) | `<scl/utility/attribute/indeterminate.h>` | Opt-in to indeterminate (uninitialised) storage (C++26) |
| [SCL_HOT / SCL_COLD](attribute/hotcold.md) | `<scl/utility/attribute/hotcold.h>` | Function-frequency hints for hot/cold code paths |
| [SCL_LIFETIMEBOUND](attribute/lifetimebound.md) | `<scl/utility/attribute/lifetimebound.h>` | Dangling-reference detection via lifetime-bound parameter annotation |

### Concepts — concept form of the standard type traits

| Component | Header | Description |
|-----------|--------|-------------|
| `reference`, `lvalue_reference`, `rvalue_reference`, `non_reference` | `<scl/utility/concepts/reference.h>` | Reference classifiers |
| `const_type`, `volatile_type`, `cv_type` | `<scl/utility/concepts/qualifier.h>` | Qualifier classifiers |
| `enum_type` and the other category classifiers | `<scl/utility/concepts/type_category.h>` | Type-category classifiers |
| Type-property classifiers | `<scl/utility/concepts/type_property.h>` | Property classifiers |

### Hierarchy — parent-child trees

| Component | Header | Description |
|-----------|--------|-------------|
| [node](hierarchy/node.md) | `<scl/utility/hierarchy/node.h>` | Tree node owning its children, with O(1) transfer between parents |
| [tree](hierarchy/tree.md) | `<scl/utility/hierarchy/tree.h>` | Root-level node list notifying an observer on every change |
| [observer_tuple](hierarchy/observer_tuple.md) | `<scl/utility/hierarchy/observer_tuple.h>` | Fan-out adapter composing several observers into one |
| [algorithm](hierarchy/algorithm.md) | `<scl/utility/hierarchy/algorithm.h>` | `is_parent_of`, `is_ancestor_of`, `are_sibling` and friends, adaptable through ADL |

### Meta — compile-time reflection

| Component | Header | Description |
|-----------|--------|-------------|
| [type_name](meta/type_name.md) | `<scl/utility/meta/type.h>` | Compile-time type name extraction |
| [enum_name](meta/enum_name.md) | `<scl/utility/meta/enum.h>` | Compile-time enum member name extraction |
| [symbol_name](meta/symbol_name.md) | `<scl/utility/meta/symbol.h>` | Compile-time function/member name extraction |
| [type_key](meta/type_key.md) | `<scl/utility/meta/type_key.h>` | RTTI-free type identity key distinguishing TU-local types |
| [is_tu_local](meta/type_key.md#is_tu_local) | `<scl/utility/meta/tu_local.h>` | Detects types declared in an anonymous namespace |

### Runtime — runtime type and enum utilities

| Component | Header | Description |
|-----------|--------|-------------|
| [type_name(obj)](runtime/type_name.md) | `<scl/utility/runtime/type.h>` | Runtime type name via RTTI; supports polymorphism |
| [enum_value(v)](runtime/enum_value.md) | `<scl/utility/runtime/enum.h>` | Enum value as `"TypeName::N"`; no RTTI required |

### Hash — non-cryptographic hash utilities

| Component | Header | Description |
|-----------|--------|-------------|
| [key](hash/key.md) | `<scl/utility/hash.h>` | Strongly-typed hash value; usable as switch label, map key, and NTTP |

### Flags — type-safe bitmask over a scoped enum

| Component | Header | Description |
|-----------|--------|-------------|
| [flags](flags/flags.md) | `<scl/utility/flags.h>` | Bitmask over a scoped enum; bitwise ops, set predicates, and a bidirectional range over the set flags |

### Preprocessor — macro utilities

| Component | Header | Description |
|-----------|--------|-------------|
| [SCL_FORWARD](preprocessor/forward.md) | `<scl/utility/preprocessor/forward.h>` | Identity macro for token forwarding |
| [SCL_COUNTER](preprocessor/counter.md) | `<scl/utility/preprocessor/counter.h>` | Compile-time counter via ADL |

### Type traits — extended traits and helpers

| Component | Header | Description |
|-----------|--------|-------------|
| [detection](type_traits/detection.md) | `<scl/utility/type_traits/detection.h>` | Detection idiom (is_detected, etc.) |
| [forward_like](type_traits/forward_like.md) | `<scl/utility/type_traits/forward_like.h>` | Forward with value category of another type |
| [member_like](type_traits/member_like.md) | `<scl/utility/type_traits/member_like.h>` | Member pointer trait decomposition |
| [overload_cast](type_traits/overload_cast.md) | `<scl/utility/type_traits/overload_cast.h>` | Overloaded function disambiguation |

## Quick start

Six programs, one per topic that reads in a few lines. Each is compiled as an example, so
what follows is the code that builds, not a paraphrase of it.

Names of types and enumerators at compile time, without RTTI
([`example/quick_start/meta`](../../../example/quick_start/meta/quick_start_meta_example.cpp)):

<!-- snippet: example/quick_start/meta/quick_start_meta_example.cpp -->
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

    ::std::cout << ::scl::type_name<color>() << '\n';        // color; MSVC: enum color
    ::std::cout << ::scl::enum_name<color::green>() << '\n'; // color::green
}
```

A string as a `switch` label and as a template parameter
([`example/quick_start/hash`](../../../example/quick_start/hash/quick_start_hash_example.cpp)):

<!-- snippet: example/quick_start/hash/quick_start_hash_example.cpp -->
```cpp
#include <scl/utility/hash/key.h>

#include <iostream>
#include <string_view>

using ::scl::hash::key;

int code_of(::std::string_view command)
{
    switch (key<>{command})
    {
    case key<>{"start"}:
        return 1;
    case key<>{"stop"}:
        return 2;
    default:
        return 0;
    }
}

template <key<> Command>
constexpr bool is_start = (Command == key<>{"start"});

static_assert(is_start<key<>{"start"}>);

int main()
{
    ::std::cout << code_of("start") << code_of("stop") << code_of("pause") << '\n'; // 120
}
```

A type-safe bitmask over a scoped enum
([`example/quick_start/flags`](../../../example/quick_start/flags/quick_start_flags_example.cpp)):

<!-- snippet: example/quick_start/flags/quick_start_flags_example.cpp -->
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
([`example/quick_start/any`](../../../example/quick_start/any/quick_start_any_example.cpp)):

<!-- snippet: example/quick_start/any/quick_start_any_example.cpp -->
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
([`example/quick_start/hierarchy`](../../../example/quick_start/hierarchy/quick_start_hierarchy_example.cpp)):

<!-- snippet: example/quick_start/hierarchy/quick_start_hierarchy_example.cpp -->
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
([`example/quick_start/type_traits`](../../../example/quick_start/type_traits/quick_start_type_traits_example.cpp)):

<!-- snippet: example/quick_start/type_traits/quick_start_type_traits_example.cpp -->
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

## See also

- [Russian documentation](../ru/Main.md)
