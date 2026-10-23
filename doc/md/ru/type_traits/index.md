# Type traits

Шаблоны группы type traits предназначены для работы с типами и выражениями во время компиляции
программы.

Использование шаблонов обнаружения позволяет узнать, допустимо ли выражение с аргументами заданных
типов, можно ли вызвать функцию или метод с данным именем, применим ли оператор к операндам и какой
тип имеет результат. Любое выражение проверяется через операцию, записанную в шаблонном типе.
Для вызова функции и метода шаблоны проверки объявляются макросами, а для каждого оператора они уже
объявлены в заголовке `operator.h`.

Использование остальных шаблонов группы позволяет привести значение к категории и квалификаторам
другого типа, построить тип указателя на член по типу объекта, выбрать одну перегрузку функции
или метода и определить части сигнатуры вызываемого типа.

- Заголовок: `#include <scl/utility/type_traits.h>`

| Заголовок | Страница | Объявляет |
|---|---|---|
| `<scl/utility/type_traits/detection.h>` | [Обнаружение наличия выражения](detection.md) | `scl::is_detected`, `scl::detected_t`, `scl::detected_or`, `scl::is_detected_exact`, `scl::is_detected_convertible`, `scl::nonesuch` |
| `<scl/utility/type_traits/detection/function.h>` | [Обнаружение наличия функции](function_detection.md) | `SCL_FUNCTION_DETECTION` |
| `<scl/utility/type_traits/detection/method.h>` | [Обнаружение наличия метода](method_detection.md) | `SCL_METHOD_DETECTION` |
| `<scl/utility/type_traits/detection/operator.h>` | [Обнаружение наличия оператора](operator_detection.md) | `scl::has_plus_v`, `scl::plus_t`, `scl::has_plus_member_v` и такие же шаблоны для каждого оператора |
| `<scl/utility/type_traits/forward_like.h>` | [Приведение значения к категории и cv-квалификаторам заданного типа](forward_like.md) | `scl::forward_like`, `scl::forward_like_t` |
| `<scl/utility/type_traits/member_like.h>` | [Формирование типа указателя на член по типу объекта](member_like.md) | `scl::member_like_t`, `scl::member_function_like_t`, `scl::member_object_like_t` |
| `<scl/utility/type_traits/overload_cast.h>` | [Выбор перегрузки функции или метода по типам параметров](overload_cast.md) | `scl::overload_cast` |
| `<scl/utility/type_traits/signature.h>` | [Определение сигнатуры вызываемого типа](signature.md) | `scl::signature_t`, `scl::signature::result_t` и остальные шаблоны пространства имён `scl::signature` |

## Документация группы

### Обнаружение

- [Обнаружение наличия выражения](detection.md) - проверка того, допустимо ли выражение, записанное
  в шаблонном типе, и определение типа его результата
- [Обнаружение наличия функции](function_detection.md) - проверка того, можно ли вызвать функцию
  с данным именем с аргументами заданных типов и объявлена ли она с такими параметрами
- [Обнаружение наличия метода](method_detection.md) - те же проверки для метода с данным именем
  и объекта заданного типа
- [Обнаружение наличия оператора](operator_detection.md) - проверка того, применим ли оператор
  к операндам заданных типов и объявлен ли он методом класса

### Построение типов

- [Приведение значения к категории и cv-квалификаторам заданного типа](forward_like.md) - ссылка
  на аргумент с видом ссылки и cv-квалификаторами другого типа
- [Формирование типа указателя на член по типу объекта](member_like.md) - тип указателя на метод
  или на поле с квалификаторами, взятыми из типа объекта
- [Выбор перегрузки функции или метода по типам параметров](overload_cast.md) - указатель
  на перегрузку, параметры которой имеют в точности заданные типы
- [Определение сигнатуры вызываемого типа](signature.md) - тип результата, типы параметров,
  квалификаторы и другие свойства функций, методов и лямбда-выражений

### Примеры

- [`example/type_traits/detection`](../../../../example/type_traits/detection/type_traits_detection_example.cpp)
- [`example/type_traits/forward_like`](../../../../example/type_traits/forward_like/type_traits_forward_like_example.cpp)
- [`example/type_traits/function_detection`](../../../../example/type_traits/function_detection/type_traits_function_detection_example.cpp)
- [`example/type_traits/member_like`](../../../../example/type_traits/member_like/type_traits_member_like_example.cpp)
- [`example/type_traits/method_detection`](../../../../example/type_traits/method_detection/type_traits_method_detection_example.cpp)
- [`example/type_traits/operator_detection`](../../../../example/type_traits/operator_detection/type_traits_operator_detection_example.cpp)
- [`example/type_traits/overload_cast`](../../../../example/type_traits/overload_cast/type_traits_overload_cast_example.cpp)
- [`example/type_traits/signature`](../../../../example/type_traits/signature/type_traits_signature_example.cpp)

---

Далее: [Обнаружение наличия выражения](detection.md) | [К главной странице](../Main.md)
