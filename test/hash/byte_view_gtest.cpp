#include <gtest_utils.h>

#include <scl/utility/hash/byte_view.h>
#include <scl/utility/hash/concepts.h>
#include <scl/utility/hash/djb2.h>
#include <scl/utility/hash/fnv1a.h>
#include <scl/utility/hash/jenkins_ota.h>
#include <scl/utility/hash/key.h>
#include <scl/utility/hash/sdbm.h>
#include <scl/utility/hash/siphash.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <deque>
#include <forward_list>
#include <list>
#include <memory>
#include <ranges>
#include <span>
#include <sstream>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace
{
    using ::scl::hash::concepts::hashable_range;

    /// Satisfied when @ref scl::hash::byte_view accepts @p Range.
    template <typename Range>
    concept spellable = requires(Range & range) { ::scl::hash::byte_view(range); };

    enum class byte_enum : unsigned char
    {
        value = 7
    };

    /// Satisfied when @p Range reaches a hash function or @ref scl::hash::byte_view.
    template <typename Range>
    concept taken = hashable_range<Range> || spellable<Range>;

    /// Satisfied when @p Hasher accepts @p Range.
    template <typename Hasher, typename Range>
    concept hasher_takes = requires(Range & range) { Hasher{}(range); };

    /// A hash function written outside the group, stating its constraint with the concept.
    template <::scl::hash::concepts::hashable_range Range>
    constexpr ::std::uint64_t rolling(Range && range)
    {
        ::std::uint64_t h = 0;
        for (::std::ranges::range_value_t<Range> const c : ::std::forward<Range>(range))
            h = (h * 31ull) + static_cast<::std::uint8_t>(c);
        return h;
    }

    /// The same function under no constraint beyond being a range.
    template <::std::ranges::range Range>
    constexpr ::std::uint64_t unconstrained(Range && range)
    {
        ::std::uint64_t h = 0;
        for (::std::ranges::range_value_t<Range> const c : ::std::forward<Range>(range))
            h = (h * 31ull) + static_cast<::std::uint8_t>(c);
        return h;
    }

    /// Satisfied when @ref rolling accepts @p Range.
    template <typename Range>
    concept rolling_takes = requires(Range & range) { rolling(range); };

    /// Satisfied when @ref unconstrained accepts @p Range.
    template <typename Range>
    concept unconstrained_takes = requires(Range & range) { unconstrained(range); };

    /// The one operation of a traversal that the fixtures below let throw.
    enum class throwing_operation : ::std::uint8_t
    {
        none,
        begin,
        end,
        compare,
        increment,
        dereference,
        conversion,
        size
    };

    /// A reference read without throwing, whose conversion to the element type throws.
    struct throwing_reference
    {
        char c;

        operator char() const { return c; }
    };

    template <throwing_operation Operation>
    struct throwing_iterator
    {
        using value_type = char;
        using difference_type = ::std::ptrdiff_t;
        using reference_type =
            ::std::conditional_t<Operation == throwing_operation::conversion, throwing_reference, char>;

        char const * position = nullptr;

        reference_type operator*() const /**/
            noexcept(Operation != throwing_operation::dereference)
        {
            return {*position};
        }

        throwing_iterator & operator++() /**/
            noexcept(Operation != throwing_operation::increment)
        {
            ++position;
            return *this;
        }

        void operator++(int) /**/
            noexcept(Operation != throwing_operation::increment)
        {
            ++position;
        }

        bool operator==(throwing_iterator const & other) const /**/
            noexcept(Operation != throwing_operation::compare)
        {
            return position == other.position;
        }
    };

    template <throwing_operation Operation>
    struct throwing_range : public ::std::ranges::view_interface<throwing_range<Operation>>
    {
        char const * first = "abc";

        throwing_iterator<Operation> begin() const /**/
            noexcept(Operation != throwing_operation::begin)
        {
            return {first};
        }

        throwing_iterator<Operation> end() const /**/
            noexcept(Operation != throwing_operation::end)
        {
            return {first + 3};
        }
    };

    /// A forward counterpart of @ref throwing_iterator: its postfix increment answers a copy.
    template <throwing_operation Operation>
    struct forward_throwing_iterator
    {
        using value_type = char;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::forward_iterator_tag;

        char const * position = nullptr;

        char operator*() const /**/
            noexcept(Operation != throwing_operation::dereference)
        {
            return *position;
        }

        forward_throwing_iterator & operator++() /**/
            noexcept(Operation != throwing_operation::increment)
        {
            ++position;
            return *this;
        }

        forward_throwing_iterator operator++(int) /**/
            noexcept(Operation != throwing_operation::increment)
        {
            auto taken = *this;
            ++position;
            return taken;
        }

        bool operator==(forward_throwing_iterator const & other) const /**/
            noexcept(Operation != throwing_operation::compare)
        {
            return position == other.position;
        }
    };

    template <throwing_operation Operation>
    struct forward_throwing_range : public ::std::ranges::view_interface<forward_throwing_range<Operation>>
    {
        char const * first = "abc";

        forward_throwing_iterator<Operation> begin() const /**/
            noexcept(Operation != throwing_operation::begin)
        {
            return {first};
        }

        forward_throwing_iterator<Operation> end() const /**/
            noexcept(Operation != throwing_operation::end)
        {
            return {first + 3};
        }
    };

    /// A range whose reported size is the only operation that can throw.
    template <throwing_operation Operation>
    struct sized_throwing_range : public ::std::ranges::view_interface<sized_throwing_range<Operation>>
    {
        char const * first = "abc";

        char const * begin() const noexcept { return first; }

        char const * end() const noexcept { return first + 3; }

        ::std::size_t size() const /**/
            noexcept(Operation != throwing_operation::size)
        {
            return 3;
        }
    };

    /// The same, reporting a size only through a non-const reference.
    template <throwing_operation Operation>
    struct mutable_sized_throwing_range
        : public ::std::ranges::view_interface<mutable_sized_throwing_range<Operation>>
    {
        char const * first = "abc";

        char const * begin() noexcept { return first; }

        char const * end() noexcept { return first + 3; }

        ::std::size_t size() /**/
            noexcept(Operation != throwing_operation::size)
        {
            return 3;
        }
    };

    /**
     * A container whose copies share one buffer until a write. Reading it through a
     * non-constant reference is what would separate that buffer.
     */
    struct shared_buffer
    {
        ::std::shared_ptr<::std::array<char, 3>> held =
            ::std::make_shared<::std::array<char, 3>>(::std::array<char, 3>{'a', 'b', 'c'});

        inline static int mutable_reads = 0;

        char * begin()
        {
            ++mutable_reads;
            return held->data();
        }

        char * end() { return held->data() + held->size(); }

        char const * begin() const noexcept { return held->data(); }

        char const * end() const noexcept { return held->data() + held->size(); }
    };

    /// A range whose constant traversal answers a wider element than its mutable one.
    struct widening_range
    {
        static constexpr char narrow[]{'a', 'b', 'c'};
        static constexpr wchar_t wide[]{L'x', L'y', L'z'};

        char const * begin() noexcept { return narrow; }
        char const * end() noexcept { return narrow + 3; }
        wchar_t const * begin() const noexcept { return wide; }
        wchar_t const * end() const noexcept { return wide + 3; }
    };

    /// The traversal of @ref throwing_range, offered only through a non-constant reference.
    template <throwing_operation Operation>
    struct mutable_throwing_range : public ::std::ranges::view_interface<mutable_throwing_range<Operation>>
    {
        char const * first = "abc";

        throwing_iterator<Operation> begin() /**/
            noexcept(Operation != throwing_operation::begin)
        {
            return {first};
        }

        throwing_iterator<Operation> end() /**/
            noexcept(Operation != throwing_operation::end)
        {
            return {first + 3};
        }
    };

    /// An iterator whose move construction is the only operation that can throw.
    struct move_throwing_iterator
    {
        using value_type = char;
        using difference_type = ::std::ptrdiff_t;

        char const * position = nullptr;

        move_throwing_iterator() = default;

        explicit move_throwing_iterator(char const * const at) noexcept
            : position{at}
        {}

        move_throwing_iterator(move_throwing_iterator const &) = default;

        move_throwing_iterator(move_throwing_iterator && other) /**/
            noexcept(false)
            : position{other.position}
        {}

        ~move_throwing_iterator() = default;

        move_throwing_iterator & operator=(move_throwing_iterator const &) = default;

        move_throwing_iterator & operator=(move_throwing_iterator && other) /**/
            noexcept(false)
        {
            position = other.position;
            return *this;
        }

        char operator*() const noexcept { return *position; }

        move_throwing_iterator & operator++() noexcept
        {
            ++position;
            return *this;
        }

        void operator++(int) noexcept { ++position; }

        bool operator==(move_throwing_iterator const & other) const noexcept
        {
            return position == other.position;
        }
    };

    /// A sentinel of its own type, telling it without throwing.
    struct plain_sentinel
    {
        char const * last = nullptr;

        friend bool
        operator==(move_throwing_iterator const & position, plain_sentinel const & sentinel) noexcept
        {
            return position.position == sentinel.last;
        }
    };

    struct move_throwing_iterator_range : public ::std::ranges::view_interface<move_throwing_iterator_range>
    {
        char const * first = "abc";

        move_throwing_iterator begin() const noexcept { return move_throwing_iterator{first}; }

        plain_sentinel end() const noexcept { return plain_sentinel{first + 3}; }
    };

    /// A sentinel whose move construction is the only operation that can throw.
    struct move_throwing_sentinel
    {
        char const * last = nullptr;

        move_throwing_sentinel() = default;

        explicit move_throwing_sentinel(char const * const at) noexcept
            : last{at}
        {}

        move_throwing_sentinel(move_throwing_sentinel const &) = default;

        move_throwing_sentinel(move_throwing_sentinel && other) /**/
            noexcept(false)
            : last{other.last}
        {}

        ~move_throwing_sentinel() = default;

        move_throwing_sentinel & operator=(move_throwing_sentinel const &) = default;

        move_throwing_sentinel & operator=(move_throwing_sentinel && other) /**/
            noexcept(false)
        {
            last = other.last;
            return *this;
        }

        friend bool
        operator==(char const * const position, move_throwing_sentinel const & sentinel) noexcept
        {
            return position == sentinel.last;
        }
    };

    struct move_throwing_sentinel_range : public ::std::ranges::view_interface<move_throwing_sentinel_range>
    {
        char const * first = "abc";

        char const * begin() const noexcept { return first; }

        move_throwing_sentinel end() const noexcept { return move_throwing_sentinel{first + 3}; }
    };

    /// A sentinel whose comparison with the iterator is the only operation that can throw.
    struct compare_throwing_sentinel
    {
        char const * last = nullptr;

        friend bool
        operator==(char const * const position, compare_throwing_sentinel const & sentinel) /**/
            noexcept(false)
        {
            return position == sentinel.last;
        }
    };

    struct distinct_sentinel_range : public ::std::ranges::view_interface<distinct_sentinel_range>
    {
        char const * first = "abc";

        char const * begin() const noexcept { return first; }

        compare_throwing_sentinel end() const noexcept
        {
            return compare_throwing_sentinel{first + 3};
        }
    };

    /// A forward iterator whose copy construction is the only operation that can throw.
    struct copy_throwing_iterator
    {
        using value_type = char;
        using difference_type = ::std::ptrdiff_t;
        using iterator_concept = ::std::forward_iterator_tag;

        char const * position = nullptr;

        copy_throwing_iterator() = default;

        explicit copy_throwing_iterator(char const * const at) noexcept
            : position{at}
        {}

        copy_throwing_iterator(copy_throwing_iterator const & other) /**/
            noexcept(false)
            : position{other.position}
        {}

        copy_throwing_iterator(copy_throwing_iterator &&) = default;

        ~copy_throwing_iterator() = default;

        copy_throwing_iterator & operator=(copy_throwing_iterator const & other) /**/
            noexcept(false)
        {
            position = other.position;
            return *this;
        }

        copy_throwing_iterator & operator=(copy_throwing_iterator &&) = default;

        char operator*() const noexcept { return *position; }

        copy_throwing_iterator & operator++() noexcept
        {
            ++position;
            return *this;
        }

        copy_throwing_iterator operator++(int) /**/
            noexcept(false)
        {
            auto taken = *this;
            ++position;
            return taken;
        }

        bool operator==(copy_throwing_iterator const & other) const noexcept
        {
            return position == other.position;
        }
    };

    struct copy_throwing_range : public ::std::ranges::view_interface<copy_throwing_range>
    {
        char const * first = "abc";

        copy_throwing_iterator begin() const noexcept { return copy_throwing_iterator{first}; }

        copy_throwing_iterator end() const noexcept { return copy_throwing_iterator{first + 3}; }
    };

    /// A container whose buffer reports how many owners it has.
    struct counted_buffer
    {
        ::std::shared_ptr<::std::array<char16_t, 3>> held;

        char16_t const * begin() const noexcept { return held->data(); }

        char16_t const * end() const noexcept { return held->data() + held->size(); }
    };

    struct two_bytes
    {
        char low;
        char high;
    };

    /// A range whose move constructor is the only operation that can throw.
    struct move_throwing_range : public ::std::ranges::view_interface<move_throwing_range>
    {
        char const * first = "abc";

        move_throwing_range() = default;
        move_throwing_range(move_throwing_range const &) = default;

        move_throwing_range(move_throwing_range && other) /**/
            noexcept(false)
            : first{other.first}
        {}

        ~move_throwing_range() = default;

        move_throwing_range & operator=(move_throwing_range const &) = default;

        move_throwing_range & operator=(move_throwing_range && other) /**/
            noexcept(false)
        {
            first = other.first;
            return *this;
        }

        char const * begin() const noexcept { return first; }

        char const * end() const noexcept { return first + 3; }
    };

    struct one_byte
    {
        char c;
    };

    struct empty
    {};
} // namespace

