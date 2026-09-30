# Runtime Benchmarks

- Suite: `benchmark/runtime/`
- Configure: `cmake --preset <preset> -DSCL_BUILD_BENCHMARKS=ON`
- Timing target: `utility_runtime_gbench`
- Size target: `utility_runtime_size`

## What this page answers

What one call of each function template of the group costs, what the cost rests on, and which
optimisation attribute earns a place in the code of the group. An attribute that speeds a call up
can grow the code, so the size of the part of the group a bare-metal build admits is measured
beside the time.

## Method

### Pairing

Every variant is compared against a baseline built from the same sources in the same round. The
baseline runs twice in each round, first and last, and the rounds run back to back, so a machine
that warms up over the hour warms up under both members of every pair.

The difference between the two runs of the baseline is the null channel: two runs of one binary
must agree, so whatever separates them is the floor below which no difference means anything. A
variant counts only where it moves a case the same way in every round and by more than the 90th
percentile of the null channel of its compiler.

### Settings

Each case runs 15 repetitions of at least 0.5 seconds, and the median of the repetitions is the
value of the case. Each compiler runs 4 rounds. On Windows the process is pinned to one logical
processor and raised to high priority. On Linux, under WSL 2 on the same machine, the process is
pinned by `taskset` and keeps the normal priority.

| System | Compiler | Library | Flags |
|---|---|---|---|
| Windows | GCC 13.1 (MinGW) | libstdc++ | `-O3`, `-std=gnu++20` |
| Windows | Clang 22.1 | MSVC | `-O3`, `-std=gnu++20`, and `-std=gnu++26` |
| Windows | MSVC 19.44 | MSVC | `/O2 /Ob2`, `/std:c++20`, and `/std:c++latest` |
| Linux | GCC 16.0.1 | libstdc++ | `-O3`, `-std=c++20`, and `-std=c++26` |
| Linux | Clang 22.1 | libstdc++ | `-O3`, `-std=c++20`, and `-std=c++26` |

GCC 13.1 has no C++26 mode. In C++23 every attribute macro of the module expands to what it expands
to in C++20 on every compiler, so a C++23 build compiles the code of a C++20 one and is not
measured apart. On Linux both compilers take the path of `<cxxabi.h>`, which on Windows GCC alone
takes, and the measurements there cover the attributes the group applies and the cache of
demangled names.

| Case | What it calls |
|---|---|
| `type_name_dynamic` | `type_name(obj)` for an object of a derived class reached through a reference to its base |
| `type_name_template` | the same for an object of a specialization of a class template |
| `type_short_name_dynamic` | `type_short_name(obj)` for the object of `type_name_dynamic` |
| `type_short_name_template` | `type_short_name(obj)` for the object of `type_name_template` |
| `enum_string_decimal` | `enum_string(value)` for the value -3 of an `int` underlying type |
| `enum_string_decimal_wide` | `enum_string(value)` for the least value of `std::int64_t` |
| `enum_string_format` | `enum_string(value, format)` with a function object spelling the number in hexadecimal |

## Cost per call

The median time of one call, in nanoseconds:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 | GCC 16.0.1, Linux | Clang 22.1, Linux |
|---|---|---|---|---|---|
| `enum_string_decimal` | 5.9 | 6.0 | 13.6 | 6.3 | 9.3 |
| `enum_string_decimal_wide` | 61.3 | 64.2 | 71.9 | 40.0 | 38.3 |
| `enum_string_format` | 10.4 | 8.9 | 19.0 | 11.6 | 14.4 |
| `type_name_dynamic` | 42.7 | 47.9 | 49.6 | 18.6 | 21.4 |
| `type_name_template` | 43.3 | 48.0 | 49.5 | 18.6 | 20.9 |
| `type_short_name_dynamic` | 21.8 | 29.3 | 39.3 | 21.0 | 26.7 |
| `type_short_name_template` | 30.7 | 50.1 | 63.4 | 35.1 | 54.7 |

MSVC and Clang on the MSVC library have no `<cxxabi.h>`, so there `type_name(obj)` returns the
name `typeid` gives and demangles nothing.

## What the cost rests on

