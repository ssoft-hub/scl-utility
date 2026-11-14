# Концепты ссылочных типов

Применение концептов ссылочных типов позволяет определить, является ли проверяемый тип ссылкой,
и если является, то какой именно, lvalue-ссылкой или rvalue-ссылкой.

- Заголовок: `#include <scl/utility/concepts/reference.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept reference = ::scl::concepts::compound<T> && ::std::is_reference_v<T>;

    template <typename T>
    concept lvalue_reference = ::scl::concepts::reference<T> && ::std::is_lvalue_reference_v<T>;

    template <typename T>
    concept rvalue_reference = ::scl::concepts::reference<T> && ::std::is_rvalue_reference_v<T>;
}
```

Концепт `non_reference`, обратный концепту `reference`, объявлен в заголовочном файле
[`type_category.h`](type_category.md), который включается заголовочным файлом `reference.h`:

```cpp
namespace scl::concepts
{
    template <typename T>
    concept non_reference = !::std::is_reference_v<T>;
}
```

## Семантика

### `reference<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является ссылкой любого вида,
  lvalue-ссылкой или rvalue-ссылкой, независимо от типа, на который она ссылается.
- **Указатель.** Указатель не удовлетворяет концепту, поскольку не является ссылкой.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_reference_v<T>`.

### `lvalue_reference<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является lvalue-ссылкой, например
  `int &` или `int const &`.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_lvalue_reference_v<T>`.

### `rvalue_reference<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является rvalue-ссылкой, например
  `int &&` или `int const &&`.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_rvalue_reference_v<T>`.

### `non_reference<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если не является ссылкой, то есть является
  типом объекта, типом функции или типом `void`.
- **Свойство стандартной библиотеки.** В стандартной библиотеке нет отдельного свойства для такой
  проверки, поэтому концепт записан как отрицание свойства `std::is_reference_v<T>`.

### Пересылающая ссылка

- **Вывод типа.** Для параметра `Argument &&` шаблона функции тип `Argument` выводится из аргумента.
  Для аргумента lvalue выводится lvalue-ссылка, а для аргумента rvalue выводится тип без ссылки.
- **Результат.** Поэтому такой тип `Argument` удовлетворяет концепту `lvalue_reference`
  только при аргументе lvalue и не удовлетворяет концепту `rvalue_reference` ни при каком аргументе.

### Выбор перегрузки

- **Правило.** Каждый концепт ссылочных типов объявлен через концепт [`compound`](type_category.md)
  или через концепт `reference`, поэтому его ограничение строже ограничений этих концептов.
  Если проверяемый тип удовлетворяет двум концептам и ограничение одного из них строже, компилятор
  выбирает шаблон функции, ограниченный более строгим концептом.
- **Более слабые ограничения.** Для каждого концепта перечислены концепты с более слабым
  ограничением:

| Концепт | Концепты с более слабым ограничением |
|---|---|
| `reference` | `compound` |
| `lvalue_reference` | `reference`, `compound` |
| `rvalue_reference` | `reference`, `compound` |

- **Концепт `non_reference`.** Концепт `non_reference` не объявлен через другие концепты, поэтому
  его ограничение не строже ограничения ни одного другого концепта группы. При этом концепты
  `object`, `fundamental`, `function` и `void_type` со страницы
  [Концепты категорий типов](type_category.md) и концепты `const_qualified` и `volatile_qualified`
  со страницы [Концепты cv-квалификаторов](qualifier.md) объявлены через концепт `non_reference`,
  и их ограничения строже ограничения концепта `non_reference`.

## Примеры

Проверка ссылки любого вида:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp reference -->
```cpp
static_assert(scl::concepts::reference<int &>);
static_assert(scl::concepts::reference<int const &&>);
static_assert(!scl::concepts::reference<int>);
static_assert(!scl::concepts::reference<int *>);
```

Проверка lvalue-ссылки:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp lvalue_reference -->
```cpp
static_assert(scl::concepts::lvalue_reference<int &>);
static_assert(scl::concepts::lvalue_reference<int const &>);
static_assert(!scl::concepts::lvalue_reference<int &&>);
static_assert(!scl::concepts::lvalue_reference<int>);
```

Проверка rvalue-ссылки:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp rvalue_reference -->
```cpp
static_assert(scl::concepts::rvalue_reference<int &&>);
static_assert(scl::concepts::rvalue_reference<int const &&>);
static_assert(!scl::concepts::rvalue_reference<int &>);
static_assert(!scl::concepts::rvalue_reference<int>);
```

Проверка типа, который не является ссылкой:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp non_reference -->
```cpp
static_assert(scl::concepts::non_reference<int>);
static_assert(scl::concepts::non_reference<int const>);
static_assert(scl::concepts::non_reference<int *>);
static_assert(!scl::concepts::non_reference<int &>);
static_assert(!scl::concepts::non_reference<int &&>);
```

Проверка категории значения аргумента, переданного через пересылающую ссылку:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp forwarding -->
```cpp
template <typename Argument>
std::string_view category(Argument &&)
{
    if constexpr (scl::concepts::lvalue_reference<Argument>)
        return "lvalue";
    else
        return "rvalue";
}
```

Запрет ссылочного типа для поля класса. Параметр шаблона класса `box` ограничен концептом
`non_reference`, поскольку поле ссылочного типа может пережить объект, на который оно ссылается:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp box -->
```cpp
template <scl::concepts::non_reference Value>
struct box
{
    Value value;
};
```

Выбор перегрузки по виду ссылки. Для lvalue-ссылки компилятор выбирает перегрузку, ограниченную
концептом `lvalue_reference`, а для rvalue-ссылки выбирает перегрузку, ограниченную концептом
`reference`:

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp binding -->
```cpp
template <scl::concepts::reference Type>
constexpr std::string_view binding()
{
    return "reference";
}

template <scl::concepts::lvalue_reference Type>
constexpr std::string_view binding()
{
    return "lvalue reference";
}
```

<!-- snippet: example/concepts/reference/concepts_reference_example.cpp use -->
```cpp
    std::string text{"text"};
    std::cout << category(text) << '\n';                     // lvalue
    std::cout << category(std::string{"temporary"}) << '\n'; // rvalue

    box<std::string> const label{text};
    std::cout << label.value << '\n'; // text

    std::cout << binding<int &>() << '\n';  // lvalue reference
    std::cout << binding<int &&>() << '\n'; // reference
```

## Смотрите также

- [`example/concepts/reference/concepts_reference_example.cpp`](../../../../example/concepts/reference/concepts_reference_example.cpp)

---

Назад: [Концепты cv-квалификаторов](qualifier.md) |
Далее: [Концепты категорий типов](type_category.md) | [К группе](index.md) |
[К главной странице](../Main.md)
