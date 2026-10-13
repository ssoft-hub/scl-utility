# `type_key`

Класс `type_key` идентифицирует тип без RTTI, а шаблон функции `type_key_of<T>()` возвращает ссылку
на ключ типа `T`. Типы с одним именем, объявленные в безымянных пространствах имён двух единиц
трансляции, получают разные ключи.

- Заголовок: `#include <scl/utility/meta/type_key.h>`

```cpp
namespace scl
{
    class type_key
    {
    public:
        constexpr type_key() noexcept = default;
        constexpr type_key(type_key const &) = default;
        constexpr type_key(type_key &&) = default;
        constexpr type_key & operator=(type_key const &) = default;
        constexpr type_key & operator=(type_key &&) = default;
        constexpr ~type_key() = default;

        [[nodiscard]] constexpr ::std::string_view name() const noexcept;

        [[nodiscard]] friend constexpr bool operator==(type_key const & left,
            type_key const & right) noexcept;
    };

    template <typename T, ::std::byte const * Unit = /* якорь единицы трансляции или nullptr */>
    [[nodiscard]] constexpr type_key const & type_key_of() noexcept;
}
```

## Семантика

- **Что хранит ключ:** ключ хранит имя, которое даёт типу [`scl::type_name<T>()`](type_name.md)
  и которое возвращает функция-член `name()`, а рядом с ним дискриминатор, который не возвращает
  ни одна функция-член. Для типа с внешним связыванием дискриминатор равен нулевому указателю,
  кроме типа, в имени которого стоит один из символов, описанных ниже. Для типа, локального в своей
  единице трансляции, как это определяет [`scl::is_tu_local`](is_tu_local.md), дискриминатор хранит
  адрес объекта, который есть только в этой единице трансляции, поэтому типы с одним именем из двух
  единиц трансляции получают разные дискриминаторы.
- **Сравнение:** два ключа равны, если равны их имена и их дискриминаторы, а `!=` возвращает
  отрицание `==`. Две ссылки на один ключ равны сразу, поэтому сравнение сохранённой ссылки
  с результатом `type_key_of<T>()` стоит одного сравнения адресов, если ссылка указывает на этот
  ключ. Типы с одним именем в безымянных пространствах имён двух единиц трансляции различаются
  дискриминатором, а ключи типа с внешним связыванием, используемого и в исполняемом файле,
  и в разделяемой библиотеке, равны по имени, кроме типа, в имени которого стоит один из символов,
  описанных ниже.
- **Два типа с одним именем:** у GCC и Clang одно имя может обозначать два типа, как описано
  на странице [`type_short_name<T>()`](type_name.md). Оба компилятора записывают указатель
  на `operator-` перед закрывающей угловой скобкой `>` как `operator->`, а указатель на `operator<=`
  перед ней как `operator<=>`. Clang к тому же записывает указатель на `operator>` перед ней
  как `operator>>`. GCC записывает указатель на такую специализацию, `Tagged<&P::operator- > *`,
  как `Tagged<&P::operator->*`. У GCC и Clang тип, в имени которого стоит `operator->`,
  `operator->*`, `operator<=>` или `operator>>`, получает дискриминатором адрес собственного
  объекта. Тогда два таких типа получают разные ключи, а ключ такого типа из другого модуля может
  оказаться не равен его ключу из этого модуля.
- **Откуда берётся ключ:** только `type_key_of<T>()` возвращает ключ, который идентифицирует тип.
  Класс хранит данные только в закрытых полях, и ни одна функция-член, кроме присваивания, не меняет
  ключ. Вызывающий код сам записывает только пустой ключ `scl::type_key{}`, который
  не идентифицирует ни один тип и не равен ни одному ключу, возвращённому `type_key_of<T>()`.
- **Ключ является значением:** ключ допускает копирование, перемещение и присваивание, и копия
  идентифицирует тот же тип, что и источник.
- **Ни порядка, ни хеша:** у ключей нет ни `operator<=>`, ни `std::less`, потому что тождество ключа
  локального типа и типа, в имени которого стоит один из символов выше, опирается на адрес,
  а стандарт оставляет неуказанным взаимный порядок адресов несвязанных объектов, и в константном
  выражении нельзя прочитать этот порядок. У ключа нет хеша, а в примере ниже показано, как написать
  для ключа хеш.
