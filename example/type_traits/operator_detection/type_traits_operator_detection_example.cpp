/**
 * @example type_traits_operator_detection_example.cpp
 * @brief Tells whether an operator applies to given operands, as an expression or a member.
 *
 * The trait has_plus_v checks the expression, has_plus_member_v the member function operator+,
 * and has_plus_member_exact_v that member function declared with exactly the given parameter
 * types.
 */

#include <scl/utility/type_traits/detection/operator.h>

#include <compare>
#include <cstddef>
#include <functional>
#include <iostream>
#include <type_traits>
#include <vector>

//! [types]
struct money
{
    long cents;

    money operator+(money other) const { return {cents + other.cents}; }
    bool operator==(money const & other) const = default;
};

money operator*(money amount, int times);
//! [types]

//! [expression]
static_assert(scl::has_plus_v<money, money>);
static_assert(std::is_same_v<scl::plus_t<money, money>, money>);
static_assert(scl::has_multiplies_v<money, int>);
static_assert(!scl::has_multiplies_v<money, money>);
static_assert(scl::has_less_v<int, double>);
static_assert(!scl::has_less_v<money, money>);
//! [expression]

//! [member]
static_assert(scl::has_plus_member_v<money, money>);
static_assert(!scl::has_multiplies_member_v<money, int>);
static_assert(scl::has_plus_member_exact_v<money const, money>);
static_assert(!scl::has_plus_member_exact_v<money, money>);
static_assert(!scl::has_plus_member_exact_v<money const, money const &>);
//! [member]

//! [special]
static_assert(scl::has_subscript_v<std::vector<int> &, std::size_t>);
static_assert(std::is_same_v<scl::subscript_t<std::vector<int> &, std::size_t>, int &>);
static_assert(scl::has_call_v<std::plus<>, int, int>);
static_assert(scl::has_prefix_increment_v<int &>);
static_assert(!scl::has_prefix_increment_v<int>);
//! [special]

//! [address_of]
struct address_of_operand
{
    int * operator&();
};

static_assert(scl::has_address_of_v<int &>);
static_assert(std::is_same_v<scl::address_of_member_t<address_of_operand>, int *>);
static_assert(scl::has_address_of_member_exact_v<address_of_operand>);
static_assert(!scl::has_address_of_member_exact_v<address_of_operand &>);
//! [address_of]

//! [indirection]
struct indirection_operand
{
    int operator*() const;
};

static_assert(scl::has_indirection_v<int *>);
static_assert(std::is_same_v<scl::indirection_member_t<indirection_operand const>, int>);
static_assert(scl::has_indirection_member_exact_v<indirection_operand const>);
static_assert(!scl::has_indirection_member_exact_v<indirection_operand const &>);
//! [indirection]

//! [unary_plus]
struct unary_plus_operand
{
    int operator+() const;
};

static_assert(scl::has_unary_plus_v<int>);
static_assert(std::is_same_v<scl::unary_plus_member_t<unary_plus_operand const>, int>);
static_assert(scl::has_unary_plus_member_exact_v<unary_plus_operand const>);
static_assert(!scl::has_unary_plus_member_exact_v<unary_plus_operand const &>);
//! [unary_plus]

//! [negate]
struct negate_operand
{
    int operator-() const;
};

static_assert(scl::has_negate_v<int>);
static_assert(std::is_same_v<scl::negate_member_t<negate_operand const>, int>);
static_assert(scl::has_negate_member_exact_v<negate_operand const>);
static_assert(!scl::has_negate_member_exact_v<negate_operand const &>);
//! [negate]

//! [prefix_increment]
struct prefix_increment_operand
{
    prefix_increment_operand & operator++();
};

static_assert(scl::has_prefix_increment_v<int &>);
static_assert(
    std::is_same_v<scl::prefix_increment_member_t<prefix_increment_operand>, prefix_increment_operand &>);
static_assert(scl::has_prefix_increment_member_exact_v<prefix_increment_operand>);
static_assert(!scl::has_prefix_increment_member_exact_v<prefix_increment_operand &>);
//! [prefix_increment]

//! [prefix_decrement]
struct prefix_decrement_operand
{
    prefix_decrement_operand & operator--();
};

static_assert(scl::has_prefix_decrement_v<int &>);
static_assert(
    std::is_same_v<scl::prefix_decrement_member_t<prefix_decrement_operand>, prefix_decrement_operand &>);
static_assert(scl::has_prefix_decrement_member_exact_v<prefix_decrement_operand>);
static_assert(!scl::has_prefix_decrement_member_exact_v<prefix_decrement_operand &>);
//! [prefix_decrement]

