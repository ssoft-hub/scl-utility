# `SCL_ASSUME`, `SCL_UNREACHABLE`

Оптимизатор считает условие `SCL_ASSUME` истинным в той точке, где записан макрос, а точку
`SCL_UNREACHABLE` считает недостижимой.

- Заголовок: `#include <scl/utility/attribute/assume.h>`

```cpp
#define SCL_ASSUME(expr) /* see below */
#define SCL_UNREACHABLE() /* see below */
```

## `SCL_ASSUME`

- **Что сообщается компилятору:** `expr` истинно каждый раз, когда выполнение доходит до оператора.
  Оптимизатор может убрать ветвь или проверку, которую условие делает лишней.
- **Ложное обещание:** поведение программы не определено, если в этой точке `expr` ложно,
  а раскрытие выбрано по одной из первых пяти строк таблицы.
- **Аргумент:** `expr` не вычисляется при первых четырёх строках таблицы и вычисляется при последних
  двух, поэтому следует передавать выражение без побочных эффектов.
- **`SCL_ASSUME(false)`:** обещание совпадает с обещанием `SCL_UNREACHABLE`, но Clang всё равно
  выдаёт предупреждение `-Wreturn-type`, если функция завершается после `[[assume(false)]]`.
  Для такого обещания следует писать `SCL_UNREACHABLE()`.
- **Место:** макрос записывается отдельным оператором, который завершается точкой с запятой.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| `__has_cpp_attribute(assume)`, если `__cplusplus` или `_MSVC_LANG` больше `202002L` | `[[assume(expr)]]` |
| MSVC, кроме clang-cl | `__assume(expr)` |
| `__has_builtin(__builtin_assume)` (Clang) | `__builtin_assume(expr)` |
| `__has_attribute(__assume__)` | `__attribute__((__assume__(expr)))` |
| `__has_builtin(__builtin_unreachable)` | `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())` |
| ничего из перечисленного | `((void)(expr))` |

Макрос раскрывается в `__assume(expr)` на MSVC, в `__builtin_assume(expr)` на Clang
и в `__attribute__((__assume__(expr)))` на GCC в C++20. Макрос раскрывается в `[[assume(expr)]]`,
только если `__cplusplus` или `_MSVC_LANG` больше `202002L` и `__has_cpp_attribute(assume)` не равно
`0`, как на GCC 13 и Clang 22 под `-std=c++23`. MSVC 19.44 сообщает `0` для `assume`
под `/std:c++latest`, поэтому там остаётся раскрытие `__assume(expr)`.

## `SCL_UNREACHABLE`

- **Что сообщается компилятору:** выполнение не доходит до оператора. Компилятор перестаёт
  предупреждать, что функция, которая возвращает значение, может завершиться без него.
- **Ложное обещание:** поведение программы не определено, если выполнение доходит до оператора,
  а раскрытие выбрано по одной из первых двух строк таблицы.
- **Место:** макрос записывается отдельным оператором, который завершается точкой с запятой.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| MSVC, кроме clang-cl | `__assume(false)` |
| `__has_builtin(__builtin_unreachable)` (GCC, Clang) | `__builtin_unreachable()` |
| ничего из перечисленного | `((void)0)` |

Макрос раскрывается в `__assume(false)` на MSVC и в `__builtin_unreachable()` на GCC и Clang.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_ASSUME(expr)` | `__attribute__((__assume__(expr)))`, `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())`, `((void)(expr))` | `__builtin_assume(expr)`, `__attribute__((__assume__(expr)))`, `(static_cast<bool>(expr) ? (void)0 : __builtin_unreachable())`, `((void)(expr))` | `__assume(expr)`, `((void)(expr))` |
| `SCL_UNREACHABLE()` | `__builtin_unreachable()`, `((void)0)` | `__builtin_unreachable()`, `((void)0)` | `__assume(false)`, `((void)0)` |

В C++23 на GCC и на Clang 19 и более поздних допускается также `[[assume(expr)]]`.

## Примеры

Количество, которое обещано кратным четырём:

<!-- snippet: example/attribute/assume/attribute_assume_example.cpp assume -->
```cpp
int sum_of_quads(int const * values, int count)
{
    SCL_ASSUME(count % 4 == 0);
    int sum = 0;
    for (int index = 0; index < count; ++index)
        sum += values[index];
    return sum;
}
```

Точка после `switch` с возвратом значения в каждой ветви:

<!-- snippet: example/attribute/assume/attribute_assume_example.cpp unreachable -->
```cpp
enum class severity
{
    low,
    high
};

int priority(severity level)
{
    switch (level)
    {
    case severity::low:
        return 1;
    case severity::high:
        return 2;
    }
    SCL_UNREACHABLE();
}
```

## Смотрите также

- [`example/attribute/assume/attribute_assume_example.cpp`](../../../../example/attribute/assume/attribute_assume_example.cpp)

---

Далее: [`SCL_DEPRECATED`](deprecated.md) | [К группе](index.md) | [К главной странице](../Main.md)
