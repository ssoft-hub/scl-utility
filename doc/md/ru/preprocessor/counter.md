# `SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT`

Макрос `SCL_COUNTER_VALUE(Tag)` читает на этапе компиляции счётчик тега `Tag`, а макрос
`SCL_COUNTER_NEXT(Tag)` увеличивает этот счётчик на единицу до конца единицы трансляции.

- Заголовок: `#include <scl/utility/preprocessor/counter.h>`

```cpp
#define SCL_COUNTER_VALUE(...) /* constant expression of type std::uint_fast32_t */
#define SCL_COUNTER_NEXT(...)  /* declaration */
```

## Семантика

- **Значение:** `SCL_COUNTER_VALUE(Tag)` является константным выражением типа
  `std::uint_fast32_t`. До первого `SCL_COUNTER_NEXT(Tag)` единицы трансляции оно равно `0`,
  а после каждого из них становится на единицу больше.
- **Размещение:** `SCL_COUNTER_NEXT(Tag)` записывается только в глобальном пространстве имён,
  а не в теле функции, не в классе и не в пространстве имён, объявленном вызывающим кодом.
- **Внутри шаблона:** в шаблоне `SCL_COUNTER_VALUE(Tag)` равен счёту в том месте,
  где записан макрос, а не там, где шаблон инстанцируется. Это верно для любого тега,
  в том числе для тега, зависящего от параметра шаблона.
- **Что является тегом:** каждый тип является тегом собственного счётчика, в том числе класс
  без конструктора по умолчанию, неполный или абстрактный класс, тип функции, ссылка и `void`.
- **Разные теги:** каждой паре ниже соответствуют два разных счётчика:
  - две специализации одного шаблона класса, например `per_type<int>` и `per_type<long>`;
  - ссылка и тип, на который она ссылается, например `int &` и `int`;
  - тип с `const` и тот же тип без `const`, например `int const` и `int`;
  - массив и указатель на его элемент, например `int[3]` и `int *`.
- **Запись тега:** тегом является весь список аргументов макроса, поэтому имя с запятыми,
  например `std::pair<int, char>`, записывается как есть.
- **Полное имя тега:** рекомендуется записывать имена тега полностью, начиная с глобального
  пространства имён, например `::std::pair<::lamp, int>`. Если имя тега конфликтует
  с именем, объявленным в другой области видимости, программа не компилируется
  либо без диагностики читается или увеличивается счётчик другого тега.
- **Единица трансляции:** в каждой единице трансляции счёт ведётся отдельно. Встроенная функция
  или шаблон в заголовке, читающие счётчик, могут получить в разных единицах трансляции разные
  значения, что нарушает правило одного определения. Рекомендуется читать и увеличивать счётчик
  в пределах одного исходного файла.

## Примеры

Теги примера:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp tags -->
```cpp
struct apples
{};

struct pears
{};
```

Два счётчика, которые увеличиваются раздельно:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp count -->
```cpp
static_assert(SCL_COUNTER_VALUE(apples) == 0);

SCL_COUNTER_NEXT(apples)
SCL_COUNTER_NEXT(apples)
SCL_COUNTER_NEXT(pears)

static_assert(SCL_COUNTER_VALUE(apples) == 2);
static_assert(SCL_COUNTER_VALUE(pears) == 1);
```

Две специализации одного шаблона класса как два тега:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp template_tag -->
```cpp
template <typename Type>
struct per_type
{};

SCL_COUNTER_NEXT(per_type<int>)

static_assert(SCL_COUNTER_VALUE(per_type<int>) == 1);
static_assert(SCL_COUNTER_VALUE(per_type<long>) == 0);
```

Тег, записанный полностью, начиная с глобального пространства имён, и прочитанный
в пространстве имён, где объявлено то же имя:

<!-- snippet: example/preprocessor/counter/preprocessor_counter_example.cpp full_name -->
```cpp
struct lamp
{};

namespace shop
{
    struct lamp
    {};
} // namespace shop

SCL_COUNTER_NEXT(::lamp)

namespace shop
{
    static_assert(SCL_COUNTER_VALUE(::lamp) == 1);
    static_assert(SCL_COUNTER_VALUE(lamp) == 0); // shop::lamp, another tag
} // namespace shop
```

## Смотрите также

- [`example/preprocessor/counter/preprocessor_counter_example.cpp`](../../../../example/preprocessor/counter/preprocessor_counter_example.cpp)

---

Назад: [Preprocessor](index.md) | Далее: [`SCL_FORWARD`](forward.md) | [К группе](index.md) |
[К главной странице](../Main.md)
