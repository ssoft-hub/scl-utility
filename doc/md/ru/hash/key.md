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

Все хеш-функции принимают диапазон, отвечающий концепту `std::ranges::range`, элемент
которого занимает ровно один байт данных и тривиально копируется, а сам диапазон не является
ограниченным массивом. Этим требованиям отвечают типы `std::string_view`, `std::string`,
`std::span<std::byte>` и вектор байтов. Диапазон, допускающий обход только по неконстантной
ссылке, тоже подходит. Массив передать нельзя; причина изложена в примечании ниже.

Более широкий элемент — `wchar_t`, `char16_t`, `char32_t` или любой арифметический
тип — отвергается на этапе компиляции, а не усекается до младшего байта: усечение
позволило бы двум входным данным, различающимся выше этого байта, дать одно
хеш-значение.

`scl::hash::byte_view` — то место, где такой диапазон говорит, какие байты имеются
в виду:

```cpp
#include <scl/utility/hash/byte_view.h>
#include <scl/utility/hash/fnv1a.h>

#include <string_view>

constexpr auto value = scl::hash::fnv1a(scl::hash::byte_view(std::u16string_view{u"start"}));
```

Каждый элемент отдаёт `sizeof(элемент)` байт, начиная с младшего, независимо от порядка
байт самой машины, — две машины получают одно хеш-значение для одного входа. Однобайтовый
элемент проходит без изменений, поэтому `byte_view` там, где он не нужен, ничего не меняет.

Функция `byte_view` не принимает числа с плавающей точкой, потому что их байты различают
значения, равные при сравнении (значения `0.0` и `-0.0`, разные значения `NaN`).
Представление раскладывает на байты целое число любой ширины, в том числе значение типа
`wchar_t`.

Ширину типов `wchar_t`, `int`, `long` и `size_t` задаёт платформа, и тип `wchar_t` занимает
два байта на Windows и четыре на остальных платформах. Поэтому хеш-значение по такому
элементу сопоставимо внутри одной сборки, а не между платформами. Ширину типов `char16_t` и
`char32_t` задаёт стандарт, поэтому один и тот же текст занимает в них одинаковое число
байтов на любой платформе. Если хеш-значения сравниваются между сборками для разных платформ,
вызывающей стороне следует брать элемент фиксированной ширины.

> **Примечание о массивах.**
> Хешируются те байты, которые охватывает диапазон, а у массива тип не показывает, какие
> из его байтов вызывающая сторона считает данными. Массив типа `char[64]` с тремя
> символами - это шестьдесят четыре элемента с точки зрения типа и три с точки зрения
> того, кто его заполнил, и в типе это различие не выражено. У строкового литерала размер
> задан языком, а не вызывающей стороной, и шестой элемент типа `char const[6]` у
> литерала `"hello"` - завершающий нуль, отсутствующий в тексте. Поэтому массив не
> принимается ни при каком типе элемента, и массив типа `std::uint8_t[64]` с тремя байтами
> полезных данных - тот же случай.
>
> Вызывающей стороне следует явно указывать, какие байты берутся; тогда
> хеш-значение совпадает с эталонным значением алгоритма для этих байтов:
>
> ```cpp
> #include <scl/utility/hash/fnv1a.h>
>
> #include <span>
> #include <string_view>
>
> using namespace std::string_view_literals;
>
> char buffer[64]{'a', 'b', 'c'};                       // три элемента из интерфейса C
>
> auto const named = scl::hash::fnv1a("hello"sv);                // пять символов
> auto const text = scl::hash::fnv1a(std::string_view{buffer});  // текст в буфере
> auto const whole = scl::hash::fnv1a(std::span{buffer});        // каждый элемент буфера
> ```
>
> Длина заполненной части буфера известна только вызывающей стороне.
> Выражение `std::string_view{buffer}` охватывает байты до завершающего нуля, которого
> в буфере с байтами, а не с текстом, может не быть. Выражение
> `std::string_view{buffer, length}` или `std::span{buffer, length}` охватывает ровно те
> байты, которые записаны в буфер.
>
> Правило опирается на то, что различает система типов, а не на то, где возникает
> неверный результат. Строковый литерал и объявленный буфер относятся к одному виду
> типов, и объявление `constexpr char declared[6]` даёт тот же тип `char const[6]`, что и
> литерал `"hello"` выше. Ограничение, не принимающее литерал, не примет и объявленный
> буфер, потому что различить их оно не может. Правило касается только ограниченного
> массива. Размер типа `std::array<char, 64>` задан так же, как размер типа `char[64]`, и всё же
> объект типа `std::array<char, 64>` принимается, а заполненный частично объект
> хешируется целиком. Принимаются объекты типов `std::string_view`, `std::span` и
> `std::array`, а также объект любого контейнера, элемент которого занимает один байт
> данных.
>
> Правило касается только самого массива, поэтому представление
> `std::views::all(buffer)` принимается и охватывает массив целиком.

