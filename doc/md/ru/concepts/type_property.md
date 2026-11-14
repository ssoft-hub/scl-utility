# Концепты свойств типов

Применение концептов свойств типов позволяет определить, допускает ли проверяемый тип побайтное
копирование, имеет ли он стандартное размещение, является ли агрегатом, для класса или объединения
также является ли он финальным, а для класса также является ли он пустым, полиморфным
или абстрактным. Эти свойства определены в стандарте
отдельно от категорий типов.

- Заголовок: `#include <scl/utility/concepts/type_property.h>`

```cpp
namespace scl::concepts
{
    template <typename T>
    concept trivially_copyable = ::std::is_trivially_copyable_v<T>;

    template <typename T>
    concept standard_layout = ::std::is_standard_layout_v<T>;

    template <typename T>
    concept empty = ::std::is_empty_v<T>;

    template <typename T>
    concept polymorphic = ::std::is_polymorphic_v<T>;

    template <typename T>
    concept abstract = ::std::is_abstract_v<T>;

    template <typename T>
    concept final = ::std::is_final_v<T>;

    template <typename T>
    concept aggregate = ::std::is_aggregate_v<T>;
}
```

## Семантика

### Общие правила

- **Cv-квалификаторы.** Для типа с cv-квалификаторами каждый концепт даёт тот же результат,
  что и для типа без них.
- **Ссылки.** Ссылочный тип не удовлетворяет ни одному концепту страницы.
- **Полный тип.** Проверяемый тип должен отвечать тем же требованиям, что и аргумент свойства
  стандартной библиотеки, на котором построен концепт. Требование для каждого концепта приведено
  в его подразделе. Код, который использует концепты, не должен проверять ими тип,
  который не отвечает этим требованиям.

### `trivially_copyable<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если его объекты допускают побайтное
  копирование, то есть если тип является скалярным типом, тривиально копируемым классом или массивом
  таких типов.
- **Копирование.** Представление такого объекта можно скопировать функцией `std::memcpy` в другой
  объект того же типа, и после копирования второй объект имеет то же значение. При этом ни один
  из двух объектов не должен быть ни подобъектом базового класса, ни полем, объявленным с атрибутом
  `[[no_unique_address]]`, поскольку такой подобъект может занимать байты другого поля объемлющего
  объекта, и это поле будет перезаписано при копировании.
- **Полный тип.** Тип `std::remove_all_extents_t<T>` должен быть полным типом или типом `void`.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_trivially_copyable_v<T>`.

### `standard_layout<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если имеет стандартное размещение, то есть
  является скалярным типом, классом со стандартным размещением или массивом таких типов. Класс
  с виртуальной функцией или с виртуальным базовым классом не имеет стандартного размещения.
- **Полный тип.** Тип `std::remove_all_extents_t<T>` должен быть полным типом или типом `void`.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_standard_layout_v<T>`.

### `empty<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является пустым классом, то есть
  классом, но не объединением, у которого нет нестатических полей, кроме подобъектов нулевого
  размера, нет виртуальных функций, виртуальных базовых классов и непустых базовых классов.
- **Атрибут `[[no_unique_address]]`.** Пустое поле с этим атрибутом является подобъектом нулевого
  размера только в ABI Itanium, которому GCC и Clang следуют на всех платформах, кроме платформы
  MSVC. В ABI MSVC, которому MSVC и Clang следуют при компиляции для платформы MSVC, атрибут
  не действует, и класс с таким полем не является пустым.
- **Макрос `SCL_NO_UNIQUE_ADDRESS`.** Класс с пустым полем, объявленным с макросом
  [`SCL_NO_UNIQUE_ADDRESS`](../attribute/no_unique_address.md), является пустым в обоих ABI,
  если макрос раскрывается в атрибут `[[no_unique_address]]` для ABI Itanium и в атрибут
  `[[msvc::no_unique_address]]` для ABI MSVC.
- **Полный тип.** Класс, который не является объединением, должен быть полным.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_empty_v<T>`.

### `polymorphic<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является классом, который объявляет
  или наследует виртуальную функцию.
- **Полный тип.** Класс, который не является объединением, должен быть полным.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_polymorphic_v<T>`.

### `abstract<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является классом, у которого есть хотя
  бы одна чисто виртуальная функция, объявленная в самом классе или унаследованная
  и не переопределённая в нём. Объект такого класса существует только как подобъект производного
  класса.
- **Полный тип.** Класс, который не является объединением, должен быть полным.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_abstract_v<T>`.

### `final<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является классом или объединением,
  объявленным со спецификатором `final`. Нельзя унаследовать класс от такого типа.
