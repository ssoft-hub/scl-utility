# Строковое представление значения перечисления

Возвращает строку вида `"TypeName::N"`, где `N` — числовое значение базового типа перечисления,
включая значения вне диапазона именованных константов.

- Заголовок: `#include <scl/utility/runtime/enum.h>`
- RTTI **не требуется**.

---

## `enum_string(value)`

- Заголовок: `#include <scl/utility/runtime/enum.h>`
- Объявление: `template <typename E> std::string enum_string(E value) requires std::is_enum_v<E>;`

### Семантика

- **Имя типа** извлекается на этапе компиляции через `scl::type_short_name<E>()` — RTTI не нужен.
- **Числовое значение** перед форматированием приводится к самому широкому целому той же
  знаковости: знак и величина соответствуют объявлению, а символьный базовый тип и `bool`
  тоже дают число (например, `unsigned char` отображается как `0`–`255`, `char` `'P'` как `80`).
- **Значения вне диапазона**, не имеющие именованного константа, представляются своим числом.

### Примеры

```cpp
#include <scl/utility/runtime/enum.h>

enum class Color : int  { Red = 1, Green = 2, Blue = -3 };
enum class Flags : unsigned { None = 0, A = 1, B = 2 };

namespace app { enum class Status : int { Ok = 0, Err = 42 }; }

auto const red_name     = scl::enum_string(Color::Red);       // "Color::1"
auto const blue_name    = scl::enum_string(Color::Blue);      // "Color::-3"
auto const other_name   = scl::enum_string(Color{42});        // "Color::42"  - безымянное значение
auto const flag_name    = scl::enum_string(Flags::B);         // "Flags::2"
auto const status_name  = scl::enum_string(app::Status::Err); // "Status::42" - пространство имён отброшено
```

### Сравнение с аналогом времени компиляции

| | `scl::enum_name<V>()` | `scl::enum_string(v)` |
|---|---|---|
| Вычисление | Время компиляции (`constexpr`) | Время выполнения |
| Возвращаемый тип | `std::string_view` (без аллокации) | `std::string` |
| Результат | Идентификатор константы (`"Color::Red"`) | Числовое значение (`"Color::1"`) |
| Значения вне диапазона | Неприменимо — `V` должен быть именованным | Поддерживаются |
| RTTI | Не требуется | Не требуется |

## Смотрите также

- [`example/runtime/enum_string/runtime_enum_string_example.cpp`](../../../../example/runtime/enum_string/runtime_enum_string_example.cpp) —
  рабочая версия: именованная константа рядом с её аналогом времени компиляции,
  значение вне набора констант, беззнаковый базовый тип и перечисление в пространстве
  имён.
