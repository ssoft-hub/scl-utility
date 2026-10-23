# Приведение значения к категории и cv-квалификаторам заданного типа

Вызов `forward_like<Base>(t)` возвращает ссылку на аргумент `t`. Вид ссылки, lvalue или rvalue,
берётся из типа `Base`, а квалификаторы `const` и `volatile` типа `Base` добавляются к собственным
квалификаторам аргумента.

Шаблонный тип `forward_like_t<Base, Type>` является типом результата вызова функции `forward_like`
для аргумента типа `Type`.

- Заголовок: `#include <scl/utility/type_traits/forward_like.h>`

```cpp
namespace scl
{
    template <typename Base, typename Type>
    using forward_like_t = unspecified;

    template <typename Base, typename T>
    [[nodiscard]]
    constexpr decltype(auto) forward_like(T && t) noexcept;
}
```

## Семантика

### Шаблонный тип `forward_like_t`

- **Вид ссылки.** Шаблонный тип `forward_like_t<Base, Type>` является lvalue-ссылкой, если тип
  `Base` является lvalue-ссылкой, и rvalue-ссылкой в остальных случаях, то есть для типа без ссылки
  и для rvalue-ссылки. Ссылка самого типа `Type` на вид ссылки результата не влияет, поэтому
  аргумент типа `double &` для типа `Base`, равного `int`, возвращается как `double &&`.
- **Cv-квалификаторы.** Квалификаторы `const` и `volatile` типа `Base` добавляются
  к cv-квалификаторам типа `Type`. Квалификаторы, которые уже есть у типа `Type`, сохраняются,
  поэтому тип результата содержит cv-квалификаторы обоих типов. Например, тип
  `forward_like_t<int const &, double volatile>` является типом `double const volatile &`.
- **Тип `void`.** Если тип `Type` является типом `void` с cv-квалификаторами или без них, шаблонный
  тип `forward_like_t` является типом `void` с cv-квалификаторами обоих типов и без ссылки.

### Шаблон функции `forward_like`

- **Приведение.** Вызов `forward_like<Base>(t)` возвращает аргумент `t`, приведённый к типу
  `forward_like_t<Base, T>`, где тип `T` выводится из аргумента. Функция `forward_like`
  не имеет побочных эффектов. Аргумент не копируется и не перемещается, а возвращается ссылка на тот
  же объект. Функция объявлена со спецификаторами `constexpr` и `noexcept` и с атрибутом
  `[[nodiscard]]`.

### Отличие от `std::forward_like`

Шаблон функции `scl::forward_like` добавляет к типу результата квалификатор `volatile` типа `Base`
так же, как квалификатор `const`. К типу результата шаблона функции `std::forward_like` из C++23
квалификатор `volatile` не добавляется.

## Примеры

Тип результата для разных типов `Base` и `Type`:

<!-- snippet: example/type_traits/forward_like/type_traits_forward_like_example.cpp type -->
```cpp
static_assert(std::is_same_v<scl::forward_like_t<int const &, double>, double const &>);
static_assert(std::is_same_v<scl::forward_like_t<int &&, double const>, double const &&>);
static_assert(std::is_same_v<scl::forward_like_t<int, double &>, double &&>);
static_assert(std::is_same_v<scl::forward_like_t<int volatile &, double>, double volatile &>);
```

Член, переданный дальше с категорией значения объекта, которому он принадлежит. Функция `text_of`
возвращает поле `text` как lvalue-ссылку для lvalue-объекта и как rvalue-ссылку для rvalue-объекта:

<!-- snippet: example/type_traits/forward_like/type_traits_forward_like_example.cpp member -->
```cpp
struct label
{
    std::string text;
};

template <typename Owner>
decltype(auto) text_of(Owner && owner)
{
    return scl::forward_like<Owner>(owner.text);
}

static_assert(std::is_same_v<decltype(text_of(std::declval<label &>())), std::string &>);
static_assert(std::is_same_v<decltype(text_of(std::declval<label const &>())), std::string const &>);
static_assert(std::is_same_v<decltype(text_of(std::declval<label>())), std::string &&>);
```

<!-- snippet: example/type_traits/forward_like/type_traits_forward_like_example.cpp move -->
```cpp
    label first{"first"};
    std::string const copied = text_of(first);
    std::string const moved = text_of(std::move(first));
    std::cout << copied << ' ' << moved << '\n'; // first first
```

## Смотрите также

- [`example/type_traits/forward_like/type_traits_forward_like_example.cpp`](../../../../example/type_traits/forward_like/type_traits_forward_like_example.cpp)

---

Назад: [Обнаружение наличия оператора](operator_detection.md) |
Далее: [Формирование типа указателя на член по типу объекта](member_like.md) | [К группе](index.md) |
[К главной странице](../Main.md)
