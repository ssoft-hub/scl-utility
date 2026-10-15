# `SCL_COUNTER_VALUE`, `SCL_COUNTER_NEXT`

Макрос `SCL_COUNTER_VALUE(Tag)` читает на этапе компиляции счётчик тега `Tag`, а макрос
`SCL_COUNTER_NEXT(Tag)` увеличивает этот счётчик на единицу до конца единицы трансляции.

- Заголовок: `#include <scl/utility/preprocessor/counter.h>`

```cpp
#define SCL_COUNTER_VALUE(...) /* constant expression of type std::uint_fast32_t */
#define SCL_COUNTER_NEXT(...)  /* declaration */
```

## Семантика

- **Счётчик для каждого тега:** каждый тип служит тегом собственного счётчика, и две
  специализации одного шаблона класса являются двумя тегами. Тегом служат и класс
  без конструктора по умолчанию, и неполный или абстрактный класс, и тип функции, и ссылка,
  и `void`. Ссылка, тип с `const` и массив служат тегами отдельно от типа, на который ссылается
  ссылка, от типа без `const` и от указателя на элемент массива.
- **Запись тега:** тегом служит весь список аргументов макроса, поэтому имя с запятыми,
  например `std::pair<int, char>`, записывается как есть.
- **Полное имя тега:** рекомендуется записывать имена тега полностью, начиная с глобального
  пространства имён, например `::std::pair<::fruit::apple, int>`. Иначе при конфликте имени
  тега с именем, объявленным в другой области видимости, программа не компилируется
  либо без диагностики читается и увеличивается счётчик другого тега.
- **Значение:** `SCL_COUNTER_VALUE(Tag)` является константным выражением типа
  `std::uint_fast32_t`. До первого `SCL_COUNTER_NEXT(Tag)` единицы трансляции оно равно `0`
  и после каждого из них становится на единицу больше.
- **Внутри шаблона:** `SCL_COUNTER_VALUE(Tag)` в шаблоне читает счёт в том месте, где макрос
  записан, а не там, где шаблон инстанцируется, при любом теге, в том числе зависящем
  от параметра шаблона.
- **Увеличение:** `SCL_COUNTER_NEXT(Tag)` стоит только в глобальном пространстве имён,
  а не в теле функции, не в классе и не в пространстве имён вызывающего кода.
- **Единица трансляции:** в каждой единице трансляции счёт ведётся отдельно. Встроенная функция
  или шаблон в заголовке, читающие счётчик, могут прочитать в разных единицах трансляции
  разные значения, что нарушает правило одного определения.

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

## Смотрите также

- [`example/preprocessor/counter/preprocessor_counter_example.cpp`](../../../../example/preprocessor/counter/preprocessor_counter_example.cpp)

---

Назад: [Preprocessor](index.md) | Далее: [`SCL_FORWARD`](forward.md) | [К группе](index.md) |
[К главной странице](../Main.md)
