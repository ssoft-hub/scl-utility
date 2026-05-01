# Утилиты для получения имени типа во время выполнения

Извлечение человекочитаемых имён типов через RTTI с поддержкой полиморфизма.

- Заголовок: `#include <scl/utility/runtime/type.h>`
- Требует: включённый RTTI (`/GR` на MSVC; не `-fno-rtti`)

Содержание:
- [`type_name(obj)`](#type_nameobj)
- [`type_short_name(obj)`](#type_short_nameobj)

---

## `type_name(obj)`

Возвращает деманглированное полностью квалифицированное имя **динамического** типа объекта `obj`
в виде `std::string`.

- Заголовок: `#include <scl/utility/runtime/type.h>`
- Объявление: `template <typename T> std::string type_name(T const& obj);`

### Семантика

- **Динамическая диспетчеризация:** Если тип `T` является полиморфным и `obj` доступен через
  ссылку на базовый класс, `typeid(obj)` возвращает фактический тип времени выполнения,
  а не статический тип в точке вызова.
- **Деманглирование:** На GCC и Clang мангленное имя из `typeid` деманглируется через
  `abi::__cxa_demangle`. На MSVC `typeid().name()` уже читабелен и возвращается без изменений.
- **Защита от отсутствия RTTI:** Обе функции отключаются при сборке без RTTI.
  Используйте макрос `SCL_DETAIL_HAS_RTTI` для условной компиляции при необходимости.

### Примеры

```cpp
#include <scl/utility/runtime/type.h>
#include <memory>

struct Base  { virtual ~Base() = default; };
struct Child : Base {};

// Фундаментальный тип
int i = 0;
scl::type_name(i);   // "int"

// Статический тип
Child c;
scl::type_name(c);   // "Child"

// Полиморфизм: возвращает динамический тип через ссылку на базовый класс
std::unique_ptr<Base> p = std::make_unique<Child>();
scl::type_name(*p);  // "Child"  (не "Base")
```

### Сравнение с аналогом времени компиляции

| | `scl::type_name<T>()` | `scl::type_name(obj)` |
|---|---|---|
| Вычисление | Время компиляции (`constexpr`) | Время выполнения |
| Возвращаемый тип | `std::string_view` (без аллокации) | `std::string` |
| Полиморфизм | Нет — `T` фиксирован в точке вызова | Да — возвращает динамический тип |
| RTTI | Не требуется | Требуется |

---

## `type_short_name(obj)`

Возвращает неквалифицированный идентификатор динамического типа объекта `obj`, удаляя все
квалификаторы пространств имён и шаблонные аргументы.

- Заголовок: `#include <scl/utility/runtime/type.h>`
- Объявление: `template <typename T> std::string type_short_name(T const& obj);`

### Пример

```cpp
#include <scl/utility/runtime/type.h>
#include <memory>

namespace app { template <typename T> struct Task : Base {}; }

std::unique_ptr<Base> p = std::make_unique<app::Task<int>>();
scl::type_name(*p);        // "app::Task<int>"
scl::type_short_name(*p);  // "Task"
```
