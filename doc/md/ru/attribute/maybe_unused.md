# `SCL_MAYBE_UNUSED`

Сущность, помеченная `SCL_MAYBE_UNUSED`, может остаться неиспользованной в какой-то сборке,
и компилятор не выдаёт о ней предупреждения.

- Заголовок: `#include <scl/utility/attribute/maybe_unused.h>`

```cpp
#define SCL_MAYBE_UNUSED /* see below */
```

## Семантика

- **Что сообщается компилятору:** сущность может не использоваться, например параметр, который
  читается только в проверке `assert` в сборке без `NDEBUG`. Компилятор не выдаёт предупреждения
  о неиспользованной сущности, и сборки с макросом и без него различаются только этим
  предупреждением.
- **Место:** макрос записывается перед объявлением переменной, параметра или функции, а в объявлении
  типа после `struct`, `class`, `enum` или `enum class`.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| `__has_cpp_attribute(maybe_unused)` (C++17) | `[[maybe_unused]]` |
| `__has_attribute(__unused__)` | `__attribute__((__unused__))` |
| ничего из перечисленного | пусто |

Макрос раскрывается в `[[maybe_unused]]` на GCC, Clang и MSVC в C++20.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_MAYBE_UNUSED` | `[[maybe_unused]]`, `__attribute__((__unused__))`, пусто | `[[maybe_unused]]`, `__attribute__((__unused__))`, пусто | `[[maybe_unused]]`, пусто |

## Примеры

Параметр, который читается только в проверке `assert`:

<!-- snippet: example/attribute/maybe_unused/attribute_maybe_unused_example.cpp maybe_unused -->
```cpp
void resize(std::vector<int> & storage, std::size_t size, SCL_MAYBE_UNUSED std::size_t capacity)
{
    assert(size <= capacity);
    storage.resize(size);
}
```

## Смотрите также

- [`example/attribute/maybe_unused/attribute_maybe_unused_example.cpp`](../../../../example/attribute/maybe_unused/attribute_maybe_unused_example.cpp)

---

Назад: [`SCL_LIKELY`](likely.md) | Далее: [`SCL_NODISCARD`](nodiscard.md) |
[К группе](index.md) | [К главной странице](../Main.md)
