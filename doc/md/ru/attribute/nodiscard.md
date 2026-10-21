# `SCL_NODISCARD`, `SCL_NODISCARD_MSG`

Компилятор предупреждает, если отброшен результат, помеченный `SCL_NODISCARD`
или `SCL_NODISCARD_MSG`.

- Заголовок: `#include <scl/utility/attribute/nodiscard.h>`

```cpp
#define SCL_NODISCARD /* see below */
#define SCL_NODISCARD_MSG(msg) /* see below */
```

## Семантика

- **Что сообщается компилятору:** отбросить результат функции значит ошибиться. Компилятор
  предупреждает там, где вызов функции записан отдельным оператором.
- **На типе:** компилятор предупреждает, если отброшен результат любой функции, которая возвращает
  этот тип по значению.
- **Сообщение:** строковый литерал `msg` из `SCL_NODISCARD_MSG` вставляется в предупреждение,
  если `__has_cpp_attribute(nodiscard)` равно `201907L` или больше. Сообщение в остальных случаях
  не вставляется, а `SCL_NODISCARD_MSG` раскрывается в `SCL_NODISCARD`, если `SCL_NODISCARD`
  определён до заголовка.
- **Место:** любой из макросов записывается перед объявлением функции, а в объявлении типа
  после `struct`, `class`, `enum` или `enum class`.

| Условие, первое совпавшее | `SCL_NODISCARD` |
|---|---|
| `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
| `__has_attribute(__warn_unused_result__)` | `__attribute__((__warn_unused_result__))` |
| ничего из перечисленного | пусто |

| Условие, первое совпавшее | `SCL_NODISCARD_MSG(msg)` |
|---|---|
| `__has_cpp_attribute(nodiscard)` не меньше `201907L` (C++20) | `[[nodiscard(msg)]]` |
| `__has_cpp_attribute(nodiscard)` (C++17) | `[[nodiscard]]` |
| `__has_attribute(__warn_unused_result__)` | `__attribute__((__warn_unused_result__))` |
| ничего из перечисленного | `SCL_NODISCARD` |

Макросы раскрываются в `[[nodiscard]]` и `[[nodiscard(msg)]]` на GCC, Clang и MSVC в C++20.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_NODISCARD` | `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, пусто | `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, пусто | `[[nodiscard]]`, пусто |
| `SCL_NODISCARD_MSG(msg)` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, `SCL_NODISCARD` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `__attribute__((__warn_unused_result__))`, `SCL_NODISCARD` | `[[nodiscard(msg)]]`, `[[nodiscard]]`, `SCL_NODISCARD` |

## Примеры

Функция, результат которой равен `false`, если значение не сохранено, и тип, значение которого
предназначено для чтения:

<!-- snippet: example/attribute/nodiscard/attribute_nodiscard_example.cpp nodiscard -->
```cpp
SCL_NODISCARD_MSG("the result tells whether the value was stored")
bool store(std::vector<int> & storage, int value)
{
    if (storage.size() == storage.capacity())
        return false;
    storage.push_back(value);
    return true;
}

struct SCL_NODISCARD status
{
    int code;
};

status open_channel(int id) { return status{id < 0 ? 1 : 0}; }
```

## Смотрите также

- [`example/attribute/nodiscard/attribute_nodiscard_example.cpp`](../../../../example/attribute/nodiscard/attribute_nodiscard_example.cpp)

---

Назад: [`SCL_MAYBE_UNUSED`](maybe_unused.md) | Далее: [`SCL_NORETURN`](noreturn.md) |
[К группе](index.md) | [К главной странице](../Main.md)