- **Полный тип.** Класс или объединение должны быть полными.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_final_v<T>`.

### `aggregate<T>`

- **Условие.** Проверяемый тип удовлетворяет концепту, если является агрегатом, то есть массивом
  либо классом или объединением, у которого нет объявленных пользователем и унаследованных
  конструкторов, виртуальных функций, закрытых и защищённых нестатических полей самого класса,
  виртуальных базовых классов, прямых или косвенных, и закрытых и защищённых прямых базовых классов.
- **Полный тип.** Тип должен быть массивом, полным типом или типом `void`.
- **Свойство стандартной библиотеки.** Результат проверки совпадает со значением свойства
  `std::is_aggregate_v<T>`.

### Выбор перегрузки

- **Правило.** Каждый концепт свойства объявлен только через своё свойство, поэтому его ограничение
  не строже ограничения ни одного другого концепта группы.
- **Неоднозначный выбор.** Если тип удовлетворяет двум концептам, а для вызова подходят шаблоны
  функции, ограниченные этими концептами, компилятор отклоняет вызов как неоднозначный. Например,
  так происходит с абстрактным классом и шаблонами, ограниченными концептами `abstract`
  и `polymorphic`.

## Примеры

Типы, используемые в примерах:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp types -->
```cpp
struct sample
{
    int id;
    double value;
};

struct tag
{};

struct shape
{
    virtual ~shape() = default;
    [[nodiscard]]
    virtual double area() const = 0;
};

struct square final : shape
{
    double side = 0.0;

    [[nodiscard]]
    double area() const override
    {
        return side * side;
    }
};

struct counter
{
    counter() = default;
    explicit counter(int start) noexcept
        : count{start}
    {}

    int count = 0;
};
```

Проверка возможности побайтного копирования:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp trivially_copyable -->
```cpp
static_assert(scl::concepts::trivially_copyable<int>);
static_assert(scl::concepts::trivially_copyable<sample>);
static_assert(!scl::concepts::trivially_copyable<std::string>);
static_assert(!scl::concepts::trivially_copyable<square>);
```

Проверка стандартного размещения:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp standard_layout -->
```cpp
static_assert(scl::concepts::standard_layout<int>);
static_assert(scl::concepts::standard_layout<sample>);
static_assert(!scl::concepts::standard_layout<square>);
```

Проверка пустого класса:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp empty -->
```cpp
static_assert(scl::concepts::empty<tag>);
static_assert(!scl::concepts::empty<sample>);
static_assert(!scl::concepts::empty<shape>);
static_assert(!scl::concepts::empty<int>);
```

Проверка полиморфного класса:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp polymorphic -->
```cpp
static_assert(scl::concepts::polymorphic<shape>);
static_assert(scl::concepts::polymorphic<square>);
static_assert(!scl::concepts::polymorphic<sample>);
```

Проверка абстрактного класса:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp abstract -->
```cpp
static_assert(scl::concepts::abstract<shape>);
static_assert(!scl::concepts::abstract<square>);
static_assert(!scl::concepts::abstract<int>);
```

Проверка финального класса:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp final -->
```cpp
static_assert(scl::concepts::final<square>);
static_assert(!scl::concepts::final<shape>);
static_assert(!scl::concepts::final<int>);
```

Проверка агрегата:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp aggregate -->
```cpp
static_assert(scl::concepts::aggregate<sample>);
static_assert(scl::concepts::aggregate<int[3]>);
static_assert(!scl::concepts::aggregate<counter>);
static_assert(!scl::concepts::aggregate<int>);
```

Побайтное копирование объекта, допустимое только для типа, который удовлетворяет концепту
`trivially_copyable`:

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp copy_bytes -->
```cpp
template <scl::concepts::trivially_copyable Value>
void copy_bytes(Value & target, Value const & source) noexcept
{
    std::memcpy(&target, &source, sizeof(Value));
}
```

<!-- snippet: example/concepts/type_property/concepts_type_property_example.cpp use -->
```cpp
    sample const source{7, 2.5};
    sample target{};
    copy_bytes(target, source);
    std::cout << target.id << ' ' << target.value << '\n'; // 7 2.5
```

## Смотрите также

- [`example/concepts/type_property/concepts_type_property_example.cpp`](../../../../example/concepts/type_property/concepts_type_property_example.cpp)

---

Назад: [Концепты категорий типов](type_category.md) | [К группе](index.md) |
[К главной странице](../Main.md)