- **Один ключ на тип:** ключ типа является переменной `inline constexpr`, поэтому в пределах одного
  модуля каждый вызов `type_key_of<T>()` для типа с внешним связыванием возвращает ссылку на один
  и тот же объект. Вызывающий код хранит либо эту ссылку, которая сравнивается по адресу, либо
  копию, которая сравнивается по содержимому. Вызывающий код передаёт только `T`. Аргумент `Unit`
  по умолчанию равен якорю единицы трансляции для локального типа и нулевому указателю для любого
  другого типа. С якорем в аргументах GCC даёт ключу локального типа внутреннее связывание.
  Без якоря GCC дал бы такому ключу внешнее связывание, если `T` содержит указатель на член типа
  из безымянного пространства имён, и компоновщик оставил бы один ключ на одноимённые локальные типы
  двух единиц трансляции. Так же GCC даёт внешнее связывание специализации зависящего только от `T`
  шаблона из вызывающего кода, например шаблона переменной со значением `&type_key_of<T>()`.
  Компоновщик оставляет одну такую специализацию на все единицы трансляции, и она хранит ключ
  локального типа только одной из них.
- **Время жизни:** ключ ссылается на данные создавшего его модуля (исполняемого файла
  или разделяемой библиотеки). Имя ссылается на строку этого модуля, а ненулевой дискриминатор
  ссылается на объект этого модуля. Ключ не должен жить дольше модуля, который создал его. После
  `FreeLibrary` или `dlclose` висячими становятся имя и ненулевой дискриминатор, а модуль,
  загруженный позже по тому же адресу, может сделать устаревший ключ равным ключу другого типа.
- **Предусловие:** тип `T` не является локальным классом или типом замыкания и не построен из них.
  Никакая проверка не обнаруживает нарушение. Clang записывает локальный класс `duck` функции
  как `duck` в любом пространстве имён функции, то есть так же, как класс `duck` из глобального
  пространства имён, поэтому эти два класса получают равные ключи. Класс, перенесённый в безымянное
  пространство имён той же единицы трансляции, получает собственный ключ.

## Примеры

Ключи локального типа и типа с внешним связыванием:

<!-- snippet: example/meta/type_key/meta_type_key_example.cpp type_key_of -->
```cpp
namespace
{
    struct duck
    {};
} // namespace

struct goose
{};

constexpr auto & duck_key = scl::type_key_of<duck>();
constexpr auto & goose_key = scl::type_key_of<goose>();

static_assert(duck_key == scl::type_key_of<duck>());
static_assert(duck_key != goose_key);
static_assert(&scl::type_key_of<goose>() == &goose_key); // one object per type
static_assert(scl::type_key{} != goose_key);             // the empty key identifies no type
```

Обёртка над ключом с хешем его имени, которая служит ключом неупорядоченного контейнера:

<!-- snippet: example/meta/type_key/meta_type_key_example.cpp hash_key -->
```cpp
// A handle of the key of a type with a hash value of its name, computed once.
class hash_key
{
public:
    explicit constexpr hash_key(scl::type_key const & key) noexcept
        : m_key{&key}
        , m_hash{static_cast<std::size_t>(scl::hash::fnv1a(key.name()))}
    {}

    template <typename T>
    [[nodiscard]]
    static constexpr hash_key of() noexcept
    {
        return hash_key{scl::type_key_of<T>()};
    }

    [[nodiscard]]
    constexpr scl::type_key const & key() const noexcept
    {
        return *m_key;
    }

    [[nodiscard]]
    constexpr std::size_t hash() const noexcept
    {
        return m_hash;
    }

    // Types of one name in unnamed namespaces of two translation units share a hash value, so
    // the keys decide a match.
    friend constexpr bool operator==(hash_key const & left, hash_key const & right) noexcept
    {
        return left.key() == right.key();
    }

private:
    scl::type_key const * m_key;
    std::size_t m_hash;
};

template <>
struct std::hash<hash_key>
{
    std::size_t operator()(hash_key const & key) const noexcept { return key.hash(); }
};
```

<!-- snippet: example/meta/type_key/meta_type_key_example.cpp registry -->
```cpp
    std::unordered_map<hash_key, std::string> registry;
    registry.emplace(hash_key::of<duck>(), "duck");
    registry.emplace(hash_key::of<goose>(), "goose");

    auto const lookup = [&registry](hash_key const & key) -> std::string_view {
        auto const found = registry.find(key);
        return found == registry.end() ? "<unknown>" : std::string_view{found->second};
    };

    std::cout << lookup(hash_key::of<duck>()) << '\n';  // duck
    std::cout << lookup(hash_key::of<goose>()) << '\n'; // goose
    std::cout << lookup(hash_key::of<int>()) << '\n';   // <unknown>
```

## Смотрите также

- [`scl::is_tu_local<T>`](is_tu_local.md), который определяет, локален ли тип в своей единице
  трансляции
- [`example/meta/type_key/meta_type_key_example.cpp`](../../../../example/meta/type_key/meta_type_key_example.cpp)

---

Назад: [`is_tu_local<T>`](is_tu_local.md) | [К группе](index.md) | [К главной странице](../Main.md)
