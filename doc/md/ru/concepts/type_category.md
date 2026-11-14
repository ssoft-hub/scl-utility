# Концепты категорий типов

Применение концептов категорий типов позволяет определить, к какой категории относится проверяемый
тип. В стандарте все типы разделены на основные категории `void`, `std::nullptr_t`, целочисленные
типы, типы с плавающей точкой, массивы, указатели, lvalue-ссылки, rvalue-ссылки, указатели на поля,
указатели на методы, перечисления, объединения, классы и функции. Составные категории, например
арифметические типы, скалярные типы и типы объектов, объединяют несколько основных категорий.
Концепты ссылочных типов описаны на странице [Концепты ссылочных типов](reference.md).

- Заголовок: `#include <scl/utility/concepts/type_category.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept enum_type = ::std::is_enum_v<T>;

    template <auto V>
    concept enum_value = ::std::is_enum_v<decltype(V)>;

#if defined(__cpp_lib_is_scoped_enum) && __cpp_lib_is_scoped_enum >= 202011L
    template <typename T>
    concept scoped_enum = ::scl::concepts::enum_type<T> && ::std::is_scoped_enum_v<T>;
#else
    template <typename T>
    concept scoped_enum = ::scl::concepts::enum_type<T> &&
        !::std::is_convertible_v<T, ::std::underlying_type_t<T>>;
#endif

    template <typename T>
    concept void_type = ::std::is_void_v<T>;

    template <typename T>
    concept pointer = ::std::is_pointer_v<T>;

    template <typename T>
    concept null_pointer = ::std::is_null_pointer_v<T>;

    template <typename T>
    concept member_pointer = ::std::is_member_pointer_v<T>;

    template <typename T>
    concept member_object_pointer = ::std::is_member_object_pointer_v<T>;

    template <typename T>
    concept member_function_pointer = ::std::is_member_function_pointer_v<T>;

    template <typename T>
    concept array = ::std::is_array_v<T>;

    template <typename T>
    concept bounded_array = ::std::is_bounded_array_v<T>;

    template <typename T>
    concept unbounded_array = ::std::is_unbounded_array_v<T>;

    template <typename T>
    concept function = ::std::is_function_v<T>;

    template <typename T>
    concept class_type = ::std::is_class_v<T>;

    template <typename T>
    concept union_type = ::std::is_union_v<T>;

    template <typename T>
    concept fundamental = ::std::is_fundamental_v<T>;

    template <typename T>
    concept arithmetic = ::std::is_arithmetic_v<T>;

    template <typename T>
    concept scalar = ::std::is_scalar_v<T>;

    template <typename T>
    concept object = ::std::is_object_v<T>;

    template <typename T>
    concept compound = ::std::is_compound_v<T>;
}
```

## Семантика

### Общие правила

- **Cv-квалификаторы.** Для типа с cv-квалификаторами каждый концепт, кроме концепта `enum_value`,
  даёт тот же результат, что и для типа без них.
- **Ссылки.** Ссылка на тип любой категории сама относится к категории ссылок, поэтому ссылочный тип
  не удовлетворяет ни одному концепту, кроме концепта `compound`.
- **Свойство стандартной библиотеки.** Результат проверки каждого концепта, кроме концептов
  `non_reference` и `enum_value`, совпадает со значением свойства стандартной библиотеки, имя
  которого состоит из префикса `is_` и имени концепта без суффикса `_type`. Например, результат
  концепта `pointer` совпадает со значением свойства `std::is_pointer_v<T>`, а результат концепта
  `enum_type` совпадает со значением свойства `std::is_enum_v<T>`.

### Перечисления

- **`enum_type<T>`.** Проверяемый тип удовлетворяет концепту, если является перечислением,
  с областью видимости или без неё.
- **`scoped_enum<T>`.** Проверяемый тип удовлетворяет концепту, если является перечислением
  с областью видимости, то есть объявлен через `enum class` или `enum struct`. Если стандартная
  библиотека содержит свойство `std::is_scoped_enum` из C++23 (макрос `__cpp_lib_is_scoped_enum`
  не меньше `202011L`), концепт построен на этом свойстве. В противном случае концепт проверяет,
  что перечисление не преобразуется неявно к своему базовому типу. Оба определения дают одинаковый
  результат.