using namespace ::scl::hash;

// ============================================================================
// Accepted element types — one byte wide, and every bit of it is data
// ============================================================================

/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
/**
 * @test A range of byte-sized elements is accepted, `std::byte` included.
 */
TEST(HashElementTest, ByteSizedElementsAccepted)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::string_view>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<char>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<signed char>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<unsigned char>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::u8string_view>);
    STATIC_EXPECT_TRUE(hashable_range<::std::span<::std::byte const>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<byte_enum>>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<one_byte>>);
}

/**
 * @test A `std::byte` range hashes as the byte values it holds.
 */
TEST(HashElementTest, ByteSpanMatchesItsCharacters)
{
    static constexpr ::std::byte bytes[]{::std::byte{'a'}, ::std::byte{'b'}, ::std::byte{'c'}};
    EXPECT_EQ(fnv1a(::std::span{bytes}), fnv1a(::std::string_view{"abc"}));
    EXPECT_EQ(siphash(::std::span{bytes}), siphash(::std::string_view{"abc"}));
}

/**
 * @test A range whose element is wider than a byte is rejected.
 */
TEST(HashElementTest, WiderThanByteElementsRejected)
{
    STATIC_EXPECT_FALSE(hashable_range<::std::wstring_view>);
    STATIC_EXPECT_FALSE(hashable_range<::std::u16string_view>);
    STATIC_EXPECT_FALSE(hashable_range<::std::u32string_view>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<int>>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<unsigned>>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<double>>);
}

