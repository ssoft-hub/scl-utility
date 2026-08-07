# SCL_FALLTHROUGH

Аннотация намеренного провала между ветками switch.

- Заголовок: `#include <scl/utility/attribute/fallthrough.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_FALLTHROUGH

Помечает конец тела `case`, который намеренно проваливается в следующий `case`
без `break`, `return` или `throw`. Подавляет предупреждение
`-Wimplicit-fallthrough` (GCC, Clang) и предупреждение C26819 (анализатор
MSVC).

Размещается как последняя инструкция в теле `case`, завершается точкой с
запятой.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(fallthrough)` (C++17) | `[[fallthrough]]` |
| `__has_attribute(fallthrough)` (GCC ≥ 7, Clang) | `__attribute__((fallthrough))` |
| Ни одно не выполнено | `((void)0)` — тихий холостой ход |

### Использование

```cpp
#include <scl/utility/attribute/fallthrough.h>

void handle(int code) {
    switch (code) {
    case 1:
        prepare();
        SCL_FALLTHROUGH;
    case 2:
        execute();
        break;
    case 3:
        log();
        SCL_FALLTHROUGH;
    case 4:
        cleanup();
        break;
    default:
        break;
    }
}
```

---

## Примечания

- `SCL_FALLTHROUGH` лишь подавляет предупреждение; на поведение программы
  во время выполнения не влияет.
- Запасное значение `((void)0)` является корректным выражением-инструкцией
  на всех целевых компиляторах и компилируется без побочных эффектов.
- Макрос можно переопределить до включения заголовка через
  `#define SCL_FALLTHROUGH`.