---

## Алгоритмы хеширования

### FNV-1a — `scl::hash::fnv1a`

```cpp
#include <scl/utility/hash/fnv1a.h>

using namespace std::string_view_literals;

constexpr auto h = scl::hash::fnv1a("hello"sv);
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
#include <scl/utility/hash/fnv1a.h>

#include <cstdint>
#include <string_view>

std::uint64_t chained()
{
    auto h = scl::hash::fnv1a(std::string_view{"foo"});
    h      = scl::hash::fnv1a(std::string_view{"bar"}, h);
    return h;   // == fnv1a(std::string_view{"foobar"})
}
```

---

### djb2a — `scl::hash::djb2`

```cpp
#include <scl/utility/hash/djb2.h>

using namespace std::string_view_literals;

constexpr auto h = scl::hash::djb2("hello"sv);
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

using namespace std::string_view_literals;

constexpr auto h = scl::hash::sdbm("hello"sv);
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

using namespace std::string_view_literals;

constexpr auto h = scl::hash::jenkins_ota("hello"sv);
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

using namespace std::string_view_literals;

constexpr scl::hash::siphash_key my_key{0xdeadbeefull, 0xcafebabeull};

constexpr auto with_default = scl::hash::siphash("hello"sv);          // ключ по умолчанию
constexpr auto with_own     = scl::hash::siphash("hello"sv, my_key);  // собственный ключ
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
#include <scl/utility/hash/fnv1a.h>

#include <string_view>

using namespace std::string_view_literals;

scl::hash::fnv1a_hasher h;
auto value = h("hello"sv);   // то же, что scl::hash::fnv1a("hello"sv)
```

`siphash_hasher<Key>` встраивает ключ как нетиповой параметр шаблона, поэтому
два экземпляра с разными ключами являются **разными типами**:

```cpp
#include <scl/utility/hash/siphash.h>

#include <type_traits>

constexpr scl::hash::siphash_key key_a{1, 2};
constexpr scl::hash::siphash_key key_b{3, 4};

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

Шаблон класса `key` принимает любой хешер, у которого есть конструктор по умолчанию,
объявлен целочисленный тип `result_type` и определён оператор вызова
`operator()(Range&&)`.

Ограничение стоит в каждой сигнатуре отдельно, и единого места, откуда ограниченный массив не
принимался бы сразу во всех хеш-функциях модуля `scl::hash`, нет. Поэтому в хешере,
написанном вызывающей стороной, это ограничение следует указывать явно - концептом
`hashable_range`, который требует однобайтового тривиально копируемого элемента и запрещает
ограниченный массив:

```cpp
struct rolling_hasher {
    using result_type = std::uint64_t;

    template <scl::hash::concepts::hashable_range Range>
    constexpr result_type operator()(Range&& range) const;
};
```

Без такого ограничения вызов хешера с массивом типа `char[64]` компилируется, и в
хеш-значение входят все шестьдесят четыре элемента, а не те три, что записаны в массив.
Построить из того же массива объект типа `key<rolling_hasher>` всё равно не удастся:
ограничение стоит в конструкторе шаблона класса `key` и действует при любом хешере.

---

## `key<Hasher>` — строго типизированное хеш-значение

```cpp
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/key.h>

#include <string_view>

using namespace std::string_view_literals;
using namespace scl::hash;

constexpr key<> id{"my_event"sv};           // по умолчанию: siphash_hasher<>
constexpr key<fnv1a_hasher> fnv_id{"x"sv};
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
#include <scl/utility/hash/key.h>

#include <array>
#include <string_view>

using namespace std::string_view_literals;
using namespace scl::hash;

