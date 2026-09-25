# `type_name(obj)`

Функция возвращает имя типа, с которым был создан объект полиморфного класса, а для любого
другого объекта возвращает имя его статического типа. Имя читается через RTTI. Функция
`type_short_name(obj)` возвращает то же имя без квалификаторов.

- Заголовок: `#include <scl/utility/runtime/type.h>`
- Обе функции объявлены только там, где `SCL_HAS_RTTI` равен `1`; см.
  [Без RTTI](index.md#без-rtti)

```cpp
template <typename T>
[[nodiscard]] ::std::string type_name(T const & obj);

template <typename T>
[[nodiscard]] ::std::string type_short_name(T const & obj);
```

## Семантика

- **Динамический тип:** тип определяется через `typeid(obj)`, поэтому для объекта,
  полученного через указатель или ссылку на полиморфный базовый класс, возвращается имя
  класса, с которым объект был создан. Для объекта, полученного через указатель или ссылку на
  базовый класс без виртуальных функций, возвращается имя этого базового класса.
- **Написание:** там, где есть `<cxxabi.h>`, как у GCC и у Clang на Linux, macOS и MinGW,
  имя восстанавливается через `abi::__cxa_demangle`. В остальных случаях, как у MSVC и у
  Clang с библиотекой MSVC, результат `typeid().name()` возвращается как есть, с префиксом
  вроде `class` или `struct`, если он у типа есть.
- **Короткое имя:** `type_short_name(obj)` возвращает имя без квалификаторов пространств
  имён и классов и без аргументов шаблона: для `app::Task<int>` результат равен `Task`.

## Примеры

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp types -->
```cpp
namespace app
{
    struct Base
    {
        virtual ~Base() = default;
    };

    struct Derived : Base
    {};

    template <typename T>
    struct Task : Base
    {};
} // namespace app
```

При доступе через указатель или ссылку на базовый класс на этапе компиляции получается имя
статического типа, а во время выполнения получается имя динамического типа:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp dynamic -->
```cpp
    ::std::unique_ptr<::app::Base> const pointer = ::std::make_unique<::app::Derived>();

    auto const static_name = ::scl::type_name<::app::Base>(); // app::Base
    auto const dynamic_name = ::scl::type_name(*pointer); // app::Derived, or struct app::Derived
```

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp short -->
```cpp
    ::std::unique_ptr<::app::Base> const pointer = ::std::make_unique<::app::Task<int>>();

    auto const full_name = ::scl::type_name(*pointer); // app::Task<int>, or struct app::Task<int>
    auto const short_name = ::scl::type_short_name(*pointer); // Task
```

## Сравнение с именем времени компиляции

| | `scl::type_name<T>()` | `scl::type_name(obj)` |
|---|---|---|
| Вычисляется | на этапе компиляции | во время выполнения |
| Возвращает | `std::string_view` | `std::string` |
| Имя какого типа | записанного в точке вызова | динамического типа полиморфного объекта |
| RTTI | не нужен | нужен |

## Смотрите также

- [Runtime](index.md)
- [`example/runtime/type_name/runtime_type_name_example.cpp`](../../../../example/runtime/type_name/runtime_type_name_example.cpp)
