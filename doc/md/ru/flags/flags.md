# Flags

Типобезопасное множество значений scoped enum.

- Заголовок: `#include <scl/utility/flags.h>`

## Обзор

`scl::flags<Enum, bit_count>` хранит помещённые в него значения перечисления, по
одному биту на перечислитель: индекс бита равен подлежащему значению перечислителя
(`One == 0`, `Two == 1`, ...), поэтому явные значения перечислителям не нужны.
Множества комбинируются операторами, а не приведением к подлежащему целому.

Биты хранятся в `std::array<std::byte>` размером под `bit_count` (по умолчанию 32),
поэтому все операции пригодны для вычисления на этапе компиляции на базовом
уровне C++20 — в отличие от `std::bitset`, чьи операции запроса и изменения стали
`constexpr` лишь в C++23.

Принимаются только scoped-перечисления; не-scoped `enum` отклоняется через
`static_assert`. Перечислитель с порядковым номером `>= bit_count` находится вне диапазона:
конструктор и `operator[]` выбрасывают исключение `std::out_of_range` во время выполнения
и некорректны в константном вычислении. Предикаты исключений не выбрасывают: порядковый
номер вне диапазона трактуется как отсутствующее значение. Там, где
[`SCL_HAS_EXCEPTIONS`](../preprocessor/exceptions.md) равен `0`, тот же вызов
завершает программу через `std::abort()`: нарушению предусловия нечего вернуть
вместо ответа.

## Возможности

- По одному биту на порядковый номер перечислителя; явные значения не требуются
- Полностью `constexpr`-пригодно (C++20)
- Алгебра множеств `| & ^ -` и составные `|= &= ^= -=` в формах множество-множество
  и множество-`Enum`
- `operator[]` — проверка принадлежности
- Предикаты `all_of` / `any_of` / `none_of` над пакетом значений и над другим
  множеством (подмножество / пересечение / непересечение)
- Запросы `any` / `none` над всем множеством
- Двунаправленный диапазон по хранимым значениям, по возрастанию порядкового номера

## Справочник по API

### Конструирование

Каждый блок ниже взят из
[`example/flags/common/flags_common_example.cpp`](../../../../example/flags/common/flags_common_example.cpp),
который CI компилирует и запускает, поэтому страница показывает работающий код.

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

Пустой фигурный список выбирает конструктор по умолчанию: множество пусто.
Передача значения с порядковым номером `>= bit_count` выбрасывает исключение
`std::out_of_range`.

Второй параметр шаблона задаёт ширину хранилища; используйте его, когда 32 бита
по умолчанию не подходят:

```cpp
using small = scl::flags<permission, 4>; // 4-битное хранилище
```

### Принадлежность и предикаты

`all_of` / `any_of` / `none_of` принимают либо пакет значений, либо другой `flags` — так
записываются подмножество, пересечение и непересечение. Пустой пакет истинен для `all_of`
и `none_of` и ложен для `any_of`. Запроса «хранит все значения» нет: какие значения считать
полным множеством, называет вызывающая сторона, и спрашивает о нём тот же `all_of`.

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

Объединение, пересечение, симметрическая разность и разность существуют в формах
множество-множество и множество-`Enum`, у каждой есть составной вариант. Все они
замкнуты на значениях, которые хранят операнды.

Оператора дополнения нет. Дополнение берётся относительно полного множества, а `flags`
знает только `capacity` — ширину хранилища, которую перечисление вправе занять не
полностью или разредить. Дополнение по ширине вернуло бы порядковые номера, которым не
соответствует ни один перечислитель. Полное множество называется явно, и разность берётся
от него.

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

`flags` — двунаправленный диапазон по хранимым значениям: они выдаются как `Enum`
по возрастанию порядкового номера. `size()` — сколько значений хранится, в отличие
от статического `capacity`, который равен ширине хранилища.

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
  программа, из которой взяты блоки: принадлежность и предикаты, алгебра множеств
  над двумя множествами и обход в обе стороны.
- [Английская документация](../../en/flags/flags.md)
