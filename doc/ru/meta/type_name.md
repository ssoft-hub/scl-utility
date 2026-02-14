# Утилиты для получения имени типа

Извлечение человекочитаемых имён типов на этапе компиляции без RTTI.

- Заголовок: `#include <scl/utility/meta/type.h>`

Содержание:
- [`type_name<T>`](#type_name-method)
- [`type_short_name<T>`](#type_short_name-method)

---

## type_name<T>

Возвращает полное квалифицированное имя шаблонного типа `T` на этапе компиляции в виде `std::string_view`.

Утилита использует специфичные для компилятора макросы для захвата декорированной сигнатуры функции и извлечения точного представления типа. Обеспечивает кроссплатформенную согласованность за счёт нормализации вывода различных компиляторов.

- Заголовок: `#include <scl/utility/meta/type.h>`
- Объявление: `template <typename T> constexpr std::string_view type_name() noexcept;`

### Семантика

- **Нормализация:**
  На MSVC функция автоматически удаляет префиксы `struct `, `class ` и `union ` у пользовательских типов, приводя вывод к формату GCC и Clang.
- **Квалифицированные имена:**
  Результат содержит все квалификаторы пространств имён и вложенных классов (например, `std::vector<int>`).
- **Вычисление на этапе компиляции:**
  Вся логика извлечения является `constexpr`, что позволяет использовать результат в статических утверждениях и других контекстах этапа компиляции.

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
// Примечание: точная строка для типов std может незначительно отличаться в зависимости от реализации STL
static_assert(type_name<std::string>().find("basic_string") != std::string_view::npos);

// Пользовательские типы (нормализованы на MSVC)
static_assert(type_name<MyStruct>() == "MyStruct");

// Шаблонные типы
static_assert(type_name<std::vector<MyStruct>>().find("vector<MyStruct>") != std::string_view::npos);
```

### Типичные сценарии использования

Генерация человекочитаемых логов, сообщений об ошибках в обобщённом коде или реализация легковесных систем рефлексии, где идентификаторы типов необходимо отображать или сравнивать в виде строк без накладных расходов `typeid(T).name()`.

---

## type_short_name<T>

Возвращает только терминальный идентификатор типа `T`, удаляя все квалификаторы пространств имён и классов.

- Заголовок: `#include <scl/utility/meta/type.h>`
- Объявление: `template <typename T> constexpr std::string_view type_short_name() noexcept;`

### Описание

Функция обрабатывает результат `type_name<T>()` и удаляет всё до последнего разделителя `::`. Идеально подходит для кратких логов или отображения в пользовательском интерфейсе.

### Пример

```cpp
#include <scl/utility/meta/type.h>

namespace app::core { struct Task {}; }

int main() {
    constexpr auto full  = scl::type_name<app::core::Task>();       // "app::core::Task"
    constexpr auto short_n = scl::type_short_name<app::core::Task>(); // "Task"
}
```