/**
 * @test An element carrying no value of its own is rejected.
 */
TEST(HashElementTest, EmptyElementRejected)
{
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<empty>>);
}

/**
 * @test A wider range becomes hashable once spelled through byte_view.
 */
TEST(HashByteViewTest, MakesAWiderRangeHashable)
{
    STATIC_EXPECT_FALSE(hashable_range<::std::u16string_view>);
    STATIC_EXPECT_TRUE(hashable_range<decltype(byte_view(::std::u16string_view{}))>);
    STATIC_EXPECT_TRUE(hashable_range<decltype(byte_view(::std::declval<::std::vector<int> const &>()))>);
}

/**
 * @test Two cursors on one element are told apart by the byte they stand on.
 */
TEST(HashByteViewTest, TwoCursorsOnOneElementDifferByTheirByte)
{
    ::std::u16string_view const wide{u"A"};
    auto bytes = byte_view(wide);

    auto const first = ::std::ranges::begin(bytes);
    auto const second = ::std::ranges::next(first);

    EXPECT_TRUE(first != second);
    EXPECT_EQ(*first, 0x41u);
    EXPECT_EQ(*second, 0x00u);
    EXPECT_TRUE(::std::ranges::next(second) == ::std::ranges::end(bytes));
}

/**
 * @test Postfix increment answers the byte it stood on and steps past it.
 */
TEST(HashByteViewTest, PostfixIncrementAnswersTheByteItStoodOn)
{
    ::std::u16string_view const wide{u"A"};
    auto bytes = byte_view(wide);

    auto moving = ::std::ranges::begin(bytes);
    auto const taken = moving++;

    EXPECT_EQ(*taken, 0x41u);
    EXPECT_EQ(*moving, 0x00u);
}

/**
 * @test A range that cannot be read through a const reference is taken.
 */
TEST(HashByteViewTest, ARangeThatCannotBeReadAsConstIsTaken)
{
    ::std::u16string_view const wide{u"AxB"};
    auto kept = wide | ::std::views::filter([](char16_t const c) { return c != u'x'; });

    STATIC_EXPECT_FALSE(::std::ranges::range<decltype(kept) const>);
    EXPECT_EQ(fnv1a(byte_view(kept)), fnv1a(byte_view(::std::u16string_view{u"AB"})));
}

/**
 * @test A container handed over as a temporary is taken and owned.
 */
TEST(HashByteViewTest, ATemporaryContainerIsTakenAndOwned)
{
    EXPECT_EQ(fnv1a(byte_view(::std::vector<char16_t>{u'A', u'B'})),
        fnv1a(byte_view(::std::u16string_view{u"AB"})));
}

/**
 * @test A source that reports a size only while mutable still reports one through the view.
 */
TEST(HashByteViewTest, ASourceSizedOnlyWhileMutableStillReportsASize)
{
    ::std::u16string_view const wide{u"AB"};
    auto counted = wide | ::std::views::transform([seen = 0](char16_t const c) mutable {
        ++seen;
        return c;
    });

    STATIC_EXPECT_TRUE(::std::ranges::sized_range<decltype(counted)>);
    STATIC_EXPECT_FALSE(::std::ranges::range<decltype(counted) const>);

    auto bytes = byte_view(counted);
    EXPECT_EQ(bytes.size(), 4u);
}

