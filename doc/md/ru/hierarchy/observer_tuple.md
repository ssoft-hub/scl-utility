# observer_tuple

Адаптер-развётвитель, объединяющий несколько наблюдателей `scl::hierarchy::tree`
в одного.

- Заголовочный файл: `#include <scl/utility/hierarchy/observer_tuple.h>`

## Обзор

Шаблонный параметр `Observer` дерева принимает ровно один тип наблюдателя.
`observer_tuple<Tree, Observers...>` позволяет сделать этот единственный тип
составным: он хранит `std::tuple<Observers...>` и пробрасывает каждое
уведомление всем им по очереди, в порядке объявления. Каждый внутренний
наблюдатель должен независимо удовлетворять `concepts::observer` (см.
[tree.md](tree.md#контракт-наблюдателя)).

## Возможности

- Пробрасывает все четыре коллбэка (`on_insert`, `on_erase`, `on_clear`,
  `on_change`) каждому внутреннему наблюдателю через `std::apply`
- Доступ к внутренним наблюдателям по индексу или по типу через `get()`
- Не добавляет косвенности сверх самого кортежа — ни виртуальной диспетчеризации,
  ни выделения памяти в куче

## Справочник API

```cpp
template <typename Tree, typename... Observers>
class observer_tuple;
```

### Создание

По умолчанию; каждый внутренний наблюдатель конструируется по умолчанию.

```cpp
template <typename T>
using two_counters = scl::hierarchy::observer_tuple<T, counter_a<T>, counter_b<T>>;

scl::hierarchy::tree<int, two_counters> t;   // t.get_observer() хранит оба счётчика
```

### Доступ к внутренним наблюдателям

```cpp
tuple_type &       tuple() noexcept;         // сам std::tuple<Observers...>
tuple_type const & tuple() const noexcept;

template <std::size_t I> decltype(auto) get() noexcept;         // по индексу
template <std::size_t I> decltype(auto) get() const noexcept;
template <typename T>    decltype(auto) get() noexcept;         // по типу
template <typename T>    decltype(auto) get() const noexcept;
```

### Контракт наблюдателя (реализация)

```cpp
void on_insert(node_item item);                                  // tree::iterator
void on_erase(const_node_item item);                              // tree::const_iterator
void on_clear();
void on_change(const_payload_reference old_value, const_payload_reference new_value);
```

Каждый метод вызывает соответствующий коллбэк у всех внутренних наблюдателей
через `std::apply`, в порядке объявления, безусловно (прервать распространение
на середине кортежа нельзя).

## Примеры использования

### Пример 1: композиция двух наблюдателей

```cpp
template <typename Tree>
struct logger {
    void on_insert(Tree::iterator it)
    { std::cout << "insert: " << (*it).value() << '\n'; }
    void on_erase(Tree::const_iterator it)
    { std::cout << "erase: " << (*it).value() << '\n'; }
    void on_clear() { std::cout << "clear\n"; }
    void on_change(int old_v, int new_v)
    { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
};

template <typename Tree>
struct counter {
    std::size_t count = 0;
    void on_insert(Tree::iterator) { ++count; }
    void on_erase(Tree::const_iterator) { --count; }
    void on_clear() { count = 0; }
    void on_change(int, int) {}
};

template <typename T>
using multi_obs = scl::hierarchy::observer_tuple<T, logger<T>, counter<T>>;

scl::hierarchy::tree<int, multi_obs> t;
t.push_back(1);   // logger печатает "insert: 1"; counter.count становится 1
t.push_back(2);
std::cout << t.get_observer().get<counter<decltype(t)>>().count; // 2
```

### Пример 2: доступ по индексу и по типу

```cpp
auto & first_by_index = t.get_observer().get<0>();                       // logger<...>
auto & second_by_type = t.get_observer().get<counter<decltype(t)>>();    // counter<...>
```

## Примечания

- Внутренние наблюдатели не видят друг друга и не влияют друг на друга —
  `observer_tuple` развётвляет, а не связывает в цепочку и не прерывает.
- Порядок пакета `Observers...` — это порядок доставки коллбэков; другого
  влияния он не имеет, поскольку наблюдатели не взаимодействуют.
