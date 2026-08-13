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
- Алгебра множеств `| & ^ -` и составные `|= &= ^= -=` в формах flags-flags и flags-`Enum`
- `operator[]` — проверка принадлежности
- Предикаты `all_of` / `any_of` / `none_of` в вариативной форме и форме над маской
  (подмножество / пересечение / непересечение)
- Запросы над всей маской: `any` / `none`
- Двунаправленный диапазон по выставленным флагам, по возрастанию ординала

## Справочник по API

### Конструирование

Каждый блок ниже взят из компилируемого примера, поэтому страница показывает
работающий код.

<!-- snippet: example/flags/common/flags_common_example.cpp declare -->
```cpp
enum class permission
{
    read,    // bit 0
    write,   // bit 1
    execute, // bit 2
    remove,  // bit 3
};

using permissions = ::scl::flags<permission>;

// The set the others are measured against. A flags knows its storage width, not which
// values an enumeration declares, so a caller who needs the whole set names it.
constexpr permissions all_permissions{
    permission::read, permission::write, permission::execute, permission::remove};
```

Пустой фигурный список выбирает конструктор по умолчанию (пустая маска).
Передача значения с ординалом `>= bit_count` бросает `std::out_of_range`.

Второй параметр шаблона задаёт ширину хранилища; используйте его, когда 32 бита
по умолчанию не подходят:

```cpp
using small = scl::flags<permission, 4>; // 4-битное хранилище
```

### Принадлежность и предикаты

<!-- snippet: example/flags/common/flags_common_example.cpp membership -->
```cpp
constexpr permissions granted{permission::read, permission::write};

static_assert(granted[permission::read]);
static_assert(granted.all_of(permission::read, permission::write));
static_assert(granted.any_of(permission::write, permission::execute));
static_assert(granted.none_of(permission::remove));

static_assert(granted.all_of(permissions{permission::read}));    // subset
static_assert(granted.any_of(permissions{permission::write}));   // intersection
static_assert(granted.none_of(permissions{permission::remove})); // disjoint

static_assert(granted.any());
static_assert(!granted.none());
static_assert(!granted.all_of(all_permissions)); // two flags of the four
static_assert(granted.size() == 2);
```

### Комбинирование

<!-- snippet: example/flags/common/flags_common_example.cpp algebra -->
```cpp
constexpr permissions required{permission::read, permission::execute};

static_assert((granted | required) == permissions{permission::read, permission::write, permission::execute});
static_assert((granted & required) == permissions{permission::read});
static_assert((granted ^ required) == permissions{permission::write, permission::execute});
static_assert((granted - required) == permissions{permission::write});
static_assert((granted | permission::remove).size() == 3); // flags-Enum form

// A complement is taken against a named set, never against the storage width.
static_assert((all_permissions - granted) == permissions{permission::execute, permission::remove});

// The compound forms mutate in place, so they need an object rather than a constant.
constexpr permissions effective = [] {
    permissions result{permission::read};
    result |= permission::write;
    result -= permission::read;
    return result;
}();
static_assert(effective == permissions{permission::write});
```

### Итерация

`flags` — двунаправленный диапазон, элементы которого — *выставленные* флаги как
значения `Enum` по возрастанию ординала. `size()` — число выставленных флагов
(популяционный счёт), в отличие от статического `capacity` (ширина в битах).

<!-- snippet: example/flags/common/flags_common_example.cpp iteration -->
```cpp
// Iterating yields the set flags as Enum values, ascending by ordinal; the reverse
// iterators walk the same flags back. size() counts them, capacity is the bit width.
static void print_flags(permissions const & flags)
{
    for (permission const flag : flags)
        ::std::cout << ' ' << static_cast<int>(flag);
    ::std::cout << " |";

    for (auto it = flags.rbegin(); it != flags.rend(); ++it)
        ::std::cout << ' ' << static_cast<int>(*it);

    ::std::cout << " (" << flags.size() << " of " << permissions::capacity << ")\n";
}
```

Поскольку `flags` моделирует `std::ranges::bidirectional_range` и
`std::ranges::sized_range`, он совместим с представлениями и алгоритмами `<ranges>`.

## Смотрите также

- [`example/flags/common/flags_common_example.cpp`](../../../../example/flags/common/flags_common_example.cpp) —
  рабочая версия: объединение и проверка флагов, алгебра множеств над двумя масками
  и обход установленных флагов в обе стороны.
- [Английская документация](../../en/flags/flags.md)
