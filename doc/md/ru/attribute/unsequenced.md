# `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE`

Функция, помеченная `SCL_UNSEQUENCED` или `SCL_REPRODUCIBLE`, не меняет ничего наблюдаемого
вызывающим кодом, поэтому оптимизатор может объединять, переносить или отбрасывать её вызовы.

- Заголовок: `#include <scl/utility/attribute/unsequenced.h>`

```cpp
#define SCL_UNSEQUENCED(...) /* see below */
#define SCL_REPRODUCIBLE(...) /* see below */
```

## Семантика

- **Аргумент:** аргументом любого из макросов являются тип возвращаемого значения и декларатор
  функции без тела. Атрибут ставится туда, где его требует каждая форма записи.
- **Что сообщается компилятору через `SCL_UNSEQUENCED`:** результат зависит только от аргументов.
  Функция не меняет ничего наблюдаемого вызывающим кодом и не читает память, которая может
  измениться между двумя вызовами, в том числе через аргумент-указатель или аргумент-ссылку. GCC
  и Clang могут объединить два вызова с равными аргументами, вынести вызов из цикла и отбросить
  вызов, результат которого не читается.
- **Что сообщается компилятору через `SCL_REPRODUCIBLE`:** функция не меняет ничего наблюдаемого
  вызывающим кодом, но может читать глобальную память и память, на которую ссылается
  аргумент-указатель или аргумент-ссылка. Оптимизатор может объединить два вызова, между которыми
  нет записи в память.
- **Ложное обещание:** поведение программы не определено, если раскрытие выбрано по строке таблицы,
  которая содержит атрибут, а функция меняет наблюдаемое вызывающим кодом или под `SCL_UNSEQUENCED`
  читает память, которая может измениться между двумя вызовами.
- **Аргумент с побочным эффектом:** инкремент в `f(++i)` выполняется, но вызов `f` может быть
  объединён с более ранним или отброшен.
- **Форма записи C23:** в C23 есть родственные атрибуты `[[unsequenced]]` и `[[reproducible]]`,
  которые пишутся после декларатора и дают собственные обещания. Ни одна редакция C++ не приняла эти
  атрибуты. Первая строка каждой таблицы нужна для компилятора, который предложит их в C++,
  и при раскрытии по этой строке компилятору сообщается обещание C23 вместо описанного выше.

| Условие, первое совпавшее | `SCL_UNSEQUENCED(...)` |
|---|---|
| `__has_cpp_attribute(unsequenced)`, не GCC, `unsequenced` не является макросом | `__VA_ARGS__ [[unsequenced]]` |
| MSVC, кроме clang-cl | `__VA_ARGS__` |
| `__has_cpp_attribute(__gnu__::__const__)` (GCC, Clang) | `[[__gnu__::__const__]] __VA_ARGS__` |
| `__has_attribute(__const__)` | `__attribute__((__const__)) __VA_ARGS__` |
| ничего из перечисленного | `__VA_ARGS__` |

| Условие, первое совпавшее | `SCL_REPRODUCIBLE(...)` |
|---|---|
| `__has_cpp_attribute(reproducible)`, не GCC, `reproducible` не является макросом | `__VA_ARGS__ [[reproducible]]` |
| MSVC, кроме clang-cl | `__VA_ARGS__` |
| `__has_cpp_attribute(__gnu__::__pure__)` (GCC, Clang) | `[[__gnu__::__pure__]] __VA_ARGS__` |
| `__has_attribute(__pure__)` | `__attribute__((__pure__)) __VA_ARGS__` |
| ничего из перечисленного | `__VA_ARGS__` |

Макросы раскрываются в `__VA_ARGS__` на MSVC, а в `[[__gnu__::__const__]] __VA_ARGS__`
и `[[__gnu__::__pure__]] __VA_ARGS__` на GCC и Clang. Clang 22 не сообщает об атрибутах C23 в C++.
GCC 15 и более поздние версии сообщают об обоих атрибутах, но в C++ игнорируют их с предупреждением,
поэтому первая строка таблицы на GCC не выбирается.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_UNSEQUENCED(...)` | `[[__gnu__::__const__]] __VA_ARGS__`, `__attribute__((__const__)) __VA_ARGS__`, `__VA_ARGS__` | `[[__gnu__::__const__]] __VA_ARGS__`, `__attribute__((__const__)) __VA_ARGS__`, `__VA_ARGS__` | `__VA_ARGS__` |
| `SCL_REPRODUCIBLE(...)` | `[[__gnu__::__pure__]] __VA_ARGS__`, `__attribute__((__pure__)) __VA_ARGS__`, `__VA_ARGS__` | `[[__gnu__::__pure__]] __VA_ARGS__`, `__attribute__((__pure__)) __VA_ARGS__`, `__VA_ARGS__` | `__VA_ARGS__` |

## Примеры

Функция, результат которой зависит только от аргумента, и функция, которая читает массив, на который
указывает её аргумент:

<!-- snippet: example/attribute/unsequenced/attribute_unsequenced_example.cpp unsequenced -->
```cpp
SCL_UNSEQUENCED(int square(int value)) { return value * value; }

SCL_REPRODUCIBLE(std::size_t count_zeros(int const * values, std::size_t size))
{
    std::size_t zeros = 0;
    for (std::size_t index = 0; index < size; ++index)
        if (values[index] == 0)
            ++zeros;
    return zeros;
}
```

## Смотрите также

- [`example/attribute/unsequenced/attribute_unsequenced_example.cpp`](../../../../example/attribute/unsequenced/attribute_unsequenced_example.cpp)

---

Назад: [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) | [К группе](index.md) |
[К главной странице](../Main.md)
