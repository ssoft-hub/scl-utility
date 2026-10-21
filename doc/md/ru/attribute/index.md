# Attribute

Макросы группы attribute раскрываются в атрибут или подсказку компилятору в той форме, которую
принимает данный компилятор. Макрос раскрывается в пустоту или в простую замену, например `inline`
или `((void)0)`, если у компилятора такого атрибута нет. Вместо цепочки `#if` по компиляторам
пишется один макрос.

- Заголовок: `#include <scl/utility/attribute.h>`

| Заголовок | Страница | Определяет |
|---|---|---|
| `<scl/utility/attribute/assume.h>` | [`SCL_ASSUME`](assume.md) | `SCL_ASSUME`, `SCL_UNREACHABLE` |
| `<scl/utility/attribute/deprecated.h>` | [`SCL_DEPRECATED`](deprecated.md) | `SCL_DEPRECATED`, `SCL_DEPRECATED_MSG` |
| `<scl/utility/attribute/fallthrough.h>` | [`SCL_FALLTHROUGH`](fallthrough.md) | `SCL_FALLTHROUGH` |
| `<scl/utility/attribute/inline.h>` | [`SCL_FORCE_INLINE`](inline.md) | `SCL_FORCE_INLINE`, `SCL_NOINLINE` |
| `<scl/utility/attribute/hotcold.h>` | [`SCL_HOT`](hotcold.md) | `SCL_HOT`, `SCL_COLD` |
| `<scl/utility/attribute/indeterminate.h>` | [`SCL_INDETERMINATE`](indeterminate.md) | `SCL_INDETERMINATE` |
| `<scl/utility/attribute/lifetimebound.h>` | [`SCL_LIFETIMEBOUND`](lifetimebound.md) | `SCL_LIFETIMEBOUND` |
| `<scl/utility/attribute/likely.h>` | [`SCL_LIKELY`](likely.md) | `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR` |
| `<scl/utility/attribute/maybe_unused.h>` | [`SCL_MAYBE_UNUSED`](maybe_unused.md) | `SCL_MAYBE_UNUSED` |
| `<scl/utility/attribute/nodiscard.h>` | [`SCL_NODISCARD`](nodiscard.md) | `SCL_NODISCARD`, `SCL_NODISCARD_MSG` |
| `<scl/utility/attribute/noreturn.h>` | [`SCL_NORETURN`](noreturn.md) | `SCL_NORETURN` |
| `<scl/utility/attribute/no_unique_address.h>` | [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) | `SCL_NO_UNIQUE_ADDRESS` |
| `<scl/utility/attribute/unsequenced.h>` | [`SCL_UNSEQUENCED`](unsequenced.md) | `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE` |

## Два вида макросов

Каждый макрос группы несёт либо факт о коде, либо подсказку оптимизатору.

| Вид | Макросы | Что определяет, где пишется макрос |
|---|---|---|
| Факт о коде | `SCL_DEPRECATED`, `SCL_FALLTHROUGH`, `SCL_LIFETIMEBOUND`, `SCL_MAYBE_UNUSED`, `SCL_NODISCARD`, `SCL_NORETURN`, `SCL_NO_UNIQUE_ADDRESS` | смысл объявления: результат, который нужно прочитать, или функция, которая не возвращает управление |
| Подсказка оптимизатору | `SCL_ASSUME`, `SCL_UNREACHABLE`, `SCL_FORCE_INLINE`, `SCL_NOINLINE`, `SCL_HOT`, `SCL_COLD`, `SCL_INDETERMINATE`, `SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`, `SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE` | замер в том месте, где записана подсказка |

Оптимизатор учитывает подсказку в том месте, где она записана, и на одном компиляторе одна подсказка
может ускорить одно место и замедлить другое. Выигрыш в скорости не обещан ни на одной странице
группы. Выигрыш от подсказки известен только по замеру программы и только для измеренных мест.

Некоторые макросы обоих видов несут обещание, которое компилятор принимает за верное без проверки.
Поведение программы не определено, если такое обещание ложно, а раскрытие содержит атрибут
или подсказку этого обещания, поэтому обещание должно соблюдаться на любом компиляторе. Обещание
каждого такого макроса и строки его таблицы, которые содержат обещание, указаны на его странице.

Макрос `SCL_FORCE_INLINE` является подсказкой, которая к тому же содержит `inline`, поэтому функция,
определённая с ним в заголовке, может быть определена в каждой единице трансляции, куда включён
заголовок.

## Замена макроса

Каждый макрос группы определён под `#ifndef`, поэтому определение, сделанное раньше первого
включённого заголовка модуля, сохраняется и раскрывается в каждом заголовке модуля. Определение
должно быть записано как строка таблицы макроса, условие которой выполнено на данном компиляторе,
или как последняя строка этой таблицы. Сборка с любым другим текстом завершается ошибкой
`static_assert`, в сообщении которой стоит имя макроса. Пробелы учитываются только между двумя
идентификаторами, поэтому `[[ __gnu__::__hot__ ]]` и `[[__gnu__::__hot__]]` являются записями одной
строки.

Определение в командной строке компилятора, например `-DSCL_FORCE_INLINE=inline -DSCL_HOT=`, видно
сразу во всех единицах трансляции. В такой сборке `inline` сохраняется, а снимаются обе подсказки.
`SCL_DEPRECATED_MSG` и `SCL_NODISCARD_MSG` раскрываются в определение `SCL_DEPRECATED`
и `SCL_NODISCARD`, если до заголовка определён только основной макрос, и тогда теряется сообщение.

