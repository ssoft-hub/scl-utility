# Хеш-ключи и хешеры

- Заголовок: `#include <scl/utility/hash.h>`
- Отдельные заголовки: `#include <scl/utility/hash/key.h>`, `<scl/utility/hash/fnv1a.h>` и др.

## Обзор

Модуль `scl::hash` предоставляет набор некриптографических хеш-функций и
строго типизированную обёртку над хеш-значением — `key<Hasher>`. Вместе они
позволяют:

- **Хешировать строки во время компиляции** — все функции и `key` являются
  `constexpr`.
- **Диспетчеризацию через `switch`/`case` по строкам** — `key` неявно
  преобразуется в целое число, что делает его пригодным в качестве метки
  `case`.
- **Интеграцию с контейнерами STL** — специализация `std::hash<key<Hasher>>`
  присутствует в коде.
- **Параметры шаблона по строке (NTTP)** — `key` является структурным типом,
  поэтому в C++20 его можно использовать как нетиповой параметр шаблона.

Все хеш-функции принимают любой `std::ranges::range`, элемент которого занимает один
байт: строковые литералы, `std::string_view`, `std::string`, `std::span<std::byte>`,
байтовые векторы.

Более широкий элемент — `wchar_t`, `char16_t`, `char32_t` или любой арифметический
тип — отвергается на этапе компиляции, а не усекается до младшего байта: усечение
позволило бы двум входным данным, различающимся выше этого байта, дать одно
хеш-значение.

`scl::hash::byte_view` — то место, где такой диапазон говорит, какие байты имеются
в виду:

```cpp
#include <scl/utility/hash/byte_view.h>

constexpr auto value = scl::hash::fnv1a(scl::hash::byte_view(std::u16string_view{u"start"}));
```

Каждый элемент отдаёт `sizeof(элемент)` байт, начиная с младшего, независимо от порядка
байт самой машины, — две машины получают одно хеш-значение для одного входа. Однобайтовый
элемент проходит без изменений, поэтому `byte_view` там, где он не нужен, ничего не меняет.

Два типа элементов не принимает и `byte_view`. Числа с плавающей точкой — их байты
различают значения, равные при сравнении (`0.0` и `-0.0`, разные `NaN`). И `wchar_t` — его
ширина такая, какой её объявила платформа: два байта на Windows, четыре в других местах,
поэтому один и тот же текст дошёл бы до хеш-функции разным числом байт. `char16_t` и
`char32_t` зафиксированы стандартом и несут один и тот же текст везде.

Ширина `int`, `long` и `size_t` тоже принадлежит платформе, поэтому хеш-значение по ним
сопоставимо внутри одной сборки, а не между платформами. Если хеш-значению предстоит
путешествовать, берите элемент фиксированной ширины.

> **Примечание о строковых литералах.**
> Строковый литерал `"hello"` — это `const char[6]`, последний элемент которого —
> нулевой терминатор. Он не является частью текста и не хешируется, поэтому
> `"hello"`, `std::string_view{"hello"}` и `std::string{"hello"}` дают **одно и то
> же** хеш-значение. Правило распространяется на символьный массив любой ширины
> элемента: `char` и `char8_t` попадают в хеш-функцию напрямую, а `char16_t` и
> `char32_t` — через `byte_view`, который применяет правило там, где массив ещё
> виден. Затрагивается только последний элемент: массив, не оканчивающийся нулём —
> `const char raw[3]{'a', 'b', 'c'}` — хешируется целиком, как и массив любого
> другого типа элемента, где `std::uint8_t data[4]{1, 2, 3, 0}` сохраняет все четыре
> байта: нулевой байт там — данные, а не терминатор.

---

## Алгоритмы хеширования

### FNV-1a — `scl::hash::fnv1a`

```cpp
#include <scl/utility/hash/fnv1a.h>

constexpr auto h = scl::hash::fnv1a("hello");
```

| Свойство | Значение |
|---|---|
| Разрядность | 64 бита |
| Начальное значение (offset basis) | `14695981039346656037` (`0xcbf29ce484222325`) |
| Алгоритм | `h ^= byte; h *= 1099511628211` |
| Цепочечное хеширование | Да — передайте предыдущий результат вторым аргументом |