### Константа перечисления

- **Условие.** Концепт `enum_value<V>` проверяет константу, а не тип. Константа удовлетворяет
  концепту, если её тип является перечислением.
- **Параметр шаблона.** Параметр шаблона, объявленный как `scl::concepts::enum_type auto V`,
  допускает те же константы. Концепт `enum_value` предназначен для условия `requires` и для других
  мест, где известна константа, а не её тип.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_enum_v<decltype(V)>`.
- **Выбор перегрузки.** Концепт не объявлен через другие концепты, поэтому его ограничение не строже
  ограничения ни одного другого концепта группы.
- **Ограничение модуля.** Модуль не поддерживает перегрузки, которые различаются только ограничением
  на тип константного параметра шаблона, например `scl::concepts::enum_type auto V`
  и `scl::concepts::scoped_enum auto V`, поскольку для перечисления с областью видимости Clang
  и GCC 16 выбирают перегрузку, ограниченную концептом `scoped_enum`, а более ранние версии GCC
  и MSVC отклоняют такой вызов как неоднозначный.

### `void` и указатели

- **`void_type<T>`.** Проверяемый тип удовлетворяет концепту, если является типом `void`.
- **`pointer<T>`.** Проверяемый тип удовлетворяет концепту, если является указателем на объект,
  на `void` или на функцию. Указатель на член и тип `std::nullptr_t` не удовлетворяют концепту.
- **`null_pointer<T>`.** Проверяемый тип удовлетворяет концепту, если является типом
  `std::nullptr_t`, то есть типом литерала `nullptr`.
- **`member_pointer<T>`.** Проверяемый тип удовлетворяет концепту, если является указателем на поле
  или указателем на метод.
- **`member_object_pointer<T>`.** Проверяемый тип удовлетворяет концепту, если является указателем
  на поле.
- **`member_function_pointer<T>`.** Проверяемый тип удовлетворяет концепту, если является указателем
  на метод, независимо от квалификаторов метода.

### Массивы и функции

- **`array<T>`.** Проверяемый тип удовлетворяет концепту, если является массивом с известной
  границей, например `int[3]`, или с неизвестной границей, например `int[]`. Указатель на элемент
  не удовлетворяет концепту, поскольку не является массивом.
- **`bounded_array<T>`.** Проверяемый тип удовлетворяет концепту, если является массивом с известной
  границей. Например, массив `int[2][3]` является массивом с известной границей, элементы которого
  также являются такими массивами.
- **`unbounded_array<T>`.** Проверяемый тип удовлетворяет концепту, если является массивом
  с неизвестной границей, в том числе типом `int[][3]`.
- **`function<T>`.** Проверяемый тип удовлетворяет концепту, если является типом функции, например
  `void(int)`, в том числе с cv-квалификаторами, ссылочным квалификатором или спецификатором
  `noexcept`. Указатель на функцию и ссылка на функцию не удовлетворяют концепту.

### Классы и объединения

- **`class_type<T>`.** Проверяемый тип удовлетворяет концепту, если является классом,
  но не объединением. Тип замыкания лямбда-выражения также является классом. Объединение
  и перечисление не удовлетворяют концепту.
- **`union_type<T>`.** Проверяемый тип удовлетворяет концепту, если является объединением.

### Составные категории

- **`fundamental<T>`.** Проверяемый тип удовлетворяет концепту, если является арифметическим типом,
  типом `void` или типом `std::nullptr_t`.
- **`arithmetic<T>`.** Проверяемый тип удовлетворяет концепту, если является целочисленным типом
  или типом с плавающей точкой, в том числе типом `bool` или символьным типом. Перечисление
  не удовлетворяет концепту, поскольку не является арифметическим типом.
- **`scalar<T>`.** Проверяемый тип удовлетворяет концепту, если является арифметическим типом,
  перечислением, указателем, указателем на член или типом `std::nullptr_t`.
- **`object<T>`.** Проверяемый тип удовлетворяет концепту, если не является типом функции, ссылочным
  типом или типом `void`. Массив также является типом объекта.
- **`compound<T>`.** Проверяемый тип удовлетворяет концепту, если не является фундаментальным типом,
  то есть является указателем, указателем на член, ссылкой, массивом, функцией, классом,
  объединением или перечислением.

### Выбор перегрузки

- **Правило.** Каждый концепт категории, кроме концепта `scoped_enum`, объявлен только через своё
  свойство, поэтому его ограничение не строже ограничения ни одного другого концепта группы. Концепт
  `scoped_enum` объявлен через концепт `enum_type`, поэтому его ограничение строже ограничения
  концепта `enum_type`, и для перечисления с областью видимости компилятор выбирает шаблон функции,
  ограниченный концептом `scoped_enum`, а не шаблон, ограниченный концептом `enum_type`.
- **Неоднозначный выбор.** Если тип удовлетворяет двум концептам и ни одно из их ограничений
  не строже другого, а для вызова подходят шаблоны функции, ограниченные этими концептами,
  компилятор отклоняет вызов как неоднозначный. Например, так происходит с массивом `int[3]`
  и шаблонами, ограниченными концептами `array` и `bounded_array`.

## Примеры

Типы, используемые в примерах:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp types -->
```cpp
enum plain_color
{
    red,
    green
};