| Property | The alternative, measured |
|---|---|
| The scan for the last `::` of a name stops at it, unless a glued operator symbol such as `->>` stands on the way, and looks closer only at a quote and an angle bracket | a scan checking every character: the scan alone takes 421-454 ns instead of 20-41 ns with Clang on a name of a few dozen characters |
| `enum_string` writes the number by `std::to_chars` into a buffer on the stack | `std::format`: about twice the time of a call with Clang, and 15130 bytes of Cortex-M4 code instead of 1398 |
| The short name of the enumeration type is a constant of `enum_string` | the short name computed on every call: 2.3 to 4.5 times the time of a call |
| A demangled name is kept where `<cxxabi.h>` exists | demangling on every call, below |

The cache of demangled names against demangling on every call:

| Case | GCC 13.1 | GCC 16.0.1, Linux | Clang 22.1, Linux |
|---|---|---|---|
| `type_name_dynamic` | **-82.1%** | **-92.3%** | **-91.2%** |
| `type_name_template` | **-85.1%** | **-93.6%** | **-93.0%** |
| `type_short_name_dynamic` | **-92.1%** | **-92.4%** | **-90.2%** |
| `type_short_name_template` | **-90.8%** | **-90.2%** | **-86.0%** |

The cache is one for the whole program, under `std::mutex`, and is never destroyed, so a
destructor that runs after `main` returns still reads it. Its key is the mangled name rather than
the address of the `type_info` object, which a library loaded after another is unloaded may reuse.
Each thread keeps names in eight slots that have no destructor, a slot chosen by the address of the
type and matched by that address and the address of the mangled name, and takes the lock only for
a name its slot does not hold. The slots against the lock alone:

| Case | GCC 13.1 | GCC 16.0.1, Linux | Clang 22.1, Linux |
|---|---|---|---|
| `type_name_dynamic` | **-44.6%** | **-63.6%** | **-62.5%** |
| `type_name_template` | **-45.9%** | **-62.1%** | **-58.9%** |
| `type_short_name_dynamic` | **-60.3%** | **-59.9%** | **-56.7%** |
| `type_short_name_template` | **-53.0%** | **-46.9%** | **-35.7%** |

Two other forms of the cache are not used:

- A map of each thread in `thread_local` storage is destroyed before every object the thread
  created before its first call, and before every static object, so a destructor of such an object
  cannot ask for a name. With GCC on MinGW, a process whose threads end while holding a
  `thread_local` object with a destructor also crashes.
- `std::shared_mutex` in place of `std::mutex` makes a call 8-25% slower on Linux and 1.9 to 2.4
  times as slow with GCC 13.1: a shared lock costs more than a plain one for a lookup this short.
  With the `libwinpthread-1.dll` library of MSYS2, which Git for Windows ships as well, the shared
  lock alone takes 1300 ns.

## Attributes

The null channel of each compiler, in per cent of the value of a case:

| System | Compiler | median | 90th percentile | worst |
|---|---|---|---|---|
| Windows | Clang 22.1, C++20 | +0.06% | 3.32% | 5.42% |
| Windows | Clang 22.1, C++26 | -0.04% | 1.53% | 4.32% |
| Windows | GCC 13.1, C++20 | -0.16% | 1.52% | 2.11% |
| Windows | MSVC 19.44, C++20 | -0.12% | 1.13% | 2.02% |
| Windows | MSVC 19.44, C++26 | -0.04% | 1.91% | 6.62% |
| Linux | GCC 16.0.1, C++20 | -0.18% | 2.08% | 5.55% |
| Linux | GCC 16.0.1, C++26 | -0.66% | 2.31% | 5.19% |
| Linux | Clang 22.1, C++20 | +0.20% | 2.34% | 12.37% |
| Linux | Clang 22.1, C++26 | +0.02% | 2.01% | 3.59% |

A cell reads the median change of the case against the baseline of its round, in bold where it
clears the floor; `noise` where it does not, and `no code` where the macro expands to nothing on
that compiler, so the variant builds the code of the baseline.

### `SCL_INDETERMINATE` - applied

The buffer of the digits in `enum_string(value)` is declared without an initializer, since only the
digits `std::to_chars` writes are read. No compiler measured here has `[[indeterminate]]` yet, so
the macro expands to nothing, and the declaration alone leaves the buffer unset.

