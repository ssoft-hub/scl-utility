# SCL_FORCE_INLINE / SCL_NOINLINE

Макросы управления встраиванием функций.

- Заголовок: `#include <scl/utility/attribute/inline.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_FORCE_INLINE

Предлагает компилятору всегда встраивать аннотированную функцию вопреки его
собственным эвристикам встраивания. Размещается перед возвращаемым типом.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| MSVC, кроме clang-cl | `__forceinline` |
| `__has_cpp_attribute(gnu::always_inline)` (GCC, Clang) | `[[gnu::always_inline]] inline` |
| `__has_attribute(always_inline)` (более старые GCC и Clang) | `__attribute__((always_inline)) inline` |
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

Предлагает компилятору не встраивать аннотированную функцию. Полезно для:

- Холодных путей обработки ошибок, которые не должны раздувать горячий код.
- Функций, которые должны отображаться как именованные кадры в профилировщике
  или трассировках стека.
- Намеренного запрета встраивания при отладке.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| MSVC, кроме clang-cl | `__declspec(noinline)` |
| `__has_cpp_attribute(gnu::noinline)` (GCC, Clang) | `[[gnu::noinline]]` |
| `__has_attribute(noinline)` (более старые GCC и Clang) | `__attribute__((noinline))` |
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
- Рекурсивная функция с `SCL_FORCE_INLINE` не просто остаётся невстроенной:
  GCC отказывается её собирать на `-O0`, `-O1` и `-Og` (`inlining failed in call
  to 'always_inline'`), а начиная с `-O2` собирает. Clang принимает её на любом
  уровне.
- Каждый макрос можно переопределить до включения заголовка через
  `#define SCL_FORCE_INLINE` или `#define SCL_NOINLINE`.
