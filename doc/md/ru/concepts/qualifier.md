# Концепты cv-квалификаторов

Применение концептов cv-квалификаторов позволяет определить, имеет ли проверяемый тип квалификатор
`const`, квалификатор `volatile` или оба одновременно. При этом не учитывается квалификатор типа,
на который указывает указатель или ссылается ссылка.

- Заголовок: `#include <scl/utility/concepts/qualifier.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept const_qualified = ::std::is_const_v<T>;

    template <typename T>
    concept volatile_qualified = ::std::is_volatile_v<T>;

    template <typename T>
    concept cv_qualified = ::scl::concepts::const_qualified<T> && ::scl::concepts::volatile_qualified<T>;
}
```

## Семантика

### `const_qualified<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если имеет квалификатор `const`, например
  `int const` или `int * const`.
- **Массив.** Массив константных элементов, например `int const[3]`, также считается константным
  типом.
- **Указатель.** Тип `int const *` не удовлетворяет концепту, поскольку квалификатор относится
  к типу, на который указывает указатель, а не к самому указателю.
- **Ссылка.** Тип `int const &` не удовлетворяет концепту, поскольку ссылка не может иметь
  собственных квалификаторов.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_const_v<T>`.
- **Выбор перегрузки.** Концепт не объявлен через другие концепты, поэтому его ограничение не строже
  ограничения ни одного другого концепта группы.

### `volatile_qualified<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если имеет квалификатор `volatile`.
- **Массив, указатель и ссылка.** Квалификатор учитывается по тем же правилам, что и квалификатор
  `const`, поэтому типы `int volatile` и `int volatile[3]` удовлетворяют концепту, а типы
  `int volatile *` и `int volatile &` не удовлетворяют ему.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_volatile_v<T>`.
- **Выбор перегрузки.** Концепт не объявлен через другие концепты, поэтому его ограничение не строже
  ограничения ни одного другого концепта группы.

### `cv_qualified<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если имеет оба квалификатора одновременно,
  например `int const volatile`. Тип, имеющий только один из квалификаторов, не удовлетворяет
  концепту.
- **Свойство стандартной библиотеки.** В стандартной библиотеке нет отдельного свойства для такой
  проверки.
- **Выбор перегрузки.** Концепт объявлен через концепты `const_qualified` и `volatile_qualified`,
  поэтому его ограничение строже ограничений этих концептов. Для типа с обоими квалификаторами
  компилятор выбирает шаблон функции, ограниченный концептом `cv_qualified`, а не шаблон,
  ограниченный концептом `const_qualified` или `volatile_qualified`. Если шаблон, ограниченный
  концептом `cv_qualified`, не объявлен, а такой тип подходит и для шаблона, ограниченного концептом
  `const_qualified`, и для шаблона, ограниченного концептом `volatile_qualified`, компилятор
  отклоняет вызов как неоднозначный.

## Примеры

Проверка квалификатора `const`:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp const_qualified -->
```cpp
static_assert(scl::concepts::const_qualified<int const>);
static_assert(scl::concepts::const_qualified<int const volatile>);
static_assert(scl::concepts::const_qualified<int * const>);
static_assert(scl::concepts::const_qualified<int const[3]>);
static_assert(!scl::concepts::const_qualified<int>);
static_assert(!scl::concepts::const_qualified<int const *>);
static_assert(!scl::concepts::const_qualified<int const &>);
```

Проверка квалификатора `volatile`:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp volatile_qualified -->
```cpp
static_assert(scl::concepts::volatile_qualified<int volatile>);
static_assert(scl::concepts::volatile_qualified<int const volatile>);
static_assert(scl::concepts::volatile_qualified<int volatile[3]>);
static_assert(!scl::concepts::volatile_qualified<int>);
static_assert(!scl::concepts::volatile_qualified<int volatile *>);
static_assert(!scl::concepts::volatile_qualified<int volatile &>);
```

Проверка обоих квалификаторов одновременно:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp cv_qualified -->
```cpp
static_assert(scl::concepts::cv_qualified<int const volatile>);
static_assert(!scl::concepts::cv_qualified<int const>);
static_assert(!scl::concepts::cv_qualified<int volatile>);
static_assert(!scl::concepts::cv_qualified<int const volatile &>);
```

Выбор перегрузки по квалификаторам объекта. Для объекта с обоими квалификаторами вызывается
перегрузка, ограниченная концептом `cv_qualified`:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp qualifiers -->
```cpp
template <scl::concepts::const_qualified Value>
std::string_view qualifiers(Value &)
{
    return "const";
}

template <scl::concepts::cv_qualified Value>
std::string_view qualifiers(Value &)
{
    return "const volatile";
}
```

Запрет вызова функции для константного объекта. Функция `reset` записывает значение в свой аргумент,
поэтому её параметр шаблона `Value` ограничен отрицанием концепта `const_qualified`.
Для константного объекта тип `Value` выводится как `int const`, и вызов не компилируется, поскольку
этот тип не удовлетворяет ограничению:

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp reset -->
```cpp
template <typename Value>
void reset(Value & value)
    requires(!scl::concepts::const_qualified<Value>)
{
    value = Value{};
}

template <typename Value>
concept resettable = requires(Value & value) { reset(value); };

static_assert(resettable<int>);
static_assert(!resettable<int const>);
```

<!-- snippet: example/concepts/qualifier/concepts_qualifier_example.cpp use -->
```cpp
    int counter = 5;
    reset(counter);
    std::cout << counter << '\n'; // 0

    int const fixed = 1;
    int const volatile shared = 2;
    std::cout << qualifiers(fixed) << '\n';  // const
    std::cout << qualifiers(shared) << '\n'; // const volatile
```

## Смотрите также

- [`example/concepts/qualifier/concepts_qualifier_example.cpp`](../../../../example/concepts/qualifier/concepts_qualifier_example.cpp)

---

Далее: [Концепты ссылочных типов](reference.md) | [К группе](index.md) |
[К главной странице](../Main.md)
