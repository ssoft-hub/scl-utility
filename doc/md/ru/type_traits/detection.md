# Обнаружение наличия выражения

Использование шаблонов идиомы обнаружения позволяет при компиляции программы проверить, корректно
ли выражение с аргументами заданных типов, определить тип его результата и сравнить этот тип
с ожидаемым.

Проверяемое выражение записывается в шаблонном типе, параметрами которого служат типы аргументов.
Такой шаблонный тип называется операцией. Операцией может быть и шаблон класса, об этом сказано
в подразделе "Корректная операция" раздела "Семантика". Например, операция `size_operation` ниже
описывает вызов метода `size()` для константной ссылки на контейнер. Операция передаётся каждому
шаблону страницы параметром `Operation`, а типы, которые подставляются в операцию, передаются за ней
как `Arguments...`.

Имена шаблонов взяты из Library Fundamentals TS v2.

- Заголовок: `#include <scl/utility/type_traits/detection.h>`

```cpp
namespace scl
{
    struct nonesuch;

    template <template <typename...> class Operation, typename... Arguments>
    using is_detected = unspecified;
    template <template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_v = is_detected<Operation, Arguments...>::value;

    template <template <typename...> class Operation, typename... Arguments>
    using detected_t = unspecified;

    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or = unspecified;
    template <typename Default, template <typename...> class Operation, typename... Arguments>
    using detected_or_t = detected_or<Default, Operation, Arguments...>::type;

    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    using is_detected_exact = std::is_same<Expected, detected_t<Operation, Arguments...>>;
    template <typename Expected, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_exact_v = is_detected_exact<Expected, Operation, Arguments...>::value;

    template <typename To, template <typename...> class Operation, typename... Arguments>
    using is_detected_convertible = std::is_convertible<detected_t<Operation, Arguments...>, To>;
    template <typename To, template <typename...> class Operation, typename... Arguments>
    inline constexpr bool is_detected_convertible_v =
        is_detected_convertible<To, Operation, Arguments...>::value;
}
```

## Семантика

### Корректная операция

- **Операция.** Операцией служит шаблонный тип или шаблон класса. Операция называется корректной
  для типов `Arguments...`, если подстановка этих типов в объявление операции удаётся.
  Для шаблонного типа объявлением является тип, которым он определён. Например, операция
  `size_operation<Container>` определена типом `decltype(std::declval<Container const &>().size())`
  и корректна для `std::vector<int>`, но не для `int`, потому что у типа `int` нет метода `size()`.
  Операция `value_type_operation<Container>` определена типом `typename Container::value_type`
  и корректна только для типа, в котором объявлен вложенный тип `value_type`.
- **Шаблон класса как операция.** Для шаблона класса проверяется только подстановка в его
  объявление, то есть в параметры шаблона и ограничение `requires`, а тело класса не проверяется.
  Поэтому ошибка в теле класса не делает операцию некорректной. Проверяемое выражение следует
  записывать в шаблонном типе.
- **Ошибка в теле функции.** Если операция вызывает функцию, тип результата которой выводится
  из тела, например шаблон функции с типом результата `auto` или `decltype(auto)` или обобщённое
  лямбда-выражение, то для определения типа результата тело функции инстанцируется. При ошибке
  в этом теле операция не становится некорректной, и сборка завершается сообщением об этой ошибке.
  Если тип результата записан в объявлении функции, тело для проверки не нужно и не инстанцируется.
  Чтобы операция была некорректной для неподходящих аргументов, условие следует записать
  в объявлении функции, то есть в типе результата или в ограничении `requires`.

### Шаблоны и маркерный тип `nonesuch`

- **`is_detected`.** Шаблонный тип `is_detected<Operation, Arguments...>` является типом
  `std::true_type`, если операция корректна для типов `Arguments...`, и типом `std::false_type`
  в остальных случаях. Шаблонная константа `is_detected_v` равна
  `is_detected<Operation, Arguments...>::value`.
- **`detected_t`.** Шаблонный тип `detected_t<Operation, Arguments...>` является типом
  `Operation<Arguments...>`, если операция корректна. Если операция записана
  как `decltype(выражение)`, это тип самого выражения. Например,
  для `size_operation<std::vector<int>>` это тип `std::size_t`. Если операция некорректна, шаблонный
  тип `detected_t` является маркерным типом `scl::nonesuch`.
- **`nonesuch`.** Маркерный тип `nonesuch` обозначает результат некорректной операции и ни для чего
  другого не предназначен. Объект этого типа нельзя создать, скопировать, переместить
  или уничтожить.
