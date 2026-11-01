# Обнаружение наличия оператора

Шаблоны заголовка `operator.h` предназначены для проверки оператора C++ для операндов заданных
типов:

- применим ли оператор к операндам, то есть компилируется ли выражение с ним, каким бы ни был
  оператор, встроенным, методом класса или свободной функцией
- можно ли вызвать оператор как метод класса с аргументами, которые при вызове могут неявно
  преобразовываться в типы параметров (нестрогое соответствие)
- объявлен ли оператор как метод класса с параметрами, которые в точности совпадают с заданными
  типами, и с квалификаторами, которые соответствуют типу объекта (строгое соответствие).

Кроме того, проверяется, объявлен ли оператор как метод с явным параметром объекта (C++23),
а для операторов `()` и `[]` также как статический метод (C++23). Проверки выполняются
при компиляции программы. Для каждой проверки объявлены шаблонная константа `has_..._v`
и шаблонный тип `..._t` с типом результата.

- Заголовок: `#include <scl/utility/type_traits/detection/operator.h>`

Ниже приведены объявления для бинарного оператора `+`, которому дано имя `plus`. Такие же шаблоны
объявлены для каждого оператора из таблицы, и в их именах вместо `plus` стоит имя этого оператора.
Отличия некоторых операторов от этой схемы описаны в разделе "Семантика".

```cpp
namespace scl
{
    template <typename Left, typename Right>
    using plus_operation = decltype(std::declval<Left>() + std::declval<Right>());
    template <typename... Arguments>
    inline constexpr auto has_plus_v = is_detected_v<plus_operation, Arguments...>;
    template <typename... Arguments>
    using plus_t = detected_t<plus_operation, Arguments...>;

    template <typename Type, typename... Arguments>
    using plus_member_operation = decltype(std::declval<Type>().operator+(std::declval<Arguments>()...));
    template <typename... Arguments>
    inline constexpr auto has_plus_member_v = is_detected_v<plus_member_operation, Arguments...>;
    template <typename... Arguments>
    using plus_member_t = detected_t<plus_member_operation, Arguments...>;

    template <typename Type, typename... Arguments>
    using plus_member_exact_operation = unspecified;
    template <typename... Arguments>
    inline constexpr auto has_plus_member_exact_v = is_detected_v<plus_member_exact_operation, Arguments...>;
    template <typename... Arguments>
    using plus_member_exact_t = detected_t<plus_member_exact_operation, Arguments...>;

    template <typename Type, typename... Arguments>
    using plus_explicit_member_exact_operation = unspecified;
    template <typename... Arguments>
    inline constexpr auto has_plus_explicit_member_exact_v =
        is_detected_v<plus_explicit_member_exact_operation, Arguments...>;
    template <typename... Arguments>
    using plus_explicit_member_exact_t = detected_t<plus_explicit_member_exact_operation, Arguments...>;
}
```

## Операторы

| Вид | Оператор и имя |
|---|---|
| Префиксные | `&` `address_of`, `*` `indirection`, `+` `unary_plus`, `-` `negate`, `++` `prefix_increment`, `--` `prefix_decrement`, `~` `bit_not`, `!` `logical_not` |
| Постфиксные | `++` `postfix_increment`, `--` `postfix_decrement` |
| Сравнения | `==` `equal_to`, `!=` `not_equal_to`, `<` `less`, `<=` `less_equal`, `>` `greater`, `>=` `greater_equal`, `<=>` `compare_three_way` |
| Арифметические и поразрядные | `*` `multiplies`, `/` `divides`, `%` `modulus`, `+` `plus`, `-` `minus`, `<<` `left_shift`, `>>` `right_shift`, `&` `bit_and`, `\|` `bit_or`, `^` `bit_xor` |
| Логические | `&&` `logical_and`, `\|\|` `logical_or` |
| Присваивания | `=` `assign`, `*=` `multiplies_assign`, `/=` `divides_assign`, `%=` `modulus_assign`, `+=` `plus_assign`, `-=` `minus_assign`, `<<=` `left_shift_assign`, `>>=` `right_shift_assign`, `&=` `bit_and_assign`, `\|=` `bit_or_assign`, `^=` `bit_xor_assign` |
| Прочие | `->*` `pointer_to_member_access`, `->` `member_access`, `,` `comma`, `[]` `subscript`, `()` `call` |

Если в стандартной библиотеке есть функциональный объект для того же оператора, например
`std::multiplies` или `std::bit_and`, имя оператора совпадает с именем этого шаблона класса.

## Семантика

### Форма выражения

- **Условие.** Шаблонная константа `has_plus_v<Left, Right>` равна `true`, если выражение
  `std::declval<Left>() + std::declval<Right>()` компилируется, и `false` в остальных случаях.
  Оператор при этом может быть встроенным, методом класса или свободной функцией, и он выбирается
  по правилам разрешения перегрузки. Шаблонный тип `plus_t<Left, Right>` является типом результата
  выражения или маркерным типом [`scl::nonesuch`](detection.md), если выражение не компилируется.
  Для префиксного оператора шаблонам выражения передаётся один тип операнда, например
  `has_negate_v<int>`, для оператора `[]` первым передаётся тип объекта, а за ним тип одного
  индекса, например `has_subscript_v<std::vector<int> &, std::size_t>`. Для оператора `()` за типом
  объекта передаются типы аргументов.