**FNV-1a** (Fowler–Noll–Vo, вариант 1a) — классический некриптографический
хеш, широко применяемый в компиляторах, компоновщиках и встраиваемых системах.
Порядок «XOR, затем умножение» обеспечивает лучший лавинный эффект по сравнению
с оригинальным FNV-1 (умножение, затем сложение).

**Цепочечное хеширование** двух диапазонов в одно хеш-значение:

```cpp
auto h = scl::hash::fnv1a(std::string_view{"foo"});
h      = scl::hash::fnv1a(std::string_view{"bar"}, h);
// h == fnv1a(std::string_view{"foobar"})
```

---

### djb2a — `scl::hash::djb2`

```cpp
#include <scl/utility/hash/djb2.h>

constexpr auto h = scl::hash::djb2("hello");
```

| Свойство | Значение |
|---|---|
| Разрядность | 64 бита |
| Начальное значение | `5381` |
| Алгоритм | `h = h * 33 ^ byte` |
| Цепочечное хеширование | Да |

**djb2a** (Дэниел Дж. Бернштейн, XOR-вариант) известен простотой и хорошим
распределением для коротких ASCII-ключей. Значения `5381` и `33` выбраны
эмпирически. XOR-вариант (`djb2a`) превосходит сложение (`djb2`) для входных
данных с одинаковыми префиксами.

---

### SDBM — `scl::hash::sdbm`

```cpp
#include <scl/utility/hash/sdbm.h>

constexpr auto h = scl::hash::sdbm("hello");
```

| Свойство | Значение |
|---|---|
| Разрядность | 64 бита |
| Начальное значение | `0` |
| Алгоритм | `h = byte + (h << 6) + (h << 16) - h` |
| Цепочечное хеширование | Да |

**SDBM** появился в одноимённой библиотеке баз данных. Формула со смешанными
сдвигами эффективно распределяет биты по хеш-значению и хорошо работает как для
коротких, так и для длинных ключей с повторяющимися подстроками.

---

### Jenkins One-at-a-Time — `scl::hash::jenkins_ota`

```cpp
#include <scl/utility/hash/jenkins_ota.h>

constexpr auto h = scl::hash::jenkins_ota("hello");
```

| Свойство | Значение |
|---|---|
| Разрядность | **32 бита** |
| Начальное значение | `0` |
| Алгоритм | Пошаговое смешивание + финализирующий лавинный проход |
| Цепочечное хеширование | Нет (финализация неассоциативна) |

**Jenkins OAT** (Боб Дженкинс, 1997) — единственный 32-битный алгоритм в
модуле. Финализирующий проход (`h += h<<3; h ^= h>>11; h += h<<15`) гарантирует
полный лавинный эффект: каждый бит выходного значения зависит от каждого бита
входного. Для 64-битного хеш-значения используйте один из других алгоритмов.

---

### SipHash-2-4 — `scl::hash::siphash`

```cpp
#include <scl/utility/hash/siphash.h>

constexpr auto h = scl::hash::siphash("hello");            // ключ по умолчанию
constexpr auto h = scl::hash::siphash("hello", my_key);   // произвольный ключ
```

| Свойство | Значение |
|---|---|
| Разрядность | 64 бита |
| Ключ | 128 бит (`siphash_key{k0, k1}`) |
| Раунды | 2 сжатия + 4 финализации (SipHash-**2-4**) |
| Цепочечное хеширование | Нет (с ключом, с сохранением состояния) |
| Ключ по умолчанию | Эталонные тест-векторы из статьи (`siphash_default_key`) |

**SipHash-2-4** (Аумассон и Бернштейн, 2012) разработан специально для замены
ad-hoc хеш-функций строк в хеш-таблицах. Он обеспечивает надёжную защиту от
**атак хеш-флудингом**: злоумышленник, не знающий ключа, не может создать
входные данные, вызывающие коллизии. Является хешем строк по умолчанию в
Python ≥ 3.4, Rust, Ruby и Perl.

#### Ключи

```cpp
// Фиксированный ключ — подходит для compile-time идентификаторов и switch/case.
constexpr scl::hash::siphash_key compile_key{0xdeadbeefcafeull, 0xabad1deaull};

// Случайный ключ времени выполнения — необходим для защиты от флудинга.
scl::hash::siphash_key runtime_key{
    generate_random_uint64(),
    generate_random_uint64()
};
```