- **`detected_or`.** Шаблонный тип `detected_or<Default, Operation, Arguments...>` является классом
  с членами `type` и `value_t`. Член `type` является типом `Operation<Arguments...>`, если операция
  корректна, и типом `Default` в остальных случаях. Член `value_t` является типом
  `is_detected<Operation, Arguments...>`. Шаблонный тип `detected_or_t` является типом
  `detected_or<Default, Operation, Arguments...>::type`, то есть совпадает с шаблонным типом
  `detected_t`, в котором маркерный тип `nonesuch` заменён типом `Default`.
- **`is_detected_exact`.** Шаблонный тип `is_detected_exact<Expected, Operation, Arguments...>`
  является типом `std::true_type`, если тип `detected_t<Operation, Arguments...>` в точности
  совпадает с типом `Expected`, и типом `std::false_type` в остальных случаях. Для некорректной
  операции этот тип является маркерным типом `nonesuch`, поэтому шаблонная константа
  `is_detected_exact_v<nonesuch, Operation, Arguments...>` для некорректной операции равна `true`.
- **`is_detected_convertible`.** Шаблонный тип
  `is_detected_convertible<To, Operation, Arguments...>` является типом `std::true_type`, если тип
  `detected_t<Operation, Arguments...>` неявно преобразуется в тип `To`, и типом `std::false_type`
  в остальных случаях. Поэтому для некорректной операции он равен `std::true_type` для типа `To`,
  в который преобразуется маркерный тип `nonesuch`, например для типа `nonesuch const &`
  или для класса с конструктором `template <typename T> To(T &&)`, принимающим аргумент любого типа.


## Примеры

Операции, которые используются в примерах ниже:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp operations -->
```cpp
template <typename Container>
using size_operation = decltype(std::declval<Container const &>().size());

template <typename Container>
using value_type_operation = typename Container::value_type;
```

Проверка того, корректна ли операция:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp is_detected -->
```cpp
static_assert(scl::is_detected_v<size_operation, std::vector<int>>);
static_assert(!scl::is_detected_v<size_operation, int>);
static_assert(std::is_same_v<scl::is_detected<size_operation, int>, std::false_type>);
```

Тип результата корректной операции и маркерный тип `scl::nonesuch` для некорректной:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp detected_t -->
```cpp
static_assert(std::is_same_v<scl::detected_t<value_type_operation, std::vector<long>>, long>);
static_assert(std::is_same_v<scl::detected_t<value_type_operation, int>, scl::nonesuch>);
```

Тип `Default` вместо результата некорректной операции:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp detected_or -->
```cpp
static_assert(std::is_same_v<scl::detected_or_t<int, value_type_operation, std::string>, char>);
static_assert(std::is_same_v<scl::detected_or_t<int, value_type_operation, double>, int>);
static_assert(scl::detected_or<int, value_type_operation, std::string>::value_t::value);
static_assert(!scl::detected_or<int, value_type_operation, double>::value_t::value);
```

Сравнение типа результата с ожидаемым типом и проверка его преобразования в другой тип:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp exact -->
```cpp
static_assert(scl::is_detected_exact_v<std::size_t, size_operation, std::vector<int>>);
static_assert(!scl::is_detected_exact_v<int, size_operation, std::vector<int>>);
static_assert(scl::is_detected_convertible_v<double, size_operation, std::vector<int>>);
static_assert(!scl::is_detected_convertible_v<std::string, size_operation, std::vector<int>>);
```

Выбор ветви программы по результату проверки. Функция `count_of` возвращает размер контейнера,
если у аргумента есть метод `size()`, и `1` для любого другого аргумента:

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp dispatch -->
```cpp
template <typename Container>
std::size_t count_of(Container const & container)
{
    if constexpr (scl::is_detected_v<size_operation, Container>)
        return container.size();
    else
        return 1;
}
```

<!-- snippet: example/type_traits/detection/type_traits_detection_example.cpp run -->
```cpp
    std::cout << count_of(std::array<int, 3>{1, 2, 3}) << '\n'; // 3
    std::cout << count_of(42) << '\n';                          // 1
```

## Смотрите также

- [Обнаружение наличия функции](function_detection.md),
  [обнаружение наличия метода](method_detection.md)
  и [обнаружение наличия оператора](operator_detection.md) - операции для вызова функции и метода,
  объявляемые макросами, и готовые операции для каждого оператора
- [`example/type_traits/detection/type_traits_detection_example.cpp`](../../../../example/type_traits/detection/type_traits_detection_example.cpp)

---

Назад: [Type traits](index.md) | Далее: [Обнаружение наличия функции](function_detection.md) |
[К группе](index.md) | [К главной странице](../Main.md)