constexpr key<> a{"hello"sv};                          // пять символов
constexpr std::array<char, 5> held{'h', 'e', 'l', 'l', 'o'};
constexpr key<> b{held};                               // те же пять, в хранилище
static_assert(a == b);                                 // одни и те же байты
```

Диапазон читается по константной ссылке везде, где он допускает обход по ней с тем же типом
элемента. У контейнера с копированием при записи буфер остаётся общим с копиями контейнера, и
чтение не вызывает копирования. Представление над изменяемым объектом обращается к этому
объекту по неконстантной ссылке независимо от того, как квалифицировано само.

Конструктор не выбрасывает исключений, если их не выбрасывает обход диапазона, и в его
объявлении это записано как `noexcept(noexcept(Hasher{}(range)))`. Такое же условие стоит у
функций `fnv1a`, `djb2`, `sdbm`, `jenkins_ota` и `siphash`. Условие проверяется по
спецификациям `noexcept` у операций самого диапазона, а эти спецификации зависят от
реализации стандартной библиотеки, и у итератора контейнера `std::vector<bool>` в реализации
libstdc++ их нет, а в реализации MSVC есть.

### Диспетчеризация через `switch`/`case`

Неявное преобразование к `value_type` позволяет `key` использоваться в
качестве метки `case`. Это заменяет длинные цепочки `if`/`else if` на
целочисленный switch с нулевыми накладными расходами:

```cpp
#include <scl/utility/hash/key.h>

#include <string_view>

using namespace std::string_view_literals;

int handle(scl::hash::key<> cmd)
{
    switch (cmd) {
    case scl::hash::key<>{"start"sv}:  return 1;
    case scl::hash::key<>{"stop"sv}:   return 2;
    case scl::hash::key<>{"status"sv}: return 3;
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
#include <scl/utility/hash/key.h>

#include <string_view>
#include <unordered_map>

using namespace std::string_view_literals;

void fill(std::unordered_map<scl::hash::key<>, int> & registry)
{
    registry[scl::hash::key<>{"alpha"sv}] = 1;
    registry[scl::hash::key<>{"beta"sv}]  = 2;
}
```

### Нетиповой параметр шаблона (NTTP)

`key<Hasher>` является **структурным типом** (все члены публичны, базовый тип —
скаляр), поэтому в C++20 его можно использовать как нетиповой параметр шаблона:

```cpp
#include <scl/utility/hash/key.h>

#include <string_view>

using namespace std::string_view_literals;

// 1. Тег типа из строкового литерала, известного при компиляции
template <scl::hash::key<> Tag>
struct event {};

using start_event = event<scl::hash::key<>{"start"sv}>;
using stop_event  = event<scl::hash::key<>{"stop"sv}>;
static_assert(!std::is_same_v<start_event, stop_event>);

// 2. Специализация шаблона по строковому ключу
template <scl::hash::key<> Cmd> struct handler { static constexpr int value = 0; };
template <> struct handler<scl::hash::key<>{"start"sv}> { static constexpr int value = 1; };
template <> struct handler<scl::hash::key<>{"stop"sv}>  { static constexpr int value = 2; };

static_assert(handler<scl::hash::key<>{"start"sv}>::value == 1);
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

// Каким должен быть элемент и чем не должен быть диапазон
namespace concepts {
    template <typename Type> concept byte_element;     // один байт данных
    template <typename Type> concept integer_element;  // byte_view разложит его на байты
    template <typename Range> concept hashable_range;  // однобайтовый элемент и запрет массива
}

// Явное преобразование для широкого элемента
constexpr auto byte_view(Range&&);   // view из uint8_t, младший байт первым

// Свободные функции - каждая noexcept там, где обход диапазона не выбрасывает исключений
constexpr uint64_t fnv1a(Range&&, uint64_t h = offset_basis) noexcept(...);
constexpr uint64_t djb2 (Range&&, uint64_t h = 5381) noexcept(...);
constexpr uint64_t sdbm (Range&&, uint64_t h = 0) noexcept(...);
constexpr uint32_t jenkins_ota(Range&&) noexcept(...);
constexpr uint64_t siphash(Range&&, siphash_key key = siphash_default_key) noexcept(...);

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

    explicit constexpr key(Range&& range) noexcept(noexcept(Hasher{}(range)));
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
  рабочий пример: объект типа `key` стоит нетиповым параметром шаблона. Программа строит
  тег типа из строкового литерала, известного при компиляции, выбирает специализацию по
  строковому значению и выполняет диспетчеризацию по такому ключу.
- [`example/hash/byte_view`](../../../../example/hash/byte_view/hash_byte_view_example.cpp) —
  рабочий пример: программа хеширует объекты типов `std::u16string_view` и
  `std::vector<std::uint32_t>` через функцию `byte_view` и показывает, какой результат даёт
  фиксированный порядок байтов.
