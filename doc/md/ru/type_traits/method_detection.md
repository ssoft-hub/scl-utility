# Обнаружение наличия метода

Макрос `SCL_METHOD_DETECTION(add)` объявляет шаблоны для проверки метода с именем `add` у объекта
заданного типа:

- можно ли вызвать метод `add` для объекта с аргументами заданных типов, которые при вызове могут
  неявно преобразовываться в типы параметров (нестрогое соответствие)
- объявлен ли метод `add`, параметры которого в точности совпадают с заданными типами,
  а квалификаторы соответствуют типу объекта (строгое соответствие)
- можно ли вызвать статический метод `add` через имя класса и объявлен ли статический метод
  с заданными типами параметров.

Проверки выполняются при компиляции программы, и для них достаточно объявления метода, определение
не нужно. Имя проверяемого метода передаётся в макрос вместо имени `add`, и из него составляются
имена объявленных шаблонов. Например, для макроса `SCL_METHOD_DETECTION(resize)` объявляются
шаблонные константы `has_resize_method_v`, `has_resize_method_exact_v` и остальные шаблоны с тем
же корнем.

- Заголовок: `#include <scl/utility/type_traits/detection/method.h>`

Макрос можно раскрывать в пространстве имён или в классе. Раскрытие `SCL_METHOD_DETECTION(add)`
содержит объявления ниже. Каждому шаблону раскрытия первым аргументом передаётся тип объекта `Type`
вместе с его cv-квалификаторами и ссылкой, а за ним передаются типы аргументов вызова. Например,
шаблонная константа `has_add_method_v<counter &, int>` равна `true`, если метод `add` можно вызвать
с аргументом типа `int` для lvalue типа `counter`.

```cpp
#define SCL_METHOD_DETECTION(name)

template <typename Type, typename... Arguments>
using add_method_operation = decltype(std::declval<Type>().add(std::declval<Arguments>()...));
template <typename... Arguments>
inline static constexpr auto has_add_method_v = scl::is_detected_v<add_method_operation, Arguments...>;
template <typename... Arguments>
using add_method_t = scl::detected_t<add_method_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_method_exact_operation = unspecified;
template <typename... Arguments>
inline static constexpr auto has_add_method_exact_v = scl::is_detected_v<add_method_exact_operation, Arguments...>;
template <typename... Arguments>
using add_method_exact_t = scl::detected_t<add_method_exact_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_static_method_operation = decltype(std::remove_cvref_t<Type>::add(std::declval<Arguments>()...));
template <typename... Arguments>
inline static constexpr auto has_add_static_method_v = scl::is_detected_v<add_static_method_operation, Arguments...>;
template <typename... Arguments>
using add_static_method_t = scl::detected_t<add_static_method_operation, Arguments...>;

template <typename Type, typename... Arguments>
using add_static_method_exact_operation = unspecified;
template <typename... Arguments>
inline static constexpr auto has_add_static_method_exact_v =
    scl::is_detected_v<add_static_method_exact_operation, Arguments...>;
template <typename... Arguments>
using add_static_method_exact_t = scl::detected_t<add_static_method_exact_operation, Arguments...>;
```

## Семантика

### Общие правила

- **Объект.** Объект передаётся в вызов как `std::declval<Type>()`. Для типа `Type` вида `X &`
  объект является lvalue, а для типов `X` и `X &&` объект является rvalue. Квалификаторы `const`
  и `volatile` типа `Type` становятся квалификаторами объекта.
- **Нет такого члена.** Для класса без члена `add` все шаблонные константы равны `false`.

### Нестрогое соответствие

- **Условие.** Шаблонная константа `has_add_method_v<Type, Arguments...>` равна `true`, если вызов
  `std::declval<Type>().add(std::declval<Arguments>()...)` компилируется, и `false` в остальных
  случаях. Аргументы при этом могут неявно преобразовываться в типы параметров, а метод выбирается
  по правилам разрешения перегрузки. Например, для метода `void add(int) &` шаблонная константа
  `has_add_method_v<counter &, short>` равна `true`. Шаблонный тип
  `add_method_t<Type, Arguments...>` является типом результата вызова или маркерным типом
  [`scl::nonesuch`](detection.md), если вызов не компилируется.

