# Утилиты для получения имени символа

Извлечение имён функций, методов и членов данных класса на этапе компиляции.

- Заголовок: `#include <scl/utility/meta/symbol.h>`

Содержание:
- [`symbol_name<S>`](#symbol_name-s)
- [`symbol_short_name<S>`](#symbol_short_name-s)

---

## `symbol_name<S>`

Возвращает квалифицированное строковое имя символа (глобальная функция, метод класса или член данных).

Утилита предоставляет единый способ извлечения идентификаторов для различных сущностей C++, переданных как нетиповые параметры шаблона.

- Заголовок: `#include <scl/utility/meta/symbol.h>`
- Объявление: `template <auto S> constexpr std::string_view symbol_name() noexcept;`

### Семантика

- **Агрессивная очистка:**
  Утилита автоматически удаляет специфичный для компилятора «шум», такой как:
  - Операторы взятия адреса (`&`).
  - Окружающие скобки и лишние пробелы (характерно для MSVC при глобальных переменных).
  - Информацию о типе, встроенную MSVC в сигнатуру.
- **Поддержка членов класса:**
  Работает как с указателями на функции-члены (`&Class::Method`), так и с указателями на члены данных (`&Class::Field`).

### Примеры

```cpp
#include <scl/utility/meta/symbol.h>

namespace sys {
    void initialize() {}
    struct Clock {
        int ticks;
        void reset() {}
    };
}

// Глобальная функция / функция в пространстве имён
static_assert(scl::symbol_name<sys::initialize>() == "sys::initialize");

// Функция-член
static_assert(scl::symbol_name<&sys::Clock::reset>() == "sys::Clock::reset");

// Член данных (свойство)
static_assert(scl::symbol_name<&sys::Clock::ticks>() == "sys::Clock::ticks");
```

### Типичные сценарии использования

Реализация фреймворков сериализации, автоматическое отображение аргументов командной строки или отладчики, где строковое имя поля класса или функции требуется на этапе компиляции.

---

## `symbol_short_name<S>`

Возвращает только терминальный идентификатор символа `S`.

- Заголовок: `#include <scl/utility/meta/symbol.h>`
- Объявление: `template <auto S> constexpr std::string_view symbol_short_name() noexcept;`

### Пример

```cpp
#include <scl/utility/meta/symbol.h>

struct Processor {
    void execute() {}
};

int main() {
    // Возвращает "execute" вместо "Processor::execute"
    constexpr auto name = scl::symbol_short_name<&Processor::execute>();
}
```