//! [bit_not]
struct bit_not_operand
{
    int operator~() const;
};

static_assert(scl::has_bit_not_v<int>);
static_assert(std::is_same_v<scl::bit_not_member_t<bit_not_operand const>, int>);
static_assert(scl::has_bit_not_member_exact_v<bit_not_operand const>);
static_assert(!scl::has_bit_not_member_exact_v<bit_not_operand const &>);
//! [bit_not]

//! [logical_not]
struct logical_not_operand
{
    bool operator!() const;
};

static_assert(scl::has_logical_not_v<bool>);
static_assert(std::is_same_v<scl::logical_not_member_t<logical_not_operand const>, bool>);
static_assert(scl::has_logical_not_member_exact_v<logical_not_operand const>);
static_assert(!scl::has_logical_not_member_exact_v<logical_not_operand const &>);
//! [logical_not]

//! [postfix_increment]
struct postfix_increment_operand
{
    postfix_increment_operand operator++(int);
};

static_assert(scl::has_postfix_increment_v<int &>);
static_assert(
    std::is_same_v<scl::postfix_increment_member_t<postfix_increment_operand>, postfix_increment_operand>);
static_assert(scl::has_postfix_increment_member_exact_v<postfix_increment_operand>);
static_assert(!scl::has_postfix_increment_member_exact_v<postfix_increment_operand &>);
//! [postfix_increment]

//! [postfix_decrement]
struct postfix_decrement_operand
{
    postfix_decrement_operand operator--(int);
};

static_assert(scl::has_postfix_decrement_v<int &>);
static_assert(
    std::is_same_v<scl::postfix_decrement_member_t<postfix_decrement_operand>, postfix_decrement_operand>);
static_assert(scl::has_postfix_decrement_member_exact_v<postfix_decrement_operand>);
static_assert(!scl::has_postfix_decrement_member_exact_v<postfix_decrement_operand &>);
//! [postfix_decrement]

//! [equal_to]
struct equal_to_operand
{
    bool operator==(int value) const;
};

static_assert(scl::has_equal_to_v<int, long>);
static_assert(std::is_same_v<scl::equal_to_member_t<equal_to_operand const, short>, bool>);
static_assert(scl::has_equal_to_member_exact_v<equal_to_operand const, int>);
static_assert(!scl::has_equal_to_member_exact_v<equal_to_operand const, short>);
//! [equal_to]

//! [not_equal_to]
struct not_equal_to_operand
{
    bool operator!=(int value) const;
};

static_assert(scl::has_not_equal_to_v<int, long>);
static_assert(std::is_same_v<scl::not_equal_to_member_t<not_equal_to_operand const, short>, bool>);
static_assert(scl::has_not_equal_to_member_exact_v<not_equal_to_operand const, int>);
static_assert(!scl::has_not_equal_to_member_exact_v<not_equal_to_operand const, short>);
//! [not_equal_to]

//! [less]
struct less_operand
{
    bool operator<(int value) const;
};

static_assert(scl::has_less_v<int, long>);
static_assert(std::is_same_v<scl::less_member_t<less_operand const, short>, bool>);
static_assert(scl::has_less_member_exact_v<less_operand const, int>);
static_assert(!scl::has_less_member_exact_v<less_operand const, short>);
//! [less]

//! [less_equal]
struct less_equal_operand
{
    bool operator<=(int value) const;
};

static_assert(scl::has_less_equal_v<int, long>);
static_assert(std::is_same_v<scl::less_equal_member_t<less_equal_operand const, short>, bool>);
static_assert(scl::has_less_equal_member_exact_v<less_equal_operand const, int>);
static_assert(!scl::has_less_equal_member_exact_v<less_equal_operand const, short>);
//! [less_equal]

//! [greater]
struct greater_operand
{
    bool operator>(int value) const;
};

static_assert(scl::has_greater_v<int, long>);
static_assert(std::is_same_v<scl::greater_member_t<greater_operand const, short>, bool>);
static_assert(scl::has_greater_member_exact_v<greater_operand const, int>);
static_assert(!scl::has_greater_member_exact_v<greater_operand const, short>);
//! [greater]

//! [greater_equal]
struct greater_equal_operand
{
    bool operator>=(int value) const;
};

static_assert(scl::has_greater_equal_v<int, long>);
static_assert(std::is_same_v<scl::greater_equal_member_t<greater_equal_operand const, short>, bool>);
static_assert(scl::has_greater_equal_member_exact_v<greater_equal_operand const, int>);
static_assert(!scl::has_greater_equal_member_exact_v<greater_equal_operand const, short>);
//! [greater_equal]

