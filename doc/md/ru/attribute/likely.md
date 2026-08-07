# SCL_LIKELY / SCL_UNLIKELY / SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR

Макросы подсказок предсказателю ветвлений.

- Заголовок: `#include <scl/utility/attribute/likely.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_LIKELY

Портабельная обёртка над атрибутом инструкции `[[likely]]` из C++20. Размещается
перед инструкцией, сообщая оптимизатору, что данная ветвь является основным
путём исполнения. По стилю соответствует остальным макросам `SCL_*` — без аргументов.

### Размещение

| Позиция | Синтаксис |
|---------|-----------|
| Ветвь `if` | `if (cond) SCL_LIKELY { ... }` |
| Метка `case` в `switch` | `SCL_LIKELY case N: ...` |
| Метка `default` в `switch` | `SCL_LIKELY default: ...` |

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(likely)` (C++20) | `[[likely]]` |
| Ни одно не выполнено | *(пусто — подсказка отсутствует)* |

### Использование

```cpp
#include <scl/utility/attribute/likely.h>

if (cache_hit) SCL_LIKELY {
    return cache_value;
}

switch (msg.type) {
SCL_LIKELY case MsgType::Data:
    process(msg);
    break;
SCL_UNLIKELY default:
    handle_unknown(msg);
    break;
}
```

---

## SCL_UNLIKELY

Аналогично `SCL_LIKELY`, но помечает ветвь как редкий путь исполнения.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(unlikely)` (C++20) | `[[unlikely]]` |
| Ни одно не выполнено | *(пусто — подсказка отсутствует)* |

### Использование

```cpp
if (error) SCL_UNLIKELY {
    log_and_abort(error);
}
```

---

## SCL_LIKELY_EXPR(expr)

Подсказка предсказателю ветвлений на уровне выражения через `__builtin_expect`.
Оборачивает булево или целочисленное выражение и возвращает его значение без
изменений. Используется внутри условий `if`, `while` или тернарного оператора,
где атрибут инструкции разместить невозможно.

На MSVC (использующем профильную оптимизацию) выражение проходит без изменений.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_builtin(__builtin_expect)` (GCC, Clang) | `__builtin_expect(!!(expr), 1)` |
| Ни одно не выполнено | `(!!(expr))` |

### Использование

```cpp
if (SCL_LIKELY_EXPR(ptr != nullptr)) {
    // быстрый путь — предсказатель смещён сюда
}

while (SCL_LIKELY_EXPR(queue.size() > 0)) {
    drain(queue);
}
```

---

## SCL_UNLIKELY_EXPR(expr)

Аналогично `SCL_LIKELY_EXPR`, но смещает предсказатель в сторону ложного исхода.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_builtin(__builtin_expect)` (GCC, Clang) | `__builtin_expect(!!(expr), 0)` |
| Ни одно не выполнено | `(!!(expr))` |

### Использование

```cpp
if (SCL_UNLIKELY_EXPR(error_code != 0)) {
    handle_error(error_code);
}
```

---

## Примечания

- `SCL_LIKELY` / `SCL_UNLIKELY` — **атрибуты инструкции**: аннотируют ветвь,
  а не выражение. Внутри условия не применяются.
- `SCL_LIKELY_EXPR` / `SCL_UNLIKELY_EXPR` — **обёртки выражений**: допустимы
  в любом месте, где ожидается целочисленное выражение.
- Оба варианта всегда сохраняют точное значение во время выполнения; влияют
  только на раскладку ветвей оптимизатором.
- Не используйте подсказки умозрительно — неверные подсказки снижают
  производительность.
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_LIKELY`, `#define SCL_UNLIKELY`, `#define SCL_LIKELY_EXPR(expr)`,
  `#define SCL_UNLIKELY_EXPR(expr)`.
