/**
 * @example preprocessor_counter_example.cpp
 * @brief Counts at compile time, one counter per tag.
 *
 * Each SCL_COUNTER_NEXT adds one to the counter of its tag for the rest of the translation unit,
 * and SCL_COUNTER_VALUE reads the count at the point where it stands.
 */

#include <scl/utility/preprocessor/counter.h>

#include <iostream>

//! [tags]
struct apples
{};

struct pears
{};
//! [tags]

//! [count]
static_assert(SCL_COUNTER_VALUE(apples) == 0);

SCL_COUNTER_NEXT(apples)
SCL_COUNTER_NEXT(apples)
SCL_COUNTER_NEXT(pears)

static_assert(SCL_COUNTER_VALUE(apples) == 2);
static_assert(SCL_COUNTER_VALUE(pears) == 1);
//! [count]

//! [template_tag]
template <typename Type>
struct per_type
{};

SCL_COUNTER_NEXT(per_type<int>)

static_assert(SCL_COUNTER_VALUE(per_type<int>) == 1);
static_assert(SCL_COUNTER_VALUE(per_type<long>) == 0);
//! [template_tag]

int main()
{
    std::cout << "apples: " << SCL_COUNTER_VALUE(apples) << '\n';
    std::cout << "pears : " << SCL_COUNTER_VALUE(pears) << '\n';
}
