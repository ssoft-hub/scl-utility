# `signature_t` и пространство имён `signature`

Семейство шаблонных типов и шаблонных констант `signature` предназначено для определения на
этапе компиляции свойств типов вызываемых объектов (свободных функций, методов классов,
лямбда-выражений и т.д.), таких как типы параметров, тип возвращаемого значения,
cv-квалификаторы и ссылочный квалификатор.

- Заголовок: `#include <scl/utility/type_traits/signature.h>`

Примеры на этой странице построены на классе `printer` и на типе `print_t` указателя на его
метод `print`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp types -->
```cpp
struct printer
{
    int print(long value, ::std::string const & label) const & noexcept;
    void clear();
    void clear(int line);
};

using print_t = decltype(&printer::print); // int (printer::*)(long, std::string const &) const & noexcept
```

В семейство `signature` входят:

| Шаблон | Свойство | Значение для типа `print_t` |
|---|---|---|
| `scl::signature_t<F>` | тип функции | `int(long, std::string const &) noexcept` |
| `scl::signature::result_t<F>` | тип возвращаемого значения | `int` |
| `scl::signature::parameters_t<F>` | типы параметров в виде `std::tuple` | `std::tuple<long, std::string const &>` |
| `scl::signature::parameter_count_v<F>` | число параметров | `2` |
| `scl::signature::parameter_t<F, I>` | тип параметра с индексом `I` | `std::string const &` при `I = 1` |
| `scl::signature::object_t<F>` | тип объекта, для которого вызван метод | `printer const &` |
| `scl::signature::is_const_v<F>` | объявлен ли метод с квалификатором `const` | `true` |
| `scl::signature::is_volatile_v<F>` | объявлен ли метод с квалификатором `volatile` | `false` |
| `scl::signature::is_lvalue_reference_v<F>` | объявлен ли метод со ссылочным квалификатором `&` | `true` |
| `scl::signature::is_rvalue_reference_v<F>` | объявлен ли метод со ссылочным квалификатором `&&` | `false` |
| `scl::signature::is_noexcept_v<F>` | объявлена ли функция со спецификатором `noexcept` | `true` |
| `scl::signature::is_variadic_v<F>` | принимает ли функция переменное число аргументов | `false` |

## Вызываемые типы

Параметром `F` может быть:

- тип функции, например `int(long)`;
- указатель или ссылка на функцию;
- указатель на метод с любым сочетанием cv-квалификаторов, ссылочного квалификатора и
  спецификатора `noexcept`;
- класс или объединение с единственным оператором вызова, который не является шаблоном, в том
  числе тип лямбда-выражения.

Любой из этих типов может описывать функцию с переменным числом аргументов в стиле C, то есть список
параметров может заканчиваться `...`, как у функции `int printf(char const *, ...)`. Речь не о
шаблонах с переменным числом параметров: специализация `F<Args...>` уже является конкретным типом.
Для класса с cv-квалификаторами и для ссылки на класс значения шаблонов такие же, как для самого
класса. Класс должен быть полным типом: обращение к шаблонам семейства до определения класса делает
программу некорректной, и компилятор не обязан об этом сообщать.

Начиная с C++23 объект можно передать в оператор вызова через явный параметр объекта. Такой
параметр не входит в список параметров типа `signature_t<F>`, а тип этого параметра равен
`signature::object_t<F>`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp explicit -->
```cpp
#if defined(__cpp_explicit_this_parameter)
struct stepper
{
    int operator()(this stepper const & self, long step);
};

static_assert(::std::is_same_v<::scl::signature_t<stepper>, int(long)>);
static_assert(::std::is_same_v<::scl::signature::object_t<stepper>, stepper const &>);
#endif
```

У статического оператора вызова, который тоже появился в C++23, объекта нет, и все его
параметры входят в список параметров типа `signature_t<F>`.

У обобщённого лямбда-выражения, у класса с перегруженным оператором вызова и у типа, объект которого
нельзя вызвать, единственной сигнатуры нет. Сигнатуры нет также у указателя на поле класса, у типа
функции с cv- или ссылочным квалификатором, например `int() const &`, и у функции с соглашением о
вызове, отличным от принятого по умолчанию, например `__vectorcall`, а на 32-битном x86 и
`__stdcall`. Для таких типов обращение к шаблонам семейства не компилируется. Подстановка в
шаблонный тип `signature_t` при этом не даёт типа, поэтому проверить наличие сигнатуры можно
выражением `requires`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp absent -->
```cpp
template <typename Callable>
concept has_signature = requires { typename ::scl::signature_t<Callable>; };

auto const generic = [](auto value) { return value; };
static_assert(!has_signature<decltype(generic)>);
static_assert(!has_signature<int>);
```

