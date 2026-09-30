# `enum_string(value)`

Шаблон функции `enum_string` возвращает значение перечисляемого типа, записанное в виде `Type::N`,
где на месте части `Type` стоит имя перечисляемого типа, а на месте части `N` число, которое хранит
значение. RTTI для него не требуется, поэтому шаблон функции объявлен и в сборке с отключённым RTTI.
Перегрузка принимает вторым аргументом функциональный объект и ставит на место `N` без изменений
текст, который этот объект возвращает для числа.

- Заголовок: `#include <scl/utility/runtime/enum.h>`

```cpp
template <::scl::concepts::enum_type E>
[[nodiscard]] ::std::string enum_string(E value);

namespace scl::concepts
{
    template <typename Format, typename Enum>
    concept enum_string_format = /* см. раздел "Функциональный объект" ниже */;
}

template <::scl::concepts::enum_type E, typename Format>
[[nodiscard]] ::std::string enum_string(E value, Format && format)
    requires ::scl::concepts::enum_string_format<Format, E>;
```

## Семантика

- **Тип:** на месте части `Type` стоит короткое имя, которое шаблон функции
  [`scl::type_short_name<E>()`](../meta/type_name.md#type_short_name) возвращает на этапе
  компиляции, поэтому перечисляемый тип, объявленный в пространстве имён или в классе, записывается
  без имени пространства имён или класса. Для безымянного перечисляемого типа на месте части `Type`
  стоит имя, которое создаёт компилятор; оно различается у разных компиляторов и может измениться
  в более поздней версии модуля.
- **Число:** на месте части `N` стоит значение в базовом типе, в десятичной записи и со знаком этого
  типа. Для символьного базового типа и для базового типа `bool` на месте `N` тоже стоит число:
  значение `'P'` записывается как `80`.
- **Любое значение:** значение, которому не соответствует ни одна константа, записывается
  так же, как значение константы.

## Функциональный объект

Перегрузка `enum_string(value, format)` вызывает функциональный объект `format` в том виде, в каком
он передан, поэтому подходит и лямбда-выражение со спецификатором `mutable`. Перегрузка передаёт
число константным lvalue целого типа того же размера и той же знаковости, что и базовый тип,
а для базового типа `bool` константным lvalue типа `unsigned char`.

Функциональный объект должен возвращать значение любого типа, неявно преобразуемого
в `std::string_view`: объект типа `std::string` или `std::string_view`, ссылку на такой объект
или указатель на строку, завершённую нулём.

Если у функционального объекта есть единственная сигнатура, тип его первого параметра определяется
с помощью шаблонного типа `scl::signature::parameter_t`. Этот тип без ссылки и cv-квалификаторов
должен быть целым типом той же знаковости, что и базовый тип, с шириной не меньше ширины базового
типа. Тип `bool` и символьные типы считаются целыми. Ширина равна числу битов значения, поэтому
ширина типа `bool` равна одному биту.

| Базовый тип | Тип параметра | Вызов |
|---|---|---|
| `char16_t` | `char16_t`, `char32_t`, `std::uint16_t`, `std::uint32_t` | компилируется |
| `char16_t` | `std::uint8_t`, меньшей ширины | не компилируется |
| `char16_t` | `std::int32_t`, другой знаковости | не компилируется |
| `bool` | `bool`, `unsigned char` | компилируется |
| `std::uint8_t` | `bool`, шириной в один бит | не компилируется |
| `char16_t` | `std::uint32_t &`, ссылка на неконстантный тип | не компилируется, потому что число передаётся константным lvalue |

Нет единственной сигнатуры у обобщённого лямбда-выражения, у объекта класса с перегруженным
оператором вызова и у объектов, которые возвращают шаблоны функций `std::ref` и `std::bind_front`.
Для такого функционального объекта, а также для функции со списком параметров `(...)`, у которой нет
первого параметра, проверяются только вызов и тип его результата.

Все эти условия собраны в концепте `scl::concepts::enum_string_format<Format, E>`, поэтому
функциональный объект можно проверить до передачи в шаблон функции `scl::enum_string`.

## Примеры

Перечисляемые типы примеров:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp types -->
```cpp
enum class Color : int
{
    Red = 1,
    Blue = -3,
};

enum class Grade : char
{
    Pass = 'P',
};

namespace net
{
    enum class Status : unsigned
    {
        Err = 42,
    };
} // namespace net
```

Значение, которому соответствует константа, рядом с именем этой константы, полученным на этапе
компиляции:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp named -->
```cpp
    constexpr auto name = scl::enum_name<Color::Red>(); // Color::Red
    auto const red = scl::enum_string(Color::Red);      // Color::1
    auto const blue = scl::enum_string(Color::Blue);    // Color::-3
```

Значение, которому не соответствует ни одна константа:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp unnamed -->
```cpp
    auto const unnamed = scl::enum_string(Color{42}); // Color::42
```

Символьный базовый тип и перечисляемый тип, объявленный в пространстве имён:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp underlying -->
```cpp
    auto const grade = scl::enum_string(Grade::Pass);       // Grade::80
    auto const status = scl::enum_string(net::Status::Err); // Status::42
```

Функциональный объект, который возвращает число, записанное в двоичном виде во всю ширину типа
числа:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp formatted -->
```cpp
    auto const binary = [](auto number) {
        return std::bitset<sizeof(number) * CHAR_BIT>(number).to_string();
    };
    auto const grade = scl::enum_string(Grade::Pass, binary); // Grade::01010000
```

Проверка двух функциональных объектов с помощью концепта до их передачи:

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp concept -->
```cpp
constexpr auto wide = [](unsigned long long number) { return std::to_string(number); };
constexpr auto narrow = [](unsigned char number) { return std::to_string(number); };

static_assert(scl::concepts::enum_string_format<decltype(wide), net::Status>);
static_assert(!scl::concepts::enum_string_format<decltype(narrow), net::Status>);
```

## Сравнение с именем, получаемым на этапе компиляции

| | шаблон функции `scl::enum_name<V>()` | шаблон функции `scl::enum_string(value)` |
|---|---|---|
| Вычисляется | на этапе компиляции | во время выполнения |
| Возвращает | `std::string_view` | `std::string` |
| Результат содержит | константу, `Color::Red` | число, `Color::1` |
| Принимает | константу, названную на этапе компиляции | любое значение |

## Смотрите также

- [`scl::enum_name<V>()`](../meta/enum_name.md), имя константы на этапе компиляции
- [`scl::signature`](../type_traits/signature.md)
- [`example/runtime/enum_string/runtime_enum_string_example.cpp`](../../../../example/runtime/enum_string/runtime_enum_string_example.cpp)

---

Назад: [`type_name(obj)`](type_name.md) | Далее: [Замеры группы Runtime](benchmark.md) |
[К группе](index.md)
