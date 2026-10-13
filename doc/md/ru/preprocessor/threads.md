# `SCL_HAS_THREADS`

Макрос `SCL_HAS_THREADS` раскрывается в `1`, если стандартная библиотека поддерживает потоки,
и в `0`, если она собрана без них.

- Заголовок: `#include <scl/utility/preprocessor/threads.h>`

```cpp
#define SCL_HAS_THREADS /* 1 or 0 */
```

## Семантика

- **Определён всегда:** вызывающий код проверяет макрос директивой `#if`, а не `#ifdef`. При `#if`
  имя с опечаткой читается как `0`, и компилятор с ключом `-Wundef` выдаёт предупреждение;
  при `#ifdef` такое имя молча читается как ложное.
- **Источник:** значение макроса определяется по настройкам библиотеки:
  `_GLIBCXX_HAS_GTHREADS` (libstdc++), `_LIBCPP_HAS_NO_THREADS` или `_LIBCPP_HAS_THREADS`
  (libc++). Любая другая библиотека считается поддерживающей потоки.
- **Библиотека без потоков:** такая библиотека, как библиотека `arm-none-eabi`, не объявляет
  `std::mutex`, а её `std::thread` не может запустить поток. Значение стандартного
  макроса `__STDCPP_THREADS__` зависит от целевой платформы, а не от библиотеки, и Clang
  определяет его и для `arm-none-eabi`.
- **Не переключатель:** значение макроса зависит от того, как собрана стандартная библиотека,
  и потоки не добавляются и не убираются определением макроса вручную. Его не меняет ни один ключ
  из перечисленных на [странице группы](index.md#ключи-компилятора).

## Примеры

Функция, которая захватывает мьютекс, только если библиотека поддерживает потоки:

<!-- snippet: example/preprocessor/threads/preprocessor_threads_example.cpp next_ticket -->
```cpp
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

## Смотрите также

- [`SCL_HAS_RTTI`](rtti.md) и [`SCL_HAS_EXCEPTIONS`](exceptions.md), значения которых
  зависят от единицы трансляции, а не от библиотеки
- [`example/preprocessor/threads/preprocessor_threads_example.cpp`](../../../../example/preprocessor/threads/preprocessor_threads_example.cpp)

---

Назад: [`SCL_HAS_RTTI`](rtti.md) | [К группе](index.md) | [К главной странице](../Main.md)
