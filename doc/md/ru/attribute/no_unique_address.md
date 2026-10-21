# `SCL_NO_UNIQUE_ADDRESS`

Пустой член, помеченный `SCL_NO_UNIQUE_ADDRESS`, может разделить адрес с другим членом и тогда
не занимает собственной памяти.

- Заголовок: `#include <scl/utility/attribute/no_unique_address.h>`

```cpp
#define SCL_NO_UNIQUE_ADDRESS /* see below */
```

## Семантика

- **Что сообщается компилятору:** члену не нужен адрес, отличный от адресов других членов.
  Компилятор может разместить пустой член по адресу другого, и тогда в классе под него не отводится
  ни одного байта.
- **Форма записи по цели:** `__has_cpp_attribute(no_unique_address)` равно `0` на MSVC,
  а также на Clang 22, включая clang-cl, для ABI MSVC. Раскладка меняется там
  только с `[[msvc::no_unique_address]]`. GCC и Clang принимают стандартную форму для ABI Itanium.
- **Два члена одного типа:** два члена одного типа даже с макросом имеют разные адреса, но каждый
  из них может разделить адрес с членом другого типа.
- **Место:** макрос записывается перед объявлением нестатического члена данных.

| Условие, первое совпавшее | Раскрытие |
|---|---|
| `__has_cpp_attribute(no_unique_address)` | `[[no_unique_address]]` |
| `__has_cpp_attribute(msvc::no_unique_address)`, `msvc` не является макросом | `[[msvc::no_unique_address]]` |
| ничего из перечисленного | пусто, и пустой член занимает байт |

Макрос раскрывается в `[[no_unique_address]]` на GCC и в `[[msvc::no_unique_address]]` на MSVC.
Макрос раскрывается на Clang 22 в `[[no_unique_address]]` для ABI Itanium
и в `[[msvc::no_unique_address]]` для ABI MSVC.

## Определения до заголовка

Определение макроса, сделанное до его заголовка, сохраняется только в виде одного из раскрытий ниже
для данного компилятора. Сборка с любым другим текстом завершается с ошибкой, как описано
на [странице группы](index.md#замена-макроса).

| Макрос | GCC | Clang, clang-cl | MSVC |
|---|---|---|---|
| `SCL_NO_UNIQUE_ADDRESS` | `[[no_unique_address]]`, пусто | `[[no_unique_address]]`, пусто; для ABI MSVC `[[msvc::no_unique_address]]`, пусто | `[[msvc::no_unique_address]]`, пусто |

## Примеры

Класс, пустой член которого не занимает собственной памяти:

<!-- snippet: example/attribute/no_unique_address/attribute_no_unique_address_example.cpp no_unique_address -->
```cpp
struct no_policy
{};

struct counter
{
    SCL_NO_UNIQUE_ADDRESS no_policy policy;
    int value;
};

static_assert(sizeof(counter) == sizeof(int));
```

## Смотрите также

- [`example/attribute/no_unique_address/attribute_no_unique_address_example.cpp`](../../../../example/attribute/no_unique_address/attribute_no_unique_address_example.cpp)

---

Назад: [`SCL_NORETURN`](noreturn.md) | Далее: [`SCL_UNSEQUENCED`](unsequenced.md) |
[К группе](index.md) | [К главной странице](../Main.md)
