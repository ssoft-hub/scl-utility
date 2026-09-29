# Поддержка потоков

Сообщает, поддерживает ли стандартная библиотека потоки, и управляет тем кодом ScL,
который захватывает мьютекс.

- Заголовок: `#include <scl/utility/preprocessor/threads.h>`

Содержание:
- [`SCL_HAS_THREADS`](#scl_has_threads)

---

## `SCL_HAS_THREADS`

Раскрывается в `1`, если стандартная библиотека поддерживает потоки, иначе в `0`.

- Заголовок: `#include <scl/utility/preprocessor/threads.h>`
- Объявление: `#define SCL_HAS_THREADS /* 1 или 0 */`

### Семантика

- **Определён всегда:** проверять через `#if`, а не через `#ifdef`. Тогда опечатку в имени
  сообщит `-Wundef`, а не молчаливое вычисление в ложь.
- **Выводится по библиотеке:** `_GLIBCXX_HAS_GTHREADS` (libstdc++), `_LIBCPP_HAS_NO_THREADS`
  или `_LIBCPP_HAS_THREADS` (libc++); любая другая библиотека считается поддерживающей потоки.
  Библиотека, собранная без поддержки потоков, например библиотека `arm-none-eabi`,
  не объявляет `std::mutex`, а её `std::thread` не может запустить поток. Стандартный макрос
  `__STDCPP_THREADS__` сообщает о целевой платформе, а не о библиотеке, и Clang определяет его
  и для `arm-none-eabi`, поэтому этот макрос и существует.
- **Это отчёт, а не переключатель:** определение его вручную не добавляет и не убирает потоки.
  Он сообщает, как собрана стандартная библиотека.
- **Что он закрывает в ScL:** хранилище восстановленных имён, которым пользуются
  `type_name(obj)` и `type_short_name(obj)`, захватывает `std::mutex` только там, где макрос
  равен `1`, а там, где он равен `0`, мьютекс не захватывает;
  см. [`type_name`](../runtime/type_name.md).

### Примеры

```cpp
#include <scl/utility/preprocessor/threads.h>

#if SCL_HAS_THREADS
#include <mutex>
#endif

int next_ticket()
{
    static int ticket = 0;
#if SCL_HAS_THREADS
    static std::mutex mutex;
    std::lock_guard const lock{mutex};
#endif
    return ++ticket;
}
```

## Примечания

- [`SCL_HAS_RTTI`](rtti.md) и [`SCL_HAS_EXCEPTIONS`](exceptions.md) сообщают о единице
  трансляции, настройки которой задаёт командная строка компилятора. Этот макрос сообщает
  о стандартной библиотеке, которая поставляется с набором инструментов.
