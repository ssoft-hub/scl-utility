# Формирование типа указателя на член по типу объекта

Шаблонные типы `member_function_like_t`, `member_object_like_t` и `member_like_t` являются типами
указателя на метод или на поле класса, построенными по типу объекта, для которого этот член
используется. Класс указателя и квалификаторы члена берутся из типа объекта. Например, для объекта
типа `gauge const &` указатель на метод имеет квалификаторы `const &`, а указатель на поле указывает
на поле с квалификатором `const`.

Такой тип нужен, например, когда перегрузки метода различаются только квалификаторами. Адрес
`&gauge::label` в этом случае не имеет единственного типа, а перегрузка для данного типа объекта
выбирается приведением адреса к построенному типу указателя.

- Заголовок: `#include <scl/utility/type_traits/member_like.h>`

```cpp
namespace scl
{
    template <typename Type, typename Signature>
    using member_function_like_t = unspecified;

    template <typename Type, typename Member>
    using member_object_like_t = unspecified;

    template <typename Type, typename Member>
    using member_like_t = unspecified;
}
```

## Семантика

### Общие правила

- **Класс.** Каждый из шаблонных типов страницы является типом указателя на член класса
  `std::remove_cvref_t<Type>`, то есть класса объекта без cv-квалификаторов и ссылки.

### Указатель на метод

- **`member_function_like_t<Type, Signature>`.** Шаблонный тип является типом указателя на метод.
  Тип результата, типы параметров и спецификатор `noexcept` метода берутся из типа функции
  `Signature`, который записывается как `Result(Parameters...)`
  или `Result(Parameters...) noexcept`. Cv-квалификаторы метода берутся из типа `Type`, а ссылочный
  квалификатор равен `&` для типа `X &`, `&&` для типа `X &&` и отсутствует для типа `X` без ссылки.
  Например, тип `member_function_like_t<gauge const &, std::string()>` является типом
  `std::string (gauge::*)() const &`.
- **Неподходящий тип `Signature`.** Для типа функции, список параметров которого заканчивается
  многоточием `...` в стиле C, например `void(int, ...)`, и для типа функции с cv- или ссылочными
  квалификаторами, например `void() const`, шаблонный тип `member_function_like_t` не определён.
  Для такого типа подстановка завершается неудачей, а не ошибкой сборки, поэтому это можно проверить
  шаблонами [обнаружения наличия выражения](detection.md). Если тип `Signature` вообще не является
  типом функции или тип `std::remove_cvref_t<Type>` не является классом, сборка завершается
  с ошибкой.

### Указатель на поле

- **`member_object_like_t<Type, Member>`.** Шаблонный тип является типом указателя на поле, тип
  которого равен типу `Member` с добавленными квалификаторами `const` и `volatile` типа `Type`.
  Собственные cv-квалификаторы типа `Member` сохраняются, а ссылка типа `Type` на результат
  не влияет. Например, тип `member_object_like_t<gauge volatile, int const>` является типом
  `int const volatile gauge::*`. Сборка завершается с ошибкой, если тип `Member` не является типом
  объекта или тип `std::remove_cvref_t<Type>` не является классом или объединением.

### Выбор между методом и полем

- **`member_like_t<Type, Member>`.** Если тип `Member` является типом функции, шаблонный тип
  `member_like_t` является типом `member_function_like_t<Type, Member>`, в остальных случаях он
  является типом `member_object_like_t<Type, Member>`. Сборка завершается с ошибкой, если тип
  `Member` является ссылкой, в том числе ссылкой на функцию.

## Примеры

Тип примеров:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp types -->
```cpp
struct gauge
{
    int level;
    std::string unit;

    std::string label() const & { return unit; }
    std::string label() && { return std::move(unit); }
    void set(int value) noexcept { level = value; }
};
```

Указатель на метод с cv-квалификаторами и ссылкой типа объекта:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp function -->
```cpp
static_assert(std::is_same_v<
    scl::member_function_like_t<gauge const &, std::string()>, std::string (gauge::*)() const &>);
static_assert(
    std::is_same_v<scl::member_function_like_t<gauge &&, std::string()>, std::string (gauge::*)() &&>);
static_assert(
    std::is_same_v<scl::member_function_like_t<gauge, void(int) noexcept>, void (gauge::*)(int) noexcept>);
```

Указатель на поле с cv-квалификаторами типа объекта:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp object -->
```cpp
static_assert(std::is_same_v<scl::member_object_like_t<gauge, int>, int gauge::*>);
static_assert(std::is_same_v<scl::member_object_like_t<gauge const &, int>, int const gauge::*>);
static_assert(
    std::is_same_v<scl::member_object_like_t<gauge volatile, int const>, int const volatile gauge::*>);
```

Вид указателя в шаблонном типе `member_like_t` зависит от типа члена:

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp member_like -->
```cpp
static_assert(std::is_same_v<scl::member_like_t<gauge, void(int) noexcept>, decltype(&gauge::set)>);
static_assert(std::is_same_v<scl::member_like_t<gauge, int>, decltype(&gauge::level)>);
```

Выбор перегрузки по типу объекта. Методы `label` различаются только квалификаторами, поэтому
переменная, объявленная со спецификатором `auto`, не может быть инициализирована адресом
`&gauge::label`. Адрес приводится к типу указателя, построенному по типу объекта, поэтому перегрузка
`const &` выбирается для константного lvalue, а перегрузка `&&` выбирается для неконстантного
rvalue. Для неконстантного lvalue строится тип `std::string (gauge::*)() &`, а для константного
rvalue тип `std::string (gauge::*)() const &&`. Перегрузок с такими квалификаторами нет, поэтому
приведение не компилируется, хотя для неконстантного lvalue метод `label` вызвать можно.

Тип `Object &&`, а не тип `Object`, передаётся шаблонному типу `member_like_t` первым аргументом.
Для lvalue-аргумента параметр `Object` выводится как lvalue-ссылка, а для rvalue-аргумента как тип
без ссылки. Тип `Object &&` в обоих случаях сохраняет категорию значения аргумента.

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp overload -->
```cpp
template <typename Object>
std::string label_of(Object && object)
{
    auto const label = static_cast<scl::member_like_t<Object &&, std::string()>>(&gauge::label);
    return (std::forward<Object>(object).*label)();
}
```

<!-- snippet: example/type_traits/member_like/type_traits_member_like_example.cpp use -->
```cpp
    gauge tank{7, "litre"};
    gauge const & view = tank;

    std::string const copied = label_of(view);
    std::string const moved = label_of(std::move(tank));
    std::cout << copied << ' ' << moved << '\n'; // litre litre
```

## Примечания

- Тип указателя на функцию не является типом функции. Поэтому для типа `Member`, равного, например,
  `void (*)(int)`, шаблонный тип `member_like_t` является типом указателя на поле, тип которого
  равен типу указателя на функцию.

## Смотрите также

- [Определение сигнатуры вызываемого типа](signature.md) - шаблонный тип `scl::signature::object_t`,
  который подходит первым аргументом шаблонного типа `member_like_t`
- [`example/type_traits/member_like/type_traits_member_like_example.cpp`](../../../../example/type_traits/member_like/type_traits_member_like_example.cpp)

---

Назад: [Приведение значения к категории и cv-квалификаторам заданного типа](forward_like.md) |
Далее: [Выбор перегрузки функции или метода по типам параметров](overload_cast.md) |
[К группе](index.md) | [К главной странице](../Main.md)
