# `SCL_FALLTHROUGH`

Выполнение намеренно переходит из ветви `switch`, помеченной `SCL_FALLTHROUGH`, в следующую ветвь.

- Заголовок: `#include <scl/utility/attribute/fallthrough.h>`

```cpp
#define SCL_FALLTHROUGH /* see below */
```

## Семантика

- **Что сообщается компилятору:** ветвь намеренно завершается без `break`. Компилятор не выдаёт
  для неё предупреждения `-Wimplicit-fallthrough`. MSVC не выдаёт предупреждения C5262, которое
  по умолчанию выключено, а при анализе кода с ключами `/analyze /analyze:plugin EspXEngine.dll`
  не выдаёт предупреждения C26819. Порядок выполнения одинаков с макросом и без него.
- **Место:** макрос записывается последним оператором ветви перед следующей меткой и завершается
  точкой с запятой.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| `__has_cpp_attribute(fallthrough)` (C++17) | `[[fallthrough]]` |
| `__has_attribute(__fallthrough__)` | `__attribute__((__fallthrough__))` |
| ничего из перечисленного | `((void)0)` |

Макрос раскрывается в `[[fallthrough]]` на GCC, Clang и MSVC в C++20.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_FALLTHROUGH` | `[[fallthrough]]`, `__attribute__((__fallthrough__))`, `((void)0)` | `[[fallthrough]]`, `__attribute__((__fallthrough__))`, `((void)0)` | `[[fallthrough]]`, `((void)0)` |

## Примеры

Все шаги от заданной стадии до последней:

<!-- snippet: example/attribute/fallthrough/attribute_fallthrough_example.cpp fallthrough -->
```cpp
int steps_left(int stage)
{
    int steps = 0;
    switch (stage)
    {
    case 0:
        ++steps;
        SCL_FALLTHROUGH;
    case 1:
        ++steps;
        SCL_FALLTHROUGH;
    case 2:
        ++steps;
        break;
    default:
        break;
    }
    return steps;
}
```

## Смотрите также

- [`example/attribute/fallthrough/attribute_fallthrough_example.cpp`](../../../../example/attribute/fallthrough/attribute_fallthrough_example.cpp)

---

Назад: [`SCL_DEPRECATED`](deprecated.md) | Далее: [`SCL_FORCE_INLINE`](inline.md) |
[К группе](index.md) | [К главной странице](../Main.md)