/**
 * @test A source that can be read once is taken.
 */
TEST(HashByteViewTest, AnInputOnlySourceIsTaken)
{
    ::std::istringstream stream{"A B"};

    EXPECT_EQ(fnv1a(byte_view(::std::ranges::istream_view<char>(stream))), fnv1a(::std::string_view{"AB"}));
}

/**
 * @test Three properties follow the source, and the view is never more than forward.
 */
TEST(HashByteViewTest, TheViewCarriesThePropertiesOfItsSource)
{
    using over_contiguous = decltype(byte_view(::std::declval<::std::u16string_view &>()));
    using over_list = decltype(byte_view(::std::declval<::std::list<char16_t> &>()));
    using over_forward = decltype(byte_view(::std::declval<::std::forward_list<char16_t> &>()));

    STATIC_EXPECT_TRUE(::std::ranges::view<over_contiguous>);
    STATIC_EXPECT_TRUE(::std::ranges::range<over_contiguous const>);
    STATIC_EXPECT_TRUE(::std::ranges::sized_range<over_contiguous const>);
    STATIC_EXPECT_TRUE(::std::ranges::forward_range<over_contiguous>);
    STATIC_EXPECT_FALSE(::std::ranges::random_access_range<over_contiguous>);
    STATIC_EXPECT_FALSE(::std::ranges::common_range<over_contiguous>);
    STATIC_EXPECT_TRUE(::std::ranges::forward_range<over_list>);

    STATIC_EXPECT_TRUE(::std::ranges::sized_range<over_list>);
    STATIC_EXPECT_FALSE(::std::ranges::sized_range<over_forward>);

    STATIC_EXPECT_TRUE((::std::is_same_v<::std::ranges::range_value_t<over_contiguous>, ::std::uint8_t>));
}

/**
 * @test The byte count is the element count times the width of one element.
 */
TEST(HashByteViewTest, TheSizeIsTheElementCountTimesTheElementWidth)
{
    ::std::u16string_view const wide{u"AB"};
    ::std::string_view const narrow{"AB"};

    STATIC_EXPECT_EQ(byte_view(::std::u16string_view{u"AB"}).size(), 4u);
    STATIC_EXPECT_EQ(byte_view(::std::string_view{"AB"}).size(), 2u);
    EXPECT_EQ(::std::ranges::distance(byte_view(wide)), 4);
    EXPECT_EQ(::std::ranges::distance(byte_view(narrow)), 2);
}

/**
 * @test A named result reads through a const reference, so const by default holds.
 */
TEST(HashByteViewTest, ANamedResultReadsThroughAConstReference)
{
    ::std::u16string_view const wide{u"AB"};
    auto const bytes = byte_view(wide);

    STATIC_EXPECT_TRUE(::std::ranges::range<decltype(bytes)>);
    EXPECT_EQ(fnv1a(bytes), fnv1a(::std::string_view{"A\0B\0", 4}));
}

/**
 * @test A range is spelled whether or not it reports a size or gives random access, and
 *       every hash function of the group then takes it.
 */
TEST(HashByteViewTest, ARangeNeedsNeitherRandomAccessNorSize)
{
    using unsized = decltype(::std::views::take_while(::std::u16string_view{}, [](char16_t const c) {
        return c != u'x';
    }));

    STATIC_EXPECT_TRUE(spellable<::std::list<char16_t>>);         // sized, not random-access
    STATIC_EXPECT_TRUE(spellable<unsized>);                       // random-access, not sized
    STATIC_EXPECT_TRUE(spellable<::std::forward_list<char16_t>>); // neither
    STATIC_EXPECT_TRUE(spellable<::std::deque<char16_t>>);        // both, not contiguous

    ::std::forward_list<char16_t> wide{u'A', u'B'};
    ::std::u16string_view contiguous{u"AB"};

    EXPECT_EQ(fnv1a(byte_view(wide)), fnv1a(byte_view(contiguous)));
    EXPECT_EQ(djb2(byte_view(wide)), djb2(byte_view(contiguous)));
    EXPECT_EQ(sdbm(byte_view(wide)), sdbm(byte_view(contiguous)));
    EXPECT_EQ(jenkins_ota(byte_view(wide)), jenkins_ota(byte_view(contiguous)));
    EXPECT_EQ(siphash(byte_view(wide)), siphash(byte_view(contiguous)));
}

/**
 * @test A floating-point element is refused, and an integer of any width is spelled.
 */
TEST(HashByteViewTest, AFloatingPointElementIsRefused)
{
    STATIC_EXPECT_FALSE(spellable<::std::vector<double>>);
    STATIC_EXPECT_TRUE(spellable<::std::wstring_view>);
    STATIC_EXPECT_TRUE(spellable<::std::u16string_view>);
    STATIC_EXPECT_TRUE(spellable<::std::u32string_view>);
}

/**
 * @test A source of wchar_t spells as many bytes per element as the platform gives it.
 */
TEST(HashByteViewTest, AWideCharacterSourceSpellsThePlatformWidth)
{
    static constexpr ::std::uint8_t expected[sizeof(wchar_t)]{0x41U};

    STATIC_EXPECT_TRUE(::std::ranges::equal(::scl::hash::byte_view(::std::wstring_view{L"A"}),
        ::std::span{expected}));
}

/**
 * @test Inputs differing above the low byte no longer collide.
 */
TEST(HashByteViewTest, WideInputsDifferingAboveTheLowByteDiffer)
{
    STATIC_EXPECT_NE(fnv1a(byte_view(::std::u16string_view{u"Ā"})),
        fnv1a(byte_view(::std::u16string_view{u"Ȁ"})));
    STATIC_EXPECT_NE(siphash(byte_view(::std::u16string_view{u"AB"})),
        siphash(byte_view(::std::u16string_view{u"Łł"})));
}

/**
 * @test A wide spelling of a text is a different input from its narrow one.
 */
