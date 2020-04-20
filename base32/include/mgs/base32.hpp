#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include <mgs/base32/detail/encoding_traits.hpp>
#include <mgs/base_n/basic_codec.hpp>

namespace mgs
{
using base32 = base_n::basic_codec<detail::base32_encoding_traits<>>;
}
