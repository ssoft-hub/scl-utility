# ScL.Utility

Набор утилит C++20, отсутствующих в стандартной библиотеке.

ScL.Utility — header-only модуль инструментария ScL Toolkit, предоставляющий помощников
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

### Meta — рефлексия на этапе компиляции

| Компонент | Заголовок | Описание |
|-----------|-----------|----------|
| [type_name](meta/type_name.md) | `<scl/utility/meta/type.h>` | Извлечение имени типа на этапе компиляции |
| [enum_name](meta/enum_name.md) | `<scl/utility/meta/enum.h>` | Извлечение имени члена перечисления на этапе компиляции |
| [symbol_name](meta/symbol_name.md) | `<scl/utility/meta/symbol.h>` | Извлечение имени функции/члена на этапе компиляции |

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