- **Категория значения операнда.** Каждый операнд передаётся в выражение как `std::declval` своего
  типа. Для типа операнда вида `X &` операнд является lvalue, а для типов `X` и `X &&` операнд
  является rvalue. Поэтому для встроенных операторов, которым нужен lvalue, тип операнда следует
  передавать со ссылкой `&`, как в примере `has_prefix_increment_v<int &>` ниже.

### Формы метода

- **Нестрогое соответствие.** Шаблонная константа `has_plus_member_v<Type, Arguments...>` равна
  `true`, если вызов метода `std::declval<Type>().operator+(std::declval<Arguments>()...)`
  компилируется. Аргументы при этом могут неявно преобразовываться в типы параметров. Встроенный
  оператор и свободная функция `operator+` в этой проверке не учитываются, потому что проверяется
  вызов именно метода. Шаблонный тип `plus_member_t` является типом результата этого вызова. Вызов
  метода `operator[]` с несколькими индексами (C++23) проверяется только формами метода, например
  `has_subscript_member_v<X &, int, int>`, а шаблоны выражения принимают один индекс.
- **Строгое соответствие.** Шаблонная константа `has_plus_member_exact_v<Type, Arguments...>` равна
  `true`, если в классе `std::remove_cvref_t<Type>` или в его открытом базовом классе объявлен метод
  `operator+`, параметры которого имеют в точности типы `Arguments...`, квалификаторы которого
  соответствуют типу `Type`, и этот метод не удалён и доступен. Квалификаторы должны соответствовать
  типу `Type` по тем же правилам, что на странице [обнаружения наличия метода](method_detection.md).
  Например, для типа `X const` нужен метод с квалификатором `const` без ссылочного квалификатора,
  для типа `X &` нужен метод с квалификатором `&` и так далее. В этой форме находится только метод
  с неявным параметром объекта.
- **Тип результата и неоднозначный вызов.** Тип результата метода в строгом соответствии
  не сравнивается. Шаблонный тип `plus_member_exact_t` является типом результата найденного метода.
  Метод находится и тогда, когда вызов метода `operator+` неоднозначен, например для перегрузок
  `long operator+(int) const` и `char operator+(int &&) const`. Если среди перегрузок `operator+`
  есть шаблон метода, находится только перегрузка, тип результата которой совпадает с типом
  результата вызова `std::declval<Type>().operator+(std::declval<Arguments>()...)`.
- **Спецификатор `noexcept`.** Спецификатор `noexcept` оператора в строгом соответствии
  не сравнивается. Его можно проверить оператором `noexcept`, применённым к вызову
  `std::declval<Type>().operator+(std::declval<Arguments>()...)`. Если этот вызов неоднозначен,
  спецификатор виден в типе указателя на нужную перегрузку, который возвращает
  [`scl::member_overload_cast`](overload_cast.md) для адреса оператора.

### Явный параметр объекта и статические операторы

- **Явный параметр объекта.** Шаблонная константа
  `has_plus_explicit_member_exact_v<Type, Arguments...>` равна `true`, если объявлен метод
  `operator+` с явным параметром объекта (C++23), тип которого в точности равен типу `Type`,
  с остальными параметрами в точности типов `Arguments...`, и этот метод не удалён и доступен.
  Правила этой формы те же, что у формы с явным параметром объекта на странице
  [обнаружения наличия метода](method_detection.md). Статическими могут быть только операторы `()`
  и `[]`, поэтому только для них оператор с явным параметром объекта не находится рядом
  со статическим оператором, который принимает объект первым.
- **Статические `operator()` и `operator[]`.** Шаблонная константа
  `has_call_static_member_v<Type, Arguments...>` равна `true`, если вызов через имя класса
  `std::remove_cvref_t<Type>::operator()(std::declval<Arguments>()...)` компилируется. Шаблонная
  константа `has_call_static_member_exact_v<Type, Arguments...>` равна `true`, если объявлен
  статический `operator()`, параметры которого имеют в точности типы `Arguments...`. Такие
  же шаблоны с именем `subscript`, например `has_subscript_static_member_exact_v`, объявлены
  для `operator[]`. Как и статическая строгая форма метода, шаблонная константа
  `has_call_static_member_exact_v` равна `true` и для оператора `operator()` с явным параметром
  объекта, если тип его параметра объекта совпадает с первым из типов `Arguments...`. На MSVC
  для такого оператора шаблонная константа равна `false`, как описано в пункте "MSVC" ниже.

### Операторы с собственными правилами

- **Операторы `->` и `->*`.** Для операторов `->` и `->*` выражение не проверяется, а шаблонные
  константы `has_member_access_v` и `has_pointer_to_member_access_v` равны `true`,
  если компилируется вызов метода `operator->` или `operator->*`. Поэтому для встроенного указателя
  и для свободной функции `operator->*` эти шаблонные константы равны `false`.
