# Дерево иерархии

Дерево с уведомлением наблюдателя об изменениях значений и структуры.

- Заголовочный файл: `#include <scl/utility/hierarchy/tree.h>`

## Обзор

`scl::hierarchy::tree<Payload, Observer, Allocator>` владеет корневым списком
`std::list` из узлов `node<Payload, Allocator>` и направляет все структурные
изменения через политику `Observer<tree>`. «Несколько корней» означает именно
это: дерево хранит *список* корневых узлов (лес), а не единственный обязательный
корень — можно вызвать `push_back` несколько раз и получить несколько независимых
корней, каждый со своим поддеревом.

Доступ к узлам осуществляется через прокси `reference` / `const_reference`,
которые объединяют ссылку на дерево со ссылкой на узел, так что узел нельзя
изменить без уведомления дерева. Композиция нескольких наблюдателей в одного
рассмотрена отдельно в [observer_tuple.md](observer_tuple.md).

## Возможности

- Уведомление наблюдателя при каждой вставке, удалении, очистке и изменении
  внутреннего значения
- Несколько корней (лес): `push_back`/`push_front` добавляют независимые
  корневые поддеревья
- Прокси `reference`/`const_reference` повторяют полный API управления
  дочерними узлами из `node`, направляя каждое изменение через наблюдателя
- O(1) `transfer` (splice) как на уровне корня, так и через `reference`, в
  пределах одного дерева или между деревьями
- `remove()` отсоединяет узел независимо от того, где он живёт — у родителя
  или в списке корней — без необходимости вызывающему коду это знать
- Полный интерфейс итераторов, совместимый с `std::list`, по корневым узлам

## Контракт наблюдателя

```cpp
template <typename Tree>
struct my_observer {
    void on_insert(typename Tree::iterator);              // после вставки узла
    void on_erase (typename Tree::const_iterator);        // перед удалением узла
    void on_clear ();                                     // перед очисткой дерева
    void on_change(typename Tree::const_payload_reference old_value,
                   typename Tree::const_payload_reference new_value);
};
```

- `on_insert` вызывается сверху вниз: при вставке поддерева родительский узел
  уведомляется раньше своих потомков.
- `on_erase` вызывается снизу вверх: сначала уведомляется самый глубокий потомок.
- Шаблонный параметр `Observer` проверяется по `concepts::observer` в момент
  инстанциации — несоответствующий тип не компилируется с понятным сообщением,
  а не стеной ошибок подстановки.

## Справочник API

### tree

```cpp
template <typename Payload,
          template <typename> class Observer,
          template <typename> class Allocator = std::allocator>
class tree;
```

Создаётся по умолчанию; пустое. Других конструкторов нет — дерево строится
вставкой.

#### Доступ к наблюдателю

```cpp
observer_reference       get_observer() noexcept;
observer_const_reference get_observer() const noexcept;
```

#### Запросы к корневым узлам

```cpp
bool      empty() const noexcept;
size_type size()  const noexcept;

reference       front() noexcept;      // а также back()
const_reference front() const noexcept;
```

#### Вставка корневых узлов

```cpp
iterator push_back (Argument && argument);          // добавить корень в конец; вызывает on_insert
iterator push_front(Argument && argument);          // добавить корень в начало; вызывает on_insert
iterator emplace_back (Arguments &&... arguments);  // сконструировать значение корня на месте
iterator emplace_front(Arguments &&... arguments);
iterator insert (const_iterator where, Argument && argument);
iterator emplace(const_iterator where, Arguments &&... arguments);
```

#### Удаление корневых узлов

```cpp
void     pop_back()  noexcept;
void     pop_front() noexcept;
Iterator erase(Iterator position) noexcept;              // вызывает on_erase снизу вверх
Iterator erase(Iterator first, Iterator last) noexcept;
void     clear() noexcept;                                // вызывает on_clear

void remove(Iterator position);   // отсоединяет от родителя, либо из списка
                                   // корней, если у position нет родителя —
                                   // вызывающему коду не нужно знать, что именно
```

#### Перенос (O(1) splice, уровень корня)

```cpp
void transfer(tree & from);
void transfer(tree & from, const_iterator first);
void transfer(tree & from, const_iterator first, const_iterator last);
void transfer(const_iterator where, tree & from);
void transfer(const_iterator where, tree & from, const_iterator first);
void transfer(const_iterator where, tree & from, const_iterator first, const_iterator last);
```

Переносит корневые поддеревья между двумя деревьями (или в пределах одного).
Межтрое перенос вызывает `on_erase` на дереве-источнике и `on_insert` на
дереве-назначении для каждого перенесённого узла.

#### Итераторы

Стандартные `begin/end/cbegin/cend` и обратные варианты по корневым узлам.

### reference (изменяемый прокси узла)

Никогда не создаётся напрямую — получается разыменованием `tree::iterator`
либо возвращается методами вставки `tree`/`reference`.

#### Доступ к внутреннему значению

```cpp
const_payload_reference value() const noexcept;     // внутреннее значение узла
void set_value(Arguments &&... arguments);          // заменить значение; вызывает on_change
```

#### Тождественность и доступ к самому узлу

```cpp
tree_reference       tree() noexcept;               // владеющее дерево
const_tree_reference tree() const noexcept;
const_node_reference leaf() const noexcept;          // сам узел
bool operator==(const_reference other) const noexcept;   // тождественность по указателю на узел
```

#### Доступ к родительскому узлу

```cpp
bool            has_parent() const noexcept;
reference       parent() noexcept;
const_reference parent() const noexcept;
```

