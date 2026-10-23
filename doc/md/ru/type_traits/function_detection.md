# Обнаружение наличия функции

Макрос `SCL_FUNCTION_DETECTION(foo)` объявляет шаблоны для проверки функции с именем `foo`:

- можно ли вызвать функцию `foo` с аргументами заданных типов, которые при вызове могут неявно
  преобразовываться в типы параметров (нестрогое соответствие)
- объявлена ли функция `foo`, параметры которой в точности совпадают с заданными типами (строгое
  соответствие)

Проверки выполняются при компиляции программы, и для них достаточно объявления функции, определение
не нужно. Имя проверяемой функции передаётся в макрос вместо имени `foo`, и из него составляются
имена объявленных шаблонов. Например, для макроса `SCL_FUNCTION_DETECTION(print)` объявляются
шаблонные константы `has_print_function_v`, `has_print_function_exact_v` и остальные шаблоны с тем
же корнем.

- Заголовок: `#include <scl/utility/type_traits/detection/function.h>`

Макрос следует раскрывать в пространстве имён после объявлений проверяемой функции. Раскрытие
`SCL_FUNCTION_DETECTION(foo)` содержит объявления ниже.

```cpp
#define SCL_FUNCTION_DETECTION(name)

template <typename... Arguments>
using foo_function_operation = decltype(foo(std::declval<Arguments>()...));
template <typename... Arguments>
inline constexpr auto has_foo_function_v = scl::is_detected_v<foo_function_operation, Arguments...>;
template <typename... Arguments>
using foo_function_t = scl::detected_t<foo_function_operation, Arguments...>;

template <typename... Arguments>
using foo_function_exact_operation = decltype(
    static_cast<foo_function_operation<Arguments...> (*)(Arguments...)>(&foo)(std::declval<Arguments>()...));
template <typename... Arguments>
inline constexpr auto has_foo_function_exact_v = scl::is_detected_v<foo_function_exact_operation, Arguments...>;
template <typename... Arguments>
using foo_function_exact_t = scl::detected_t<foo_function_exact_operation, Arguments...>;
```

## Семантика

### Нестрогое соответствие

- **Условие.** Шаблонная константа `has_foo_function_v<Arguments...>` равна `true`, если вызов
  `foo(std::declval<Arguments>()...)` компилируется, и `false` в остальных случаях. Аргументы
  при этом могут неявно преобразовываться в типы параметров. Например, для функции
  `int foo(int, int)` шаблонная константа `has_foo_function_v<int, long>` равна `true`,
  потому что аргумент типа `long` преобразуется в параметр типа `int`. Шаблонный тип
  `foo_function_t<Arguments...>` является типом результата этого вызова или маркерным типом
  [`scl::nonesuch`](detection.md), если вызов не компилируется.
- **Поиск функции.** Функция для вызова ищется среди объявлений, видимых в месте раскрытия макроса,
  а также в пространствах имён и классах аргументов (ADL). Функция, найденная поиском по аргументам,
  может быть объявлена и после раскрытия макроса, потому что такой поиск выполняется тогда,
  когда шаблонная константа используется с конкретными типами аргументов.
- **Вызываемый объект.** Шаблонная константа `has_foo_function_v` равна `true` и тогда, когда `foo`
  является переменной, например указателем на функцию или функциональным объектом, и вызов этой
  переменной с аргументами заданных типов компилируется.

### Строгое соответствие

- **Условие.** Шаблонная константа `has_foo_function_exact_v<Arguments...>` равна `true`,
  если до раскрытия макроса объявлена функция `foo`, параметры которой имеют в точности типы
  `Arguments...`, и вызов `foo(std::declval<Arguments>()...)` компилируется. Квалификаторы `const`
  и `volatile` верхнего уровня у типов `Arguments...` не учитываются, подробнее об этом сказано
  в разделе "Примечания". Например, для перегрузок `int foo(int, int)`
  и `double foo(double, double)` шаблонная константа `has_foo_function_exact_v<int, int>` равна
  `true`, а шаблонная константа `has_foo_function_exact_v<int, long>` равна `false`, хотя вызов
  с такими аргументами компилируется.
- **Поиск функции.** Функция ищется только среди объявлений, видимых в месте раскрытия макроса,
  без поиска по аргументам (ADL). Поэтому для функции, объявленной позже, шаблонная константа
  `has_foo_function_exact_v` равна `false`. Для указателя на функцию и для функционального объекта
  с именем `foo` она тоже равна `false`, потому что такая переменная не является функцией.
