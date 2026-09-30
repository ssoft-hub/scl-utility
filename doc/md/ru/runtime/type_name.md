# `type_name(obj)`

Шаблон функции `type_name(obj)` с помощью RTTI возвращает имя динамического типа для объекта
полиморфного класса и имя статического типа для любого другого объекта. Шаблон функции
`type_short_name(obj)` возвращает то же имя без квалификаторов пространств имён и классов,
без аргументов шаблона и без префикса, например `class` или `struct`.

- Заголовок: `#include <scl/utility/runtime/type.h>`
- Оба шаблона функций объявлены только там, где макрос
  [`SCL_HAS_RTTI`](../preprocessor/rtti.md) равен `1`.
- Там, где макрос [`SCL_HAS_THREADS`](../preprocessor/threads.md) равен `1`, оба шаблона функций
  можно вызывать из нескольких потоков одновременно.
- Код для остальных случаев приведён в разделе [Без RTTI](index.md#без-rtti) страницы группы.

```cpp
template <typename T>
[[nodiscard]] ::std::string type_name(T const & obj);

template <typename T>
[[nodiscard]] ::std::string type_short_name(T const & obj);
```

## Семантика

- **Динамический тип:** тип определяется с помощью оператора `typeid`. Поэтому для объекта,
  полученного через указатель или ссылку на полиморфный базовый класс, шаблон функции возвращает имя
  класса, экземпляром которого был создан объект, а для объекта, полученного через указатель
  или ссылку на базовый класс без виртуальных функций, возвращает имя этого базового класса.
- **Написание:** там, где есть заголовок `<cxxabi.h>`, как у GCC и у Clang на Linux, macOS и MinGW,
  имя восстанавливается с помощью функции `abi::__cxa_demangle`. В остальных случаях, как у MSVC
  и у Clang с библиотекой MSVC, результат `typeid().name()` возвращается без изменений, с префиксом,
  например `class` или `struct`, если он есть в этом имени. Если восстановить имя не удаётся, оно
  возвращается без изменений. Там, где имя восстанавливается, имя каждого типа восстанавливается
  один раз и хранится до завершения программы, поэтому оба шаблона функций можно вызывать
  и из деструктора, который выполняется после возврата из `main`.
- **Для чего предназначены результаты:** результаты обоих шаблонов функций предназначены для показа
  человеку, например в строке журнала или в сообщении об ошибке, а не для различения типов: у двух
  разных типов, например у классов с одним именем в безымянных пространствах имён двух единиц
  трансляции, имя может совпасть. Написание различается у разных компиляторов и стандартных
  библиотек и может измениться в более поздней версии модуля, поэтому результат не следует
  сравнивать с литералом, разбирать или сохранять. Тип, имя которого возвращают шаблоны функций,
  однозначно определяется значением `std::type_index(typeid(obj))`.
- **Короткое имя:** для типа `app::Task<int>` шаблон функции `type_short_name(obj)` возвращает
  `Task`. Для типа замыкания, безымянного класса или перечисления шаблон функции возвращает имя,
  которое создаёт компилятор, например `<lambda_1>` у MSVC.

## Примеры

Классы примеров:

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

Для объекта, полученного через указатель на базовый класс, на этапе компиляции получается имя
статического типа, а во время выполнения имя динамического типа:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp dynamic -->
```cpp
    std::unique_ptr<app::Base> const pointer = std::make_unique<app::Derived>();

    auto const static_name = scl::type_name<app::Base>(); // app::Base
    auto const dynamic_name = scl::type_name(*pointer);   // app::Derived, or struct app::Derived
```

Полное и короткое имя объекта специализации шаблона класса во время выполнения:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp short -->
```cpp
    std::unique_ptr<app::Base> const pointer = std::make_unique<app::Task<int>>();

    auto const full_name = scl::type_name(*pointer); // app::Task<int>, or struct app::Task<int>
    auto const short_name = scl::type_short_name(*pointer); // Task
```

## Сравнение с именем, получаемым на этапе компиляции

| | шаблон функции `scl::type_name<T>()` | шаблон функции `scl::type_name(obj)` |
|---|---|---|
| Вычисляется | на этапе компиляции | во время выполнения |
| Возвращает | `std::string_view` | `std::string` |
| Называет | тип, записанный в точке вызова | динамический тип полиморфного объекта |
| RTTI | не требуется | требуется |

## Смотрите также

- [`scl::type_name<T>()`](../meta/type_name.md), имя на этапе компиляции
- [`example/runtime/type_name/runtime_type_name_example.cpp`](../../../../example/runtime/type_name/runtime_type_name_example.cpp)

---

Назад: [Runtime](index.md) | Далее: [`enum_string(value)`](enum_string.md) |
[Английская документация](../../en/runtime/type_name.md)
