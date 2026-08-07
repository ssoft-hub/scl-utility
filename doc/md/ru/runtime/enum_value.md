# Строковое представление значения перечисления

Возвращает строку вида `"TypeName::N"`, где `N` — числовое значение базового типа перечисления,
включая значения вне диапазона именованных константов.

- Заголовок: `#include <scl/utility/runtime/enum.h>`
- RTTI **не требуется**.

---

## `enum_value(value)`

- Заголовок: `#include <scl/utility/runtime/enum.h>`
- Объявление: `template <typename E> std::string enum_value(E value) requires std::is_enum_v<E>;`

### Семантика

- **Имя типа** извлекается на этапе компиляции через `scl::type_short_name<E>()` — RTTI не нужен.
- **Числовое значение** приводится к `std::underlying_type_t<E>` перед форматированием:
  знак и величина всегда соответствуют объявлению (например, `unsigned char` отображается
  как `0`–`255`, `int` — со знаком `-` для отрицательных значений).
- **Значения вне диапазона**, не имеющие именованного константа, представляются своим числом.

### Примеры

```cpp
#include <scl/utility/runtime/enum.h>

enum class Color : int  { Red = 1, Green = 2, Blue = -3 };
enum class Flags : unsigned { None = 0, A = 1, B = 2 };

namespace app { enum class Status : int { Ok = 0, Err = 42 }; }

scl::enum_value(Color::Red);       // "Color::1"
scl::enum_value(Color::Blue);      // "Color::-3"
scl::enum_value(Color{42});        // "Color::42"  — безымянное значение
scl::enum_value(Flags::B);         // "Flags::2"
scl::enum_value(app::Status::Err); // "Status::42" — пространство имён отброшено
```

### Сравнение с аналогом времени компиляции

| | `scl::enum_name<V>()` | `scl::enum_value(v)` |
|---|---|---|
| Вычисление | Время компиляции (`constexpr`) | Время выполнения |
| Возвращаемый тип | `std::string_view` (без аллокации) | `std::string` |
| Результат | Идентификатор константы (`"Color::Red"`) | Числовое значение (`"Color::1"`) |
| Значения вне диапазона | Неприменимо — `V` должен быть именованным | Поддерживаются |
| RTTI | Не требуется | Не требуется |