TEST(HashByteViewTest, WideDiffersFromNarrow)
{
    STATIC_EXPECT_NE(fnv1a(byte_view(::std::u16string_view{u"AB"})), fnv1a(::std::string_view{"AB"}));
}

/**
 * @test Each element contributes its bytes in little-endian order, whatever the platform.
 *
 * The order is the order of the value, not the order of the element's storage. Reading the
 * storage would make two machines hash one input differently.
 */
TEST(HashByteViewTest, ElementsSpellTheirBytesLittleEndian)
{
    static constexpr ::std::uint32_t values[]{0x04030201U, 0x08070605U};
    static constexpr ::std::uint8_t expected[]{1, 2, 3, 4, 5, 6, 7, 8};
    STATIC_EXPECT_TRUE(::std::ranges::equal(byte_view(::std::span{values}), ::std::span{expected}));
}

/**
 * @test A byte-sized element passes through unchanged.
 */
TEST(HashByteViewTest, ByteElementIsUnchanged)
{
    STATIC_EXPECT_EQ(fnv1a(byte_view(::std::string_view{"hello"})), fnv1a(::std::string_view{"hello"}));
}

/**
 * @test hashable_range holds both rules at once: the range is not a bounded array, and its
 *       element is a single byte. A type that is no range at all fails on the first, and
 *       the second is never reached for it.
 */
TEST(HashElementTest, HashableRangeIsAByteRangeThatIsNotABoundedArray)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::string_view>);
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<::std::byte>>);
    STATIC_EXPECT_TRUE((hashable_range<::std::array<char, 4>>));
    STATIC_EXPECT_TRUE(hashable_range<::std::forward_list<char>>);
    STATIC_EXPECT_FALSE(hashable_range<char[4]>);
    STATIC_EXPECT_FALSE(hashable_range<::std::u16string_view>);
    STATIC_EXPECT_FALSE(hashable_range<int>);
}

/**
 * @test A hash function written outside the group refuses an array once it states the
 *       concept, and takes one without it: the refusal stands in each signature rather
 *       than in a mechanism the group shares.
 */
TEST(HashElementTest, AnOwnHashFunctionTakesTheRefusalFromTheConcept)
{
    STATIC_EXPECT_TRUE(rolling_takes<::std::string_view>);
    STATIC_EXPECT_FALSE(rolling_takes<char[4]>);

    STATIC_EXPECT_TRUE(unconstrained_takes<::std::string_view>);
    STATIC_EXPECT_TRUE(unconstrained_takes<char[4]>);

    STATIC_EXPECT_EQ(rolling(::std::string_view{"abc"}), unconstrained(::std::string_view{"abc"}));
}

/**
 * @test An array is refused, whatever its element type: nothing about a bound tells the
 *       storage from the content.
 */
TEST(HashByteViewTest, ArrayIsRefused)
{
    STATIC_EXPECT_FALSE(taken<char[4]>);
    STATIC_EXPECT_FALSE(taken<char8_t[4]>);
    STATIC_EXPECT_FALSE(taken<char16_t[4]>);
    STATIC_EXPECT_FALSE(taken<char32_t[4]>);
    STATIC_EXPECT_FALSE(taken<::std::uint32_t[4]>);
    STATIC_EXPECT_FALSE(taken<::std::byte[4]>);
    STATIC_EXPECT_FALSE(taken<unsigned char[4]>);
    STATIC_EXPECT_FALSE(taken<signed char[4]>);
    STATIC_EXPECT_FALSE(taken<::std::uint16_t[4]>);
    STATIC_EXPECT_FALSE(taken<wchar_t[4]>);
    STATIC_EXPECT_FALSE(taken<::std::uint8_t[4]>);
}

/**
 * @test The refusal stops at the array itself: a view over one is taken, and answers for
 *       the whole bound.
 */
TEST(HashByteViewTest, AViewOverAnArrayIsTakenAndAnswersForItsWholeBound)
{
    static constexpr char raw[4]{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE(taken<decltype(::std::views::all(raw))>);
    STATIC_EXPECT_EQ(fnv1a(::std::views::all(raw)), fnv1a(::std::string_view{"abc", 4}));
}

/**
 * @test A partially filled std::array is taken whole, which is where the rule stops: its
 *       bound is four, and four bytes are hashed.
 */
TEST(HashByteViewTest, APartiallyFilledArrayObjectIsTakenWhole)
{
    static constexpr ::std::array<char, 4> held{'a', 'b', 'c'};
    STATIC_EXPECT_TRUE((taken<::std::array<char, 4>>));
    STATIC_EXPECT_EQ(fnv1a(held), fnv1a(::std::string_view{"abc", 4}));
}

/**
 * @test An element one byte past the rule is refused, whatever spells it.
 */
TEST(HashElementTest, AnElementJustPastTheByteRuleIsRefused)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<one_byte>>);
    STATIC_EXPECT_FALSE(hashable_range<::std::vector<two_bytes>>);
}

/**
 * @test The view carries the guarantee of its source: hashing it throws nothing where
 *       walking the source throws nothing, and stops promising where the source does.
 */
TEST(HashByteViewTest, NoexceptFollowsTheSource)
{
    ::std::u16string_view const wide{u"start"};
    throwing_range<throwing_operation::none> const safe;
    throwing_range<throwing_operation::dereference> risky;

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::fnv1a(::scl::hash::byte_view(wide))));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::fnv1a(::scl::hash::byte_view(safe))));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::fnv1a(::scl::hash::byte_view(risky))));
}

/**
 * @test A view built over a temporary owns it, so the source outlives the expression that
 *       made the view.
 */
TEST(HashByteViewTest, AViewOfATemporaryKeepsTheSourceAlive)
{
    auto const kept = ::std::make_shared<::std::array<char16_t, 3>>(::std::array<char16_t, 3>{
        u'A', u'B', u'C'});

    auto const bytes = ::scl::hash::byte_view(counted_buffer{kept});

    EXPECT_EQ(kept.use_count(), 2);
    EXPECT_EQ(::scl::hash::fnv1a(bytes),
        ::scl::hash::fnv1a(::scl::hash::byte_view(::std::u16string_view{u"ABC"})));
}

/**
 * @test A source the view owns is walked by the constant cursor, so a container whose copies
 *       share one buffer is not read through a non-constant reference.
 */
