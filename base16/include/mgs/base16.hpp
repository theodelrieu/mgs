#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base16/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_codec.hpp>

namespace mgs
{
using base16 = base_n::basic_codec<detail::base16_encoding_traits<>>;
}
