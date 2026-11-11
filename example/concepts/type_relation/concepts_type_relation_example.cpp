/**
 * @example concepts_type_relation_example.cpp
 * @brief Checks whether a class is a base class or a virtual base class of another class.
 *
 * The concept scl::concepts::base_of is declared everywhere, and the concept
 * scl::concepts::virtual_base_of is checked where the header declares it.
 */

#include <scl/utility/concepts/type_relation.h>

//! [types]
struct node
{};

struct left_branch : virtual node
{};

struct right_branch : virtual node
{};

struct joined
    : left_branch
    , right_branch
{};

struct plain : node
{};

struct hidden : private node
{};
//! [types]

//! [base_of]
static_assert(scl::concepts::base_of<node, plain>);
static_assert(scl::concepts::base_of<node, hidden>);
static_assert(scl::concepts::base_of<node, joined>);
static_assert(scl::concepts::base_of<node, node>);
static_assert(!scl::concepts::base_of<plain, node>);
static_assert(!scl::concepts::base_of<int, int>);
//! [base_of]

#if SCL_HAS_VIRTUAL_BASE_OF_CONCEPT
//! [virtual_base_of]
static_assert(scl::concepts::virtual_base_of<node, left_branch>);
static_assert(scl::concepts::virtual_base_of<node, joined>);
static_assert(!scl::concepts::virtual_base_of<node, plain>);
static_assert(!scl::concepts::virtual_base_of<node, node>);
static_assert(!scl::concepts::virtual_base_of<left_branch, node>);
//! [virtual_base_of]
#endif

int main() {}
