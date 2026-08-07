# SCL_NODISCARD / SCL_NODISCARD_MSG

Макросы предупреждения об игнорировании возвращаемого значения.

- Заголовок: `#include <scl/utility/attribute/nodiscard.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_NODISCARD

Предписывает компилятору выдавать предупреждение, если возвращаемое значение
декорированной функции (или объект декорированного типа) игнорируется
вызывающим. Применяется к объявлениям функций и объявлениям классов/
структур/перечислений.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(nodiscard)` ≥ 201907L (C++20) | `[[nodiscard]]` |
| `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
| `__has_attribute(warn_unused_result)` (GCC, Clang) | `__attribute__((warn_unused_result))` |
| Ни одно не выполнено | *(пусто — предупреждение не выдаётся)* |

### Использование

```cpp
#include <scl/utility/attribute/nodiscard.h>

SCL_NODISCARD bool try_lock();

SCL_NODISCARD struct ErrorCode { int value; };
```

---

## SCL_NODISCARD_MSG(msg)

Аналогично `SCL_NODISCARD`, но прикрепляет строковый литерал `msg` к
диагностике, объясняя последствия игнорирования значения. Доступно в форме
`[[nodiscard(msg)]]` начиная с C++20. На компиляторах, где форма с сообщением
недоступна, разворачивается в `SCL_NODISCARD` (предупреждение выдаётся без
сообщения).

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(nodiscard)` ≥ 201907L (C++20) | `[[nodiscard(msg)]]` |
| `__has_cpp_attribute(nodiscard)` (C++17) или запасной вариант | `SCL_NODISCARD` (сообщение опускается) |

### Использование

```cpp
#include <scl/utility/attribute/nodiscard.h>

SCL_NODISCARD_MSG("игнорирование кода ошибки приведёт к утечке дескриптора файла")
ErrorCode write(int fd, const void* buf, std::size_t n);

SCL_NODISCARD_MSG("вызовите release() во избежание утечки ресурса")
ResourceHandle acquire(std::size_t bytes);
```

---

## Примечания

- Применение `SCL_NODISCARD` к типу неявно помечает все функции, возвращающие
  этот тип, как nodiscard.
- Приведение к `void` подавляет предупреждение в конкретном месте вызова:
  `(void)try_lock();`
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_NODISCARD` или `#define SCL_NODISCARD_MSG(msg)`.
