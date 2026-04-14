# SCL_DEPRECATED / SCL_DEPRECATED_MSG

Макросы аннотации устаревших сущностей.

- Заголовок: `#include <scl/utility/attribute/deprecated.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_DEPRECATED

Помечает любую именованную сущность (функцию, класс, переменную, typedef,
перечисление) как устаревшую. Компилятор выдаёт предупреждение при каждом
использовании, побуждая пользователей перейти на замену.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated]]` |
| MSVC native (не Clang-cl) | `__declspec(deprecated)` |
| `__has_attribute(deprecated)` (GCC, Clang) | `__attribute__((deprecated))` |
| Ни одно не выполнено | *(пусто — предупреждение не выдаётся)* |

### Использование

```cpp
#include <scl/utility/attribute/deprecated.h>

SCL_DEPRECATED void old_connect(const char* host, int port);

SCL_DEPRECATED struct LegacyHandle { int fd; };
```

---

## SCL_DEPRECATED_MSG(msg)

Аналогично `SCL_DEPRECATED`, но включает строковый литерал `msg` в текст
диагностического сообщения, объясняя причину устаревания и указывая замену.
На компиляторах, не поддерживающих форму с сообщением, макрос разворачивается
в `SCL_DEPRECATED` (предупреждение выдаётся без текста).

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated(msg)]]` |
| MSVC native (не Clang-cl) | `__declspec(deprecated(msg))` |
| `__has_attribute(deprecated)` (GCC, Clang) | `__attribute__((deprecated(msg)))` |
| Ни одно не выполнено | *(пусто)* |

### Использование

```cpp
#include <scl/utility/attribute/deprecated.h>

SCL_DEPRECATED_MSG("используйте connect(Endpoint) вместо этого")
void old_connect(const char* host, int port);

SCL_DEPRECATED_MSG("заменено на ResourceHandle в v2")
struct LegacyHandle { int fd; };
```

---

## Примечания

- Оба макроса не подавляют существующие предупреждения, а лишь добавляют
  новые в местах использования.
- Чтобы заглушить предупреждение о устаревании внутри собственной реализации
  (например, в обёртке, вызывающей устаревшую функцию), используйте прагму
  подавления диагностики в этом месте.
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_DEPRECATED` или `#define SCL_DEPRECATED_MSG(msg)`.
