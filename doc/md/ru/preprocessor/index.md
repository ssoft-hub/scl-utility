# Preprocessor

В группе preprocessor собраны макросы, которые раскрываются в `1` или `0` в зависимости от того,
скомпилирована ли единица трансляции с RTTI и с исключениями и поддерживает ли стандартная
библиотека потоки; макрос, который передаёт разделённый запятыми список через один параметр
другого макроса; и два макроса, которые читают и увеличивают счётчик на этапе компиляции.

- Заголовок: `#include <scl/utility/preprocessor.h>`

| Заголовок | Страница | Определяет |
|---|---|---|
| `<scl/utility/preprocessor/counter.h>` | [`SCL_COUNTER_VALUE`](counter.md) | `SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT` |
| `<scl/utility/preprocessor/exceptions.h>` | [`SCL_HAS_EXCEPTIONS`](exceptions.md) | `SCL_HAS_EXCEPTIONS` |
| `<scl/utility/preprocessor/forward.h>` | [`SCL_FORWARD`](forward.md) | `SCL_FORWARD` |
| `<scl/utility/preprocessor/rtti.h>` | [`SCL_HAS_RTTI`](rtti.md) | `SCL_HAS_RTTI` |
| `<scl/utility/preprocessor/threads.h>` | [`SCL_HAS_THREADS`](threads.md) | `SCL_HAS_THREADS` |

## Ключи компилятора

Значения макросов [`SCL_HAS_RTTI`](rtti.md) и [`SCL_HAS_EXCEPTIONS`](exceptions.md) задаются
ключами командной строки компилятора. Значение макроса [`SCL_HAS_THREADS`](threads.md) зависит
от того, как собрана стандартная библиотека, поэтому его не меняет ни один ключ таблицы. Первые
два макроса принимают значения:

| Ключи | Компиляторы | `SCL_HAS_RTTI` | `SCL_HAS_EXCEPTIONS` |
|---|---|---|---|
| ни одного из ключей ниже | GCC, Clang | `1` | `1` |
| `-fno-rtti` | GCC, Clang | `0` | `1` |
| `-fno-exceptions` | GCC, Clang | `1` | `0` |
| `/EHsc` | MSVC, clang-cl | `1` | `1` |
| `/EHs-c-` или ни одного ключа `/EH` | MSVC, clang-cl | `1` | `0` |
| `/EHsc /GR-` | MSVC, clang-cl | `0` | `1` |
| `/GR-` | MSVC, clang-cl | `0` | `0` |

Макросы [`SCL_FORWARD`](forward.md), [`SCL_COUNTER_VALUE` и `SCL_COUNTER_NEXT`](counter.md)
ведут себя одинаково при любых ключах сборки.

## Документация группы

### Макросы

- [`SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT`](counter.md) - счётчики, по одному на каждый тип-тег,
  которые читаются и увеличиваются на этапе компиляции
- [`SCL_FORWARD`](forward.md) - разделённый запятыми список, переданный через один параметр
  другого макроса
- [`SCL_HAS_EXCEPTIONS`](exceptions.md) - скомпилирована ли единица трансляции с исключениями
- [`SCL_HAS_RTTI`](rtti.md) - скомпилирована ли единица трансляции с RTTI
- [`SCL_HAS_THREADS`](threads.md) - поддерживает ли стандартная библиотека потоки

### Примеры

- [`example/preprocessor/counter`](../../../../example/preprocessor/counter/preprocessor_counter_example.cpp)
- [`example/preprocessor/exceptions`](../../../../example/preprocessor/exceptions/preprocessor_exceptions_example.cpp)
- [`example/preprocessor/forward`](../../../../example/preprocessor/forward/preprocessor_forward_example.cpp)
- [`example/preprocessor/rtti`](../../../../example/preprocessor/rtti/preprocessor_rtti_example.cpp)
- [`example/preprocessor/threads`](../../../../example/preprocessor/threads/preprocessor_threads_example.cpp)

---

Далее: [`SCL_COUNTER_VALUE`](counter.md) | [К главной странице](../Main.md)