//! [multiplies]
struct multiplies_operand
{
    long operator*(int value) const;
};

static_assert(scl::has_multiplies_v<int, long>);
static_assert(std::is_same_v<scl::multiplies_member_t<multiplies_operand const, short>, long>);
static_assert(scl::has_multiplies_member_exact_v<multiplies_operand const, int>);
static_assert(!scl::has_multiplies_member_exact_v<multiplies_operand const, short>);
//! [multiplies]

//! [divides]
struct divides_operand
{
    long operator/(int value) const;
};

static_assert(scl::has_divides_v<int, long>);
static_assert(std::is_same_v<scl::divides_member_t<divides_operand const, short>, long>);
static_assert(scl::has_divides_member_exact_v<divides_operand const, int>);
static_assert(!scl::has_divides_member_exact_v<divides_operand const, short>);
//! [divides]

//! [modulus]
struct modulus_operand
{
    long operator%(int value) const;
};

static_assert(scl::has_modulus_v<int, long>);
static_assert(std::is_same_v<scl::modulus_member_t<modulus_operand const, short>, long>);
static_assert(scl::has_modulus_member_exact_v<modulus_operand const, int>);
static_assert(!scl::has_modulus_member_exact_v<modulus_operand const, short>);
//! [modulus]

//! [plus]
struct plus_operand
{
    long operator+(int value) const;
};

static_assert(scl::has_plus_v<int, long>);
static_assert(std::is_same_v<scl::plus_member_t<plus_operand const, short>, long>);
static_assert(scl::has_plus_member_exact_v<plus_operand const, int>);
static_assert(!scl::has_plus_member_exact_v<plus_operand const, short>);
//! [plus]

//! [minus]
struct minus_operand
{
    long operator-(int value) const;
};

static_assert(scl::has_minus_v<int, long>);
static_assert(std::is_same_v<scl::minus_member_t<minus_operand const, short>, long>);
static_assert(scl::has_minus_member_exact_v<minus_operand const, int>);
static_assert(!scl::has_minus_member_exact_v<minus_operand const, short>);
//! [minus]

//! [left_shift]
struct left_shift_operand
{
    long operator<<(int value) const;
};

static_assert(scl::has_left_shift_v<int, long>);
static_assert(std::is_same_v<scl::left_shift_member_t<left_shift_operand const, short>, long>);
static_assert(scl::has_left_shift_member_exact_v<left_shift_operand const, int>);
static_assert(!scl::has_left_shift_member_exact_v<left_shift_operand const, short>);
//! [left_shift]

//! [right_shift]
struct right_shift_operand
{
    long operator>>(int value) const;
};

static_assert(scl::has_right_shift_v<int, long>);
static_assert(std::is_same_v<scl::right_shift_member_t<right_shift_operand const, short>, long>);
static_assert(scl::has_right_shift_member_exact_v<right_shift_operand const, int>);
static_assert(!scl::has_right_shift_member_exact_v<right_shift_operand const, short>);
//! [right_shift]

//! [bit_and]
struct bit_and_operand
{
    long operator&(int value) const;
};

static_assert(scl::has_bit_and_v<int, long>);
static_assert(std::is_same_v<scl::bit_and_member_t<bit_and_operand const, short>, long>);
static_assert(scl::has_bit_and_member_exact_v<bit_and_operand const, int>);
static_assert(!scl::has_bit_and_member_exact_v<bit_and_operand const, short>);
//! [bit_and]

//! [bit_or]
struct bit_or_operand
{
    long operator|(int value) const;
};

static_assert(scl::has_bit_or_v<int, long>);
static_assert(std::is_same_v<scl::bit_or_member_t<bit_or_operand const, short>, long>);
static_assert(scl::has_bit_or_member_exact_v<bit_or_operand const, int>);
static_assert(!scl::has_bit_or_member_exact_v<bit_or_operand const, short>);
//! [bit_or]

//! [bit_xor]
struct bit_xor_operand
{
    long operator^(int value) const;
};

static_assert(scl::has_bit_xor_v<int, long>);
static_assert(std::is_same_v<scl::bit_xor_member_t<bit_xor_operand const, short>, long>);
static_assert(scl::has_bit_xor_member_exact_v<bit_xor_operand const, int>);
static_assert(!scl::has_bit_xor_member_exact_v<bit_xor_operand const, short>);
//! [bit_xor]

