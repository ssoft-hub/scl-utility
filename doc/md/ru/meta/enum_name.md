# `enum_name<V>()`

Шаблон функции `enum_name<V>()` возвращает на этапе компиляции имя константы перечисления, которая
имеет значение `V`, вместе с пространствами имён и классами, а для перечисления с областью видимости
и с типом. Шаблон функции `enum_short_name<V>()` возвращает только имя константы.

- Заголовок: `#include <scl/utility/meta/enum.h>`
- Оба шаблона функций работают без RTTI, и их можно вычислять в константном выражении.

```cpp
namespace scl
{
    template <::scl::concepts::enum_type auto V>
    [[nodiscard]] constexpr ::std::string_view enum_name() noexcept;

    template <::scl::concepts::enum_type auto V>
    [[nodiscard]] constexpr ::std::string_view enum_short_name() noexcept;
}
```

## Семантика

- **Ограничение:** концепту `scl::concepts::enum_type` удовлетворяют перечисления с областью
  видимости и без неё, поэтому `V` может быть значением любого из них.
- **Полное имя:** константа перечисления с областью видимости записывается
  вместе с типом, константа перечисления без области видимости записывается без него, и обе
  записываются вместе с пространствами имён и классами, в которых объявлено перечисление,
  например `app::Color::Red` и `app::Active`.
- **Значение без константы:** для значения, которого нет ни у одной константы его типа, например
  `Color{42}`, оба шаблона функций возвращают пустой `std::string_view`.
- **Написание:** пространства имён записываются так, как их записывает компилятор. Для константы
  перечисления, объявленного в безымянном пространстве имён, Clang пишет
  `(anonymous namespace)::Hidden::Shown`, GCC пишет `<unnamed>::Hidden::Shown`, а MSVC пишет
  `` `anonymous-namespace'::Hidden::Shown ``.
- **Для чего предназначены результаты:** результаты обоих шаблонов функций предназначены для показа.
  Написание результата `enum_name<V>()` может различаться у разных компиляторов, например для типа
  из безымянного пространства имён, а написание обоих может измениться в более поздней версии
  модуля, поэтому не следует сохранять результат, разбирать его или сравнивать с литералом. Примеры
  ниже сравнивают результаты с литералами, чтобы показать написание этой версии.

## Примеры

Перечисления примеров:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp types -->
```cpp
enum class Color
{
    Red,
    Green,
};

enum Status
{
    Active,
};
```

Полное имя:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp enum_name -->
```cpp
static_assert(scl::enum_name<Color::Red>() == "Color::Red");
static_assert(scl::enum_name<Active>() == "Active");
```

Короткое имя:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp enum_short_name -->
```cpp
static_assert(scl::enum_short_name<Color::Green>() == "Green");
static_assert(scl::enum_short_name<Active>() == "Active");
```

Значение, которого нет ни у одной константы:

<!-- snippet: example/meta/enum_name/meta_enum_name_example.cpp no_constant -->
```cpp
// No constant of Color has the value 42.
static_assert(scl::enum_name<Color{42}>().empty());
static_assert(scl::enum_short_name<Color{42}>().empty());
```

## Смотрите также

- [`example/meta/enum_name/meta_enum_name_example.cpp`](../../../../example/meta/enum_name/meta_enum_name_example.cpp)

---

Назад: [Meta](index.md) | Далее: [`symbol_name<S>()`](symbol_name.md) | [К группе](index.md) |
[К главной странице](../Main.md)
