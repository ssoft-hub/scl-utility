# Утилиты для получения имени перечисления

Интроспекция идентификаторов членов перечислений на этапе компиляции.

- Заголовок: `#include <scl/utility/meta/enum.h>`

Содержание:
- [`enum_name<V>`](#enum_name-v)
- [`enum_short_name<V>`](#enum_short_name-v)

---

## `enum_name<V>`

Возвращает квалифицированное строковое представление члена перечисления `V` на этапе компиляции.

- Заголовок: `#include <scl/utility/meta/enum.h>`
- Объявление: `template <auto V> requires std::is_enum_v<decltype(V)> constexpr std::string_view enum_name() noexcept;`

### Семантика

- **Кроссплатформенная нормализация:**
  MSVC часто включает префикс `enum ` в сигнатуры функций. Утилита автоматически обнаруживает
  и удаляет его для согласованности с GCC и Clang.
- **Квалифицированный результат:**
  В зависимости от компилятора и типа перечисления (scoped или unscoped) результат обычно
  включает имя типа (например, `Color::Red`).

### Примеры

```cpp
#include <scl/utility/meta/enum.h>

enum class Color { Red, Green, Blue };
enum Status { Active };

// Перечисления с областью видимости (scoped)
static_assert(scl::enum_name<Color::Red>() == "Color::Red");

// Перечисления без области видимости (unscoped)
static_assert(scl::enum_name<Active>() == "Active");
```

---

## `enum_short_name<V>`

Возвращает только идентификатор члена перечисления `V`, удаляя все квалификаторы типа и
пространства имён.

- Заголовок: `#include <scl/utility/meta/enum.h>`
- Объявление: `template <auto V> requires std::is_enum_v<decltype(V)> constexpr std::string_view enum_short_name() noexcept;`

### Пример

```cpp
#include <scl/utility/meta/enum.h>

namespace gfx { enum class Mode { Fast, Safe }; }

void log_mode() {
    constexpr auto name = scl::enum_short_name<gfx::Mode::Fast>();
    std::cout << "Current mode: " << name << std::endl; // Вывод: "Current mode: Fast"
}
```