### Строгое соответствие

- **Условие.** Шаблонная константа `has_add_method_exact_v<Type, Arguments...>` равна `true`,
  если в классе `std::remove_cvref_t<Type>` или в его открытом базовом классе объявлен метод `add`,
  параметры которого имеют в точности типы `Arguments...`, квалификаторы которого соответствуют типу
  `Type`, и этот метод не удалён (`= delete`) и доступен. Метод закрытого или защищённого базового
  класса не находится, даже если в классе `std::remove_cvref_t<Type>` он открыт объявлением `using`.
  Квалификаторы `const` и `volatile` верхнего уровня у типов `Arguments...` не учитываются,
  подробнее об этом сказано в разделе "Примечания". В этой форме находится только метод с неявным
  параметром объекта, то есть обычный нестатический метод. Метод с явным параметром объекта
  проверяется отдельной шаблонной константой, описанной ниже.
- **Соответствие квалификаторов.** В строгом соответствии cv-квалификаторы метода должны совпадать
  с cv-квалификаторами типа `Type`, а ссылочный квалификатор метода должен быть равен `&` для типа
  `X &`, `&&` для типа `X &&` и отсутствовать для типа `X`. Например, для типа `X const &` шаблонная
  константа равна `true` только для метода `add(...) const &`, а для метода `add(...) const`
  без ссылочного квалификатора она равна `false`, хотя такой метод можно вызвать для этого объекта.
- **Тип результата.** Тип результата метода в строгом соответствии не сравнивается. Шаблонный тип
  `add_method_exact_t<Type, Arguments...>` является типом результата найденного метода, и его можно
  сравнить с ожидаемым типом, например с помощью `std::is_same_v`.
- **Неоднозначный вызов.** В строгом соответствии метод находится и тогда, когда обычный вызов
  неоднозначен. Например, для перегрузок `long add(int) &` и `char add(int &&) &` шаблонная
  константа `has_add_method_v<X &, int>` равна `false`, а шаблонные константы
  `has_add_method_exact_v<X &, int>` и `has_add_method_exact_v<X &, int &&>` равны `true`.
- **Шаблон метода среди перегрузок.** Если среди перегрузок `add` есть шаблон метода, метод
  с нужными параметрами выбирается по типу результата обычного вызова, то есть берётся перегрузка,
  тип результата которой совпадает с типом результата вызова
  `std::declval<Type>().add(std::declval<Arguments>()...)`. Если этот вызов неоднозначен, шаблонная
  константа `has_add_method_exact_v` равна `false`.
- **Спецификатор `noexcept`.** Спецификатор `noexcept` метода в строгом соответствии
  не сравнивается. Его можно проверить оператором `noexcept`, применённым к вызову, как в примере
  ниже. Если вызов неоднозначен, а перегрузки различаются не только квалификаторами, спецификатор
  виден в типе указателя на нужную перегрузку, который возвращает
  [`scl::overload_cast`](overload_cast.md).

### Статический метод и явный параметр объекта

- **Статический метод.** Шаблонная константа `has_add_static_method_v<Type, Arguments...>` равна
  `true`, если вызов через имя класса `std::remove_cvref_t<Type>::add(std::declval<Arguments>()...)`
  компилируется. Шаблонная константа `has_add_static_method_exact_v<Type, Arguments...>` равна
  `true`, если объявлен статический метод `add`, параметры которого имеют в точности типы
  `Arguments...`. Как и в строгом соответствии метода, такой статический метод находится и тогда,
  когда вызов через имя класса неоднозначен. Тип объекта `Type` для статических форм задаёт
  только класс, его квалификаторы не учитываются.

### Ограничения

