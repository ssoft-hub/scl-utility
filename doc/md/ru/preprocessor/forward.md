# `SCL_FORWARD`

Макрос `SCL_FORWARD(...)` раскрывается в свои аргументы без изменений, поэтому разделённый
запятыми список проходит через один параметр другого макроса.

- Заголовок: `#include <scl/utility/preprocessor/forward.h>`

```cpp
#define SCL_FORWARD(...) __VA_ARGS__
```

## Семантика

- **Раскрытие:** `SCL_FORWARD(a, b)` раскрывается в `a, b` и не добавляет скобок вокруг списка.
- **Аргумент другого макроса:** препроцессор делит аргументы макроса по каждой запятой
  вне скобок, поэтому `SCL_FORWARD(a, b)`, записанный аргументом другого макроса, остаётся одним
  аргументом, и параметр получает `a, b` после раскрытия этого аргумента.
- **`#` и `##`:** параметр, к которому другой макрос применяет `#` или `##`, получает аргумент
  нераскрытым, в виде `SCL_FORWARD(a, b)`.

## Примеры

Список аргументов через один параметр:

<!-- snippet: example/preprocessor/forward/preprocessor_forward_example.cpp apply -->
```cpp
#define APPLY(function, arguments) function(arguments)

constexpr int add(int left, int right) noexcept { return left + right; }

static_assert(APPLY(add, SCL_FORWARD(1, 2)) == 3);
```

Тип с запятой в имени через один параметр:

<!-- snippet: example/preprocessor/forward/preprocessor_forward_example.cpp type_argument -->
```cpp
#define DECLARE_ALIAS(name, type) using name = type;

DECLARE_ALIAS(phone_book, SCL_FORWARD(std::map<std::string, int>))
```

## Смотрите также

- [`example/preprocessor/forward/preprocessor_forward_example.cpp`](../../../../example/preprocessor/forward/preprocessor_forward_example.cpp)

---

Назад: [`SCL_COUNTER_VALUE`](counter.md) | Далее: [`SCL_HAS_EXCEPTIONS`](exceptions.md) |
[К группе](index.md) | [К главной странице](../Main.md)