Каждый макрос должен иметь одно и то же определение во всех единицах трансляции одной программы. Два
определения одного класса, одной встраиваемой функции или одного шаблона, которые отличаются
атрибутом, нарушают правило одного определения, и компилятор не обязан сообщать об этом.

## Макросы вызывающего кода

Не рекомендуется определять макрос с именем лексемы атрибута, которая используется в группе,
например `noinline`, `hot`, `msvc` или имя C23 `unsequenced`.

Раскрытия группы на GCC и Clang не меняются таким макросом. Лексемы GCC и Clang и их пространства
имён записаны с подчёркиваниями, `[[__gnu__::__noinline__]]` и `__attribute__((__hot__))`. Имена C23
`unsequenced` и `reproducible` и имя C++26 `indeterminate` проверяются только там, где они
не являются макросами, а имя `assume` проверяется только после C++20, где оно зарезервировано.

MSVC и ABI MSVC не имеют формы записи атрибута, которая не меняется макросом вызывающего кода,
поэтому сборка завершается с ошибкой в случаях из таблицы ниже, где иначе потерялся бы атрибут.
Макрос группы, который нужно определить пустым до заголовка, указан в сообщении об ошибке, и тогда
атрибут на MSVC не применяется.

| Макрос вызывающего кода | Где сборка завершается с ошибкой | Что определить до заголовка |
|---|---|---|
| `msvc` | для ABI MSVC: на MSVC, clang-cl и Clang | `SCL_NO_UNIQUE_ADDRESS` пустым макросом во всех единицах трансляции программы |
| `msvc` или `lifetimebound` | на MSVC | `SCL_LIFETIMEBOUND` пустым макросом |
| `noinline` | на MSVC | `SCL_NOINLINE` пустым макросом |

Имена стандартных атрибутов, например `nodiscard` или `noreturn`, не должны определяться
как макросы. Исключение составляют имена `likely` и `unlikely`. Их можно определить как макросы
с параметрами, и ни один из них не вызывается в раскрытии `SCL_LIKELY` или `SCL_UNLIKELY`.

## Документация группы

### Макросы

- [`SCL_ASSUME`, `SCL_UNREACHABLE`](assume.md) - условие, которое оптимизатор считает истинным,
  и точка, до которой выполнение не доходит
- [`SCL_DEPRECATED`, `SCL_DEPRECATED_MSG`](deprecated.md) - сущность, при каждом использовании
  которой компилятор выдаёт предупреждение
- [`SCL_FALLTHROUGH`](fallthrough.md) - ветвь `switch`, которая продолжается в следующую
- [`SCL_FORCE_INLINE`, `SCL_NOINLINE`](inline.md) - функция, которую компилятор просят встраивать
  в каждом месте вызова, и функция, которую его просят никогда не встраивать
- [`SCL_HOT`, `SCL_COLD`](hotcold.md) - функция, которая вызывается часто, и функция, которая
  вызывается редко
- [`SCL_INDETERMINATE`](indeterminate.md) - переменная, в которую пишут до каждого чтения и которой
  не нужно ошибочное значение
- [`SCL_LIFETIMEBOUND`](lifetimebound.md) - результат, который не должен жить дольше аргумента
- [`SCL_LIKELY`, `SCL_UNLIKELY`, `SCL_LIKELY_EXPR`, `SCL_UNLIKELY_EXPR`](likely.md) - ветвь, которую
  программа выбирает обычно
- [`SCL_MAYBE_UNUSED`](maybe_unused.md) - сущность, которая в какой-то сборке может остаться
  неиспользованной
- [`SCL_NODISCARD`, `SCL_NODISCARD_MSG`](nodiscard.md) - результат, при отбрасывании которого
  компилятор выдаёт предупреждение
- [`SCL_NORETURN`](noreturn.md) - функция, которая не возвращает управление вызывающему коду
- [`SCL_NO_UNIQUE_ADDRESS`](no_unique_address.md) - пустой член, который может не занимать
  собственной памяти
- [`SCL_UNSEQUENCED`, `SCL_REPRODUCIBLE`](unsequenced.md) - функция, вызовы которой оптимизатор
  может объединять или отбрасывать

### Примеры

- [`example/attribute/assume`](../../../../example/attribute/assume/attribute_assume_example.cpp)
- [`example/attribute/deprecated`](../../../../example/attribute/deprecated/attribute_deprecated_example.cpp)
- [`example/attribute/fallthrough`](../../../../example/attribute/fallthrough/attribute_fallthrough_example.cpp)
- [`example/attribute/hotcold`](../../../../example/attribute/hotcold/attribute_hotcold_example.cpp)
- [`example/attribute/indeterminate`](../../../../example/attribute/indeterminate/attribute_indeterminate_example.cpp)
- [`example/attribute/inline`](../../../../example/attribute/inline/attribute_inline_example.cpp)
- [`example/attribute/lifetimebound`](../../../../example/attribute/lifetimebound/attribute_lifetimebound_example.cpp)
- [`example/attribute/likely`](../../../../example/attribute/likely/attribute_likely_example.cpp)
- [`example/attribute/maybe_unused`](../../../../example/attribute/maybe_unused/attribute_maybe_unused_example.cpp)
- [`example/attribute/no_unique_address`](../../../../example/attribute/no_unique_address/attribute_no_unique_address_example.cpp)
- [`example/attribute/nodiscard`](../../../../example/attribute/nodiscard/attribute_nodiscard_example.cpp)
- [`example/attribute/noreturn`](../../../../example/attribute/noreturn/attribute_noreturn_example.cpp)
- [`example/attribute/unsequenced`](../../../../example/attribute/unsequenced/attribute_unsequenced_example.cpp)

---

Далее: [`SCL_ASSUME`](assume.md) | [К главной странице](../Main.md)
