# `type_name<T>()`

Шаблон функции `type_name<T>()` возвращает на этапе компиляции имя типа `T` в том виде, в каком его
записывает компилятор. Шаблон функции `type_short_name<T>()` возвращает то же имя без квалификаторов
пространств имён и классов, без аргументов шаблона, без `const`, `volatile`, ссылки и префикса,
например `struct`.

- Заголовок: `#include <scl/utility/meta/type.h>`
- Оба шаблона функций работают без RTTI, и их можно вычислять в константном выражении.

```cpp
namespace scl
{
    template <typename T>
    [[nodiscard]] constexpr ::std::string_view type_name() noexcept;

    template <typename T>
    [[nodiscard]] constexpr ::std::string_view type_short_name() noexcept
        requires(see below class_enum_or_fundamental);
}
```

## Семантика

- **Написание:** имя берётся из сигнатуры, которую компилятор даёт шаблону функции, у MSVC
  из `__FUNCSIG__`, у GCC и Clang из `__PRETTY_FUNCTION__`, вместе с пространствами имён и классами
  типа и с его `const`, `volatile` и ссылкой. Перед именем класса, объединения или перечисления MSVC
  пишет ключ `class`, `struct`, `union` или `enum`, и на верхнем уровне, и в аргументах шаблона,
  а GCC и Clang не пишут этот ключ. GCC записывает тип стандартной библиотеки со всеми
  пространствами имён, в которых его объявляет библиотека, включая встроенные, например
  `std::__cxx11` у libstdc++, а Clang опускает встроенное пространство имён. MSVC пишет и аргументы
  по умолчанию шаблона такого типа, которые опускают GCC и Clang. Так записывается `std::string`
  в GCC 13.1, в Clang 22.1 с библиотекой MSVC и в MSVC 19.44:

  ```text
  GCC     std::__cxx11::basic_string<char>
  Clang   std::basic_string<char>
  MSVC    class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
  ```
- **Для чего предназначены результаты:** результаты обоих шаблонов функций предназначены для показа
  человеку, например в строке журнала или в сообщении об ошибке, а не для различения типов. Они
  различаются у разных компиляторов и стандартных библиотек, а их написание может измениться в более
  поздней версии модуля, поэтому не следует сохранять результат, разбирать его или сравнивать
  с литералом. Для сравнения типов служат ключи, которые возвращает
  [`scl::type_key_of<T>()`](type_key.md).
- **Короткое имя:** для типа `app::Task<int> const &` шаблон функции `type_short_name<T>()`
  возвращает `Task`. Для типа замыкания, безымянного класса или перечисления он возвращает имя,
  которое создаёт компилятор, например `<lambda()>` у GCC, а для фундаментального типа возвращает
  имя, которым его записывает компилятор, без пространства имён, например `int`,
  а для `std::nullptr_t` возвращает `nullptr_t` у GCC и Clang и `nullptr` у MSVC.
  Для `unsigned long long` GCC записывает `long long unsigned int`, а MSVC записывает
  `unsigned __int64`.
- **Ограничение:** в объявлении `type_short_name<T>()` ограничению `class_enum_or_fundamental`
  удовлетворяют класс, объединение, перечисление и фундаментальный тип, с `const`, `volatile`
  и ссылкой или без них. Ему не удовлетворяют указатель, массив, функция и указатель на член,
  и компилятор отвергает вызов с таким типом.
- **Два типа с одним именем:** GCC и Clang записывают указатель на `operator-` перед закрывающей
  угловой скобкой `>` как `operator->`, поэтому два разных типа могут получить одно имя. Пусть
  у класса `P` есть оба оператора, а у каждого из шаблонов классов `Holder` и `Wrap` есть вложенный
  шаблон класса `Inner`. Тогда Clang даёт типам
  `Wrap<Holder<&P::operator- >>::Inner<&P::operator-> >`
  и `Wrap<Holder<&P::operator-> >::Inner<&P::operator- >>` одно имя
  `Wrap<Holder<&P::operator->>::Inner<&P::operator->>`. Поэтому шаблон функции не может различить
  эти типы и возвращает `Wrap` для обоих, хотя первый из них является специализацией шаблона
  `Inner`.

## Примеры

Типы примеров:

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp types -->
```cpp
namespace app
{
    struct Item
    {};

    template <typename T>
    struct Task
    {};
} // namespace app
```

Полное имя, которое проверяется только в той части, которую все компиляторы записывают одинаково:

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp type_name -->
```cpp
// Every compiler spells int alike.
static_assert(scl::type_name<int>() == "int");

// MSVC writes the class key before the name of a class: app::Item, or struct app::Item.
static_assert(scl::type_name<app::Item>().ends_with("app::Item"));

// GCC spells a type of the standard library with an inline namespace of its library, such as
// std::__cxx11::basic_string<char>, and Clang leaves that namespace out, as in
// std::basic_string<char>.
static_assert(scl::type_name<std::string>().find("string") != std::string_view::npos);
```

Короткое имя:

<!-- snippet: example/meta/type_name/meta_type_name_example.cpp type_short_name -->
```cpp
static_assert(scl::type_short_name<app::Task<int>>() == "Task");
static_assert(scl::type_short_name<app::Item const &>() == "Item");
static_assert(scl::type_short_name<std::vector<app::Item>>() == "vector");
```

## Смотрите также

- [`scl::type_key_of<T>()`](type_key.md), ключ, который идентифицирует тип
- [`example/meta/type_name/meta_type_name_example.cpp`](../../../../example/meta/type_name/meta_type_name_example.cpp)

---

Назад: [`symbol_name<S>()`](symbol_name.md) | Далее: [`is_tu_local<T>`](is_tu_local.md) |
[К группе](index.md) | [К главной странице](../Main.md)
