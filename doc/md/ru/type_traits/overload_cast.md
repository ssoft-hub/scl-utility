# Выбор перегрузки функции или метода по типам параметров

Адрес перегруженной функции `&f` или перегруженного метода `&Class::f` не имеет единственного типа,
поэтому его нельзя сохранить в переменной, объявленной со спецификатором `auto`, или передать
в шаблон функции. Функциональный объект `overload_cast<Arguments...>` выбирает из набора перегрузок
ту, параметры которой имеют в точности типы `Arguments...`, и возвращает указатель на неё.

Если перегрузки метода различаются только квалификаторами, например `int & count() &`
и `int count() const &`, выбрать между ними по типам параметров нельзя. Для такого случая
функциональный объект `member_overload_cast<Type, Arguments...>` выбирает метод, параметры которого
имеют в точности типы `Arguments...`, а квалификаторы соответствуют типу объекта `Type`.

- Заголовок: `#include <scl/utility/type_traits/overload_cast.h>`

```cpp
namespace scl
{
    template <typename... Arguments>
    inline constexpr auto overload_cast = unspecified;

    template <typename Type, typename... Arguments>
    inline constexpr auto member_overload_cast = unspecified;
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
  и `int f(int) const`, потому что выбрать между ними по типам параметров нельзя. Одну из таких
  перегрузок выбирает функциональный объект `member_overload_cast`.
- **Метод с явным параметром объекта.** Адрес метода с явным параметром объекта (C++23), например
  `int f(this Class const & self, int)`, является обычным указателем на функцию типа
  `int (*)(Class const &, int)`. Поэтому такой метод выбирается вызовом
  `overload_cast<Class const &, int>(&Class::f)`, в котором первым передаётся тип параметра объекта.

### Функциональный объект `member_overload_cast`

- **Вызов.** Вызов `member_overload_cast<Type, Arguments...>(&Class::f)` возвращает указатель
  на метод, параметры которого имеют в точности типы `Arguments...`, а квалификаторы соответствуют
  типу объекта `Type`. Cv-квалификаторы метода должны совпадать с cv-квалификаторами типа `Type`,
  а ссылочный квалификатор метода должен быть равен `&` для lvalue-ссылки, `&&` для rvalue-ссылки
  и отсутствовать для типа без ссылки, как в шаблонном типе
  [`member_function_like_t`](member_like.md). Например, для перегрузок `int & count() &`
  и `int count() const &` вызов `member_overload_cast<counter const &>(&counter::count)` выбирает
  вторую из них. Метод без ссылочного квалификатора выбирается только для типа без ссылки, поэтому
  для метода `int level() const` нужен тип `gauge const`, а с типом `gauge const &` вызов
  не компилируется, хотя такой метод можно вызвать для объекта этого типа. Тип результата
  и спецификатор `noexcept` метода могут быть любыми. Возвращённый указатель является указателем
  на член того класса, в котором объявлен метод. Функциональный объект `member_overload_cast`
  никогда не выбирает метод с явным параметром объекта, потому что адрес такого метода не является
  указателем на член. Оператор вызова объявлен со спецификаторами `constexpr` и `noexcept`
  и с атрибутом `[[nodiscard]]`.

### Ограничения

- **Шаблоны функций.** Вызов любого из двух функциональных объектов не компилируется, если в наборе
  перегрузок есть шаблон функции, даже когда перегрузка с параметрами `Arguments...` шаблоном
  не является.
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

class counter
{
private:
    int m_count{};

public:
    int & count() & { return m_count; }
    int count() const & { return m_count; }
};

class gauge
{
private:
    int m_level{};

public:
    int & level() { return m_level; }
    int level() const { return m_level; }
};
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

Перегрузки метода, которые различаются только квалификаторами, выбираются по типу объекта:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp qualified -->
```cpp
auto const count_mutable = scl::member_overload_cast<counter &>(&counter::count);
auto const count_const = scl::member_overload_cast<counter const &>(&counter::count);

static_assert(std::is_same_v<decltype(count_mutable), int & (counter::* const)() &>);
static_assert(std::is_same_v<decltype(count_const), int (counter::* const)() const &>);
```

Метод без ссылочного квалификатора, который выбирается только для типа объекта без ссылки:

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp no_ref_qualifier -->
```cpp
template <typename Type>
using level_cast = decltype(scl::member_overload_cast<Type>(&gauge::level));

[[maybe_unused]]
auto const level_const = scl::member_overload_cast<gauge const>(&gauge::level);

static_assert(std::is_same_v<decltype(level_const), int (gauge::* const)() const>);
static_assert(!scl::is_detected_v<level_cast, gauge const &>);
```

<!-- snippet: example/type_traits/overload_cast/type_traits_overload_cast_example.cpp call -->
```cpp
    canvas board;
    (board.*resize_two)(3, 4);
    counter clicks;
    (clicks.*count_mutable)() = 7;
    int const count = (clicks.*count_const)();
    std::cout << board.width() << ' ' << twice_int(21) << ' ' << count << '\n'; // 12 42 7
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