- **Постфиксные `++` и `--`.** Шаблонам `has_postfix_increment_member_v`,
  `has_postfix_increment_member_exact_v` и другим шаблонам постфиксных операторов `++` и `--`,
  которые проверяют метод, передаётся только тип объекта. Вызов `operator++(0)` или `operator--(0)`
  с фиктивным аргументом типа `int` проверяется для этого объекта.

### Ограничения

- **MSVC.** На MSVC доступ проверяется так же, как описано на странице
  [обнаружения наличия метода](method_detection.md). Поэтому закрытый и защищённый методы оператора
  находятся в форме выражения и в нестрогой форме метода, а в строгой форме только в том случае,
  если в классе объявлены и другие перегрузки этого оператора. Строгое соответствие на MSVC
  также требует, чтобы вызов метода оператора компилировался, поэтому там при неоднозначном вызове
  метода `operator+` шаблонная константа `has_plus_member_exact_v` равна `false`. Оператор
  `operator()` или `operator[]` с явным параметром объекта нельзя вызвать через имя класса,
  поэтому на MSVC шаблонные константы `has_call_static_member_exact_v`
  и `has_subscript_static_member_exact_v` для такого оператора равны `false`.

## Примеры

Типы примеров:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp types -->
```cpp
struct money
{
    long cents;

    money operator+(money other) const { return {cents + other.cents}; }
    bool operator==(money const & other) const = default;
};

money operator*(money amount, int times);
```

Выражение и тип его результата. Сложение объектов типа `money` объявлено методом, а умножение
на `int` объявлено свободной функцией, и шаблонная константа выражения равна `true` для обоих
операторов:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp expression -->
```cpp
static_assert(scl::has_plus_v<money, money>);
static_assert(std::is_same_v<scl::plus_t<money, money>, money>);
static_assert(scl::has_multiplies_v<money, int>);
static_assert(!scl::has_multiplies_v<money, money>);
static_assert(scl::has_less_v<int, double>);
static_assert(!scl::has_less_v<money, money>);
```

Метод и строгое соответствие. Метод `operator+` объявлен с квалификатором `const`, без ссылочного
квалификатора и с параметром `money`. Поэтому шаблонная константа строгого соответствия равна `true`
для объекта типа `money const` и аргумента типа `money`. Для объекта типа `money` она равна `false`,
потому что у метода есть квалификатор `const`, а у объекта его нет. Для аргумента типа
`money const &` она тоже равна `false`, потому что тип параметра `money` не совпадает с ним
в точности. Свободная функция `operator*` методом не является, поэтому шаблонная константа
`has_multiplies_member_v` равна `false`:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp member -->
```cpp
static_assert(scl::has_plus_member_v<money, money>);
static_assert(!scl::has_multiplies_member_v<money, int>);
static_assert(scl::has_plus_member_exact_v<money const, money>);
static_assert(!scl::has_plus_member_exact_v<money, money>);
static_assert(!scl::has_plus_member_exact_v<money const, money const &>);
```

Индексирование, вызов и префиксный инкремент:

<!-- snippet: example/type_traits/operator_detection/type_traits_operator_detection_example.cpp special -->
```cpp
static_assert(scl::has_subscript_v<std::vector<int> &, std::size_t>);
static_assert(std::is_same_v<scl::subscript_t<std::vector<int> &, std::size_t>, int &>);
static_assert(scl::has_call_v<std::plus<>, int, int>);
static_assert(scl::has_prefix_increment_v<int &>);
static_assert(!scl::has_prefix_increment_v<int>);
```

## Примечания

- Встроенный префиксный инкремент применим только к lvalue. Поэтому шаблонная константа
  `has_prefix_increment_v<int>` равна `false`, а шаблонная константа `has_prefix_increment_v<int &>`
  равна `true`.
- Постфиксный оператор отличается от префиксного фиктивным параметром типа `int`, по которому
  различаются объявления `operator++()` и `operator++(int)`.
- Квалификаторы `const` и `volatile` верхнего уровня у типа параметра не входят в тип функции.
  Поэтому в строгом соответствии они не учитываются, и для метода `operator+(money) const` шаблонная
  константа `has_plus_member_exact_v<money const, money const>` тоже равна `true`.

## Смотрите также

- [Обнаружение наличия метода](method_detection.md) - те же проверки для метода с любым именем
- [Обнаружение наличия выражения](detection.md) - шаблоны `scl::is_detected` и `scl::detected_t`,
  на которых построены шаблоны заголовка `operator.h`
- [`example/type_traits/operator_detection/type_traits_operator_detection_example.cpp`](../../../../example/type_traits/operator_detection/type_traits_operator_detection_example.cpp)

---

Назад: [Обнаружение наличия метода](method_detection.md) |
Далее: [Приведение значения к категории и cv-квалификаторам заданного типа](forward_like.md) |
[К группе](index.md) | [К главной странице](../Main.md)
