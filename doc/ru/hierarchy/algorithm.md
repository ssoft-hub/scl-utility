# Алгоритмы иерархии

Свободные функции для запроса структурных отношений между узлами иерархии.

- Заголовочный файл: `#include <scl/utility/hierarchy/algorithm.h>`

## Обзор

Набор шаблонных функций, выражающих структурные отношения между узлами. Они
работают с любым типом, подключённым через три безымянных вызова —
`adl_parent(value)`, `adl_has_parent(value)`, `adl_identity(value)` —
разрешаемых обычным поиском и
[ADL](https://en.cppreference.com/w/cpp/language/adl) (поиском, зависящим от
аргументов). Общего класса-трейта или generic-заголовка нет:
`scl::hierarchy::node` определяет свои три перегрузки прямо рядом с классом, в
`node.h`; `tree::reference`/`tree::const_reference` подключаются через
concept-ограниченный шаблон, объявленный в `tree.h` (их природа вложенного типа
шаблона класса делает невозможным назвать их напрямую в списке параметров
функции с выводом типа — см. комментарий над concept-ом `node_proxy` в
`tree.h`). Оба работают из коробки. Чужой тип подключается так же, как
собственные перегрузки node: определите `adl_parent`/`adl_has_parent`/
`adl_identity` для него в его собственном namespace.

## Справочник API

### `parent_of`

```cpp
template <typename Value>
constexpr decltype(auto) parent_of(Value && value);
```

Возвращает родительский узел `value` через безымянный вызов `adl_parent(value)`
(ADL), сохраняя категорию значения.

### `are_identical`

```cpp
template <typename First, typename Second>
constexpr bool are_identical(First && first, Second && second) noexcept;
```

Возвращает `true`, когда оба операнда обозначают **один и тот же узел**.
Тождественность — это непрозрачный маркер узла, обозначаемого аргументом,
получаемый через безымянный вызов `adl_identity(value)` (ADL). Результат не
зависит от категории значения и никогда не сводится к равенству значений —
разные объекты с равными значениями не тождественны.

### `has_parent`

```cpp
template <typename Value>
constexpr bool has_parent(Value && value);
```

Возвращает `true`, когда у `value` есть родительский узел (через безымянный
вызов `adl_has_parent(value)`, ADL). Никогда не разыменовывает нулевой
указатель на родителя.

### `is_parent_of`

```cpp
template <typename Parent, typename Value>
constexpr bool is_parent_of(Parent && parent, Value && value);
```

Возвращает `true`, когда `parent` — **непосредственный** родитель `value`.
Сначала проверяет `has_parent(value)`, поэтому `parent_of` не вычисляется для
узла без родителя.

### `is_ancestor_of`

```cpp
template <typename Parent, typename Value>
constexpr bool is_ancestor_of(Parent && parent, Value && value);
```

Возвращает `true`, когда `parent` — **прямой или транзитивный** предок `value`.
Поднимается по цепочке родителей; узел не является предком самого себя.
Сложность O(глубины).

### `are_sibling`

```cpp
template <typename First, typename Second>
constexpr bool are_sibling(First && first, Second && second);
```

Возвращает `true`, когда у обоих узлов один и тот же непосредственный родитель.
Возвращает `false`, если хотя бы один из узлов — корневой (без родителя).

## Примеры использования

### Пример 1: базовые проверки

```cpp
using Node = scl::hierarchy::node<int>;

Node parent;
auto it = parent.push_back(42);

assert( scl::hierarchy::has_parent(*it));
assert(!scl::hierarchy::has_parent(parent));
assert( scl::hierarchy::is_parent_of(parent, *it));
assert( scl::hierarchy::are_identical(parent, parent));
assert(!scl::hierarchy::are_identical(parent, *it));
```

### Пример 2: определение предка

```cpp
Node root;
auto child = root.push_back(1);
auto grand = child->push_back(2);

assert( scl::hierarchy::is_ancestor_of(root, *grand));
assert(!scl::hierarchy::is_ancestor_of(*grand, root));
```

### Пример 3: определение соседних узлов

```cpp
Node parent;
auto a = parent.push_back(1);
auto b = parent.push_back(2);

assert( scl::hierarchy::are_sibling(*a, *b));
```
