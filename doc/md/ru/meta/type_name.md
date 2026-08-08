# Утилиты для получения имени типа

Извлечение человекочитаемых имён типов на этапе компиляции без RTTI.

- Заголовок: `#include <scl/utility/meta/type.h>`

Содержание:
- [`type_name<T>`](#type_name-method)
- [`type_short_name<T>`](#type_short_name-method)

---

## type_name<T>

Возвращает полное квалифицированное имя шаблонного типа `T` на этапе компиляции в виде
`std::string_view`.

Утилита использует специфичные для компилятора макросы для захвата декорированной сигнатуры
функции и извлечения точного представления типа.

- Заголовок: `#include <scl/utility/meta/type.h>`
- Объявление: `template <typename T> constexpr std::string_view type_name() noexcept;`

### Семантика

- **Зависимость от компилятора:**
  На MSVC результат включает префиксы `struct `, `class `, `union ` и `enum ` для
  пользовательских типов (`"struct MyStruct"`, `"enum Color"`) - как на верхнем уровне, так
  и внутри аргументов шаблона. GCC и Clang эти ключевые слова не добавляют. Сверх того
  расходятся встроенные пространства имён и аргументы шаблона, заданные по умолчанию.
  `std::string`, измерено на GCC 13.1, Clang 22.1 и MSVC 19.44:

  ```text
  GCC    std::__cxx11::basic_string<char>
  Clang  std::basic_string<char>
  MSVC   class std::basic_string<char,struct std::char_traits<char>,class std::allocator<char> >
  ```
- **Для чего предназначен результат:**
  Для показа - строка журнала, сообщение об ошибке, имя для человека. Это не устойчивый
  идентификатор: совпадение хоть какой-то его части между компиляторами ничем не
  гарантировано, поэтому его не следует сравнивать с литералом, разбирать или сохранять.
  Для сравнения типов предназначен [`type_key`](type_key.md); если нужен только
  идентификатор типа, объявленного в области пространства имён, - `type_short_name<T>()`.
- **Квалифицированные имена:**
  Результат содержит все квалификаторы пространств имён и вложенных классов
  (например, `std::vector<int>`).
- **Вычисление на этапе компиляции:**
  Вся логика извлечения является `constexpr`, что позволяет использовать результат
  в статических утверждениях и других контекстах этапа компиляции.

### Примеры

```cpp
#include <scl/utility/meta/type.h>
#include <vector>
#include <string>

using ::scl::type_name;

struct MyStruct {};

// Фундаментальные типы
static_assert(type_name<int>() == "int");

// Типы стандартной библиотеки
// Примечание: точная строка может незначительно отличаться в зависимости от реализации STL
static_assert(type_name<std::string>().find("basic_string") != std::string_view::npos);

// Пользовательские типы
// GCC/Clang: "MyStruct"  |  MSVC: "struct MyStruct"
static_assert(type_name<MyStruct>().find("MyStruct") != std::string_view::npos);

// Шаблонные типы
// GCC/Clang: "std::vector<MyStruct>"
// MSVC:      "class std::vector<struct MyStruct,class std::allocator<struct MyStruct> >"
static_assert(type_name<std::vector<MyStruct>>().find("MyStruct") != std::string_view::npos);
```

### Типичные сценарии использования

Генерация человекочитаемых журналов и сообщений об ошибках в обобщённом коде, а также
обозначение типа в диагностике - без RTTI и без искажённого написания, которое даёт
`typeid(T).name()`. Если тип нужно распознать, а не показать, для этого предназначен
[`type_key`](type_key.md).

---

## type_short_name<T>

Возвращает только терминальный идентификатор типа `T`, удаляя все квалификаторы пространств
имён и классов.

- Заголовок: `#include <scl/utility/meta/type.h>`
- Объявление: `template <typename T> constexpr std::string_view type_short_name() noexcept;`

### Описание

Функция обрабатывает результат `type_name<T>()` и удаляет всё до последнего разделителя `::`.
Дополнительно срезаются префиксы `struct`/`class`/`union`/`enum` (присутствующие на MSVC)
и шаблонные аргументы, поэтому результатом всегда является только голый идентификатор.

### Пример

```cpp
#include <scl/utility/meta/type.h>

namespace app::core { struct Task {}; }

int main() {
    constexpr auto full    = scl::type_name<app::core::Task>();       // "app::core::Task" (GCC/Clang)
    constexpr auto short_n = scl::type_short_name<app::core::Task>(); // "Task"
}
```