`siphash_default_key` использует эталонные байты из статьи (`0x00..0x0f`).
**Не применяйте его там, где требуется защита от хеш-флудинга.**

---

## Типы хешеров

К каждому алгоритму прилагается вызываемая структура-обёртка, удовлетворяющая
концепту `scl::hash::concepts::byte_hasher`:

| Структура | `result_type` | Алгоритм |
|---|---|---|
| `fnv1a_hasher` | `std::uint64_t` | FNV-1a |
| `djb2_hasher` | `std::uint64_t` | djb2a |
| `sdbm_hasher` | `std::uint64_t` | SDBM |
| `jenkins_ota_hasher` | `std::uint32_t` | Jenkins OAT |
| `siphash_hasher<Key>` | `std::uint64_t` | SipHash-2-4 |

```cpp
scl::hash::fnv1a_hasher h;
auto value = h("hello");   // то же, что scl::hash::fnv1a("hello")
```

`siphash_hasher<Key>` встраивает ключ как нетиповой параметр шаблона, поэтому
два экземпляра с разными ключами являются **разными типами**:

```cpp
using hasher_a = scl::hash::siphash_hasher<key_a>;
using hasher_b = scl::hash::siphash_hasher<key_b>;
static_assert(!std::is_same_v<hasher_a, hasher_b>);
```

### Концепт `byte_hasher`

```cpp
template <typename H>
concept byte_hasher =
    std::default_initializable<H> &&
    requires { typename H::result_type; } &&
    std::integral<typename H::result_type>;
```

Любой пользовательский хешер, предоставляющий `result_type` и
`operator()(Range&&)`, можно использовать с `key<>`.

---

## `key<Hasher>` — строго типизированное хеш-значение

```cpp
#include <scl/utility/hash/key.h>

using namespace scl::hash;

constexpr key<> id{"my_event"};           // по умолчанию: siphash_hasher<>
constexpr key<fnv1a_hasher> fnv_id{"x"};
```

`key<Hasher>` оборачивает целочисленное хеш-значение, созданное `Hasher`, в
именованный тип, исключая случайное смешение сырых целых чисел с хеш-
значениями. Тип `value_type` выводится из `Hasher::result_type`.

| Член | Описание |
|---|---|
| `hasher_type` | Тип `Hasher` |
| `value_type` | `Hasher::result_type` — базовый целочисленный тип |
| `value` | Само хеш-значение |
| `operator value_type()` | Неявное преобразование к базовому типу |
| `operator<=>` | Трёхстороннее сравнение (`==`, `!=`, `<`, `<=`, `>`, `>=`) |

### Конструирование

```cpp
constexpr key<> a{"hello"};                     // строковый литерал (с '\0')
constexpr key<> b{std::string_view{"hello"}};   // 5 байт, без '\0'
// a != b — разные последовательности байт
```

### Диспетчеризация через `switch`/`case`

Неявное преобразование к `value_type` позволяет `key` использоваться в
качестве метки `case`. Это заменяет длинные цепочки `if`/`else if` на
целочисленный switch с нулевыми накладными расходами:

```cpp
int handle(scl::hash::key<> cmd)
{
    switch (cmd) {
    case scl::hash::key<>{"start"}:  return 1;
    case scl::hash::key<>{"stop"}:   return 2;
    case scl::hash::key<>{"status"}: return 3;
    default:                          return 0;
    }
}
```

Все значения `case` вычисляются во время компиляции — switch компилируется в
тот же код, как если бы целочисленные константы были написаны вручную.

### Контейнеры STL

`std::hash<key<Hasher>>` специализирован, поэтому `key` работает
непосредственно как ключ в `std::unordered_map` и `std::unordered_set`:

```cpp
std::unordered_map<scl::hash::key<>, int> registry;
registry[scl::hash::key<>{"alpha"}] = 1;
registry[scl::hash::key<>{"beta"}]  = 2;
```

### Нетиповой параметр шаблона (NTTP)

`key<Hasher>` является **структурным типом** (все члены публичны, базовый тип —
скаляр), поэтому в C++20 его можно использовать как нетиповой параметр шаблона:

```cpp
// 1. Тип-тег из строкового литерала времени компиляции
template <scl::hash::key<> Tag>
struct event {};

using start_event = event<scl::hash::key<>{"start"}>;
using stop_event  = event<scl::hash::key<>{"stop"}>;
static_assert(!std::is_same_v<start_event, stop_event>);

// 2. Специализация шаблона по строковому ключу
template <scl::hash::key<> Cmd> struct handler { static constexpr int value = 0; };
template <> struct handler<scl::hash::key<>{"start"}> { static constexpr int value = 1; };
template <> struct handler<scl::hash::key<>{"stop"}>  { static constexpr int value = 2; };

static_assert(handler<scl::hash::key<>{"start"}>::value == 1);
```

---

## Выбор алгоритма

| Алгоритм | Биты | С ключом | `constexpr` | Область применения |
|---|---|---|---|---|
| SipHash-2-4 | 64 | Да | Да | Хеш-таблицы, защита от флудинга |
| FNV-1a | 64 | Нет | Да | Compile-time идентификаторы, `switch`/case, встроенные системы |
| djb2a | 64 | Нет | Да | Короткие ASCII-ключи, простое использование |
| SDBM | 64 | Нет | Да | Ключи БД, строки с повторяющимися подстроками |
| Jenkins OAT | 32 | Нет | Да | 32-битные среды, совместимость с legacy-кодом |

**`key<>` по умолчанию** использует `siphash_hasher<>`. Если требуется защита
от хеш-флудинга во время выполнения, сконструируйте `siphash_key` из
случайного источника и передайте его в `siphash_hasher`. Для использования
исключительно на этапе компиляции подходит любой алгоритм.

---

## Краткий справочник API

```cpp
namespace scl::hash {

// Каким должен быть элемент
namespace concepts {
    template <typename Type> concept byte_element;     // один байт данных
    template <typename Type> concept integer_element;  // byte_view разложит его на байты
}

// Явное преобразование для широкого элемента
constexpr auto byte_view(Range&&);   // view из uint8_t, младший байт первым

// Свободные функции
constexpr uint64_t fnv1a(Range&&, uint64_t h = offset_basis);
constexpr uint64_t djb2 (Range&&, uint64_t h = 5381);
constexpr uint64_t sdbm (Range&&, uint64_t h = 0);
constexpr uint32_t jenkins_ota(Range&&);
constexpr uint64_t siphash(Range&&, siphash_key key = siphash_default_key);

// Структуры хешеров (удовлетворяют byte_hasher)
struct fnv1a_hasher;
struct djb2_hasher;
struct sdbm_hasher;
struct jenkins_ota_hasher;
template <siphash_key Key = siphash_default_key>
struct siphash_hasher;

// Тип ключа
template <byte_hasher Hasher = siphash_hasher<>>
struct key {
    using hasher_type = Hasher;
    using value_type  = Hasher::result_type;
    value_type value{};

    constexpr key(Range&&) noexcept;
    constexpr operator value_type() const noexcept;
    friend constexpr auto operator<=>(key const&, key const&) noexcept = default;
};

} // namespace scl::hash

// Интеграция с STL
template <typename Hasher>
struct std::hash<scl::hash::key<Hasher>>;
```

---

## Ссылки

- Aumasson, J.-P. & Bernstein, D. J. (2012). [SipHash: a fast short-input PRF](https://www.131002.net/siphash/)
- Fowler, G., Noll, L. C. & Vo, P. (1991). FNV Hash — общественное достояние.
- Bernstein, D. J. (1990). djb2 — общественное достояние.
- Jenkins, B. (1997). [Hash Functions](http://www.burtleburtle.net/bob/hash/doobs.html) — общественное достояние.

## Смотрите также

- [`example/hash/key_nttp`](../../../../example/hash/key_nttp/hash_key_nttp_example.cpp) —
  рабочая версия: `key` как нетиповой параметр шаблона — тег типа из строкового
  литерала, специализация по строковому значению и диспетчеризация по ключу,
  вычисленному на этапе компиляции.
- [`example/hash/byte_view`](../../../../example/hash/byte_view/hash_byte_view_example.cpp) —
  рабочая версия: хеширование `std::u16string_view` и `std::vector<std::uint32_t>`
  через `byte_view` и что даёт фиксированный порядок байт.
