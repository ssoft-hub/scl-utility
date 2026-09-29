// One externally visible wrapper per public function, so that -ffunction-sections puts each in a
// section of its own for arm-none-eabi-size.

#include <scl/utility/runtime/enum.h>
#include <scl/utility/runtime/type.h>

#include <array>
#include <charconv>
#include <string>

namespace scl::benchmarks
{
    enum class color : int
    {
        blue = -3,
    };

    struct hex_format
    {
        template <typename Number>
        [[nodiscard]]
        ::std::string operator()(Number number) const
        {
            ::std::array<char, 32> buf{};
            auto const end = ::std::to_chars(buf.data(), buf.data() + buf.size(), number, 16).ptr;
            return ::std::string{buf.data(), end};
        }
    };

    ::std::string enum_string_of_color(color value) { return ::scl::enum_string(value); }

    ::std::string enum_string_formatted(color value)
    {
        return ::scl::enum_string(value, hex_format{});
    }

#if SCL_HAS_RTTI
    struct base
    {
        virtual ~base() = default;
    };

    ::std::string type_name_of(base const & object) { return ::scl::type_name(object); }

    ::std::string type_short_name_of(base const & object) { return ::scl::type_short_name(object); }
#endif

} // namespace scl::benchmarks
