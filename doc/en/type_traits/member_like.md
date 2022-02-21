# Member-like type traits

Utilities for building pointer-to-member types based on an object type’s cv/ref qualifiers and a target descriptor. These traits help you form pointer-to-member-function and pointer-to-data-member types in a uniform, expressive way.

- Header: `#include <scl/utility/type_traits/member_like.h>`

Contents:
- [`member_function_like_t<Type, Signature>`](#member_function_like_ttype-signature)
- [`member_property_like_t<Type, Member>`](#member_property_like_ttype-member)
- [`member_like_t<Type, T>`](#member_like_ttype-t)

---

## `member_function_like_t<Type, Signature>`

Builds a pointer-to-non-static-member-function type for class `Type` and function `Signature`, mirroring the cv- and ref-qualifiers from `Type` and the `noexcept` from `Signature`.

- Declaration: `template <typename Type, typename Signature> using member_function_like_t = ...;`

### Semantics

- Object type:
  - The class in the pointer is `std::remove_cvref_t<Type>`.
- Function qualifiers:
  - The member function’s `const`/`volatile` and ref-qualifier (`&` or `&&`) are copied from `Type`.
  - If `Type` has no ref-qualifier (i.e., not a reference), the resulting member function is unqualified with respect to ref (no `&`/`&&`).
- Noexcept:
  - If `Signature` is of the form `R(Args...) noexcept`, the resulting member function pointer is `noexcept`; otherwise it is not.
- Signature:
  - Return type and parameter types are taken verbatim from `Signature`.

### Examples

```cpp
#include <type_traits>
#include <scl/utility/type_traits/member_like.h>

struct X {};

using ::scl::member_function_like_t;

// No ref-qualifier, no noexcept
static_assert(std::is_same_v<member_function_like_t<X, void()>, void (X::*)()>);
static_assert(std::is_same_v<member_function_like_t<X const, void()>, void (X::*)() const>);
static_assert(std::is_same_v<member_function_like_t<X volatile, void()>, void (X::*)() volatile>);
static_assert(std::is_same_v<member_function_like_t<X const volatile, void()>, void (X::*)() const volatile>);

// Lvalue-qualified (&), no noexcept
static_assert(std::is_same_v<member_function_like_t<X&, void()>, void (X::*)() &>);
static_assert(std::is_same_v<member_function_like_t<X const&, void()>, void (X::*)() const &>);
static_assert(std::is_same_v<member_function_like_t<X volatile&, void()>, void (X::*)() volatile &>);
static_assert(std::is_same_v<member_function_like_t<X const volatile&, void()>, void (X::*)() const volatile &>);

// Rvalue-qualified (&&), no noexcept
static_assert(std::is_same_v<member_function_like_t<X&&, void()>, void (X::*)() &&>);
static_assert(std::is_same_v<member_function_like_t<X const&&, void()>, void (X::*)() const &&>);
static_assert(std::is_same_v<member_function_like_t<X volatile&&, void()>, void (X::*)() volatile &&>);
static_assert(std::is_same_v<member_function_like_t<X const volatile&&, void()>, void (X::*)() const volatile &&>);

// With noexcept
static_assert(std::is_same_v<member_function_like_t<X, void() noexcept>, void (X::*)() noexcept>);
static_assert(std::is_same_v<member_function_like_t<X const, void() noexcept>, void (X::*)() const noexcept>);

// With arguments retained
static_assert(std::is_same_v<member_function_like_t<X const&, int(short)>, int (X::*)(short) const &>);
```

### Typical Use Cases

- Metaprogramming utilities that need to form member function pointers based on an object type’s cv/ref qualifiers (e.g., generating callable traits, adaptors, or reflection-like utilities).
- Enforcing invocation constraints at compile time by encoding lvalue/rvalue and const/volatile object requirements into the member function pointer type.

### See Also

- [`member_property_like_t`](#member_property_like_ttype-member) — Forms pointer-to-data-member types.
- [`member_like_t`](#member_like_ttype-t) — Generic selector between function and data member pointer formation.

---

## `member_property_like_t<Type, Member>`

Builds a pointer-to-data-member type for class `Type` and member type `Member`, overlaying the object’s cv-qualifiers onto the member type.

- Declaration: `template <typename Type, typename Member> using member_property_like_t = ...;`

### Semantics

- Object type:
  - The class in the pointer is `std::remove_cvref_t<Type>`.
- Member qualifiers:
  - The `const`/`volatile` qualifiers from `Type` are added to `Member` (preserving any qualifiers already on `Member`).
  - Reference qualifiers on `Type` are ignored (data members are not ref-qualified).
- Result:
  - Forms `MemberLikeCv (std::remove_cvref_t<Type>::*)`, where `MemberLikeCv` is `Member` with `const` and/or `volatile` added if present on `Type`.

### Examples

```cpp
#include <scl/utility/type_traits/member_like.h>

struct X { int a = 0; int const b = 1; };

using ::scl::member_property_like_t;

// Object qualifiers overlay onto Member
static_assert(std::is_same_v<member_property_like_t<X, int>, int X::*>);
static_assert(std::is_same_v<member_property_like_t<X const, int>, int const X::*>);
static_assert(std::is_same_v<member_property_like_t<X volatile, int>, int volatile X::*>);
static_assert(std::is_same_v<member_property_like_t<X const volatile, int>, int const volatile X::*>);

// References on Type do not matter for data members
static_assert(std::is_same_v<member_property_like_t<X&, int>, int X::*>);
static_assert(std::is_same_v<member_property_like_t<X const&, int>, int const X::*>);

// Member’s own cv is preserved (and overlaid with object’s cv)
static_assert(std::is_same_v<member_property_like_t<X, int const>, int const X::*>);
static_assert(std::is_same_v<member_property_like_t<X const, int volatile>, int const volatile X::*>);
```

### Typical Use Cases

- Generating type-correct pointers to data members that reflect const- or volatile-qualified object access, aiding generic code which must differentiate readable vs. writable member access.
- Building traits or reflection helpers that operate on data member types.

### See Also

- [`member_function_like_t`](#member_function_like_ttype-signature) — Forms pointer-to-member-function types.
- [`member_like_t`](#member_like_ttype-t) — Generic selector between function and data member pointer formation.

---

## `member_like_t<Type, T>`

Generic selector that produces a pointer-to-member type for class `Type` and a descriptor `T`:
- If `T` is a function type (`R(Args...)` or `R(Args...) noexcept`), behaves like `member_function_like_t<Type, T>`.
- Otherwise, behaves like `member_property_like_t<Type, T>`.

- Declaration: `template <typename Type, typename T> using member_like_t = ...;`

### Semantics

- Function case:
  - `member_like_t<Type, R(Args...) [noexcept]>` yields  
    `R (std::remove_cvref_t<Type>::*)(Args...) [cv from Type] [ref from Type] [noexcept from T]`.
- Data case:
  - `member_like_t<Type, Member>` yields  
    `MemberLikeCv (std::remove_cvref_t<Type>::*)`, where `MemberLikeCv` overlays `const`/`volatile` from `Type` onto `Member`.
- Detection:
  - `T` must be a pure function type (not a pointer or reference to function) to select the function branch.
  - If you need to accept `R(*)(Args...)` or `R(&)(Args...)`, strip pointer/reference first.

### Examples

```cpp
#include <type_traits>
#include <scl/utility/type_traits/member_like.h>

struct X { int a = 0; int const b = 1; };

using ::scl::member_like_t;

// Function members
static_assert(std::is_same_v<member_like_t<X, void()>, void (X::*)()>);
static_assert(std::is_same_v<member_like_t<X const&, int(short)>, int (X::*)(short) const &>);
static_assert(std::is_same_v<member_like_t<X const volatile, void(int) noexcept>, void (X::*)(int) const volatile noexcept>);

// Data members
static_assert(std::is_same_v<member_like_t<X, int>, int X::*>);
static_assert(std::is_same_v<member_like_t<X const, int>, int const X::*>);
static_assert(std::is_same_v<member_like_t<X, int const>, int const X::*>);
```

### Typical Use Cases

- Single entry point for building either member-function or data-member pointers in generic libraries and meta-algorithms, avoiding partial specializations at the call site.
- Bridging APIs that accept either function- or data-member descriptors and need a unified trait to form the corresponding pointer type.

### See Also

- [`member_function_like_t`](#member_function_like_ttype-signature) — Pointer-to-member-function builder.
- [`member_property_like_t`](#member_property_like_ttype-member) — Pointer-to-data-member builder.

---

## Notes

- Constraints:
  - For `member_function_like_t`, `Type` must be a class type; `Signature` must be a function type (`R(Args...)` or `R(Args...) noexcept`).
  - For `member_property_like_t`, `Type` must be a class or union type; `Member` must not be a function type.
- As usual for pointer-to-member types, invocation and access require objects of compatible cv/ref categories, enforced by the formed type at compile time.
