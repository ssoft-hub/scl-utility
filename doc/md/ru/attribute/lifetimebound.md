# SCL_LIFETIMEBOUND

Аннотация параметра, ограниченного временем жизни возвращаемого значения.

- Заголовок: `#include <scl/utility/attribute/lifetimebound.h>`
- Агрегатор: `#include <scl/utility/attribute.h>`

---

## SCL_LIFETIMEBOUND

Помечает параметр функции (или неявный `*this` метода) как ограниченный
временем жизни возвращаемого значения. При аннотации компилятор может
предупредить, если возвращаемая ссылка, указатель или представление
переживают аргумент — позволяя обнаруживать ошибки висячих ссылок
во время компиляции.

**Размещение** — после объявления параметра или после cv-квалификатора
метода.

> **Примечание:** В настоящее время поддерживается только Clang. GCC и MSVC
> не реализуют этот атрибут; запасной вариант — пустая строка: предупреждений
> нет, но код корректно компилируется.

### Определение наличия

| Условие | Раскрытие |
|---------|-----------|
| `__has_cpp_attribute(clang::lifetimebound)` (Clang) | `[[clang::lifetimebound]]` |
| `__has_attribute(lifetimebound)` (Clang старше) | `__attribute__((lifetimebound))` |
| Ни одно не выполнено | *(пусто — анализ времени жизни отсутствует)* |

### Использование

```cpp
#include <scl/utility/attribute/lifetimebound.h>

// Параметр — возвращаемое значение не должно переживать 's'
std::string_view first_word(const std::string& s SCL_LIFETIMEBOUND);

// Несколько параметров
const int& clamp(const int& v SCL_LIFETIMEBOUND,
                 const int& lo SCL_LIFETIMEBOUND,
                 const int& hi SCL_LIFETIMEBOUND);

// Метод — возвращаемое значение не должно переживать *this
class Builder {
public:
    Builder& set_name(std::string_view name) SCL_LIFETIMEBOUND;
    std::string_view name() const SCL_LIFETIMEBOUND;
};
```

### Что обнаруживает Clang

```cpp
std::string_view sv = first_word(std::string{"hello world"});
//                                            ^~~~~~~~~~~~~ временный объект уничтожен здесь
// warning: address of stack memory associated with temporary ...
```

---

## Примечания

- `SCL_LIFETIMEBOUND` не имеет накладных расходов во время выполнения —
  это исключительно подсказка компилятору.
- Не заменяет RAII или типы владения; используйте для аннотации функций,
  возвращающих невладеющие представления.
- Макрос можно переопределить до включения заголовка через
  `#define SCL_LIFETIMEBOUND`.