//! [logical_and]
struct logical_and_operand
{
    bool operator&&(int value) const;
};

static_assert(scl::has_logical_and_v<bool, bool>);
static_assert(std::is_same_v<scl::logical_and_member_t<logical_and_operand const, short>, bool>);
static_assert(scl::has_logical_and_member_exact_v<logical_and_operand const, int>);
static_assert(!scl::has_logical_and_member_exact_v<logical_and_operand const, short>);
//! [logical_and]

//! [logical_or]
struct logical_or_operand
{
    bool operator||(int value) const;
};

static_assert(scl::has_logical_or_v<bool, bool>);
static_assert(std::is_same_v<scl::logical_or_member_t<logical_or_operand const, short>, bool>);
static_assert(scl::has_logical_or_member_exact_v<logical_or_operand const, int>);
static_assert(!scl::has_logical_or_member_exact_v<logical_or_operand const, short>);
//! [logical_or]

//! [assign]
struct assign_operand
{
    assign_operand & operator=(int value);
};

static_assert(scl::has_assign_v<int &, long>);
static_assert(std::is_same_v<scl::assign_member_t<assign_operand, short>, assign_operand &>);
static_assert(scl::has_assign_member_exact_v<assign_operand, int>);
static_assert(!scl::has_assign_member_exact_v<assign_operand, short>);
//! [assign]

//! [multiplies_assign]
struct multiplies_assign_operand
{
    multiplies_assign_operand & operator*=(int value);
};

static_assert(scl::has_multiplies_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::multiplies_assign_member_t<multiplies_assign_operand, short>, multiplies_assign_operand &>);
static_assert(scl::has_multiplies_assign_member_exact_v<multiplies_assign_operand, int>);
static_assert(!scl::has_multiplies_assign_member_exact_v<multiplies_assign_operand, short>);
//! [multiplies_assign]

//! [divides_assign]
struct divides_assign_operand
{
    divides_assign_operand & operator/=(int value);
};

static_assert(scl::has_divides_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::divides_assign_member_t<divides_assign_operand, short>, divides_assign_operand &>);
static_assert(scl::has_divides_assign_member_exact_v<divides_assign_operand, int>);
static_assert(!scl::has_divides_assign_member_exact_v<divides_assign_operand, short>);
//! [divides_assign]

//! [modulus_assign]
struct modulus_assign_operand
{
    modulus_assign_operand & operator%=(int value);
};

static_assert(scl::has_modulus_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::modulus_assign_member_t<modulus_assign_operand, short>, modulus_assign_operand &>);
static_assert(scl::has_modulus_assign_member_exact_v<modulus_assign_operand, int>);
static_assert(!scl::has_modulus_assign_member_exact_v<modulus_assign_operand, short>);
//! [modulus_assign]

//! [plus_assign]
struct plus_assign_operand
{
    plus_assign_operand & operator+=(int value);
};

static_assert(scl::has_plus_assign_v<int &, long>);
static_assert(std::is_same_v<scl::plus_assign_member_t<plus_assign_operand, short>, plus_assign_operand &>);
static_assert(scl::has_plus_assign_member_exact_v<plus_assign_operand, int>);
static_assert(!scl::has_plus_assign_member_exact_v<plus_assign_operand, short>);
//! [plus_assign]

//! [minus_assign]
struct minus_assign_operand
{
    minus_assign_operand & operator-=(int value);
};

static_assert(scl::has_minus_assign_v<int &, long>);
static_assert(
    std::is_same_v<scl::minus_assign_member_t<minus_assign_operand, short>, minus_assign_operand &>);
static_assert(scl::has_minus_assign_member_exact_v<minus_assign_operand, int>);
static_assert(!scl::has_minus_assign_member_exact_v<minus_assign_operand, short>);
//! [minus_assign]

//! [left_shift_assign]
struct left_shift_assign_operand
{
    left_shift_assign_operand & operator<<=(int value);
};

static_assert(scl::has_left_shift_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::left_shift_assign_member_t<left_shift_assign_operand, short>, left_shift_assign_operand &>);
static_assert(scl::has_left_shift_assign_member_exact_v<left_shift_assign_operand, int>);
static_assert(!scl::has_left_shift_assign_member_exact_v<left_shift_assign_operand, short>);
//! [left_shift_assign]

//! [right_shift_assign]
struct right_shift_assign_operand
{
    right_shift_assign_operand & operator>>=(int value);
};

