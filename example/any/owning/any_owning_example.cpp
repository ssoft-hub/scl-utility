/**
 * @example any_owning_example.cpp
 * @brief Demonstrates the owning ScL Any — scl::any and scl::basic_any.
 *
 * scl::any holds one object of any type, the way std::any does, in the two
 * configurations std::any cannot serve: it names types through scl::type_key
 * rather than typeid, so it compiles with RTTI disabled, and its whole
 * lifecycle runs during constant evaluation. It is two pointers wide.
 *
 * A small, nothrow-movable object lives inside the any itself; anything else is
 * allocated through the allocator that scl::basic_any takes as a parameter. A
 * stored type has to be destructible without throwing and constructible from
 * the arguments given; an immovable or non-copyable type is admitted. Copying
 * is therefore not a constructor: the type is move-only, and a copy is asked
 * for through try_copy().
 */

#include <scl/utility/any.h>

#include <iostream>
#include <memory>
#include <memory_resource>
#include <string>

// ============================================================================
// Pattern 1 — store, read, write
// ============================================================================

static void show_round_trip()
{
    ::scl::any value{::std::string{"Hello Any!"}};

    // Pointer form: a pointer to the stored object, or nullptr on a type
    // mismatch. Access follows the handle, so a non-const any grants writing.
    if (auto * text = ::scl::any_cast<::std::string>(&value))
        *text += " Again!";

    ::std::cout << "  " << *::scl::any_cast<::std::string>(&value) << '\n';
    ::std::cout << "  type_name=\"" << value.type_name() << "\"\n";
    ::std::cout << "  as double? " << (::scl::any_cast<double>(&value) != nullptr) << '\n'; // 0
}

// ============================================================================
// Pattern 2 — where the object lives, and what it costs
// ============================================================================

static void show_storage()
{
    ::std::cout
        << "  sizeof(scl::any) == " << sizeof(::scl::any)
        << " bytes, buffer_capacity == " << ::scl::any::buffer_capacity << '\n';

    // An int fits the buffer and never reaches the allocator; a std::string does
    // not fit and is allocated. The choice is made per type at compile time.
    ::scl::any const small{42};
    ::scl::any const large{::std::string{"too wide for the buffer"}};

    ::std::cout << "  small type_name=\"" << small.type_name() << "\"\n";
    ::std::cout << "  large type_name=\"" << large.type_name() << "\"\n";

    // A wider buffer is a template parameter away, at the price of size.
    using roomy_any = ::scl::basic_any<::std::allocator<::std::byte>, 48>;
    ::std::cout << "  sizeof(basic_any<allocator, 48>) == " << sizeof(roomy_any) << " bytes\n";
}

// ============================================================================
// Pattern 3 — the whole lifecycle during constant evaluation
// ============================================================================

//! [constant_evaluation]
static constexpr ::std::size_t stored_length()
{
    ::scl::any value{::std::string{"Hello Any!"}};
    ::std::size_t const length = ::scl::any_cast<::std::string>(&value)->size();

    value.reset();
    return length;
}
//! [constant_evaluation]

static void show_constant_evaluation()
{
    static_assert(stored_length() == 10U, "an owning any runs at compile time");

    ::std::cout << "  stored_length() folded to " << stored_length() << " at compile time\n";
}

// ============================================================================
// Pattern 4 — move-only by design, copies on request
// ============================================================================

//! [copying]
static void show_copying()
{
    ::scl::any const text{::std::string{"copy me"}};

    // is_copyable() asks about the stored type, so an empty result of the copy is
    // never ambiguous.
    ::scl::any const copy = ::scl::any::try_copy(text);

    // A move-only type is storable precisely because copying is not a constructor.
    ::scl::any const owned{::std::make_unique<int>(42)};
    ::scl::any const refused = ::scl::any::try_copy(owned);

    ::std::cout
        << "  is_copyable=" << text.is_copyable() << " copy=\""
        << *::scl::any_cast<::std::string>(&copy) << "\"\n";
    ::std::cout
        << "  unique_ptr is_copyable=" << owned.is_copyable()
        << " copy has_value=" << refused.has_value() << '\n'; // 0 0
}
//! [copying]

// ============================================================================
// Pattern 5 — moving hands over the object, never copies it
// ============================================================================

//! [moving]
static void show_moving()
{
    // An allocated object moves by handing over its pointer, an in-place one by
    // relocation - which only a nothrow-movable type ever undergoes.
    ::scl::any source{::std::string{"moved"}};
    ::scl::any const target{::std::move(source)};

    // NOLINTNEXTLINE(bugprone-use-after-move): emptiness after a move is the contract
    ::std::cout << "  after the move, source has_value=" << source.has_value() << '\n'; // 0
    ::std::cout << "  target holds \"" << *::scl::any_cast<::std::string>(&target) << "\"\n";
}
//! [moving]

// ============================================================================
// Pattern 6 — an allocator with state
// ============================================================================

//! [allocator]
static void show_allocator()
{
    ::std::pmr::monotonic_buffer_resource resource;
    using pmr_any = ::scl::basic_any<::std::pmr::polymorphic_allocator<::std::byte>>;

    pmr_any value{::std::allocator_arg, ::std::pmr::polymorphic_allocator<::std::byte>{&resource}};
    value.emplace<::std::string>(64U, 'x');

    ::std::cout
        << "  allocated from the given resource? " << (value.get_allocator().resource() == &resource) << '\n'; // 1

    // The allocator travels with the object: what one allocator acquired, only
    // that allocator may release.
    pmr_any const taken{::std::move(value)};
    ::std::cout << "  and travels with it? " << (taken.get_allocator().resource() == &resource) << '\n'; // 1
}
//! [allocator]

// ============================================================================
// Pattern 7 — the views read the content, not the box
// ============================================================================

static void report(::scl::any_view value)
{
    ::std::cout << "  view sees type_name=\"" << value.type_name() << "\"\n";
}

static void bump(::scl::any_arg value)
{
    if (auto * number = ::scl::any_cast<int>(&value))
        ++*number;
}

static void show_views()
{
    ::scl::any value{42};

    report(value); // names int, not the any
    bump(value);   // writes through to the stored object

    ::std::cout << "  after the write, the any holds " << *::scl::any_cast<int>(&value) << '\n';

    auto const describe =
        ::scl::any_switch<::std::string>()
            .in_case<int>([](int number) {
        return "int " + ::std::to_string(number);
    }).or_else("something else");

    ::std::cout << "  " << *describe.apply(value) << '\n';

    // The direction reverses as well: an any takes a copy of what a handle refers to,
    // never the handle itself, and the copy is asked for by name.
    ::scl::any const taken = ::scl::any::try_copy(::scl::any_view{value});
    ::std::cout << "  an any built from a view holds type_name=\"" << taken.type_name() << "\"\n"; // int
}

// ============================================================================
// main
// ============================================================================

int main(int, char **)
{
    ::std::cout << "=== Store, read, write ===\n";
    show_round_trip();

    ::std::cout << "\n=== Where the object lives ===\n";
    show_storage();

    ::std::cout << "\n=== Constant evaluation ===\n";
    show_constant_evaluation();

    ::std::cout << "\n=== Move-only, copies on request ===\n";
    show_copying();

    ::std::cout << "\n=== Moving ===\n";
    show_moving();

    ::std::cout << "\n=== An allocator with state ===\n";
    show_allocator();

    ::std::cout << "\n=== The views read the content ===\n";
    show_views();

    return {};
}
