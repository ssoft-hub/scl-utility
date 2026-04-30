# Алгоритмы иерархии

Запросы структурных отношений между узлами иерархии.

- Заголовочный файл: `#include <scl/utility/hierarchy/algorithm.h>`

## Обзор

Набор шаблонных свободных функций, выражающих структурные отношения между
узлами.  Все функции работают с любым типом, у которого есть член `parent()`,
возвращающий ссылку на родителя.

Проверка идентичности использует `std::addressof` для lvalue-ссылок и
откатывается к `operator==` для значений.  Нулевой указатель на родителя
означает «нет родителя» — через проверку `addressof(x) != nullptr`.

## Справочник API

### `are_identical`

```cpp
template <typename FirstValue, typename SecondValue>
bool are_identical(FirstValue&& first, SecondValue&& second);
```

Возвращает `true`, если `first` и `second` ссылаются на **один и тот же
объект** (одинаковый адрес для ссылок, `operator==` для значений).

### `has_parent`

```cpp
template <typename Value>
bool has_parent(Value&& value);
```

Возвращает `true`, если у `value` есть родитель.  Реализован сравнением
`parent_of(value)` с самим собой — нулевой указатель на родителя даёт нулевой
адрес, который не проходит проверку `!= nullptr`.

### `is_parent_of`

```cpp
template <typename Parent, typename Value>
bool is_parent_of(Parent&& parent, Value&& value);
```

Возвращает `true`, если `parent` является **непосредственным** родителем
`value`.

### `is_ancestor_of`

```cpp
template <typename Parent, typename Value>
bool is_ancestor_of(Parent&& parent, Value&& value);
```

Возвращает `true`, если `parent` является **прямым или транзитивным**
предком `value`.  Рекурсивно обходит цепочку через `parent_of`.

### `are_sibling`

```cpp
template <typename ParentedFirst, typename ParentedSecond>
bool are_sibling(ParentedFirst&& first, ParentedSecond&& second);
```

Возвращает `true`, если `first` и `second` имеют одного родителя.  Для
корневых узлов возвращает `false` (нет родителя, нулевые адреса не совпадают).

## Примеры использования

### Пример 1: Базовые проверки

```cpp
using Node = scl::hierarchy::node<int>;

Node parent;
auto it = parent.append(42);

assert( scl::hierarchy::has_parent(*it));
assert(!scl::hierarchy::has_parent(parent));
assert( scl::hierarchy::is_parent_of(parent, *it));
assert( scl::hierarchy::are_identical(parent, parent));
assert(!scl::hierarchy::are_identical(parent, *it));
```

### Пример 2: Определение предка

```cpp
Node root;
auto child = root.append(1);
auto grand = child->append(2);

assert( scl::hierarchy::is_ancestor_of(root, *grand));
assert(!scl::hierarchy::is_ancestor_of(*grand, root));
```

### Пример 3: Определение «сестринских» узлов

```cpp
Node parent;
auto it1 = parent.append(1);
auto it2 = parent.append(2);

assert( scl::hierarchy::are_sibling(*it1, *it2));
```

## Настройка

Проверка идентичности настраивается через структуру политики `IdentityOf`.
Специализируйте её для собственных типов значений или ссылок, если сравнение
по умолчанию через `std::addressof` неподходяще.

```cpp
namespace scl::hierarchy {
    template <>
    struct identity_of<MyType, MyType> {
        static bool check(MyType a, MyType b) { return a.id() == b.id(); }
    };
}
```
