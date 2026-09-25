# Runtime

В группе собраны функции, которые получают имена из значения во время выполнения программы:
имя типа, с которым был создан объект полиморфного класса, и число, которое хранится в
значении перечисления. Имена, которые тип имеет на этапе компиляции, возвращают функции
группы [meta](../meta/type_name.md).

- Заголовок: `#include <scl/utility/runtime.h>`

| Страница | Вопрос | RTTI |
|---|---|---|
| [`type_name(obj)`](type_name.md) | какого класса этот объект, в том числе полученный через указатель или ссылку на полиморфный базовый класс | нужен |
| [`enum_string(value)`](enum_string.md) | какое число хранится в этом значении, названном или нет | не нужен |

| Вопрос | Группа |
|---|---|
| Как называется тип `T`, записанный здесь? | meta: `scl::type_name<T>()` |
| Как называется тип, с которым был создан этот полиморфный объект? | runtime: `scl::type_name(obj)` |
| Как называется константа перечисления `V`? | meta: `scl::enum_name<V>()` |
| Какое число хранится в этом значении? | runtime: `scl::enum_string(value)` |

## Без RTTI

`type_name(obj)` и `type_short_name(obj)` объявлены только там, где `SCL_HAS_RTTI` равен
`1`, а `enum_string` объявлена в обеих сборках. В коде, который собирается в обеих сборках,
проверяется этот макрос, который определён всегда; без RTTI берётся имя статического типа:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp no_rtti -->
```cpp
template <typename T>
::std::string name_of(T const & object)
{
#if SCL_HAS_RTTI
    return ::scl::type_name(object); // the dynamic type of a polymorphic object
#else
    static_cast<void>(object);
    return ::std::string{::scl::type_name<T>()}; // the static type only
#endif
}
```

[К обзору](../Main.md)
