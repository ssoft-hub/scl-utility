# Узел иерархии

Заголовочный узел древовидной структуры с хранением потомков.

- Заголовочный файл: `#include <scl/utility/hierarchy/node.h>`

## Обзор

`scl::hierarchy::node<Payload, Allocator>` — узел дерева, который хранит
внутреннее значение типа `Payload` и владеет нулём или более дочерними узлами в
`std::list`. Перемещение потомков между узлами выполняется за O(1) (через
`std::list::splice`); глубокое копирование поддерева — за O(N).

При обходе узел возвращает свои **непосредственные дочерние узлы** как ссылки
`node &`, что делает `node` стандартным range-контейнером, у которого
`value_type` — это сам `node`. Внутреннее значение доступно через `get()`, а не
при обходе.

## Возможности

- Рекурсивная структура «узел и его потомки» с обнуляемым обратным указателем на
  родительский узел
- Перенос потомков между узлами за O(1) (splice)
- Глубокое копирование конструктором копирования; перемещение за O(1)
- Полный интерфейс итераторов, совместимый с `std::list`
- Работа с любым шаблоном аллокатора

## Справочник API

### Создание

```cpp
node<int> root;                 // значение по умолчанию
node<int> n(42);                // аргумент пробрасывается в конструктор Payload
node<std::string> s("hello");
```

Конструктор копирования выполняет глубокое копирование всего поддерева;
конструктор перемещения переносит список потомков за O(1) и переписывает их
указатели на родительский узел. Присваивание (копирующее и перемещающее)
**удалено** — для переноса потомков между узлами используйте `transfer()`.

### Доступ к внутреннему значению

```cpp
payload_reference       get() noexcept;         // изменяемое внутреннее значение
const_payload_reference get() const noexcept;   // неизменяемое внутреннее значение
```

### Доступ к родительскому узлу

```cpp
bool            has_parent() const noexcept;
reference       parent() noexcept;              // UB, если !has_parent()
const_reference parent() const noexcept;
```

Проверяйте `has_parent()` перед вызовом `parent()`. Тип обратной ссылки на
родителя имеет псевдоним `node_pointer = node *`.

### Запросы к дочерним узлам

```cpp
bool      empty() const noexcept;
size_type size()  const noexcept;

reference       front() noexcept;
const_reference front() const noexcept;
reference       back()  noexcept;
const_reference back()  const noexcept;
```

### Вставка дочерних узлов

```cpp
iterator push_back (Argument && argument);      // копирование/перемещение узла в конец
iterator push_front(Argument && argument);      // копирование/перемещение узла в начало
iterator emplace_back (Arguments &&... args);   // конструирование значения потомка на месте
iterator emplace_front(Arguments &&... args);
iterator insert (const_iterator where, Argument && argument);
iterator emplace(const_iterator where, Arguments &&... args);
```

Вся вставка проходит через `emplace`; каждый вариант возвращает `iterator` на
вставленный узел (намеренное отличие от `std::list`, избавляющее от линейного
поиска позиции).

### Удаление дочерних узлов

```cpp
void     pop_back()  noexcept;
void     pop_front() noexcept;
iterator erase(const_iterator position) noexcept;
iterator erase(const_iterator first, const_iterator last) noexcept;
void     clear() noexcept;

node take      (const_iterator position) noexcept;   // отсоединить и вернуть отдельный узел
node take_first() noexcept;
node take_last () noexcept;
```

`take()` перемещает потомка наружу (без родителя); его собственные потомки
сохраняют указатели на родительский узел.

### Перенос дочерних узлов (O(1) splice)

```cpp
void transfer(node & from);
void transfer(node & from, const_iterator first);
void transfer(node & from, const_iterator first, const_iterator last);
void transfer(const_iterator where, node & from);
void transfer(const_iterator where, node & from, const_iterator first);
void transfer(const_iterator where, node & from, const_iterator first, const_iterator last);
```

### Итераторы

Стандартные `begin/end/cbegin/cend` и обратные варианты по непосредственным
дочерним узлам.

## Примеры использования

### Пример 1: построение небольшой иерархии

```cpp
using Node = scl::hierarchy::node<std::string>;

Node root("root");
auto child_it = root.emplace_back("child");
child_it->emplace_back("grandchild");

for (auto & c : root)
    std::cout << c.get() << '\n'; // "child"
```

### Пример 2: запрос отношений

```cpp
Node parent("p");
auto it = parent.push_back("c");

assert(scl::hierarchy::has_parent(*it));
assert(scl::hierarchy::is_parent_of(parent, *it));
assert(scl::hierarchy::is_ancestor_of(parent, *it));
```

### Пример 3: перенос дочерних узлов

```cpp
Node src;
src.emplace_back(1);
src.emplace_back(2);

Node dst;
dst.transfer(src);          // переносит всех потомков; src становится пустым
assert(src.empty());
assert(dst.size() == 2);
```

### Пример 4: извлечение поддерева

```cpp
Node parent;
auto it = parent.emplace_back("child");
it->emplace_back("grandchild");

Node extracted = parent.take_first();
assert(parent.empty());
assert(extracted.size() == 1);   // потомок перенесён вместе с узлом
assert(!extracted.has_parent());
```

## Примечания

- `parent()` разыменовывает нулевой указатель, если у узла нет родителя. Всегда
  проверяйте через `scl::hierarchy::has_parent()`.
- Конструктор копирования рекурсивно глубоко копирует всех потомков; результат не
  имеет родителя.
- Конструктор перемещения переносит потомков за O(1) и переписывает их указатели
  на новый родительский узел.
- `take()` перемещает потомка из дерева; для массового переноса за O(1)
  используйте `transfer()`.

## Смотрите также

- [`example/hierarchy_node/node_example.cpp`](../../../example/hierarchy_node/node_example.cpp) —
  рабочая версия примеров выше (оргструктура, реорганизация через
  `take()`/`transfer()`).
