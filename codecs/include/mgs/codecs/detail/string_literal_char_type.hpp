#pragma once

#include <type_traits>

#include <mgs/config.hpp>

namespace mgs
{
namespace detail
{
template <typename CharT>
struct is_string_literal_char_type : std::false_type
{
};

template <>
struct is_string_literal_char_type<char> : std::true_type
{
};

template <>
struct is_string_literal_char_type<wchar_t> : std::true_type
{
};

template <>
struct is_string_literal_char_type<char16_t> : std::true_type
{
};

template <>
struct is_string_literal_char_type<char32_t> : std::true_type
{
};

#ifdef MGS_HAS_CPP20
template <>
struct is_string_literal_char_type<char8_t> : std::true_type
{
};
#endif
}
}
