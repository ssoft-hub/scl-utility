# Узел иерархии

Заголовочный узел дерева «родитель — дочерний» для C++20.

- Заголовочный файл: `#include <scl/utility/hierarchy/node.h>`

## Обзор

`scl::hierarchy::node<Value, Allocator>` — узел дерева, хранящий полезную
нагрузку типа `Value` и владеющий нулём или более дочерними узлами в
`std::list`.  Перемещение дочерних узлов между узлами выполняется за O(1)
(через `std::list::splice`); глубокое копирование поддерева — за O(N).

Итерация по узлу даёт его **непосредственных потомков** в виде ссылок
`node<Value>&`, что делает `node` стандартным range-контейнером, в котором
`value_type` — это сам `node<Value>`.

## Возможности

- Рекурсивная иерархия «родитель — дочерний» со стабильными итераторами
- O(1) перенос (splice) между узлами
- Глубокое копирование через конструктор копирования
- Полный итераторный интерфейс, совместимый с `std::list`
- Работает с любым шаблоном аллокатора

## Справочник API

### Создание

```cpp
node<int> root;               // полезная нагрузка по умолчанию
node<int> n(42);              // пробрасывание в конструктор Value
node<std::string> s("hello");
```

### Доступ к полезной нагрузке

```cpp
reference_type       value() noexcept;
const_reference_type value() const noexcept;
```

### Доступ к родителю

```cpp
node_reference_type       parent() noexcept;       // UB, если нет родителя
node_const_reference_type parent() const noexcept;
```

Используйте `scl::hierarchy::has_parent(n)` перед вызовом `parent()`.

### Запросы к дочерним узлам

```cpp
bool       empty()  const noexcept;
node_count count()  const noexcept;
node_count size()   const noexcept; // псевдоним

node_reference_type       first() noexcept;
node_const_reference_type first() const noexcept;
node_reference_type       last()  noexcept;
node_const_reference_type last()  const noexcept;
```

### Вставка дочерних узлов

```cpp
iterator append (/* аргументы Value */);   // конструирует в конец
iterator prepend(/* аргументы Value */);   // конструирует в начало
iterator emplace(const_iterator pos, /* аргументы Value */);
```

### Удаление дочерних узлов

```cpp
iterator       erase(iterator pos) noexcept;
iterator       erase(iterator first, iterator last) noexcept;
const_iterator erase(const_iterator pos) noexcept;
const_iterator erase(const_iterator first, const_iterator last) noexcept;
void           clear() noexcept;

node take      (const_iterator pos) noexcept;  // глубокое копирование с извлечением
node take_first() noexcept;
node take_last () noexcept;
```

### Перенос дочерних узлов (O(1) splice)

```cpp
void transfer(node& from);
void transfer(node& from, const_iterator from_pos);
void transfer(node& from, const_iterator from_first, const_iterator from_last);
void transfer(const_iterator before, node& from);
void transfer(const_iterator before, node& from, const_iterator from_pos);
void transfer(const_iterator before, node& from, const_iterator from_first, const_iterator from_last);
```

### Итераторы

Стандартные `begin/end/cbegin/cend` по непосредственным потомкам.

## Примеры использования

### Пример 1: Построение простой иерархии

```cpp
using Node = scl::hierarchy::node<std::string>;

Node root("root");
auto child_it = root.append("child");
child_it->append("grandchild");

for (auto& c : root)
    std::cout << c.value() << '\n'; // "child"
```

### Пример 2: Запрос отношений

```cpp
Node parent("p");
auto it = parent.append("c");

assert(scl::hierarchy::has_parent(*it));
assert(scl::hierarchy::is_parent_of(parent, *it));
assert(scl::hierarchy::is_ancestor_of(parent, *it));
```

### Пример 3: Перенос дочерних узлов

```cpp
Node src;
src.append(1);
src.append(2);

Node dst;
dst.transfer(src);          // переносит всех потомков; src становится пустым
assert(src.empty());
assert(dst.count() == 2);
```

### Пример 4: Извлечение поддерева

```cpp
Node parent;
auto it = parent.append("child");
it->append("grandchild");

Node extracted = parent.take_first();
// extracted — глубокая копия бывшего первого дочернего поддерева
assert(parent.empty());
assert(extracted.count() == 1); // grandchild включён
```

## Примечания

- `parent()` возвращает висячую ссылку, если у узла нет родителя.
  Всегда проверяйте с помощью `scl::hierarchy::has_parent()`.
- `take()` выполняет глубокое копирование (вызывает конструктор копирования),
  а не перемещение. Для O(1) массового перемещения используйте `transfer()`.
- Конструктор копирования рекурсивно глубоко копирует всех потомков.
- Конструктор перемещения передаёт дочерние узлы и обновляет их указатели
  на родителя.
