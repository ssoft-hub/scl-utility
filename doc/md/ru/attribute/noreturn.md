# `SCL_NORETURN`

Функция, помеченная `SCL_NORETURN`, не возвращает управление вызывающему коду.

- Заголовок: `#include <scl/utility/attribute/noreturn.h>`

```cpp
#define SCL_NORETURN /* see below */
```

## Семантика

- **Что сообщается компилятору:** каждый вызов функции завершает программу, выбрасывает исключение,
  передаёт управление в другое место или не завершается никогда. Компилятор считает код после вызова
  недостижимым и перестаёт предупреждать, что функция, которая возвращает значение, может
  завершиться без него.
- **Ложное обещание:** поведение программы не определено, если функция возвращает управление,
  а раскрытие выбрано по любой строке таблицы, кроме последней.
- **Рядом с другими макросами:** `SCL_NORETURN` должен быть записан раньше `SCL_NOINLINE`
  и `SCL_FORCE_INLINE`. `[[noreturn]]` после `__declspec(noinline)` или `__forceinline` является
  на MSVC ошибкой C3829, а `[[noreturn]]` после `inline` из раскрытия `SCL_FORCE_INLINE` является
  ошибкой и на GCC и Clang.
- **Место:** макрос записывается перед объявлением функции.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| `__has_cpp_attribute(noreturn)` (C++11) | `[[noreturn]]` |
| MSVC, кроме clang-cl | `__declspec(noreturn)` |
| `__has_attribute(__noreturn__)` | `__attribute__((__noreturn__))` |
| ничего из перечисленного | пусто |

Макрос раскрывается в `[[noreturn]]` на GCC, Clang и MSVC в C++20.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_NORETURN` | `[[noreturn]]`, `__attribute__((__noreturn__))`, пусто | `[[noreturn]]`, `__attribute__((__noreturn__))`, пусто | `[[noreturn]]`, `__declspec(noreturn)`, пусто |

## Примеры

Функция, которая завершает программу, и функция, которая её вызывает:

<!-- snippet: example/attribute/noreturn/attribute_noreturn_example.cpp noreturn -->
```cpp
SCL_NORETURN void fail(char const * message)
{
    std::cerr << "fatal: " << message << '\n';
    std::exit(EXIT_FAILURE);
}

int divide(int dividend, int divisor)
{
    if (divisor == 0)
        fail("division by zero");
    return dividend / divisor;
}
```

## Смотрите также

- [`example/attribute/noreturn/attribute_noreturn_example.cpp`](../../../../example/attribute/noreturn/attribute_noreturn_example.cpp)

---

Назад: [`SCL_NODISCARD`](nodiscard.md) | Далее: [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) |
[К группе](index.md) | [К главной странице](../Main.md)