#### Запросы к дочерним узлам

```cpp
bool      empty() const noexcept;
size_type size()  const noexcept;
reference front() noexcept;               // а также back()
const_reference front() const noexcept;
```

#### Вставка

```cpp
iterator push_back (Argument && argument);          // добавить потомка в конец; вызывает on_insert
iterator push_front(Argument && argument);          // добавить потомка в начало; вызывает on_insert
iterator emplace_back (Arguments &&... arguments);  // сконструировать значение потомка на месте
iterator emplace_front(Arguments &&... arguments);
iterator insert (const_iterator where, Argument && argument);
iterator emplace(const_iterator where, Arguments &&... arguments);
```

#### Удаление и извлечение

```cpp
void     pop_back()  noexcept;
void     pop_front() noexcept;
Iterator erase(Iterator position) noexcept;         // вызывает on_erase снизу вверх
Iterator erase(Iterator first, Iterator last) noexcept;
void     clear() noexcept;                           // вызывает on_erase для каждого потомка

node take      (const_iterator position) noexcept;   // отсоединить; вызывает on_erase
node take_first() noexcept;
node take_last () noexcept;
```

#### Перенос (O(1) splice)

```cpp
void transfer(reference from);
void transfer(reference from, const_iterator first);
void transfer(reference from, const_iterator first, const_iterator last);
void transfer(const_iterator where, reference from);
void transfer(const_iterator where, reference from, const_iterator first);
void transfer(const_iterator where, reference from, const_iterator first, const_iterator last);
```

Межтрое перенос вызывает `on_erase` на дереве-источнике и `on_insert` на
дереве-назначении для каждого перенесённого узла.

#### Итераторы

Стандартные `begin/end/cbegin/cend` и обратные варианты по непосредственным
дочерним узлам.

### const_reference

Доступная только для чтения версия `reference`, получается разыменованием
`tree::const_iterator` либо преобразованием из `reference`. Повторяет
доступную только для чтения часть `reference`:

```cpp
const_payload_reference value() const noexcept;
const_tree_reference    tree()  const noexcept;
const_node_reference    leaf()  const noexcept;

bool            has_parent() const noexcept;
const_reference parent()     const noexcept;

bool      empty() const noexcept;
size_type size()  const noexcept;
const_reference front() const noexcept;   // а также back()

bool operator==(const_reference other) const noexcept;   // тождественность по указателю на узел

// begin/end/cbegin/cend и обратные варианты по непосредственным дочерним узлам
```

У `const_reference` нет ни изменения, ни вставки, ни удаления, ни переноса.

## Примеры использования

### Пример 1: простой наблюдатель

```cpp
template <typename Tree>
struct logger {
    void on_insert(typename Tree::iterator it)
    { std::cout << "insert: " << (*it).value() << '\n'; }
    void on_erase(typename Tree::const_iterator it)
    { std::cout << "erase: " << (*it).value() << '\n'; }
    void on_clear() { std::cout << "clear\n"; }
    void on_change(int old_v, int new_v)
    { std::cout << "change: " << old_v << " -> " << new_v << '\n'; }
};

scl::hierarchy::tree<int, logger> t;
auto root = t.push_back(1);    // insert: 1
(*root).push_back(2);          // insert: 2
(*root).set_value(10);         // change: 1 -> 10
t.clear();                     // clear
```

### Пример 2: запрос иерархии через reference

Свободные функции алгоритмов из `algorithm.h` находят `reference`/
`const_reference` через ADL (см. [algorithm.md](algorithm.md)) — вызывайте их
напрямую, без разворачивания:

```cpp
scl::hierarchy::tree<std::string, logger> t;
auto root  = t.push_back("root");
auto child = (*root).push_back("child");
auto grand = (*child).push_back("grand");

assert(scl::hierarchy::is_ancestor_of(*root, *grand));
assert(scl::hierarchy::are_sibling(*child, *child));
```

### Пример 3: перенос между reference

```cpp
scl::hierarchy::tree<int, logger> t;
auto r1 = t.push_back(0);
auto r2 = t.push_back(0);
(*r1).push_back(1);
(*r1).push_back(2);

(*r2).transfer(*r1);  // переносит всех потомков r1 в r2
assert((*r1).empty());
assert((*r2).size() == 2);
```

### Пример 4: remove() отсоединяет по тождественности, от родителя или из списка корней

```cpp
scl::hierarchy::tree<int, logger> t;
auto root  = t.push_back(1);          // insert: 1
auto child = (*root).push_back(2);    // insert: 2

t.remove(child);    // у child есть родитель -> удаляется через (*root), не через t
assert((*root).empty());

t.remove(root);      // у root нет родителя -> удаляется прямо из списка корней
assert(t.empty());
```

## Примечания

- `on_erase` вызывается снизу вверх: сначала уведомляется самый глубокий потомок.
- `on_insert` вызывается сверху вниз: при вставке поддерева родительский узел
  уведомляется раньше потомков.
- Межтрое `transfer()` вызывает `on_erase` на дереве-источнике и `on_insert` на
  дереве-назначении.
- `reference`/`const_reference` — это прокси, а не владеющие хендлы: они
  остаются валидными ровно столько, сколько существует исходный узел.

## Смотрите также

- [`example/hierarchy_tree/tree_example.cpp`](../../../example/hierarchy_tree/tree_example.cpp) —
  рабочая версия: доска задач с двумя наблюдателями, объединёнными через
  `observer_tuple`, — индексы остаются синхронными при `set_value()` и при
  переносе поддерева между родителями (`transfer()`).
