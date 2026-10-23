# Выбор перегрузки функции или метода по типам параметров

Адрес перегруженной функции `&f` или перегруженного метода `&Class::f` не имеет единственного типа,
поэтому его нельзя сохранить в переменной, объявленной со спецификатором `auto`, или передать
в шаблон функции. Функциональный объект `overload_cast<Arguments...>` выбирает из набора перегрузок
ту, параметры которой имеют в точности типы `Arguments...`, и возвращает указатель на неё.

- Заголовок: `#include <scl/utility/type_traits/overload_cast.h>`

```cpp
namespace scl
{
    template <typename... Arguments>
    inline constexpr auto overload_cast = unspecified;
}
```

## Семантика

### Функциональный объект `overload_cast`

- **Вызов.** Вызовы `overload_cast<Arguments...>(&f)` и `overload_cast<Arguments...>(&Class::f)`
  возвращают указатель на перегрузку функции или метода, параметры которой имеют в точности типы
  `Arguments...`. Тип результата и спецификатор `noexcept` перегрузки при этом могут быть любыми
  и сохраняются в типе возвращённого указателя. Например, для перегрузок `int twice(int)`
  и `double twice(double) noexcept` вызов `overload_cast<double>(&twice)` возвращает указатель типа
  `double (*)(double) noexcept`. Оператор вызова функционального объекта объявлен со спецификаторами
  `constexpr` и `noexcept` и с атрибутом `[[nodiscard]]`.
- **Точные типы параметров.** Тип параметра должен совпадать с типом из `Arguments...` в точности,
  неявные преобразования при выборе не учитываются. Поэтому для перегрузок `f(int &)`
  и `f(int const &)` вызовы `overload_cast<int &>(&f)` и `overload_cast<int const &>(&f)` выбирают
  разные перегрузки. Квалификаторы `const` и `volatile` верхнего уровня у типов `Arguments...`
  не учитываются, подробнее об этом сказано в разделе "Примечания".
- **Квалификаторы метода.** Квалификаторы самого метода (`const`, `volatile`, `&` и `&&`) при выборе
  перегрузки не учитываются, то есть метод с подходящими параметрами выбирается при любых
  квалификаторах. Возвращённый указатель имеет тип выбранной перегрузки вместе с этими
  квалификаторами.
- **Нет перегрузки или их несколько.** Вызов не компилируется, если ни одна перегрузка не имеет
  параметров `Arguments...`. Вызов не компилируется и тогда, когда параметры `Arguments...` есть
  у нескольких методов, которые различаются только квалификаторами, например `int f(int)`
  и `int f(int) const`, потому что выбрать между ними по типам параметров нельзя.
- **Метод с явным параметром объекта.** Адрес метода с явным параметром объекта (C++23), например
  `int f(this Class const & self, int)`, является обычным указателем на функцию типа
  `int (*)(Class const &, int)`. Поэтому такой метод выбирается вызовом
  `overload_cast<Class const &, int>(&Class::f)`, в котором первым передаётся тип параметра объекта.

### Ограничения

- **Шаблоны функций.** Вызов не компилируется, если в наборе перегрузок есть шаблон функции,
  даже когда перегрузка с параметрами `Arguments...` шаблоном не является.
- **Функции с многоточием.** Перегрузка, список параметров которой заканчивается многоточием `...`
  в стиле C, никогда не выбирается. Набор, в котором рядом с перегрузкой с параметрами
  `Arguments...` есть перегрузка с теми же параметрами и многоточием, не поддерживается,
  потому что результат вызова для такого набора зависит от компилятора.

## Примеры

Типы примеров:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp types -->
```cpp
class canvas
{
private:
    int m_width{};

public:
    void resize(int width) { m_width = width; }
    void resize(int width, int height) noexcept { m_width = width * height; }
    int width() const & { return m_width; }
};

int twice(int value);
double twice(double value) noexcept;
```

Перегрузки метода, которые различаются параметрами. У метода `width` перегрузок нет, но вызов
`overload_cast<>` подходит и для него и возвращает указатель вместе с квалификаторами `const &`:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp member -->
```cpp
[[maybe_unused]]
auto const resize_one = scl::overload_cast<int>(&canvas::resize);
auto const resize_two = scl::overload_cast<int, int>(&canvas::resize);

static_assert(std::is_same_v<decltype(resize_one), void (canvas::* const)(int)>);
static_assert(std::is_same_v<decltype(resize_two), void (canvas::* const)(int, int) noexcept>);
static_assert(std::is_same_v<decltype(scl::overload_cast<>(&canvas::width)), int (canvas::*)() const &>);
```

Перегрузки функции. Спецификатор `noexcept` перегрузки сохраняется в типе указателя:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp function -->
```cpp
auto const twice_int = scl::overload_cast<int>(&twice);
[[maybe_unused]]
auto const twice_double = scl::overload_cast<double>(&twice);

static_assert(std::is_same_v<decltype(twice_int), int (* const)(int)>);
static_assert(std::is_same_v<decltype(twice_double), double (* const)(double) noexcept>);
```

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp call -->
```cpp
    canvas board;
    (board.*resize_two)(3, 4);
    std::cout << board.width() << ' ' << twice_int(21) << '\n'; // 12 42
```

## Примечания

- Квалификаторы `const` и `volatile` верхнего уровня у типа параметра не входят в тип функции,
  поэтому `void f(int)` и `void f(int const)` являются объявлениями одной и той же функции. По этой
  причине вызов `overload_cast<int const>(&f)` выбирает перегрузку `f(int)`.

## Смотрите также

- [Определение сигнатуры вызываемого типа](signature.md) - шаблонный тип `scl::signature_t`,
  применимый к типу указателя, возвращённого функциональным объектом `overload_cast`
- [`example/type_traits/overload_cast/type_traits_overload_cast_example.cpp`](../../../../example/type_traits/overload_cast/type_traits_overload_cast_example.cpp)

---

Назад: [Формирование типа указателя на член по типу объекта](member_like.md) |
Далее: [Определение сигнатуры вызываемого типа](signature.md) | [К группе](index.md) |
[К главной странице](../Main.md)