static_assert(scl::has_right_shift_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::right_shift_assign_member_t<right_shift_assign_operand, short>, right_shift_assign_operand &>);
static_assert(scl::has_right_shift_assign_member_exact_v<right_shift_assign_operand, int>);
static_assert(!scl::has_right_shift_assign_member_exact_v<right_shift_assign_operand, short>);
//! [right_shift_assign]

//! [bit_and_assign]
struct bit_and_assign_operand
{
    bit_and_assign_operand & operator&=(int value);
};

static_assert(scl::has_bit_and_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::bit_and_assign_member_t<bit_and_assign_operand, short>, bit_and_assign_operand &>);
static_assert(scl::has_bit_and_assign_member_exact_v<bit_and_assign_operand, int>);
static_assert(!scl::has_bit_and_assign_member_exact_v<bit_and_assign_operand, short>);
//! [bit_and_assign]

//! [bit_or_assign]
struct bit_or_assign_operand
{
    bit_or_assign_operand & operator|=(int value);
};

static_assert(scl::has_bit_or_assign_v<int &, long>);
static_assert(
    std::is_same_v<scl::bit_or_assign_member_t<bit_or_assign_operand, short>, bit_or_assign_operand &>);
static_assert(scl::has_bit_or_assign_member_exact_v<bit_or_assign_operand, int>);
static_assert(!scl::has_bit_or_assign_member_exact_v<bit_or_assign_operand, short>);
//! [bit_or_assign]

//! [bit_xor_assign]
struct bit_xor_assign_operand
{
    bit_xor_assign_operand & operator^=(int value);
};

static_assert(scl::has_bit_xor_assign_v<int &, long>);
static_assert(std::is_same_v<
    scl::bit_xor_assign_member_t<bit_xor_assign_operand, short>, bit_xor_assign_operand &>);
static_assert(scl::has_bit_xor_assign_member_exact_v<bit_xor_assign_operand, int>);
static_assert(!scl::has_bit_xor_assign_member_exact_v<bit_xor_assign_operand, short>);
//! [bit_xor_assign]

//! [arrow_to_pointer]
struct arrow_to_pointer_operand
{
    long operator->*(int value) const;
};

static_assert(scl::has_arrow_to_pointer_v<arrow_to_pointer_operand const, int>);
static_assert(!scl::has_arrow_to_pointer_v<arrow_to_pointer_operand *, long arrow_to_pointer_operand::*>);
static_assert(std::is_same_v<scl::arrow_to_pointer_member_t<arrow_to_pointer_operand const, short>, long>);
static_assert(scl::has_arrow_to_pointer_member_exact_v<arrow_to_pointer_operand const, int>);
static_assert(!scl::has_arrow_to_pointer_member_exact_v<arrow_to_pointer_operand const, short>);
//! [arrow_to_pointer]

//! [arrow]
struct arrow_operand
{
    long const * operator->() const;
};

static_assert(scl::has_arrow_v<arrow_operand const>);
static_assert(!scl::has_arrow_v<arrow_operand *>);
static_assert(std::is_same_v<scl::arrow_member_t<arrow_operand const>, long const *>);
static_assert(scl::has_arrow_member_exact_v<arrow_operand const>);
static_assert(!scl::has_arrow_member_exact_v<arrow_operand const &>);
//! [arrow]

//! [comma]
struct comma_operand
{
    long operator,(int value) const;
};

static_assert(scl::has_comma_v<int, long>);
static_assert(std::is_same_v<scl::comma_member_t<comma_operand const, short>, long>);
static_assert(scl::has_comma_member_exact_v<comma_operand const, int>);
static_assert(!scl::has_comma_member_exact_v<comma_operand const, short>);
//! [comma]

//! [subscript]
struct subscript_operand
{
    long operator[](int index) const;
};

static_assert(scl::has_subscript_v<int *, int>);
static_assert(std::is_same_v<scl::subscript_member_t<subscript_operand const, short>, long>);
static_assert(scl::has_subscript_member_exact_v<subscript_operand const, int>);
static_assert(!scl::has_subscript_member_exact_v<subscript_operand const, short>);
//! [subscript]

//! [call]
struct call_operand
{
    long operator()(int value) const;
};

static_assert(scl::has_call_v<long (*)(int), int>);
static_assert(std::is_same_v<scl::call_member_t<call_operand const, short>, long>);
static_assert(scl::has_call_member_exact_v<call_operand const, int>);
static_assert(!scl::has_call_member_exact_v<call_operand const, short>);
//! [call]

money operator*(money amount, int times) { return {amount.cents * times}; }

int main()
{
    money const price{250};
    std::cout << (price + price * 2).cents << '\n'; // 750
}
