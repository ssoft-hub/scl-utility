# `SCL_HOT`, `SCL_COLD`

Компилятору сообщается, что функция, помеченная `SCL_HOT`, вызывается часто, а функция, помеченная
`SCL_COLD`, вызывается редко.

- Заголовок: `#include <scl/utility/attribute/hotcold.h>`

```cpp
#define SCL_HOT /* see below */
#define SCL_COLD /* see below */
```

## Семантика

- **Что сообщается компилятору:** функция вызывается часто (`SCL_HOT`) или редко (`SCL_COLD`).
- **Что делают GCC и Clang:** GCC и Clang отмечают функцию с `SCL_HOT` как часто вызываемую, а GCC
  к тому же оптимизирует её сильнее. Оба компилятора оптимизируют функцию с `SCL_COLD` по размеру
  и считают маловероятной ветвь, которая ведёт к её вызову. Оба компилятора размещают часто
  вызываемые функции в одной секции, а редко вызываемые в другой, отдельно от остального кода,
  если это позволяют цель и ключи компилятора. Вероятность ветви, которая ведёт к вызову часто
  вызываемой функции, остаётся прежней.
- **MSVC:** у MSVC нет атрибута для функции, которая вызывается часто или редко, и оба макроса
  раскрываются там в пустоту.
- **Только один из двух:** макросы `SCL_HOT` и `SCL_COLD` не должны быть записаны вместе на одной
  функции. Clang отвергает такую пару, а GCC с предупреждением отбрасывает один из двух атрибутов.
- **Место:** любой из макросов записывается перед объявлением функции.

| Условие, первое совпавшее | `SCL_HOT` | `SCL_COLD` |
|---|---|---|
| MSVC, кроме clang-cl | пусто | пусто |
| `__has_cpp_attribute(__gnu__::__hot__)`, `__has_cpp_attribute(__gnu__::__cold__)` (GCC, Clang) | `[[__gnu__::__hot__]]` | `[[__gnu__::__cold__]]` |
| `__has_attribute(__hot__)`, `__has_attribute(__cold__)` | `__attribute__((__hot__))` | `__attribute__((__cold__))` |
| ничего из перечисленного | пусто | пусто |

Макросы раскрываются в пустоту на MSVC, а в `[[__gnu__::__hot__]]` и `[[__gnu__::__cold__]]` на GCC
и Clang.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_HOT` | `[[__gnu__::__hot__]]`, `__attribute__((__hot__))`, пусто | `[[__gnu__::__hot__]]`, `__attribute__((__hot__))`, пусто | пусто |
| `SCL_COLD` | `[[__gnu__::__cold__]]`, `__attribute__((__cold__))`, пусто | `[[__gnu__::__cold__]]`, `__attribute__((__cold__))`, пусто | пусто |

## Примеры

Функция, которая выполняется для каждого блока данных, и функция, которая выполняется
только для повреждённого блока:

<!-- snippet: example/attribute/hotcold/attribute_hotcold_example.cpp hotcold -->
```cpp
SCL_HOT unsigned checksum(unsigned char const * bytes, std::size_t size)
{
    unsigned sum = 0;
    for (std::size_t index = 0; index < size; ++index)
        sum = sum * 31 + bytes[index];
    return sum;
}

SCL_COLD void report_corruption(std::size_t block)
{
    std::cerr << "block " << block << " is damaged\n";
}
```

## Смотрите также

- [`SCL_FORCE_INLINE`, `SCL_NOINLINE`](inline.md), подсказки о встраивании функции
- [`SCL_LIKELY`, `SCL_UNLIKELY`](likely.md), подсказки об одной ветви, а не обо всей функции
- [`example/attribute/hotcold/attribute_hotcold_example.cpp`](../../../../example/attribute/hotcold/attribute_hotcold_example.cpp)

---

Назад: [`SCL_FORCE_INLINE`](inline.md) | Далее: [`SCL_INDETERMINATE`](indeterminate.md) |
[К группе](index.md) | [К главной странице](../Main.md)