- **MSVC.** На MSVC доступ не проверяется при подстановке в вызове нестатического метода.
  Поэтому закрытый и защищённый методы находятся в нестрогом соответствии, как и метод закрытого
  или защищённого базового класса. В строгом соответствии и в статических формах закрытый
  или защищённый метод находится на MSVC, только если в классе объявлены и другие перегрузки этого
  имени. Адрес удалённого метода на MSVC тоже не отбрасывается, поэтому там строгое соответствие
  дополнительно требует, чтобы компилировался обычный вызов, а статическая строгая форма требует,
  чтобы компилировался вызов через имя класса. Поэтому на MSVC при неоднозначном вызове шаблонные
  константы `has_add_method_exact_v` и `has_add_static_method_exact_v` равны `false`.

## Примеры

Класс и шаблоны, объявленные для его методов:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp declare -->
```cpp
class counter
{
private:
    int m_value{};

public:
    int value() const noexcept { return m_value; }
    void add(int step) & { m_value += step; }
    static counter zero() { return {}; }
};

SCL_METHOD_DETECTION(value)
SCL_METHOD_DETECTION(add)
SCL_METHOD_DETECTION(zero)
```

Нестрогое соответствие для объектов с разными квалификаторами и категорией значения:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp call -->
```cpp
static_assert(has_value_method_v<counter>);
static_assert(has_value_method_v<counter const &>);
static_assert(std::is_same_v<value_method_t<counter>, int>);
static_assert(has_add_method_v<counter &, int>);
static_assert(has_add_method_v<counter &, short>);
static_assert(!has_add_method_v<counter, int>);
```

Строгое соответствие квалификаторов объекта и типов параметров. Метод `value()` объявлен
с квалификатором `const`, поэтому шаблонная константа равна `true` для типа `counter const`
и `false` для типа `counter`. Метод `add` принимает параметр типа `int`, поэтому для типа аргумента
`short` шаблонная константа равна `false`:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp exact -->
```cpp
static_assert(has_value_method_exact_v<counter const>);
static_assert(!has_value_method_exact_v<counter>);
static_assert(has_add_method_exact_v<counter &, int>);
static_assert(!has_add_method_exact_v<counter &, short>);
```

Тип результата и спецификатор `noexcept`, проверенные отдельно:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp properties -->
```cpp
static_assert(std::is_same_v<value_method_exact_t<counter const>, int>);
static_assert(noexcept(std::declval<counter const &>().value()));
```

Статический метод:

<!-- snippet: example/type_traits/method_detection/type_traits_method_detection_example.cpp static -->
```cpp
static_assert(has_zero_static_method_v<counter>);
static_assert(has_zero_static_method_exact_v<counter>);
static_assert(!has_value_static_method_v<counter>);
```

## Примечания

- Метод с квалификатором `&` без квалификатора `const` можно вызвать только для lvalue
  без квалификатора `const`, то есть для типа `X &`. Метод с квалификатором `const &` можно вызвать
  для любого объекта без квалификатора `volatile`. Метод с квалификатором `&&` можно вызвать
  только для rvalue, то есть для типов `X` и `X &&`. Метод с квалификатором `const` можно вызвать
  и для объекта без квалификатора `const`, поэтому в нестрогом соответствии такой метод находится
  и для типа `X`, и для типа `X const`.
- Квалификаторы `const` и `volatile` верхнего уровня у типа параметра не входят в тип функции.
  Поэтому в строгом соответствии они не учитываются, и для метода `add(int) &` шаблонная константа
  `has_add_method_exact_v<X &, int const>` тоже равна `true`.

## Смотрите также

- [Обнаружение наличия функции](function_detection.md) - те же проверки для функции
- [Обнаружение наличия оператора](operator_detection.md) - те же проверки для оператора,
  объявленного как метод класса
- [`example/type_traits/method_detection/type_traits_method_detection_example.cpp`](../../../../example/type_traits/method_detection/type_traits_method_detection_example.cpp)

---

Назад: [Обнаружение наличия функции](function_detection.md) |
Далее: [Обнаружение наличия оператора](operator_detection.md) | [К группе](index.md) |
[К главной странице](../Main.md)
