# `SCL_HAS_EXCEPTIONS`

Макрос `SCL_HAS_EXCEPTIONS` раскрывается в `1`, если единица трансляции скомпилирована
с исключениями, и в `0`, если без них.

- Заголовок: `#include <scl/utility/preprocessor/exceptions.h>`

```cpp
#define SCL_HAS_EXCEPTIONS /* 1 or 0 */
```

## Семантика

- **Определён всегда:** вызывающий код проверяет макрос директивой `#if`, а не `#ifdef`. При `#if`
  имя с опечаткой читается как `0`, и компилятор с ключом `-Wundef` выдаёт предупреждение;
  при `#ifdef` такое имя молча читается как ложное.
- **Источник:** значение макроса определяется по `__has_feature(cxx_exceptions)`, где компилятор
  его поддерживает (Clang, clang-cl, GCC 14 и новее), а иначе по `_CPPUNWIND` (MSVC)
  или `__EXCEPTIONS` (GCC 13). Язык не определяет для исключений макроса проверки возможностей.
- **Не переключатель:** значение макроса задаётся ключами командной строки компилятора,
  и исключения не включаются и не выключаются определением макроса вручную. Ключи, при которых
  он равен `0`, перечислены на [странице группы](index.md#ключи-компилятора).

## Примеры

Функция, которая читает текст в обеих сборках:

<!-- snippet: example/preprocessor/exceptions/preprocessor_exceptions_example.cpp text_of -->
```cpp
std::string text_of(scl::any_view const & view)
{
#if SCL_HAS_EXCEPTIONS
    try
    {
        return scl::any_cast<std::string>(view);
    }
    catch (scl::bad_any_cast const &)
    {
        return "<not a string>";
    }
#else
    auto const * text = scl::any_cast<std::string>(&view);
    return text != nullptr ? *text : "<not a string>";
#endif
}
```

## Смотрите также

- [`SCL_HAS_RTTI`](rtti.md), такой же макрос для RTTI; два макроса не зависят друг от друга
- [`example/preprocessor/exceptions/preprocessor_exceptions_example.cpp`](../../../../example/preprocessor/exceptions/preprocessor_exceptions_example.cpp)

---

Назад: [`SCL_FORWARD`](forward.md) | Далее: [`SCL_HAS_RTTI`](rtti.md) | [К группе](index.md) |
[К главной странице](../Main.md)
