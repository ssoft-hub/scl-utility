# Flags

Типобезопасная битовая маска над scoped enum.

- Заголовок: `#include <scl/utility/flags.h>`

## Обзор

`scl::flags<Enum, bit_count>` хранит по одному биту на перечислитель: индекс бита
флага равен подлежащему значению перечислителя (`One == 0`, `Two == 1`, ...),
поэтому явные значения перечислителям не нужны. Флаги комбинируются битовыми
операторами без приведения к подлежащему целому.

Биты хранятся в `std::array<std::byte>` размером под `bit_count` (по умолчанию 32),
поэтому все операции пригодны для вычисления на этапе компиляции на базовом
уровне C++20 — в отличие от `std::bitset`, чьи операции запроса и изменения стали
`constexpr` лишь в C++23.

Принимаются только scoped-перечисления; не-scoped `enum` отклоняется через
`static_assert`. Перечислитель с ординалом `>= bit_count` находится вне диапазона:
конструктор и `operator[]` бросают `std::out_of_range` во время выполнения и
некорректны в константном вычислении. Предикаты не бросают — ординал вне
диапазона трактуется как невыставленный. Там, где
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) равен `0`, тот же вызов
завершает программу через `std::abort()`: нарушению предусловия нечего вернуть
вместо ответа.

## Возможности

- По одному биту на ординал перечислителя; явные значения не требуются
- Полностью `constexpr`-пригодно (C++20)
- Битовые `~ | & ^` и составные `|= &= ^=` в формах flags-flags и flags-`Enum`
- `operator[]` — проверка принадлежности
- Предикаты `all_of` / `any_of` / `none_of` в вариативной форме и форме над маской
  (подмножество / пересечение / непересечение)
- Запросы над всей маской: `any` / `none` / `all`
- Двунаправленный диапазон по выставленным флагам, по возрастанию ординала

## Справочник по API

### Конструирование

```cpp
enum class permission { read, write, execute, remove };
using permissions = scl::flags<permission>;

permissions none{};                                  // без флагов
permissions rw{permission::read, permission::write}; // два флага
```

Пустой фигурный список выбирает конструктор по умолчанию (пустая маска).
Передача значения с ординалом `>= bit_count` бросает `std::out_of_range`.

Второй параметр шаблона задаёт ширину хранилища; используйте его, когда 32 бита
по умолчанию не подходят:

```cpp
using small = scl::flags<permission, 4>; // 4-битное хранилище
```

### Принадлежность и предикаты

```cpp
constexpr permissions p{permission::read, permission::write};

p[permission::read];                                   // true
p.all_of(permission::read, permission::write);         // true (все перечисленные)
p.any_of(permission::execute);                         // false (хотя бы один)
p.none_of(permission::remove);                         // true (ни одного)
p.all_of();                                            // true  (пустой пакет)
p.any_of();                                            // false (пустой пакет)
```

Те же предикаты принимают другой `flags` для отношений множеств:

```cpp
p.all_of(permissions{permission::read});   // подмножество:  все флаги аргумента выставлены
p.any_of(permissions{permission::write});  // пересечение:   есть общий выставленный флаг
p.none_of(permissions{permission::remove}); // непересечение: общих флагов нет
```

Запросы над всей маской смотрят на все `bit_count` бит:

```cpp
p.any();   // выставлен хотя бы один бит
p.none();  // не выставлено ни одного
p.all();   // выставлены все bit_count бит
```

### Комбинирование

```cpp
constexpr permissions a{permission::read, permission::write};
constexpr permissions b{permission::write, permission::execute};

a | b;                    // объединение
a & b;                    // пересечение
a ^ b;                    // симметрическая разность
~a;                       // дополнение по bit_count битам
a | permission::execute;  // форма flags-Enum (flags слева)

permissions m{permission::read};
m |= permission::write;   // составное присваивание (формы flags и Enum)
```

### Итерация

`flags` — двунаправленный диапазон, элементы которого — *выставленные* флаги как
значения `Enum` по возрастанию ординала. `size()` — число выставленных флагов
(популяционный счёт), в отличие от статического `capacity` (ширина в битах).

```cpp
permissions granted{permission::read, permission::execute};

for (permission flag : granted) { /* read, затем execute */ }

for (auto it = granted.rbegin(); it != granted.rend(); ++it) { /* execute, затем read */ }

granted.size();      // 2  (выставленные флаги)
permissions::capacity; // 32 (ширина в битах)
```

Поскольку `flags` моделирует `std::ranges::bidirectional_range` и
`std::ranges::sized_range`, он совместим с представлениями и алгоритмами `<ranges>`.

## Смотрите также

- [`example/flags/common/flags_common_example.cpp`](../../../../example/flags/common/flags_common_example.cpp) —
  рабочая версия: объединение и проверка флагов, алгебра множеств над двумя масками
  и обход установленных флагов в обе стороны.
- [Английская документация](../../en/flags/flags.md)
