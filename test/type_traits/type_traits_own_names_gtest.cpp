#include <gtest_utils.h>

// Each namespace declared here before the group is included takes the name of a namespace the
// headers use, so a name the headers write below it without the leading :: finds this namespace
// and breaks the build of this unit: std:: and scl:: anywhere under scl, detail:: and concepts::
// inside scl::detail, scl::concepts and scl::signature, and signature:: inside scl::concepts and
// scl::signature.
namespace scl
{
    namespace std
    {}

    namespace scl
    {}
} // namespace scl

namespace scl::detail
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}
} // namespace scl::detail

namespace scl::concepts
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}

    namespace signature
    {}
} // namespace scl::concepts

namespace scl::signature
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    namespace concepts
    {}

    namespace signature
    {}
} // namespace scl::signature

// A namespace named scl, nominated at global scope, makes a name written as scl:: outside the
// namespace scl ambiguous.
namespace own_names
{
    namespace scl
    {}
} // namespace own_names

using namespace own_names;

#include <scl/utility/type_traits.h>

#include <tuple>
#include <type_traits>
#include <utility>

namespace own_caller
{
    namespace std
    {}

    namespace scl
    {}

    namespace detail
    {}

    struct gauge
    {
        long read(int value) const &;
        long read(int value) &&;
        long operator+(int value) const;
    };

    long measure(gauge value, int scale);

    SCL_FUNCTION_DETECTION(measure)
    SCL_METHOD_DETECTION(read)

    template <typename Type>
    using read_operation = decltype(::std::declval<Type const &>().read(0));
} // namespace own_caller

TEST(TypeTraitsOwnNames, DetectionAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::scl::is_detected_v<own_caller::read_operation, own_caller::gauge>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::detected_t<own_caller::read_operation, int>, ::scl::nonesuch>));
    STATIC_EXPECT_TRUE(
        (::std::is_same_v<::scl::detected_or_t<char, own_caller::read_operation, int>, char>));
    STATIC_EXPECT_TRUE((::scl::is_detected_exact_v<long, own_caller::read_operation, own_caller::gauge>));
    STATIC_EXPECT_TRUE(
        (::scl::is_detected_convertible_v<double, own_caller::read_operation, own_caller::gauge>));
}

TEST(TypeTraitsOwnNames, DetectionMacrosExpandBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((own_caller::has_measure_function_exact_v<own_caller::gauge, int>));
    STATIC_EXPECT_TRUE((own_caller::has_read_method_exact_v<own_caller::gauge const &, int>));
    STATIC_EXPECT_TRUE((::std::is_same_v<own_caller::read_method_t<own_caller::gauge &&, int>, long>));
}

TEST(TypeTraitsOwnNames, OperatorDetectionAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::plus_member_exact_t<own_caller::gauge const, int>, long>));
    STATIC_EXPECT_TRUE((::scl::has_plus_v<own_caller::gauge, int>));
}

TEST(TypeTraitsOwnNames, ForwardLikeAnswersBesideTheNamesOfTheCaller)
{
    int value = 1;
    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(::scl::forward_like<int const &>(value)), int const &>));
}

TEST(TypeTraitsOwnNames, MemberLikeAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_like_t<own_caller::gauge const &, long(int)>,
        long (own_caller::gauge::*)(int) const &>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        ::scl::member_like_t<own_caller::gauge const, int>, int const own_caller::gauge::*>));
}

TEST(TypeTraitsOwnNames, OverloadCastAnswersBesideTheNamesOfTheCaller)
{
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::member_overload_cast<own_caller::gauge &&, int>(&own_caller::gauge::read)),
        long (own_caller::gauge::*)(int) &&>));
    STATIC_EXPECT_TRUE((::std::is_same_v<
        decltype(::scl::overload_cast<int>(&own_caller::gauge::operator+)),
        long (own_caller::gauge::*)(int) const>));
}

TEST(TypeTraitsOwnNames, SignatureAnswersBesideTheNamesOfTheCaller)
{
    using read_t = long (own_caller::gauge::*)(int) const &;
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature_t<read_t>, long(int)>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::parameters_t<read_t>, ::std::tuple<int>>));
    STATIC_EXPECT_TRUE((::std::is_same_v<::scl::signature::object_t<read_t>, own_caller::gauge const &>));
    STATIC_EXPECT_TRUE(::scl::signature::is_lvalue_reference_v<read_t>);
}
