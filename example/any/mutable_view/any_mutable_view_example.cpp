/**
 * @example any_mutable_view_example.cpp
 * @brief Demonstrates the writing ScL Any view — scl::any_mutable_view.
 *
 * scl::any_mutable_view is scl::any_view with the right to write. Both may be
 * stored — a field, a container element, a return value — and both cost two
 * pointers; only this one hands out a pointer a caller may write through.
 *
 * The binding is narrower in exchange: a const object has no write to grant and
 * a temporary would not outlive the view, so both are refused. What the view
 * gains is a write that survives the call which set it up, which is what a
 * parameter-only scl::any_arg cannot offer.
 */

#include <scl/utility/any.h>
#include <scl/utility/preprocessor/rtti.h>

#if SCL_HAS_RTTI
#include <any>
#endif
#include <iostream>
#include <string>
#include <unordered_map>

// ============================================================================
// Pattern 1 — a write that outlives the call which set it up
// ============================================================================

//! [stored_write]
class binder
{
public:
    template <typename Type>
    void bind(::std::string name, Type & slot)
    {
        m_slots.insert_or_assign(::std::move(name), ::scl::any_mutable_view{slot});
    }

    template <typename Type>
    bool fill(::std::string const & name, Type value) const
    {
        auto const found = m_slots.find(name);
        if (found == m_slots.end())
            return false;

        auto * const target = ::scl::any_cast<Type>(&found->second);
        if (target == nullptr)
            return false;

        *target = ::std::move(value);
        return true;
    }

private:
    ::std::unordered_map<::std::string, ::scl::any_mutable_view> m_slots;
};
//! [stored_write]

static void show_stored_write()
{
    int jobs = 1;
    ::std::string target{"debug"};

    binder bound;
    bound.bind("--jobs", jobs);
    bound.bind("--target", target);

    ::std::cout << "  filled --jobs? " << bound.fill("--jobs", 8) << '\n'; // 1
    ::std::cout
        << "  filled --target? " << bound.fill("--target", ::std::string{"release"}) // 1
        << '\n';
    ::std::cout << "  wrong type refused? " << !bound.fill("--jobs", 1.5) << '\n'; // 1

    ::std::cout << "  jobs == " << jobs << ", target == \"" << target << "\"\n";
}

// ============================================================================
// Pattern 2 — the view's own constness forbids rebinding, not writing
// ============================================================================

//! [const_view_writes]
static void bump(::scl::any_mutable_view const & view)
{
    if (auto * const number = ::scl::any_cast<int>(&view))
        ++*number;
    else
        ::std::cout << "  refused: the referent is not a writable int\n";
}
//! [const_view_writes]

// ============================================================================
// Pattern 3 — narrowing towards the reading handles
// ============================================================================

static void report(::scl::any_view value)
{
    ::std::cout << "  read as " << value.type_name() << '\n';
}

static void bump_argument(::scl::any_arg value)
{
    if (auto * const number = ::scl::any_cast<int>(&value))
        ++*number;
}

static void show_narrowing()
{
    int number = 41;
    ::scl::any_mutable_view const view{number};

    report(view);        // narrowed to reading
    bump_argument(view); // still writes: an argument adopts the binding as it stands

    ::std::cout << "  number == " << number << '\n'; // 42
}

// ============================================================================
// Pattern 4 — a branch chain writes through the same view
// ============================================================================

static void show_switch()
{
    int number = 41;
    ::scl::any_mutable_view const view{number};

    ::scl::any_switch<>().in_case<int &>([](int & reached) {
        ++reached;
    }).in_case<::std::string &>([](::std::string & reached) {
        reached += "!";
    }).apply(view);

    ::std::cout << "  number == " << number << '\n'; // 42
}

// ============================================================================
// Pattern 5 — writing during constant evaluation
// ============================================================================

// A storable view carries no anchor of its own, so the caller declares one; over an object
// an scl::any owns the holder already stands in for it.
static constexpr int incremented() noexcept
{
    int value = 41;
    ::scl::any_anchor const anchor{value};
    ::scl::any_mutable_view const view{anchor};

    if (auto * const reached = ::scl::any_cast<int>(&view))
        ++*reached;
    return value;
}

static constexpr int incremented_in_owner()
{
    ::scl::any owner{41};
    ::scl::any_mutable_view const view{owner};

    if (auto * const reached = ::scl::any_cast<int>(&view))
        ++*reached;
    return *::scl::any_cast<int const>(&owner);
}

int main()
{
    ::std::cout << "=== A stored write ===\n";
    show_stored_write();

    ::std::cout << "\n=== A const view still writes ===\n";
    int number = 41;
    bump(::scl::any_mutable_view{number});
    ::std::cout << "  number == " << number << '\n'; // 42
    ::std::string text{"not an int"};
    bump(::scl::any_mutable_view{text}); // type mismatch: refused

#if SCL_HAS_RTTI
    ::std::cout << "\n=== Writing into a boxed object ===\n";
    ::std::any boxed{7};
    ::scl::any_mutable_view const over_any{boxed};
    *::scl::any_cast<int>(&over_any) = 11;
    ::std::cout << "  boxed == " << ::std::any_cast<int>(boxed) << '\n'; // 11
#endif

    ::std::cout << "\n=== Narrowing towards the reading handles ===\n";
    show_narrowing();

    ::std::cout << "\n=== A branch chain that writes ===\n";
    show_switch();

    ::std::cout << "\n=== Writing during constant evaluation ===\n";
    static_assert(incremented() == 42);
    static_assert(incremented_in_owner() == 42);
    ::std::cout << "  through an anchor: " << incremented() << '\n';           // 42
    ::std::cout << "  over an owning any: " << incremented_in_owner() << '\n'; // 42

    return 0;
}