Для перегруженной функции или перегруженного метода следует сначала выбрать нужную перегрузку
с помощью функционального объекта `scl::overload_cast`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp overload -->
```cpp
using clear_t = decltype(::scl::overload_cast<int>(&printer::clear));

static_assert(::std::is_same_v<::scl::signature_t<clear_t>, void(int)>);
```

## `signature_t`

```cpp
namespace scl
{
    template <typename Callable>
    using signature_t = /* Result(Parameters...) noexcept(...) */;
}
```

Шаблонный тип `signature_t` является типом функции, из которого удалены указатель, класс,
cv-квалификаторы и ссылочный квалификатор. В этом типе сохраняются спецификатор `noexcept` и
`...` в конце списка параметров.

## `signature::result_t`

```cpp
namespace scl::signature
{
    template <typename Callable>
    using result_t = /* Result */;
}
```

Шаблонный тип `signature::result_t` является типом возвращаемого значения.

## `signature::parameters_t`, `signature::parameter_count_v` и `signature::parameter_t`

```cpp
namespace scl::signature
{
    template <typename Callable>
    using parameters_t = ::std::tuple</* Parameters... */>;

    template <typename Callable>
    inline constexpr ::std::size_t parameter_count_v = ::std::tuple_size_v<parameters_t<Callable>>;

    template <typename Callable, ::std::size_t Index> // Index < parameter_count_v<Callable>
    using parameter_t = ::std::tuple_element_t<Index, parameters_t<Callable>>;
}
```

Шаблонный тип `signature::parameters_t` является специализацией шаблона `std::tuple`, аргументы
которой совпадают с типами параметров функции в порядке их объявления. Шаблонная константа
`signature::parameter_count_v` равна числу параметров функции. Шаблонный тип
`signature::parameters_t` не содержит `...` в конце списка параметров, и шаблонная константа
`signature::parameter_count_v` его не считает.

Шаблонный тип `signature::parameter_t` является типом параметра с индексом `Index`. Если значение
параметра `Index` не меньше числа параметров функции, обращение к шаблонному типу
`signature::parameter_t` не компилируется, и это тоже можно проверить выражением `requires`.

Тип функции, тип возвращаемого значения и типы параметров метода `printer::print`. Здесь и ниже
`print_t` является типом указателя на этот метод, объявленным в начале страницы:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp signature -->
```cpp
static_assert(::std::is_same_v<::scl::signature_t<print_t>, int(long, ::std::string const &) noexcept>);
static_assert(::std::is_same_v<::scl::signature::result_t<print_t>, int>);
static_assert(::std::is_same_v<::scl::signature::parameters_t<print_t>,
    ::std::tuple<long, ::std::string const &>>);
static_assert(::std::is_same_v<::scl::signature::parameter_t<print_t, 1>, ::std::string const &>);
```

## `signature::object_t`

```cpp
namespace scl::signature
{
    template <typename Callable>
    using object_t = /* класс с cv-квалификаторами и ссылочным квалификатором метода */;
}
```

Шаблонный тип `signature::object_t` является типом объекта, для которого вызван метод или
оператор вызова. Этот тип является классом метода с теми cv-квалификаторами и ссылочным
квалификатором, с которыми объявлен метод. Если метод класса `widget` объявлен с
квалификаторами `const &`, это тип `widget const &`, а если квалификаторов нет, это тип
`widget`. Тип класса без квалификаторов можно определить с помощью
`std::remove_cvref_t<signature::object_t<F>>`.

Для класса с оператором вызова шаблонный тип `signature::object_t` является самим этим классом, даже
если оператор вызова объявлен в базовом классе. Для оператора вызова с явным параметром объекта
шаблонный тип `signature::object_t` является типом этого параметра, в том числе когда оператор
вызова объявлен в базовом классе.

Тип объекта имеет тот же вид, что и параметр `Type` шаблонного типа `scl::member_like_t`, поэтому
для указателя на метод с постоянным числом аргументов этот указатель можно получить с помощью
`scl::member_like_t<signature::object_t<F>, signature_t<F>>`. Для типа `print_t` указателя на
метод `printer::print`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp object -->
```cpp
static_assert(::std::is_same_v<::scl::signature::object_t<print_t>, printer const &>);
static_assert(::std::is_same_v<
    ::scl::member_like_t<::scl::signature::object_t<print_t>, ::scl::signature_t<print_t>>,
    print_t>);
```

