# Runtime

В группе runtime собраны шаблоны функций, которые во время выполнения программы получают имя класса,
экземпляром которого был создан объект полиморфного класса, и число, которое хранит значение
перечисляемого типа.

- Заголовок: `#include <scl/utility/runtime.h>`

| Вопрос | Группа и шаблон функции | RTTI |
|---|---|---|
| Как называется тип `T`, записанный здесь? | meta: `scl::type_name<T>()` | не требуется |
| Как называется класс, экземпляром которого был создан этот полиморфный объект? | runtime: [`scl::type_name(obj)`](type_name.md) | требуется |
| Как называется константа перечисления `V`? | meta: `scl::enum_name<V>()` | не требуется |
| Какое значение хранит переменная перечисляемого типа? | runtime: [`scl::enum_string(value)`](enum_string.md) | не требуется |

## Без RTTI

Шаблоны функций `type_name(obj)` и `type_short_name(obj)` объявлены только там, где макрос
`SCL_HAS_RTTI` равен `1`. Шаблон функции `enum_string` объявлен и с RTTI, и без него. В коде,
который собирается и с RTTI, и без него, следует проверять этот макрос, определённый всегда,
и при отключённом RTTI брать имя статического типа:

<!-- snippet: example/runtime/type_name/runtime_type_name_example.cpp no_rtti -->
```cpp
template <typename T>
std::string name_of([[maybe_unused]] T const & object)
{
#if SCL_HAS_RTTI
    return scl::type_name(object); // the dynamic type of a polymorphic object
#else
    return std::string{scl::type_name<T>()}; // the static type only
#endif
}
```

## Производительность

Сколько стоит один вызов и какой атрибут оптимизации применён в группе:
[Замеры группы Runtime](benchmark.md).

[К обзору](../Main.md)
