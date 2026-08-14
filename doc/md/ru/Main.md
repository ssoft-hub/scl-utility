# ScL Utility

Набор утилит C++20, отсутствующих в стандартной библиотеке.

ScL Utility — header-only модуль инструментария ScL Toolkit. Он предоставляет стирающие тип
представления над произвольным значением, портабельные макросы атрибутов, концепты для
классификации типов, множество значений scoped enum, некриптографические хеш-функции,
иерархии «родитель-потомок», извлечение имён на этапе компиляции и во время выполнения,
утилиты препроцессора и расширенные свойства типов (type traits).

## Требования

- Компилятор с поддержкой C++20 (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.23+, и только для сборки тестов, примеров или устанавливаемого пакета —
  использование заголовков сборки не требует

## Установка

Модуль header-only: достаточно добавить его каталог `src` в пути поиска заголовков.

```cmake
target_include_directories(your_target PRIVATE path/to/scl-utility/src)
```

Чтобы получить цель `scl::utility`, возьмите её из установленного инструментария:

```cmake
find_package(scl REQUIRED COMPONENTS utility)
target_link_libraries(your_target PRIVATE scl::utility)
```

Та же цель доступна из супер-проекта, собираемого в составе вашего дерева, — он собирает
модуль вместе с соседними:

```cmake
add_subdirectory(path/to/scl-kit)             # или FetchContent_MakeAvailable(scl-kit)
target_link_libraries(your_target PRIVATE scl::utility)
```

Собственного `CMakeLists.txt` у `module/utility` нет — точка входа CMake это
`project/cmake/`, который супер-проект подключает для каждого модуля.

Затем подключите общий заголовок или заголовки отдельных компонентов:

```cpp
#include <scl/utility.h>           // все группы
#include <scl/utility/meta.h>      // только meta
```

## Компоненты

### Any — один стёртый объект: во владении или по месту

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [any](any/any.md) | `<scl/utility/any/any.h>` | Владелец одного объекта любого типа; без RTTI, работает на этапе компиляции, шириной в два указателя |
| [any_view](any/any_view.md) | `<scl/utility/any/any_view.h>` | Хранимое read-only представление над `std::any` или типизированным lvalue; принимает только lvalue |
| [any_arg](any/any_arg.md) | `<scl/utility/any/any_arg.h>` | Представление для позиции параметра, принимает и rvalue; действительно в пределах вызова |
| [any_switch](any/any_switch.md) | `<scl/utility/any/any_switch.h>` | Цепочка ветвей: выполняет одну ветвь, выбранную по фактическому типу значения; само значение не хранит |

### Attribute — портабельные макросы атрибутов компилятора

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [SCL_NO_UNIQUE_ADDRESS](attribute/no_unique_address.md) | `<scl/utility/attribute/no_unique_address.h>` | Портабельный `[[no_unique_address]]` для MSVC/GCC/Clang |
| [SCL_ASSUME / SCL_UNREACHABLE](attribute/assume.md) | `<scl/utility/attribute/assume.h>` | Подсказки оптимизатору: предположение и недостижимый код |
| [SCL_FORCE_INLINE / SCL_NOINLINE](attribute/inline.md) | `<scl/utility/attribute/inline.h>` | Управление встраиванием функций |
| [SCL_NORETURN](attribute/noreturn.md) | `<scl/utility/attribute/noreturn.h>` | Пометка функции, не возвращающей управление |
| [SCL_DEPRECATED / SCL_DEPRECATED_MSG](attribute/deprecated.md) | `<scl/utility/attribute/deprecated.h>` | Аннотации устаревших сущностей |
| [SCL_FALLTHROUGH](attribute/fallthrough.md) | `<scl/utility/attribute/fallthrough.h>` | Пометка намеренного провала между ветками switch |
| [SCL_NODISCARD / SCL_NODISCARD_MSG](attribute/nodiscard.md) | `<scl/utility/attribute/nodiscard.h>` | Предупреждение об игнорировании возвращаемого значения |
| [SCL_MAYBE_UNUSED](attribute/maybe_unused.md) | `<scl/utility/attribute/maybe_unused.h>` | Подавление предупреждений о неиспользуемых сущностях |
| [SCL_LIKELY / SCL_UNLIKELY / SCL_LIKELY_EXPR / SCL_UNLIKELY_EXPR](attribute/likely.md) | `<scl/utility/attribute/likely.h>` | Подсказки предсказателю ветвлений |
| [SCL_UNSEQUENCED / SCL_REPRODUCIBLE](attribute/unsequenced.md) | `<scl/utility/attribute/unsequenced.h>` | Аннотации чистых функций (C++26 / gnu::const / gnu::pure) |
| [SCL_INDETERMINATE](attribute/indeterminate.md) | `<scl/utility/attribute/indeterminate.h>` | Явный выбор неопределённого хранилища (C++26) |
| [SCL_HOT / SCL_COLD](attribute/hotcold.md) | `<scl/utility/attribute/hotcold.h>` | Подсказки о частоте вызова функции |
| [SCL_LIFETIMEBOUND](attribute/lifetimebound.md) | `<scl/utility/attribute/lifetimebound.h>` | Обнаружение висячих ссылок через аннотацию времени жизни параметра |

### Concepts — концепты для стандартных свойств типов

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| `reference`, `lvalue_reference`, `rvalue_reference`, `non_reference` | `<scl/utility/concepts/reference.h>` | Классификаторы ссылок |
| `const_type`, `volatile_type`, `cv_type` | `<scl/utility/concepts/qualifier.h>` | Классификаторы квалификаторов |
| `enum_type` и другие классификаторы категорий | `<scl/utility/concepts/type_category.h>` | Классификаторы категорий типов |
| Классификаторы свойств типов | `<scl/utility/concepts/type_property.h>` | Классификаторы свойств |

### Hierarchy — иерархии «родитель-потомок»

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [node](hierarchy/node.md) | `<scl/utility/hierarchy/node.h>` | Узел дерева, владеющий потомками; перенос между родителями за O(1) |
| [tree](hierarchy/tree.md) | `<scl/utility/hierarchy/tree.h>` | Список узлов верхнего уровня, уведомляющий наблюдателя о каждом изменении |
| [observer_tuple](hierarchy/observer_tuple.md) | `<scl/utility/hierarchy/observer_tuple.h>` | Адаптер, объединяющий несколько наблюдателей в одного |
| [algorithm](hierarchy/algorithm.md) | `<scl/utility/hierarchy/algorithm.h>` | `is_parent_of`, `is_ancestor_of`, `are_sibling` и другие; адаптируются через ADL |

### Meta — рефлексия на этапе компиляции

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [type_name](meta/type_name.md) | `<scl/utility/meta/type.h>` | Извлечение имени типа на этапе компиляции |
| [enum_name](meta/enum_name.md) | `<scl/utility/meta/enum.h>` | Извлечение имени члена перечисления на этапе компиляции |
| [symbol_name](meta/symbol_name.md) | `<scl/utility/meta/symbol.h>` | Извлечение имени функции/члена на этапе компиляции |
| [type_key](meta/type_key.md) | `<scl/utility/meta/type_key.h>` | Ключ идентичности типа без RTTI, различающий TU-локальные типы |
| [is_tu_local](meta/type_key.md#is_tu_local) | `<scl/utility/meta/tu_local.h>` | Определение типов, объявленных в анонимном пространстве имён |

### Runtime — утилиты для времени выполнения

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [type_name(obj)](runtime/type_name.md) | `<scl/utility/runtime/type.h>` | Имя типа через RTTI с поддержкой полиморфизма |
| [enum_value(v)](runtime/enum_value.md) | `<scl/utility/runtime/enum.h>` | Значение перечисления как `"TypeName::N"`; RTTI не требуется |

### Hash — некриптографические хеш-утилиты

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [key](hash/key.md) | `<scl/utility/hash.h>` | Строго типизированное хеш-значение: метка switch, ключ контейнера, NTTP |

### Flags — типобезопасное множество значений scoped enum

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [flags](flags/flags.md) | `<scl/utility/flags.h>` | Множество значений scoped enum: алгебра множеств, предикаты и двунаправленный диапазон по хранимым значениям |

### Preprocessor — макроутилиты

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [SCL_FORWARD](preprocessor/forward.md) | `<scl/utility/preprocessor/forward.h>` | Макрос-идентификатор для проброса токенов |
| [SCL_COUNTER](preprocessor/counter.md) | `<scl/utility/preprocessor/counter.h>` | Счётчик времени компиляции через ADL |
| [SCL_HAS_RTTI](preprocessor/rtti.md) | `<scl/utility/preprocessor/rtti.h>` | Есть ли в единице трансляции RTTI и что от этого зависит |
| [SCL_HAS_EXCEPTIONS](preprocessor/exceptions.md) | `<scl/utility/preprocessor/exceptions.h>` | Есть ли в единице трансляции исключения и что от этого зависит |

### Type traits — расширенные свойства типов

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [detection](type_traits/detection.md) | `<scl/utility/type_traits/detection.h>` | Идиома обнаружения (is_detected и др.) |
| [forward_like](type_traits/forward_like.md) | `<scl/utility/type_traits/forward_like.h>` | Проброс с категорией значения другого типа |
| [member_like](type_traits/member_like.md) | `<scl/utility/type_traits/member_like.h>` | Декомпозиция свойств указателей на члены |
| [overload_cast](type_traits/overload_cast.md) | `<scl/utility/type_traits/overload_cast.h>` | Разрешение неоднозначности перегруженных функций |

## Быстрый старт

Шесть программ, по одной на тему, которая читается в несколько строк. Каждая собирается как
пример, поэтому ниже приведён код, который компилируется, а не его пересказ.

Имена типов и констант перечислений на этапе компиляции, без RTTI
([`example/quick_start/meta`](../../../example/quick_start/meta/quick_start_meta_example.cpp)):

<!-- snippet: example/quick_start/meta/quick_start_meta_example.cpp -->
```cpp
#include <scl/utility/meta/enum.h>
#include <scl/utility/meta/type.h>

#include <iostream>

enum class color
{
    red,
    green,
};

int main()
{
    static_assert(::scl::type_name<int>() == "int");
    static_assert(::scl::enum_short_name<color::green>() == "green");

    ::std::cout << ::scl::type_name<color>() << '\n';        // color; MSVC: enum color
    ::std::cout << ::scl::enum_name<color::green>() << '\n'; // color::green
}
```

Строка как метка `switch` и как параметр шаблона
([`example/quick_start/hash`](../../../example/quick_start/hash/quick_start_hash_example.cpp)):

<!-- snippet: example/quick_start/hash/quick_start_hash_example.cpp -->
```cpp
#include <scl/utility/hash/key.h>

#include <iostream>
#include <string_view>

using ::scl::hash::key;

int code_of(::std::string_view command)
{
    switch (key<>{command})
    {
    case key<>{"start"}:
        return 1;
    case key<>{"stop"}:
        return 2;
    default:
        return 0;
    }
}

template <key<> Command>
constexpr bool is_start = (Command == key<>{"start"});

static_assert(is_start<key<>{"start"}>);

int main()
{
    ::std::cout << code_of("start") << code_of("stop") << code_of("pause") << '\n'; // 120
}
```

Типобезопасное множество значений scoped enum
([`example/quick_start/flags`](../../../example/quick_start/flags/quick_start_flags_example.cpp)):

<!-- snippet: example/quick_start/flags/quick_start_flags_example.cpp -->
```cpp
#include <scl/utility/flags.h>

#include <iostream>

enum class permission
{
    read,
    write,
    execute,
};

using permissions = ::scl::flags<permission>;

constexpr permissions read_write{permission::read, permission::write};

static_assert(read_write.all_of(permission::read, permission::write));
static_assert(read_write.none_of(permission::execute));

int main()
{
    permissions granted = read_write;
    granted |= permission::execute;

    ::std::cout
        << granted[permission::read]    // 1
        << granted[permission::execute] // 1
        << granted.size() << '\n';      // 3
}
```

Одна функция, принимающая аргумент любого типа, и одна цепочка, выбирающая ветвь
([`example/quick_start/any`](../../../example/quick_start/any/quick_start_any_example.cpp)):

<!-- snippet: example/quick_start/any/quick_start_any_example.cpp -->
```cpp
#include <scl/utility/any.h>

#include <iostream>
#include <string>

using ::std::string;

string from_int(int number) { return "int " + ::std::to_string(number); }
string from_string(string const & text) { return "string " + text; }
string from_other(::scl::any_arg value) { return "other " + string{value.type_name()}; }

// The chain holds no subject: build it once, apply it to whatever turns up.
auto const describe =
    ::scl::any_switch<string>() //
        .in_case<int>(from_int)
        .in_case<string const &>(from_string)
        .or_else(from_other);

void print(::scl::any_arg value) { ::std::cout << *describe.apply(value) << '\n'; }

int main()
{
    print(string{"text"}); // string text
    print(42);             // int 42
    print(2.5);            // other double
}
```

Дерево «родитель-потомок»: связи остаются доступными для запроса, а изменения
доходят до наблюдателя
([`example/quick_start/hierarchy`](../../../example/quick_start/hierarchy/quick_start_hierarchy_example.cpp)):

<!-- snippet: example/quick_start/hierarchy/quick_start_hierarchy_example.cpp -->
```cpp
#include <scl/utility/hierarchy.h>

#include <iostream>
#include <string>

using person = ::scl::hierarchy::node<::std::string>;

// Every insertion, erasure and payload change reaches the observer the tree carries.
template <typename Tree>
struct headcount
{
    int size = 0;

    void on_insert(Tree::iterator) { ++size; }
    void on_erase(Tree::const_iterator) { --size; }
    void on_clear() { size = 0; }
    void on_change(Tree::const_payload_reference, Tree::const_payload_reference) {}
};

using company = ::scl::hierarchy::tree<::std::string, headcount>;

int main()
{
    person lead{"lead"};
    person & senior = *lead.emplace_back("senior");
    senior.emplace_back("junior");

    person const & junior = senior.front();
    ::std::cout
        << ::std::boolalpha                                     //
        << ::scl::hierarchy::is_parent_of(senior, junior)       // true
        << ::scl::hierarchy::is_ancestor_of(lead, junior)       // true
        << ::scl::hierarchy::is_parent_of(lead, junior) << '\n' // false
        ;

    company staff;
    company::reference root = *staff.push_back("lead");
    root.push_back("senior");
    ::std::cout << staff.get_observer().size << '\n'; // 2
}
```

Выбор перегрузки и перенос категории значения
([`example/quick_start/type_traits`](../../../example/quick_start/type_traits/quick_start_type_traits_example.cpp)):

<!-- snippet: example/quick_start/type_traits/quick_start_type_traits_example.cpp -->
```cpp
#include <scl/utility/type_traits/forward_like.h>
#include <scl/utility/type_traits/overload_cast.h>

#include <iostream>
#include <type_traits>

struct widget
{
    void update(int) { ::std::cout << "update(int)\n"; }
    void update(double) { ::std::cout << "update(double)\n"; }
};

// The value category and cv-qualification of the owner, applied to a member type.
static_assert(::std::is_same_v<::scl::forward_like_t<widget const &, int>, int const &>);
static_assert(::std::is_same_v<::scl::forward_like_t<widget &&, int>, int &&>);

int main()
{
    auto const update_int = ::scl::overload_cast<int>(&widget::update);

    widget instance;
    (instance.*update_int)(1); // update(int)
}
```

## См. также

- [English documentation](../en/Main.md)
