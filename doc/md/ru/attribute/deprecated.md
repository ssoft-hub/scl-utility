# `SCL_DEPRECATED`, `SCL_DEPRECATED_MSG`

Компилятор предупреждает о каждом использовании сущности, помеченной `SCL_DEPRECATED`
или `SCL_DEPRECATED_MSG`.

- Заголовок: `#include <scl/utility/attribute/deprecated.h>`

```cpp
#define SCL_DEPRECATED /* see below */
#define SCL_DEPRECATED_MSG(msg) /* see below */
```

## Семантика

- **Что сообщается компилятору:** сущность сохранена только для старого кода, а в новом коде следует
  использовать другую. Компилятор предупреждает о каждом использовании сущности и не выдаёт
  предупреждения о самом объявлении.
- **Сообщение:** строковый литерал `msg` из `SCL_DEPRECATED_MSG` вставляется в предупреждение.
  `SCL_DEPRECATED_MSG` раскрывается в `SCL_DEPRECATED`, и сообщение в предупреждение не вставляется,
  если не совпала ни одна строка до последней или если `SCL_DEPRECATED` определён до заголовка.
- **Место:** любой из макросов записывается перед объявлением функции или переменной, а в объявлении
  типа после `struct`, `class`, `enum` или `enum class`.

| Условие, первое совпавшее | `SCL_DEPRECATED` | `SCL_DEPRECATED_MSG(msg)` |
|---|---|---|
| `__has_cpp_attribute(deprecated)` (C++14) | `[[deprecated]]` | `[[deprecated(msg)]]` |
| MSVC, кроме clang-cl | `__declspec(deprecated)` | `__declspec(deprecated(msg))` |
| `__has_attribute(__deprecated__)` | `__attribute__((__deprecated__))` | `__attribute__((__deprecated__(msg)))` |
| ничего из перечисленного | пусто | `SCL_DEPRECATED` |

Макросы раскрываются в `[[deprecated]]` и `[[deprecated(msg)]]` на GCC, Clang и MSVC в C++20.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_DEPRECATED` | `[[deprecated]]`, `__attribute__((__deprecated__))`, пусто | `[[deprecated]]`, `__attribute__((__deprecated__))`, пусто | `[[deprecated]]`, `__declspec(deprecated)`, пусто |
| `SCL_DEPRECATED_MSG(msg)` | `[[deprecated(msg)]]`, `__attribute__((__deprecated__(msg)))`, `SCL_DEPRECATED` | `[[deprecated(msg)]]`, `__attribute__((__deprecated__(msg)))`, `SCL_DEPRECATED` | `[[deprecated(msg)]]`, `__declspec(deprecated(msg))`, `SCL_DEPRECATED` |

## Примеры

Функция и тип, оставленные только для старого кода:

<!-- snippet: example/attribute/deprecated/attribute_deprecated_example.cpp deprecated -->
```cpp
struct endpoint
{
    char const * host;
    int port;
};

bool connect(endpoint const & target) { return target.port > 0; }

SCL_DEPRECATED_MSG("use connect(endpoint) instead")
bool connect(char const * host, int port);

struct SCL_DEPRECATED legacy_handle
{
    int descriptor;
};
```

## Смотрите также

- [`example/attribute/deprecated/attribute_deprecated_example.cpp`](../../../../example/attribute/deprecated/attribute_deprecated_example.cpp)

---

Назад: [`SCL_ASSUME`](assume.md) | Далее: [`SCL_FALLTHROUGH`](fallthrough.md) |
[К группе](index.md) | [К главной странице](../Main.md)