On Windows:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | **-3.3%** | **-3.6%** | **-5.8%** |
| `enum_string_decimal_wide` | **+2.6%** | **-5.1%** | noise |
| `enum_string_format` | noise | noise | noise |
| `type_name_dynamic` | noise | noise | noise |
| `type_name_template` | noise | noise | noise |
| `type_short_name_dynamic` | noise | noise | noise |
| `type_short_name_template` | noise | noise | noise |

On Windows in C++26, where reading a variable left unset is erroneous behaviour rather than
undefined:

| Case | Clang 22.1 | MSVC 19.44 |
|---|---|---|
| `enum_string_decimal` | noise | **-5.6%** |
| `enum_string_decimal_wide` | **-7.1%** | noise |
| `enum_string_format` | **+3.4%** | noise |
| `type_name_dynamic` | noise | noise |
| `type_name_template` | noise | noise |
| `type_short_name_dynamic` | noise | noise |
| `type_short_name_template` | noise | noise |

On Linux:

| Case | GCC 16.0.1, C++20 | GCC 16.0.1, C++26 | Clang 22.1, C++20 | Clang 22.1, C++26 |
|---|---|---|---|---|
| `enum_string_decimal` | **-4.1%** | **+4.3%** | **-3.7%** | **-4.2%** |
| `enum_string_decimal_wide` | **+2.3%** | noise | noise | noise |
| `enum_string_format` | noise | noise | noise | noise |

In C++20 the short number is 3-6% faster with every compiler. In C++26 it is 4-6% faster with MSVC
and with Clang on Linux, 4% slower with GCC 16.0.1, and unchanged with Clang on Windows. The
longest number is 5-7% faster with Clang on Windows and 2-3% slower with GCC in C++20. The code of
Cortex-M4 is 20 bytes smaller.

### `SCL_FORCE_INLINE` on the join - applied, except with MSVC

On the helper that joins the type and the number, on Windows:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | noise | **-40.3%** | **+6.8%** |
| `enum_string_decimal_wide` | noise | **-11.0%** | **+10.2%** |
| `enum_string_format` | **-1.7%** | **-27.6%** | **+6.3%** |
| `type_name_dynamic` | noise | noise | **-1.8%** |
| `type_name_template` | noise | noise | noise |
| `type_short_name_dynamic` | noise | noise | noise |
| `type_short_name_template` | noise | noise | noise |

On Linux:

| Case | GCC 16.0.1, C++20 | GCC 16.0.1, C++26 | Clang 22.1, C++20 |
|---|---|---|---|
| `enum_string_decimal` | **-45.5%** | **-38.5%** | **-11.8%** |
| `enum_string_decimal_wide` | **-7.8%** | **-8.2%** | **-13.1%** |
| `enum_string_format` | **-25.3%** | **-24.1%** | **-4.8%** |

Clang and GCC 16.0.1 do not inline the helper on their own and run `enum_string` up to 45% faster
when it is forced. GCC 13.1 gains 2% at most. MSVC inlines the helper on its own and runs 6-10%
slower when it is forced, so with MSVC the helper stays `inline`. The forced helper costs
28 bytes of Cortex-M4 code at `-Os` for each call site of `enum_string`.

### `SCL_FORCE_INLINE` elsewhere - not applied

On `short_name_from`, which scans for the short name, and on the lookup of the cache of demangled
names:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | noise | noise | **+1.9%** |
| `enum_string_decimal_wide` | noise | noise | noise |
| `enum_string_format` | **-2.6%** | noise | noise |
| `type_name_dynamic` | noise | noise | noise |
| `type_name_template` | noise | noise | noise |
| `type_short_name_dynamic` | noise | **-14.2%** | **-6.8%** |
| `type_short_name_template` | noise | **-7.2%** | **-5.0%** |

On every function of the group at once, the helper of the join included:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | **+6.4%** | **-40.6%** | **+48.2%** |
| `enum_string_decimal_wide` | noise | **-11.0%** | noise |
| `enum_string_format` | noise | **-28.9%** | **+51.5%** |
| `type_name_dynamic` | noise | noise | **-2.3%** |
| `type_name_template` | noise | noise | noise |
| `type_short_name_dynamic` | noise | **-12.1%** | **-8.0%** |
| `type_short_name_template` | **+5.1%** | **-8.3%** | **-2.4%** |

