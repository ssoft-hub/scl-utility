# `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`

Ветвь, помеченная `SCL_LIKELY`, выбирается обычно, а ветвь, помеченная `SCL_UNLIKELY`, выбирается
редко. Выражение в `SCL_LIKELY_EXPR` обычно истинно, а выражение в `SCL_UNLIKELY_EXPR` обычно ложно.

- Заголовок: `#include <scl/utility/attribute/likely.h>`

```cpp
#define SCL_LIKELY /* see below */
#define SCL_UNLIKELY /* see below */
#define SCL_LIKELY_EXPR(expr) /* see below */
#define SCL_UNLIKELY_EXPR(expr) /* see below */
```

## `SCL_LIKELY`, `SCL_UNLIKELY`

- **Что сообщается компилятору:** оператор после макроса находится на пути, который программа
  выбирает обычно (`SCL_LIKELY`) или редко (`SCL_UNLIKELY`). Оптимизатор может расположить код
  под этот путь.
- **Место:** любой из макросов записывается перед оператором ветви `if` или `else` или перед меткой
  `case` или `default`, но не внутри условия.

| Условие, первое совпавшее | `SCL_LIKELY` | `SCL_UNLIKELY` |
|---|---|---|
| `__has_cpp_attribute(likely)`, `__has_cpp_attribute(unlikely)` (C++20) | `[[likely]]` | `[[unlikely]]` |
| ничего из перечисленного | пусто | пусто |

Макросы раскрываются в `[[likely]]` и `[[unlikely]]` на GCC, Clang и MSVC в C++20.

## `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`

- **Что сообщается компилятору:** `expr` обычно истинно (`SCL_LIKELY_EXPR`) или обычно ложно
  (`SCL_UNLIKELY_EXPR`).
- **Аргумент:** `expr` является выражением, к которому применим оператор `!`.
- **Значение:** значением раскрытия является значение истинности `expr`, и на любом компиляторе оно
  имеет тип `bool`.
- **MSVC:** подсказка на MSVC не передаётся, и в раскрытии остаётся только проверка истинности.
- **Место:** любой из макросов записывается в любом выражении, в том числе в условии `if`, `while`
  или `?:`.

| Условие, первое совпавшее | `SCL_LIKELY_EXPR(expr)` | `SCL_UNLIKELY_EXPR(expr)` |
|---|---|---|
| MSVC, кроме clang-cl | `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
| `__has_builtin(__builtin_expect)` (GCC, Clang) | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))` |
| ничего из перечисленного | `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |

Макросы раскрываются в `(static_cast<bool>(!!(expr)))` на MSVC, а на GCC и Clang в раскрытия
с `__builtin_expect`.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_LIKELY` | `[[likely]]`, пусто | `[[likely]]`, пусто | `[[likely]]`, пусто |
| `SCL_UNLIKELY` | `[[unlikely]]`, пусто | `[[unlikely]]`, пусто | `[[unlikely]]`, пусто |
| `SCL_LIKELY_EXPR(expr)` | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 1)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |
| `SCL_UNLIKELY_EXPR(expr)` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(__builtin_expect(!!(expr), 0)))`, `(static_cast<bool>(!!(expr)))` | `(static_cast<bool>(!!(expr)))` |

## Примеры

Ветвь для цифры, обычная при разборе, и другая ветвь:

<!-- snippet: example/attribute/likely/attribute_likely_example.cpp likely -->
```cpp
int parse_digit(char symbol)
{
    if (symbol >= '0' && symbol <= '9')
        SCL_LIKELY { return symbol - '0'; }
    else
        SCL_UNLIKELY { return -1; }
}
```

Условие ошибки, редкое в цикле, и результат, который обычно истинен:

<!-- snippet: example/attribute/likely/attribute_likely_example.cpp likely_expr -->
```cpp
int count_errors(int const * codes, std::size_t size)
{
    int errors = 0;
    for (std::size_t index = 0; index < size; ++index)
        if (SCL_UNLIKELY_EXPR(codes[index] != 0))
            ++errors;
    return errors;
}

bool all_valid(int const * codes, std::size_t size)
{
    return SCL_LIKELY_EXPR(count_errors(codes, size) == 0);
}
```

## Смотрите также

- [`SCL_HOT`, `SCL_COLD`](hotcold.md), подсказки обо всей функции, а не об одной ветви
- [`example/attribute/likely/attribute_likely_example.cpp`](../../../../example/attribute/likely/attribute_likely_example.cpp)

---

Назад: [`SCL_LIFETIMEBOUND`](lifetimebound.md) | Далее: [`SCL_MAYBE_UNUSED`](maybe_unused.md) |
[К группе](index.md) | [К главной странице](../Main.md)
