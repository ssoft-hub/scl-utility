# Дерево иерархии

Дерево с несколькими корнями, поддерживающее оповещение наблюдателей об
изменении значений и структуры.

- Заголовочный файл: `#include <scl/utility/hierarchy/tree.h>`

## Обзор

`scl::hierarchy::tree<Value, Observer, Allocator>` оборачивает коллекцию
корневых узлов `node<Value>` и направляет все структурные изменения через
политику `Observer<tree>`.  Пользователи работают с узлами через прокси
`node_wrapper` / `const_node_wrapper`, которые объединяют ссылку на дерево со
ссылкой на узел, что делает невозможным изменение узла без уведомления дерева.

`tuple_of_observers` позволяет компоновать несколько независимых наблюдателей
в один.

## Контракт наблюдателя

Наблюдатель должен предоставлять:

```cpp
template <typename Tree>
struct my_observer {
    void after_insert(typename Tree::node_wrapper_iterator);
    void before_erase(typename Tree::const_node_wrapper_iterator);
    void before_clear();
    void on_change(typename Tree::const_reference_type old_val,
                   typename Tree::const_reference_type new_val);
};
```

`after_insert` вызывается после вставки узла (итератор уже валиден).
`before_erase` вызывается перед удалением (итератор указывает на живой узел).
При вложенном удалении `before_erase` вызывается снизу вверх (сначала
наиболее глубокий потомок).

## Справочник API

### tree

```cpp
template <typename Value,
          template <typename> class Observer,
          template <typename> class Allocator = std::allocator>
class tree;
```

#### Методы

```cpp
observer_reference       observer() noexcept;
observer_const_reference observer() const noexcept;

bool empty() const noexcept;

node_wrapper_iterator append (/* аргументы Value */);
node_wrapper_iterator prepend(/* аргументы Value */);
node_wrapper_iterator emplace(const_node_wrapper_iterator before, /* аргументы Value */);

Iterator erase(Iterator pos) noexcept;
Iterator erase(Iterator first, Iterator last) noexcept;
void     clear() noexcept;

void remove(Iterator pos);   // удаляет из родителя или списка корней

node_wrapper_iterator       begin()  noexcept;
const_node_wrapper_iterator cbegin() const noexcept;
node_wrapper_iterator       end()    noexcept;
const_node_wrapper_iterator cend()   const noexcept;
```

### node_wrapper

```cpp
void         set_value(const_reference_type val); // вызывает on_change
const_reference_type value() const;

node_wrapper       parent() noexcept;
const_node_wrapper parent() const noexcept;

bool       empty() const noexcept;
node_count count() const;

node_wrapper       first() noexcept;
const_node_wrapper first() const noexcept;
node_wrapper       last()  noexcept;
const_node_wrapper last()  const noexcept;

node_wrapper_iterator append (/* аргументы Value */);
node_wrapper_iterator prepend(/* аргументы Value */);
node_wrapper_iterator emplace(const_node_wrapper_iterator before, /* аргументы Value */);

Item erase(Item pos) noexcept;
Item erase(Item first, Item last) noexcept;
void clear() noexcept;

node release(const_node_wrapper_iterator pos) noexcept;
node const& access() const noexcept;

void transfer(node_wrapper from);
void transfer(const_node_wrapper_iterator before, node_wrapper from);
// ... (полные перегрузки transfer, отражающие node::transfer)
```

### tuple_of_observers

```cpp
template <typename Tree, typename... Observers>
class tuple_of_observers;
```

Пробрасывает все четыре коллбэка наблюдателя каждому внутреннему наблюдателю.

Доступ к внутренним наблюдателям по индексу или типу:

```cpp
auto& obs0 = tree.observer().get<0>();
auto& obs1 = tree.observer().get<my_observer<Tree>>();
```

## Примеры использования

### Пример 1: Простой наблюдатель

```cpp
template <typename Tree>
struct logger {
    void after_insert(typename Tree::node_wrapper_iterator it)
    { std::cout << "insert: " << (*it).value() << '\n'; }
    void before_erase(typename Tree::const_node_wrapper_iterator it)
    { std::cout << "erase: " << (*it).value() << '\n'; }
    void before_clear() { std::cout << "clear\n"; }
    void on_change(int old_v, int new_v)
    { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
};

scl::hierarchy::tree<int, logger> t;
auto root = t.append(1);       // insert: 1
(*root).append(2);             // insert: 2
(*root).set_value(10);         // change: 1 -> 10
t.clear();                     // clear
```

### Пример 2: Компоновка наблюдателей с tuple_of_observers

```cpp
template <typename T>
using multi_obs = scl::hierarchy::tuple_of_observers<T, logger<T>, counter<T>>;

scl::hierarchy::tree<int, multi_obs> t;
t.append(1);
t.append(2);
std::cout << t.observer().get<counter<decltype(t)>>().count; // 2
```

### Пример 3: Запрос иерархии через node_wrapper

```cpp
scl::hierarchy::tree<std::string, logger> t;
auto root  = t.append("root");
auto child = (*root).append("child");
auto grand = (*child).append("grand");

assert(scl::hierarchy::is_ancestor_of(*root, *grand));
assert(scl::hierarchy::are_sibling(*child, *child));
```

### Пример 4: Перенос между обёртками

```cpp
scl::hierarchy::tree<int, logger> t;
auto r1 = t.append(0);
auto r2 = t.append(0);
(*r1).append(1);
(*r1).append(2);

(*r2).transfer(*r1);  // перемещает всех потомков r1 в r2
assert((*r1).empty());
assert((*r2).count() == 2);
```

## Примечания

- `before_erase` вызывается снизу вверх: сначала уведомляются наиболее
  глубокие потомки.
- `after_insert` вызывается сверху вниз: родитель уведомляется раньше
  своих дочерних узлов при вставке поддерева.
- `release()` выполняет глубокое копирование сырого узла; используйте его
  для извлечения поддерева и повторной вставки в другом месте.
- Межтрие `transfer()` вызывает `before_erase` на дереве-источнике и
  `after_insert` на дереве-назначении.