TEST(HashByteViewTest, AnOwnedSourceIsWalkedByTheConstantCursor)
{
    auto owned = ::scl::hash::byte_view(shared_buffer{});
    shared_buffer::mutable_reads = 0;

    for (auto position = ::std::ranges::begin(owned); !(position == ::std::ranges::end(owned)); ++position)
        static_cast<void>(*position);

    EXPECT_EQ(shared_buffer::mutable_reads, 0);
}

/**
 * @test The cursor a source readable only through a non-constant reference answers follows
 *       that source, as the constant one does.
 */
TEST(HashByteViewTest, TheMutableCursorFollowsTheSource)
{
    mutable_throwing_range<throwing_operation::none> safe;
    mutable_throwing_range<throwing_operation::dereference> risky;
    auto safe_bytes = ::scl::hash::byte_view(safe);
    auto risky_bytes = ::scl::hash::byte_view(risky);

    STATIC_EXPECT_TRUE(noexcept(::std::ranges::begin(safe_bytes)));
    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(risky_bytes)));
}

/**
 * @test The cursor promises nothing where moving the source iterator can throw.
 */
TEST(HashByteViewTest, TheCursorFollowsAnIteratorThatCanThrowWhenMoved)
{
    move_throwing_iterator_range source;
    auto bytes = ::scl::hash::byte_view(source);

    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(bytes)));
}

/**
 * @test The cursor promises nothing where moving the sentinel can throw.
 */
TEST(HashByteViewTest, TheCursorFollowsAnEndThatCanThrowWhenMoved)
{
    move_throwing_sentinel_range source;
    auto bytes = ::scl::hash::byte_view(source);

    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(bytes)));
}

/**
 * @test The cursor reads an element through the element type, so a reference whose conversion
 *       throws stops the promise.
 */
TEST(HashByteViewTest, TheCursorFollowsTheConversionToTheElementType)
{
    throwing_range<throwing_operation::conversion> sneaky;
    auto bytes = ::scl::hash::byte_view(sneaky);

    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(bytes)));
}

/**
 * @test The cursor weighs the iterator against the sentinel, which a source whose two
 *       differ tells apart from weighing the iterator against itself.
 */
TEST(HashByteViewTest, TheCursorComparesTheIteratorAgainstTheEnd)
{
    distinct_sentinel_range source;
    auto bytes = ::scl::hash::byte_view(source);

    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(bytes)));
}

/**
 * @test Postfix increment copies the cursor, so a copy that can throw stops its promise while
 *       the prefix one keeps it.
 */
TEST(HashByteViewTest, ThePostfixIncrementFollowsTheCopyOfTheCursor)
{
    copy_throwing_range source;
    auto bytes = ::scl::hash::byte_view(source);
    auto position = ::std::ranges::begin(bytes);

    STATIC_EXPECT_TRUE(noexcept(++position));
    STATIC_EXPECT_FALSE(noexcept(position++));
}

/**
 * @test A byte-sized element with no unsigned counterpart passes through unchanged.
 */
TEST(HashByteViewTest, AByteSizedElementWithNoUnsignedCounterpartPassesThrough)
{
    ::std::vector<bool> const flags{true, false, true};

    EXPECT_EQ(::scl::hash::fnv1a(::scl::hash::byte_view(flags)), ::scl::hash::fnv1a(flags));
}

/**
 * @test A source of one element spells exactly the bytes of that element.
 */
TEST(HashByteViewTest, ASingleElementSourceSpellsItsBytes)
{
    static constexpr ::std::uint8_t expected[]{0x41U, 0x00U};

    STATIC_EXPECT_TRUE(::std::ranges::equal(::scl::hash::byte_view(::std::u16string_view{u"A"}),
        ::std::span{expected}));
}

/**
 * @test The cursor promises nothing its source does not promise, operation by operation.
 */
TEST(HashByteViewTest, TheCursorPromisesNothingTheSourceDoesNotPromise)
{
    throwing_range<throwing_operation::compare> risky;
    throwing_range<throwing_operation::none> safe;
    auto risky_bytes = ::scl::hash::byte_view(risky);
    auto safe_bytes = ::scl::hash::byte_view(safe);
    auto slow = ::std::ranges::begin(risky_bytes);
    auto quick = ::std::ranges::begin(safe_bytes);

    STATIC_EXPECT_TRUE(noexcept(::std::ranges::begin(safe_bytes)));
    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(risky_bytes)));
    STATIC_EXPECT_TRUE(noexcept(++quick));
    STATIC_EXPECT_FALSE(noexcept(++slow));
    STATIC_EXPECT_TRUE(noexcept(quick++));
    STATIC_EXPECT_FALSE(noexcept(slow++));
    STATIC_EXPECT_TRUE(noexcept(quick == ::std::ranges::end(safe_bytes)));
    STATIC_EXPECT_FALSE(noexcept(slow == ::std::ranges::end(risky_bytes)));
    STATIC_EXPECT_TRUE(noexcept(quick == quick));
    STATIC_EXPECT_FALSE(noexcept(slow == slow));
    STATIC_EXPECT_TRUE(noexcept(*quick));
}

/**
 * @test The cursor a const view answers follows the source as the mutable one does.
 */
TEST(HashByteViewTest, TheConstCursorFollowsTheSource)
{
    throwing_range<throwing_operation::compare> const risky;
    ::std::u16string_view const wide{u"AB"};
    auto const bytes = ::scl::hash::byte_view(risky);
    auto const safe = ::scl::hash::byte_view(wide);

    STATIC_EXPECT_TRUE(noexcept(::std::ranges::begin(safe)));
    STATIC_EXPECT_FALSE(noexcept(::std::ranges::begin(bytes)));
}

/**
 * @test The postfix increment of a forward cursor follows the source, and answers a copy.
 */
