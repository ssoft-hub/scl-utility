# Forward-like типажи

Это набор вспомогательных инструментов (типажей) для работы с типами в C++.

Они позволяют "пробрасывать" (копировать) квалификаторы типа, такие как const,
volatile и тип ссылки (& или &&), от одного типа объекта к другому.

- Заголовочный файл: `#include <scl/utility/type_traits/forward_like.h>`

Содержание:
- [`forward_like_t<Base, Type>`](#forward_like_tbase-type)
- [`forward_like<Base, Type>`](#forward_likebase-type)

---

## forward_like_t<Base, Type>

Создаёт новый тип, применяя к `Type` cv/ref-семантику из `Base` в соответствии с
правилами C++23 `std::forward_like`.

Данный типаж модифицирует `Type` путём **наложения** `const`/`volatile` квалификаторов
из `Base` (сохраняя собственные квалификаторы `Type`) и полной **замены** ссылочной
категории `Type` на категорию из `Base`.

- Заголовок: `#include <scl/utility/type_traits.h>`
- Объявление: `template <typename Base, typename Type> using forward_like_t = ...;`

### Семантика

- **Применение cv-квалификаторов (наложение):**
  Квалификаторы `const` и `volatile` из `Base` (без учёта его ссылки) **добавляются**
  к `Type`. Имеющиеся у `Type` квалификаторы сохраняются, происходит их объединение.

- **Применение ссылки (замена):**
  Ссылочная категория (`&` или `&&`) полностью определяется типом `Base`. Существующая
  у `Type` ссылка игнорируется и заменяется. Правила схлопывания ссылок здесь не
  применяются.

- **Особые случаи:**
  Если `Type` является `void`, ссылка к нему не применяется, но cv-квалификаторы
  из `Base` добавляются. При работе с массивами cv-квалификаторы добавляются
  на тип элемента, а ссылка из `Base` применяется ко всему массиву.

### Примеры

```cpp
#include <scl/utility/type_traits.h>

using ::scl::forward_like_t;

// CV-квалификаторы добавляются (объединяются)
static_assert(std::is_same_v<forward_like_t<const int, double>, const double>);
static_assert(std::is_same_v<forward_like_t<const int, volatile double>, const volatile double>);
static_assert(std::is_same_v<forward_like_t<int, const double>, const double>);

// Ссылочная категория заменяется (копируется из Base)
static_assert(std::is_same_v<forward_like_t<int&, double&&>, double&>); // && у double заменяется на &
static_assert(std::is_same_v<forward_like_t<int&&, double&>, double&&>); // & у double заменяется на &&
static_assert(std::is_same_v<forward_like_t<int, double&>, double>);    // & у double удаляется

// Гибридное поведение
static_assert(std::is_same_v<forward_like_t<const int&, volatile double&&>, const volatile double&>);

// Особый случай для void
static_assert(std::is_same_v<forward_like_t<const int&, void>, const void>);
static_assert(std::is_same_v<forward_like_t<int&&, const void>, const void>); // Ссылка не применяется

// Массивы
static_assert(std::is_same_v<forward_like_t<const int, int volatile[3]>, int const volatile[3]>);
static_assert(std::is_same_v<forward_like_t<int&, int[3]>, int (&)[3]>);
static_assert(std::is_same_v<forward_like_t<int&&, int const(&)[3]>, int const (&&)[3]>);
```

## Типичные применения

Используется для точного воспроизведения поведения `std::forward_like` из C++23,
обеспечивая консистентную логику передачи аргументов в разных версиях стандарта C++.
Полезен, когда квалификаторы типа-обёртки `Base` должны быть применены
к передаваемому значению `Type`.

## См. также

- `forward_like` — соответствующая функция, использующая этот типаж для приведения типов.

---

# forward_like<Base, T>(t)

Вспомогательная функция, которая передаёт значение `t` как ссылку с cv/ref-квалификаторами,
определёнными с помощью `forward_like_t`. Является прямым аналогом `std::forward_like`
из C++23.

- Заголовок: `#include <scl/utility/type_traits.h>`
- Объявление: `template <typename Base, typename T> constexpr decltype(auto) forward_like(T&& t) noexcept;`

## Описание

Эта функция выполняет `static_cast` на входном значении `t` к типу, полученному
из `forward_like_t<Base, T&&>`. Она идеально передаёт значение с желаемыми
cv/ref-свойствами, делая обобщённый код чище и выразительнее.

## Пример

```cpp
#include <scl/utility/type_traits.h>
#include <iostream>
#include <string>

// Функция, обрабатывающая строку, с поведением, зависящим от квалификаторов
void process(const std::string& str) { std::cout << "Обработка const lvalue-ссылки: " << str << std::endl; }
void process(std::string& str)       { std::cout << "Обработка non-const lvalue-ссылки: " << str << std::endl; }
void process(std::string&& str)      { std::cout << "Обработка rvalue-ссылки: " << str << std::endl; }

// Обобщённая обёртка, которая передаёт свой аргумент "подобно" параметру T
template <typename T, typename U>
void wrapper(U&& arg)
{
    // Передаём arg с квалификаторами из T
    process(scl::forward_like<T>(arg));
}

int main()
{
    std::string s = "привет";
    const std::string cs = "мир";

    wrapper<const int&>(s); // Передаём s как const lvalue-ссылку
    wrapper<int&>(s);       // Передаём s как non-const lvalue-ссылку
    wrapper<int&&>(std::move(s)); // Передаём временный объект как rvalue-ссылку
    // wrapper<int&>(cs);      // Ошибка компиляции
}
```
Последняя закомментированная строка в `main` не скомпилируется, потому что
`forward_like_t<int&, const std::string&>` даёт тип `const std::string&`.
У функции `process` нет перегрузки, которая могла бы связать `const std::string&`
с параметром `std::string&`. Это демонстрирует, как `forward_like` корректно
сохраняет `const`-квалификатор от базового типа значения.
