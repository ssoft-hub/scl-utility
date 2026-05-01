# ScL Utility

Набор утилит C++20, отсутствующих в стандартной библиотеке.

ScL Utility — header-only модуль инструментария ScL Toolkit, предоставляющий помощников
для метапрограммирования на этапе компиляции, утилиты препроцессора и расширенные свойства типов (type traits).

## Требования

- Компилятор с поддержкой C++20 (MSVC 19.30+, GCC 13+, Clang 16+)
- CMake 3.20+

## Установка

Добавьте как подкаталог в CMake и свяжите с интерфейсной целью:

```cmake
add_subdirectory(module/utility)
target_link_libraries(your_target PRIVATE scl::utility)
```

Затем подключите общий заголовок или заголовки отдельных компонентов:

```cpp
#include <scl/utility.h>           // всё
#include <scl/utility/meta.h>      // только meta
```

## Компоненты

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

### Meta — рефлексия на этапе компиляции

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [type_name](meta/type_name.md) | `<scl/utility/meta/type.h>` | Извлечение имени типа на этапе компиляции |
| [enum_name](meta/enum_name.md) | `<scl/utility/meta/enum.h>` | Извлечение имени члена перечисления на этапе компиляции |
| [symbol_name](meta/symbol_name.md) | `<scl/utility/meta/symbol.h>` | Извлечение имени функции/члена на этапе компиляции |

### Runtime — утилиты для времени выполнения

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [type_name(obj)](runtime/type_name.md) | `<scl/utility/runtime/type.h>` | Имя типа через RTTI с поддержкой полиморфизма |
| [enum_value(v)](runtime/enum_value.md) | `<scl/utility/runtime/enum.h>` | Значение перечисления как `"TypeName::N"`; RTTI не требуется |

### Hash — некриптографические хеш-утилиты

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [key](hash/key.md) | `<scl/utility/hash.h>` | Строго типизированный хеш-дайджест: метка switch, ключ контейнера, NTTP |

### Preprocessor — макроутилиты

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [SCL_FORWARD](preprocessor/forward.md) | `<scl/utility/preprocessor/forward.h>` | Макрос-идентификатор для проброса токенов |
| [SCL_COUNTER](preprocessor/counter.md) | `<scl/utility/preprocessor/counter.h>` | Счётчик времени компиляции через ADL |

### Type traits — расширенные свойства типов

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [detection](type_traits/detection.md) | `<scl/utility/type_traits/detection.h>` | Идиома обнаружения (is_detected и др.) |
| [forward_like](type_traits/forward_like.md) | `<scl/utility/type_traits/forward_like.h>` | Проброс с категорией значения другого типа |
| [member_like](type_traits/member_like.md) | `<scl/utility/type_traits/member_like.h>` | Декомпозиция свойств указателей на члены |
| [overload_cast](type_traits/overload_cast.md) | `<scl/utility/type_traits/overload_cast.h>` | Разрешение неоднозначности перегруженных функций |

## Быстрый старт

```cpp
#include <scl/utility/meta/type.h>
#include <scl/utility/meta/enum.h>
#include <iostream>

enum class Color { Red, Green, Blue };

int main() {
    std::cout << scl::type_name<std::vector<int>>() << std::endl;
    std::cout << scl::enum_name<Color::Red>() << std::endl;
}
```

## См. также

- [English documentation](../en/Main.md)