TEST(HashByteViewTest, TheForwardCursorPostfixFollowsTheSource)
{
    forward_throwing_range<throwing_operation::increment> risky;
    forward_throwing_range<throwing_operation::none> safe;
    auto risky_bytes = ::scl::hash::byte_view(risky);
    auto safe_bytes = ::scl::hash::byte_view(safe);
    auto slow = ::std::ranges::begin(risky_bytes);
    auto quick = ::std::ranges::begin(safe_bytes);

    STATIC_EXPECT_TRUE((::std::is_same_v<decltype(quick++), decltype(quick)>));
    STATIC_EXPECT_TRUE(noexcept(quick++));
    STATIC_EXPECT_FALSE(noexcept(slow++));
}

/**
 * @test Taking a source that can throw when it is moved is not itself noexcept.
 */
TEST(HashByteViewTest, TakingASourceThatCanThrowWhenMovedIsNotNoexcept)
{
    move_throwing_range source;
    ::std::u16string_view const text{u"AB"};

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::byte_view(text)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::byte_view(source)));
}

/**
 * @test The reported size throws exactly where the source reports its own size throwing.
 */
TEST(HashByteViewTest, TheSizeThrowsExactlyWhereTheSourceSizeThrows)
{
    sized_throwing_range<throwing_operation::none> safe;
    sized_throwing_range<throwing_operation::size> risky;
    mutable_sized_throwing_range<throwing_operation::none> safe_while_mutable;
    mutable_sized_throwing_range<throwing_operation::size> risky_while_mutable;

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::byte_view(safe).size()));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::byte_view(risky).size()));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::byte_view(safe_while_mutable).size()));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::byte_view(risky_while_mutable).size()));
}

/**
 * @test An empty source spells no bytes, and the cursor reads no element to find that out.
 */
TEST(HashByteViewTest, AnEmptySourceSpellsNoBytes)
{
    STATIC_EXPECT_TRUE(::scl::hash::byte_view(::std::u16string_view{}).empty());
    STATIC_EXPECT_EQ(::scl::hash::fnv1a(::scl::hash::byte_view(::std::u16string_view{})),
        ::scl::hash::fnv1a(::std::string_view{}));
}

/**
 * @test A view is iterable through a const reference exactly where its source is.
 */
TEST(HashByteViewTest, AConstViewIsIterableExactlyWhereItsSourceIs)
{
    ::std::u16string_view const wide{u"AxB"};
    auto kept = wide | ::std::views::filter([](char16_t const c) { return c != u'x'; });

    STATIC_EXPECT_FALSE(::std::ranges::range<decltype(::scl::hash::byte_view(kept)) const>);
    STATIC_EXPECT_TRUE(::std::ranges::range<decltype(::scl::hash::byte_view(wide)) const>);
}

/**
 * @test Every hasher takes and refuses exactly what the free function it wraps does.
 */
TEST(HashElementTest, EveryHasherRefusesAnArray)
{
    STATIC_EXPECT_TRUE((hasher_takes<fnv1a_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<djb2_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<sdbm_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<jenkins_ota_hasher, ::std::string_view>));
    STATIC_EXPECT_TRUE((hasher_takes<siphash_hasher<>, ::std::string_view>));

    STATIC_EXPECT_FALSE((hasher_takes<fnv1a_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<djb2_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<sdbm_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<jenkins_ota_hasher, char[4]>));
    STATIC_EXPECT_FALSE((hasher_takes<siphash_hasher<>, char[4]>));

    STATIC_EXPECT_FALSE((hasher_takes<fnv1a_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<djb2_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<sdbm_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<jenkins_ota_hasher, ::std::u16string_view>));
    STATIC_EXPECT_FALSE((hasher_takes<siphash_hasher<>, ::std::u16string_view>));
}

/**
 * @test Every range the element rule admits is taken unless it is a bounded array, a
 *       fixed-extent std::array and std::span included.
 */
TEST(HashByteViewTest, EveryRangeButTheBoundedArrayIsTaken)
{
    STATIC_EXPECT_TRUE(hashable_range<::std::string_view>);
    STATIC_EXPECT_TRUE((hashable_range<::std::array<char, 4>>));
    STATIC_EXPECT_TRUE((hashable_range<::std::span<char const, 4>>));
    STATIC_EXPECT_TRUE(hashable_range<::std::vector<char>>);
    STATIC_EXPECT_TRUE(spellable<::std::u16string_view>);
    STATIC_EXPECT_TRUE((spellable<::std::array<char16_t, 4>>));
    STATIC_EXPECT_TRUE((spellable<::std::span<char16_t const, 4>>));
}

/**
 * @test A range whose end of traversal is of its own type is hashed by every function of
 *       the group, which a fold over two iterators of one type could not take.
 */
TEST(HashElementTest, ARangeWhoseSentinelIsOfItsOwnTypeIsHashed)
{
    ::std::string_view const text{"abcx"};
    auto taken = ::std::views::take_while(text, [](char const c) { return c != 'x'; });

    STATIC_EXPECT_TRUE(hashable_range<decltype(taken)>);
    EXPECT_EQ(::scl::hash::djb2(taken), ::scl::hash::djb2(::std::string_view{"abc"}));
    EXPECT_EQ(::scl::hash::sdbm(taken), ::scl::hash::sdbm(::std::string_view{"abc"}));
    EXPECT_EQ(::scl::hash::fnv1a(taken), ::scl::hash::fnv1a(::std::string_view{"abc"}));
}

/**
 * @test A range that cannot be traversed through a reference to a constant reaches every
 *       hash function of the group.
 */
TEST(HashElementTest, ARangeReadableOnlyWhileMutableIsHashed)
{
    ::std::string_view const text{"abc"};
    int counter = 0;
    auto counted = text | ::std::views::transform([counter](char const c) mutable {
        ++counter;
        return c;
    });

    STATIC_EXPECT_FALSE(::std::ranges::range<decltype(counted) const>);
    STATIC_EXPECT_TRUE(hashable_range<decltype(counted)>);
    EXPECT_EQ(::scl::hash::fnv1a(counted), ::scl::hash::fnv1a(text));
}

/**
 * @test A range whose reference is a proxy is read through its element type.
 */
TEST(HashElementTest, AProxyReferenceIsReadThroughTheElementType)
{
    ::std::vector<bool> flags{true, false, true};
    ::std::vector<bool> const frozen{true, false, true};
    ::std::array<bool, 3> const held{true, false, true};

    STATIC_EXPECT_TRUE(hashable_range<::std::vector<bool>>);
    EXPECT_EQ(::scl::hash::fnv1a(flags), ::scl::hash::fnv1a(held));
    EXPECT_EQ(::scl::hash::fnv1a(frozen), ::scl::hash::fnv1a(held));
}