On the scan, Clang and MSVC gain 5-14% on `type_short_name(obj)` and GCC nothing, for about 330
bytes of x86-64 code at each call site past the first, and each type a caller names is a call site
of its own; the size is measured over 1 to 8 call sites with GCC 13.1 at `-Os`. That is not
a trade the library makes on the caller's behalf: a caller who wants the gain for one hot site
forces a wrapper of their own inline.

### `SCL_LIKELY` and `SCL_UNLIKELY` - not applied

The default case of the scan marked likely, its quote cases unlikely, and a successful demangling
likely:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | noise | **-7.1%** | noise |
| `enum_string_decimal_wide` | noise | **-5.6%** | noise |
| `enum_string_format` | noise | noise | noise |
| `type_name_dynamic` | noise | noise | noise |
| `type_name_template` | noise | noise | noise |
| `type_short_name_dynamic` | noise | noise | noise |
| `type_short_name_template` | **+5.0%** | noise | noise |

The two moves of `enum_string` fall on code the hints do not touch, since `enum_string` takes the
short name of its type at compile time: they belong to the layout of the code, and the one on the
scan goes the wrong way.

### `SCL_ASSUME` - not applied

The index of the scan assumed to lie inside the name, so a bounds check could go:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | noise | noise | noise |
| `enum_string_decimal_wide` | noise | noise | **+3.5%** |
| `enum_string_format` | noise | noise | noise |
| `type_name_dynamic` | noise | noise | noise |
| `type_name_template` | noise | noise | noise |
| `type_short_name_dynamic` | noise | noise | noise |
| `type_short_name_template` | noise | **+4.3%** | noise |

### `SCL_UNSEQUENCED` and `SCL_REPRODUCIBLE` - not applied

The character tests of the scan marked unsequenced, the symbol length and `short_name_from` marked
reproducible:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | noise | **-5.2%** | no code |
| `enum_string_decimal_wide` | noise | noise | no code |
| `enum_string_format` | noise | **+6.9%** | no code |
| `type_name_dynamic` | noise | noise | no code |
| `type_name_template` | noise | noise | no code |
| `type_short_name_dynamic` | noise | noise | no code |
| `type_short_name_template` | noise | noise | no code |

### `SCL_HOT` - not applied

On the four public function templates:

| Case | GCC 13.1 | Clang 22.1 | MSVC 19.44 |
|---|---|---|---|
| `enum_string_decimal` | noise | noise | no code |
| `enum_string_decimal_wide` | noise | noise | no code |
| `enum_string_format` | noise | noise | no code |
| `type_name_dynamic` | noise | noise | no code |
| `type_name_template` | noise | noise | no code |
| `type_short_name_dynamic` | noise | noise | no code |
| `type_short_name_template` | noise | noise | no code |

## Code size

`.text` of `runtime_size.cpp`, `arm-none-eabi-g++` 13.2.1, Cortex-M4, `-Os`, no RTTI and no
exceptions, as the `arm-none-eabi` preset builds it. With no RTTI the build admits `enum_string`
alone, at two call sites:

| Build | bytes |
|---|---|
| as shipped | 1398 |
| the helper of the join not forced inline | 1342 |
| the buffer of the digits zeroed | 1418 |
| the number spelled through `std::format` | 15130 |

The same source with GCC 13.1 for x86-64 at `-Os`, where `type_name(obj)` and `type_short_name(obj)`
build as well:

| Build | bytes |
|---|---|
| as shipped | 8272 |
| the helper of the join not forced inline | 8208 |
| the buffer of the digits zeroed | 8324 |

## Trading speed and size

The buffer left unset makes the code both faster and smaller, so a build has nothing to trade back
there. Predefining `SCL_INDETERMINATE` changes nothing: the buffer is left unset by its
declaration, and the macro only names that intent to a compiler that knows `[[indeterminate]]`.

The forced helper of the join trades 28 bytes for each call site of `enum_string`
for its time. A build that counts bytes before nanoseconds should predefine `SCL_FORCE_INLINE` as
`inline`, which gives the bytes back for every function the module forces inline.

[Back to the group](index.md)
