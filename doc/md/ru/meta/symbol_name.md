# `symbol_name<S>()`

Шаблон функции `symbol_name<S>()` возвращает на этапе компиляции имя функции, переменной или члена
класса, на которые указывает `S`, вместе с пространствами имён и классами. Шаблон функции
`symbol_short_name<S>()` возвращает только это имя.

- Заголовок: `#include <scl/utility/meta/symbol.h>`
- Оба шаблона функций работают без RTTI, и их можно вычислять в константном выражении.

```cpp
namespace scl
{
    template <auto S>
    [[nodiscard]] constexpr ::std::string_view symbol_name() noexcept;

    template <auto S>
    [[nodiscard]] constexpr ::std::string_view symbol_short_name() noexcept;
}
```

## Семантика

- **Аргумент:** `S` является указателем на функцию, на переменную, на функцию-член или на член
  данных. Имя функции в аргументе шаблона само преобразуется в указатель на неё, поэтому
  `symbol_name<start>()` и `symbol_name<&start>()` являются одной специализацией шаблона
  и возвращают `start`.
- **Полное имя:** имя записывается вместе с пространствами имён и классами, например
  `app::Clock::reset` для `&app::Clock::reset`, без оператора взятия адреса, скобок и сигнатуры,
  которые компилятор пишет вокруг имени. GCC пишет `__conv_op` вместо имени оператора
  преобразования, например `app::Clock::__conv_op`, а MSVC пишет вместо имени виртуальной
  функции-члена имя созданной компилятором промежуточной функции, например
  ``app::Clock::`vcall'{0}'``.
- **Короткое имя:** записывается только само имя, например `reset` для `&app::Clock::reset`.
- **Для чего предназначены результаты:** результаты обоих шаблонов функций предназначены для показа.
  Их написание различается у разных компиляторов и может измениться в более поздней версии модуля,
  поэтому не следует сохранять результат, разбирать его или сравнивать с литералом. Примеры ниже
  сравнивают результаты с литералами, чтобы показать написание этой версии.

## Примеры

Функция и класс примеров:

<!-- snippet: example/meta/symbol_name/meta_symbol_name_example.cpp types -->
```cpp
void start() {}

namespace app
{
    struct Clock
    {
        int ticks = 0;

        void reset() {}
    };
} // namespace app
```

Полное имя:

<!-- snippet: example/meta/symbol_name/meta_symbol_name_example.cpp symbol_name -->
```cpp
static_assert(scl::symbol_name<start>() == "start");
static_assert(scl::symbol_name<&app::Clock::reset>() == "app::Clock::reset");
static_assert(scl::symbol_name<&app::Clock::ticks>() == "app::Clock::ticks");
```

Короткое имя:

<!-- snippet: example/meta/symbol_name/meta_symbol_name_example.cpp symbol_short_name -->
```cpp
static_assert(scl::symbol_short_name<&app::Clock::reset>() == "reset");
static_assert(scl::symbol_short_name<&app::Clock::ticks>() == "ticks");
```

## Смотрите также

- [`example/meta/symbol_name/meta_symbol_name_example.cpp`](../../../../example/meta/symbol_name/meta_symbol_name_example.cpp)

---

Назад: [`enum_name<V>()`](enum_name.md) | Далее: [`type_name<T>()`](type_name.md) |
[К группе](index.md) | [К главной странице](../Main.md)