Для свободной функции и для статического оператора вызова обращение к шаблонному типу
`signature::object_t` не компилируется.

## `signature::is_const_v` и `signature::is_volatile_v`

```cpp
namespace scl::signature
{
    template <typename Callable>
    inline constexpr bool is_const_v = /* ... */;

    template <typename Callable>
    inline constexpr bool is_volatile_v = /* ... */;
}
```

Шаблонная константа `signature::is_const_v` равна `true`, если метод или оператор вызова
объявлен с квалификатором `const`. Шаблонная константа `signature::is_volatile_v` равна
`true`, если метод или оператор вызова объявлен с квалификатором `volatile`. Для свободной
функции и для метода без cv-квалификаторов обе константы равны `false`. Для оператора вызова
с явным параметром объекта значения определяются по типу этого параметра.

## `signature::is_lvalue_reference_v` и `signature::is_rvalue_reference_v`

```cpp
namespace scl::signature
{
    template <typename Callable>
    inline constexpr bool is_lvalue_reference_v = /* ... */;

    template <typename Callable>
    inline constexpr bool is_rvalue_reference_v = /* ... */;
}
```

Шаблонная константа `signature::is_lvalue_reference_v` равна `true`, если метод или оператор
вызова объявлен со ссылочным квалификатором `&`. Шаблонная константа
`signature::is_rvalue_reference_v` равна `true`, если метод или оператор вызова объявлен со
ссылочным квалификатором `&&`. Для свободной функции и для метода без ссылочного квалификатора
обе константы равны `false`.

Для метода значения этих констант совпадают со значениями шаблонных констант
`std::is_lvalue_reference_v` и `std::is_rvalue_reference_v` для типа
`signature::object_t<F>`.

## `signature::is_noexcept_v` и `signature::is_variadic_v`

```cpp
namespace scl::signature
{
    template <typename Callable>
    inline constexpr bool is_noexcept_v = /* ... */;

    template <typename Callable>
    inline constexpr bool is_variadic_v = /* ... */;
}
```

Шаблонная константа `signature::is_noexcept_v` равна `true`, если функция, метод или оператор
вызова объявлен со спецификатором `noexcept`. Значение соответствует объявлению, а не вызову:
для функции `void f(std::string) noexcept` оно равно `true`, хотя копирование аргумента в
параметр может выбросить исключение.

Шаблонная константа `signature::is_variadic_v` равна `true`, если функция, метод или оператор вызова
принимает переменное число аргументов, то есть список параметров заканчивается `...`, как у типа
функции `int(char const *, ...)`.

## Примеры

Квалификаторы и спецификатор `noexcept` метода `printer::print`, указатель на который имеет тип
`print_t`:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp qualifiers -->
```cpp
static_assert(::scl::signature::is_const_v<print_t>);
static_assert(!::scl::signature::is_volatile_v<print_t>);
static_assert(::scl::signature::is_lvalue_reference_v<print_t>);
static_assert(!::scl::signature::is_rvalue_reference_v<print_t>);
static_assert(::scl::signature::is_noexcept_v<print_t>);
```

В шаблон функции `on_port` можно передать функцию обратного вызова, параметр которой вмещает любое
16-битное беззнаковое значение. Можно передать также обобщённое лямбда-выражение: сигнатуры у него
нет, и для него проверяется только возможность вызова с аргументом типа `std::uint16_t`. Функция
обратного вызова с параметром типа `std::uint8_t` отвергается:

<!-- snippet: example/type_traits/signature/type_traits_signature_example.cpp constrain -->
```cpp
template <typename Callback>
void on_port(Callback && callback)
    requires(::std::is_invocable_v<Callback, ::std::uint16_t>) &&
    (!has_signature<Callback> ||
        (::std::is_unsigned_v<::std::remove_cvref_t<::scl::signature::parameter_t<Callback, 0>>> &&
            sizeof(::scl::signature::parameter_t<Callback, 0>) >= sizeof(::std::uint16_t)))
{
    ::std::forward<Callback>(callback)(::std::uint16_t{8080});
}

template <typename Callback>
concept accepted_by_on_port = requires(Callback callback) { on_port(callback); };

auto const narrow = [](::std::uint8_t port) { return port; };
static_assert(!accepted_by_on_port<decltype(narrow)>);
```

## Смотрите также

- [Главная страница](../Main.md)
- [`scl::member_like_t`](member_like.md)
- [`scl::overload_cast`](overload_cast.md)
- [`example/type_traits/signature/type_traits_signature_example.cpp`](../../../../example/type_traits/signature/type_traits_signature_example.cpp)
