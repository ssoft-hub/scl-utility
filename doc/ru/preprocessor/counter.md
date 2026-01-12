# Счётчик времени компиляции

Механизм счётчика на этапе компиляции, основанный на перегрузке функций и ADL.

- Заголовок: `#include <scl/utility/preprocessor/counter.h>`

Содержание:
- [`SCL_COUNTER_VALUE(Tag)`](#scl_counter_value-tag)
- [`SCL_COUNTER_NEXT(Tag)`](#scl_counter_next-tag)

---

## `SCL_COUNTER_VALUE(Tag)`

Считывает текущее значение счётчика времени компиляции, идентифицируемого тегом `Tag`.

- Заголовок: `#include <scl/utility/preprocessor/counter.h>`
- Объявление: `#define SCL_COUNTER_VALUE(Tag) /* ... */`

### Семантика

- **На основе тегов:** Каждый уникальный тип-тег определяет независимый счётчик. Разные теги не влияют друг на друга.
- **Шаблонные теги:** Специализации одного и того же шаблонного тега считаются отдельными счётчиками (например, `Counter<int>` и `Counter<float>` независимы).
- **32-битный диапазон:** Счётчик поддерживает значения от 0 до 2^32 - 1, используя 32 бита разрешения перегрузки.
- **Этап компиляции:** Результат — значение `constexpr` типа `std::uint_fast32_t`.
- **Начальное значение:** Счётчик начинается с 0 до каких-либо вызовов `SCL_COUNTER_NEXT` для данного тега.

### Примеры

```cpp
#include <scl/utility/preprocessor/counter.h>

struct MyTag {};

constexpr auto v0 = SCL_COUNTER_VALUE(MyTag);
static_assert(v0 == 0);

SCL_COUNTER_NEXT(MyTag)
constexpr auto v1 = SCL_COUNTER_VALUE(MyTag);
static_assert(v1 == 1);
```

---

## `SCL_COUNTER_NEXT(Tag)`

Инкрементирует счётчик времени компиляции, идентифицируемый тегом `Tag`.

- Заголовок: `#include <scl/utility/preprocessor/counter.h>`
- Объявление: `#define SCL_COUNTER_NEXT(Tag) /* ... */`

### Семантика

- **Побочный эффект:** Определяет новую перегрузку функции в `scl::preprocessor::detail`, которая заставляет последующие вызовы `SCL_COUNTER_VALUE(Tag)` возвращать увеличенное значение.
- **Монотонность:** Каждый вызов увеличивает счётчик ровно на 1.
- **Область действия — единица трансляции:** Состояние счётчика локально для единицы трансляции, в которой используются макросы.

### Примеры

```cpp
#include <scl/utility/preprocessor/counter.h>

struct TagA {};
struct TagB {};

// Два независимых счётчика
constexpr auto a0 = SCL_COUNTER_VALUE(TagA);  // 0
constexpr auto b0 = SCL_COUNTER_VALUE(TagB);  // 0

SCL_COUNTER_NEXT(TagA)
constexpr auto a1 = SCL_COUNTER_VALUE(TagA);  // 1
constexpr auto b1 = SCL_COUNTER_VALUE(TagB);  // по-прежнему 0

SCL_COUNTER_NEXT(TagA)
SCL_COUNTER_NEXT(TagB)
constexpr auto a2 = SCL_COUNTER_VALUE(TagA);  // 2
constexpr auto b2 = SCL_COUNTER_VALUE(TagB);  // 1
```

### Пример с шаблонным тегом

```cpp
#include <scl/utility/preprocessor/counter.h>

template <typename T>
struct TypeCounter {};

constexpr auto int_v0 = SCL_COUNTER_VALUE(TypeCounter<int>);
static_assert(int_v0 == 0);

SCL_COUNTER_NEXT(TypeCounter<int>)
constexpr auto int_v1 = SCL_COUNTER_VALUE(TypeCounter<int>);
static_assert(int_v1 == 1);

// Другая специализация — отдельный счётчик
constexpr auto float_v0 = SCL_COUNTER_VALUE(TypeCounter<float>);
static_assert(float_v0 == 0);
```

## Замечания

- Макрос `SCL_FORWARD` используется внутри для обработки шаблонных тегов, содержащих запятые.
- Данная техника опирается на метапрограммирование с состоянием через ADL. Хотя она широко поддерживается основными компиляторами (GCC, Clang, MSVC), она основана на недоопределённом поведении стандарта C++ ([CWG 2118](http://www.open-std.org/jtc1/sc22/wg21/docs/cwg_active.html#2118)).
