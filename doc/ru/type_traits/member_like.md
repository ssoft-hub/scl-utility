# Member like типажи

Это набор вспомогательных инструментов (типажей) для построения типов указателей 
на члены класса на основе cv/ref-квалификаторов типа объекта и дескриптора целевого члена.
Эти шаблоны помогают формировать типы указателей на методы и свойства класса
единообразным и выразительным способом.

- Заголовочный файл: `#include <scl/utility/type_traits/member_like.h>`

Содержание:
- [`member_function_like_t<Type, Signature>`](#member_function_like_ttype-signature)
- [`member_property_like_t<Type, Member>`](#member_property_like_ttype-member)
- [`member_like_t<Type, T>`](#member_like_ttype-t)

---

## `member_function_like_t<Type, Signature>`

Формирует тип указателя на нестатическую функцию-член для класса `Type` и сигнатуры `Signature`, копируя cv- и ref-квалификаторы из `Type` и спецификатор `noexcept` из `Signature`.

- Объявление: `template <typename Type, typename Signature> using member_function_like_t = ...;`

### Семантика

- Тип объекта:
  - Классом в указателе становится `std::remove_cvref_t<Type>`.
- Квалификаторы функции:
  - `const`/`volatile` и ref-квалификатор (`&` или `&&`) функции-члена копируются из `Type`.
  - Если `Type` не имеет ref-квалификатора (т.е. не является ссылкой), результирующая функция-член не будет ref-квалифицирована (без `&`/`&&`).
- Спецификатор noexcept:
  - Если `Signature` имеет вид `R(Args...) noexcept`, результирующий указатель на функцию-член будет `noexcept`; в противном случае — нет.
- Сигнатура:
  - Тип возвращаемого значения и типы параметров берутся непосредственно из `Signature`.

### Примеры

```cpp
#include <type_traits>
#include <scl/utility/type_traits/member_like.h>

struct X {};

using ::scl::member_function_like_t;

// Без ref-квалификатора и noexcept
static_assert(std::is_same_v<member_function_like_t<X, void()>, void (X::*)()>);
static_assert(std::is_same_v<member_function_like_t<X const, void()>, void (X::*)() const>);
static_assert(std::is_same_v<member_function_like_t<X volatile, void()>, void (X::*)() volatile>);
static_assert(std::is_same_v<member_function_like_t<X const volatile, void()>, void (X::*)() const volatile>);

// С lvalue-квалификатором (&), без noexcept
static_assert(std::is_same_v<member_function_like_t<X&, void()>, void (X::*)() &>);
static_assert(std::is_same_v<member_function_like_t<X const&, void()>, void (X::*)() const &>);
static_assert(std::is_same_v<member_function_like_t<X volatile&, void()>, void (X::*)() volatile &>);
static_assert(std::is_same_v<member_function_like_t<X const volatile&, void()>, void (X::*)() const volatile &>);

// С rvalue-квалификатором (&&), без noexcept
static_assert(std::is_same_v<member_function_like_t<X&&, void()>, void (X::*)() &&>);
static_assert(std::is_same_v<member_function_like_t<X const&&, void()>, void (X::*)() const &&>);
static_assert(std::is_same_v<member_function_like_t<X volatile&&, void()>, void (X::*)() volatile &&>);
static_assert(std::is_same_v<member_function_like_t<X const volatile&&, void()>, void (X::*)() const volatile &&>);

// С noexcept
static_assert(std::is_same_v<member_function_like_t<X, void() noexcept>, void (X::*)() noexcept>);
static_assert(std::is_same_v<member_function_like_t<X const, void() noexcept>, void (X::*)() const noexcept>);

// С сохранением аргументов
static_assert(std::is_same_v<member_function_like_t<X const&, int(short)>, int (X::*)(short) const &>);
```

### Типичные сценарии использования

- Утилиты метапрограммирования, которым необходимо формировать указатели на функции-члены на основе cv/ref-квалификаторов типа объекта (например, при создании типажей для вызываемых объектов, адаптеров или утилит, подобных рефлексии).
- Обеспечение ограничений на вызов на этапе компиляции путём кодирования требований к lvalue/rvalue и const/volatile объектам в типе указателя на функцию-член.

### См. также

- [`member_property_like_t`](#member_property_like_ttype-member) — Формирует типы указателей на члены-данные.
- [`member_like_t`](#member_like_ttype-t) — Обобщённый селектор для формирования указателей на функции-члены и члены-данные.

---

## `member_property_like_t<Type, Member>`

Формирует тип указателя на член-данное для класса `Type` и типа члена `Member`, накладывая cv-квалификаторы объекта на тип члена.

- Объявление: `template <typename Type, typename Member> using member_property_like_t = ...;`

### Семантика

- Тип объекта:
  - Классом в указателе становится `std::remove_cvref_t<Type>`.
- Квалификаторы члена:
  - Квалификаторы `const`/`volatile` из `Type` добавляются к `Member` (сохраняя любые квалификаторы, уже имеющиеся у `Member`).
  - Ref-квалификаторы у `Type` игнорируются (члены-данные не могут быть ref-квалифицированы).
- Результат:
  - Формирует тип `MemberLikeCv (std::remove_cvref_t<Type>::*)`, где `MemberLikeCv` — это `Member` с добавленными `const` и/или `volatile`, если они присутствуют в `Type`.

### Примеры

```cpp
#include <type_traits>
#include <scl/utility/type_traits/member_like.h>

struct X { int a = 0; int const b = 1; };

using ::scl::member_property_like_t;

// Квалификаторы объекта накладываются на Member
static_assert(std::is_same_v<member_property_like_t<X, int>, int X::*>);
static_assert(std::is_same_v<member_property_like_t<X const, int>, int const X::*>);
static_assert(std::is_same_v<member_property_like_t<X volatile, int>, int volatile X::*>);
static_assert(std::is_same_v<member_property_like_t<X const volatile, int>, int const volatile X::*>);

// Ссылки на Type не влияют на члены-данные
static_assert(std::is_same_v<member_property_like_t<X&, int>, int X::*>);
static_assert(std::is_same_v<member_property_like_t<X const&, int>, int const X::*>);

// Собственные cv-квалификаторы члена сохраняются (и объединяются с квалификаторами объекта)
static_assert(std::is_same_v<member_property_like_t<X, int const>, int const X::*>);
static_assert(std::is_same_v<member_property_like_t<X const, int volatile>, int const volatile X::*>);
```

### Типичные сценарии использования

- Создание типобезопасных указателей на члены-данные, которые отражают доступ к объекту с `const` или `volatile` квалификаторами, что помогает в обобщённом коде, где необходимо различать доступ к члену для чтения и для записи.
- Построение типажей или вспомогательных утилит рефлексии, работающих с типами членов-данных.

### См. также

- [`member_function_like_t`](#member_function_like_ttype-signature) — Формирует типы указателей на функции-члены.
- [`member_like_t`](#member_like_ttype-t) — Обобщённый селектор для формирования указателей на функции-члены и члены-данные.

---

## `member_like_t<Type, T>`

Обобщённый селектор, который создаёт тип указателя на член для класса `Type` и дескриптора `T`:
- Если `T` — это функциональный тип (`R(Args...)` или `R(Args...) noexcept`), ведёт себя как `member_function_like_t<Type, T>`.
- В противном случае, ведёт себя как `member_property_like_t<Type, T>`.

- Объявление: `template <typename Type, typename T> using member_like_t = ...;`

### Семантика

- Случай функции:
  - `member_like_t<Type, R(Args...) [noexcept]>` даёт
    `R (std::remove_cvref_t<Type>::*)(Args...) [cv из Type] [ref из Type] [noexcept из T]`.
- Случай данных:
  - `member_like_t<Type, Member>` даёт
    `MemberLikeCv (std::remove_cvref_t<Type>::*)`, где `MemberLikeCv` накладывает `const`/`volatile` из `Type` на `Member`.
- Определение типа:
  - Для выбора ветки функции `T` должен быть чистым функциональным типом (не указателем или ссылкой на функцию).
  - Если вам нужно принимать `R(*)(Args...)` или `R(&)(Args...)`, сначала удалите указатель/ссылку.

### Примеры

```cpp
#include <type_traits>
#include <scl/utility/type_traits/member_like.h>

struct X { int a = 0; int const b = 1; };

using ::scl::member_like_t;

// Функции-члены
static_assert(std::is_same_v<member_like_t<X, void()>, void (X::*)()>);
static_assert(std::is_same_v<member_like_t<X const&, int(short)>, int (X::*)(short) const &>);
static_assert(std::is_same_v<member_like_t<X const volatile, void(int) noexcept>, void (X::*)(int) const volatile noexcept>);

// Члены-данные
static_assert(std::is_same_v<member_like_t<X, int>, int X::*>);
static_assert(std::is_same_v<member_like_t<X const, int>, int const X::*>);
static_assert(std::is_same_v<member_like_t<X, int const>, int const X::*>);
```

### Типичные сценарии использования

- Единая точка входа для построения указателей как на функции-члены, так и на члены-данные в обобщённых библиотеках и мета-алгоритмах, что позволяет избежать частичных специализаций в месте вызова.
- Создание мостов между API, которые принимают дескрипторы как функций-членов, так и членов-данных, и нуждаются в едином типаже для формирования соответствующего типа указателя.

### См. также

- [`member_function_like_t`](#member_function_like_ttype-signature) — Конструктор указателей на функции-члены.
- [`member_property_like_t`](#member_property_like_ttype-member) — Конструктор указателей на члены-данные.

---

## Примечания

- Ограничения:
  - Для `member_function_like_t`: `Type` должен быть классовым типом; `Signature` должен быть функциональным типом (`R(Args...)` или `R(Args...) noexcept`).
  - Для `member_property_like_t`: `Type` должен быть классом или объединением (union); `Member` не должен быть функциональным типом.
- Как и для обычных указателей на члены, вызов функции или доступ к данным требует объектов с совместимыми cv/ref-категориями, что обеспечивается сформированным типом на этапе компиляции.