- **Тип результата.** Тип результата функции в строгом соответствии не сравнивается. Шаблонный тип
  `foo_function_exact_t<Arguments...>` является типом результата обычного вызова, и его можно
  сравнить с ожидаемым типом, например с помощью `std::is_same_v`. Функция находится и тогда,
  когда её тип результата выводится из тела (`auto`), и тогда, когда она является специализацией
  шаблона функции с параметрами в точности типов `Arguments...`.
- **Неоднозначный вызов.** Строгое соответствие требует, чтобы обычный вызов
  `foo(std::declval<Arguments>()...)` компилировался. Например, для перегрузок `long f(int)`
  и `char f(int &&)` вызов `f(std::declval<int>())` неоднозначен, поэтому шаблонные константы
  `has_f_function_v<int>` и `has_f_function_exact_v<int>` равны `false`, хотя функция `f(int)`
  объявлена.
- **Спецификатор `noexcept`.** Спецификатор `noexcept` функции в строгом соответствии
  не сравнивается. Его можно проверить оператором `noexcept`, применённым к вызову, например
  `noexcept(foo(1, 2))`.

### Ограничения

- **Место раскрытия.** Сборка завершается с ошибкой, если в месте раскрытия макроса не видно
  ни одного объявления `foo` или если макрос раскрыт внутри класса.

## Примеры

Перегрузки функции и шаблоны, объявленные для них:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp declare -->
```cpp
int foo(int value, int factor);
double foo(double value, double factor);

SCL_FUNCTION_DETECTION(foo)
```

Нестрогое соответствие, при котором аргументы преобразуются в типы параметров:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp call -->
```cpp
static_assert(has_foo_function_v<int, int>);
static_assert(has_foo_function_v<int, long>);
static_assert(!has_foo_function_v<int>);
static_assert(std::is_same_v<foo_function_t<double, double>, double>);
```

Строгое соответствие типов параметров:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp exact -->
```cpp
static_assert(has_foo_function_exact_v<int, int>);
static_assert(!has_foo_function_exact_v<int, long>);
```

Тип результата и спецификатор `noexcept`, проверенные отдельно:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp properties -->
```cpp
static_assert(std::is_same_v<foo_function_exact_t<double, double>, double>);
static_assert(!noexcept(foo(std::declval<int>(), std::declval<int>())));
```

Вызов дружественной функции, определённой внутри класса, компилируется, но шаблонная константа
строгого соответствия для неё равна `false`:

<!-- snippet: example/type_traits/function_detection/type_traits_function_detection_example.cpp adl -->
```cpp
namespace shapes
{
    struct circle
    {
        double radius;

        friend double foo(circle shape, double factor) { return shape.radius * factor; }
    };
} // namespace shapes

static_assert(has_foo_function_v<shapes::circle, double>);
static_assert(!has_foo_function_exact_v<shapes::circle, double>);
```

## Примечания

- Квалификаторы `const` и `volatile` верхнего уровня у типа параметра не входят в тип функции,
  поэтому `void f(int)` и `void f(int const)` являются объявлениями одной и той же функции. По этой
  причине в строгом соответствии такие квалификаторы не учитываются, и для функции `f(int)`
  шаблонная константа `has_f_function_exact_v<int const>` тоже равна `true`. Квалификаторы, которые
  стоят не на верхнем уровне, учитываются, и для функции `f(int &)` шаблонная константа
  `has_f_function_exact_v<int const &>` равна `false`.
- Дружественная функция, определённая внутри класса, видна только поиску по аргументам (ADL).
  Поэтому для неё шаблонная константа `has_foo_function_v` равна `true`, а шаблонная константа
  `has_foo_function_exact_v` равна `false`, как в последнем примере.

## Смотрите также

- [Обнаружение наличия метода](method_detection.md) - те же проверки для метода
- [Обнаружение наличия выражения](detection.md) - шаблоны `scl::is_detected` и `scl::detected_t`,
  на которых построены объявленные шаблоны
- [`example/type_traits/function_detection/type_traits_function_detection_example.cpp`](../../../../example/type_traits/function_detection/type_traits_function_detection_example.cpp)

---

Назад: [Обнаружение наличия выражения](detection.md) |
Далее: [Обнаружение наличия метода](method_detection.md) | [К группе](index.md) |
[К главной странице](../Main.md)