enum class direction : unsigned char
{
    north,
    south
};

struct point
{
    int x;
    int y;
};

union number
{
    int integer;
    float real;
};
```

### Перечисления

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp enum_type -->
```cpp
static_assert(scl::concepts::enum_type<plain_color>);
static_assert(scl::concepts::enum_type<direction const>);
static_assert(!scl::concepts::enum_type<int>);
static_assert(!scl::concepts::enum_type<direction &>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp enum_value -->
```cpp
static_assert(scl::concepts::enum_value<direction::north>);
static_assert(scl::concepts::enum_value<red>);
static_assert(!scl::concepts::enum_value<42>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp scoped_enum -->
```cpp
static_assert(scl::concepts::scoped_enum<direction>);
static_assert(scl::concepts::scoped_enum<direction const>);
static_assert(!scl::concepts::scoped_enum<plain_color>);
static_assert(!scl::concepts::scoped_enum<int>);
```

Получение значения перечисления в его базовом типе. Функция `to_underlying` получает это значение
из аргумента, а шаблонная переменная `underlying_of` получает его из параметра шаблона:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp underlying -->
```cpp
template <scl::concepts::enum_type Enum>
constexpr std::underlying_type_t<Enum> to_underlying(Enum value) noexcept
{
    return static_cast<std::underlying_type_t<Enum>>(value);
}

template <auto Value>
    requires scl::concepts::enum_value<Value>
constexpr auto underlying_of = to_underlying(Value);

static_assert(underlying_of<direction::south> == 1);
```

Выбор перегрузки по виду перечисления. Для перечисления с областью видимости компилятор выбирает
перегрузку, ограниченную концептом `scoped_enum`:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp describe -->
```cpp
template <scl::concepts::enum_type Enum>
std::string_view describe(Enum)
{
    return "enumeration";
}

template <scl::concepts::scoped_enum Enum>
std::string_view describe(Enum)
{
    return "scoped enumeration";
}
```

Вызовы функции `describe`:

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp use -->
```cpp
    std::cout << describe(green) << '\n';            // enumeration
    std::cout << describe(direction::north) << '\n'; // scoped enumeration
```

### `void` и указатели

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp void_type -->
```cpp
static_assert(scl::concepts::void_type<void>);
static_assert(scl::concepts::void_type<void const>);
static_assert(!scl::concepts::void_type<void *>);
static_assert(!scl::concepts::void_type<int>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp pointer -->
```cpp
static_assert(scl::concepts::pointer<int *>);
static_assert(scl::concepts::pointer<int const * const>);
static_assert(scl::concepts::pointer<void *>);
static_assert(scl::concepts::pointer<void (*)(int)>);
static_assert(!scl::concepts::pointer<std::nullptr_t>);
static_assert(!scl::concepts::pointer<int point::*>);
static_assert(!scl::concepts::pointer<int *&>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp null_pointer -->
```cpp
static_assert(scl::concepts::null_pointer<std::nullptr_t>);
static_assert(scl::concepts::null_pointer<std::nullptr_t const>);
static_assert(!scl::concepts::null_pointer<void *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp member_pointer -->
```cpp
static_assert(scl::concepts::member_pointer<int point::*>);
static_assert(scl::concepts::member_pointer<int (point::*)() const noexcept>);
static_assert(!scl::concepts::member_pointer<int *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp member_object_pointer -->
```cpp
static_assert(scl::concepts::member_object_pointer<int point::*>);
static_assert(!scl::concepts::member_object_pointer<int (point::*)() const noexcept>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp member_function_pointer -->
```cpp
static_assert(scl::concepts::member_function_pointer<int (point::*)() const noexcept>);
static_assert(!scl::concepts::member_function_pointer<int point::*>);
static_assert(!scl::concepts::member_function_pointer<void (*)()>);
```

### Массивы и функции

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp array -->
```cpp
static_assert(scl::concepts::array<int[3]>);
static_assert(scl::concepts::array<int[]>);
static_assert(!scl::concepts::array<int *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp bounded_array -->
```cpp
static_assert(scl::concepts::bounded_array<int[3]>);
static_assert(scl::concepts::bounded_array<int[2][3]>);
static_assert(!scl::concepts::bounded_array<int[]>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp unbounded_array -->
```cpp
static_assert(scl::concepts::unbounded_array<int[]>);
static_assert(scl::concepts::unbounded_array<int[][3]>);
static_assert(!scl::concepts::unbounded_array<int[3]>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp function -->
```cpp
static_assert(scl::concepts::function<void(int)>);
static_assert(scl::concepts::function<int(int, double) const noexcept>);
static_assert(!scl::concepts::function<void (*)(int)>);
static_assert(!scl::concepts::function<void (&)(int)>);
```

### Классы и объединения

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp class_type -->
```cpp
static_assert(scl::concepts::class_type<point>);
static_assert(!scl::concepts::class_type<number>);
static_assert(!scl::concepts::class_type<plain_color>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp union_type -->
```cpp
static_assert(scl::concepts::union_type<number>);
static_assert(!scl::concepts::union_type<point>);
```

### Составные категории

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp fundamental -->
```cpp
static_assert(scl::concepts::fundamental<int>);
static_assert(scl::concepts::fundamental<void>);
static_assert(scl::concepts::fundamental<std::nullptr_t>);
static_assert(!scl::concepts::fundamental<int *>);
static_assert(!scl::concepts::fundamental<plain_color>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp arithmetic -->
```cpp
static_assert(scl::concepts::arithmetic<int>);
static_assert(scl::concepts::arithmetic<bool>);
static_assert(scl::concepts::arithmetic<double const>);
static_assert(!scl::concepts::arithmetic<plain_color>);
static_assert(!scl::concepts::arithmetic<int *>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp scalar -->
```cpp
static_assert(scl::concepts::scalar<int>);
static_assert(scl::concepts::scalar<plain_color>);
static_assert(scl::concepts::scalar<int *>);
static_assert(scl::concepts::scalar<int point::*>);
static_assert(scl::concepts::scalar<std::nullptr_t>);
static_assert(!scl::concepts::scalar<point>);
static_assert(!scl::concepts::scalar<int[3]>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp object -->
```cpp
static_assert(scl::concepts::object<int>);
static_assert(scl::concepts::object<int[3]>);
static_assert(scl::concepts::object<point>);
static_assert(!scl::concepts::object<void>);
static_assert(!scl::concepts::object<int &>);
static_assert(!scl::concepts::object<void(int)>);
```

<!-- snippet: example/concepts/type_category/concepts_type_category_example.cpp compound -->
```cpp
static_assert(scl::concepts::compound<int *>);
static_assert(scl::concepts::compound<int &>);
static_assert(scl::concepts::compound<point>);
static_assert(scl::concepts::compound<plain_color>);
static_assert(scl::concepts::compound<void(int)>);
static_assert(!scl::concepts::compound<int>);
static_assert(!scl::concepts::compound<void>);
```

## Смотрите также

- [`example/concepts/type_category/concepts_type_category_example.cpp`](../../../../example/concepts/type_category/concepts_type_category_example.cpp)

---

Назад: [Концепты ссылочных типов](reference.md) |
Далее: [Концепты свойств типов](type_property.md) | [К группе](index.md) |
[К главной странице](../Main.md)
