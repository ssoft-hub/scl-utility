# Концепты отношений между типами

Применение концептов отношений между типами позволяет определить, является ли один класс базовым
классом другого класса, в том числе виртуальным базовым классом.

- Заголовок: `#include <scl/utility/concepts/type_relation.h>`

```cpp
#define SCL_HAS_VIRTUAL_BASE_OF_CONCEPT /* 1 or 0 */
```

```cpp
namespace scl::concepts
{
    template <typename Base, typename Derived>
    concept base_of = ::scl::concepts::class_type<Base> && ::scl::concepts::class_type<Derived> &&
        ::std::is_base_of_v<Base, Derived>;

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
#if defined(__cpp_lib_is_virtual_base_of) && __cpp_lib_is_virtual_base_of >= 202406L
    template <typename Base, typename Derived>
    concept virtual_base_of = ::scl::concepts::base_of<Base, Derived> &&
        ::std::is_virtual_base_of_v<Base, Derived>;
#else
    template <typename Base, typename Derived>
    concept virtual_base_of = ::scl::concepts::base_of<Base, Derived> &&
        __builtin_is_virtual_base_of(Base, Derived);
#endif
#endif
}
```

## Семантика

### Общие правила

- **Cv-квалификаторы и ссылки.** Cv-квалификаторы обоих типов не учитываются. Если хотя бы один
  из типов является ссылкой, пара типов не удовлетворяет ни одному из концептов.
- **Параметр шаблона.** Концепт можно указать при объявлении параметра шаблона, например
  `template <scl::concepts::base_of<Derived> Base>`. В этом случае параметр шаблона проверяется
  как тип `Base`.
- **Полный тип.** Если оба типа являются классами, но не объединениями, и не совпадают, тип
  `Derived` должен быть полным, как того требуют свойства `std::is_base_of`
  и `std::is_virtual_base_of`. Код, который использует концепты, не должен проверять ими тип,
  который не отвечает этому требованию.

### `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT`

- **Значение.** Макрос показывает, объявлен ли концепт `virtual_base_of`. Макрос раскрывается в `1`,
  если стандартная библиотека содержит свойство `std::is_virtual_base_of` (макрос
  `__cpp_lib_is_virtual_base_of` не меньше `202406L`) или компилятор предоставляет встроенную
  функцию `__builtin_is_virtual_base_of`. В остальных случаях макрос раскрывается в `0`, и концепт
  `virtual_base_of` не объявляется.
- **Проверка макроса.** Макрос определён всегда, поэтому коду, который использует макрос, следует
  проверять его через `#if`, а не через `#ifdef`. При проверке через `#if` опечатка в имени макроса
  читается как `0` и приводит к предупреждению `-Wundef`, если компилятору GCC или Clang задан
  параметр, который включает это предупреждение, а при проверке через `#ifdef` опечатка читается
  как ложь без предупреждения.

### `base_of<Base, Derived>`

- **Условие.** Пара проверяемых типов удовлетворяет концепту, если оба типа являются классами,
  но не объединениями, и класс `Base` совпадает с классом `Derived` или является его базовым
  классом, прямым или косвенным.
- **Доступ и повторение.** Доступ к базовому классу не учитывается, и класс `Base` может входить
  в класс `Derived` как базовый класс несколько раз. Этим концепт отличается от концепта
  `std::derived_from`, который требует открытого и однозначного базового класса.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_base_of_v<Base, Derived>`.

### `virtual_base_of<Base, Derived>`

- **Объявление.** Концепт объявлен, только если макрос `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT`
  раскрывается в `1`.
- **Условие.** Пара проверяемых типов удовлетворяет концепту, если класс `Base` является виртуальным
  базовым классом класса `Derived`, прямым или косвенным. Класс не является виртуальным базовым
  классом самого себя.
- **Доступ и повторение.** Доступ к базовому классу не учитывается. Класс `Derived` может содержать
  класс `Base` одновременно как виртуальный и как невиртуальный базовый класс.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_virtual_base_of_v<Base, Derived>` из C++26.

### Выбор перегрузки

- **Правило.** Концепт `base_of` объявлен через концепт [`class_type`](type_category.md) каждого
  из двух типов, а концепт `virtual_base_of` объявлен через концепт `base_of`. Поэтому ограничение
  каждого из них строже ограничений концептов `class_type`, `object`, `compound` и `non_reference`
  каждого из типов.
- **Выбор шаблона.** Компилятор выбирает шаблон функции, ограниченный концептом `virtual_base_of`,
  а не шаблон, ограниченный концептом `base_of`. Точно так же шаблон, ограниченный концептом
  `base_of`, выбирается вместо шаблона, ограниченного концептом `class_type`.

## Примеры

Типы, используемые в примерах:

<!-- snippet: example/concepts/type_relation/concepts_type_relation_example.cpp types -->
```cpp
struct node
{};

struct left_branch : virtual node
{};

struct right_branch : virtual node
{};

struct joined
    : left_branch
    , right_branch
{};

struct plain : node
{};

struct hidden : private node
{};
```

Проверка базового класса:

<!-- snippet: example/concepts/type_relation/concepts_type_relation_example.cpp base_of -->
```cpp
static_assert(scl::concepts::base_of<node, plain>);
static_assert(scl::concepts::base_of<node, hidden>);
static_assert(scl::concepts::base_of<node, joined>);
static_assert(scl::concepts::base_of<node, node>);
static_assert(!scl::concepts::base_of<plain, node>);
static_assert(!scl::concepts::base_of<int, int>);
```

Проверка виртуального базового класса, если макрос `SCL_HAS_VIRTUAL_BASE_OF_CONCEPT` раскрывается
в `1`:

<!-- snippet: example/concepts/type_relation/concepts_type_relation_example.cpp virtual_base_of -->
```cpp
static_assert(scl::concepts::virtual_base_of<node, left_branch>);
static_assert(scl::concepts::virtual_base_of<node, joined>);
static_assert(!scl::concepts::virtual_base_of<node, plain>);
static_assert(!scl::concepts::virtual_base_of<node, node>);
static_assert(!scl::concepts::virtual_base_of<left_branch, node>);
```

## Смотрите также

- [`example/concepts/type_relation/concepts_type_relation_example.cpp`](../../../../example/concepts/type_relation/concepts_type_relation_example.cpp)

---

Назад: [Концепты свойств типов](type_property.md) | [К группе](index.md) |
[К главной странице](../Main.md)
