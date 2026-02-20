# Идиома Обнаружения (Detection Idiom)

Реализация идиомы обнаружения для C++20, основанная на предложении
[N4502](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf).

- Заголовок: `#include <scl/utility/type_traits/detection.h>`

## Обзор

Идиома обнаружения предоставляет чистый и стандартизированный способ проверки
корректности выражений на этапе компиляции с использованием техники SFINAE
(Substitution Failure Is Not An Error — ошибка подстановки не является ошибкой).
Это особенно полезно для: 

- Обнаружения функций-членов, типов или переменных
- Реализации ограничений, подобных концептам, до появления концептов в C++20
- Написания обобщённого кода, адаптирующегося к свойствам типов
- Определения свойств типов и интроспекции

## Возможности

- Полное соответствие стандарту N4502
- Совместимость с C++20
- Библиотека, состоящая только из заголовочных файлов
- Нулевые накладные расходы во время выполнения

## Справочник API

### Базовое обнаружение

#### `is_detected<Operation, Arguments... >`

Проверяет, является ли `Operation<Arguments...>` корректным выражением.

**Возвращает:** `std::true_type` или `std::false_type`

```cpp
template <typename T>
using has_size_t = decltype(std:: declval<T>().size());

static_assert(scl::is_detected<has_size_t, std::vector<int>>::value);
```

#### `is_detected_v<Operation, Arguments...>` (C++17)

Вариабельный шаблон для `is_detected`.

**Возвращает:** `bool`

```cpp
if constexpr (scl::is_detected_v<has_size_t, MyType>) {
    // Тип имеет член size()
}
```

### Извлечение типов

#### `detected_t<Operation, Arguments... >`

Извлекает тип `Operation<Arguments...>` или возвращает `void`, если выражение некорректно.

```cpp
template <typename T>
using value_type_t = typename T::value_type;

using VecValueType = scl::detected_t<value_type_t, std::vector<int>>;
// VecValueType — это int
```

#### `detected_or_t<Default, Operation, Arguments...>`

Извлекает тип `Operation<Arguments...>` или возвращает `Default`, если выражение некорректно.

```cpp
using ValueType = scl::detected_or_t<void, value_type_t, MyType>;
// ValueType — это MyType:: value_type, если он существует, иначе void
```

### Расширенное обнаружение

#### `is_detected_exact<Expected, Operation, Arguments...>`

Проверяет, что `Operation<Arguments...>` возвращает в точности тип `Expected`.

```cpp
static_assert(
    scl::is_detected_exact_v<int, value_type_t, std::vector<int>>
);
```

#### `is_detected_convertible<To, Operation, Arguments...>`

Проверяет, что `Operation<Arguments...>` приводится к типу `To`.

```cpp
static_assert(
    scl::is_detected_convertible_v<double, value_type_t, std::vector<int>>
);
```

## Примеры использования

### Пример 1: Обнаружение функций-членов

```cpp
template <typename T>
using has_serialize_t = decltype(std::declval<T>().serialize());

template <typename T>
void process(const T& obj) {
    if constexpr (scl::is_detected_v<has_serialize_t, T>) {
        obj.serialize();
    } else {
        // Запасное поведение
    }
}
```

### Пример 2: Обнаружение типов-членов

```cpp
template <typename T>
using value_type_t = typename T:: value_type;

template <typename Container>
auto sum(const Container& c) {
    using ValueType = scl::detected_or_t<int, value_type_t, Container>;
    ValueType result{};
    for (const auto& elem : c) {
        result += elem;
    }
    return result;
}
```

### Пример 3: Определение типовых свойств

```cpp
template <typename T>
using has_begin_end_t = decltype(
    std::declval<T>().begin(),
    std::declval<T>().end()
);

template <typename T>
inline constexpr bool is_iterable_v = scl::is_detected_v<has_begin_end_t, T>;

template <typename T>
requires is_iterable_v<T>
void print(const T& container) {
    for (const auto& elem : container) {
        std::cout << elem << " ";
    }
}
```

### Пример 4: Обнаружение операторов

```cpp
template <typename T, typename U>
using has_plus_t = decltype(std::declval<T>() + std::declval<U>());

template <typename T, typename U>
inline constexpr bool is_addable_v = scl::is_detected_v<has_plus_t, T, U>;

template <typename T, typename U>
auto safe_add(const T& a, const U& b) {
    if constexpr (is_addable_v<T, U>) {
        return a + b;
    } else {
        return T{};
    }
}
```

### Пример 5: Ограничения в стиле концептов (до C++20)

```cpp
template <typename T>
using is_container = std::conjunction<
    scl::is_detected<has_begin_end_t, T>,
    scl::is_detected<value_type_t, T>,
    scl::is_detected<has_size_t, T>
>;

template <typename T>
inline constexpr bool is_container_v = is_container<T>::value;

template <typename T, std::enable_if_t<is_container_v<T>, int> = 0>
void process_container(const T& container) {
    // Реализация
}
```

## Производительность

Идиома обнаружения — это средство **исключительно времени компиляции**,
для которого характерно:

- Нулевые накладные расходы во время выполнения
- Отсутствие генерации кода
- Полное разрешение на этапе компиляции

## Сравнение с концептами C++20

Хотя концепты C++20 предоставляют более мощный и выразительный способ наложения 
ограничений на шаблоны, идиома обнаружения остаётся полезной для: 

- **Интроспекции типов**: извлечение типов с помощью `detected_t`
- **Обратной совместимости**: поддержка кодовых баз до C++20
- **Метапрограммирования**: сложные паттерны SFINAE

```cpp
// Версия с концептами (C++20)
template <typename T>
concept HasSize = requires(T t) {
    { t.size() } -> std::convertible_to<std::size_t>;
};

// Версия с идиомой обнаружения
template <typename T>
using has_size_t = decltype(std::declval<T>().size());

template <typename T>
inline constexpr bool has_size_v = 
    scl::is_detected_convertible_v<std::size_t, has_size_t, T>;
```

## Ссылки

- [N4502 — The Detection Idiom](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/n4502.pdf)
- [Library Fundamentals TS v2](https://en.cppreference.com/w/cpp/experimental/lib_extensions_2)
