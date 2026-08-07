/**
 * @example hierarchy.cpp
 * @brief Quick start: a parent-child tree, its relationships and its observer.
 */

//! [quick_start]
#include <scl/utility/hierarchy.h>

#include <iostream>
#include <string>

using person = ::scl::hierarchy::node<::std::string>;

// Every insertion, erasure and payload change reaches the observer the tree carries.
template <typename Tree>
struct headcount
{
    int size = 0;

    void on_insert(Tree::iterator) { ++size; }
    void on_erase(Tree::const_iterator) { --size; }
    void on_clear() { size = 0; }
    void on_change(Tree::const_payload_reference, Tree::const_payload_reference) {}
};

using company = ::scl::hierarchy::tree<::std::string, headcount>;

int main()
{
    person lead{"lead"};
    person & senior = *lead.emplace_back("senior");
    senior.emplace_back("junior");

    person const & junior = senior.front();
    ::std::cout
        << ::std::boolalpha                                     //
        << ::scl::hierarchy::is_parent_of(senior, junior)       // true
        << ::scl::hierarchy::is_ancestor_of(lead, junior)       // true
        << ::scl::hierarchy::is_parent_of(lead, junior) << '\n' // false
        ;

    company staff;
    company::reference root = *staff.push_back("lead");
    root.push_back("senior");
    ::std::cout << staff.get_observer().size << '\n'; // 2
}
//! [quick_start]
