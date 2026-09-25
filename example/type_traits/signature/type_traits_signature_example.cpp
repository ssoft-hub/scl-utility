/**
 * @example type_traits_signature_example.cpp
 * @brief Reads the properties of a callable type: its function type, result, parameters, class
 *        and qualifiers.
 *
 * scl::signature_t<F> is the function type of F, and the namespace scl::signature holds its
 * parts; a pointer to a member function is built back from them with scl::member_like_t.
 */

#include <scl/utility/type_traits/member_like.h>
#include <scl/utility/type_traits/overload_cast.h>
#include <scl/utility/type_traits/signature.h>

#include <cstdint>
#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

//! [types]
struct printer
{
    int print(long value, ::std::string const & label) const & noexcept;
    void clear();
    void clear(int line);
};

using print_t = decltype(&printer::print); // int (printer::*)(long, std::string const &) const & noexcept
//! [types]

//! [signature]
static_assert(::std::is_same_v<::scl::signature_t<print_t>, int(long, ::std::string const &) noexcept>);
static_assert(::std::is_same_v<::scl::signature::result_t<print_t>, int>);
static_assert(::std::is_same_v<::scl::signature::parameters_t<print_t>,
    ::std::tuple<long, ::std::string const &>>);
static_assert(::std::is_same_v<::scl::signature::parameter_t<print_t, 1>, ::std::string const &>);
//! [signature]

//! [qualifiers]
static_assert(::scl::signature::is_const_v<print_t>);
static_assert(!::scl::signature::is_volatile_v<print_t>);
static_assert(::scl::signature::is_lvalue_reference_v<print_t>);
static_assert(!::scl::signature::is_rvalue_reference_v<print_t>);
static_assert(::scl::signature::is_noexcept_v<print_t>);
//! [qualifiers]

//! [object]
static_assert(::std::is_same_v<::scl::signature::object_t<print_t>, printer const &>);
static_assert(::std::is_same_v<
    ::scl::member_like_t<::scl::signature::object_t<print_t>, ::scl::signature_t<print_t>>,
    print_t>);
//! [object]

//! [overload]
using clear_t = decltype(::scl::overload_cast<int>(&printer::clear));

static_assert(::std::is_same_v<::scl::signature_t<clear_t>, void(int)>);
//! [overload]

//! [absent]
template <typename Callable>
concept has_signature = requires { typename ::scl::signature_t<Callable>; };

auto const generic = [](auto value) { return value; };
static_assert(!has_signature<decltype(generic)>);
static_assert(!has_signature<int>);
//! [absent]

//! [function]
int parse(char const * text, int base) noexcept;

template <typename Callable>
concept has_object = requires { typename ::scl::signature::object_t<Callable>; };

static_assert(::std::is_same_v<::scl::signature_t<decltype(parse)>, int(char const *, int) noexcept>);
static_assert(::std::is_same_v<::scl::signature::result_t<decltype(&parse)>, int>);
static_assert(::scl::signature::parameter_count_v<decltype(parse)> == 2);
static_assert(::scl::signature::is_noexcept_v<decltype(&parse)>);
static_assert(!::scl::signature::is_const_v<decltype(&parse)>);
static_assert(!has_object<decltype(&parse)>);
//! [function]

//! [lambda]
auto const add = [](int left, int right) { return left + right; };
auto counter = [calls = 0]() mutable { return ++calls; };

static_assert(::std::is_same_v<::scl::signature_t<decltype(add)>, int(int, int)>);
static_assert(::std::is_same_v<::scl::signature::object_t<decltype(add)>, decltype(add) const>);
static_assert(::scl::signature::is_const_v<decltype(add)>);
static_assert(!::scl::signature::is_const_v<decltype(counter)>);
//! [lambda]

//! [variadic]
using log_t = void (*)(void * context, char const * format, ...);

static_assert(::std::is_same_v<::scl::signature_t<log_t>, void(void *, char const *, ...)>);
static_assert(::std::is_same_v<::scl::signature::parameters_t<log_t>, ::std::tuple<void *, char const *>>);
static_assert(::scl::signature::parameter_count_v<log_t> == 2);
static_assert(::scl::signature::is_variadic_v<log_t>);
static_assert(!::scl::signature::is_variadic_v<void (*)(char const *)>);
//! [variadic]

//! [index]
template <typename Callable, ::std::size_t Index>
concept has_parameter = requires { typename ::scl::signature::parameter_t<Callable, Index>; };

static_assert(has_parameter<print_t, 1>);
static_assert(!has_parameter<print_t, 2>);
//! [index]

//! [rvalue]
struct builder
{
    builder && name(char const * text) && noexcept;
};

using name_t = decltype(&builder::name);

static_assert(::std::is_same_v<::scl::signature::object_t<name_t>, builder &&>);
static_assert(::scl::signature::is_rvalue_reference_v<name_t>);
static_assert(!::scl::signature::is_lvalue_reference_v<name_t>);
static_assert(!::scl::signature::is_const_v<name_t>);
//! [rvalue]

//! [inherited]
struct base_handler
{
    void operator()(int code) const;
};

struct handler : base_handler
{};

static_assert(::std::is_same_v<::scl::signature::object_t<handler>, handler const>);
static_assert(::std::is_same_v<::scl::signature::object_t<decltype(&handler::operator())>, base_handler const>);
//! [inherited]

//! [explicit]
#if defined(__cpp_explicit_this_parameter)
struct stepper
{
    int operator()(this stepper const & self, long step);
};

static_assert(::std::is_same_v<::scl::signature_t<stepper>, int(long)>);
static_assert(::std::is_same_v<::scl::signature::object_t<stepper>, stepper const &>);
#endif
//! [explicit]

//! [constrain]
template <typename Callback>
void on_port(Callback && callback)
    requires(::std::is_invocable_v<Callback, ::std::uint16_t>) &&
    (!has_signature<Callback> ||
        (::std::is_unsigned_v<::std::remove_cvref_t<::scl::signature::parameter_t<Callback, 0>>> &&
            sizeof(::scl::signature::parameter_t<Callback, 0>) >= sizeof(::std::uint16_t)))
{
    ::std::forward<Callback>(callback)(::std::uint16_t{8080});
}

template <typename Callback>
concept accepted_by_on_port = requires(Callback callback) { on_port(callback); };

auto const narrow = [](::std::uint8_t port) { return port; };
static_assert(!accepted_by_on_port<decltype(narrow)>);
//! [constrain]

int main(int, char **)
{
    ::std::cout << "=== Constrained callback ===\n";
    on_port([](::std::uint32_t port) { ::std::cout << "port " << port << '\n'; });
    on_port([](auto port) { ::std::cout << "port " << port << '\n'; });

    return {};
}