/**
 * @test A range that can throw while iterated is reported as such, and one that cannot is
 *       still nothrow at every entry point.
 */
TEST(HashElementTest, NoexceptFollowsTheRange)
{
    ::std::string_view safe{"abc"};
    throwing_range<throwing_operation::dereference> risky;
    throwing_range<throwing_operation::conversion> sneaky;

    STATIC_EXPECT_FALSE(noexcept(::scl::hash::fnv1a(sneaky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::fnv1a(safe)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::fnv1a_hasher{}(safe)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::key<>{safe}));

    STATIC_EXPECT_FALSE(noexcept(::scl::hash::fnv1a(risky)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::fnv1a_hasher{}(risky)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::key<>{risky}));
}

/**
 * @test Each operation the concept names answers on its own: a range that throws at
 *       exactly one of them is refused, and the same range throwing at none is taken.
 */
TEST(HashElementTest, EveryOperationTheConceptNamesIsAnswered)
{
    using ::scl::hash::detail::nothrow_iterable;

    STATIC_EXPECT_TRUE(nothrow_iterable<throwing_range<throwing_operation::none> &>);
    STATIC_EXPECT_FALSE(nothrow_iterable<throwing_range<throwing_operation::begin> &>);
    STATIC_EXPECT_FALSE(nothrow_iterable<throwing_range<throwing_operation::end> &>);
    STATIC_EXPECT_FALSE(nothrow_iterable<throwing_range<throwing_operation::compare> &>);
    STATIC_EXPECT_FALSE(nothrow_iterable<throwing_range<throwing_operation::increment> &>);
    STATIC_EXPECT_FALSE(nothrow_iterable<throwing_range<throwing_operation::dereference> &>);
    STATIC_EXPECT_FALSE(nothrow_iterable<throwing_range<throwing_operation::conversion> &>);
}

/**
 * @test Every free function of the group throws where walking the range it is given throws.
 */
TEST(HashElementTest, EveryFreeFunctionFollowsTheRange)
{
    ::std::string_view safe{"abc"};
    throwing_range<throwing_operation::dereference> risky;

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::djb2(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::djb2(risky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::sdbm(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::sdbm(risky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::jenkins_ota(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::jenkins_ota(risky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::siphash(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::siphash(risky)));
}

/**
 * @test Every hasher throws exactly where the free function it wraps throws.
 */
TEST(HashElementTest, EveryHasherFollowsTheRange)
{
    ::std::string_view safe{"abc"};
    throwing_range<throwing_operation::dereference> risky;

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::djb2_hasher{}(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::djb2_hasher{}(risky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::sdbm_hasher{}(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::sdbm_hasher{}(risky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::jenkins_ota_hasher{}(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::jenkins_ota_hasher{}(risky)));
    STATIC_EXPECT_TRUE(noexcept(::scl::hash::siphash_hasher<>{}(safe)));
    STATIC_EXPECT_FALSE(noexcept(::scl::hash::siphash_hasher<>{}(risky)));
}

/**
 * @test The concept weighs the iterator against the sentinel, not against itself.
 */
TEST(HashElementTest, TheConceptComparesTheIteratorAgainstTheEnd)
{
    STATIC_EXPECT_FALSE(::scl::hash::detail::nothrow_iterable<distinct_sentinel_range &>);
}

/**
 * @test A container whose copies share one buffer is read through a reference to a constant,
 *       whatever names it, so reading it never separates that buffer.
 */
TEST(HashElementTest, ASharedBufferIsReadThroughAConstantReference)
{
    shared_buffer::mutable_reads = 0;
    shared_buffer named;

    static_cast<void>(::scl::hash::fnv1a(named));
    static_cast<void>(::scl::hash::fnv1a(shared_buffer{}));
    static_cast<void>(::scl::hash::fnv1a(::scl::hash::byte_view(named)));
    static_cast<void>(::scl::hash::fnv1a(::scl::hash::byte_view(shared_buffer{})));

    EXPECT_EQ(shared_buffer::mutable_reads, 0);
}

/**
 * @test Every hash function of the group reads a container whose copies share one buffer
 *       through a reference to a constant, not only the one the other test walks.
 */
TEST(HashElementTest, EveryFreeFunctionReadsASharedBufferThroughAConstantReference)
{
    shared_buffer named;
    shared_buffer::mutable_reads = 0;

    static_cast<void>(::scl::hash::djb2(named));
    static_cast<void>(::scl::hash::sdbm(named));
    static_cast<void>(::scl::hash::jenkins_ota(named));
    static_cast<void>(::scl::hash::siphash(named));
    static_cast<void>(::scl::hash::key<>{named});

    EXPECT_EQ(shared_buffer::mutable_reads, 0);
}

/**
 * @test A range answering a wider element to a constant traversal is traversed through the
 *       mutable one, which is the element the constraint weighed.
 */
TEST(HashElementTest, AWiderElementUnderAConstantTraversalIsLeftToTheMutableOne)
{
    widening_range source;

    STATIC_EXPECT_FALSE(::scl::hash::detail::same_element_as_const<widening_range>);
    STATIC_EXPECT_TRUE(::std::ranges::range<widening_range const>);
    EXPECT_EQ(::scl::hash::fnv1a(source), ::scl::hash::fnv1a(::std::string_view{"abc"}));
}

/**
 * @test A hash function reports the guarantee of the traversal it performs, which for a source
 *       promising nothing while mutable is the one its constant operations carry.
 */
TEST(HashElementTest, ASharedBufferIsWeighedByItsConstantOperations)
{
    shared_buffer named;

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::fnv1a(named)));
}

/**
 * @test A source readable only through a non-constant reference is walked through that
 *       reference, and keeps the guarantee its own operations carry.
 */
TEST(HashElementTest, ASourceReadableOnlyWhileMutableKeepsItsGuarantee)
{
    mutable_throwing_range<throwing_operation::none> safe;

    STATIC_EXPECT_TRUE(noexcept(::scl::hash::fnv1a(safe)));
}
