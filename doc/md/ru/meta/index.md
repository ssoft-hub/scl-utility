# Meta

В группе meta собраны шаблоны функций, которые на этапе компиляции и без RTTI возвращают имя типа,
имя константы перечисления и имя функции, переменной или члена класса. В группу входят также класс
`scl::type_key`, объект которого идентифицирует тип, и шаблон класса `scl::is_tu_local`, который
определяет, локален ли тип в своей единице трансляции.

- Заголовок: `#include <scl/utility/meta.h>`

| Заголовок | Страница | Объявляет |
|---|---|---|
| `<scl/utility/meta/enum.h>` | [`enum_name<V>()`](enum_name.md) | `scl::enum_name`, `scl::enum_short_name` |
| `<scl/utility/meta/symbol.h>` | [`symbol_name<S>()`](symbol_name.md) | `scl::symbol_name`, `scl::symbol_short_name` |
| `<scl/utility/meta/tu_local.h>` | [`is_tu_local<T>`](is_tu_local.md) | `scl::is_tu_local`, `scl::is_tu_local_v` |
| `<scl/utility/meta/type.h>` | [`type_name<T>()`](type_name.md) | `scl::type_name`, `scl::type_short_name` |
| `<scl/utility/meta/type_key.h>` | [`type_key`](type_key.md) | `scl::type_key`, `scl::type_key_of` |

## Документация группы

### Шаблоны функций

- [`enum_name<V>()`](enum_name.md) - имя константы перечисления
- [`symbol_name<S>()`](symbol_name.md) - имя функции, переменной или члена класса
- [`type_name<T>()`](type_name.md) - полное и короткое имя типа

### Классы и шаблоны классов

- [`is_tu_local<T>`](is_tu_local.md) - локален ли тип в своей единице трансляции
- [`type_key`](type_key.md) - ключ, который идентифицирует тип

### Примеры

- [`example/meta/enum_name`](../../../../example/meta/enum_name/meta_enum_name_example.cpp)
- [`example/meta/is_tu_local`](../../../../example/meta/is_tu_local/meta_is_tu_local_example.cpp)
- [`example/meta/symbol_name`](../../../../example/meta/symbol_name/meta_symbol_name_example.cpp)
- [`example/meta/type_key`](../../../../example/meta/type_key/meta_type_key_example.cpp)
- [`example/meta/type_name`](../../../../example/meta/type_name/meta_type_name_example.cpp)

---

Далее: [`enum_name<V>()`](enum_name.md) | [К главной странице](../Main.md)
