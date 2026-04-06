# SCL_FORCE_INLINE / SCL_NOINLINE

Макросы управления встраиванием функций.

- Заголовок: `#include <scl/utility/attribute/inline.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_FORCE_INLINE

Требует от компилятора всегда встраивать декорированную функцию, игнорируя
собственные эвристики. Размещается перед возвращаемым типом.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| MSVC native (не Clang-cl) | `__forceinline` |
| `__has_cpp_attribute(gnu::always_inline)` (GCC, Clang) | `[[gnu::always_inline]] inline` |
| Ни одно не выполнено | `inline` |

> **Примечание:** Раскрытие для GCC/Clang уже содержит `inline`. Не пишите
> `inline` отдельно при использовании `SCL_FORCE_INLINE`.

### Использование

```cpp
#include <scl/utility/attribute/inline.h>

SCL_FORCE_INLINE int clamp(int v, int lo, int hi) {
    return v < lo ? lo : v > hi ? hi : v;
}
```

---

## SCL_NOINLINE

Запрещает компилятору встраивать декорированную функцию. Полезно для:

- Холодных путей обработки ошибок, которые не должны раздувать горячий код.
- Функций, которые должны отображаться как именованные кадры в профилировщике
  или трассировках стека.
- Намеренного запрета встраивания при отладке.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| MSVC native (не Clang-cl) | `__declspec(noinline)` |
| `__has_cpp_attribute(gnu::noinline)` (GCC, Clang) | `[[gnu::noinline]]` |
| Ни одно не выполнено | *(пусто — функция компилируется без подсказки)* |

### Использование

```cpp
#include <scl/utility/attribute/inline.h>

SCL_NOINLINE void report_error(int code, const char* msg);

SCL_NOINLINE void cold_path() {
    // дорогостоящая диагностика — выносим из горячего кода
}
```

---

## Примечания

- Оба макроса действуют на одно объявление или определение функции; рекурсивного
  эффекта нет.
- Компилятор может проигнорировать `SCL_FORCE_INLINE` в особых случаях
  (рекурсивные функции, взятие адреса функции).
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_FORCE_INLINE` или `#define SCL_NOINLINE`.
