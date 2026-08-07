# overload_cast

Утилита для разрешения неоднозначности перегруженных функций и функций-членов по типам аргументов.

- Заголовок: `#include <scl/utility/type_traits/overload_cast.h>`

Содержание:
- [`overload_cast<Args...>`](#overload_cast-args)

---

## `overload_cast<Args...>`

Выбирает конкретную перегрузку указателя на функцию или функцию-член на основе типов аргументов.

- Заголовок: `#include <scl/utility/type_traits/overload_cast.h>`
- Объявление: `template <class... Args> inline constexpr detail::overload_cast<Args...> overload_cast{};`

### Семантика

- **Вызываемый объект:** `overload_cast<Args...>` — это шаблонная переменная `constexpr`, чей `operator()` принимает указатель на функцию или указатель на член и возвращает его без изменений, при этом компилятор выбирает корректную перегрузку.
- **Поддержка квалификаторов:** Обрабатывает все комбинации квалификаторов функций-членов:
  - cv-квалификаторы: `const`, `volatile`, `const volatile`
  - ref-квалификаторы: `&`, `&&`
  - спецификатор `noexcept`
- **Свободные функции:** Также работает с указателями на свободные (не являющиеся членами) функции.
- **Нулевые накладные расходы:** Оператор вызова является `constexpr` и `noexcept`; он просто возвращает указатель.

### Мотивация

Взятие адреса перегруженной функции обычно требует многословного `static_cast` к точному типу указателя на функцию. `overload_cast` предоставляет лаконичную и читаемую альтернативу:

```cpp
// Без overload_cast — многословно и подвержено ошибкам
auto p = static_cast<void (Widget::*)(int)>(&Widget::update);

// С overload_cast — лаконично и понятно
auto p = scl::overload_cast<int>(&Widget::update);
```

### Примеры

```cpp
#include <scl/utility/type_traits/overload_cast.h>

struct Widget {
    void update(int);
    void update(double) const;
    void update(int, int) noexcept;
};

void process(int);
void process(double) noexcept;

// Перегрузки функций-членов
auto p1 = scl::overload_cast<int>(&Widget::update);          // void(int)
auto p2 = scl::overload_cast<double>(&Widget::update);       // void(double) const
auto p3 = scl::overload_cast<int, int>(&Widget::update);     // void(int, int) noexcept

// Перегрузки свободных функций
auto f1 = scl::overload_cast<int>(&process);                 // void(int)
auto f2 = scl::overload_cast<double>(&process);              // void(double) noexcept
```

### Типичные сценарии использования

- Передача перегруженных функций-членов в `std::bind`, `std::invoke` или системы сигналов/слотов, где компилятор не может вывести нужную перегрузку.
- Подключение сигналов Qt к перегруженным слотам.
- Регистрация перегруженных обратных вызовов в системах рефлексии или привязках к скриптовым языкам.
