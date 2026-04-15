# SCL_ASSUME / SCL_UNREACHABLE

Подсказки оптимизатору: предположение об истинности выражения и пометка недостижимого кода.

- Заголовок: `#include <scl/utility/attribute/assume.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_ASSUME(expr)

Сообщает оптимизатору, что `expr` всегда истинно в данной точке. Компилятор
может использовать это для устранения лишних ветвлений, исключения проверок на
null или уточнения границ циклов.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(assume)` (C++23) | `[[assume(expr)]]` |
| MSVC native (не Clang-cl) | `__assume(expr)` |
| `__has_builtin(__builtin_assume)` (Clang) | `__builtin_assume(expr)` |
| `__has_attribute(assume)` (GCC ≥ 13) | `__attribute__((assume(expr)))` |
| `__has_builtin(__builtin_unreachable)` (GCC < 13) | `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())` |
| Ни одно не выполнено | `((void)(expr))` |

> **Важно:** В первых четырёх ветках `expr` **не вычисляется** во время
> выполнения. В запасной форме для GCC < 13 — вычисляется. Никогда не
> передавайте выражения с побочными эффектами.

### Использование

```cpp
#include <scl/utility/attribute/assume.h>

void scale(float* data, int n) {
    SCL_ASSUME(data != nullptr);
    SCL_ASSUME(n > 0);
    SCL_ASSUME(n % 4 == 0);        // подсказка: количество итераций кратно 4
    for (int i = 0; i < n; ++i)
        data[i] *= 2.0f;
}
```

---

## SCL_UNREACHABLE()

Помечает текущую точку кода как недостижимую. Если выполнение доходит до неё
во время работы программы — поведение **неопределённо**. Применяется для:

- Подавления предупреждений «не все пути возврата возвращают значение».
- Устранения мёртвых ветвей в генерируемом коде.
- Обозначения того, что `switch` исчерпывающе покрывает все значения перечисления.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| MSVC native (не Clang-cl) | `__assume(false)` |
| `__has_builtin(__builtin_unreachable)` (GCC, Clang) | `__builtin_unreachable()` |
| Ни одно не выполнено | `((void)0)` |

### Использование

```cpp
#include <scl/utility/attribute/assume.h>

int priority(Severity s) {
    switch (s) {
    case Severity::Low:      return 1;
    case Severity::Medium:   return 2;
    case Severity::High:     return 3;
    case Severity::Critical: return 4;
    }
    SCL_UNREACHABLE();  // все значения перечисления покрыты выше
}
```

---

## Примечания

- Оба макроса работают на уровне инструкций — завершайте точкой с запятой.
- `SCL_ASSUME(false)` эквивалентен `SCL_UNREACHABLE()` на всех компиляторах.
  Для ясности кода предпочитайте `SCL_UNREACHABLE()`.
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_ASSUME(expr)` или `#define SCL_UNREACHABLE()`.
