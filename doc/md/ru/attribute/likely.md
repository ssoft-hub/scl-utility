# SCL_LIKELY / SCL_UNLIKELY / SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR

Макросы подсказок предсказателю ветвлений.

- Заголовок: `#include <scl/utility/attribute/likely.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_LIKELY

Переносимая обёртка над атрибутом инструкции `[[likely]]` из C++20. Размещается
перед инструкцией и тем самым сообщает оптимизатору, что эта ветвь — основной
путь исполнения. По стилю соответствует остальным макросам `SCL_*` — без аргументов.

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
Оборачивает булево или целочисленное выражение и возвращает `0` или `1`. Используется внутри условий `if`, `while`
или тернарного оператора, где атрибут инструкции разместить невозможно.

На MSVC, который полагается на оптимизацию по профилю, обёртка подсказки не
даёт: остаётся только проверка выражения на истинность.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| MSVC, кроме clang-cl | `(!!(expr))` |
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
| MSVC, кроме clang-cl | `(!!(expr))` |
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
  а не выражение. Внутри условия их поставить нельзя.
- `SCL_LIKELY_EXPR` / `SCL_UNLIKELY_EXPR` — **обёртки выражений**: допустимы
  в любом месте, где ожидается целочисленное выражение.
- `SCL_LIKELY` и `SCL_UNLIKELY` ничего не вычисляют и значения не меняют;
  обёртки `_EXPR` возвращают `0` или `1`. Меняется лишь то, как оптимизатор
  раскладывает ветви.
- Не расставляйте подсказки наугад — неверные подсказки могут снижать
  производительность.
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_LIKELY`, `#define SCL_UNLIKELY`, `#define SCL_LIKELY_EXPR(expr)`,
  `#define SCL_UNLIKELY_EXPR(expr)`.
