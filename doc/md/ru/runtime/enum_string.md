# `enum_string(value)`

Функция возвращает значение перечисления в виде строки `Type::N`, где `N` обозначает число,
которое хранится в значении. RTTI не нужен.

- Заголовок: `#include <scl/utility/runtime/enum.h>`

```cpp
template <::scl::concepts::enum_type E>
[[nodiscard]] ::std::string enum_string(E value);
```

## Семантика

- **Тип:** на место `Type` подставляется `scl::type_short_name<E>()`, прочитанное на этапе
  компиляции, поэтому перечисление из пространства имён записывается без имени пространства.
- **Число:** на место `N` подставляется значение в базовом типе с учётом знака базового типа. Для
  символьного базового типа и для `bool` тоже записывается число: для `'P'` результат равен
  `80`.
- **Любое значение:** значение, которому не соответствует ни одна константа, записывается так
  же, как значение константы.

## Примеры

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

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp named -->
```cpp
    constexpr auto name = ::scl::enum_name<Color::Red>(); // Color::Red
    auto const red = ::scl::enum_string(Color::Red);      // Color::1
    auto const blue = ::scl::enum_string(Color::Blue);    // Color::-3
```

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp unnamed -->
```cpp
    auto const unnamed = ::scl::enum_string(Color{42}); // Color::42
```

<!-- snippet: example/runtime/enum_string/runtime_enum_string_example.cpp underlying -->
```cpp
    auto const grade = ::scl::enum_string(Grade::Pass);         // Grade::80
    auto const status = ::scl::enum_string(::net::Status::Err); // Status::42
```

## Сравнение с именем времени компиляции

| | `scl::enum_name<V>()` | `scl::enum_string(value)` |
|---|---|---|
| Вычисляется | на этапе компиляции | во время выполнения |
| Возвращает | `std::string_view` | `std::string` |
| В строке | константа, `Color::Red` | число, `Color::1` |
| Аргумент | константа, названная на этапе компиляции | любое значение |

## Смотрите также

- [Runtime](index.md)
- [`example/runtime/enum_string/runtime_enum_string_example.cpp`](../../../../example/runtime/enum_string/runtime_enum_string_example.cpp)
