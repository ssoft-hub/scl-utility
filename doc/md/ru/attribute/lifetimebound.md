# `SCL_LIFETIMEBOUND`

Макрос `SCL_LIFETIMEBOUND` записывается на параметре, на объект которого ссылается результат
функции. Clang, а при анализе кода и MSVC, предупреждают, если результат живёт дольше этого объекта.

- Заголовок: `#include <scl/utility/attribute/lifetimebound.h>`

```cpp
#define SCL_LIFETIMEBOUND /* see below */
```

## Семантика

- **Что сообщается компилятору:** ссылка, указатель или представление, которые возвращает функция,
  ссылаются на тот же объект, что и параметр. То же сообщается о `*this` на функции-члене.
- **Что выдаёт Clang:** Clang выдаёт предупреждение `-Wdangling`, если результат живёт дольше
  объекта. Так бывает, если `std::string_view` инициализируется
  из `first_word(std::string{"hello world"})`, а аргумент уничтожается в конце полного выражения.
- **Что выдаёт MSVC:** MSVC выдаёт предупреждение C26815, если результат живёт дольше объекта,
  при анализе кода с ключами `/analyze /analyze:plugin EspXEngine.dll`. MSVC не выдаёт ничего
  без этого модуля анализа.
- **GCC:** макрос раскрывается на GCC в пустоту. GCC выдаёт своё предупреждение
  `-Wdangling-reference`, которое входит в `-Wall`, по собственному анализу, и этот анализ одинаков
  с макросом и без него.
- **Место:** макрос записывается после декларатора параметра или после квалификаторов функции-члена.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| MSVC, кроме clang-cl, `__has_cpp_attribute(msvc::lifetimebound)`, ни `msvc`, ни `lifetimebound` не являются макросами | `[[msvc::lifetimebound]]` |
| `__has_cpp_attribute(_Clang::__lifetimebound__)` (Clang) | `[[_Clang::__lifetimebound__]]` |
| `__has_attribute(__lifetimebound__)` | `__attribute__((__lifetimebound__))` |
| ничего из перечисленного | пусто |

Макрос раскрывается в `[[msvc::lifetimebound]]` на MSVC 19.44, в `[[_Clang::__lifetimebound__]]`
на Clang и в пустоту на GCC.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_LIFETIMEBOUND` | пусто | `[[_Clang::__lifetimebound__]]`, `__attribute__((__lifetimebound__))`, пусто | `[[msvc::lifetimebound]]`, пусто |

## Примеры

Представление строки, переданной функции:

<!-- snippet: example/attribute/lifetimebound/attribute_lifetimebound_example.cpp lifetimebound -->
```cpp
std::string_view first_word(std::string const & text SCL_LIFETIMEBOUND)
{
    return std::string_view{text}.substr(0, text.find(' '));
}
```

## Смотрите также

- [`example/attribute/lifetimebound/attribute_lifetimebound_example.cpp`](../../../../example/attribute/lifetimebound/attribute_lifetimebound_example.cpp)

---

Назад: [`SCL_INDETERMINATE`](indeterminate.md) | Далее: [`SCL_LIKELY`](likely.md) |
[К группе](index.md) | [К главной странице](../Main.md)
