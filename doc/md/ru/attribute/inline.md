# `SCL_FORCE_INLINE`, `SCL_NOINLINE`

У компилятора запрашивается встраивание функции, помеченной `SCL_FORCE_INLINE`, в каждом месте
вызова и отказ от встраивания функции, помеченной `SCL_NOINLINE`.

- Заголовок: `#include <scl/utility/attribute/inline.h>`

```cpp
#define SCL_FORCE_INLINE /* see below */
#define SCL_NOINLINE /* see below */
```

## `SCL_FORCE_INLINE`

- **Что сообщается компилятору:** функцию следует встраивать в каждом месте вызова независимо
  от эвристик компилятора. Clang и MSVC оставляют вызовом прямой вызов, который не могут встроить,
  а GCC отвергает такой вызов, как описано в пункте «Рекурсия». MSVC не встраивает ничего на `/Od`.
- **`inline` уже включён:** каждое раскрытие содержит ключевое слово `inline` или `__forceinline`,
  поэтому функция, определённая с макросом в заголовке, может быть определена в каждой единице
  трансляции, куда включён заголовок. Рядом с макросом не должен быть записан `inline`,
  потому что GCC отвергает второй `inline`.
- **Рекурсия:** GCC отвергает рекурсивную функцию с макросом с ошибкой
  `inlining failed in call to 'always_inline'` на `-O0`, `-O1` и `-Og`, а на остальных уровнях
  отвергает её тоже, если не может заменить рекурсию циклом. Clang принимает такую функцию на любом
  уровне. MSVC принимает её, не встраивает и выдаёт предупреждение C4714 под `/W4` с `/O1`
  или `/O2`.
- **Место:** макрос записывается перед объявлением функции, после всех остальных макросов группы,
  которые стоят там же. Раскрытие на GCC и Clang заканчивается словом `inline`, и стандартный
  атрибут после него является ошибкой.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| MSVC, кроме clang-cl | `__forceinline` |
| `__has_cpp_attribute(__gnu__::__always_inline__)` (GCC, Clang) | `[[__gnu__::__always_inline__]] inline` |
| `__has_attribute(__always_inline__)` | `__attribute__((__always_inline__)) inline` |
| ничего из перечисленного | `inline` |

Макрос раскрывается в `__forceinline` на MSVC и в `[[__gnu__::__always_inline__]] inline` на GCC
и Clang.

## `SCL_NOINLINE`

- **Что сообщается компилятору:** функция не встраивается в вызывающий код, поэтому у неё остаётся
  отдельный кадр в стеке вызовов. GCC всё равно может создать специализированную копию функции
  на `-O3`. Вызов функции без побочных эффектов, результат которого не читается, тоже может быть
  отброшен.
- **Место:** макрос записывается перед объявлением функции, после всех макросов группы, которые
  раскрываются в стандартный атрибут. MSVC отбрасывает `[[nodiscard]]` и `[[maybe_unused]]`,
  записанные после `__declspec(noinline)`, и сообщает об этом только предупреждением C5240, которое
  выключено по умолчанию.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| MSVC, кроме clang-cl | `__declspec(noinline)` |
| `__has_cpp_attribute(__gnu__::__noinline__)` (GCC, Clang) | `[[__gnu__::__noinline__]]` |
| `__has_attribute(__noinline__)` | `__attribute__((__noinline__))` |
| ничего из перечисленного | пусто |

Макрос раскрывается в `__declspec(noinline)` на MSVC и в `[[__gnu__::__noinline__]]` на GCC и Clang.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_FORCE_INLINE` | `[[__gnu__::__always_inline__]] inline`, `__attribute__((__always_inline__)) inline`, `inline` | `[[__gnu__::__always_inline__]] inline`, `__attribute__((__always_inline__)) inline`, `inline` | `__forceinline`, `inline` |
| `SCL_NOINLINE` | `[[__gnu__::__noinline__]]`, `__attribute__((__noinline__))`, пусто | `[[__gnu__::__noinline__]]`, `__attribute__((__noinline__))`, пусто | `__declspec(noinline)`, пусто |

## Примеры

Короткая функция, которую компилятор просят встраивать в каждом месте вызова:

<!-- snippet: example/attribute/inline/attribute_inline_example.cpp force_inline -->
```cpp
SCL_FORCE_INLINE
int clamp(int value, int low, int high)
{
    return value < low ? low : value > high ? high : value;
}
```

Сообщение об ошибке, которое компилятор просят оставить отдельной функцией:

<!-- snippet: example/attribute/inline/attribute_inline_example.cpp noinline -->
```cpp
SCL_NOINLINE
void report_out_of_range(int value)
{
    std::cerr << value << " is out of range\n";
}
```

## Смотрите также

- [`SCL_HOT`, `SCL_COLD`](hotcold.md), подсказки о том, как часто вызывается функция
- [`example/attribute/inline/attribute_inline_example.cpp`](../../../../example/attribute/inline/attribute_inline_example.cpp)

---

Назад: [`SCL_FALLTHROUGH`](fallthrough.md) | Далее: [`SCL_HOT`](hotcold.md) |
[К группе](index.md) | [К главной странице](../Main.md)
