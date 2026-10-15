# `SCL_HAS_RTTI`

Макрос `SCL_HAS_RTTI` раскрывается в `1`, если единица трансляции скомпилирована с RTTI, и в `0`,
если без него.

- Заголовок: `#include <scl/utility/preprocessor/rtti.h>`

```cpp
#define SCL_HAS_RTTI /* 1 or 0 */
```

## Семантика

- **Определён всегда:** вызывающий код проверяет макрос директивой `#if`, а не `#ifdef`. При `#if`
  имя с опечаткой читается как `0`, и компилятор с ключом `-Wundef` выдаёт предупреждение;
  при `#ifdef` такое имя молча читается как ложное.
- **Источник:** значение макроса определяется по `__has_feature(cxx_rtti)`, где компилятор его
  поддерживает (Clang, clang-cl, GCC 14 и новее), а иначе по `_CPPRTTI` (MSVC) или `__GXX_RTTI`
  (GCC 13). Язык не определяет для RTTI макроса проверки возможностей.
- **Не переключатель:** значение макроса задаётся ключами командной строки компилятора, и RTTI
  не включается и не выключается определением макроса вручную. Ключи, при которых он равен `0`,
  перечислены на [странице группы](index.md#ключи-компилятора).

## Примеры

Функция, которая возвращает имя типа объекта в обеих сборках:

<!-- snippet: example/preprocessor/rtti/preprocessor_rtti_example.cpp name_of -->
```cpp
template <typename Type>
std::string name_of([[maybe_unused]] Type const & object)
{
#if SCL_HAS_RTTI
    return scl::type_short_name(object); // the dynamic type of a polymorphic object
#else
    return std::string{scl::type_short_name<Type>()}; // the static type of the argument
#endif
}
```

## Смотрите также

- [`SCL_HAS_EXCEPTIONS`](exceptions.md), такой же макрос для исключений; два макроса
  не зависят друг от друга
- [`example/preprocessor/rtti/preprocessor_rtti_example.cpp`](../../../../example/preprocessor/rtti/preprocessor_rtti_example.cpp)

---

Назад: [`SCL_HAS_EXCEPTIONS`](exceptions.md) | Далее: [`SCL_HAS_THREADS`](threads.md) |
[К группе](index.md) | [К главной странице](../Main.md)
