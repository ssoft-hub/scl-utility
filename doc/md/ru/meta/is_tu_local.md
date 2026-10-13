# `is_tu_local<T>`

Шаблон класса `is_tu_local<T>` определяет на этапе компиляции, локален ли тип `T` в своей единице
трансляции, а шаблон переменной `is_tu_local_v<T>` хранит его ответ.

- Заголовок: `#include <scl/utility/meta/tu_local.h>`

```cpp
namespace scl
{
    template <typename T>
    struct is_tu_local;

    template <typename T>
    inline constexpr bool is_tu_local_v = is_tu_local<T>::value;
}
```

## Семантика

- **Значение:** шаблон класса наследует `std::true_type`, если тип `T` без `const` и `volatile`
  объявлен в безымянном пространстве имён или построен из такого типа, и `std::false_type`
  в остальных случаях. Тип, объявленный в безымянном пространстве имён, имеет внутреннее связывание,
  и объявление с тем же именем в другой единице трансляции объявляет другой тип.
- **Типы, построенные из локального типа:** локальными считаются указатель, ссылка и массив
  локального типа. Тип функции и указатель на член считаются локальными, если среди их типов есть
  локальный тип, а специализация шаблона класса с параметрами-типами считается локальной,
  если локален один из её аргументов.
- **Типы, которые классифицируются по имени:** по имени, которое записывает для них компилятор,
  классифицируются специализация шаблона класса с параметром, который не является типом, и тип
  функции с cv-квалификатором, ref-квалификатором или многоточием. Строка или символ, которые хранит
  аргумент шаблона, не считаются частью этого имени.
- **Предусловие:** тип `T` не является локальным классом или типом замыкания и не построен
  из них. Компиляторы записывают такие типы без надёжного признака локальности.

## Примеры

Типы примера:

<!-- snippet: example/meta/is_tu_local/meta_is_tu_local_example.cpp types -->
```cpp
namespace
{
    struct duck
    {};
} // namespace

struct goose
{};
```

Ответы:

<!-- snippet: example/meta/is_tu_local/meta_is_tu_local_example.cpp is_tu_local -->
```cpp
static_assert(scl::is_tu_local_v<duck>);
static_assert(scl::is_tu_local_v<duck const *>);
static_assert(scl::is_tu_local_v<std::vector<duck>>);
static_assert(!scl::is_tu_local_v<goose>);
static_assert(!scl::is_tu_local_v<int>);
```

## Смотрите также

- [`scl::type_key_of<T>()`](type_key.md), ключи которого различают локальные типы с одним именем
- [`example/meta/is_tu_local/meta_is_tu_local_example.cpp`](../../../../example/meta/is_tu_local/meta_is_tu_local_example.cpp)

---

Назад: [`type_name<T>()`](type_name.md) | Далее: [`type_key`](type_key.md) | [К группе](index.md) |
[К главной странице](../Main.md)
