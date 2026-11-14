# Concepts

Применение концептов группы позволяет определить во время компиляции программы, обладает ли тип
свойством, и ограничить этим свойством параметр шаблона. Каждый концепт группы соответствует
свойству типа из стандартной библиотеки, для которого в ней нет своего концепта. Концепты
для свойств, у которых такой концепт есть, например для свойств `std::is_integral`
и `std::is_floating_point`, не входят в группу.

Концепт указывается при объявлении параметра шаблона, например
`template <scl::concepts::pointer Pointer>` или `scl::concepts::pointer auto`. Нельзя так указать
свойство стандартной библиотеки с суффиксом `_v`, поскольку оно является значением типа `bool`,
а не концептом. Такое свойство записывается в условии `requires` шаблона, например
`template <typename T> requires std::is_pointer_v<T>`.

Большинство концептов объявлено только через своё свойство, поэтому ограничение такого концепта
не строже ограничения ни одного другого концепта группы. Если тип удовлетворяет двум таким
концептам, а для вызова подходят шаблоны функции, ограниченные этими концептами, компилятор
отклоняет вызов как неоднозначный. Концепт `scoped_enum` объявлен через концепт `enum_type`,
а концепт `cv_qualified` через концепты `const_qualified` и `volatile_qualified`, поэтому для типа,
который удовлетворяет такому концепту, компилятор выбирает шаблон функции, ограниченный этим
концептом, а не шаблон, ограниченный концептом, через который объявлен этот концепт. На каждой
странице группы описан выбор перегрузки для концептов её заголовочного файла.

Имя концепта совпадает с именем соответствующего свойства без префикса `is_`, как имена концептов
`std::integral` и `std::floating_point` стандартной библиотеки. Например, концепт
`scl::concepts::pointer` соответствует свойству `std::is_pointer_v<T>`. Если имя без префикса
совпадает с ключевым словом языка, к нему добавляется суффикс `_type`, отсюда имена `enum_type`,
`void_type`, `class_type` и `union_type`. Исключение составляют концепты cv-квалификаторов, имя
каждого из которых состоит из обозначения квалификатора и суффикса `_qualified`, например
`const_qualified`, `volatile_qualified` и `cv_qualified`. Для трёх концептов результат проверки
не совпадает со значением одного свойства для проверяемого типа. Концепт `cv_qualified` объявлен
через концепты `const_qualified` и `volatile_qualified`, концепт `non_reference` записан
как отрицание свойства `std::is_reference_v`, а концепт `enum_value` проверяет свойством
`std::is_enum_v` тип константы.

Все концепты группы объявлены в пространстве имён `scl::concepts`.

- Заголовок: `#include <scl/utility/concepts.h>`

| Заголовочный файл | Страница | Объявляет |
|---|---|---|
| `<scl/utility/concepts/qualifier.h>` | [Концепты cv-квалификаторов](qualifier.md) | `const_qualified`, `volatile_qualified`, `cv_qualified` |
| `<scl/utility/concepts/reference.h>` | [Концепты ссылочных типов](reference.md) | `reference`, `lvalue_reference`, `rvalue_reference`, `non_reference` |
| `<scl/utility/concepts/type_category.h>` | [Концепты категорий типов](type_category.md) | `enum_type`, `enum_value`, `scoped_enum`, `void_type`, `pointer`, `null_pointer`, `member_pointer`, `member_object_pointer`, `member_function_pointer`, `array`, `bounded_array`, `unbounded_array`, `function`, `class_type`, `union_type`, `fundamental`, `arithmetic`, `scalar`, `object`, `compound` |
| `<scl/utility/concepts/type_property.h>` | [Концепты свойств типов](type_property.md) | `trivially_copyable`, `standard_layout`, `empty`, `polymorphic`, `abstract`, `final`, `aggregate` |

## Документация группы

### Концепты

- [Концепты cv-квалификаторов](qualifier.md) - проверка квалификаторов `const` и `volatile` у самого
  типа
- [Концепты ссылочных типов](reference.md) - проверка того, является ли тип ссылкой,
  и если является, то какого вида
- [Концепты категорий типов](type_category.md) - проверка основной или составной категории типа
- [Концепты свойств типов](type_property.md) - проверка свойств типа, которые определены в стандарте
  отдельно от категорий

### Примеры

- [`example/concepts/qualifier`](../../../../example/concepts/qualifier/concepts_qualifier_example.cpp)
- [`example/concepts/reference`](../../../../example/concepts/reference/concepts_reference_example.cpp)
- [`example/concepts/type_category`](../../../../example/concepts/type_category/concepts_type_category_example.cpp)
- [`example/concepts/type_property`](../../../../example/concepts/type_property/concepts_type_property_example.cpp)

---

Далее: [Концепты cv-квалификаторов](qualifier.md) | [К главной странице](../Main.md)
